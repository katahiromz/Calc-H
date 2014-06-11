// HParserAST.h

#ifndef HPARSER_AST_H_
#define HPARSER_AST_H_

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
            SHITE
        } m_type;
        shared_ptr<Mono>        m_mono;
        shared_ptr<Expr>        m_expr;
        shared_ptr<Suruto>      m_suruto;
        shared_ptr<Num>         m_num;
        shared_ptr<ExprList>    m_exprlist;
        shared_ptr<Shite>       m_shite;
    };

    struct Suruto : Node
    {
        enum {
            EXPRLIST_ADD, EXPRLIST_MUL,
            MONO_ADD, MONO_MUL, MONO_SUB, MONO_DIV, MONO_ONLY,
            MONO_WO_EXPR_BAI, MONO_WO_BAI,
            SHITE_TASUTO, SHITE_KAKERUTO, SHITE_HIKUTO, SHITE_WARUTO,
            MONO_WO_EXPR_SUB, MONO_DE_EXPR_DIV,
            MONO_WO_EXPR_JOU, MONO_WO_JIJOU
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
            SHITE_EXPR_JOU, SHITE_JIJOU
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
            MONO_FUNC1ARG, SHITE_EXPR_JOU, SHITE_JIJOU,
            MONO_EXPR_JOU, MONO_JIJOU
        } m_type;
        shared_ptr<ExprList>    m_exprlist;
        shared_ptr<Mono>        m_mono;
        shared_ptr<Shite>       m_shite;
        shared_ptr<Expr>        m_expr;
        shared_ptr<Suruto>      m_suruto;
        shared_ptr<Term>        m_term;
        shared_ptr<Fact>        m_fact;
        shared_ptr<Func1Arg>    m_func1arg;
    };

    struct Expr : Node
    {
        enum {
            ADD, SUB, TERM_ONLY
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
            PI, E, FUNC1ARG_JIJOU, FUNC1ARG_JOU
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
            POW, POW2, SINGLE
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
            SIN, COS, TAN, ASIN, ACOS, ATAN
        } m_type;
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

        std::string text()
        {
            return m_text;
        }

        const std::string text() const
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
