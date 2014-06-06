// Calc-H.cpp
// (Japanese, Shift_JIS)
// Linux�̏ꍇ�́AUTF-8�ɕϊ����ĉ������B

#include "Calc-H.h"
#include "HParseHeader.h"

namespace Calc_H
{
    CH_Value s_sore = 0;
    std::string s_message;

    static const char *s_jdigits[] =
    {
        "����", "����", "��", "����", "���",
        "��", "�낭", "�Ȃ�", "�͂�", "���イ"
    };
}

using namespace Calc_H;

CH_Value ChCalcExpr(const shared_ptr<Expr>& expr);
CH_Value ChCalcMono(const shared_ptr<Mono>& mono);
CH_Value ChCalcFact(const shared_ptr<Fact>& fact);
CH_Value ChCalcShite(const shared_ptr<Shite>& shite);

////////////////////////////////////////////////////////////////////////////

CH_Value ChCalcNum(const shared_ptr<Num>& num)
{
    return num->m_value;
}

CH_Value ChCalcPrim(const shared_ptr<Prim>& prim)
{
    switch (prim->m_type)
    {
    case Prim::MONO:
        return ChCalcMono(prim->m_mono);

    case Prim::MINUS:
        return -ChCalcPrim(prim->m_prim);

    case Prim::NUM:
        return ChCalcNum(prim->m_num);

    case Prim::BUNSUU:
        {
            CH_Value num = ChCalcNum(prim->m_num);
            CH_Value denom = ChCalcPrim(prim->m_prim);
            if (num.is_zero())
                return 0;
            if (denom.is_zero())
            {
                Calc_H::s_message = "���ꂪ�[���ɂȂ����̂Ōv�Z�ł��܂���B";
                return 0;
            }
            if (num.is_i() && denom.is_i())
                return CH_Value(num, denom);
            else
                return denom / num;
        }

	case Prim::TAIBUNSUU:
        {
            CH_Value seisuu = ChCalcNum(prim->m_num);
            CH_Value denom = ChCalcPrim(prim->m_prim);
            CH_Value num = ChCalcNum(prim->m_num2);
            if (num.is_zero())
                return seisuu;
            if (denom.is_zero())
            {
                Calc_H::s_message = "���ꂪ�[���ɂȂ����̂Ōv�Z�ł��܂���B";
                return 0;
            }
            if (num.is_i() && denom.is_i())
                return seisuu + CH_Value(num, denom);
            else
                return seisuu + denom / num;
        }

    default:
        assert(0);
        return 0;
    }
}

CH_Value 
ChPowFactPrim(const shared_ptr<Fact>& fact, const shared_ptr<Prim>& prim)
{
    CH_Value value = 1;
    CH_Value count = ChCalcFact(fact);
    for (int i = 0; i < count; ++i)
    {
        value *= ChCalcPrim(prim);
    }
    return value;
}

CH_Value ChCalcFact(const shared_ptr<Fact>& fact)
{
    switch (fact->m_type)
    {
    case Fact::POW:
        return ChPowFactPrim(fact->m_fact, fact->m_prim);

    case Fact::POW2:
        return ChCalcFact(fact->m_fact) * ChCalcFact(fact->m_fact);

    case Fact::SINGLE:
        return ChCalcPrim(fact->m_prim);

    default:
        assert(0);
        return 0;
    }
}

CH_Value ChCalcTerm(const shared_ptr<Term>& term)
{
    switch (term->m_type)
    {
    case Term::MUL:
        return ChCalcTerm(term->m_term) * ChCalcFact(term->m_fact);

    case Term::DIV:
        return ChCalcTerm(term->m_term) / ChCalcFact(term->m_fact);

    case Term::FACT_ONLY:
        return ChCalcFact(term->m_fact);

    default:
        assert(0);
        return 0;
    }
}

CH_Value ChCalcExpr(const shared_ptr<Expr>& expr)
{
    switch (expr->m_type)
    {
    case Expr::ADD:
        return ChCalcExpr(expr->m_expr) + ChCalcTerm(expr->m_term);

    case Expr::SUB:
        return ChCalcExpr(expr->m_expr) - ChCalcTerm(expr->m_term);

    case Expr::TERM_ONLY:
        return ChCalcTerm(expr->m_term);

    default:
        assert(0);
        return 0;
    }
}

CH_Value ChAddExprList(const shared_ptr<ExprList>& exprlist)
{
    CH_Value value = 0;
    assert(exprlist.get());
    ExprList *el = exprlist.get();
    ExprList::iterator it = el->begin();
    ExprList::iterator end = el->end();
    for (; it != end; ++it)
    {
        value += ChCalcExpr(*it);
    }
    return value;
}

CH_Value ChMulExprList(const shared_ptr<ExprList>& exprlist)
{
    CH_Value value = 1;
    assert(exprlist.get());
    ExprList *el = exprlist.get();
    ExprList::iterator it = el->begin();
    ExprList::iterator end = el->end();
    for (; it != end; ++it)
    {
        value *= ChCalcExpr(*it);
    }
    return value;
}

CH_Value ChSubExprList(const shared_ptr<ExprList>& exprlist)
{
    assert(exprlist.get());
    ExprList *el = exprlist.get();
    if (el->size() >= 3)
        Calc_H::s_message = "�O�ȏ�̑Ώۂ����鍷�͋��߂��܂���B";
    ExprList::iterator it = el->begin();
    ExprList::iterator end = el->end();
    CH_Value value = ChCalcExpr(*it);
    for (++it; it != end; ++it)
    {
        value -= ChCalcExpr(*it);
    }
    return value;
}

CH_Value ChDivExprList(const shared_ptr<ExprList>& exprlist)
{
    assert(exprlist.get());
    ExprList *el = exprlist.get();
    if (el->size() >= 3)
        Calc_H::s_message = "�O�ȏ�̑Ώۂ����鏤�͋��߂��܂���B";
    ExprList::iterator it = el->begin();
    ExprList::iterator end = el->end();
    CH_Value value = ChCalcExpr(*it);
    for (++it; it != end; ++it)
    {
        value /= ChCalcExpr(*it);
    }
    return value;
}

CH_Value ChCalcMono(const shared_ptr<Mono>& mono)
{
    CH_Value v1, v2;
    switch (mono->m_type)
    {
    case Mono::EXPRLIST_ADD:
        return ChAddExprList(mono->m_exprlist);

    case Mono::EXPRLIST_MUL:
        return ChMulExprList(mono->m_exprlist);

    case Mono::EXPRLIST_SUB:
        return ChSubExprList(mono->m_exprlist);

    case Mono::EXPRLIST_DIV:
        return ChDivExprList(mono->m_exprlist);

    case Mono::MONO_ADD:
        return ChCalcMono(mono->m_mono) + ChCalcExpr(mono->m_expr);

    case Mono::MONO_MUL:
        return ChCalcMono(mono->m_mono) * ChCalcExpr(mono->m_expr);

    case Mono::MONO_SUB:
        return ChCalcMono(mono->m_mono) - ChCalcExpr(mono->m_expr);

    case Mono::MONO_DIV:
        return ChCalcMono(mono->m_mono) / ChCalcExpr(mono->m_expr);

    case Mono::MONO_ONLY:
        return ChCalcMono(mono->m_mono);

    case Mono::MONO_EXPR_BAI:
        return ChCalcMono(mono->m_mono) * ChCalcExpr(mono->m_expr);

    case Mono::MONO_BAI:
        return ChCalcMono(mono->m_mono) * 2;

    case Mono::SHITE_ADD:
        return ChCalcShite(mono->m_shite) + ChCalcExpr(mono->m_expr);

    case Mono::SHITE_MUL:
        return ChCalcShite(mono->m_shite) * ChCalcExpr(mono->m_expr);

    case Mono::SHITE_SUB:
        return ChCalcShite(mono->m_shite) - ChCalcExpr(mono->m_expr);

    case Mono::SHITE_DIV:
        return ChCalcShite(mono->m_shite) / ChCalcExpr(mono->m_expr);

    case Mono::SHITE_EXPR_BAI:
        return ChCalcShite(mono->m_shite) * ChCalcExpr(mono->m_expr);

    case Mono::SHITE_BAI:
        return ChCalcShite(mono->m_shite) * 2;

    case Mono::EXPR_ONLY:
        return ChCalcExpr(mono->m_expr);

    case Mono::SORE:
        return s_sore;

    case Mono::MONO_TO_EXPRLIST_ADD:
        return ChCalcMono(mono->m_mono) + ChAddExprList(mono->m_exprlist);

    case Mono::MONO_TO_EXPRLIST_MUL:
        return ChCalcMono(mono->m_mono) * ChMulExprList(mono->m_exprlist);
        
    case Mono::MONO_TO_EXPR_ADD:
        return ChCalcMono(mono->m_mono) + ChCalcExpr(mono->m_expr);

    case Mono::MONO_TO_EXPR_MUL:
        return ChCalcMono(mono->m_mono) * ChCalcExpr(mono->m_expr);
        
    case Mono::MONO_TO_EXPR_SUB:
        return ChCalcMono(mono->m_mono) - ChCalcExpr(mono->m_expr);

    case Mono::MONO_TO_EXPR_DIV:
        return ChCalcMono(mono->m_mono) / ChCalcExpr(mono->m_expr);

    case Mono::MONO_WO_EXPR_KARA_SUB:
        return ChCalcExpr(mono->m_expr) - ChCalcMono(mono->m_mono);

    case Mono::MONO_DE_EXPR_WO_DIV:
        return ChCalcExpr(mono->m_expr) / ChCalcMono(mono->m_mono);

    default:
        assert(0);
        return 0;
    }
}

CH_Value ChCalcShite(const shared_ptr<Shite>& shite)
{
    switch (shite->m_type)
    {
    case Shite::EXPRLIST_ADD:
        return ChAddExprList(shite->m_exprlist);

    case Shite::EXPRLIST_MUL:
        return ChMulExprList(shite->m_exprlist);

    case Shite::MONO_ADD:
        return ChCalcMono(shite->m_mono) + ChCalcExpr(shite->m_expr);

    case Shite::MONO_MUL:
        return ChCalcMono(shite->m_mono) * ChCalcExpr(shite->m_expr);

    case Shite::MONO_SUB:
        return ChCalcMono(shite->m_mono) - ChCalcExpr(shite->m_expr);

    case Shite::MONO_DIV:
        return ChCalcMono(shite->m_mono) / ChCalcExpr(shite->m_expr);

    case Shite::MONO_ONLY:
        return ChCalcMono(shite->m_mono);

    case Shite::SHITE_ADD:
        return ChCalcShite(shite->m_shite) + ChCalcExpr(shite->m_expr);

    case Shite::SHITE_MUL:
        return ChCalcShite(shite->m_shite) * ChCalcExpr(shite->m_expr);

    case Shite::SHITE_SUB:
        return ChCalcShite(shite->m_shite) - ChCalcExpr(shite->m_expr);

    case Shite::SHITE_DIV:
        return ChCalcShite(shite->m_shite) / ChCalcExpr(shite->m_expr);

    case Shite::SHITE_EXPR_BAI:
        return ChCalcShite(shite->m_shite) * ChCalcExpr(shite->m_expr);

    case Shite::SHITE_BAI:
        return ChCalcShite(shite->m_shite) * 2;

    case Shite::MONO_WO_EXPR_SUB:
        return ChCalcMono(shite->m_mono) - ChCalcExpr(shite->m_expr);

    case Shite::MONO_WO_EXPR_DIV:
        return ChCalcMono(shite->m_mono) / ChCalcExpr(shite->m_expr);

    default:
        assert(0);
        return 0;
    }
}

CH_Value ChCalcSuruto(const shared_ptr<Suruto>& suruto)
{
    switch (suruto->m_type)
    {
    case Suruto::EXPRLIST_ADD:
        return ChAddExprList(suruto->m_exprlist);

    case Suruto::EXPRLIST_MUL:
        return ChMulExprList(suruto->m_exprlist);

    case Suruto::MONO_ADD:
        return ChCalcMono(suruto->m_mono) + ChCalcExpr(suruto->m_expr);

    case Suruto::MONO_MUL:
        return ChCalcMono(suruto->m_mono) * ChCalcExpr(suruto->m_expr);

    case Suruto::MONO_SUB:
        return ChCalcMono(suruto->m_mono) - ChCalcExpr(suruto->m_expr);

    case Suruto::MONO_DIV:
        return ChCalcMono(suruto->m_mono) / ChCalcExpr(suruto->m_expr);

    case Suruto::MONO_ONLY:
        return ChCalcMono(suruto->m_mono);

    case Suruto::MONO_WO_EXPR_BAI:
        return ChCalcMono(suruto->m_mono) * ChCalcExpr(suruto->m_expr);

    case Suruto::MONO_WO_BAI:
        return ChCalcMono(suruto->m_mono) * 2;

    case Suruto::SHITE_TASUTO:
        return ChCalcShite(suruto->m_shite) + ChCalcExpr(suruto->m_expr);

    case Suruto::SHITE_KAKERUTO:
        return ChCalcShite(suruto->m_shite) * ChCalcExpr(suruto->m_expr);

    case Suruto::SHITE_HIKUTO:
        return ChCalcShite(suruto->m_shite) - ChCalcExpr(suruto->m_expr);

    case Suruto::SHITE_WARUTO:
        return ChCalcShite(suruto->m_shite) / ChCalcExpr(suruto->m_expr);

    case Suruto::MONO_WO_EXPR_SUB:
        return ChCalcMono(suruto->m_mono) - ChCalcExpr(suruto->m_expr);

    case Suruto::MONO_DE_EXPR_DIV:
        return ChCalcMono(suruto->m_mono) / ChCalcExpr(suruto->m_expr);

    default:
        assert(0);
        return 0;
    }
}

CH_Value ChCalcSentence(const shared_ptr<Sentence>& sentence)
{
    switch (sentence->m_type)
    {
    case Sentence::MONO:
        return ChCalcMono(sentence->m_mono);

    case Sentence::EXPR:
        return ChCalcExpr(sentence->m_expr);

    case Sentence::SURUTO:
        return ChCalcSuruto(sentence->m_suruto);

    case Sentence::NUM:
        return ChCalcNum(sentence->m_num);

    case Sentence::EMPTY:
        return s_sore;

    case Sentence::EXPRLIST_ADD:
        return ChAddExprList(sentence->m_exprlist);

    case Sentence::EXPRLIST_MUL:
        return ChMulExprList(sentence->m_exprlist);

    case Sentence::EXPRLIST_SUB:
        return ChSubExprList(sentence->m_exprlist);

    case Sentence::SHITE:
        return ChCalcShite(sentence->m_shite);

    default:
        assert(0);
        return 0;
    }
}

////////////////////////////////////////////////////////////////////////////

void ChReplaceString(std::string& str,
                     const std::string& from,
                     const std::string& to)
{
    std::size_t i = 0;
    for (;;)
    {
        i = str.find(from, i);
        if (i == std::string::npos)
            break;

        str.replace(i, from.size(), to);
        i += to.size();
    }
}

////////////////////////////////////////////////////////////////////////////

std::string ChGetJpnDigits(const std::string& str, bool& ok)
{
    std::string result;
    const std::size_t len = str.size();

    ok = true;
    for (std::size_t i = 0; i < len; ++i)
    {
        assert('0' <= str[i] && str[i] <= '9');
        if ('0' <= str[i] && str[i] <= '9')
        {
            result += Calc_H::s_jdigits[str[i] - '0'];
        }
        else
        {
            ok = false;
            break;
        }
    }
    return result;
}

std::string ChGetJpnNumber(
    CH_Value num,
    bool add_zero = true,
    bool add_zerodot = true)
{
    std::string str;
    if (num < 0)
    {
        str += "�܂��Ȃ�";
        num = -num;
    }
    assert(num >= 0);
    if (num >= ch_kei)
    {
        str += ChGetJpnNumber(num.to_i() / ch_kei, false);
        str += "����";
        str += ChGetJpnNumber(num % ch_kei, false);
        return str;
    }
    assert(num < ch_kei);
    if (num >= ch_chou)
    {
        str += ChGetJpnNumber(num.to_i() / ch_chou, false);
        str += "���傤";
        str += ChGetJpnNumber(num % ch_chou, false);
        return str;
    }
    assert(num < ch_chou);
    if (num >= ch_oku)
    {
        str += ChGetJpnNumber(num.to_i() / ch_oku, false);
        str += "����";
        str += ChGetJpnNumber(num % ch_oku, false);
        return str;
    }
    assert(num < ch_oku);
    if (num >= ch_man)
    {
        str += ChGetJpnNumber(num.to_i() / ch_man, false);
        str += "�܂�";
        str += ChGetJpnNumber(num % ch_man, false);
        return str;
    }
    assert(num < ch_man);
    if (num >= ch_sen)
    {
        str += ChGetJpnNumber(num.to_i() / ch_sen, false);
        str += "����";
        str += ChGetJpnNumber(num % ch_sen, false);
        return str;
    }
    assert(num < ch_sen);
    if (num >= ch_hyaku)
    {
        str += ChGetJpnNumber(num.to_i() / ch_hyaku, false);
        str += "�ЂႭ";
        str += ChGetJpnNumber(num % ch_hyaku, false);
        return str;
    }
    assert(num < ch_hyaku);
    if (num >= ch_juu)
    {
        str += ChGetJpnNumber(num.to_i() / ch_juu, false);
        str += "���イ";
        str += ChGetJpnNumber(num % ch_juu, false);
        return str;
    }
    assert(num < ch_juu);
    if (num >= ch_ichi)
    {
        str += Calc_H::s_jdigits[num.convert_to<int>()];
        num %= ch_ichi;
        if (!num.is_zero())
        {
            str += "�Ă�";
            str += ChGetJpnNumber(num, false, false);
        }
        return str;
    }
    assert(num < ch_ichi);
    if (num > ch_zero)
    {
        std::string digits = num.str(ch_precision, std::ios_base::fixed);
        std::size_t i = digits.find_last_not_of('0');
        if (i != std::string::npos)
            digits = digits.substr(0, i + 1);
        i = digits.find('.');
        if (i != std::string::npos)
        {
            bool ok;
            std::string jdigits;
            jdigits = ChGetJpnDigits(digits.substr(i + 1), ok);
            if (ok && !jdigits.empty())
            {
                if (add_zerodot)
                    str += "�ꂢ�Ă�";
                str += jdigits;
            }
            else
            {
                str += "����";
            }
        }
        else
        {
            str += "����";
        }
        return str;
    }
    else
    {
        if (add_zero)
            str += "����";
    }
    return str;
}

std::string ChGetJpnNumberFixed(CH_Value num)
{
    std::string str = ChGetJpnNumber(num);
    ChReplaceString(str, "�����Ă�", "�����Ă�");
    ChReplaceString(str, "�ɂĂ�", "�ɂ��Ă�");
    ChReplaceString(str, "���イ�Ă�", "�����Ă�");
    ChReplaceString(str, "�������イ", "���イ");
    ChReplaceString(str, "�����ЂႭ", "�ЂႭ");
    ChReplaceString(str, "����ЂႭ", "����тႭ");
    ChReplaceString(str, "�낭�ЂႭ", "����҂Ⴍ");
    ChReplaceString(str, "�͂��ЂႭ", "�͂��҂Ⴍ");
    ChReplaceString(str, "��������", "����");
    ChReplaceString(str, "���񂹂�", "���񂺂�");
    ChReplaceString(str, "�͂�����", "�͂�����");
    ChReplaceString(str, "�������傤", "�������傤");
    ChReplaceString(str, "��������", "��������");
    return str;
}

std::string ChGetJpnNumberBunsuu(CH_Value num)
{
    // bunsuu is fraction
    std::string str;
    pmp::Number bunshi = pmp::numerator(num);
    pmp::Number bunbo = pmp::denominator(num);
    bool minus = false;
    if (bunshi.sign() < 0)
    {
        minus = true;
        bunshi = -bunshi;
    }
    pmp::Number seisuu(0);
    if (bunshi >= bunbo)
    {
        pmp::Number::Type old_type = pmp::SetIntDivType(pmp::Number::INTEGER);
        seisuu = bunshi / bunbo;
        pmp::SetIntDivType(old_type);
        seisuu.trim();
        bunshi -= bunbo * seisuu;
    }
    if (minus)
        str += "�܂��Ȃ�";
    if (!seisuu.is_zero())
    {
        str += ChGetJpnNumberFixed(seisuu);
        str += "��";
    }
    str += ChGetJpnNumberFixed(bunbo);
    str += "�Ԃ��";
    str += ChGetJpnNumberFixed(bunshi);
    return str;
}

std::string ChGetJpnNumber2(CH_Value num)
{
    std::string str;
    pmp::Number::Type old_type = pmp::SetIntDivType(pmp::Number::INTEGER);
    if (num.type() == pmp::Number::RATIONAL)
        str = ChGetJpnNumberBunsuu(num);
    else
        str = ChGetJpnNumberFixed(num);
    pmp::SetIntDivType(old_type);
    return str;
}

////////////////////////////////////////////////////////////////////////////

void CrTrimString(std::string& str)
{
    static const char *spaces = " \t";
    std::size_t i, j;
    bool flag;
    do
    {
        i = str.find_first_not_of(spaces);
        j = str.find_last_not_of(spaces);
        if (i != std::string::npos)
        {
            if (j != std::string::npos)
                str = str.substr(i, j - i + 1);
            else
                str = str.substr(i);
        }
        else
        {
            if (j != std::string::npos)
                str = str.substr(0, j + 1);
        }

        const std::string zenkaku_space("�@");
        for (flag = false; ;)
        {
            i = str.find(zenkaku_space);
            if (i != 0)
                break;

            flag = true;
            str = str.substr(zenkaku_space.size());
        }
    } while (flag);
}

////////////////////////////////////////////////////////////////////////////

std::string ChJustDoIt(std::string& query)
{
    std::stringstream sstream;
    shared_ptr<Sentence> sentence;
    CrTrimString(query);

    if (query.empty())
        return "";

    // comment
    if (query.size() >= 2 && query[0] == '/' && query[1] == '/')
        return "";

    sstream << "�������F";

    if (query == "exit" || query == "EXIT" ||
        query == "��������" || query == "�d�w�h�s" ||
        query == "quit" || query == "QUIT" ||
        query == "��������" || query == "�p�t�h�s" ||
        query.find("�����") != std::string::npos ||
        query.find("�����") != std::string::npos)
    {
        sstream << "���イ��傤���܂��B" << std::endl;
        return sstream.str();
    }

    std::string error;
    query += "?";
    if (query.find("���肪��") != std::string::npos ||
        query.find("���񂫂�") != std::string::npos ||
        query.find("���񂵂�") != std::string::npos)
    {
        sstream << "�����炱�������Ă���Ă��肪�Ƃ��B" << std::endl;
    }
    else if (parse_string(sentence, query, error))
    {
        pmp::Number::Type old_type =
            pmp::SetIntDivType(pmp::Number::FLOATING);
        try
        {
            Calc_H::s_message.clear();
            CH_Value value = ChCalcSentence(sentence);
            value.trim();
            if (s_message.empty())
            {
                sstream << ChGetJpnNumber2(value) <<
                    " (" << value.str() << ") " << "�ł��B" << std::endl;
                s_sore = value;
            }
            else
            {
                sstream << Calc_H::s_message << std::endl;
            }
        }
        catch (const std::overflow_error&)
        {
            sstream << "���[�΁[�ӂ�[�ł��B" << std::endl;
        }
        catch (const std::runtime_error&)
        {
            sstream << "��������ł��܂���ł����B" << std::endl;
        }
        pmp::SetIntDivType(old_type); // restore old
    }
    else
    {
        if (error.empty())
        {
            sstream << "��������ł��܂���ł����B" << std::endl;
        }
        else
        {
            sstream << error << std::endl;
        }
    }

    return sstream.str();
}

////////////////////////////////////////////////////////////////////////////
