// Calc-H.cpp
// (Japanese, Shift_JIS)
// Linuxの場合は、UTF-8に変換して下さい。

#include "Calc-H.h"
#include "HParseHeader.h"

namespace Calc_H
{
    CH_Value s_sore = 0;
    std::string s_message;

    static const char *s_jdigits[] =
    {
        "ぜろ", "いち", "に", "さん", "よん",
        "ご", "ろく", "なな", "はち", "きゅう"
    };
}

using namespace Calc_H;

CH_Value ChCalcExpr(const shared_ptr<Expr>& expr);
CH_Value ChCalcMono(const shared_ptr<Mono>& mono);
CH_Value ChCalcFact(const shared_ptr<Fact>& fact);
CH_Value ChCalcShite(const shared_ptr<Shite>& shite);
CH_Value ChCalcSuruto(const shared_ptr<Suruto>& suruto);

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
                Calc_H::s_message = "分母がゼロになったので計算できません。";
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
                Calc_H::s_message = "分母がゼロになったので計算できません。";
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
        Calc_H::s_message = "三つ以上の対象がある差は求められません。";
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
        Calc_H::s_message = "三つ以上の対象がある商は求められません。";
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

    case Mono::SHITE_ONLY:
        return ChCalcShite(mono->m_shite);

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

    case Mono::TERM_ONLY:
        return ChCalcTerm(mono->m_term);

    case Mono::FACT_ONLY:
        return ChCalcFact(mono->m_fact);

    case Mono::SURUTO_ONLY:
        return ChCalcSuruto(mono->m_suruto);

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
        return ChCalcExpr(suruto->m_expr) - ChCalcMono(suruto->m_mono);

    case Suruto::MONO_DE_EXPR_DIV:
        return ChCalcExpr(suruto->m_expr) / ChCalcMono(suruto->m_mono);

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

void ChAnalyzeExpr(shared_ptr<Expr>& expr);
void ChAnalyzeExprList(shared_ptr<ExprList>& exprlist);

void ChAnalyzeMonoTermTasukazu(shared_ptr<Mono>& mono, shared_ptr<Term>& term);
void ChAnalyzeMonoTermKakerukazu(shared_ptr<Mono>& mono, shared_ptr<Term>& term);
void ChAnalyzeMonoTermHikukazu(shared_ptr<Mono>& mono, shared_ptr<Term>& term);
void ChAnalyzeMonoTermWarukazu(shared_ptr<Mono>& mono, shared_ptr<Term>& term);

void ChAnalyzeMonoFactTasukazu(shared_ptr<Mono>& mono, shared_ptr<Fact>& fact);
void ChAnalyzeMonoFactKakerukazu(shared_ptr<Mono>& mono, shared_ptr<Fact>& fact);
void ChAnalyzeMonoFactHikukazu(shared_ptr<Mono>& mono, shared_ptr<Fact>& fact);
void ChAnalyzeMonoFactWarukazu(shared_ptr<Mono>& mono, shared_ptr<Fact>& fact);

void ChAnalyzeMonoSurutoTasukazu(shared_ptr<Mono>& mono, shared_ptr<Suruto>& suruto);
void ChAnalyzeMonoSurutoKakerukazu(shared_ptr<Mono>& mono, shared_ptr<Suruto>& suruto);
void ChAnalyzeMonoSurutoHikukazu(shared_ptr<Mono>& mono, shared_ptr<Suruto>& suruto);
void ChAnalyzeMonoSurutoWarukazu(shared_ptr<Mono>& mono, shared_ptr<Suruto>& suruto);

void ChAnalyzeMonoMonoTasukazu(shared_ptr<Mono>& mono, shared_ptr<Mono>& mono2);
void ChAnalyzeMonoMonoKakerukazu(shared_ptr<Mono>& mono, shared_ptr<Mono>& mono2);
void ChAnalyzeMonoMonoHikukazu(shared_ptr<Mono>& mono, shared_ptr<Mono>& mono2);
void ChAnalyzeMonoMonoWarukazu(shared_ptr<Mono>& mono, shared_ptr<Mono>& mono2);

void ChAnalyzeMonoExprTasukazu(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr);
void ChAnalyzeMonoExprKakerukazu(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr);
void ChAnalyzeMonoExprHikukazu(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr);
void ChAnalyzeMonoExprWarukazu(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr);

void ChAnalyzeMonoTermTasukazu(shared_ptr<Mono>& mono, shared_ptr<Term>& term)
{
    switch (term->m_type)
    {
    case Term::FACT_ONLY:
        ChAnalyzeMonoFactTasukazu(mono, term->m_fact);
        break;

    default:
        Calc_H::s_message = "たしざんではありません。";
    }
}

void ChAnalyzeMonoTermHikukazu(shared_ptr<Mono>& mono, shared_ptr<Term>& term)
{
    switch (term->m_type)
    {
    case Term::FACT_ONLY:
        ChAnalyzeMonoFactHikukazu(mono, term->m_fact);
        break;

    default:
        Calc_H::s_message = "ひきざんではありません。";
    }
}

void ChAnalyzeMonoShiteTasukazu(shared_ptr<Mono>& mono, shared_ptr<Shite>& shite)
{
    Mono *m;
    switch (shite->m_type)
    {
    case Shite::EXPRLIST_ADD:
        {
            ChAnalyzeExprList(shite->m_exprlist);
            ExprList *el = shite->m_exprlist.get();
            const std::size_t count = el->size();
            assert(count >= 2);
            shared_ptr<Expr> e = (*el)[count - 1];
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = e;
            mono = shared_ptr<Mono>(m);
        }
        break;

    case Shite::MONO_ADD:
    case Shite::SHITE_ADD:
        m = new Mono;
        m->m_type = Mono::EXPR_ONLY;
        m->m_expr = shite->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    case Shite::MONO_ONLY:
        ChAnalyzeMonoMonoTasukazu(mono, shite->m_mono);
        break;

    default:
        Calc_H::s_message = "たしざんではありません。";
    }
}

void ChAnalyzeMonoShiteKakerukazu(shared_ptr<Mono>& mono, shared_ptr<Shite>& shite)
{
    Mono *m;
    switch (shite->m_type)
    {
    case Shite::EXPRLIST_MUL:
        {
            ChAnalyzeExprList(shite->m_exprlist);
            ExprList *el = shite->m_exprlist.get();
            const std::size_t count = el->size();
            assert(count >= 2);
            shared_ptr<Expr> e = (*el)[count - 1];
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = e;
            mono = shared_ptr<Mono>(m);
        }
        break;

    case Shite::MONO_MUL:
    case Shite::SHITE_MUL:
        m = new Mono;
        m->m_type = Mono::EXPR_ONLY;
        m->m_expr = shite->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    case Shite::MONO_ONLY:
        ChAnalyzeMonoMonoKakerukazu(mono, shite->m_mono);
        break;

    default:
        Calc_H::s_message = "かけざんではありません。";
    }
}

void ChAnalyzeMonoShiteHikukazu(shared_ptr<Mono>& mono, shared_ptr<Shite>& shite)
{
    Mono *m;
    switch (shite->m_type)
    {
    case Shite::MONO_ONLY:
        ChAnalyzeMonoMonoHikukazu(mono, shite->m_mono);
        break;

    case Shite::MONO_SUB:
    case Shite::SHITE_SUB:
    case Shite::MONO_WO_EXPR_SUB:
        m = new Mono;
        m->m_type = Mono::EXPR_ONLY;
        m->m_expr = shite->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    default:
        Calc_H::s_message = "ひきざんではありません。";
    }
}

void ChAnalyzeMonoShiteWarukazu(shared_ptr<Mono>& mono, shared_ptr<Shite>& shite)
{
    Mono *m;
    switch (shite->m_type)
    {
    case Shite::MONO_ONLY:
        ChAnalyzeMonoMonoWarukazu(mono, shite->m_mono);
        break;

    case Shite::MONO_DIV:
    case Shite::SHITE_DIV:
    case Shite::MONO_WO_EXPR_DIV:
        m = new Mono;
        m->m_type = Mono::EXPR_ONLY;
        m->m_expr = shite->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    default:
        Calc_H::s_message = "わりざんではありません。";
    }
}

void ChAnalyzeMonoMonoTasukazu(shared_ptr<Mono>& mono, shared_ptr<Mono>& mono2)
{
    Mono *m;
    switch (mono2->m_type)
    {
    case Mono::EXPRLIST_ADD:
    case Mono::MONO_TO_EXPRLIST_ADD:
        {
            ChAnalyzeExprList(mono2->m_exprlist);
            ExprList *el = mono2->m_exprlist.get();
            const std::size_t count = el->size();
            assert(count >= 2);
            shared_ptr<Expr> e = (*el)[count - 1];
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = e;
            mono = shared_ptr<Mono>(m);
        }
        break;

    case Mono::MONO_ADD:
    case Mono::SHITE_ADD:
    case Mono::MONO_TO_EXPR_ADD:
        m = new Mono;
        m->m_type = Mono::EXPR_ONLY;
        m->m_expr = mono2->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    case Mono::MONO_ONLY:
        ChAnalyzeMonoMonoTasukazu(mono, mono2->m_mono);
        break;

    case Mono::SHITE_ONLY:
        ChAnalyzeMonoShiteTasukazu(mono, mono2->m_shite);
        break;

    case Mono::EXPR_ONLY:
        ChAnalyzeMonoExprTasukazu(mono, mono2->m_expr);
        break;

    //case Mono::SORE:

    case Mono::TERM_ONLY:
        ChAnalyzeMonoTermTasukazu(mono, mono2->m_term);
        break;

    case Mono::FACT_ONLY:
        ChAnalyzeMonoFactTasukazu(mono, mono2->m_fact);
        break;

    case Mono::SURUTO_ONLY:
        ChAnalyzeMonoSurutoTasukazu(mono, mono2->m_suruto);
        break;

    default:
        Calc_H::s_message = "たしざんではありません。";
    }
}

void ChAnalyzeMonoMonoKakerukazu(shared_ptr<Mono>& mono, shared_ptr<Mono>& mono2)
{
    Mono *m;
    switch (mono2->m_type)
    {
    case Mono::EXPRLIST_MUL:
    case Mono::MONO_TO_EXPRLIST_MUL:
        {
            ChAnalyzeExprList(mono2->m_exprlist);
            ExprList *el = mono2->m_exprlist.get();
            const std::size_t count = el->size();
            assert(count >= 2);
            shared_ptr<Expr> e = (*el)[count - 1];
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = e;
            mono = shared_ptr<Mono>(m);
        }
        break;

    case Mono::MONO_MUL:
    case Mono::SHITE_MUL:
    case Mono::MONO_TO_EXPR_MUL:
        m = new Mono;
        m->m_type = Mono::EXPR_ONLY;
        m->m_expr = mono2->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    case Mono::MONO_ONLY:
        ChAnalyzeMonoMonoKakerukazu(mono, mono2->m_mono);
        break;

    case Mono::SHITE_ONLY:
        ChAnalyzeMonoShiteKakerukazu(mono, mono2->m_shite);
        break;

    case Mono::EXPR_ONLY:
        ChAnalyzeMonoExprKakerukazu(mono, mono2->m_expr);
        break;

    //case Mono::SORE:

    case Mono::TERM_ONLY:
        ChAnalyzeMonoTermKakerukazu(mono, mono2->m_term);
        break;

    case Mono::FACT_ONLY:
        ChAnalyzeMonoFactKakerukazu(mono, mono2->m_fact);
        break;

    case Mono::SURUTO_ONLY:
        ChAnalyzeMonoSurutoKakerukazu(mono, mono2->m_suruto);
        break;

    default:
        Calc_H::s_message = "かけざんではありません。";
    }
}

void ChAnalyzeMonoMonoHikukazu(shared_ptr<Mono>& mono, shared_ptr<Mono>& mono2)
{
    Mono *m;
    switch (mono2->m_type)
    {
    case Mono::EXPRLIST_SUB:
        {
            ChAnalyzeExprList(mono2->m_exprlist);
            ExprList *el = mono2->m_exprlist.get();
            const std::size_t count = el->size();
            assert(count >= 2);
            shared_ptr<Expr> e = (*el)[count - 1];
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = e;
            mono = shared_ptr<Mono>(m);
        }
        break;

    case Mono::MONO_SUB:
    case Mono::MONO_TO_EXPR_SUB:
    case Mono::SHITE_SUB:
        m = new Mono;
        m->m_type = Mono::EXPR_ONLY;
        m->m_expr = mono2->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    case Mono::MONO_ONLY:
        ChAnalyzeMonoMonoHikukazu(mono, mono2->m_mono);
        break;

    case Mono::SHITE_ONLY:
        ChAnalyzeMonoShiteHikukazu(mono, mono2->m_shite);
        break;

    case Mono::EXPR_ONLY:
        ChAnalyzeMonoExprHikukazu(mono, mono2->m_expr);
        break;

    //case Mono::SORE:

    case Mono::MONO_WO_EXPR_KARA_SUB:
        mono = mono2->m_mono;
        break;

    case Mono::TERM_ONLY:
        ChAnalyzeMonoTermHikukazu(mono, mono2->m_term);
        break;

    case Mono::FACT_ONLY:
        ChAnalyzeMonoFactHikukazu(mono, mono2->m_fact);
        break;

    case Mono::SURUTO_ONLY:
        ChAnalyzeMonoSurutoHikukazu(mono, mono2->m_suruto);
        break;

    default:
        Calc_H::s_message = "ひきざんではありません。";
    }
}

void ChAnalyzeMonoMonoWarukazu(shared_ptr<Mono>& mono, shared_ptr<Mono>& mono2)
{
    Mono *m;
    switch (mono2->m_type)
    {
    case Mono::EXPRLIST_DIV:
        {
            ChAnalyzeExprList(mono2->m_exprlist);
            ExprList *el = mono2->m_exprlist.get();
            const std::size_t count = el->size();
            assert(count >= 2);
            shared_ptr<Expr> e = (*el)[count - 1];
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = e;
            mono = shared_ptr<Mono>(m);
        }
        break;

    case Mono::MONO_DIV:
    case Mono::MONO_TO_EXPR_DIV:
    case Mono::SHITE_DIV:
        m = new Mono;
        m->m_type = Mono::EXPR_ONLY;
        m->m_expr = mono2->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    case Mono::MONO_ONLY:
        ChAnalyzeMonoMonoWarukazu(mono, mono2->m_mono);
        break;

    case Mono::SHITE_ONLY:
        ChAnalyzeMonoShiteWarukazu(mono, mono2->m_shite);
        break;

    case Mono::EXPR_ONLY:
        ChAnalyzeMonoExprWarukazu(mono, mono2->m_expr);
        break;

    //case Mono::SORE:

    case Mono::MONO_DE_EXPR_WO_DIV:
        mono = mono2->m_mono;
        break;

    case Mono::TERM_ONLY:
        ChAnalyzeMonoTermWarukazu(mono, mono2->m_term);
        break;

    case Mono::FACT_ONLY:
        ChAnalyzeMonoFactWarukazu(mono, mono2->m_fact);
        break;

    case Mono::SURUTO_ONLY:
        ChAnalyzeMonoSurutoWarukazu(mono, mono2->m_suruto);
        break;

    default:
        Calc_H::s_message = "わりざんではありません。";
    }
}

void ChAnalyzeMonoFactTasukazu(shared_ptr<Mono>& mono, shared_ptr<Fact>& fact)
{
    if (fact->m_type == Fact::SINGLE && fact->m_prim->m_type == Prim::MONO)
        ChAnalyzeMonoMonoTasukazu(mono, fact->m_prim->m_mono);
    else
        Calc_H::s_message = "たしざんではありません。";
}

void ChAnalyzeMonoFactKakerukazu(shared_ptr<Mono>& mono, shared_ptr<Fact>& fact)
{
    if (fact->m_type == Fact::SINGLE && fact->m_prim->m_type == Prim::MONO)
        ChAnalyzeMonoMonoKakerukazu(mono, fact->m_prim->m_mono);
    else
        Calc_H::s_message = "かけざんではありません。";
}

void ChAnalyzeMonoFactHikukazu(shared_ptr<Mono>& mono, shared_ptr<Fact>& fact)
{
    if (fact->m_type == Fact::SINGLE && fact->m_prim->m_type == Prim::MONO)
        ChAnalyzeMonoMonoHikukazu(mono, fact->m_prim->m_mono);
    else
        Calc_H::s_message = "ひきざんではありません。";
}

void ChAnalyzeMonoFactWarukazu(shared_ptr<Mono>& mono, shared_ptr<Fact>& fact)
{
    if (fact->m_type == Fact::SINGLE && fact->m_prim->m_type == Prim::MONO)
        ChAnalyzeMonoMonoWarukazu(mono, fact->m_prim->m_mono);
    else
        Calc_H::s_message = "わりざんではありません。";
}

void ChAnalyzeMonoTermKakerukazu(shared_ptr<Mono>& mono, shared_ptr<Term>& term)
{
    Mono *m;
    switch (term->m_type)
    {
    case Term::MUL:
        m = new Mono;
        m->m_type = Mono::FACT_ONLY;
        m->m_fact = term->m_fact;
        mono = shared_ptr<Mono>(m);
        break;

    case Term::DIV:
        Calc_H::s_message = "かけざんではありません。";
        break;

    case Term::FACT_ONLY:
        ChAnalyzeMonoFactKakerukazu(mono, term->m_fact);
        break;
    }
}

void ChAnalyzeMonoTermWarukazu(shared_ptr<Mono>& mono, shared_ptr<Term>& term)
{
    Mono *m;
    switch (term->m_type)
    {
    case Term::MUL:
        Calc_H::s_message = "わりざんではありません。";
        break;

    case Term::DIV:
        m = new Mono;
        m->m_type = Mono::FACT_ONLY;
        m->m_fact = term->m_fact;
        mono = shared_ptr<Mono>(m);
        break;

    case Term::FACT_ONLY:
        ChAnalyzeMonoFactWarukazu(mono, term->m_fact);
        break;
    }
}

void ChAnalyzeMonoExprTasukazu(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
{
    Mono *m;
    ChAnalyzeExpr(expr);
    switch (expr->m_type)
    {
    case Expr::ADD:
        m = new Mono;
        m->m_type = Mono::TERM_ONLY;
        m->m_term = expr->m_term;
        mono = shared_ptr<Mono>(m);
        break;

    case Expr::TERM_ONLY:
        ChAnalyzeMonoTermTasukazu(mono, expr->m_term);
        break;

    default:
        Calc_H::s_message = "たしざんではありません。";
        break;
    }
}

void ChAnalyzeMonoExprKakerukazu(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
{
    ChAnalyzeExpr(expr);
    switch (expr->m_type)
    {
    case Expr::ADD:
    case Expr::SUB:
        Calc_H::s_message = "かけざんではありません。";
        break;

    case Expr::TERM_ONLY:
        ChAnalyzeMonoTermKakerukazu(mono, expr->m_term);
        break;
    }
}

void ChAnalyzeMonoExprHikukazu(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
{
    Mono *m;
    ChAnalyzeExpr(expr);
    switch (expr->m_type)
    {
    case Expr::ADD:
        Calc_H::s_message = "ひきざんではありません。";
        break;

    case Expr::SUB:
        m = new Mono;
        m->m_type = Mono::TERM_ONLY;
        m->m_term = expr->m_term;
        mono = shared_ptr<Mono>(m);
        break;

    case Expr::TERM_ONLY:
        ChAnalyzeMonoTermHikukazu(mono, expr->m_term);
        break;
    }
}

void ChAnalyzeMonoExprWarukazu(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
{
    ChAnalyzeExpr(expr);
    switch (expr->m_type)
    {
    case Expr::ADD:
    case Expr::SUB:
        Calc_H::s_message = "かけざんではありません。";
        break;

    case Expr::TERM_ONLY:
        ChAnalyzeMonoTermWarukazu(mono, expr->m_term);
        break;
    }
}

void ChAnalyzeMonoTermKakerarerukazu(shared_ptr<Mono>& mono, shared_ptr<Term>& term)
{
    Mono *m;
    switch (term->m_type)
    {
    case Term::MUL:
        m = new Mono;
        m->m_type = Mono::TERM_ONLY;
        m->m_term = term->m_term;
        mono = shared_ptr<Mono>(m);
        break;

    case Term::DIV:
    case Term::FACT_ONLY:
        Calc_H::s_message = "かけざんではありません。";
        break;
    }
}

void ChAnalyzeMonoExprTasarerukazu(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
{
    Mono *m;
    ChAnalyzeExpr(expr);
    switch (expr->m_type)
    {
    case Expr::ADD:
        m = new Mono;
        m->m_type = Mono::EXPR_ONLY;
        m->m_expr = expr->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    case Expr::SUB:
    case Expr::TERM_ONLY:
        Calc_H::s_message = "たしざんではありません。";
        break;
    }
}

void ChAnalyzeMonoTermWararerukazu(shared_ptr<Mono>& mono, shared_ptr<Term>& term)
{
    Mono *m;
    switch (term->m_type)
    {
    case Term::DIV:
        m = new Mono;
        m->m_type = Mono::TERM_ONLY;
        m->m_term = term->m_term;
        mono = shared_ptr<Mono>(m);
        break;

    case Term::MUL:
    case Term::FACT_ONLY:
        Calc_H::s_message = "わりざんではありません。";
        break;
    }
}

void ChAnalyzeMonoExprKakerarerukazu(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
{
    ChAnalyzeExpr(expr);
    switch (expr->m_type)
    {
    case Expr::ADD:
    case Expr::SUB:
        Calc_H::s_message = "かけざんではありません。";
        break;

    case Expr::TERM_ONLY:
        ChAnalyzeMonoTermKakerarerukazu(mono, expr->m_term);
        break;
    }
}

void ChAnalyzeMonoExprHikarerukazu(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
{
    Mono *m;
    ChAnalyzeExpr(expr);
    switch (expr->m_type)
    {
    case Expr::SUB:
        m = new Mono;
        m->m_type = Mono::EXPR_ONLY;
        m->m_expr = expr->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    case Expr::ADD:
    case Expr::TERM_ONLY:
        Calc_H::s_message = "ひきざんではありません。";
        break;
    }
}

void ChAnalyzeMonoExprWararerukazu(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
{
    ChAnalyzeExpr(expr);
    switch (expr->m_type)
    {
    case Expr::ADD:
    case Expr::SUB:
        Calc_H::s_message = "わりざんではありません。";
        break;

    case Expr::TERM_ONLY:
        ChAnalyzeMonoTermWararerukazu(mono, expr->m_term);
        break;
    }
}

void ChAnalyzeMonoSurutoTasukazu(shared_ptr<Mono>& mono, shared_ptr<Suruto>& suruto)
{
	Mono *m;
    switch (suruto->m_type)
    {
    case Suruto::EXPRLIST_ADD:
        {
            ExprList *el = suruto->m_exprlist.get();
            const std::size_t count = el->size();
            assert(count >= 2);
            shared_ptr<Expr> e = (*el)[count - 1];
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = e;
            mono = shared_ptr<Mono>(m);
        }
        break;

    case Suruto::MONO_ADD:
    case Suruto::SHITE_TASUTO:
        m = new Mono;
        m->m_type = Mono::EXPR_ONLY;
        m->m_expr = suruto->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    default:
        Calc_H::s_message = "たしざんではありません。";
    }
}

void ChAnalyzeMonoSurutoKakerukazu(shared_ptr<Mono>& mono, shared_ptr<Suruto>& suruto)
{
	Mono *m;
    switch (suruto->m_type)
    {
    case Suruto::EXPRLIST_MUL:
        {
            ExprList *el = suruto->m_exprlist.get();
            const std::size_t count = el->size();
            assert(count >= 2);
            shared_ptr<Expr> e = (*el)[count - 1];
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = e;
            mono = shared_ptr<Mono>(m);
        }
        break;

    case Suruto::MONO_MUL:
    case Suruto::SHITE_KAKERUTO:
        m = new Mono;
        m->m_type = Mono::EXPR_ONLY;
        m->m_expr = suruto->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    default:
        Calc_H::s_message = "かけざんではありません。";
    }
}

void ChAnalyzeMonoSurutoHikukazu(shared_ptr<Mono>& mono, shared_ptr<Suruto>& suruto)
{
    Mono *m;
    switch (suruto->m_type)
    {
    case Suruto::MONO_SUB:
    case Suruto::SHITE_HIKUTO:
        m = new Mono;
        m->m_type = Mono::EXPR_ONLY;
        m->m_expr = suruto->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    case Suruto::MONO_WO_EXPR_SUB:
        m = new Mono;
        m->m_type = Mono::MONO_ONLY;
        m->m_mono = suruto->m_mono;
        mono = shared_ptr<Mono>(m);
        break;

    default:
        Calc_H::s_message = "ひきざんではありません。";
    }
}

void ChAnalyzeMonoSurutoWarukazu(shared_ptr<Mono>& mono, shared_ptr<Suruto>& suruto)
{
    Mono *m;
    switch (suruto->m_type)
    {
    case Suruto::MONO_DIV:
    case Suruto::SHITE_WARUTO:
        m = new Mono;
        m->m_type = Mono::EXPR_ONLY;
        m->m_expr = suruto->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    case Suruto::MONO_DE_EXPR_DIV:
        m = new Mono;
        m->m_type = Mono::MONO_ONLY;
        m->m_mono = suruto->m_mono;
        mono = shared_ptr<Mono>(m);
        break;

    default:
        Calc_H::s_message = "わりざんではありません。";
    }
}

void ChAnalyzeMonoSurutoTasarerukazu(shared_ptr<Mono>& mono, shared_ptr<Suruto>& suruto)
{
    Mono *m;
    switch (suruto->m_type)
    {
    case Suruto::EXPRLIST_ADD:
        {
            ExprList *el = suruto->m_exprlist.get();
            const std::size_t count = el->size();
            assert(count >= 2);
            shared_ptr<Expr> e = (*el)[0];
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = e;
            mono = shared_ptr<Mono>(m);
        }
        break;

    case Suruto::MONO_ADD:
        m = new Mono;
        m->m_type = Mono::MONO_ONLY;
        m->m_mono = suruto->m_mono;
        mono = shared_ptr<Mono>(m);
        break;

    case Suruto::SHITE_TASUTO:
        m = new Mono;
        m->m_type = Mono::SHITE_ONLY;
        m->m_shite = suruto->m_shite;
        mono = shared_ptr<Mono>(m);
        break;

    default:
        Calc_H::s_message = "たしざんではありません。";
    }
}

void ChAnalyzeMonoSurutoKakerarerukazu(shared_ptr<Mono>& mono, shared_ptr<Suruto>& suruto)
{
    Mono *m;
    switch (suruto->m_type)
    {
    case Suruto::EXPRLIST_MUL:
        {
            ExprList *el = suruto->m_exprlist.get();
            const std::size_t count = el->size();
            assert(count >= 2);
            shared_ptr<Expr> e = (*el)[0];
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = e;
            mono = shared_ptr<Mono>(m);
        }
        break;

    case Suruto::MONO_MUL:
        m = new Mono;
        m->m_type = Mono::MONO_ONLY;
        m->m_mono = suruto->m_mono;
        mono = shared_ptr<Mono>(m);
        break;

    case Suruto::SHITE_KAKERUTO:
        m = new Mono;
        m->m_type = Mono::SHITE_ONLY;
        m->m_shite = suruto->m_shite;
        mono = shared_ptr<Mono>(m);
        break;

    default:
        Calc_H::s_message = "かけざんではありません。";
    }
}

void ChAnalyzeMonoSurutoHikarerukazu(shared_ptr<Mono>& mono, shared_ptr<Suruto>& suruto)
{
    Mono *m;
    switch (suruto->m_type)
    {
    case Suruto::MONO_SUB:
        m = new Mono;
        m->m_type = Mono::MONO_ONLY;
        m->m_mono = suruto->m_mono;
        mono = shared_ptr<Mono>(m);
        break;

    case Suruto::SHITE_HIKUTO:
        m = new Mono;
        m->m_type = Mono::SHITE_ONLY;
        m->m_shite = suruto->m_shite;
        mono = shared_ptr<Mono>(m);
        break;

    case Suruto::MONO_WO_EXPR_SUB:
        m = new Mono;
        m->m_type = Mono::EXPR_ONLY;
        m->m_expr = suruto->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    default:
        Calc_H::s_message = "ひきざんではありません。";
    }
}

void ChAnalyzeMonoSurutoWararerukazu(shared_ptr<Mono>& mono, shared_ptr<Suruto>& suruto)
{
    Mono *m;
    switch (suruto->m_type)
    {
    case Suruto::MONO_DIV:
        m = new Mono;
        m->m_type = Mono::MONO_ONLY;
        m->m_mono = suruto->m_mono;
        mono = shared_ptr<Mono>(m);
        break;

    case Suruto::SHITE_WARUTO:
        m = new Mono;
        m->m_type = Mono::SHITE_ONLY;
        m->m_shite = suruto->m_shite;
        mono = shared_ptr<Mono>(m);
        break;

    case Suruto::MONO_DE_EXPR_DIV:
        m = new Mono;
        m->m_type = Mono::EXPR_ONLY;
        m->m_expr = suruto->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    default:
        Calc_H::s_message = "わりざんではありません。";
    }
}

void ChAnalyzeMono(shared_ptr<Mono>& mono)
{
    Mono *m;
    switch (mono->m_type)
    {
    case Mono::MONO_NO_TASUKAZU:
        switch (mono->m_mono->m_type)
        {
        case Mono::EXPR_ONLY:
            ChAnalyzeMonoExprTasukazu(mono, mono->m_mono->m_expr);
            break;

        case Mono::SURUTO_ONLY:
            ChAnalyzeMonoSurutoTasukazu(mono, mono->m_mono->m_suruto);
            break;

        case Mono::MONO_ADD:
        case Mono::SHITE_ADD:
        case Mono::MONO_TO_EXPR_ADD:
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = mono->m_mono->m_expr;
            mono = shared_ptr<Mono>(m);
            break;

        case Mono::MONO_TO_EXPRLIST_ADD:
            m = new Mono;
            m->m_type = Mono::EXPRLIST_ADD;
            m->m_exprlist = mono->m_mono->m_exprlist;
            mono = shared_ptr<Mono>(m);
            break;

        default:
            Calc_H::s_message = "たしざんではありません。";
            break;
        }
        break;

    case Mono::MONO_NO_KAKERUKAZU:
        switch (mono->m_mono->m_type)
        {
        case Mono::EXPR_ONLY:
            ChAnalyzeMonoExprKakerukazu(mono, mono->m_mono->m_expr);
            break;

        case Mono::SURUTO_ONLY:
            ChAnalyzeMonoSurutoKakerukazu(mono, mono->m_mono->m_suruto);
            break;

        case Mono::MONO_MUL:
        case Mono::SHITE_MUL:
        case Mono::MONO_TO_EXPR_MUL:
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = mono->m_mono->m_expr;
            mono = shared_ptr<Mono>(m);
            break;

        case Mono::MONO_TO_EXPRLIST_MUL:
            m = new Mono;
            m->m_type = Mono::EXPRLIST_MUL;
            m->m_exprlist = mono->m_mono->m_exprlist;
            mono = shared_ptr<Mono>(m);
            break;

        default:
            Calc_H::s_message = "かけざんではありません。";
            break;
        }
        break;

    case Mono::MONO_NO_HIKUKAZU:
        switch (mono->m_mono->m_type)
        {
        case Mono::EXPR_ONLY:
            ChAnalyzeMonoExprHikukazu(mono, mono->m_mono->m_expr);
            break;

        case Mono::SURUTO_ONLY:
            ChAnalyzeMonoSurutoHikukazu(mono, mono->m_mono->m_suruto);
            break;

        case Mono::MONO_SUB:
        case Mono::SHITE_SUB:
        case Mono::MONO_TO_EXPR_SUB:
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = mono->m_mono->m_expr;
            mono = shared_ptr<Mono>(m);
            break;

        default:
            Calc_H::s_message = "ひきざんではありません。";
            break;
        }
        break;

    case Mono::MONO_NO_WARUKAZU:
        switch (mono->m_mono->m_type)
        {
        case Mono::EXPR_ONLY:
            ChAnalyzeMonoExprWarukazu(mono, mono->m_mono->m_expr);
            break;

        case Mono::SURUTO_ONLY:
            ChAnalyzeMonoSurutoWarukazu(mono, mono->m_mono->m_suruto);
            break;

        case Mono::MONO_DIV:
        case Mono::SHITE_DIV:
        case Mono::MONO_TO_EXPR_DIV:
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = mono->m_mono->m_expr;
            mono = shared_ptr<Mono>(m);
            break;

        default:
            Calc_H::s_message = "ひきざんではありません。";
            break;
        }
        break;

    case Mono::SURUTOKI_TASUKAZU:
        ChAnalyzeMonoSurutoTasukazu(mono, mono->m_suruto);
        break;

    case Mono::SURUTOKI_KAKERUKAZU:
        ChAnalyzeMonoSurutoKakerukazu(mono, mono->m_suruto);
        break;

    case Mono::SURUTOKI_HIKUKAZU:
        ChAnalyzeMonoSurutoHikukazu(mono, mono->m_suruto);
        break;

    case Mono::SURUTOKI_WARUKAZU:
        ChAnalyzeMonoSurutoWarukazu(mono, mono->m_suruto);
        break;

    case Mono::SURUTOKI_TASARERUKAZU:
        ChAnalyzeMonoSurutoTasarerukazu(mono, mono->m_suruto);
        break;

    case Mono::SURUTOKI_KAKERARERUKAZU:
        ChAnalyzeMonoSurutoKakerarerukazu(mono, mono->m_suruto);
        break;

    case Mono::SURUTOKI_HIKARERUKAZU:
        ChAnalyzeMonoSurutoHikarerukazu(mono, mono->m_suruto);
        break;

    case Mono::SURUTOKI_WARARERUKAZU:
        ChAnalyzeMonoSurutoWararerukazu(mono, mono->m_suruto);
        break;

    case Mono::MONO_NO_TASARERUKAZU:
        switch (mono->m_mono->m_type)
        {
        case Mono::EXPR_ONLY:
            ChAnalyzeMonoExprTasarerukazu(mono, mono->m_mono->m_expr);
            break;

        case Mono::SURUTO_ONLY:
            ChAnalyzeMonoSurutoTasarerukazu(mono, mono->m_mono->m_suruto);
            break;

        case Mono::EXPRLIST_ADD:
            {
                ExprList *el = mono->m_mono->m_exprlist.get();
                const std::size_t count = el->size();
                assert(count >= 2);
                shared_ptr<Expr> e = (*el)[0];
                m = new Mono;
                m->m_type = Mono::EXPR_ONLY;
                m->m_expr = e;
                mono = shared_ptr<Mono>(m);
            }
            break;
            
        case Mono::MONO_ADD:
        case Mono::MONO_TO_EXPRLIST_ADD:
        case Mono::MONO_TO_EXPR_ADD:
            m = new Mono;
            m->m_type = Mono::MONO_ONLY;
            m->m_mono = mono->m_mono->m_mono;
            mono = shared_ptr<Mono>(m);
            break;

        case Mono::SHITE_ADD:
            m = new Mono;
            m->m_type = Mono::SHITE_ONLY;
            m->m_shite = mono->m_mono->m_shite;
            mono = shared_ptr<Mono>(m);
            break;

        default:
            Calc_H::s_message = "たしざんではありません。";
            break;
        }
        break;

    case Mono::MONO_NO_KAKERARERUKAZU:
        switch (mono->m_mono->m_type)
        {
        case Mono::EXPR_ONLY:
            ChAnalyzeMonoExprKakerarerukazu(mono, mono->m_mono->m_expr);
            break;

        case Mono::SURUTO_ONLY:
            ChAnalyzeMonoSurutoKakerarerukazu(mono, mono->m_mono->m_suruto);
            break;

        case Mono::EXPRLIST_MUL:
            {
                ExprList *el = mono->m_mono->m_exprlist.get();
                const std::size_t count = el->size();
                assert(count >= 2);
                shared_ptr<Expr> e = (*el)[0];
                m = new Mono;
                m->m_type = Mono::EXPR_ONLY;
                m->m_expr = e;
                mono = shared_ptr<Mono>(m);
            }
            break;
            
        case Mono::MONO_MUL:
        case Mono::MONO_TO_EXPRLIST_ADD:
        case Mono::MONO_TO_EXPR_MUL:
            m = new Mono;
            m->m_type = Mono::MONO_ONLY;
            m->m_mono = mono->m_mono->m_mono;
            mono = shared_ptr<Mono>(m);
            break;

        case Mono::SHITE_MUL:
            m = new Mono;
            m->m_type = Mono::SHITE_ONLY;
            m->m_shite = mono->m_mono->m_shite;
            mono = shared_ptr<Mono>(m);
            break;

        default:
            Calc_H::s_message = "かけざんではありません。";
            break;
        }
        break;

    case Mono::MONO_NO_HIKARERUKAZU:
        switch (mono->m_mono->m_type)
        {
        case Mono::EXPR_ONLY:
            ChAnalyzeMonoExprHikarerukazu(mono, mono->m_mono->m_expr);
            break;

        case Mono::SURUTO_ONLY:
            ChAnalyzeMonoSurutoHikarerukazu(mono, mono->m_mono->m_suruto);
            break;

        case Mono::EXPRLIST_SUB:
            {
                ExprList *el = mono->m_mono->m_exprlist.get();
                const std::size_t count = el->size();
                assert(count >= 2);
                shared_ptr<Expr> e = (*el)[0];
                m = new Mono;
                m->m_type = Mono::EXPR_ONLY;
                m->m_expr = e;
                mono = shared_ptr<Mono>(m);
            }
            break;
            
        case Mono::MONO_SUB:
        case Mono::MONO_TO_EXPR_SUB:
            m = new Mono;
            m->m_type = Mono::MONO_ONLY;
            m->m_mono = mono->m_mono->m_mono;
            mono = shared_ptr<Mono>(m);
            break;

        case Mono::MONO_WO_EXPR_KARA_SUB:
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = mono->m_mono->m_expr;
            mono = shared_ptr<Mono>(m);
            break;

        case Mono::SHITE_SUB:
            m = new Mono;
            m->m_type = Mono::SHITE_ONLY;
            m->m_shite = mono->m_mono->m_shite;
            mono = shared_ptr<Mono>(m);
            break;

        default:
            Calc_H::s_message = "ひきざんではありません。";
            break;
        }
        break;

    case Mono::MONO_NO_WARARERUKAZU:
        switch (mono->m_mono->m_type)
        {
        case Mono::EXPR_ONLY:
            ChAnalyzeMonoExprWararerukazu(mono, mono->m_mono->m_expr);
            break;

        case Mono::SURUTO_ONLY:
            ChAnalyzeMonoSurutoWararerukazu(mono, mono->m_mono->m_suruto);
            break;

        case Mono::EXPRLIST_DIV:
            {
                ExprList *el = mono->m_mono->m_exprlist.get();
                const std::size_t count = el->size();
                assert(count >= 2);
                shared_ptr<Expr> e = (*el)[0];
                m = new Mono;
                m->m_type = Mono::EXPR_ONLY;
                m->m_expr = e;
                mono = shared_ptr<Mono>(m);
            }
            break;
            
        case Mono::MONO_DIV:
        case Mono::MONO_TO_EXPR_DIV:
            m = new Mono;
            m->m_type = Mono::MONO_ONLY;
            m->m_mono = mono->m_mono->m_mono;
            mono = shared_ptr<Mono>(m);
            break;

        case Mono::MONO_DE_EXPR_WO_DIV:
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = mono->m_mono->m_expr;
            mono = shared_ptr<Mono>(m);
            break;

        case Mono::SHITE_DIV:
            m = new Mono;
            m->m_type = Mono::SHITE_ONLY;
            m->m_shite = mono->m_mono->m_shite;
            mono = shared_ptr<Mono>(m);
            break;

        default:
            Calc_H::s_message = "わりざんではありません。";
            break;
        }
        break;

    default:
        break;
    }
}

void ChAnalyzeExprList(shared_ptr<ExprList>& exprlist)
{
    ExprList *el = exprlist.get();
    ExprList::iterator it = el->begin();
    ExprList::iterator end = el->end();
    while (it != end)
    {
        ChAnalyzeExpr(*it);
        ++it;
    }
}

void ChAnalyzePrim(shared_ptr<Prim>& prim)
{
    switch (prim->m_type)
    {
    case Prim::MONO:
        ChAnalyzeMono(prim->m_mono);
        break;

    case Prim::MINUS:
    case Prim::BUNSUU:
    case Prim::TAIBUNSUU:
        ChAnalyzePrim(prim->m_prim);
        break;

    default:
        break;
    }
}

void ChAnalyzeFact(shared_ptr<Fact>& fact)
{
    switch (fact->m_type)
    {
    case Fact::POW:
        ChAnalyzeFact(fact->m_fact);
        ChAnalyzePrim(fact->m_prim);
        break;

    case Fact::POW2:
        ChAnalyzeFact(fact->m_fact);
        break;

    case Fact::SINGLE:
        ChAnalyzePrim(fact->m_prim);
        break;
    }
}

void ChAnalyzeTerm(shared_ptr<Term>& term)
{
    switch (term->m_type)
    {
    case Term::MUL:
    case Term::DIV:
        ChAnalyzeTerm(term->m_term);
        ChAnalyzeFact(term->m_fact);
        break;

    case Term::FACT_ONLY:
        ChAnalyzeFact(term->m_fact);
        break;
    }
}

void ChAnalyzeShite(shared_ptr<Shite>& shite)
{
    switch (shite->m_type)
    {
    case Shite::EXPRLIST_ADD:
    case Shite::EXPRLIST_MUL:
        ChAnalyzeExprList(shite->m_exprlist);
        break;

    case Shite::MONO_ADD:
    case Shite::MONO_MUL:
    case Shite::MONO_SUB:
    case Shite::MONO_DIV:
    case Shite::MONO_WO_EXPR_SUB:
    case Shite::MONO_WO_EXPR_DIV:
        ChAnalyzeMono(shite->m_mono);
        ChAnalyzeExpr(shite->m_expr);
        break;

    case Shite::MONO_ONLY:
        ChAnalyzeMono(shite->m_mono);
        break;

    case Shite::SHITE_ADD:
    case Shite::SHITE_MUL:
    case Shite::SHITE_SUB:
    case Shite::SHITE_DIV:
    case Shite::SHITE_EXPR_BAI:
        ChAnalyzeShite(shite->m_shite);
        ChAnalyzeExpr(shite->m_expr);
        break;

    case Shite::SHITE_BAI:
        ChAnalyzeShite(shite->m_shite);
        break;
    }
}

void ChAnalyzeExpr(shared_ptr<Expr>& expr)
{
    switch (expr->m_type)
    {
    case Expr::ADD:
    case Expr::SUB:
        ChAnalyzeExpr(expr->m_expr);
        ChAnalyzeTerm(expr->m_term);
        break;

    case Expr::TERM_ONLY:
        ChAnalyzeTerm(expr->m_term);
        break;
    }
}

void ChAnalyzeSuruto(shared_ptr<Suruto>& suruto)
{
    switch (suruto->m_type)
    {
    case Suruto::MONO_ADD:
    case Suruto::MONO_MUL:
    case Suruto::MONO_SUB:
    case Suruto::MONO_DIV:
    case Suruto::MONO_WO_EXPR_BAI:
    case Suruto::MONO_WO_EXPR_SUB:
    case Suruto::MONO_DE_EXPR_DIV:
        ChAnalyzeMono(suruto->m_mono);
        ChAnalyzeExpr(suruto->m_expr);
        break;

    case Suruto::MONO_ONLY:
    case Suruto::MONO_WO_BAI:
        ChAnalyzeMono(suruto->m_mono);
        break;

    case Suruto::SHITE_TASUTO:
    case Suruto::SHITE_KAKERUTO:
    case Suruto::SHITE_HIKUTO:
    case Suruto::SHITE_WARUTO:
        ChAnalyzeShite(suruto->m_shite);
        ChAnalyzeExpr(suruto->m_expr);
        break;

    default:
        break;
    }
}

void ChAnalyzeSentence(shared_ptr<Sentence>& sentence)
{
    switch (sentence->m_type)
    {
    case Sentence::MONO:
        ChAnalyzeMono(sentence->m_mono);
        break;

    case Sentence::EXPR:
        ChAnalyzeExpr(sentence->m_expr);
        break;

    case Sentence::SURUTO:
        ChAnalyzeSuruto(sentence->m_suruto);
        break;

    default:
        break;
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
                    str += "れいてん";
                str += jdigits;
            }
            else
            {
                str += "ぜろ";
            }
        }
        else
        {
            str += "ぜろ";
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
    ChReplaceString(str, "いちせん", "せん");
    ChReplaceString(str, "さんせん", "さんぜん");
    ChReplaceString(str, "はちせん", "はっせん");
    ChReplaceString(str, "いちちょう", "いっちょう");
    ChReplaceString(str, "いちけい", "いっけい");
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
        str += "まいなす";
    if (!seisuu.is_zero())
    {
        str += ChGetJpnNumberFixed(seisuu);
        str += "と";
    }
    str += ChGetJpnNumberFixed(bunbo);
    str += "ぶんの";
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

        const std::string zenkaku_space("　");
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

    sstream << "こたえ：";

    if (query == "exit" || query == "EXIT" ||
        query == "ｅｘｉｔ" || query == "ＥＸＩＴ" ||
        query == "quit" || query == "QUIT" ||
        query == "ｑｕｉｔ" || query == "ＱＵＩＴ" ||
        query.find("おわる") != std::string::npos ||
        query.find("おわり") != std::string::npos ||
        query.find("owaru") != std::string::npos ||
        query.find("owari") != std::string::npos)
    {
        sstream << "しゅうりょうします。" << std::endl;
        return sstream.str();
    }

    std::string error;
    query += "?";
    if (query.find("ありがと") != std::string::npos ||
        query.find("さんきゅ") != std::string::npos ||
        query.find("かんしゃ") != std::string::npos)
    {
        sstream << "こちらこそつかってくれてありがとう。" << std::endl;
    }
    else if (parse_string(sentence, query, error))
    {
        Calc_H::s_message.clear();
        ChAnalyzeSentence(sentence);
        if (Calc_H::s_message.empty())
        {
            pmp::Number::Type old_type =
                pmp::SetIntDivType(pmp::Number::FLOATING);
            try
            {
                CH_Value value = ChCalcSentence(sentence);
                value.trim();
                if (s_message.empty())
                {
                    sstream << ChGetJpnNumber2(value) <<
                        " (" << value.str() << ") " << "です。" << std::endl;
                    s_sore = value;
                }
                else
                {
                    sstream << Calc_H::s_message << std::endl;
                }
            }
            catch (const std::overflow_error&)
            {
                sstream << "おーばーふろーです。" << std::endl;
            }
            catch (const std::runtime_error&)
            {
                sstream << "けいさんできませんでした。" << std::endl;
            }
            pmp::SetIntDivType(old_type); // restore old
        }
        else
        {
            sstream << Calc_H::s_message << std::endl;
        }
    }
    else
    {
        if (error.empty())
        {
            sstream << "けいさんできませんでした。" << std::endl;
        }
        else
        {
            sstream << error << std::endl;
        }
    }

    return sstream.str();
}

////////////////////////////////////////////////////////////////////////////
