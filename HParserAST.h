// HParserAST.h

#ifndef HPARSER_AST_H_
#define HPARSER_AST_H_

#include "Ndrr1D.hpp"

namespace Calc_H
{
    struct Node;
    struct Sentence;
    struct Suruto;
    struct Shite;
    struct Mono;
    struct Expr;
    struct Term;
    struct Fact;
    struct Prim;
    struct ExprList;
    struct Num;
    struct Digit;
    struct Digits;
    struct Func1Arg;
    struct VecFunc;
    struct Dom;
    struct Doms;
    struct PrimDom;
    struct Cnstr;
    struct AndCnstr;
    struct PrimCnstr;

    //
    // Node
    //
    struct Node
    {
        Node() { }
        Node(const Node& n) : m_location(n.m_location) { }
        virtual ~Node() { }

        void operator=(const Node& n)
        {
            m_location = n.m_location;
        }

              CH_Location& location()       { return m_location; }
        const CH_Location& location() const { return m_location; }

    protected:
        CH_Location  m_location;
    };

    struct Sentence : Node
    {
        enum {
            MONO, EXPR, SURUTO, NUM, EMPTY,
            EXPRLIST_ADD, EXPRLIST_MUL,
            SHITE, WARIKIRU,
            DOMS_IS_DOMS, DOMS_IS_CNSTR,
            MONO_IS_DOMS, MONO_IS_CNSTR,
            MONO_IS_CNSTRED_BUNSUU, MONO_IS_BUNSUU,
            MONO_IS_CNSTRED_SHOUSUU, MONO_IS_SHOUSUU,
            MONO_IS_MONO
        } m_type;
        shared_ptr<Mono>        m_mono;
        shared_ptr<Mono>        m_mono2;
        shared_ptr<Expr>        m_expr;
        shared_ptr<Suruto>      m_suruto;
        shared_ptr<Num>         m_num;
        shared_ptr<ExprList>    m_exprlist;
        shared_ptr<Shite>       m_shite;
        shared_ptr<Doms>        m_doms1;
        shared_ptr<Doms>        m_doms2;
        shared_ptr<Cnstr>       m_cnstr;
    };

    struct Suruto : Node
    {
        enum {
            EXPRLIST_ADD, EXPRLIST_MUL,
            MONO_ADD, MONO_MUL, MONO_SUB, MONO_DIV, MONO_ONLY,
            MONO_WO_EXPR_BAI, MONO_WO_BAI,
            SHITE_TASUTO, SHITE_KAKERUTO, SHITE_HIKUTO, SHITE_WARUTO,
            MONO_WO_EXPR_SUB, MONO_DE_EXPR_DIV,
            MONO_WO_EXPR_JOU, MONO_WO_HEIHOU, MONO_WO_RIPPOU,
            SHOUSUU, MONO_WO_SHOUSUU
        } m_type;
        shared_ptr<ExprList>    m_exprlist;
        shared_ptr<Mono>        m_mono;
        shared_ptr<Expr>        m_expr;
        shared_ptr<Shite>       m_shite;
    };

    struct Shite : Node
    {
        enum {
            EXPRLIST_ADD, EXPRLIST_MUL, 
            MONO_ADD, MONO_MUL, MONO_SUB, MONO_DIV, MONO_ONLY,
            SHITE_ADD, SHITE_MUL, SHITE_SUB, SHITE_DIV,
            SHITE_EXPR_BAI, SHITE_BAI,
            MONO_WO_EXPR_SUB, MONO_WO_EXPR_DIV,
            SHITE_EXPR_JOU, SHITE_HEIHOU, SHITE_RIPPOU,
            SHOUSUU
        } m_type;
        shared_ptr<ExprList>    m_exprlist;
        shared_ptr<Mono>        m_mono;
        shared_ptr<Shite>       m_shite;
        shared_ptr<Expr>        m_expr;
    };

    struct Mono : Node
    {
        enum {
            EXPRLIST_ADD, EXPRLIST_MUL, EXPRLIST_SUB, EXPRLIST_DIV,
            MONO_ADD, MONO_MUL, MONO_SUB, MONO_DIV, MONO_ONLY,
            MONO_EXPR_BAI, MONO_BAI,
            SHITE_ADD, SHITE_MUL, SHITE_SUB, SHITE_DIV,
            SHITE_EXPR_BAI, SHITE_BAI, SHITE_ONLY,
            EXPR_ONLY, SORE,
            MONO_TO_EXPRLIST_ADD, MONO_TO_EXPRLIST_MUL,
            MONO_TO_EXPR_ADD, MONO_TO_EXPR_MUL,
            MONO_TO_EXPR_SUB, MONO_TO_EXPR_DIV,
            MONO_WO_EXPR_KARA_SUB, MONO_DE_EXPR_WO_DIV,
            TERM_ONLY, FACT_ONLY, SURUTO_ONLY,
            SURUTOKI_TASUKAZU, SURUTOKI_KAKERUKAZU,
            SURUTOKI_HIKUKAZU, SURUTOKI_WARUKAZU,
            MONO_NO_TASUKAZU, MONO_NO_KAKERUKAZU,
            MONO_NO_HIKUKAZU, MONO_NO_WARUKAZU,
            SURUTOKI_TASARERUKAZU, SURUTOKI_KAKERARERUKAZU,
            SURUTOKI_HIKARERUKAZU, SURUTOKI_WARARERUKAZU,
            MONO_NO_TASARERUKAZU, MONO_NO_KAKERARERUKAZU,
            MONO_NO_HIKARERUKAZU, MONO_NO_WARARERUKAZU,
            SORE_NO_TASUKAZU, SORE_NO_KAKERUKAZU,
            SORE_NO_HIKUKAZU, SORE_NO_WARUKAZU,
            SORE_NO_TASARERUKAZU, SORE_NO_KAKERARERUKAZU,
            SORE_NO_HIKARERUKAZU, SORE_NO_WARARERUKAZU,
            MONO_FUNC1ARG, SHITE_EXPR_JOU, SHITE_HEIHOU, SHITE_RIPPOU,
            MONO_EXPR_JOU, MONO_HEIHOU, MONO_RIPPOU,
            MONO_NO_EXPR_PERCENT,
            TASHIZAN, KAKEZAN, HIKIZAN, WARIZAN,
            SURUTO_WA, SURUTO_SEKI, SURUTO_SA, SURUTO_SHOU,
            SURUTO_AMARI, MONO_NO_AMARI, SORE_NO_AMARI,
            MONO_MOD_EXPR, EXPR_MOD_MONO, SHITE_MOD_EXPR, TERM_MOD_FACT,
            SHOUSUU, MONO_NO_SHOUSUU, EXPRLIST_VECFUNC,
            MONO_SHOU_TO_AMARI, MONO_DIVMOD_EXPR, SHITE_DIVMOD_EXPR, EXPR_DIVMOD_MONO,
            TERM_DIVMOD_FACT, SURUTO_SHOU_TO_AMARI,
            MONO_HEIHOUKON, MONO_KAIJOU
        } m_type;
        shared_ptr<ExprList>    m_exprlist;
        shared_ptr<Mono>        m_mono;
        shared_ptr<Shite>       m_shite;
        shared_ptr<Expr>        m_expr;
        shared_ptr<Suruto>      m_suruto;
        shared_ptr<Term>        m_term;
        shared_ptr<Fact>        m_fact;
        shared_ptr<Func1Arg>    m_func1arg;
        shared_ptr<VecFunc>     m_vecfunc;
    };

    struct Expr : Node
    {
        enum {
            ADD, SUB, TERM_ONLY, ZERO
        } m_type;
        shared_ptr<Expr>    m_expr;
        shared_ptr<Term>    m_term;
    };

    struct Term : Node
    {
        enum {
            MUL, DIV, FACT_ONLY
        } m_type;
        shared_ptr<Term>    m_term;
        shared_ptr<Fact>    m_fact;
    };

    struct Prim : Node
    {
        enum {
            MONO, MINUS, NUM, BUNSUU, TAIBUNSUU, FUNC1ARG, DO,
            PI, E, FUNC1ARG_HEIHOU, FUNC1ARG_RIPPOU, FUNC1ARG_JOU, FACTORIAL
        } m_type;
        shared_ptr<Expr>        m_expr;
        shared_ptr<Prim>        m_prim;
        shared_ptr<Num>         m_num;
        shared_ptr<Num>         m_num2;
        shared_ptr<Mono>        m_mono;
        shared_ptr<Func1Arg>    m_func1arg;
    };

    struct Fact : Node
    {
        enum {
            POW, POW2, POW3, KAIJOU, SINGLE
        } m_type;
        shared_ptr<Fact>    m_fact;
        shared_ptr<Prim>    m_prim;
    };

    struct ExprList : Node, std::vector<shared_ptr<Expr> >
    {
    };

    struct Num : Node
    {
        CH_Value m_value;
    };

    struct Digit : Node
    {
        int m_digit;
    };

    struct Digits : Node, std::vector<shared_ptr<Digit> >
    {
        std::string str() const
        {
            std::string s;
            std::vector<shared_ptr<Digit>>::const_iterator it = this->begin();
            std::vector<shared_ptr<Digit>>::const_iterator end = this->end();
            while (it != end)
            {
                s += static_cast<char>('0' + (*it)->m_digit);
                ++it;
            }
            return s;
        }
    };

    struct Func1Arg : Node
    {
        enum {
            SIN, COS, TAN, ASIN, ACOS, ATAN, SQRT, ABS, INV, EXP, LOG, LOG10,
            SINH, COSH, TANH
        } m_type;
    };

    struct VecFunc : Node
    {
        enum {
            COUNT, MAX, MIN, AVERAGE
        } m_type;
    };

    struct Dom : Node
    {
        enum {
            CNSTRED_PRIMDOM, PRIMDOM_ONLY, DOM_OF_DOM
        } m_type;
        shared_ptr<Dom>         m_dom;
        shared_ptr<Cnstr>       m_cnstr;
        shared_ptr<PrimDom>     m_primdom;
    };

    struct Doms : Node, std::vector<shared_ptr<Dom> >
    {
        shared_ptr<Ndrr1D::Domains> m_domains;
    };

    struct PrimDom : Node
    {
        enum {
            POSITIVE, NEGATIVE,
            SHIZENSUU, SEISUU, GUUSUU, KISUU, JISSUU,
            SOSUU
        } m_type;
        shared_ptr<PrimDom>     m_primdom;
    };

    struct Cnstr : Node
    {
        enum {
            MATAHA, SINGLE
        } m_type;
        shared_ptr<Cnstr>           m_cnstr;
        shared_ptr<AndCnstr>        m_andcnstr;
        shared_ptr<Ndrr1D::Domains> m_domains;
    };

    struct AndCnstr : Node
    {
        enum {
            IJOU, IKA, CHIISAI, OOKII, CNSTR_ONLY, PRIMCNSTR_ONLY
        } m_type;
        shared_ptr<Expr>        m_expr;
        shared_ptr<AndCnstr>    m_andcnstr;
        shared_ptr<Cnstr>       m_cnstr;
        shared_ptr<PrimCnstr>   m_primcnstr;
    };

    struct PrimCnstr : Node
    {
        enum {
            IJOU, IKA, CHIISAI, OOKII
        } m_type;
        shared_ptr<Expr>        m_expr;
    };

    //
    // TokenInfo<TokenType>
    //
    template <typename TokenType>
    struct TokenInfo : Node
    {
        typedef TokenType token_type;

        token_type      m_token;
        std::string     m_text;

    public:
        TokenInfo() { }

        TokenInfo(const TokenInfo& info) :
            Node(info),
            m_token(info.m_token),
            m_text(info.m_text)
        {
        }

        void operator=(const TokenInfo& info)
        {
            location() = info.location();
            m_token = info.m_token;
            m_text = info.m_text;
        }

        token_type get_token() const
        {
            return m_token;
        }

        void set_token(token_type token)
        {
            m_token = token;
        }

        std::string& text()
        {
            return m_text;
        }

        const std::string& text() const
        {
            return m_text;
        }
    };

    //
    // TokenValue
    //
    #define TokenValue TokenInfo<Token>
} // namespace Calc_H

#endif  // ndef HPARSER_AST_H_
