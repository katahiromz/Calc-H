// Calc-H.cpp
// (Japanese, Shift_JIS)
// Linuxの場合は、UTF-8に変換して下さい。

#include "stdafx.h"
#include "HParseHeader.h"

namespace Calc_H
{
    CH_Value s_sore = 0;

    static const char *s_jdigits[] =
    {
        "ぜろ", "いち", "に", "さん", "よん",
        "ご", "ろく", "なな", "はち", "きゅう"
    };
}

using namespace Calc_H;

CH_Value ChCalcExpr(const shared_ptr<Expr>& expr);
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
    case Prim::EXPR:
        return ChCalcExpr(prim->m_expr);

    case Prim::MINUS:
        return -ChCalcPrim(prim->m_prim);

    case Prim::NUM:
        return ChCalcNum(prim->m_num);

    case Prim::BUNSUU:
        return ChCalcNum(prim->m_num) / ChCalcPrim(prim->m_prim);

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
    ExprList::iterator it = el->begin();
    ExprList::iterator end = el->end();
    CH_Value value = ChCalcExpr(*it);
    for (++it; it != end; ++it)
    {
        value -= ChCalcExpr(*it);
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

    case Shite::EXPR_ONLY:
        return ChCalcExpr(shite->m_expr);

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
        str += "まいなす";
        num = -num;
    }
    assert(num >= 0);
    if (num >= ch_kei)
    {
        str += ChGetJpnNumber(num.to_i() / ch_kei, false);
        str += "けい";
        str += ChGetJpnNumber(num % ch_kei, false);
        return str;
    }
    assert(num < ch_kei);
    if (num >= ch_chou)
    {
        str += ChGetJpnNumber(num.to_i() / ch_chou, false);
        str += "ちょう";
        str += ChGetJpnNumber(num % ch_chou, false);
        return str;
    }
    assert(num < ch_chou);
    if (num >= ch_oku)
    {
        str += ChGetJpnNumber(num.to_i() / ch_oku, false);
        str += "おく";
        str += ChGetJpnNumber(num % ch_oku, false);
        return str;
    }
    assert(num < ch_oku);
    if (num >= ch_man)
    {
        str += ChGetJpnNumber(num.to_i() / ch_man, false);
        str += "まん";
        str += ChGetJpnNumber(num % ch_man, false);
        return str;
    }
    assert(num < ch_man);
    if (num >= ch_sen)
    {
        str += ChGetJpnNumber(num.to_i() / ch_sen, false);
        str += "せん";
        str += ChGetJpnNumber(num % ch_sen, false);
        return str;
    }
    assert(num < ch_sen);
    if (num >= ch_hyaku)
    {
        str += ChGetJpnNumber(num.to_i() / ch_hyaku, false);
        str += "ひゃく";
        str += ChGetJpnNumber(num % ch_hyaku, false);
        return str;
    }
    assert(num < ch_hyaku);
    if (num >= ch_juu)
    {
        str += ChGetJpnNumber(num.to_i() / ch_juu, false);
        str += "じゅう";
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
            str += "てん";
            str += ChGetJpnNumber(num, false, false);
        }
        return str;
    }
    assert(num < ch_ichi);
    if (num > ch_zero)
    {
        if (add_zerodot)
            str += "れいてん";

        std::string digits = num.str(ch_precision);
        const std::size_t i = digits.find('.');
        if (i != std::string::npos)
        {
            bool ok;
            std::string jdigits;
            jdigits = ChGetJpnDigits(digits.substr(i + 1), ok);
            if (ok)
            {
                str += jdigits;
            }
        }
        return str;
    }
    else
    {
        if (add_zero)
            str += "ぜろ";
    }
    return str;
}

std::string ChGetJpnNumberFixed(CH_Value num)
{
    std::string str = ChGetJpnNumber(num);
    ChReplaceString(str, "いちてん", "いってん");
    ChReplaceString(str, "にてん", "にいてん");
    ChReplaceString(str, "じゅうてん", "じってん");
    ChReplaceString(str, "いちじゅう", "じゅう");
    ChReplaceString(str, "いちひゃく", "ひゃく");
    ChReplaceString(str, "さんひゃく", "さんびゃく");
    ChReplaceString(str, "ろくひゃく", "ろっぴゃく");
    ChReplaceString(str, "はちひゃく", "はっぴゃく");
    ChReplaceString(str, "いちせん", "いっせん");
    ChReplaceString(str, "さんせん", "さんぜん");
    ChReplaceString(str, "はちせん", "はっせん");
    ChReplaceString(str, "いちちょう", "いっちょう");
    ChReplaceString(str, "いちけい", "いっけい");
    return str;
}

////////////////////////////////////////////////////////////////////////////

bool ChJustDoIt(std::string& query)
{
    shared_ptr<Sentence> sentence;
    if (parse_string(sentence, query))
    {
        bool old_enabled = pmp::EnableIntegerDivision(false);
        try
        {
            CH_Value value = ChCalcSentence(sentence);
            pmp::EnableIntegerDivision(true);
            std::cout << "こたえ：" << ChGetJpnNumberFixed(value) <<
                " (" << value.str() << ") " <<
                "です。" << std::endl;
            s_sore = value;
        }
        catch (const std::runtime_error&)
        {
            std::cout << "こたえ：" <<
                "けいさんできませんでした。" << std::endl;
        }
        std::cout << std::endl;
        pmp::EnableIntegerDivision(old_enabled); // restore enabled
        return true;
    }
    std::cout << std::endl;
    return false;
}

////////////////////////////////////////////////////////////////////////////

void CrTrimString(std::string& str)
{
    static const char *spaces = " \t";
    std::size_t i = str.find_first_not_of(spaces);
    std::size_t j = str.find_last_not_of(spaces);
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
}

////////////////////////////////////////////////////////////////////////////

void ChShowLogo(void)
{
    std::cerr <<
        "       +--------------------------------+" << std::endl <<
        "       |  ひらがな電卓 Calc-H ver.0.3.7 |" << std::endl <<
        "       |   by 片山博文MZ (katahiromz)   |" << std::endl <<
        "       | http://katahiromz.web.fc2.com/ |" << std::endl <<
        "       | katayama.hirofumi.mz@gmail.com |" << std::endl <<
        "       +--------------------------------+" << std::endl <<
        std::endl;
}

////////////////////////////////////////////////////////////////////////////

extern "C"
int main(int argc, char **argv)
{
    std::string query;

    ChShowLogo();
    std::cerr << "コメント：しょうすうもけいさんできるようになりました。" <<
                 std::endl << std::endl;

    if (argc <= 1)
    {
        std::cerr << "「exit」か「おわる」でしゅうりょうできます。" << std::endl;
        std::cerr << std::endl;
        for (;;)
        {
            std::cout << "にゅうりょく：";

            if (!std::getline(std::cin, query))
                break;

            CrTrimString(query);

            if (query == "exit" || query == "EXIT" ||
                query == "ｅｘｉｔ" || query == "ＥＸＩＴ" ||
                query == "quit" || query == "QUIT" ||
                query == "ｑｕｉｔ" || query == "ＱＵＩＴ" ||
                query == "おわる" || query == "おわり")
                break;

            if (query.find("ありがと") != std::string::npos ||
                query.find("さんきゅ") != std::string::npos ||
                query.find("かんしゃ") != std::string::npos)
            {
                std::cout << "こちらこそつかってくれてありがとう。" << std::endl;
                continue;
            }

            query += "?";
            ChJustDoIt(query);
        }
    }
    else
    {
        query = argv[1];

        CrTrimString(query);

        query += "?";
        if (!ChJustDoIt(query))
        {
            return 1;
        }
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////
