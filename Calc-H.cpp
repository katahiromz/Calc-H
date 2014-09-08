// Calc-H.cpp
// (Japanese, Shift_JIS)
// Linuxの場合は、UTF-8に変換して下さい。

#define DRR1D_USES_PMPNUMBER
#include "PmpNumber.hpp"
#include "Ndrr1D.hpp"
#include "Calc-H.h"
#include "HParseHeader.h"

namespace Calc_H
{
    CH_Value s_sore = 0;
    shared_ptr<Sentence> s_sentence_prev;
    std::string s_message;

    static const char *s_jdigits[] =
    {
        "ぜろ", "いち", "に", "さん", "よん",
        "ご", "ろく", "なな", "はち", "きゅう"
    };
}

void ChSetMessage(const std::string& str)
{
    if (Calc_H::s_message.empty())
        Calc_H::s_message = str;
}

static const char * const ch_not_tashizan = "たしざんではありません。";
static const char * const ch_not_kakezan = "かけざんではありません。";
static const char * const ch_not_hikizan = "ひきざんではありません。";
static const char * const ch_not_warizan = "わりざんではありません。";
static const char * const ch_right = "はい、そうです。";
static const char * const ch_wrong = "いいえ、ちがいます。";
static const char * const ch_indeterminate = "どちらともいえません。";

using namespace Calc_H;
using namespace Ndrr1D;

CH_Value ChInverse(const CH_Value& value)
{
    CH_Value result;
    switch (value.type())
    {
    case pmp::Number::INTEGER:
        result = pmp::Number(1, value.get_i());
        break;

    case pmp::Number::FLOATING:
        result = 1;
        result /= value;
        break;

    case pmp::Number::RATIONAL:
        result = pmp::Number(pmp::denominator(value), pmp::numerator(value));
        break;

#ifndef PMP_DISABLE_VECTOR
    case pmp::Number::VECTOR:
        {
            pmp::vector_type v;
            for (size_t i = 0; i < value.size(); ++i)
            {
                v.push_back(value[i]);
            }
            result = CH_Value(v);
        }
#endif  // ndef PMP_DISABLE_VECTOR
    }
    return result;
}

CH_Value ChCalcFactorial(const pmp::integer_type& value)
{
    pmp::integer_type i(1), result(1);
    for (; i <= value; ++i)
    {
        result *= i;
    }
    return result;
}

CH_Value ChGCD(const CH_Value& num)
{
    assert(num.is_v());
    const pmp::vector_type& vec = num.get_v();

    if (vec.size() == 0)
        return 0;

    for (size_t i = 1; i < vec.size(); ++i)
    {
        if (!(vec[i] % 1).is_zero())
        {
            ChSetMessage("けいさんたいしょうがせいすうではないので、けいさんできません。");
            return 0;
        }
    }

    Ndrr1D::integer_type v1 = vec[0].get_i(), v2;
    for (size_t i = 1; i < vec.size(); ++i)
    {
        v2 = vec[i].get_i();
        v1 = Ndrr1D::gcd(v1, v2);
    }
    return v1;
}

CH_Value ChLCM(const CH_Value& num)
{
    assert(num.is_v());
    const pmp::vector_type& vec = num.get_v();

    if (vec.size() == 0)
        return 0;

    for (size_t i = 1; i < vec.size(); ++i)
    {
        if (!(vec[i] % 1).is_zero())
        {
            ChSetMessage("けいさんたいしょうがせいすうではないので、けいさんできません。");
            return 0;
        }
    }

    Ndrr1D::integer_type v = vec[0].get_i();
    for (size_t i = 1; i < vec.size(); ++i)
    {
        v = Ndrr1D::lcm(v, vec[i].get_i());
    }
    return v;
}

void ChAnalyzeExpr(shared_ptr<Expr>& expr);
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
                ChSetMessage("ぶんぼがぜろになのでけいさんできません。");
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
                ChSetMessage("ぶんぼがぜろになのでけいさんできません。");
                return 0;
            }
            if (num.is_i() && denom.is_i())
                return seisuu + CH_Value(num, denom);
            else
                return seisuu + denom / num;
        }

    case Prim::FUNC1ARG:
    case Prim::FUNC1ARG_HEIHOU:
    case Prim::FUNC1ARG_RIPPOU:
    case Prim::FUNC1ARG_JOU:
        {
            CH_Value value = ChCalcPrim(prim->m_prim);
            switch (prim->m_func1arg->m_type)
            {
            case Func1Arg::SIN:
                value = pmp::sin(value);
                break;

            case Func1Arg::COS:
                value = pmp::cos(value);
                break;

            case Func1Arg::TAN:
                value = pmp::tan(value);
                break;

            case Func1Arg::ASIN:
                value = pmp::asin(value);
                break;

            case Func1Arg::ACOS:
                value = pmp::acos(value);
                break;

            case Func1Arg::ATAN:
                value = pmp::atan(value);
                break;

            case Func1Arg::SQRT:
                value = pmp::sqrt(value);
                break;

            case Func1Arg::ABS:
                value = pmp::abs(value);
                break;

            case Func1Arg::INV:
                value = ChInverse(value);
                break;

            case Func1Arg::EXP:
                value = pmp::exp(value);
                break;

            case Func1Arg::LOG:
                value = pmp::log(value);
                break;

            case Func1Arg::LOG10:
                value = pmp::log10(value);
                break;

            case Func1Arg::SINH:
                value = pmp::sinh(value);
                break;

            case Func1Arg::COSH:
                value = pmp::cosh(value);
                break;

            case Func1Arg::TANH:
                value = pmp::tanh(value);
                break;
            }
            if (prim->m_type == Prim::FUNC1ARG_HEIHOU)
                value *= value;
            else if (prim->m_type == Prim::FUNC1ARG_RIPPOU)
                value = value * value * value;
            else if (prim->m_type == Prim::FUNC1ARG_JOU)
                value = pmp::pow(value, prim->m_num->m_value);
            return value;
        }

    case Prim::DO:
        {
            CH_Value value = ChCalcPrim(prim->m_prim);
            value *= pmp::atan(1) / 45;
            return value;
        }

    case Prim::PI:
        return pmp::atan(1) * 4;

    case Prim::E:
        return pmp::exp(1);

    case Prim::FACTORIAL:
        {
            CH_Value value = ChCalcPrim(prim->m_prim);
            value.trim();
            if (value.is_i() && value.get_i() >= 0)
            {
                return ChCalcFactorial(value.get_i());
            }
            else
            {
                ChSetMessage("かいじょうがけいさんできるのはしぜんすうのみです。");
                return 0;
            }
        }

    default:
        assert(0);
        return 0;
    }
}

CH_Value 
ChPowFactPrim(const shared_ptr<Fact>& fact, const shared_ptr<Prim>& prim)
{
    CH_Value value = pmp::pow(ChCalcFact(fact), ChCalcPrim(prim));
    return value;
}

CH_Value ChCalcFact(const shared_ptr<Fact>& fact)
{
    CH_Value v1, v2;
    switch (fact->m_type)
    {
    case Fact::POW:
        assert(fact->m_fact);
        assert(fact->m_prim);
        return ChPowFactPrim(fact->m_fact, fact->m_prim);

    case Fact::POW2:
        assert(fact->m_fact);
        {
            CH_Value value = ChCalcFact(fact->m_fact);
            return value * value;
        }

    case Fact::POW3:
        assert(fact->m_fact);
        {
            CH_Value value = ChCalcFact(fact->m_fact);
            return value * value * value;
        }

    case Fact::KAIJOU:
        assert(fact->m_fact);
        {
            CH_Value value = ChCalcFact(fact->m_fact);
            if (value.is_i() && value.get_i() >= 0)
                value = ChCalcFactorial(value.get_i());
            else
                ChSetMessage("かいじょうがけいさんできるのはしぜんすうのみです。");
            return value;
        }

    case Fact::SINGLE:
        assert(fact->m_prim);
        return ChCalcPrim(fact->m_prim);

    case Fact::PERCENT:
        assert(fact->m_fact);
        assert(fact->m_prim);
        v1 = ChCalcFact(fact->m_fact);
        v2 = ChCalcPrim(fact->m_prim);
        v1 *= v2;
        v1 /= 100;
        return v1;

    case Fact::WARIBIKI:
        assert(fact->m_fact);
        assert(fact->m_prim);
        v1 = ChCalcFact(fact->m_fact);
        v2 = ChCalcPrim(fact->m_prim);
        v1 -= v1 * v2 / 10;
        return v1;

    case Fact::WARIMASHI:
        assert(fact->m_fact);
        assert(fact->m_prim);
        v1 = ChCalcFact(fact->m_fact);
        v2 = ChCalcPrim(fact->m_prim);
        v1 += v1 * v2 / 10;
        return v1;

    case Fact::HEIHOUKON:
        {
            v1 = pmp::sqrt(ChCalcFact(fact->m_fact));
            pmp::vector_type vec;
            vec.push_back(-v1);
            vec.push_back(v1);
            std::sort(vec.begin(), vec.end());
            std::unique(vec.begin(), vec.end());
            return CH_Value(vec);
        }

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

    case Expr::ZERO:
        return 0;

    default:
        assert(0);
        return 0;
    }
}

void ChNumberFromExprList(pmp::Number& num, shared_ptr<ExprList>& exprlist)
{
    assert(exprlist);
    pmp::vector_type vec;
    ExprList *el = exprlist.get();
    for (size_t i = 0; i < el->size(); ++i)
        vec.push_back(ChCalcExpr((*el)[i]));
    num.assign(vec);
}

CH_Value ChCalcMono(const shared_ptr<Mono>& mono)
{
    CH_Value v1, v2, v3;
    std::vector<CH_Value> values;
    pmp::Number num;
    pmp::vector_type vec;

    switch (mono->m_type)
    {
    case Mono::EXPRLIST_ADD:
        ChNumberFromExprList(v1, mono->m_exprlist);
        return pmp::sum(v1);

    case Mono::EXPRLIST_MUL:
        ChNumberFromExprList(v1, mono->m_exprlist);
        return pmp::prod(v1);

    case Mono::EXPRLIST_SUB:
        ChNumberFromExprList(v1, mono->m_exprlist);
        return pmp::diff(v1);

    case Mono::EXPRLIST_DIV:
        ChNumberFromExprList(v1, mono->m_exprlist);
        return pmp::quot(v1);

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
        ChNumberFromExprList(v1, mono->m_exprlist);
        return ChCalcMono(mono->m_mono) + pmp::sum(v1);

    case Mono::MONO_TO_EXPRLIST_MUL:
        ChNumberFromExprList(v1, mono->m_exprlist);
        return ChCalcMono(mono->m_mono) * pmp::prod(v1);
        
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

    case Mono::MONO_FUNC1ARG:
        v1 = ChCalcMono(mono->m_mono);
        switch (mono->m_func1arg->m_type)
        {
        case Func1Arg::SIN:
            return pmp::sin(v1);

        case Func1Arg::COS:
            return pmp::cos(v1);

        case Func1Arg::TAN:
            return pmp::tan(v1);

        case Func1Arg::ASIN:
            return pmp::asin(v1);

        case Func1Arg::ACOS:
            return pmp::acos(v1);

        case Func1Arg::ATAN:
            return pmp::atan(v1);

        case Func1Arg::SQRT:
            return pmp::sqrt(v1);

        case Func1Arg::ABS:
            return pmp::abs(v1);

        case Func1Arg::INV:
            return ChInverse(v1);

        case Func1Arg::EXP:
            return pmp::exp(v1);

        case Func1Arg::LOG:
            return pmp::log(v1);

        case Func1Arg::LOG10:
            return pmp::log10(v1);

        case Func1Arg::SINH:
            return pmp::sinh(v1);

        case Func1Arg::COSH:
            return pmp::cosh(v1);

        case Func1Arg::TANH:
            return pmp::tanh(v1);

        default:
            assert(0);
            return 0;
        }

    case Mono::SHITE_EXPR_JOU:
        v1 = ChCalcShite(mono->m_shite);
        v2 = ChCalcExpr(mono->m_expr);
        return pmp::pow(v1, v2);

    case Mono::SHITE_HEIHOU:
        v1 = ChCalcShite(mono->m_shite);
        return v1 * v1;

    case Mono::SHITE_RIPPOU:
        v1 = ChCalcShite(mono->m_shite);
        return v1 * v1 * v1;

    case Mono::MONO_EXPR_JOU:
        v1 = ChCalcMono(mono->m_mono);
        v2 = ChCalcExpr(mono->m_expr);
        return pmp::pow(v1, v2);

    case Mono::MONO_HEIHOU:
        v1 = ChCalcMono(mono->m_mono);
        return v1 * v1;

    case Mono::MONO_RIPPOU:
        v1 = ChCalcMono(mono->m_mono);
        return v1 * v1 * v1;

    case Mono::MONO_NO_EXPR_PERCENT:
        v1 = ChCalcMono(mono->m_mono);
        v2 = ChCalcExpr(mono->m_expr);
        v1 *= v2;
        v1 /= 100;
        return v1;

    case Mono::MONO_MOD_EXPR:
        v1 = ChCalcMono(mono->m_mono);
        v2 = ChCalcExpr(mono->m_expr);
        v1 %= v2;
        return v1;

    case Mono::EXPR_MOD_MONO:
        v1 = ChCalcExpr(mono->m_expr);
        v2 = ChCalcMono(mono->m_mono);
        v1 %= v2;
        return v1;

    case Mono::SHITE_MOD_EXPR:
        v1 = ChCalcShite(mono->m_shite);
        v2 = ChCalcMono(mono->m_mono);
        v1 %= v2;
        return v1;

    case Mono::TERM_MOD_FACT:
        v1 = ChCalcTerm(mono->m_term);
        v2 = ChCalcFact(mono->m_fact);
        v1 %= v2;
        return v1;

    case Mono::SHOUSUU:
        v1 = ChCalcShite(mono->m_shite);
        v1 += 0.0;
        return v1;

    case Mono::MONO_NO_SHOUSUU:
        v1 = ChCalcMono(mono->m_mono);
        v1 += 0.0;
        return v1;

    case Mono::EXPRLIST_VECFUNC:
        {
            pmp::Number num;
            switch (mono->m_vecfunc->m_type)
            {
            case VecFunc::COUNT:
                ChNumberFromExprList(num, mono->m_exprlist);
                return pmp::count(num);

            case VecFunc::MAX:
                ChNumberFromExprList(num, mono->m_exprlist);
                return pmp::max(num);

            case VecFunc::MIN:
                ChNumberFromExprList(num, mono->m_exprlist);
                return pmp::min(num);

            case VecFunc::AVERAGE:
                ChNumberFromExprList(num, mono->m_exprlist);
                return pmp::average(num);

            case VecFunc::GCD:
                ChNumberFromExprList(num, mono->m_exprlist);
                return ChGCD(num);

            case VecFunc::LCM:
                ChNumberFromExprList(num, mono->m_exprlist);
                return ChLCM(num);

            default:
                assert(0);
            }
        }
        return 0;

    case Mono::MONO_DIVMOD_EXPR:
        {
            pmp::vector_type vec;
            v1 = ChCalcMono(mono->m_mono);
            v2 = ChCalcExpr(mono->m_expr);
            vec.push_back((v1 / v2).to_i());
            vec.push_back(v1 % v2);
            return pmp::Number(vec);
        }

    case Mono::SHITE_DIVMOD_EXPR:
        {
            pmp::vector_type vec;
            v1 = ChCalcShite(mono->m_shite);
            v2 = ChCalcExpr(mono->m_expr);
            vec.push_back((v1 / v2).to_i());
            vec.push_back(v1 % v2);
            return pmp::Number(vec);
        }

    case Mono::EXPR_DIVMOD_MONO:
        {
            pmp::vector_type vec;
            v1 = ChCalcExpr(mono->m_expr);
            v2 = ChCalcMono(mono->m_mono);
            vec.push_back((v1 / v2).to_i());
            vec.push_back(v1 % v2);
            return pmp::Number(vec);
        }

    case Mono::TERM_DIVMOD_FACT:
        {
            pmp::vector_type vec;
            v1 = ChCalcTerm(mono->m_term);
            v2 = ChCalcFact(mono->m_fact);
            vec.push_back((v1 / v2).to_i());
            vec.push_back(v1 % v2);
            return pmp::Number(vec);
        }

    case Mono::MONO_HEIHOUKON:
        {
            pmp::vector_type vec;
            v2 = ChCalcMono(mono->m_mono);
            v1 = pmp::sqrt(v2);
            vec.push_back(-v1);
            vec.push_back(v1);
            return pmp::Number(vec);
        }

    case Mono::MONO_KAIJOU:
        v1 = ChCalcMono(mono->m_mono);
        v1.trim();
        if (v1.is_i() && v1.get_i() >= 0)
            v2 = ChCalcFactorial(v1.get_i());
        else
            ChSetMessage("かいじょうがけいさんできるのはしぜんすうのみです。");
        return v2;

    case Mono::DOMS_SUM:
        assert(mono->m_doms);
        assert(mono->m_doms->m_domains);
        {
            Domains& domains = *mono->m_doms->m_domains.get();
            domains.FixNarrower();
            if (domains.size() == 1)
            {
                Domain& domain = *domains[0].get();
                if (domain.m_aspect.m_pnModulus)
                {
                    bool has_min, has_max;
                    CH_Value *pnLBound = domain.GetLBound(has_min);
                    CH_Value *pnUBound = domain.GetUBound(has_max);
                    if (pnLBound && pnUBound && has_min && has_max &&
                        *pnLBound <= *pnUBound)
                    {
                        v1 = *pnLBound;
                        v2 = *pnUBound;
                        v3 = (v2 - v1) / *domain.m_aspect.m_pnModulus + 1;
                        return v3 * (v1 + v2) / 2;
                    }
                }
            }
            if (domains.GetValues(values))
            {
                v1 = 0;
                size_t i, siz = values.size();
                for (i = 0; i < siz; ++i)
                {
                    v1 += values[i];
                }
                return v1;
            }
            else
            {
                ChSetMessage("けいさんできませんでした。");
                return 0;
            }
        }

    case Mono::DOMS_PROD:
        assert(mono->m_doms);
        assert(mono->m_doms->m_domains);
        if (mono->m_doms->m_domains->GetValues(values))
        {
            v1 = 1;
            size_t i, siz = values.size();
            for (i = 0; i < siz; ++i)
            {
                v1 *= values[i];
            }
            return v1;
        }
        else
        {
            ChSetMessage("けいさんできませんでした。");
            return 1;
        }

    case Mono::MONO_EXPR_WARIBIKI:
        assert(mono->m_mono);
        assert(mono->m_expr);
        v1 = ChCalcMono(mono->m_mono);
        v2 = ChCalcExpr(mono->m_expr);
        v1 -= v1 * v2 / 10;
        return v1;

    case Mono::MONO_EXPR_WARIMASHI:
        assert(mono->m_mono);
        assert(mono->m_expr);
        v1 = ChCalcMono(mono->m_mono);
        v2 = ChCalcExpr(mono->m_expr);
        v1 += v1 * v2 / 10;
        return v1;

    case Mono::EXPRLIST_NOUCHI_DOMS:
        ChNumberFromExprList(num, mono->m_exprlist);
        num.trim();
        assert(mono->m_doms->m_domains);
        for (size_t i = 0; i < num.size(); ++i)
        {
            if (mono->m_doms->m_domains->Contains(num[i]))
            {
                vec.push_back(num[i]);
            }
        }
        return pmp::Number(vec);

    default:
        assert(0);
    }
    return 0;
}

CH_Value ChCalcShite(const shared_ptr<Shite>& shite)
{
    CH_Value v1, v2;
    switch (shite->m_type)
    {
    case Shite::EXPRLIST_ADD:
        ChNumberFromExprList(v1, shite->m_exprlist);
        return pmp::sum(v1);

    case Shite::EXPRLIST_MUL:
        ChNumberFromExprList(v1, shite->m_exprlist);
        return pmp::prod(v1);

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

    case Shite::SHITE_EXPR_JOU:
        v1 = ChCalcShite(shite->m_shite);
        v2 = ChCalcExpr(shite->m_expr);
        return pmp::pow(v1, v2);

    case Shite::SHITE_HEIHOU:
        v1 = ChCalcShite(shite->m_shite);
        return v1 * v1;

    case Shite::SHITE_RIPPOU:
        v1 = ChCalcShite(shite->m_shite);
        return v1 * v1 * v1;

    case Shite::SHOUSUU:
        v1 = ChCalcShite(shite->m_shite);
        v1 += 0.0;
        return v1;

    default:
        assert(0);
        return 0;
    }
}

CH_Value ChCalcSuruto(const shared_ptr<Suruto>& suruto)
{
    CH_Value v1, v2;
    switch (suruto->m_type)
    {
    case Suruto::EXPRLIST_ADD:
        ChNumberFromExprList(v1, suruto->m_exprlist);
        return pmp::sum(v1);

    case Suruto::EXPRLIST_MUL:
        ChNumberFromExprList(v1, suruto->m_exprlist);
        return pmp::prod(v1);

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

    case Suruto::MONO_WO_EXPR_JOU:
        v1 = ChCalcMono(suruto->m_mono);
        v2 = ChCalcExpr(suruto->m_expr);
        return pmp::pow(v1, v2);

    case Suruto::MONO_WO_HEIHOU:
        v1 = ChCalcMono(suruto->m_mono);
        return v1 * v1;

    case Suruto::MONO_WO_RIPPOU:
        v1 = ChCalcMono(suruto->m_mono);
        return v1 * v1 * v1;

    case Suruto::SHOUSUU:
        v1 = ChCalcShite(suruto->m_shite);
        v1 += 0.0;
        return v1;

    case Suruto::MONO_WO_SHOUSUU:
        v1 = ChCalcMono(suruto->m_mono);
        v1 += 0.0;
        return v1;

    default:
        assert(0);
        return 0;
    }
}

void ChAnalyzeMono(shared_ptr<Mono>& mono);

bool ChIsMonoTashizan(shared_ptr<Mono>& mono);
bool ChIsMonoKakezan(shared_ptr<Mono>& mono);
bool ChIsMonoHikizan(shared_ptr<Mono>& mono);
bool ChIsMonoWarizan(shared_ptr<Mono>& mono);

bool ChIsSurutoTashizan(shared_ptr<Suruto>& suruto);
bool ChIsSurutoHikizan(shared_ptr<Suruto>& suruto);
bool ChIsSurutoKakezan(shared_ptr<Suruto>& suruto);
bool ChIsSurutoWarizan(shared_ptr<Suruto>& suruto);

CH_Value ChCalcSentence(const shared_ptr<Sentence>& sentence)
{
    CH_Value v1, v2;
    std::vector<CH_Value> values;

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
        ChNumberFromExprList(v1, sentence->m_exprlist);
        return pmp::sum(v1);

    case Sentence::EXPRLIST_MUL:
        ChNumberFromExprList(v1, sentence->m_exprlist);
        return pmp::prod(v1);

    case Sentence::SHITE:
        return ChCalcShite(sentence->m_shite);

    case Sentence::DOMS_IS_DOMS:
        assert(sentence->m_doms1);
        assert(sentence->m_doms1->m_domains);
        assert(sentence->m_doms2);
        assert(sentence->m_doms2->m_domains);
        sentence->m_doms2->m_domains.get()->FixBeforeIncludes(
            *sentence->m_doms1->m_domains.get());
        #ifdef _DEBUG
            std::cout << *sentence->m_doms1->m_domains.get() << std::endl;
            std::cout << *sentence->m_doms2->m_domains.get() << std::endl;
        #endif
        if (sentence->m_doms2->m_domains->Includes(*sentence->m_doms1->m_domains.get()))
        {
            ChSetMessage(ch_right);
        }
        else
        {
            if (sentence->m_doms1->m_domains->GetValues(values))
            {
                bool flag = true;
                size_t i, siz = values.size();
                for (i = 0; i < siz; ++i)
                {
                    if (!sentence->m_doms2->m_domains->Contains(values[i]))
                    {
                        flag = false;
                        break;
                    }
                }
                if (flag)
                    ChSetMessage(ch_right);
                else
                    ChSetMessage(ch_wrong);
            }
            else
            {
                Domains *d = Domains::Intersect(
                    sentence->m_doms2->m_domains.get(),
                    sentence->m_doms1->m_domains.get());
                if (d->empty())
                    ChSetMessage(ch_wrong);
                else
                    ChSetMessage(ch_indeterminate);
                delete d;
            }
        }
        break;

    case Sentence::DOMS_IS_CNSTR:
        assert(sentence->m_doms1);
        assert(sentence->m_doms1->m_domains);
        assert(sentence->m_cnstr);
        assert(sentence->m_cnstr->m_domains);
        sentence->m_cnstr->m_domains->FixBeforeIncludes(
            *sentence->m_doms1->m_domains.get());
        if (sentence->m_cnstr->m_domains->Includes(*sentence->m_doms1->m_domains.get()))
        {
            ChSetMessage(ch_right);
        }
        else
        {
            if (sentence->m_doms1->m_domains->GetValues(values))
            {
                bool flag = true;
                size_t i, siz = values.size();
                for (i = 0; i < siz; ++i)
                {
                    if (!sentence->m_cnstr->m_domains->Contains(values[i]))
                    {
                        flag = false;
                        break;
                    }
                }
                if (flag)
                    ChSetMessage(ch_right);
                else
                    ChSetMessage(ch_wrong);
            }
            else
            {
                Domains *d = Domains::Intersect(
                    sentence->m_cnstr->m_domains.get(),
                    sentence->m_doms1->m_domains.get());
                if (d->empty())
                    ChSetMessage(ch_wrong);
                else
                    ChSetMessage(ch_indeterminate);
                delete d;
            }
        }
        break;

    case Sentence::MONO_IS_DOMS:
        assert(sentence->m_mono);
        assert(sentence->m_doms2);
        assert(sentence->m_doms2->m_domains);
        ChAnalyzeMono(sentence->m_mono);
        v1 = ChCalcMono(sentence->m_mono);
        v1.trim();
        if (sentence->m_doms2->m_domains->Contains(v1))
            ChSetMessage(ch_right);
        else
            ChSetMessage(ch_wrong);
        break;

    case Sentence::MONO_IS_CNSTR:
        assert(sentence->m_mono);
        assert(sentence->m_cnstr);
        assert(sentence->m_cnstr->m_domains);
        ChAnalyzeMono(sentence->m_mono);
        v1 = ChCalcMono(sentence->m_mono);
        v1.trim();
        if (sentence->m_cnstr->m_domains->Contains(v1))
            ChSetMessage(ch_right);
        else
            ChSetMessage(ch_wrong);
        break;

    case Sentence::MONO_IS_CNSTRED_BUNSUU:
        assert(sentence->m_mono);
        assert(sentence->m_cnstr);
        assert(sentence->m_cnstr->m_domains);
        ChAnalyzeMono(sentence->m_mono);
        if (ChCalcMono(sentence->m_mono).is_r() &&
            sentence->m_cnstr->m_domains->Contains(ChCalcMono(sentence->m_mono)))
            ChSetMessage(ch_right);
        else
            ChSetMessage(ch_wrong);
        break;

    case Sentence::MONO_IS_BUNSUU:
        assert(sentence->m_mono);
        ChAnalyzeMono(sentence->m_mono);
        if (ChCalcMono(sentence->m_mono).is_r())
            ChSetMessage(ch_right);
        else
            ChSetMessage(ch_wrong);
        break;

    case Sentence::MONO_IS_CNSTRED_SHOUSUU:
        assert(sentence->m_mono);
        assert(sentence->m_cnstr);
        assert(sentence->m_cnstr->m_domains);
        ChAnalyzeMono(sentence->m_mono);
        if (ChCalcMono(sentence->m_mono).is_f() &&
            sentence->m_cnstr->m_domains->Contains(ChCalcMono(sentence->m_mono)))
            ChSetMessage(ch_right);
        else
            ChSetMessage(ch_wrong);
        break;

    case Sentence::MONO_IS_SHOUSUU:
        assert(sentence->m_mono);
        ChAnalyzeMono(sentence->m_mono);
        if (ChCalcMono(sentence->m_mono).is_f())
            ChSetMessage(ch_right);
        else
            ChSetMessage(ch_wrong);
        break;

    case Sentence::MONO_IS_MONO:
        assert(sentence->m_mono);
        ChAnalyzeMono(sentence->m_mono);
        assert(sentence->m_mono2);
        ChAnalyzeMono(sentence->m_mono2);
        v1 = ChCalcMono(sentence->m_mono);
        v2 = ChCalcMono(sentence->m_mono2);
        v1.trim();
        v2.trim();
        if (v1 == v2)
            ChSetMessage(ch_right);
        else
            ChSetMessage(ch_wrong);
        break;

    case Sentence::MONO_IS_TASHIZAN:
        if (ChIsMonoTashizan(sentence->m_mono))
            ChSetMessage(ch_right);
        else
            ChSetMessage(ch_wrong);
        break;

    case Sentence::MONO_IS_HIKIZAN:
        if (ChIsMonoHikizan(sentence->m_mono))
            ChSetMessage(ch_right);
        else
            ChSetMessage(ch_wrong);
        break;

    case Sentence::MONO_IS_KAKEZAN:
        if (ChIsMonoKakezan(sentence->m_mono))
            ChSetMessage(ch_right);
        else
            ChSetMessage(ch_wrong);
        break;

    case Sentence::MONO_IS_WARIZAN:
        if (ChIsMonoWarizan(sentence->m_mono))
            ChSetMessage(ch_right);
        else
            ChSetMessage(ch_wrong);
        break;

    case Sentence::SURUTO_TASHIZAN:
        if (ChIsSurutoTashizan(sentence->m_suruto))
            ChSetMessage(ch_right);
        else
            ChSetMessage(ch_wrong);
        break;

    case Sentence::SURUTO_HIKIZAN:
        if (ChIsSurutoHikizan(sentence->m_suruto))
            ChSetMessage(ch_right);
        else
            ChSetMessage(ch_wrong);
        break;

    case Sentence::SURUTO_KAKEZAN:
        if (ChIsSurutoKakezan(sentence->m_suruto))
            ChSetMessage(ch_right);
        else
            ChSetMessage(ch_wrong);
        break;

    case Sentence::SURUTO_WARIZAN:
        if (ChIsSurutoWarizan(sentence->m_suruto))
            ChSetMessage(ch_right);
        else
            ChSetMessage(ch_wrong);
        break;

    case Sentence::DOMS_IS_EXPRLIST:
        assert(sentence->m_doms1);
        assert(sentence->m_doms1->m_domains);
        assert(sentence->m_exprlist);
        if (sentence->m_doms1->m_domains->GetValues(values))
        {
            ChNumberFromExprList(v1, sentence->m_exprlist);
            pmp::vector_type& v = v1.get_v();
            if (v.size() == values.size())
            {
                bool flag = true;
                for (size_t j = 0; j < values.size(); ++j)
                {
                    flag = false;
                    values[j].trim();
                    for (size_t i = 0; i < v.size(); ++i)
                    {
                        v[i].trim();
                        if (values[j] == v[i])
                        {
                            flag = true;
                            break;
                        }
                    }
                    if (!flag)
                        break;
                }
                if (flag)
                {
                    for (size_t i = 0; i < v.size(); ++i)
                    {
                        flag = false;
                        for (size_t j = 0; j < values.size(); ++j)
                        {
                            if (values[j] == v[i])
                            {
                                flag = true;
                                break;
                            }
                        }
                        if (!flag)
                            break;
                    }
                }
                if (flag)
                    ChSetMessage(ch_right);
                else
                    ChSetMessage(ch_wrong);
            }
            else
                ChSetMessage(ch_wrong);
        }
        else
            ChSetMessage(ch_wrong);
        break;

    case Sentence::DOMS_IS_EXPR:
        assert(sentence->m_doms1);
        assert(sentence->m_doms1->m_domains);
        assert(sentence->m_expr);
        if (sentence->m_doms1->m_domains->GetValues(values))
        {
            v1 = ChCalcExpr(sentence->m_expr);
            v1.trim();
            values[0].trim();
            if (values.size() == 1 && v1 == values[0])
                ChSetMessage(ch_right);
            else
                ChSetMessage(ch_wrong);
        }
        else
            ChSetMessage(ch_wrong);
        break;

    case Sentence::DOMS_IS_WHAT:
        assert(sentence->m_doms1);
        assert(sentence->m_doms1->m_domains);
        if (sentence->m_doms1->m_domains->GetValues(values))
        {
            for (size_t i = 0; i < values.size(); ++i)
                values[i].trim();
            return CH_Value(values);
        }
        else
            ChSetMessage("むすうにあります。");
        break;

    default:
        assert(0);
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////

void ChAnalyzeDomainsOfPrimDom(
    shared_ptr<Domains>& domains, shared_ptr<PrimDom>& primdom)
{
    assert(domains);
    assert(primdom);
    Domain *d = NULL;
    switch (primdom->m_type)
    {
    case PrimDom::POSITIVE:
        d = Domain::Whole(Domain::POSITIVE);
        domains.get()->Intersect(*d);
        assert(primdom->m_primdom);
        ChAnalyzeDomainsOfPrimDom(domains, primdom->m_primdom);
        break;

    case PrimDom::NEGATIVE:
        d = Domain::Whole(Domain::NEGATIVE);
        domains.get()->Intersect(*d);
        assert(primdom->m_primdom);
        ChAnalyzeDomainsOfPrimDom(domains, primdom->m_primdom);
        break;

    case PrimDom::SHIZENSUU:
        d = Domain::Whole(Domain::NATURAL);
        domains.get()->Intersect(*d);
        break;

    case PrimDom::SEISUU:
        d = Domain::Whole(Domain::REGULAR);
        domains.get()->Intersect(*d);
        break;

    case PrimDom::GUUSUU:
        d = Domain::Whole(Domain::EVEN);
        domains.get()->Intersect(*d);
        break;

    case PrimDom::KISUU:
        d = Domain::Whole(Domain::ODD);
        domains.get()->Intersect(*d);
        break;

    case PrimDom::JISSUU:
        break;

    case PrimDom::SOSUU:
        d = Domain::Whole(Domain::PRIME);
        domains.get()->Intersect(*d);
        break;

    case PrimDom::BAISUU:
        {
            CH_Value v = ChCalcNum(primdom->m_num);
            v.trim();
            if (v.is_i())
            {
                if (v < 0)
                    v = -v;

                if (v.is_zero())
                {
                    domains.get()->Intersect(Domain(CH_Value(0)));
                }
                else
                {
                    integer_type i1 = v.get_i();
                    integer_type i2 = 0;
                    domains.get()->Intersect(Aspect(&i1, &i2));
                }
            }
            else
            {
                ChSetMessage("せいすうではないかずのばいすうはていぎされていません。");
            }
        }
        break;

    case PrimDom::YAKUSUU:
        {
            CH_Value v = ChCalcNum(primdom->m_num);
            v.trim();
            if (v.is_i())
            {
                if (v < 0)
                    v = -v;

                if (v.is_zero())
                {
                    ChSetMessage("ぜろのやくすうはていぎされていません。");
                }
                else
                {
                    Domains d;
                    for (CH_Value n = 1; n <= v; n += 1)
                    {
                        if ((v % n).is_zero())
                        {
                            d.Union(Domain(n));
                            d.Union(Domain(-n));
                        }
                    }
                    domains.get()->Intersect(d);
                }
            }
            else
            {
                ChSetMessage("せいすうではないかずのやくすうはていぎされていません。");
            }
        }
        break;

    default:
        assert(0);
    }
    delete d;
}

void ChAnalyzeDomainsOfPrimCnstr(
    shared_ptr<Domains>& domains,
    shared_ptr<PrimCnstr>& primcnstr)
{
    assert(domains);
    assert(primcnstr);
    CH_Value v;
    Range *r = NULL;
    switch (primcnstr->m_type)
    {
    case PrimCnstr::IJOU:
        ChAnalyzeExpr(primcnstr->m_expr);
        v = ChCalcExpr(primcnstr->m_expr);
        v.trim();
        r = new Range(true, false, &v, NULL);
        domains.get()->Intersect(*r);
        break;

    case PrimCnstr::IKA:
        ChAnalyzeExpr(primcnstr->m_expr);
        v = ChCalcExpr(primcnstr->m_expr);
        v.trim();
        r = new Range(false, true, NULL, &v);
        domains.get()->Intersect(*r);
        break;

    case PrimCnstr::CHIISAI:
        ChAnalyzeExpr(primcnstr->m_expr);
        v = ChCalcExpr(primcnstr->m_expr);
        v.trim();
        r = new Range(false, false, NULL, &v);
        domains.get()->Intersect(*r);
        break;

    case PrimCnstr::OOKII:
        ChAnalyzeExpr(primcnstr->m_expr);
        v = ChCalcExpr(primcnstr->m_expr);
        v.trim();
        r = new Range(false, false, &v, NULL);
        domains.get()->Intersect(*r);
        break;

    case PrimCnstr::WARIKIRU:
        ChAnalyzeMono(primcnstr->m_mono);
        v = ChCalcMono(primcnstr->m_mono);
        v.trim();
        if (v.is_i())
        {
            if (v < 0)
                v = -v;
            if (v.is_zero())
            {
                ChSetMessage("けいさんできません。");
            }
            else
            {
                Ndrr1D::integer_type i1(v.get_i());
                Ndrr1D::integer_type i2(0);
                domains.get()->Intersect(Aspect(&i1, &i2));
            }
        }
        else
        {
            ChSetMessage("わるかずがせいすうではありません。");
        }
        break;

    case PrimCnstr::HITOSHII:
        ChAnalyzeExpr(primcnstr->m_expr);
        v = ChCalcExpr(primcnstr->m_expr);
        v.trim();
        domains.get()->Intersect(Domain(v));
        break;

    default:
        assert(0);
    }
    delete r;
}

void ChAnalyzeDomainsOfCnstr(
    shared_ptr<Domains>& domains,
    shared_ptr<Cnstr>& cnstr);

void ChAnalyzeDomainsOfAndCnstr(
    shared_ptr<Domains>& domains,
    shared_ptr<AndCnstr>& andcnstr)
{
    assert(domains);
    assert(andcnstr);
    Range *r = NULL;
    CH_Value v;
    switch (andcnstr->m_type)
    {
    case AndCnstr::IJOU:
        ChAnalyzeExpr(andcnstr->m_expr);
        v = ChCalcExpr(andcnstr->m_expr);
        v.trim();
        r = new Range(true, false, &v, NULL);
        domains.get()->Intersect(*r);
        ChAnalyzeDomainsOfAndCnstr(domains, andcnstr->m_andcnstr);
        break;

    case AndCnstr::IKA:
        ChAnalyzeExpr(andcnstr->m_expr);
        v = ChCalcExpr(andcnstr->m_expr);
        v.trim();
        r = new Range(false, true, NULL, &v);
        domains.get()->Intersect(*r);
        ChAnalyzeDomainsOfAndCnstr(domains, andcnstr->m_andcnstr);
        break;

    case AndCnstr::CHIISAI:
        ChAnalyzeExpr(andcnstr->m_expr);
        v = ChCalcExpr(andcnstr->m_expr);
        v.trim();
        r = new Range(false, false, NULL, &v);
        domains.get()->Intersect(*r);
        ChAnalyzeDomainsOfAndCnstr(domains, andcnstr->m_andcnstr);
        break;

    case AndCnstr::OOKII:
        ChAnalyzeExpr(andcnstr->m_expr);
        v = ChCalcExpr(andcnstr->m_expr);
        v.trim();
        r = new Range(false, false, &v, NULL);
        domains.get()->Intersect(*r);
        ChAnalyzeDomainsOfAndCnstr(domains, andcnstr->m_andcnstr);
        break;

    case AndCnstr::CNSTR_ONLY:
        ChAnalyzeDomainsOfCnstr(domains, andcnstr->m_cnstr);
        break;

    case AndCnstr::PRIMCNSTR_ONLY:
        ChAnalyzeDomainsOfPrimCnstr(domains, andcnstr->m_primcnstr);
        break;

    case AndCnstr::HITOSHIKU:
        ChAnalyzeExpr(andcnstr->m_expr);
        v = ChCalcExpr(andcnstr->m_expr);
        v.trim();
        domains.get()->Intersect(Domain(v));
        ChAnalyzeDomainsOfAndCnstr(domains, andcnstr->m_andcnstr);
        break;

    default:
        assert(0);
    }
    delete r;
}

void ChAnalyzeDomainsOfCnstr(
    shared_ptr<Domains>& domains,
    shared_ptr<Cnstr>& cnstr)
{
    assert(domains);
    assert(cnstr);
    Domains *d1, *d2;
    shared_ptr<Domains> domains1, domains2;
    switch (cnstr->m_type)
    {
    case Cnstr::MATAHA:
        d1 = Domains::Whole();
        domains1 = shared_ptr<Domains>(d1);
        d2 = Domains::Whole();
        domains2 = shared_ptr<Domains>(d2);
        ChAnalyzeDomainsOfCnstr(domains1, cnstr->m_cnstr);
        ChAnalyzeDomainsOfAndCnstr(domains2, cnstr->m_andcnstr);
        d1->Union(*d2);
        domains2 = shared_ptr<Domains>();
        domains = domains1;
        break;

    case Cnstr::SINGLE:
        ChAnalyzeDomainsOfAndCnstr(domains, cnstr->m_andcnstr);
        break;

    default:
        assert(0);
    }
}

void ChAnalyzeDomainsOfCnstredPrimDom(
    shared_ptr<Domains>& domains,
    shared_ptr<Cnstr>& cnstr,
    shared_ptr<PrimDom>& primdom)
{
    assert(domains);
    assert(cnstr);
    assert(primdom);
    ChAnalyzeDomainsOfCnstr(domains, cnstr);
    ChAnalyzeDomainsOfPrimDom(domains, primdom);
}

////////////////////////////////////////////////////////////////////////////

void ChAnalyzeNum(shared_ptr<Num>& num)
{
    assert(num);
}

void ChAnalyzeAndCnstr(shared_ptr<AndCnstr>& andcnstr)
{
    assert(andcnstr);
    switch (andcnstr->m_type)
    {
    case AndCnstr::IJOU:
    case AndCnstr::IKA:
    case AndCnstr::CHIISAI:
    case AndCnstr::OOKII:
    case AndCnstr::CNSTR_ONLY:
    case AndCnstr::PRIMCNSTR_ONLY:
    case AndCnstr::HITOSHIKU:
        ChAnalyzeExpr(andcnstr->m_expr);
        ChAnalyzeAndCnstr(andcnstr->m_andcnstr);
        break;

    default:
        assert(0);
    }
}

void ChAnalyzeCnstr(shared_ptr<Cnstr>& cnstr)
{
    assert(cnstr);
    if (!cnstr->m_domains)
    {
        Domains *d = Domains::Whole();
        cnstr->m_domains = shared_ptr<Domains>(d);
    }

    switch (cnstr->m_type)
    {
    case Cnstr::MATAHA:
        ChAnalyzeDomainsOfCnstr(cnstr->m_domains, cnstr);
        break;

    case Cnstr::SINGLE:
        ChAnalyzeDomainsOfAndCnstr(cnstr->m_domains, cnstr->m_andcnstr);
        break;

    default:
        assert(0);
    }
}

void ChAnalyzePrimDom(shared_ptr<PrimDom>& primdom)
{
    assert(primdom);
    switch (primdom->m_type)
    {
    case PrimDom::POSITIVE:
        ChAnalyzePrimDom(primdom->m_primdom);
        break;

    case PrimDom::NEGATIVE:
        ChAnalyzePrimDom(primdom->m_primdom);
        break;

    case PrimDom::SHIZENSUU:
    case PrimDom::SEISUU:
    case PrimDom::GUUSUU:
    case PrimDom::KISUU:
    case PrimDom::JISSUU:
    case PrimDom::BAISUU:
    case PrimDom::YAKUSUU:
        break;

    default:
        assert(0);
    }
}

void ChAnalyzeDomainsOfNum(shared_ptr<Domains>& domains, shared_ptr<Num>& num)
{
    assert(domains);
    Range *range = new Range(num->m_value);
    domains.get()->Intersect(*range);
    delete range;
}

void ChAnalyzeDomainsOfExprKaraExprMade(
    shared_ptr<Domains>& domains, shared_ptr<Expr>& expr1, shared_ptr<Expr>& expr2)
{
    ChAnalyzeExpr(expr1);
    CH_Value v1 = ChCalcExpr(expr1);
    v1.trim();
    ChAnalyzeExpr(expr2);
    CH_Value v2 = ChCalcExpr(expr2);
    v2.trim();
    Range *r = new Range(true, true, &v1, &v2);
    domains.get()->Intersect(*r);
    delete r;
}

void ChAnalyzeDomainsOfDom(shared_ptr<Domains>& domains, shared_ptr<Dom>& dom)
{
    assert(domains);
    switch (dom->m_type)
    {
    case Dom::CNSTRED_PRIMDOM:
        ChAnalyzeDomainsOfCnstredPrimDom(domains, dom->m_cnstr, dom->m_primdom);
        break;

    case Dom::PRIMDOM_ONLY:
        ChAnalyzeDomainsOfPrimDom(domains, dom->m_primdom);
        break;

    case Dom::DOM_OF_DOM:
        ChAnalyzeDomainsOfDom(domains, dom->m_dom);
        ChAnalyzeDomainsOfPrimDom(domains, dom->m_primdom);
        break;

    case Dom::NUM_ONLY:
        ChAnalyzeDomainsOfNum(domains, dom->m_num);
        break;

    case Dom::EXPR_KARA_EXPR_MADE:
        ChAnalyzeDomainsOfPrimDom(domains, dom->m_primdom);
        ChAnalyzeDomainsOfExprKaraExprMade(domains, dom->m_expr1, dom->m_expr2);
        break;

    case Dom::DOM_NOUCHI_DOM:
        ChAnalyzeDomainsOfDom(domains, dom->m_dom);
        ChAnalyzeDomainsOfDom(domains, dom->m_dom2);
        break;

    default:
        assert(0);
    }
}

void ChAnalyzeDoms(shared_ptr<Doms>& doms)
{
    assert(doms);
    Domains *d0 = new Domains();
    size_t i, siz = doms->size();
    for (i = 0; i < siz; ++i)
    {
        shared_ptr<Domains> domains(Domains::Whole());
        ChAnalyzeDomainsOfDom(domains, (*doms.get())[i]);
        d0->Union(*domains.get());
    }
    doms.get()->m_domains = shared_ptr<Domains>(d0);
}

////////////////////////////////////////////////////////////////////////////

void ChAnalyzeExprList(shared_ptr<ExprList>& exprlist);
void ChAnalyzeMono(shared_ptr<Mono>& mono);
void ChAnalyzeShite(shared_ptr<Shite>& shite);
void ChAnalyzeSuruto(shared_ptr<Suruto>& suruto);
void ChAnalyzeTerm(shared_ptr<Term>& term);
void ChAnalyzeFact(shared_ptr<Fact>& fact);
void ChAnalyzePrim(shared_ptr<Prim>& prim);

void ChAnalyzeMonoTermTasukazu(shared_ptr<Mono>& mono, shared_ptr<Term>& term);
void ChAnalyzeMonoTermKakerukazu(shared_ptr<Mono>& mono, shared_ptr<Term>& term);
void ChAnalyzeMonoTermHikukazu(shared_ptr<Mono>& mono, shared_ptr<Term>& term);
void ChAnalyzeMonoTermWarukazu(shared_ptr<Mono>& mono, shared_ptr<Term>& term);

void ChAnalyzeMonoFactTasukazu(shared_ptr<Mono>& mono, shared_ptr<Fact>& fact);
void ChAnalyzeMonoFactKakerukazu(shared_ptr<Mono>& mono, shared_ptr<Fact>& fact);
void ChAnalyzeMonoFactHikukazu(shared_ptr<Mono>& mono, shared_ptr<Fact>& fact);
void ChAnalyzeMonoFactWarukazu(shared_ptr<Mono>& mono, shared_ptr<Fact>& fact);

void ChAnalyzeMonoFactTasarerukazu(shared_ptr<Mono>& mono, shared_ptr<Fact>& fact);
void ChAnalyzeMonoFactKakerarerukazu(shared_ptr<Mono>& mono, shared_ptr<Fact>& fact);
void ChAnalyzeMonoFactHikarerukazu(shared_ptr<Mono>& mono, shared_ptr<Fact>& fact);
void ChAnalyzeMonoFactWararerukazu(shared_ptr<Mono>& mono, shared_ptr<Fact>& fact);

void ChAnalyzeMonoSurutoTasukazu(shared_ptr<Mono>& mono, shared_ptr<Suruto>& suruto);
void ChAnalyzeMonoSurutoKakerukazu(shared_ptr<Mono>& mono, shared_ptr<Suruto>& suruto);
void ChAnalyzeMonoSurutoHikukazu(shared_ptr<Mono>& mono, shared_ptr<Suruto>& suruto);
void ChAnalyzeMonoSurutoWarukazu(shared_ptr<Mono>& mono, shared_ptr<Suruto>& suruto);

void ChAnalyzeMonoMonoTasukazu(shared_ptr<Mono>& mono, shared_ptr<Mono>& mono2);
void ChAnalyzeMonoMonoKakerukazu(shared_ptr<Mono>& mono, shared_ptr<Mono>& mono2);
void ChAnalyzeMonoMonoHikukazu(shared_ptr<Mono>& mono, shared_ptr<Mono>& mono2);
void ChAnalyzeMonoMonoWarukazu(shared_ptr<Mono>& mono, shared_ptr<Mono>& mono2);

void ChAnalyzeMonoMonoTasarerukazu(shared_ptr<Mono>& mono, shared_ptr<Mono>& mono2);
void ChAnalyzeMonoMonoKakerarerukazu(shared_ptr<Mono>& mono, shared_ptr<Mono>& mono2);
void ChAnalyzeMonoMonoHikarerukazu(shared_ptr<Mono>& mono, shared_ptr<Mono>& mono2);
void ChAnalyzeMonoMonoWararerukazu(shared_ptr<Mono>& mono, shared_ptr<Mono>& mono2);

void ChAnalyzeMonoExprTasukazu(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr);
void ChAnalyzeMonoExprKakerukazu(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr);
void ChAnalyzeMonoExprHikukazu(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr);
void ChAnalyzeMonoExprWarukazu(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr);

void ChAnalyzeMonoTermTasukazu(shared_ptr<Mono>& mono, shared_ptr<Term>& term)
{
    assert(mono);
    assert(term);
    switch (term->m_type)
    {
    case Term::FACT_ONLY:
        ChAnalyzeMonoFactTasukazu(mono, term->m_fact);
        break;

    default:
        ChSetMessage(ch_not_tashizan);
    }
}

void ChAnalyzeMonoTermHikukazu(shared_ptr<Mono>& mono, shared_ptr<Term>& term)
{
    assert(mono);
    assert(term);
    switch (term->m_type)
    {
    case Term::FACT_ONLY:
        ChAnalyzeMonoFactHikukazu(mono, term->m_fact);
        break;

    default:
        ChSetMessage(ch_not_hikizan);
    }
}

void ChAnalyzeMonoTermTasarerukazu(shared_ptr<Mono>& mono, shared_ptr<Term>& term)
{
    assert(mono);
    assert(term);
    switch (term->m_type)
    {
    case Term::FACT_ONLY:
        ChAnalyzeMonoFactTasarerukazu(mono, term->m_fact);
        break;

    default:
        ChSetMessage(ch_not_tashizan);
    }
}

void ChAnalyzeMonoTermHikarerukazu(shared_ptr<Mono>& mono, shared_ptr<Term>& term)
{
    assert(mono);
    assert(term);
    switch (term->m_type)
    {
    case Term::FACT_ONLY:
        ChAnalyzeMonoFactHikarerukazu(mono, term->m_fact);
        break;

    default:
        ChSetMessage(ch_not_tashizan);
    }
}

void ChAnalyzeMonoShiteTasukazu(shared_ptr<Mono>& mono, shared_ptr<Shite>& shite)
{
    assert(mono);
    assert(shite);
    Mono *m;
    switch (shite->m_type)
    {
    case Shite::EXPRLIST_ADD:
        {
            ChAnalyzeExprList(shite->m_exprlist);
            ExprList *el = shite->m_exprlist.get();
            const size_t count = el->size();
            shared_ptr<Expr> e = (*el)[count - 1];
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = e;
            mono = shared_ptr<Mono>(m);
        }
        break;

    case Shite::MONO_ADD:
    case Shite::SHITE_ADD:
        ChAnalyzeExpr(shite->m_expr);
        m = new Mono;
        m->m_type = Mono::EXPR_ONLY;
        m->m_expr = shite->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    case Shite::MONO_ONLY:
        ChAnalyzeMonoMonoTasukazu(mono, shite->m_mono);
        break;

    default:
        ChSetMessage(ch_not_tashizan);
    }
}

void ChAnalyzeMonoShiteKakerukazu(shared_ptr<Mono>& mono, shared_ptr<Shite>& shite)
{
    assert(mono);
    assert(shite);
    Mono *m;
    switch (shite->m_type)
    {
    case Shite::EXPRLIST_MUL:
        {
            ChAnalyzeExprList(shite->m_exprlist);
            ExprList *el = shite->m_exprlist.get();
            const size_t count = el->size();
            shared_ptr<Expr> e = (*el)[count - 1];
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = e;
            mono = shared_ptr<Mono>(m);
        }
        break;

    case Shite::MONO_MUL:
    case Shite::SHITE_MUL:
        ChAnalyzeExpr(shite->m_expr);
        m = new Mono;
        m->m_type = Mono::EXPR_ONLY;
        m->m_expr = shite->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    case Shite::MONO_ONLY:
        ChAnalyzeMonoMonoKakerukazu(mono, shite->m_mono);
        break;

    default:
        ChSetMessage(ch_not_kakezan);
    }
}

void ChAnalyzeMonoShiteHikukazu(shared_ptr<Mono>& mono, shared_ptr<Shite>& shite)
{
    assert(mono);
    assert(shite);
    Mono *m;
    switch (shite->m_type)
    {
    case Shite::MONO_ONLY:
        ChAnalyzeMonoMonoHikukazu(mono, shite->m_mono);
        break;

    case Shite::MONO_SUB:
    case Shite::SHITE_SUB:
    case Shite::MONO_WO_EXPR_SUB:
        ChAnalyzeExpr(shite->m_expr);
        m = new Mono;
        m->m_type = Mono::EXPR_ONLY;
        m->m_expr = shite->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    default:
        ChSetMessage(ch_not_hikizan);
    }
}

void ChAnalyzeMonoShiteWarukazu(shared_ptr<Mono>& mono, shared_ptr<Shite>& shite)
{
    assert(mono);
    assert(shite);
    Mono *m;
    switch (shite->m_type)
    {
    case Shite::MONO_ONLY:
        ChAnalyzeMonoMonoWarukazu(mono, shite->m_mono);
        break;

    case Shite::MONO_DIV:
    case Shite::SHITE_DIV:
    case Shite::MONO_WO_EXPR_DIV:
        ChAnalyzeExpr(shite->m_expr);
        m = new Mono;
        m->m_type = Mono::EXPR_ONLY;
        m->m_expr = shite->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    default:
        ChSetMessage(ch_not_warizan);
    }
}

void ChAnalyzeMonoShiteTasarerukazu(shared_ptr<Mono>& mono, shared_ptr<Shite>& shite);
void ChAnalyzeMonoShiteKakerarerukazu(shared_ptr<Mono>& mono, shared_ptr<Shite>& shite);
void ChAnalyzeMonoShiteHikarerukazu(shared_ptr<Mono>& mono, shared_ptr<Shite>& shite);
void ChAnalyzeMonoShiteWararerukazu(shared_ptr<Mono>& mono, shared_ptr<Shite>& shite);

void ChAnalyzeMonoMonoTasukazu(shared_ptr<Mono>& mono, shared_ptr<Mono>& mono2)
{
    assert(mono);
    assert(mono2);
    Mono *m;
    switch (mono2->m_type)
    {
    case Mono::EXPRLIST_ADD:
    case Mono::MONO_TO_EXPRLIST_ADD:
        {
            ChAnalyzeExprList(mono2->m_exprlist);
            ExprList *el = mono2->m_exprlist.get();
            const size_t count = el->size();
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
        ChAnalyzeExpr(mono2->m_expr);
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
        ChSetMessage(ch_not_tashizan);
    }
}

void ChAnalyzeMonoMonoKakerukazu(shared_ptr<Mono>& mono, shared_ptr<Mono>& mono2)
{
    assert(mono);
    assert(mono2);
    Mono *m;
    switch (mono2->m_type)
    {
    case Mono::EXPRLIST_MUL:
    case Mono::MONO_TO_EXPRLIST_MUL:
        {
            ChAnalyzeExprList(mono2->m_exprlist);
            ExprList *el = mono2->m_exprlist.get();
            const size_t count = el->size();
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
        ChAnalyzeExpr(mono2->m_expr);
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
        ChSetMessage(ch_not_kakezan);
    }
}

void ChAnalyzeMonoMonoHikukazu(shared_ptr<Mono>& mono, shared_ptr<Mono>& mono2)
{
    assert(mono);
    assert(mono2);
    Mono *m;
    switch (mono2->m_type)
    {
    case Mono::EXPRLIST_SUB:
        {
            ChAnalyzeExprList(mono2->m_exprlist);
            ExprList *el = mono2->m_exprlist.get();
            const size_t count = el->size();
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
        ChAnalyzeExpr(mono2->m_expr);
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
        ChAnalyzeMono(mono2->m_mono);
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
        ChSetMessage(ch_not_hikizan);
    }
}

void ChAnalyzeMonoMonoWarukazu(shared_ptr<Mono>& mono, shared_ptr<Mono>& mono2)
{
    assert(mono);
    assert(mono2);
    Mono *m;
    switch (mono2->m_type)
    {
    case Mono::EXPRLIST_DIV:
        {
            ChAnalyzeExprList(mono2->m_exprlist);
            ExprList *el = mono2->m_exprlist.get();
            const size_t count = el->size();
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
        ChAnalyzeExpr(mono2->m_expr);
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
        ChAnalyzeMono(mono2->m_mono);
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
        ChSetMessage(ch_not_warizan);
    }
}

void ChAnalyzeMonoFactTasukazu(shared_ptr<Mono>& mono, shared_ptr<Fact>& fact)
{
    assert(mono);
    assert(fact);
    if (fact->m_type == Fact::SINGLE && fact->m_prim->m_type == Prim::MONO)
        ChAnalyzeMonoMonoTasukazu(mono, fact->m_prim->m_mono);
    else
        ChSetMessage(ch_not_tashizan);
}

void ChAnalyzeMonoFactKakerukazu(shared_ptr<Mono>& mono, shared_ptr<Fact>& fact)
{
    assert(mono);
    assert(fact);
    if (fact->m_type == Fact::SINGLE && fact->m_prim->m_type == Prim::MONO)
        ChAnalyzeMonoMonoKakerukazu(mono, fact->m_prim->m_mono);
    else
        ChSetMessage(ch_not_kakezan);
}

void ChAnalyzeMonoFactHikukazu(shared_ptr<Mono>& mono, shared_ptr<Fact>& fact)
{
    assert(mono);
    assert(fact);
    if (fact->m_type == Fact::SINGLE && fact->m_prim->m_type == Prim::MONO)
        ChAnalyzeMonoMonoHikukazu(mono, fact->m_prim->m_mono);
    else
        ChSetMessage(ch_not_hikizan);
}

void ChAnalyzeMonoFactWarukazu(shared_ptr<Mono>& mono, shared_ptr<Fact>& fact)
{
    assert(mono);
    assert(fact);
    if (fact->m_type == Fact::SINGLE && fact->m_prim->m_type == Prim::MONO)
        ChAnalyzeMonoMonoWarukazu(mono, fact->m_prim->m_mono);
    else
        ChSetMessage(ch_not_warizan);
}

void ChAnalyzeMonoFactTasarerukazu(shared_ptr<Mono>& mono, shared_ptr<Fact>& fact)
{
    assert(mono);
    assert(fact);
    if (fact->m_type == Fact::SINGLE && fact->m_prim->m_type == Prim::MONO)
        ChAnalyzeMonoMonoTasarerukazu(mono, fact->m_prim->m_mono);
    else
        ChSetMessage(ch_not_tashizan);
}

void ChAnalyzeMonoFactKakerarerukazu(shared_ptr<Mono>& mono, shared_ptr<Fact>& fact)
{
    assert(mono);
    assert(fact);
    if (fact->m_type == Fact::SINGLE && fact->m_prim->m_type == Prim::MONO)
        ChAnalyzeMonoMonoKakerarerukazu(mono, fact->m_prim->m_mono);
    else
        ChSetMessage(ch_not_kakezan);
}

void ChAnalyzeMonoFactHikarerukazu(shared_ptr<Mono>& mono, shared_ptr<Fact>& fact)
{
    assert(mono);
    assert(fact);
    if (fact->m_type == Fact::SINGLE && fact->m_prim->m_type == Prim::MONO)
        ChAnalyzeMonoMonoHikarerukazu(mono, fact->m_prim->m_mono);
    else
        ChSetMessage(ch_not_hikizan);
}

void ChAnalyzeMonoFactWararerukazu(shared_ptr<Mono>& mono, shared_ptr<Fact>& fact)
{
    assert(mono);
    assert(fact);
    if (fact->m_type == Fact::SINGLE && fact->m_prim->m_type == Prim::MONO)
        ChAnalyzeMonoMonoWararerukazu(mono, fact->m_prim->m_mono);
    else
        ChSetMessage(ch_not_warizan);
}

void ChAnalyzeMonoTermKakerukazu(shared_ptr<Mono>& mono, shared_ptr<Term>& term)
{
    assert(mono);
    assert(term);
    Mono *m;
    switch (term->m_type)
    {
    case Term::MUL:
        ChAnalyzeFact(term->m_fact);
        m = new Mono;
        m->m_type = Mono::FACT_ONLY;
        m->m_fact = term->m_fact;
        mono = shared_ptr<Mono>(m);
        break;

    case Term::DIV:
        ChSetMessage(ch_not_kakezan);
        break;

    case Term::FACT_ONLY:
        ChAnalyzeMonoFactKakerukazu(mono, term->m_fact);
        break;
    }
}

void ChAnalyzeMonoTermWarukazu(shared_ptr<Mono>& mono, shared_ptr<Term>& term)
{
    assert(mono);
    assert(term);
    Mono *m;
    switch (term->m_type)
    {
    case Term::MUL:
        ChSetMessage(ch_not_warizan);
        break;

    case Term::DIV:
        ChAnalyzeFact(term->m_fact);
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
    assert(mono);
    assert(expr);
    Mono *m;
    ChAnalyzeExpr(expr);
    switch (expr->m_type)
    {
    case Expr::ADD:
        ChAnalyzeTerm(expr->m_term);
        m = new Mono;
        m->m_type = Mono::TERM_ONLY;
        m->m_term = expr->m_term;
        mono = shared_ptr<Mono>(m);
        break;

    case Expr::TERM_ONLY:
        ChAnalyzeMonoTermTasukazu(mono, expr->m_term);
        break;

    default:
        ChSetMessage(ch_not_tashizan);
        break;
    }
}

void ChAnalyzeMonoExprKakerukazu(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
{
    assert(mono);
    assert(expr);
    ChAnalyzeExpr(expr);
    switch (expr->m_type)
    {
    case Expr::ADD:
    case Expr::SUB:
    case Expr::ZERO:
        ChSetMessage(ch_not_kakezan);
        break;

    case Expr::TERM_ONLY:
        ChAnalyzeMonoTermKakerukazu(mono, expr->m_term);
        break;
    }
}

void ChAnalyzeMonoExprHikukazu(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
{
    assert(mono);
    assert(expr);
    Mono *m;
    ChAnalyzeExpr(expr);
    switch (expr->m_type)
    {
    case Expr::ADD:
    case Expr::ZERO:
        ChSetMessage(ch_not_hikizan);
        break;

    case Expr::SUB:
        ChAnalyzeTerm(expr->m_term);
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
    assert(mono);
    assert(expr);
    ChAnalyzeExpr(expr);
    switch (expr->m_type)
    {
    case Expr::ADD:
    case Expr::SUB:
    case Expr::ZERO:
        ChSetMessage(ch_not_kakezan);
        break;

    case Expr::TERM_ONLY:
        ChAnalyzeMonoTermWarukazu(mono, expr->m_term);
        break;
    }
}

void ChAnalyzeMonoTermKakerarerukazu(shared_ptr<Mono>& mono, shared_ptr<Term>& term)
{
    assert(mono);
    assert(term);
    Mono *m;
    switch (term->m_type)
    {
    case Term::MUL:
        ChAnalyzeTerm(term->m_term);
        m = new Mono;
        m->m_type = Mono::TERM_ONLY;
        m->m_term = term->m_term;
        mono = shared_ptr<Mono>(m);
        break;

    case Term::DIV:
    case Term::FACT_ONLY:
        ChSetMessage(ch_not_kakezan);
        break;
    }
}

void ChAnalyzeMonoExprTasarerukazu(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
{
    assert(mono);
    assert(expr);
    Mono *m;
    ChAnalyzeExpr(expr);
    switch (expr->m_type)
    {
    case Expr::ADD:
        ChAnalyzeExpr(expr->m_expr);
        m = new Mono;
        m->m_type = Mono::EXPR_ONLY;
        m->m_expr = expr->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    case Expr::SUB:
    case Expr::TERM_ONLY:
    case Expr::ZERO:
        ChSetMessage(ch_not_tashizan);
        break;
    }
}

void ChAnalyzeMonoTermWararerukazu(shared_ptr<Mono>& mono, shared_ptr<Term>& term)
{
    assert(mono);
    assert(term);
    Mono *m;
    switch (term->m_type)
    {
    case Term::DIV:
        ChAnalyzeTerm(term->m_term);
        m = new Mono;
        m->m_type = Mono::TERM_ONLY;
        m->m_term = term->m_term;
        mono = shared_ptr<Mono>(m);
        break;

    case Term::MUL:
    case Term::FACT_ONLY:
        ChSetMessage(ch_not_warizan);
        break;
    }
}

void ChAnalyzeMonoExprKakerarerukazu(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
{
    assert(mono);
    assert(expr);
    ChAnalyzeExpr(expr);
    switch (expr->m_type)
    {
    case Expr::ADD:
    case Expr::SUB:
    case Expr::ZERO:
        ChSetMessage(ch_not_kakezan);
        break;

    case Expr::TERM_ONLY:
        ChAnalyzeMonoTermKakerarerukazu(mono, expr->m_term);
        break;
    }
}

void ChAnalyzeMonoExprHikarerukazu(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
{
    assert(mono);
    assert(expr);
    Mono *m;
    ChAnalyzeExpr(expr);
    switch (expr->m_type)
    {
    case Expr::SUB:
        ChAnalyzeExpr(expr->m_expr);
        m = new Mono;
        m->m_type = Mono::EXPR_ONLY;
        m->m_expr = expr->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    case Expr::ADD:
    case Expr::TERM_ONLY:
    case Expr::ZERO:
        ChSetMessage(ch_not_hikizan);
        break;
    }
}

void ChAnalyzeMonoExprWararerukazu(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
{
    assert(mono);
    assert(expr);
    ChAnalyzeExpr(expr);
    switch (expr->m_type)
    {
    case Expr::ADD:
    case Expr::SUB:
    case Expr::ZERO:
        ChSetMessage(ch_not_warizan);
        break;

    case Expr::TERM_ONLY:
        ChAnalyzeMonoTermWararerukazu(mono, expr->m_term);
        break;
    }
}

void ChAnalyzeMonoSurutoTasukazu(shared_ptr<Mono>& mono, shared_ptr<Suruto>& suruto)
{
    assert(mono);
    assert(suruto);
    Mono *m;
    switch (suruto->m_type)
    {
    case Suruto::EXPRLIST_ADD:
        {
            ChAnalyzeExprList(suruto->m_exprlist);
            ExprList *el = suruto->m_exprlist.get();
            const size_t count = el->size();
            shared_ptr<Expr> e = (*el)[count - 1];
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = e;
            mono = shared_ptr<Mono>(m);
        }
        break;

    case Suruto::MONO_ADD:
    case Suruto::SHITE_TASUTO:
        ChAnalyzeExpr(suruto->m_expr);
        m = new Mono;
        m->m_type = Mono::EXPR_ONLY;
        m->m_expr = suruto->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    default:
        ChSetMessage(ch_not_tashizan);
    }
}

void ChAnalyzeMonoSurutoKakerukazu(shared_ptr<Mono>& mono, shared_ptr<Suruto>& suruto)
{
    assert(mono);
    assert(suruto);
    Mono *m;
    switch (suruto->m_type)
    {
    case Suruto::EXPRLIST_MUL:
        {
            ChAnalyzeExprList(suruto->m_exprlist);
            ExprList *el = suruto->m_exprlist.get();
            const size_t count = el->size();
            shared_ptr<Expr> e = (*el)[count - 1];
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = e;
            mono = shared_ptr<Mono>(m);
        }
        break;

    case Suruto::MONO_MUL:
    case Suruto::SHITE_KAKERUTO:
        ChAnalyzeExpr(suruto->m_expr);
        m = new Mono;
        m->m_type = Mono::EXPR_ONLY;
        m->m_expr = suruto->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    default:
        ChSetMessage(ch_not_kakezan);
    }
}

void ChAnalyzeMonoSurutoHikukazu(shared_ptr<Mono>& mono, shared_ptr<Suruto>& suruto)
{
    assert(mono);
    assert(suruto);
    Mono *m;
    switch (suruto->m_type)
    {
    case Suruto::MONO_SUB:
    case Suruto::SHITE_HIKUTO:
        ChAnalyzeExpr(suruto->m_expr);
        m = new Mono;
        m->m_type = Mono::EXPR_ONLY;
        m->m_expr = suruto->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    case Suruto::MONO_WO_EXPR_SUB:
        ChAnalyzeMono(suruto->m_mono);
        m = new Mono;
        m->m_type = Mono::MONO_ONLY;
        m->m_mono = suruto->m_mono;
        mono = shared_ptr<Mono>(m);
        break;

    default:
        ChSetMessage(ch_not_hikizan);
    }
}

void ChAnalyzeMonoSurutoWarukazu(shared_ptr<Mono>& mono, shared_ptr<Suruto>& suruto)
{
    assert(mono);
    assert(suruto);
    Mono *m;
    switch (suruto->m_type)
    {
    case Suruto::MONO_DIV:
    case Suruto::SHITE_WARUTO:
        ChAnalyzeExpr(suruto->m_expr);
        m = new Mono;
        m->m_type = Mono::EXPR_ONLY;
        m->m_expr = suruto->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    case Suruto::MONO_DE_EXPR_DIV:
        ChAnalyzeMono(suruto->m_mono);
        m = new Mono;
        m->m_type = Mono::MONO_ONLY;
        m->m_mono = suruto->m_mono;
        mono = shared_ptr<Mono>(m);
        break;

    default:
        ChSetMessage(ch_not_warizan);
    }
}

void ChAnalyzeMonoSurutoTasarerukazu(shared_ptr<Mono>& mono, shared_ptr<Suruto>& suruto)
{
    assert(mono);
    assert(suruto);
    Mono *m;
    switch (suruto->m_type)
    {
    case Suruto::EXPRLIST_ADD:
        {
            ChAnalyzeExprList(suruto->m_exprlist);
            ExprList *el = suruto->m_exprlist.get();
            assert(el->size() >= 1);
            shared_ptr<Expr> e = (*el)[0];
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = e;
            mono = shared_ptr<Mono>(m);
        }
        break;

    case Suruto::MONO_ADD:
        ChAnalyzeMono(suruto->m_mono);
        m = new Mono;
        m->m_type = Mono::MONO_ONLY;
        m->m_mono = suruto->m_mono;
        mono = shared_ptr<Mono>(m);
        break;

    case Suruto::SHITE_TASUTO:
        ChAnalyzeShite(suruto->m_shite);
        m = new Mono;
        m->m_type = Mono::SHITE_ONLY;
        m->m_shite = suruto->m_shite;
        mono = shared_ptr<Mono>(m);
        break;

    default:
        ChSetMessage(ch_not_tashizan);
    }
}

void ChAnalyzeMonoSurutoKakerarerukazu(shared_ptr<Mono>& mono, shared_ptr<Suruto>& suruto)
{
    assert(mono);
    assert(suruto);
    Mono *m;
    switch (suruto->m_type)
    {
    case Suruto::EXPRLIST_MUL:
        {
            ChAnalyzeExprList(suruto->m_exprlist);
            ExprList *el = suruto->m_exprlist.get();
            assert(el->size() >= 1);
            shared_ptr<Expr> e = (*el)[0];
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = e;
            mono = shared_ptr<Mono>(m);
        }
        break;

    case Suruto::MONO_MUL:
        ChAnalyzeMono(suruto->m_mono);
        m = new Mono;
        m->m_type = Mono::MONO_ONLY;
        m->m_mono = suruto->m_mono;
        mono = shared_ptr<Mono>(m);
        break;

    case Suruto::SHITE_KAKERUTO:
        ChAnalyzeShite(suruto->m_shite);
        m = new Mono;
        m->m_type = Mono::SHITE_ONLY;
        m->m_shite = suruto->m_shite;
        mono = shared_ptr<Mono>(m);
        break;

    default:
        ChSetMessage(ch_not_kakezan);
    }
}

void ChAnalyzeMonoSurutoHikarerukazu(shared_ptr<Mono>& mono, shared_ptr<Suruto>& suruto)
{
    assert(mono);
    assert(suruto);
    Mono *m;
    switch (suruto->m_type)
    {
    case Suruto::MONO_SUB:
        ChAnalyzeMono(suruto->m_mono);
        m = new Mono;
        m->m_type = Mono::MONO_ONLY;
        m->m_mono = suruto->m_mono;
        mono = shared_ptr<Mono>(m);
        break;

    case Suruto::SHITE_HIKUTO:
        ChAnalyzeShite(suruto->m_shite);
        m = new Mono;
        m->m_type = Mono::SHITE_ONLY;
        m->m_shite = suruto->m_shite;
        mono = shared_ptr<Mono>(m);
        break;

    case Suruto::MONO_WO_EXPR_SUB:
        ChAnalyzeExpr(suruto->m_expr);
        m = new Mono;
        m->m_type = Mono::EXPR_ONLY;
        m->m_expr = suruto->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    default:
        ChSetMessage(ch_not_hikizan);
    }
}

void ChAnalyzeMonoSurutoWararerukazu(shared_ptr<Mono>& mono, shared_ptr<Suruto>& suruto)
{
    assert(mono);
    assert(suruto);
    Mono *m;
    switch (suruto->m_type)
    {
    case Suruto::MONO_DIV:
        ChAnalyzeMono(suruto->m_mono);
        m = new Mono;
        m->m_type = Mono::MONO_ONLY;
        m->m_mono = suruto->m_mono;
        mono = shared_ptr<Mono>(m);
        break;

    case Suruto::SHITE_WARUTO:
        ChAnalyzeShite(suruto->m_shite);
        m = new Mono;
        m->m_type = Mono::SHITE_ONLY;
        m->m_shite = suruto->m_shite;
        mono = shared_ptr<Mono>(m);
        break;

    case Suruto::MONO_DE_EXPR_DIV:
        ChAnalyzeExpr(suruto->m_expr);
        m = new Mono;
        m->m_type = Mono::EXPR_ONLY;
        m->m_expr = suruto->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    default:
        ChSetMessage(ch_not_warizan);
    }
}

void ChAnalyzeMonoPrevSentenceTasukazu(shared_ptr<Mono>& mono)
{
    assert(mono);
    Mono *m;
    Sentence *sentence = Calc_H::s_sentence_prev.get();

    switch (sentence->m_type)
    {
    case Sentence::MONO:
        ChAnalyzeMono(sentence->m_mono);
        ChAnalyzeMonoMonoTasukazu(mono, sentence->m_mono);
        break;

    case Sentence::EXPR:
        ChAnalyzeExpr(sentence->m_expr);
        ChAnalyzeMonoExprTasukazu(mono, sentence->m_expr);
        break;

    case Sentence::SURUTO:
        ChAnalyzeSuruto(sentence->m_suruto);
        ChAnalyzeMonoSurutoTasukazu(mono, sentence->m_suruto);
        break;

    case Sentence::EXPRLIST_ADD:
        {
            ChAnalyzeExprList(sentence->m_exprlist);
            ExprList *el = sentence->m_exprlist.get();
            const size_t count = el->size();
            shared_ptr<Expr> e = (*el)[count - 1];
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = e;
            mono = shared_ptr<Mono>(m);
        }
        break;

    case Sentence::SHITE:
        ChAnalyzeShite(sentence->m_shite);
        ChAnalyzeMonoShiteTasukazu(mono, sentence->m_shite);
        break;

    case Sentence::EXPRLIST_MUL:
    default:
        ChSetMessage(ch_not_tashizan);
    }
}

void ChAnalyzeMonoPrevSentenceKakerukazu(shared_ptr<Mono>& mono)
{
    assert(mono);
    Mono *m;
    Sentence *sentence = Calc_H::s_sentence_prev.get();

    switch (sentence->m_type)
    {
    case Sentence::MONO:
        ChAnalyzeMono(sentence->m_mono);
        ChAnalyzeMonoMonoKakerukazu(mono, sentence->m_mono);
        break;

    case Sentence::EXPR:
        ChAnalyzeExpr(sentence->m_expr);
        ChAnalyzeMonoExprKakerukazu(mono, sentence->m_expr);
        break;

    case Sentence::SURUTO:
        ChAnalyzeSuruto(sentence->m_suruto);
        ChAnalyzeMonoSurutoKakerukazu(mono, sentence->m_suruto);
        break;

    case Sentence::EXPRLIST_MUL:
        {
            ChAnalyzeExprList(sentence->m_exprlist);
            ExprList *el = sentence->m_exprlist.get();
            const size_t count = el->size();
            shared_ptr<Expr> e = (*el)[count - 1];
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = e;
            mono = shared_ptr<Mono>(m);
        }
        break;

    case Sentence::SHITE:
        ChAnalyzeShite(sentence->m_shite);
        ChAnalyzeMonoShiteKakerukazu(mono, sentence->m_shite);
        break;

    case Sentence::EXPRLIST_ADD:
    default:
        ChSetMessage(ch_not_kakezan);
    }
}

void ChAnalyzeMonoPrevSentenceHikukazu(shared_ptr<Mono>& mono)
{
    assert(mono);
    Sentence *sentence = Calc_H::s_sentence_prev.get();

    switch (sentence->m_type)
    {
    case Sentence::MONO:
        ChAnalyzeMono(sentence->m_mono);
        ChAnalyzeMonoMonoHikukazu(mono, sentence->m_mono);
        break;

    case Sentence::EXPR:
        ChAnalyzeExpr(sentence->m_expr);
        ChAnalyzeMonoExprHikukazu(mono, sentence->m_expr);
        break;

    case Sentence::SURUTO:
        ChAnalyzeSuruto(sentence->m_suruto);
        ChAnalyzeMonoSurutoHikukazu(mono, sentence->m_suruto);
        break;

    case Sentence::SHITE:
        ChAnalyzeShite(sentence->m_shite);
        ChAnalyzeMonoShiteHikukazu(mono, sentence->m_shite);
        break;

    case Sentence::EXPRLIST_ADD:
    case Sentence::EXPRLIST_MUL:
    default:
        ChSetMessage(ch_not_hikizan);
    }
}

void ChAnalyzeMonoPrevSentenceWarukazu(shared_ptr<Mono>& mono)
{
    assert(mono);
    Sentence *sentence = Calc_H::s_sentence_prev.get();

    switch (sentence->m_type)
    {
    case Sentence::MONO:
        ChAnalyzeMono(sentence->m_mono);
        ChAnalyzeMonoMonoWarukazu(mono, sentence->m_mono);
        break;

    case Sentence::EXPR:
        ChAnalyzeExpr(sentence->m_expr);
        ChAnalyzeMonoExprWarukazu(mono, sentence->m_expr);
        break;

    case Sentence::SURUTO:
        ChAnalyzeSuruto(sentence->m_suruto);
        ChAnalyzeMonoSurutoWarukazu(mono, sentence->m_suruto);
        break;

    case Sentence::SHITE:
        ChAnalyzeShite(sentence->m_shite);
        ChAnalyzeMonoShiteWarukazu(mono, sentence->m_shite);
        break;

    case Sentence::EXPRLIST_ADD:
    case Sentence::EXPRLIST_MUL:
    default:
        ChSetMessage(ch_not_warizan);
    }
}

void ChAnalyzeMonoPrevSentenceTasarerukazu(shared_ptr<Mono>& mono)
{
    assert(mono);
    Mono *m;
    Sentence *sentence = Calc_H::s_sentence_prev.get();

    switch (sentence->m_type)
    {
    case Sentence::MONO:
        ChAnalyzeMono(sentence->m_mono);
        ChAnalyzeMonoMonoTasarerukazu(mono, sentence->m_mono);
        break;

    case Sentence::EXPR:
        ChAnalyzeExpr(sentence->m_expr);
        ChAnalyzeMonoExprTasarerukazu(mono, sentence->m_expr);
        break;

    case Sentence::SURUTO:
        ChAnalyzeSuruto(sentence->m_suruto);
        ChAnalyzeMonoSurutoTasarerukazu(mono, sentence->m_suruto);
        break;

    case Sentence::EXPRLIST_ADD:
        {
            ChAnalyzeExprList(sentence->m_exprlist);
            ExprList *el = sentence->m_exprlist.get();
            assert(el->size() >= 1);
            shared_ptr<Expr> e = (*el)[0];
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = e;
            mono = shared_ptr<Mono>(m);
        }
        break;

    case Sentence::SHITE:
        ChAnalyzeShite(sentence->m_shite);
        ChAnalyzeMonoShiteTasarerukazu(mono, sentence->m_shite);
        break;

    case Sentence::EXPRLIST_MUL:
    default:
        ChSetMessage(ch_not_tashizan);
    }
}

void ChAnalyzeMonoPrevSentenceKakerarerukazu(shared_ptr<Mono>& mono)
{
    assert(mono);
    Mono *m;
    Sentence *sentence = Calc_H::s_sentence_prev.get();

    switch (sentence->m_type)
    {
    case Sentence::MONO:
        ChAnalyzeMono(sentence->m_mono);
        ChAnalyzeMonoMonoKakerarerukazu(mono, sentence->m_mono);
        break;

    case Sentence::EXPR:
        ChAnalyzeExpr(sentence->m_expr);
        ChAnalyzeMonoExprKakerarerukazu(mono, sentence->m_expr);
        break;

    case Sentence::SURUTO:
        ChAnalyzeSuruto(sentence->m_suruto);
        ChAnalyzeMonoSurutoKakerarerukazu(mono, sentence->m_suruto);
        break;

    case Sentence::EXPRLIST_MUL:
        {
            ChAnalyzeExprList(sentence->m_exprlist);
            ExprList *el = sentence->m_exprlist.get();
            assert(el->size() >= 1);
            shared_ptr<Expr> e = (*el)[0];
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = e;
            mono = shared_ptr<Mono>(m);
        }
        break;

    case Sentence::SHITE:
        ChAnalyzeShite(sentence->m_shite);
        ChAnalyzeMonoShiteKakerarerukazu(mono, sentence->m_shite);
        break;

    case Sentence::EXPRLIST_ADD:
    default:
        ChSetMessage(ch_not_kakezan);
    }
}

void ChAnalyzeMonoPrevSentenceHikarerukazu(shared_ptr<Mono>& mono)
{
    assert(mono);
    Sentence *sentence = Calc_H::s_sentence_prev.get();

    switch (sentence->m_type)
    {
    case Sentence::MONO:
        ChAnalyzeMono(sentence->m_mono);
        ChAnalyzeMonoMonoHikarerukazu(mono, sentence->m_mono);
        break;

    case Sentence::EXPR:
        ChAnalyzeExpr(sentence->m_expr);
        ChAnalyzeMonoExprHikarerukazu(mono, sentence->m_expr);
        break;

    case Sentence::SURUTO:
        ChAnalyzeSuruto(sentence->m_suruto);
        ChAnalyzeMonoSurutoHikarerukazu(mono, sentence->m_suruto);
        break;

    case Sentence::SHITE:
        ChAnalyzeShite(sentence->m_shite);
        ChAnalyzeMonoShiteHikarerukazu(mono, sentence->m_shite);
        break;

    case Sentence::EXPRLIST_ADD:
    case Sentence::EXPRLIST_MUL:
    default:
        ChSetMessage(ch_not_hikizan);
    }
}

void ChAnalyzeMonoPrevSentenceWararerukazu(shared_ptr<Mono>& mono)
{
    assert(mono);
    Sentence *sentence = Calc_H::s_sentence_prev.get();

    switch (sentence->m_type)
    {
    case Sentence::MONO:
        ChAnalyzeMono(sentence->m_mono);
        ChAnalyzeMonoMonoWararerukazu(mono, sentence->m_mono);
        break;

    case Sentence::EXPR:
        ChAnalyzeExpr(sentence->m_expr);
        ChAnalyzeMonoExprWararerukazu(mono, sentence->m_expr);
        break;

    case Sentence::SURUTO:
        ChAnalyzeSuruto(sentence->m_suruto);
        ChAnalyzeMonoSurutoWararerukazu(mono, sentence->m_suruto);
        break;

    case Sentence::SHITE:
        ChAnalyzeShite(sentence->m_shite);
        ChAnalyzeMonoShiteWararerukazu(mono, sentence->m_shite);
        break;

    case Sentence::EXPRLIST_ADD:
    case Sentence::EXPRLIST_MUL:
    default:
        ChSetMessage(ch_not_warizan);
    }
}

void ChAnalyzeMonoMonoTasarerukazu(shared_ptr<Mono>& mono, shared_ptr<Mono>& mono2)
{
    assert(mono);
    assert(mono2);
    Mono *m;
    switch (mono2->m_type)
    {
    case Mono::EXPRLIST_ADD:
        {
            ChAnalyzeExprList(mono2->m_exprlist);
            ExprList *el = mono2->m_exprlist.get();
            assert(el->size() >= 1);
            shared_ptr<Expr> e = (*el)[0];
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = e;
            mono = shared_ptr<Mono>(m);
        }
        break;

    case Mono::MONO_ADD:
        ChAnalyzeMono(mono2->m_mono);
        mono = mono2->m_mono;
        break;

    case Mono::MONO_ONLY:
        ChAnalyzeMonoMonoTasarerukazu(mono, mono2->m_mono);
        break;

    case Mono::SHITE_ADD:
        ChAnalyzeShite(mono2->m_shite);
        m = new Mono;
        m->m_type = Mono::SHITE_ONLY;
        m->m_shite = mono2->m_shite;
        mono = shared_ptr<Mono>(m);
        break;

    case Mono::SHITE_ONLY:
        ChAnalyzeMonoShiteTasarerukazu(mono, mono2->m_shite);
        break;

    case Mono::EXPR_ONLY:
        ChAnalyzeMonoExprTasarerukazu(mono, mono2->m_expr);
        break;

    //case Mono::SORE:

    case Mono::MONO_TO_EXPRLIST_ADD:
    case Mono::MONO_TO_EXPR_ADD:
        ChAnalyzeMono(mono2->m_mono);
        mono = mono2->m_mono;
        break;

    case Mono::TERM_ONLY:
        ChAnalyzeMonoTermTasarerukazu(mono, mono2->m_term);
        break;

    case Mono::FACT_ONLY:
        ChAnalyzeMonoFactTasarerukazu(mono, mono2->m_fact);
        break;

    case Mono::SURUTO_ONLY:
        ChAnalyzeMonoSurutoTasarerukazu(mono, mono2->m_suruto);
        break;

    default:
        ChSetMessage(ch_not_tashizan);
    }
}

void ChAnalyzeMonoMonoKakerarerukazu(shared_ptr<Mono>& mono, shared_ptr<Mono>& mono2)
{
    assert(mono);
    assert(mono2);
    Mono *m;
    switch (mono2->m_type)
    {
    case Mono::EXPRLIST_MUL:
        {
            ChAnalyzeExprList(mono2->m_exprlist);
            ExprList *el = mono2->m_exprlist.get();
            assert(el->size() >= 1);
            shared_ptr<Expr> e = (*el)[0];
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = e;
            mono = shared_ptr<Mono>(m);
        }
        break;

    case Mono::MONO_MUL:
        ChAnalyzeMono(mono2->m_mono);
        mono = mono2->m_mono;
        break;

    case Mono::MONO_ONLY:
        ChAnalyzeMonoMonoKakerarerukazu(mono, mono2->m_mono);
        break;

    case Mono::SHITE_MUL:
        ChAnalyzeShite(mono2->m_shite);
        m = new Mono;
        m->m_type = Mono::SHITE_ONLY;
        m->m_shite = mono2->m_shite;
        mono = shared_ptr<Mono>(m);
        break;

    case Mono::SHITE_ONLY:
        ChAnalyzeMonoShiteKakerarerukazu(mono, mono2->m_shite);
        break;

    case Mono::EXPR_ONLY:
        ChAnalyzeMonoExprKakerarerukazu(mono, mono2->m_expr);
        break;

    //case Mono::SORE:

    case Mono::MONO_TO_EXPRLIST_MUL:
    case Mono::MONO_TO_EXPR_MUL:
        ChAnalyzeMono(mono2->m_mono);
        mono = mono2->m_mono;
        break;
        
    case Mono::TERM_ONLY:
        ChAnalyzeMonoTermKakerarerukazu(mono, mono2->m_term);
        break;

    case Mono::FACT_ONLY:
        ChAnalyzeMonoFactKakerarerukazu(mono, mono2->m_fact);
        break;

    case Mono::SURUTO_ONLY:
        ChAnalyzeMonoSurutoKakerarerukazu(mono, mono2->m_suruto);
        break;

    default:
        ChSetMessage(ch_not_kakezan);
    }
}

void ChAnalyzeMonoMonoHikarerukazu(shared_ptr<Mono>& mono, shared_ptr<Mono>& mono2)
{
    assert(mono);
    assert(mono2);
    Mono *m;
    switch (mono2->m_type)
    {
    case Mono::EXPRLIST_SUB:
        {
            ChAnalyzeExprList(mono2->m_exprlist);
            ExprList *el = mono2->m_exprlist.get();
            assert(el->size() >= 1);
            shared_ptr<Expr> e = (*el)[0];
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = e;
            mono = shared_ptr<Mono>(m);
        }
        break;

    case Mono::MONO_SUB:
        ChAnalyzeMono(mono2->m_mono);
        mono = mono2->m_mono;
        break;

    case Mono::MONO_ONLY:
        ChAnalyzeMonoMonoHikarerukazu(mono, mono2->m_mono);
        break;

    case Mono::SHITE_SUB:
        ChAnalyzeShite(mono2->m_shite);
        m = new Mono;
        m->m_type = Mono::SHITE_ONLY;
        m->m_shite = mono2->m_shite;
        mono = shared_ptr<Mono>(m);
        break;

    case Mono::SHITE_ONLY:
        ChAnalyzeMonoShiteHikarerukazu(mono, mono2->m_shite);
        break;

    case Mono::EXPR_ONLY:
        ChAnalyzeMonoExprHikarerukazu(mono, mono2->m_expr);
        break;

    //case Mono::SORE:
        
    case Mono::MONO_TO_EXPR_SUB:
        ChAnalyzeMono(mono2->m_mono);
        mono = mono2->m_mono;
        break;

    case Mono::MONO_WO_EXPR_KARA_SUB:
        ChAnalyzeExpr(mono2->m_expr);
        m = new Mono;
        m->m_type = Mono::EXPR_ONLY;
        m->m_expr = mono2->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    case Mono::TERM_ONLY:
        ChAnalyzeMonoTermHikarerukazu(mono, mono2->m_term);
        break;

    case Mono::FACT_ONLY:
        ChAnalyzeMonoFactHikarerukazu(mono, mono2->m_fact);
        break;

    case Mono::SURUTO_ONLY:
        ChAnalyzeMonoSurutoHikarerukazu(mono, mono2->m_suruto);
        break;

    default:
        ChSetMessage(ch_not_hikizan);
    }
}

void ChAnalyzeMonoMonoWararerukazu(shared_ptr<Mono>& mono, shared_ptr<Mono>& mono2)
{
    assert(mono);
    assert(mono2);
    Mono *m;
    switch (mono2->m_type)
    {
    case Mono::EXPRLIST_DIV:
        {
            ChAnalyzeExprList(mono2->m_exprlist);
            ExprList *el = mono2->m_exprlist.get();
            assert(el->size() >= 1);
            shared_ptr<Expr> e = (*el)[0];
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = e;
            mono = shared_ptr<Mono>(m);
        }
        break;

    case Mono::MONO_ONLY:
        ChAnalyzeMonoMonoWararerukazu(mono, mono2->m_mono);
        break;

    case Mono::SHITE_DIV:
        ChAnalyzeShite(mono2->m_shite);
        m = new Mono;
        m->m_type = Mono::SHITE_ONLY;
        m->m_shite = mono2->m_shite;
        mono = shared_ptr<Mono>(m);
        break;

    case Mono::SHITE_ONLY:
        ChAnalyzeMonoShiteWararerukazu(mono, mono2->m_shite);
        break;

    case Mono::EXPR_ONLY:
        ChAnalyzeMonoExprWararerukazu(mono, mono2->m_expr);
        break;

    //case Mono::SORE:

    case Mono::MONO_DIV:
    case Mono::MONO_TO_EXPR_DIV:
        ChAnalyzeMono(mono2->m_mono);
        mono = mono2->m_mono;
        break;

    case Mono::MONO_DE_EXPR_WO_DIV:
        ChAnalyzeExpr(mono2->m_expr);
        m = new Mono;
        m->m_type = Mono::EXPR_ONLY;
        m->m_expr = mono2->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    case Mono::TERM_ONLY:
        ChAnalyzeMonoTermWararerukazu(mono, mono2->m_term);
        break;

    case Mono::FACT_ONLY:
        ChAnalyzeMonoFactWararerukazu(mono, mono2->m_fact);
        break;

    case Mono::SURUTO_ONLY:
        ChAnalyzeMonoSurutoWararerukazu(mono, mono2->m_suruto);
        break;

    default:
        ChSetMessage(ch_not_warizan);
    }
}

void ChAnalyzeMonoShiteTasarerukazu(shared_ptr<Mono>& mono, shared_ptr<Shite>& shite)
{
    assert(mono);
    assert(shite);
    Mono *m;
    switch (shite->m_type)
    {
    case Shite::EXPRLIST_ADD:
        {
            ChAnalyzeExprList(shite->m_exprlist);
            ExprList *el = shite->m_exprlist.get();
            assert(el->size() >= 1);
            shared_ptr<Expr> e = (*el)[0];
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = e;
            mono = shared_ptr<Mono>(m);
        }
        break;

    case Shite::MONO_ADD:
        mono = shite->m_mono;
        break;

    case Shite::MONO_ONLY:
        ChAnalyzeMonoMonoTasarerukazu(mono, shite->m_mono);
        break;

    case Shite::SHITE_ADD:
        ChAnalyzeShite(shite->m_shite);
        m = new Mono;
        m->m_type = Mono::SHITE_ONLY;
        m->m_shite = shite->m_shite;
        mono = shared_ptr<Mono>(m);
        break;

    default:
        ChSetMessage(ch_not_tashizan);
    }
}

void ChAnalyzeMonoShiteKakerarerukazu(shared_ptr<Mono>& mono, shared_ptr<Shite>& shite)
{
    assert(mono);
    assert(shite);
    Mono *m;
    switch (shite->m_type)
    {
    case Shite::EXPRLIST_MUL:
        {
            ChAnalyzeExprList(shite->m_exprlist);
            ExprList *el = shite->m_exprlist.get();
            assert(el->size() >= 1);
            shared_ptr<Expr> e = (*el)[0];
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = e;
            mono = shared_ptr<Mono>(m);
        }
        break;

    case Shite::MONO_MUL:
        mono = shite->m_mono;
        break;

    case Shite::MONO_ONLY:
        ChAnalyzeMonoMonoKakerarerukazu(mono, shite->m_mono);
        break;

    case Shite::SHITE_MUL:
        ChAnalyzeShite(shite->m_shite);
        m = new Mono;
        m->m_type = Mono::SHITE_ONLY;
        m->m_shite = shite->m_shite;
        mono = shared_ptr<Mono>(m);
        break;

    default:
        ChSetMessage(ch_not_kakezan);
    }
}

void ChAnalyzeMonoShiteHikarerukazu(shared_ptr<Mono>& mono, shared_ptr<Shite>& shite)
{
    assert(mono);
    assert(shite);
    Mono *m;
    switch (shite->m_type)
    {
    case Shite::MONO_SUB:
    case Shite::MONO_WO_EXPR_SUB:
        mono = shite->m_mono;
        break;

    case Shite::MONO_ONLY:
        ChAnalyzeMonoMonoHikarerukazu(mono, shite->m_mono);
        break;

    case Shite::SHITE_SUB:
        ChAnalyzeShite(shite->m_shite);
        m = new Mono;
        m->m_type = Mono::SHITE_ONLY;
        m->m_shite = shite->m_shite;
        mono = shared_ptr<Mono>(m);
        break;

    default:
        ChSetMessage(ch_not_hikizan);
    }
}

void ChAnalyzeMonoShiteWararerukazu(shared_ptr<Mono>& mono, shared_ptr<Shite>& shite)
{
    assert(mono);
    assert(shite);
    Mono *m;
    switch (shite->m_type)
    {
    case Shite::MONO_DIV:
    case Shite::MONO_WO_EXPR_DIV:
        mono = shite->m_mono;
        break;

    case Shite::MONO_ONLY:
        ChAnalyzeMonoMonoWararerukazu(mono, shite->m_mono);
        break;

    case Shite::SHITE_DIV:
        ChAnalyzeShite(shite->m_shite);
        m = new Mono;
        m->m_type = Mono::SHITE_ONLY;
        m->m_shite = shite->m_shite;
        mono = shared_ptr<Mono>(m);
        break;

    default:
        ChSetMessage(ch_not_warizan);
    }
}

bool ChIsShiteTashizan(shared_ptr<Shite>& shite)
{
    assert(shite);
    switch (shite->m_type)
    {
    case Shite::EXPRLIST_ADD:
    case Shite::MONO_ADD:
    case Shite::SHITE_ADD:
        return true;

    case Shite::MONO_ONLY:
        return ChIsMonoTashizan(shite->m_mono);

    default:
        return false;
    }
}

bool ChIsShiteKakezan(shared_ptr<Shite>& shite)
{
    assert(shite);
    switch (shite->m_type)
    {
    case Shite::EXPRLIST_MUL:
    case Shite::MONO_MUL:
    case Shite::SHITE_MUL:
    case Shite::SHITE_EXPR_BAI:
    case Shite::SHITE_BAI:
        return true;

    case Shite::MONO_ONLY:
        return ChIsMonoKakezan(shite->m_mono);

    default:
        return false;
    }
}

bool ChIsShiteHikizan(shared_ptr<Shite>& shite)
{
    assert(shite);
    switch (shite->m_type)
    {
    case Shite::MONO_SUB:
    case Shite::SHITE_SUB:
    case Shite::MONO_WO_EXPR_SUB:
        return true;

    case Shite::MONO_ONLY:
        return ChIsMonoHikizan(shite->m_mono);

    default:
        return false;
    }
}

bool ChIsShiteWarizan(shared_ptr<Shite>& shite)
{
    assert(shite);
    switch (shite->m_type)
    {
    case Shite::MONO_DIV:
    case Shite::SHITE_DIV:
    case Shite::MONO_WO_EXPR_DIV:
        return true;

    case Shite::MONO_ONLY:
        return ChIsMonoWarizan(shite->m_mono);

    default:
        return false;
    }
}

bool ChIsPrimTashizan(shared_ptr<Prim>& prim)
{
    assert(prim);
    if (prim->m_type == Prim::MONO)
        return ChIsMonoTashizan(prim->m_mono);
    else
        return false;
}

bool ChIsPrimKakezan(shared_ptr<Prim>& prim)
{
    assert(prim);
    if (prim->m_type == Prim::MONO)
        return ChIsMonoKakezan(prim->m_mono);
    else
        return false;
}

bool ChIsPrimHikizan(shared_ptr<Prim>& prim)
{
    assert(prim);
    if (prim->m_type == Prim::MONO)
        return ChIsMonoHikizan(prim->m_mono);
    else
        return false;
}

bool ChIsPrimWarizan(shared_ptr<Prim>& prim)
{
    assert(prim);
    if (prim->m_type == Prim::MONO)
        return ChIsMonoWarizan(prim->m_mono);
    else
        return false;
}

bool ChIsFactTashizan(shared_ptr<Fact>& fact)
{
    assert(fact);
    if (fact->m_type == Fact::SINGLE)
        return ChIsPrimTashizan(fact->m_prim);
    else
        return false;
}

bool ChIsFactKakezan(shared_ptr<Fact>& fact)
{
    assert(fact);
    if (fact->m_type == Fact::SINGLE)
        return ChIsPrimKakezan(fact->m_prim);
    else
        return false;
}

bool ChIsFactHikizan(shared_ptr<Fact>& fact)
{
    assert(fact);
    if (fact->m_type == Fact::SINGLE)
        return ChIsPrimHikizan(fact->m_prim);
    else
        return false;
}

bool ChIsFactWarizan(shared_ptr<Fact>& fact)
{
    assert(fact);
    if (fact->m_type == Fact::SINGLE)
        return ChIsPrimWarizan(fact->m_prim);
    else
        return false;
}

bool ChIsTermTashizan(shared_ptr<Term>& term)
{
    assert(term);
    switch (term->m_type)
    {
    case Term::MUL:
    case Term::DIV:
        return false;

    case Term::FACT_ONLY:
        return ChIsFactTashizan(term->m_fact);

    default:
        return false;
    }
}

bool ChIsTermKakezan(shared_ptr<Term>& term)
{
    assert(term);
    switch (term->m_type)
    {
    case Term::MUL:
        return true;

    case Term::DIV:
        return false;

    case Term::FACT_ONLY:
        return ChIsFactKakezan(term->m_fact);

    default:
        return false;
    }
}

bool ChIsTermHikizan(shared_ptr<Term>& term)
{
    assert(term);
    switch (term->m_type)
    {
    case Term::MUL:
    case Term::DIV:
        return false;

    case Term::FACT_ONLY:
        return ChIsFactHikizan(term->m_fact);

    default:
        return false;
    }
}

bool ChIsTermWarizan(shared_ptr<Term>& term)
{
    assert(term);
    switch (term->m_type)
    {
    case Term::MUL:
        return false;

    case Term::DIV:
        return true;

    case Term::FACT_ONLY:
        return ChIsFactWarizan(term->m_fact);

    default:
        return false;
    }
}

bool ChIsExprTashizan(shared_ptr<Expr>& expr)
{
    assert(expr);
    switch (expr->m_type)
    {
    case Expr::ADD:
        return true;

    case Expr::SUB:
    case Expr::ZERO:
        return false;

    case Expr::TERM_ONLY:
        return ChIsTermTashizan(expr->m_term);

    default:
        return false;
    }
}

bool ChIsExprKakezan(shared_ptr<Expr>& expr)
{
    assert(expr);
    switch (expr->m_type)
    {
    case Expr::ADD:
    case Expr::SUB:
    case Expr::ZERO:
        return false;

    case Expr::TERM_ONLY:
        return ChIsTermKakezan(expr->m_term);

    default:
        return false;
    }
}

bool ChIsExprHikizan(shared_ptr<Expr>& expr)
{
    assert(expr);
    switch (expr->m_type)
    {
    case Expr::ADD:
    case Expr::ZERO:
        return false;

    case Expr::SUB:
        return true;

    case Expr::TERM_ONLY:
        return ChIsTermHikizan(expr->m_term);

    default:
        return false;
    }
}

bool ChIsExprWarizan(shared_ptr<Expr>& expr)
{
    assert(expr);
    switch (expr->m_type)
    {
    case Expr::ADD:
    case Expr::SUB:
    case Expr::ZERO:
        return false;

    case Expr::TERM_ONLY:
        return ChIsTermWarizan(expr->m_term);

    default:
        return false;
    }
}

bool ChIsSurutoTashizan(shared_ptr<Suruto>& suruto)
{
    assert(suruto);
    switch (suruto->m_type)
    {
    case Suruto::EXPRLIST_ADD:
    case Suruto::MONO_ADD:
    case Suruto::SHITE_TASUTO:
        return true;

    case Suruto::MONO_ONLY:
        return ChIsMonoTashizan(suruto->m_mono);

    default:
        return false;
    }
}

bool ChIsSurutoKakezan(shared_ptr<Suruto>& suruto)
{
    assert(suruto);
    switch (suruto->m_type)
    {
    case Suruto::EXPRLIST_MUL:
    case Suruto::MONO_MUL:
    case Suruto::MONO_WO_EXPR_BAI:
    case Suruto::MONO_WO_BAI:
    case Suruto::SHITE_KAKERUTO:
        return true;

    case Suruto::MONO_ONLY:
        return ChIsMonoKakezan(suruto->m_mono);

    default:
        return false;
    }
}

bool ChIsSurutoHikizan(shared_ptr<Suruto>& suruto)
{
    assert(suruto);
    switch (suruto->m_type)
    {
    case Suruto::MONO_SUB:
    case Suruto::SHITE_HIKUTO:
    case Suruto::MONO_WO_EXPR_SUB:
        return true;

    case Suruto::MONO_ONLY:
        return ChIsMonoHikizan(suruto->m_mono);

    default:
        return false;
    }
}

bool ChIsSurutoWarizan(shared_ptr<Suruto>& suruto)
{
    assert(suruto);
    switch (suruto->m_type)
    {
    case Suruto::MONO_DIV:
    case Suruto::SHITE_WARUTO:
    case Suruto::MONO_DE_EXPR_DIV:
        return true;

    case Suruto::MONO_ONLY:
        return ChIsMonoWarizan(suruto->m_mono);

    default:
        return false;
    }
}

bool ChIsMonoTashizan(shared_ptr<Mono>& mono)
{
    assert(mono);
    switch (mono->m_type)
    {
    case Mono::EXPRLIST_ADD:
    case Mono::MONO_ADD:
    case Mono::SHITE_ADD:
    case Mono::MONO_TO_EXPRLIST_ADD:
    case Mono::MONO_TO_EXPR_ADD:
        return true;

    case Mono::MONO_ONLY:
        return ChIsMonoTashizan(mono->m_mono);

    case Mono::SHITE_ONLY:
        return ChIsShiteTashizan(mono->m_shite);

    case Mono::EXPR_ONLY:
        return ChIsExprTashizan(mono->m_expr);

    case Mono::TERM_ONLY:
        return ChIsTermTashizan(mono->m_term);

    case Mono::FACT_ONLY:
        return ChIsFactTashizan(mono->m_fact);

    case Mono::SURUTO_ONLY:
        return ChIsSurutoTashizan(mono->m_suruto);

    default:
        return false;
    }
}

bool ChIsMonoKakezan(shared_ptr<Mono>& mono)
{
    assert(mono);
    switch (mono->m_type)
    {
    case Mono::EXPRLIST_MUL:
    case Mono::MONO_MUL:
    case Mono::MONO_EXPR_BAI:
    case Mono::MONO_BAI:
    case Mono::SHITE_MUL:
    case Mono::SHITE_EXPR_BAI:
    case Mono::SHITE_BAI:
    case Mono::MONO_TO_EXPRLIST_MUL:
    case Mono::MONO_TO_EXPR_MUL:
        return true;

    case Mono::MONO_ONLY:
        return ChIsMonoKakezan(mono->m_mono);

    case Mono::SHITE_ONLY:
        return ChIsShiteKakezan(mono->m_shite);

    case Mono::EXPR_ONLY:
        return ChIsExprKakezan(mono->m_expr);

    case Mono::TERM_ONLY:
        return ChIsTermKakezan(mono->m_term);

    case Mono::FACT_ONLY:
        return ChIsFactKakezan(mono->m_fact);

    case Mono::SURUTO_ONLY:
        return ChIsSurutoKakezan(mono->m_suruto);

    default:
        return false;
    }
}

bool ChIsMonoHikizan(shared_ptr<Mono>& mono)
{
    assert(mono);
    switch (mono->m_type)
    {
    case Mono::EXPRLIST_SUB:
    case Mono::MONO_SUB:
    case Mono::SHITE_SUB:
    case Mono::MONO_TO_EXPR_SUB:
    case Mono::MONO_WO_EXPR_KARA_SUB:
        return true;

    case Mono::MONO_ONLY:
        return ChIsMonoHikizan(mono->m_mono);

    case Mono::SHITE_ONLY:
        return ChIsShiteHikizan(mono->m_shite);

    case Mono::EXPR_ONLY:
        return ChIsExprHikizan(mono->m_expr);

    case Mono::TERM_ONLY:
        return ChIsTermHikizan(mono->m_term);

    case Mono::FACT_ONLY:
        return ChIsFactHikizan(mono->m_fact);

    case Mono::SURUTO_ONLY:
        return ChIsSurutoHikizan(mono->m_suruto);

    default:
        return false;
    }
}

bool ChIsMonoWarizan(shared_ptr<Mono>& mono)
{
    assert(mono);
    switch (mono->m_type)
    {
    case Mono::EXPRLIST_DIV:
    case Mono::MONO_DIV:
    case Mono::SHITE_DIV:
    case Mono::MONO_TO_EXPR_DIV:
    case Mono::MONO_DE_EXPR_WO_DIV:
        return true;

    case Mono::MONO_ONLY:
        return ChIsMonoWarizan(mono->m_mono);

    case Mono::SHITE_ONLY:
        return ChIsShiteWarizan(mono->m_shite);

    case Mono::EXPR_ONLY:
        return ChIsExprWarizan(mono->m_expr);

    case Mono::TERM_ONLY:
        return ChIsTermWarizan(mono->m_term);

    case Mono::FACT_ONLY:
        return ChIsFactWarizan(mono->m_fact);

    case Mono::SURUTO_ONLY:
        return ChIsSurutoWarizan(mono->m_suruto);

    default:
        return false;
    }
}

void ChAnalyzeMonoMonoAmari(shared_ptr<Mono>& mono, shared_ptr<Mono>& mono2);
void ChAnalyzeMonoMonoShouToAmari(shared_ptr<Mono>& mono, shared_ptr<Mono>& mono2);

void ChAnalyzeMonoSurutoAmari(shared_ptr<Mono>& mono, shared_ptr<Suruto>& suruto)
{
    assert(mono);
    assert(suruto);
    Mono *m;
    switch (suruto->m_type)
    {
    case Suruto::MONO_DIV:
        ChAnalyzeMono(suruto->m_mono);
        ChAnalyzeExpr(suruto->m_expr);
        m = new Mono;
        m->m_type = Mono::MONO_MOD_EXPR;
        m->m_mono = suruto->m_mono;
        m->m_expr = suruto->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    case Suruto::MONO_ONLY:
        ChAnalyzeMonoMonoAmari(mono, suruto->m_mono);
        break;

    case Suruto::SHITE_WARUTO:
        ChAnalyzeShite(suruto->m_shite);
        ChAnalyzeExpr(suruto->m_expr);
        m = new Mono;
        m->m_type = Mono::SHITE_MOD_EXPR;
        m->m_shite = suruto->m_shite;
        m->m_expr = suruto->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    case Suruto::MONO_DE_EXPR_DIV:
        ChAnalyzeExpr(suruto->m_expr);
        ChAnalyzeMono(suruto->m_mono);
        m = new Mono;
        m->m_type = Mono::EXPR_MOD_MONO;
        m->m_expr = suruto->m_expr;
        m->m_mono = suruto->m_mono;
        mono = shared_ptr<Mono>(m);
        break;

    default:
        ChSetMessage(ch_not_warizan);
    }
}

void ChAnalyzeMonoShiteAmari(shared_ptr<Mono> mono, shared_ptr<Shite>& shite)
{
    assert(mono);
    assert(shite);
    Mono *m;
    switch (shite->m_type)
    {
    case Shite::MONO_DIV:
        ChAnalyzeMono(shite->m_mono);
        ChAnalyzeExpr(shite->m_expr);
        m = new Mono;
        m->m_type = Mono::MONO_MOD_EXPR;
        m->m_mono = shite->m_mono;
        m->m_expr = shite->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    case Shite::MONO_ONLY:
        ChAnalyzeMono(shite->m_mono);
        ChAnalyzeMonoMonoAmari(mono, shite->m_mono);
        break;

    case Shite::SHITE_DIV:
        ChAnalyzeShite(shite->m_shite);
        ChAnalyzeExpr(shite->m_expr);
        m = new Mono;
        m->m_type = Mono::SHITE_MOD_EXPR;
        m->m_shite = shite->m_shite;
        m->m_expr = shite->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    case Shite::MONO_WO_EXPR_DIV:
        ChAnalyzeMono(shite->m_mono);
        ChAnalyzeExpr(shite->m_expr);
        m = new Mono;
        m->m_type = Mono::MONO_MOD_EXPR;
        m->m_mono = shite->m_mono;
        m->m_expr = shite->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    default:
        ChSetMessage(ch_not_warizan);
    }
}

void ChAnalyzeMonoPrimAmari(shared_ptr<Mono>& mono, shared_ptr<Prim>& prim)
{
    assert(mono);
    assert(prim);
    switch (prim->m_type)
    {
    case Prim::MONO:
        ChAnalyzeMono(prim->m_mono);
        ChAnalyzeMonoMonoAmari(mono, prim->m_mono);
        break;

    default:
        ChSetMessage(ch_not_warizan);
    }
}

void ChAnalyzeMonoFactAmari(shared_ptr<Mono>& mono, shared_ptr<Fact>& fact)
{
    assert(mono);
    assert(fact);
    switch (fact->m_type)
    {
    case Fact::SINGLE:
        ChAnalyzePrim(fact->m_prim);
        ChAnalyzeMonoPrimAmari(mono, fact->m_prim);
        break;

    default:
        ChSetMessage(ch_not_warizan);
    }
}

void ChAnalyzeMonoTermAmari(shared_ptr<Mono>& mono, shared_ptr<Term>& term)
{
    assert(mono);
    assert(term);
    Mono *m;
    switch (term->m_type)
    {
    case Term::DIV:
        ChAnalyzeTerm(term->m_term);
        ChAnalyzeFact(term->m_fact);
        m = new Mono;
        m->m_type = Mono::TERM_MOD_FACT;
        m->m_term = term->m_term;
        m->m_fact = term->m_fact;
        mono = shared_ptr<Mono>(m);
        break;

    case Term::FACT_ONLY:
        ChAnalyzeFact(term->m_fact);
        ChAnalyzeMonoFactAmari(mono, term->m_fact);
        break;

    default:
        ChSetMessage(ch_not_warizan);
    }
}

void ChAnalyzeMonoExprAmari(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
{
    assert(mono);
    assert(expr);
    switch (expr->m_type)
    {
    case Expr::TERM_ONLY:
        ChAnalyzeTerm(expr->m_term);
        ChAnalyzeMonoTermAmari(mono, expr->m_term);
        break;

    default:
        ChSetMessage(ch_not_warizan);
    }
}

void ChAnalyzeMonoMonoAmari(shared_ptr<Mono>& mono, shared_ptr<Mono>& mono2)
{
    assert(mono);
    assert(mono2);
    Mono *m;
    switch (mono2->m_type)
    {
    case Mono::MONO_DIV:
    case Mono::MONO_TO_EXPR_DIV:
        ChAnalyzeMono(mono2->m_mono);
        ChAnalyzeExpr(mono2->m_expr);
        m = new Mono;
        m->m_type = Mono::MONO_MOD_EXPR;
        m->m_mono = mono2->m_mono;
        m->m_expr = mono2->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    case Mono::MONO_ONLY:
        ChAnalyzeMonoMonoAmari(mono, mono2->m_mono);
        break;

    case Mono::SHITE_DIV:
        ChAnalyzeShite(mono2->m_shite);
        ChAnalyzeExpr(mono2->m_expr);
        m = new Mono;
        m->m_type = Mono::SHITE_MOD_EXPR;
        m->m_shite = mono2->m_shite;
        m->m_expr = mono2->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    case Mono::SHITE_ONLY:
        ChAnalyzeMonoShiteAmari(mono, mono2->m_shite);
        break;

    case Mono::EXPR_ONLY:
        ChAnalyzeExpr(mono2->m_expr);
        ChAnalyzeMonoExprAmari(mono, mono2->m_expr);
        break;

    case Mono::MONO_DE_EXPR_WO_DIV:
        ChAnalyzeMono(mono2->m_mono);
        ChAnalyzeExpr(mono2->m_expr);
        m = new Mono;
        m->m_type = Mono::EXPR_MOD_MONO;
        m->m_expr = mono2->m_expr;
        m->m_mono = mono2->m_mono;
        mono = shared_ptr<Mono>(m);
        break;

    case Mono::TERM_ONLY:
        ChAnalyzeTerm(mono2->m_term);
        ChAnalyzeMonoTermAmari(mono, mono2->m_term);
        break;

    case Mono::FACT_ONLY:
        ChAnalyzeFact(mono2->m_fact);
        ChAnalyzeMonoFactAmari(mono, mono2->m_fact);
        break;

    case Mono::SURUTO_ONLY:
        ChAnalyzeSuruto(mono2->m_suruto);
        ChAnalyzeMonoSurutoAmari(mono, mono2->m_suruto);
        break;

    default:
        ChSetMessage(ch_not_warizan);
    }
}

void ChAnalyzeMonoShiteShouToAmari(shared_ptr<Mono>& mono, shared_ptr<Shite>& shite)
{
    assert(mono);
    assert(shite);
    Mono *m;
    switch (shite->m_type)
    {
    case Shite::MONO_DIV:
        ChAnalyzeMono(shite->m_mono);
        ChAnalyzeExpr(shite->m_expr);
        m = new Mono;
        m->m_type = Mono::MONO_DIVMOD_EXPR;
        m->m_mono = shite->m_mono;
        m->m_expr = shite->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    case Shite::MONO_ONLY:
        ChAnalyzeMono(shite->m_mono);
        ChAnalyzeMonoMonoShouToAmari(mono, shite->m_mono);
        break;

    case Shite::SHITE_DIV:
        ChAnalyzeShite(shite->m_shite);
        ChAnalyzeExpr(shite->m_expr);
        m = new Mono;
        m->m_type = Mono::SHITE_DIVMOD_EXPR;
        m->m_shite = shite->m_shite;
        m->m_expr = shite->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    case Shite::MONO_WO_EXPR_DIV:
        ChAnalyzeMono(shite->m_mono);
        ChAnalyzeExpr(shite->m_expr);
        m = new Mono;
        m->m_type = Mono::MONO_DIVMOD_EXPR;
        m->m_mono = shite->m_mono;
        m->m_expr = shite->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    default:
        ChSetMessage(ch_not_warizan);
    }
}

void ChAnalyzeMonoPrimShouToAmari(shared_ptr<Mono>& mono, shared_ptr<Prim>& prim)
{
    assert(mono);
    assert(prim);
    switch (prim->m_type)
    {
    case Prim::MONO:
        ChAnalyzeMono(prim->m_mono);
        ChAnalyzeMonoMonoShouToAmari(mono, prim->m_mono);
        break;

    default:
        ChSetMessage(ch_not_warizan);
    }
}

void ChAnalyzeMonoFactShouToAmari(shared_ptr<Mono>& mono, shared_ptr<Fact>& fact)
{
    assert(mono);
    assert(fact);
    switch (fact->m_type)
    {
    case Fact::SINGLE:
        ChAnalyzePrim(fact->m_prim);
        ChAnalyzeMonoPrimShouToAmari(mono, fact->m_prim);
        break;

    default:
        ChSetMessage(ch_not_warizan);
    }
}

void ChAnalyzeMonoTermShouToAmari(shared_ptr<Mono>& mono, shared_ptr<Term>& term)
{
    assert(mono);
    assert(term);
    Mono *m;
    switch (term->m_type)
    {
    case Term::DIV:
        ChAnalyzeTerm(term->m_term);
        ChAnalyzeFact(term->m_fact);
        m = new Mono;
        m->m_type = Mono::TERM_DIVMOD_FACT;
        m->m_term = term->m_term;
        m->m_fact = term->m_fact;
        mono = shared_ptr<Mono>(m);
        break;

    case Term::FACT_ONLY:
        ChAnalyzeFact(term->m_fact);
        ChAnalyzeMonoFactShouToAmari(mono, term->m_fact);
        break;

    default:
        ChSetMessage(ch_not_warizan);
    }
}

void ChAnalyzeMonoExprShouToAmari(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
{
    assert(mono);
    assert(expr);
    switch (expr->m_type)
    {
    case Expr::TERM_ONLY:
        ChAnalyzeTerm(expr->m_term);
        ChAnalyzeMonoTermShouToAmari(mono, expr->m_term);
        break;

    default:
        ChSetMessage(ch_not_warizan);
    }
}

void ChAnalyzeMonoSurutoShouToAmari(shared_ptr<Mono>& mono, shared_ptr<Suruto>& suruto)
{
    assert(mono);
    assert(suruto);
    Mono *m;
    switch (suruto->m_type)
    {
    case Suruto::MONO_DIV:
        ChAnalyzeMono(suruto->m_mono);
        ChAnalyzeExpr(suruto->m_expr);
        m = new Mono;
        m->m_type = Mono::MONO_DIVMOD_EXPR;
        m->m_mono = suruto->m_mono;
        m->m_expr = suruto->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    case Suruto::MONO_ONLY:
        ChAnalyzeMonoMonoShouToAmari(mono, suruto->m_mono);
        break;

    case Suruto::SHITE_WARUTO:
        ChAnalyzeShite(suruto->m_shite);
        ChAnalyzeExpr(suruto->m_expr);
        m = new Mono;
        m->m_type = Mono::SHITE_DIVMOD_EXPR;
        m->m_shite = suruto->m_shite;
        m->m_expr = suruto->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    case Suruto::MONO_DE_EXPR_DIV:
        ChAnalyzeExpr(suruto->m_expr);
        ChAnalyzeMono(suruto->m_mono);
        m = new Mono;
        m->m_type = Mono::EXPR_DIVMOD_MONO;
        m->m_expr = suruto->m_expr;
        m->m_mono = suruto->m_mono;
        mono = shared_ptr<Mono>(m);
        break;

    default:
        ChSetMessage(ch_not_warizan);
    }
}

void ChAnalyzeMonoMonoShouToAmari(shared_ptr<Mono>& mono, shared_ptr<Mono>& mono2)
{
    assert(mono);
    assert(mono2);
    Mono *m;
    switch (mono2->m_type)
    {
    case Mono::MONO_DIV:
    case Mono::MONO_TO_EXPR_DIV:
        ChAnalyzeMono(mono2->m_mono);
        ChAnalyzeExpr(mono2->m_expr);
        m = new Mono;
        m->m_type = Mono::MONO_DIVMOD_EXPR;
        m->m_mono = mono2->m_mono;
        m->m_expr = mono2->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    case Mono::MONO_ONLY:
        ChAnalyzeMonoMonoShouToAmari(mono, mono2->m_mono);
        break;

    case Mono::SHITE_DIV:
        ChAnalyzeShite(mono2->m_shite);
        ChAnalyzeExpr(mono2->m_expr);
        m = new Mono;
        m->m_type = Mono::SHITE_DIVMOD_EXPR;
        m->m_shite = mono2->m_shite;
        m->m_expr = mono2->m_expr;
        mono = shared_ptr<Mono>(m);
        break;

    case Mono::SHITE_ONLY:
        ChAnalyzeMonoShiteShouToAmari(mono, mono2->m_shite);
        break;

    case Mono::EXPR_ONLY:
        ChAnalyzeExpr(mono2->m_expr);
        ChAnalyzeMonoExprShouToAmari(mono, mono2->m_expr);
        break;

    case Mono::MONO_DE_EXPR_WO_DIV:
        ChAnalyzeMono(mono2->m_mono);
        ChAnalyzeExpr(mono2->m_expr);
        m = new Mono;
        m->m_type = Mono::EXPR_DIVMOD_MONO;
        m->m_expr = mono2->m_expr;
        m->m_mono = mono2->m_mono;
        mono = shared_ptr<Mono>(m);
        break;

    case Mono::TERM_ONLY:
        ChAnalyzeTerm(mono2->m_term);
        ChAnalyzeMonoTermShouToAmari(mono, mono2->m_term);
        break;

    case Mono::FACT_ONLY:
        ChAnalyzeFact(mono2->m_fact);
        ChAnalyzeMonoFactShouToAmari(mono, mono2->m_fact);
        break;

    case Mono::SURUTO_ONLY:
        ChAnalyzeSuruto(mono2->m_suruto);
        ChAnalyzeMonoSurutoShouToAmari(mono, mono2->m_suruto);
        break;

    default:
        ChSetMessage(ch_not_warizan);
    }
}

void ChAnalyzeMonoSonoAmari(shared_ptr<Mono>& mono)
{
    assert(mono);
    Sentence *sentence = Calc_H::s_sentence_prev.get();
    switch (sentence->m_type)
    {
    case Sentence::MONO:
        ChAnalyzeMono(sentence->m_mono);
        ChAnalyzeMonoMonoAmari(mono, sentence->m_mono);
        break;

    case Sentence::EXPR:
        ChAnalyzeExpr(sentence->m_expr);
        ChAnalyzeMonoExprAmari(mono, sentence->m_expr);
        break;

    case Sentence::SURUTO:
        ChAnalyzeSuruto(sentence->m_suruto);
        ChAnalyzeMonoSurutoAmari(mono, sentence->m_suruto);
        break;

    case Sentence::SHITE:
        ChAnalyzeShite(sentence->m_shite);
        ChAnalyzeMonoShiteAmari(mono, sentence->m_shite);
        break;

    default:
        ChSetMessage(ch_not_warizan);
    }
}

void ChAnalyzeMono(shared_ptr<Mono>& mono)
{
    assert(mono);
    Mono *m;
    switch (mono->m_type)
    {
    case Mono::EXPRLIST_ADD:
        ChAnalyzeExprList(mono->m_exprlist);
        break;

    case Mono::EXPRLIST_MUL:
        ChAnalyzeExprList(mono->m_exprlist);
        break;

    case Mono::EXPRLIST_SUB:
        ChAnalyzeExprList(mono->m_exprlist);
        break;

    case Mono::EXPRLIST_DIV:
        ChAnalyzeExprList(mono->m_exprlist);
        break;

    case Mono::MONO_ADD:
    case Mono::MONO_MUL:
    case Mono::MONO_SUB:
    case Mono::MONO_DIV:
    case Mono::MONO_EXPR_BAI:
    case Mono::MONO_TO_EXPR_ADD:
    case Mono::MONO_TO_EXPR_MUL:
    case Mono::MONO_TO_EXPR_SUB:
    case Mono::MONO_TO_EXPR_DIV:
    case Mono::MONO_WO_EXPR_KARA_SUB:
    case Mono::MONO_DE_EXPR_WO_DIV:
    case Mono::MONO_EXPR_JOU:
    case Mono::MONO_NO_EXPR_PERCENT:
    case Mono::MONO_MOD_EXPR:
    case Mono::EXPR_MOD_MONO:
        ChAnalyzeMono(mono->m_mono);
        ChAnalyzeExpr(mono->m_expr);
        break;

    case Mono::MONO_ONLY:
        ChAnalyzeMono(mono->m_mono);
        mono = mono->m_mono;
        break;

    case Mono::MONO_BAI:
        ChAnalyzeMono(mono->m_mono);
        break;

    case Mono::SHITE_ADD:
    case Mono::SHITE_MUL:
    case Mono::SHITE_SUB:
    case Mono::SHITE_DIV:
    case Mono::SHITE_EXPR_BAI:
        ChAnalyzeShite(mono->m_shite);
        ChAnalyzeExpr(mono->m_expr);
        break;

    case Mono::SHITE_BAI:
    case Mono::SHITE_ONLY:
        ChAnalyzeShite(mono->m_shite);
        break;

    case Mono::EXPR_ONLY:
        ChAnalyzeExpr(mono->m_expr);
        break;

    case Mono::MONO_TO_EXPRLIST_ADD:
    case Mono::MONO_TO_EXPRLIST_MUL:
        ChAnalyzeMono(mono->m_mono);
        ChAnalyzeExprList(mono->m_exprlist);
        break;

    case Mono::TERM_ONLY:
        ChAnalyzeTerm(mono->m_term);
        break;

    case Mono::FACT_ONLY:
        ChAnalyzeFact(mono->m_fact);
        break;

    case Mono::SURUTO_ONLY:
        ChAnalyzeSuruto(mono->m_suruto);
        break;

    case Mono::SHITE_EXPR_JOU:
        ChAnalyzeShite(mono->m_shite);
        ChAnalyzeExpr(mono->m_expr);
        break;

    case Mono::SHITE_HEIHOU:
    case Mono::SHITE_RIPPOU:
        ChAnalyzeShite(mono->m_shite);
        break;

    case Mono::MONO_HEIHOU:
    case Mono::MONO_RIPPOU:
    case Mono::MONO_FUNC1ARG:
        ChAnalyzeMono(mono->m_mono);
        break;

    case Mono::SHITE_MOD_EXPR:
        ChAnalyzeShite(mono->m_shite);
        ChAnalyzeMono(mono->m_mono);
        break;

    case Mono::TERM_MOD_FACT:
        ChAnalyzeTerm(mono->m_term);
        ChAnalyzeFact(mono->m_fact);
        break;

    case Mono::SHOUSUU:
        ChAnalyzeShite(mono->m_shite);
        break;

    case Mono::MONO_NO_SHOUSUU:
        ChAnalyzeMono(mono->m_mono);
        break;

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
            ChAnalyzeExpr(mono->m_mono->m_expr);
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = mono->m_mono->m_expr;
            mono = shared_ptr<Mono>(m);
            break;

        case Mono::MONO_TO_EXPRLIST_ADD:
            ChAnalyzeExprList(mono->m_mono->m_exprlist);
            m = new Mono;
            m->m_type = Mono::EXPRLIST_ADD;
            m->m_exprlist = mono->m_mono->m_exprlist;
            mono = shared_ptr<Mono>(m);
            break;

        default:
            ChSetMessage(ch_not_tashizan);
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
            ChAnalyzeExpr(mono->m_mono->m_expr);
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = mono->m_mono->m_expr;
            mono = shared_ptr<Mono>(m);
            break;

        case Mono::MONO_TO_EXPRLIST_MUL:
            ChAnalyzeExprList(mono->m_mono->m_exprlist);
            m = new Mono;
            m->m_type = Mono::EXPRLIST_MUL;
            m->m_exprlist = mono->m_mono->m_exprlist;
            mono = shared_ptr<Mono>(m);
            break;

        default:
            ChSetMessage(ch_not_kakezan);
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
            ChAnalyzeExpr(mono->m_mono->m_expr);
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = mono->m_mono->m_expr;
            mono = shared_ptr<Mono>(m);
            break;

        default:
            ChSetMessage(ch_not_hikizan);
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
            ChAnalyzeExpr(mono->m_mono->m_expr);
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = mono->m_mono->m_expr;
            mono = shared_ptr<Mono>(m);
            break;

        default:
            ChSetMessage(ch_not_hikizan);
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
                ChAnalyzeExprList(mono->m_mono->m_exprlist);
                ExprList *el = mono->m_mono->m_exprlist.get();
                assert(el->size() >= 1);
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
            ChAnalyzeMono(mono->m_mono->m_mono);
            m = new Mono;
            m->m_type = Mono::MONO_ONLY;
            m->m_mono = mono->m_mono->m_mono;
            mono = shared_ptr<Mono>(m);
            break;

        case Mono::SHITE_ADD:
            ChAnalyzeShite(mono->m_mono->m_shite);
            m = new Mono;
            m->m_type = Mono::SHITE_ONLY;
            m->m_shite = mono->m_mono->m_shite;
            mono = shared_ptr<Mono>(m);
            break;

        default:
            ChSetMessage(ch_not_tashizan);
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
                ChAnalyzeExprList(mono->m_mono->m_exprlist);
                ExprList *el = mono->m_mono->m_exprlist.get();
                assert(el->size() >= 1);
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
            ChAnalyzeMono(mono->m_mono->m_mono);
            m = new Mono;
            m->m_type = Mono::MONO_ONLY;
            m->m_mono = mono->m_mono->m_mono;
            mono = shared_ptr<Mono>(m);
            break;

        case Mono::SHITE_MUL:
            ChAnalyzeShite(mono->m_mono->m_shite);
            m = new Mono;
            m->m_type = Mono::SHITE_ONLY;
            m->m_shite = mono->m_mono->m_shite;
            mono = shared_ptr<Mono>(m);
            break;

        default:
            ChSetMessage(ch_not_kakezan);
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
                ChAnalyzeExprList(mono->m_mono->m_exprlist);
                ExprList *el = mono->m_mono->m_exprlist.get();
                assert(el->size() >= 1);
                shared_ptr<Expr> e = (*el)[0];
                m = new Mono;
                m->m_type = Mono::EXPR_ONLY;
                m->m_expr = e;
                mono = shared_ptr<Mono>(m);
            }
            break;
            
        case Mono::MONO_SUB:
        case Mono::MONO_TO_EXPR_SUB:
            ChAnalyzeMono(mono->m_mono->m_mono);
            m = new Mono;
            m->m_type = Mono::MONO_ONLY;
            m->m_mono = mono->m_mono->m_mono;
            mono = shared_ptr<Mono>(m);
            break;

        case Mono::MONO_WO_EXPR_KARA_SUB:
            ChAnalyzeExpr(mono->m_mono->m_expr);
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = mono->m_mono->m_expr;
            mono = shared_ptr<Mono>(m);
            break;

        case Mono::SHITE_SUB:
            ChAnalyzeShite(mono->m_mono->m_shite);
            m = new Mono;
            m->m_type = Mono::SHITE_ONLY;
            m->m_shite = mono->m_mono->m_shite;
            mono = shared_ptr<Mono>(m);
            break;

        default:
            ChSetMessage(ch_not_hikizan);
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
                ChAnalyzeExprList(mono->m_mono->m_exprlist);
                ExprList *el = mono->m_mono->m_exprlist.get();
                assert(el->size() >= 1);
                shared_ptr<Expr> e = (*el)[0];
                m = new Mono;
                m->m_type = Mono::EXPR_ONLY;
                m->m_expr = e;
                mono = shared_ptr<Mono>(m);
            }
            break;
            
        case Mono::MONO_DIV:
        case Mono::MONO_TO_EXPR_DIV:
            ChAnalyzeMono(mono->m_mono->m_mono);
            m = new Mono;
            m->m_type = Mono::MONO_ONLY;
            m->m_mono = mono->m_mono->m_mono;
            mono = shared_ptr<Mono>(m);
            break;

        case Mono::MONO_DE_EXPR_WO_DIV:
            ChAnalyzeExpr(mono->m_mono->m_expr);
            m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = mono->m_mono->m_expr;
            mono = shared_ptr<Mono>(m);
            break;

        case Mono::SHITE_DIV:
            ChAnalyzeShite(mono->m_mono->m_shite);
            m = new Mono;
            m->m_type = Mono::SHITE_ONLY;
            m->m_shite = mono->m_mono->m_shite;
            mono = shared_ptr<Mono>(m);
            break;

        default:
            ChSetMessage(ch_not_warizan);
            break;
        }
        break;

    case Mono::SORE_NO_TASUKAZU:
        ChAnalyzeMonoPrevSentenceTasukazu(mono);
        break;

    case Mono::SORE_NO_KAKERUKAZU:
        ChAnalyzeMonoPrevSentenceKakerukazu(mono);
        break;

    case Mono::SORE_NO_HIKUKAZU:
        ChAnalyzeMonoPrevSentenceHikukazu(mono);
        break;

    case Mono::SORE_NO_WARUKAZU:
        ChAnalyzeMonoPrevSentenceWarukazu(mono);
        break;

    case Mono::SORE_NO_TASARERUKAZU:
        ChAnalyzeMonoPrevSentenceTasarerukazu(mono);
        break;

    case Mono::SORE_NO_KAKERARERUKAZU:
        ChAnalyzeMonoPrevSentenceKakerarerukazu(mono);
        break;

    case Mono::SORE_NO_HIKARERUKAZU:
        ChAnalyzeMonoPrevSentenceHikarerukazu(mono);
        break;

    case Mono::SORE_NO_WARARERUKAZU:
        ChAnalyzeMonoPrevSentenceWararerukazu(mono);
        break;

    case Mono::TASHIZAN:
        ChAnalyzeMono(mono->m_mono);
        if (!ChIsMonoTashizan(mono->m_mono))
            ChSetMessage(ch_not_tashizan);
        mono = mono->m_mono;
        break;

    case Mono::KAKEZAN:
        ChAnalyzeMono(mono->m_mono);
        if (!ChIsMonoKakezan(mono->m_mono))
            ChSetMessage(ch_not_kakezan);
        mono = mono->m_mono;
        break;

    case Mono::HIKIZAN:
        ChAnalyzeMono(mono->m_mono);
        if (!ChIsMonoHikizan(mono->m_mono))
            ChSetMessage(ch_not_hikizan);
        mono = mono->m_mono;
        break;

    case Mono::WARIZAN:
        ChAnalyzeMono(mono->m_mono);
        if (!ChIsMonoWarizan(mono->m_mono))
            ChSetMessage(ch_not_warizan);
        mono = mono->m_mono;
        break;

    case Mono::SURUTO_WA:
        ChAnalyzeSuruto(mono->m_suruto);
        if (!ChIsSurutoTashizan(mono->m_suruto))
            ChSetMessage(ch_not_tashizan);
        m = new Mono;
        m->m_type = Mono::SURUTO_ONLY;
        m->m_suruto = mono->m_suruto;
        mono = shared_ptr<Mono>(m);
        break;

    case Mono::SURUTO_SEKI:
        ChAnalyzeSuruto(mono->m_suruto);
        if (!ChIsSurutoKakezan(mono->m_suruto))
            ChSetMessage(ch_not_kakezan);
        m = new Mono;
        m->m_type = Mono::SURUTO_ONLY;
        m->m_suruto = mono->m_suruto;
        mono = shared_ptr<Mono>(m);
        break;

    case Mono::SURUTO_SA:
        ChAnalyzeSuruto(mono->m_suruto);
        if (!ChIsSurutoHikizan(mono->m_suruto))
            ChSetMessage(ch_not_hikizan);
        m = new Mono;
        m->m_type = Mono::SURUTO_ONLY;
        m->m_suruto = mono->m_suruto;
        mono = shared_ptr<Mono>(m);
        break;

    case Mono::SURUTO_SHOU:
        ChAnalyzeSuruto(mono->m_suruto);
        if (!ChIsSurutoWarizan(mono->m_suruto))
            ChSetMessage(ch_not_warizan);
        m = new Mono;
        m->m_type = Mono::SURUTO_ONLY;
        m->m_suruto = mono->m_suruto;
        mono = shared_ptr<Mono>(m);
        break;

    case Mono::SURUTO_AMARI:
        ChAnalyzeSuruto(mono->m_suruto);
        ChAnalyzeMonoSurutoAmari(mono, mono->m_suruto);
        break;

    case Mono::MONO_NO_AMARI:
        ChAnalyzeMono(mono->m_mono);
        ChAnalyzeMonoMonoAmari(mono, mono->m_mono);
        break;

    case Mono::SORE_NO_AMARI:
        ChAnalyzeMonoSonoAmari(mono);
        break;

    case Mono::EXPRLIST_VECFUNC:
        ChAnalyzeExprList(mono->m_exprlist);
        break;

    case Mono::MONO_SHOU_TO_AMARI:
        ChAnalyzeMono(mono->m_mono);
        ChAnalyzeMonoMonoShouToAmari(mono, mono->m_mono);
        break;

    case Mono::SURUTO_SHOU_TO_AMARI:
        ChAnalyzeSuruto(mono->m_suruto);
        ChAnalyzeMonoSurutoShouToAmari(mono, mono->m_suruto);
        break;

    case Mono::MONO_HEIHOUKON:
    case Mono::MONO_KAIJOU:
        ChAnalyzeMono(mono->m_mono);
        break;

    case Mono::DOMS_SUM:
    case Mono::DOMS_PROD:
        ChAnalyzeDoms(mono->m_doms);
        break;

    case Mono::MONO_EXPR_WARIBIKI:
    case Mono::MONO_EXPR_WARIMASHI:
        ChAnalyzeMono(mono->m_mono);
        ChAnalyzeExpr(mono->m_expr);
        break;

    case Mono::EXPRLIST_NOUCHI_DOMS:
        ChAnalyzeExprList(mono->m_exprlist);
        ChAnalyzeDoms(mono->m_doms);
        break;

    default:
        break;
    }
}

void ChAnalyzeExprList(shared_ptr<ExprList>& exprlist)
{
    assert(exprlist);
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
    assert(prim);
    switch (prim->m_type)
    {
    case Prim::MONO:
        ChAnalyzeMono(prim->m_mono);
        break;

    case Prim::MINUS:
    case Prim::BUNSUU:
    case Prim::TAIBUNSUU:
    case Prim::FUNC1ARG:
    case Prim::DO:
        ChAnalyzePrim(prim->m_prim);
        break;

    case Prim::FUNC1ARG_HEIHOU:
    case Prim::FUNC1ARG_RIPPOU:
    case Prim::FUNC1ARG_JOU:
    case Prim::FACTORIAL:
        ChAnalyzePrim(prim->m_prim);
        break;

    default:
        break;
    }
}

void ChAnalyzeFact(shared_ptr<Fact>& fact)
{
    assert(fact);
    switch (fact->m_type)
    {
    case Fact::POW:
    case Fact::PERCENT:
    case Fact::WARIBIKI:
    case Fact::WARIMASHI:
        ChAnalyzeFact(fact->m_fact);
        ChAnalyzePrim(fact->m_prim);
        break;

    case Fact::POW2:
    case Fact::POW3:
    case Fact::KAIJOU:
    case Fact::HEIHOUKON:
        ChAnalyzeFact(fact->m_fact);
        break;

    case Fact::SINGLE:
        ChAnalyzePrim(fact->m_prim);
        break;
    }
}

void ChAnalyzeTerm(shared_ptr<Term>& term)
{
    assert(term);
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
    assert(shite);
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
    case Shite::SHITE_EXPR_JOU:
        ChAnalyzeShite(shite->m_shite);
        ChAnalyzeExpr(shite->m_expr);
        break;

    case Shite::SHITE_BAI:
    case Shite::SHITE_HEIHOU:
    case Shite::SHITE_RIPPOU:
    case Shite::SHOUSUU:
        ChAnalyzeShite(shite->m_shite);
        break;
    }
}

void ChAnalyzeExpr(shared_ptr<Expr>& expr)
{
    assert(expr);
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

    case Expr::ZERO:
        break;
    }
}

void ChAnalyzeSuruto(shared_ptr<Suruto>& suruto)
{
    assert(suruto);
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
    case Suruto::MONO_WO_SHOUSUU:
        ChAnalyzeMono(suruto->m_mono);
        break;

    case Suruto::SHITE_TASUTO:
    case Suruto::SHITE_KAKERUTO:
    case Suruto::SHITE_HIKUTO:
    case Suruto::SHITE_WARUTO:
        ChAnalyzeShite(suruto->m_shite);
        ChAnalyzeExpr(suruto->m_expr);
        break;

    case Suruto::SHOUSUU:
        ChAnalyzeShite(suruto->m_shite);
        break;

    default:
        break;
    }
}

void ChAnalyzeSentence(shared_ptr<Sentence>& sentence)
{
    assert(sentence);
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

    case Sentence::EXPRLIST_ADD:
    case Sentence::EXPRLIST_MUL:
        ChAnalyzeExprList(sentence->m_exprlist);
        break;

    case Sentence::SHITE:
        ChAnalyzeShite(sentence->m_shite);
        break;

    case Sentence::DOMS_IS_DOMS:
        ChAnalyzeDoms(sentence->m_doms1);
        ChAnalyzeDoms(sentence->m_doms2);
        break;

    case Sentence::DOMS_IS_CNSTR:
        ChAnalyzeDoms(sentence->m_doms1);
        ChAnalyzeCnstr(sentence->m_cnstr);
        break;

    case Sentence::MONO_IS_DOMS:
        ChAnalyzeMono(sentence->m_mono);
        ChAnalyzeDoms(sentence->m_doms2);
        break;

    case Sentence::MONO_IS_CNSTR:
        ChAnalyzeMono(sentence->m_mono);
        ChAnalyzeCnstr(sentence->m_cnstr);
        break;

    case Sentence::MONO_IS_CNSTRED_BUNSUU:
        ChAnalyzeMono(sentence->m_mono);
        ChAnalyzeCnstr(sentence->m_cnstr);
        break;

    case Sentence::MONO_IS_BUNSUU:
        ChAnalyzeMono(sentence->m_mono);
        break;

    case Sentence::MONO_IS_CNSTRED_SHOUSUU:
        ChAnalyzeMono(sentence->m_mono);
        ChAnalyzeCnstr(sentence->m_cnstr);
        break;

    case Sentence::MONO_IS_SHOUSUU:
    case Sentence::MONO_IS_TASHIZAN:
    case Sentence::MONO_IS_HIKIZAN:
    case Sentence::MONO_IS_KAKEZAN:
    case Sentence::MONO_IS_WARIZAN:
        ChAnalyzeMono(sentence->m_mono);
        break;

    case Sentence::MONO_IS_MONO:
        ChAnalyzeMono(sentence->m_mono);
        ChAnalyzeMono(sentence->m_mono2);
        break;

    case Sentence::SURUTO_TASHIZAN:
    case Sentence::SURUTO_HIKIZAN:
    case Sentence::SURUTO_KAKEZAN:
    case Sentence::SURUTO_WARIZAN:
        ChAnalyzeSuruto(sentence->m_suruto);
        break;

    case Sentence::DOMS_IS_EXPRLIST:
        ChAnalyzeDoms(sentence->m_doms1);
        ChAnalyzeExprList(sentence->m_exprlist);
        break;

    case Sentence::DOMS_IS_EXPR:
        ChAnalyzeDoms(sentence->m_doms1);
        ChAnalyzeExpr(sentence->m_expr);
        break;

    case Sentence::DOMS_IS_WHAT:
        ChAnalyzeDoms(sentence->m_doms1);
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
    size_t i = 0;
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
    const size_t len = str.size();

    ok = true;
    for (size_t i = 0; i < len; ++i)
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

std::string ChGetJpnNumber1(
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
    if (num >= ch_muryoutaisuu * 10000)
        return "おおきすぎるかず";
    if (num >= ch_muryoutaisuu)
    {
        str += ChGetJpnNumber1(num.to_i() / ch_muryoutaisuu, false);
        str += "むりょうたいすう";
        str += ChGetJpnNumber1(num % ch_muryoutaisuu, false);
        return str;
    }
    assert(num < ch_muryoutaisuu);
    ///
    if (num >= ch_fukashigi)
    {
        str += ChGetJpnNumber1(num.to_i() / ch_fukashigi, false);
        str += "ふかしぎ";
        str += ChGetJpnNumber1(num % ch_fukashigi, false);
        return str;
    }
    assert(num < ch_fukashigi);
    if (num >= ch_nayuta)
    {
        str += ChGetJpnNumber1(num.to_i() / ch_nayuta, false);
        str += "なゆた";
        str += ChGetJpnNumber1(num % ch_nayuta, false);
        return str;
    }
    assert(num < ch_nayuta);
    if (num >= ch_asougi)
    {
        str += ChGetJpnNumber1(num.to_i() / ch_asougi, false);
        str += "あそうぎ";
        str += ChGetJpnNumber1(num % ch_asougi, false);
        return str;
    }
    assert(num < ch_asougi);
    if (num >= ch_gougasha)
    {
        str += ChGetJpnNumber1(num.to_i() / ch_gougasha, false);
        str += "ごうがしゃ";
        str += ChGetJpnNumber1(num % ch_gougasha, false);
        return str;
    }
    assert(num < ch_gougasha);
    if (num >= ch_goku)
    {
        str += ChGetJpnNumber1(num.to_i() / ch_goku, false);
        str += "ごく";
        str += ChGetJpnNumber1(num % ch_goku, false);
        return str;
    }
    assert(num < ch_goku);
    if (num >= ch_sai)
    {
        str += ChGetJpnNumber1(num.to_i() / ch_sai, false);
        str += "さい";
        str += ChGetJpnNumber1(num % ch_sai, false);
        return str;
    }
    assert(num < ch_sai);
    if (num >= ch_sei)
    {
        str += ChGetJpnNumber1(num.to_i() / ch_sei, false);
        str += "せい";
        str += ChGetJpnNumber1(num % ch_sei, false);
        return str;
    }
    assert(num < ch_sei);
    if (num >= ch_kan)
    {
        str += ChGetJpnNumber1(num.to_i() / ch_kan, false);
        str += "かん";
        str += ChGetJpnNumber1(num % ch_kan, false);
        return str;
    }
    assert(num < ch_kan);
    if (num >= ch_kou)
    {
        str += ChGetJpnNumber1(num.to_i() / ch_kou, false);
        str += "こう";
        str += ChGetJpnNumber1(num % ch_kou, false);
        return str;
    }
    assert(num < ch_kou);
    if (num >= ch_jou)
    {
        str += ChGetJpnNumber1(num.to_i() / ch_jou, false);
        str += "じょう";
        str += ChGetJpnNumber1(num % ch_jou, false);
        return str;
    }
    assert(num < ch_jou);
    if (num >= ch_jo)
    {
        str += ChGetJpnNumber1(num.to_i() / ch_jo, false);
        str += "じょ";
        str += ChGetJpnNumber1(num % ch_jo, false);
        return str;
    }
    assert(num < ch_jo);
    if (num >= ch_gai)
    {
        str += ChGetJpnNumber1(num.to_i() / ch_gai, false);
        str += "がい";
        str += ChGetJpnNumber1(num % ch_gai, false);
        return str;
    }
    assert(num < ch_gai);
    if (num >= ch_kei)
    {
        str += ChGetJpnNumber1(num.to_i() / ch_kei, false);
        str += "けい";
        str += ChGetJpnNumber1(num % ch_kei, false);
        return str;
    }
    assert(num < ch_kei);
    if (num >= ch_chou)
    {
        str += ChGetJpnNumber1(num.to_i() / ch_chou, false);
        str += "ちょう";
        str += ChGetJpnNumber1(num % ch_chou, false);
        return str;
    }
    assert(num < ch_chou);
    if (num >= ch_oku)
    {
        str += ChGetJpnNumber1(num.to_i() / ch_oku, false);
        str += "おく";
        str += ChGetJpnNumber1(num % ch_oku, false);
        return str;
    }
    assert(num < ch_oku);
    if (num >= ch_man)
    {
        str += ChGetJpnNumber1(num.to_i() / ch_man, false);
        str += "まん";
        str += ChGetJpnNumber1(num % ch_man, false);
        return str;
    }
    assert(num < ch_man);
    if (num >= ch_sen)
    {
        str += ChGetJpnNumber1(num.to_i() / ch_sen, false);
        str += "せん";
        str += ChGetJpnNumber1(num % ch_sen, false);
        return str;
    }
    assert(num < ch_sen);
    if (num >= ch_hyaku)
    {
        str += ChGetJpnNumber1(num.to_i() / ch_hyaku, false);
        str += "ひゃく";
        str += ChGetJpnNumber1(num % ch_hyaku, false);
        return str;
    }
    assert(num < ch_hyaku);
    if (num >= ch_juu)
    {
        str += ChGetJpnNumber1(num.to_i() / ch_juu, false);
        str += "じゅう";
        str += ChGetJpnNumber1(num % ch_juu, false);
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
            str += ChGetJpnNumber1(num, false, false);
        }
        return str;
    }
    assert(num < ch_ichi);
    if (num > ch_zero)
    {
        std::string digits = num.str(ch_precision, std::ios_base::fixed);
        size_t i = digits.find_last_not_of('0');
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

std::string ChGetJpnNumberFixed1(CH_Value num)
{
    num.trim(ch_precision);
    std::string str = ChGetJpnNumber1(num);
    ChReplaceString(str, "いちてん", "いってん");
    ChReplaceString(str, "にてん", "にいてん");
    ChReplaceString(str, "ごてん", "ごうてん");
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
    ChReplaceString(str, "じゅうちょう", "じゅっちょう");
    ChReplaceString(str, "いちけい", "いっけい");
    ChReplaceString(str, "じゅうけい", "じゅっけい");
    ChReplaceString(str, "いちこう", "いっこう");
    ChReplaceString(str, "じゅうこう", "じゅっこう");
    ChReplaceString(str, "いちかん", "いっかん");
    ChReplaceString(str, "じゅうかん", "じゅっかん");
    ChReplaceString(str, "いちせい", "いっせい");
    ChReplaceString(str, "じゅうせい", "じゅっせい");
    ChReplaceString(str, "いちさい", "いっさい");
    ChReplaceString(str, "じゅうさい", "じゅっさい");
    return str;
}

std::string ChGetJpnNumberBunsuu(CH_Value num)
{
    // bunsuu is fraction
    std::string str;
    CH_Value bunshi = pmp::numerator(num);
    CH_Value bunbo = pmp::denominator(num);
    bool minus = false;
    if (bunshi.sign() < 0)
    {
        minus = true;
        bunshi = -bunshi;
    }
    CH_Value seisuu(0);
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
        str += ChGetJpnNumberFixed1(seisuu);
        str += "と";
    }
    str += ChGetJpnNumberFixed1(bunbo);
    str += "ぶんの";
    str += ChGetJpnNumberFixed1(bunshi);
    return str;
}

std::string ChGetJpnNumberFixed2(CH_Value num)
{
    std::string str;
    num.trim(ch_precision);
    if (num.is_v())
    {
        if (num.empty())
            return "なし";

        str = ChGetJpnNumberFixed2(num[0]);
        for (size_t i = 1; i < num.size(); ++i)
        {
            str += "と、";
            str += ChGetJpnNumberFixed2(num[i]);
        }
        return str;
    }
    pmp::Number::Type old_type = pmp::SetIntDivType(pmp::Number::INTEGER);
    if (num.type() == pmp::Number::RATIONAL)
        str = ChGetJpnNumberBunsuu(num);
    else
        str = ChGetJpnNumberFixed1(num);
    pmp::SetIntDivType(old_type);
    return str;
}

////////////////////////////////////////////////////////////////////////////

void CrTrimString(std::string& str)
{
    static const char *spaces = " \t";
    size_t i, j;
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
                    sstream << ChGetJpnNumberFixed2(value) <<
                        " (" << value.str(ch_precision) <<
                                ") " << "です。" << std::endl;
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
        s_sentence_prev = sentence;
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
