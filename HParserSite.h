// HParserSite.h

#ifndef HPARSERSITE_H_
#define HPARSERSITE_H_

namespace Calc_H
{
    //
    // ParserSite
    //
    struct ParserSite
    {
    public:
        ParserSite()
        {
        }

        void syntax_error()
        {
        }

        void stack_overflow()
        {
        }

        void downcast(int& x, const shared_ptr<Node>& y)
        {
        }

        void upcast(shared_ptr<Node>& x, const int& y)
        {
            x = shared_ptr<Node>(new Node);
        }

        template <typename T>
        void downcast(shared_ptr<T>& x, const shared_ptr<Node>& y)
        {
            x = dynamic_pointer_cast<T, Node>(y);
        }

        template <typename T>
        void upcast(shared_ptr<Node>& x, const shared_ptr<T>& y)
        {
            x = static_pointer_cast<Node, T>(y);
        }

        //
        // semantic actions
        //
        shared_ptr<Sentence> DoSent1(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSent1" << std::endl;
            #endif
            Sentence *s = new Sentence;
            s->m_type = Sentence::MONO;
            s->m_mono = mono;
            return shared_ptr<Sentence>(s);
        }

        shared_ptr<Sentence> DoSent3(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSent3" << std::endl;
            #endif
            Sentence *s = new Sentence;
            s->m_type = Sentence::SURUTO;
            s->m_suruto = suruto;
            return shared_ptr<Sentence>(s);
        }

        shared_ptr<Sentence> DoSent4(shared_ptr<Shite>& shite)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSent4" << std::endl;
            #endif
            Sentence *s = new Sentence;
            s->m_type = Sentence::SHITE;
            s->m_shite = shite;
            return shared_ptr<Sentence>(s);
        }

        shared_ptr<Sentence> DoSent6()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSent6" << std::endl;
            #endif
            Sentence *s = new Sentence;
            s->m_type = Sentence::EMPTY;
            return shared_ptr<Sentence>(s);
        }

        shared_ptr<Suruto> DoSuruto1(shared_ptr<ExprList>& exprlist)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSuruto1" << std::endl;
            #endif
            Suruto *s = new Suruto;
            s->m_type = Suruto::EXPRLIST_ADD;
            s->m_exprlist = exprlist;
            return shared_ptr<Suruto>(s);
        }

        shared_ptr<Suruto> DoSuruto2(shared_ptr<ExprList>& exprlist)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSuruto2" << std::endl;
            #endif
            Suruto *s = new Suruto;
            s->m_type = Suruto::EXPRLIST_MUL;
            s->m_exprlist = exprlist;
            return shared_ptr<Suruto>(s);
        }

        shared_ptr<Suruto> DoSuruto3(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSuruto3" << std::endl;
            #endif
            Suruto *s = new Suruto;
            s->m_type = Suruto::MONO_ADD;
            s->m_mono = mono;
            s->m_expr = expr;
            return shared_ptr<Suruto>(s);
        }

        shared_ptr<Suruto> DoSuruto4(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSuruto4" << std::endl;
            #endif
            Suruto *s = new Suruto;
            s->m_type = Suruto::MONO_MUL;
            s->m_mono = mono;
            s->m_expr = expr;
            return shared_ptr<Suruto>(s);
        }

        shared_ptr<Suruto> DoSuruto5(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSuruto5" << std::endl;
            #endif
            Suruto *s = new Suruto;
            s->m_type = Suruto::MONO_SUB;
            s->m_mono = mono;
            s->m_expr = expr;
            return shared_ptr<Suruto>(s);
        }

        shared_ptr<Suruto> DoSuruto6(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSuruto6" << std::endl;
            #endif
            Suruto *s = new Suruto;
            s->m_type = Suruto::MONO_DIV;
            s->m_mono = mono;
            s->m_expr = expr;
            return shared_ptr<Suruto>(s);
        }

        shared_ptr<Suruto> DoSuruto7(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSuruto7" << std::endl;
            #endif
            Suruto *s = new Suruto;
            s->m_type = Suruto::MONO_ONLY;
            s->m_mono = mono;
            return shared_ptr<Suruto>(s);
        }

        shared_ptr<Suruto> DoSuruto8(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSuruto8" << std::endl;
            #endif
            Suruto *s = new Suruto;
            s->m_type = Suruto::MONO_WO_EXPR_BAI;
            s->m_mono = mono;
            s->m_expr = expr;
            return shared_ptr<Suruto>(s);
        }

        shared_ptr<Suruto> DoSuruto9(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSuruto9" << std::endl;
            #endif
            Suruto *s = new Suruto;
            s->m_type = Suruto::MONO_WO_BAI;
            s->m_mono = mono;
            return shared_ptr<Suruto>(s);
        }

        shared_ptr<Suruto> DoSuruto10(shared_ptr<Shite>& shite, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSuruto10" << std::endl;
            #endif
            Suruto *s = new Suruto;
            s->m_type = Suruto::SHITE_TASUTO;
            s->m_shite = shite;
            s->m_expr = expr;
            return shared_ptr<Suruto>(s);
        }

        shared_ptr<Suruto> DoSuruto11(shared_ptr<Shite>& shite, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSuruto11" << std::endl;
            #endif
            Suruto *s = new Suruto;
            s->m_type = Suruto::SHITE_KAKERUTO;
            s->m_shite = shite;
            s->m_expr = expr;
            return shared_ptr<Suruto>(s);
        }

        shared_ptr<Suruto> DoSuruto12(shared_ptr<Shite>& shite, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSuruto12" << std::endl;
            #endif
            Suruto *s = new Suruto;
            s->m_type = Suruto::SHITE_HIKUTO;
            s->m_shite = shite;
            s->m_expr = expr;
            return shared_ptr<Suruto>(s);
        }

        shared_ptr<Suruto> DoSuruto13(shared_ptr<Shite>& shite, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSuruto13" << std::endl;
            #endif
            Suruto *s = new Suruto;
            s->m_type = Suruto::SHITE_WARUTO;
            s->m_shite = shite;
            s->m_expr = expr;
            return shared_ptr<Suruto>(s);
        }

        shared_ptr<Suruto> DoSuruto16(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSuruto16" << std::endl;
            #endif
            Suruto *s = new Suruto;
            s->m_type = Suruto::MONO_WO_EXPR_SUB;
            s->m_mono = mono;
            s->m_expr = expr;
            return shared_ptr<Suruto>(s);
        }

        shared_ptr<Suruto> DoSuruto17(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSuruto17" << std::endl;
            #endif
            Suruto *s = new Suruto;
            s->m_type = Suruto::MONO_DE_EXPR_DIV;
            s->m_mono = mono;
            s->m_expr = expr;
            return shared_ptr<Suruto>(s);
        }

        shared_ptr<Suruto> DoSuruto18(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSuruto18" << std::endl;
            #endif
            Suruto *s = new Suruto;
            s->m_type = Suruto::MONO_WO_EXPR_JOU;
            s->m_mono = mono;
            s->m_expr = expr;
            return shared_ptr<Suruto>(s);
        }

        shared_ptr<Suruto> DoSuruto19(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSuruto19" << std::endl;
            #endif
            Suruto *s = new Suruto;
            s->m_type = Suruto::MONO_WO_HEIHOU;
            s->m_mono = mono;
            return shared_ptr<Suruto>(s);
        }

        shared_ptr<Suruto> DoSuruto20(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSuruto20" << std::endl;
            #endif
            Suruto *s = new Suruto;
            s->m_type = Suruto::MONO_WO_RIPPOU;
            s->m_mono = mono;
            return shared_ptr<Suruto>(s);
        }

        shared_ptr<Suruto> DoSuruto21(shared_ptr<Shite>& shite)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSuruto21" << std::endl;
            #endif
            Suruto *s = new Suruto;
            s->m_type = Suruto::SHOUSUU;
            s->m_shite = shite;
            return shared_ptr<Suruto>(s);
        }

        shared_ptr<Suruto> DoSuruto22(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSuruto22" << std::endl;
            #endif
            Suruto *s = new Suruto;
            s->m_type = Suruto::MONO_WO_SHOUSUU;
            s->m_mono = mono;
            return shared_ptr<Suruto>(s);
        }

        shared_ptr<Shite> DoShite1(shared_ptr<ExprList>& exprlist)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoShite1" << std::endl;
            #endif
            Shite *s = new Shite;
            s->m_type = Shite::EXPRLIST_ADD;
            s->m_exprlist = exprlist;
            return shared_ptr<Shite>(s);
        }

        shared_ptr<Shite> DoShite2(shared_ptr<ExprList>& exprlist)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoShite2" << std::endl;
            #endif
            Shite *s = new Shite;
            s->m_type = Shite::EXPRLIST_MUL;
            s->m_exprlist = exprlist;
            return shared_ptr<Shite>(s);
        }

        shared_ptr<Shite> DoShite3(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoShite3" << std::endl;
            #endif
            Shite *s = new Shite;
            s->m_type = Shite::MONO_ADD;
            s->m_mono = mono;
            s->m_expr = expr;
            return shared_ptr<Shite>(s);
        }

        shared_ptr<Shite> DoShite4(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoShite4" << std::endl;
            #endif
            Shite *s = new Shite;
            s->m_type = Shite::MONO_MUL;
            s->m_mono = mono;
            s->m_expr = expr;
            return shared_ptr<Shite>(s);
        }

        shared_ptr<Shite> DoShite5(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoShite5" << std::endl;
            #endif
            Shite *s = new Shite;
            s->m_type = Shite::MONO_SUB;
            s->m_mono = mono;
            s->m_expr = expr;
            return shared_ptr<Shite>(s);
        }

        shared_ptr<Shite> DoShite6(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoShite6" << std::endl;
            #endif
            Shite *s = new Shite;
            s->m_type = Shite::MONO_DIV;
            s->m_mono = mono;
            s->m_expr = expr;
            return shared_ptr<Shite>(s);
        }

        shared_ptr<Shite> DoShite7(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoShite7" << std::endl;
            #endif
            Shite *s = new Shite;
            s->m_type = Shite::MONO_ONLY;
            s->m_mono = mono;
            return shared_ptr<Shite>(s);
        }

        shared_ptr<Shite> DoShite8(shared_ptr<Shite>& shite, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoShite8" << std::endl;
            #endif
            Shite *s = new Shite;
            s->m_type = Shite::SHITE_ADD;
            s->m_shite = shite;
            s->m_expr = expr;
            return shared_ptr<Shite>(s);
        }

        shared_ptr<Shite> DoShite9(shared_ptr<Shite>& shite, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoShite9" << std::endl;
            #endif
            Shite *s = new Shite;
            s->m_type = Shite::SHITE_MUL;
            s->m_shite = shite;
            s->m_expr = expr;
            return shared_ptr<Shite>(s);
        }

        shared_ptr<Shite> DoShite10(shared_ptr<Shite>& shite, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoShite10" << std::endl;
            #endif
            Shite *s = new Shite;
            s->m_type = Shite::SHITE_SUB;
            s->m_shite = shite;
            s->m_expr = expr;
            return shared_ptr<Shite>(s);
        }

        shared_ptr<Shite> DoShite11(shared_ptr<Shite>& shite, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoShite11" << std::endl;
            #endif
            Shite *s = new Shite;
            s->m_type = Shite::SHITE_DIV;
            s->m_shite = shite;
            s->m_expr = expr;
            return shared_ptr<Shite>(s);
        }

        shared_ptr<Shite> DoShite12(shared_ptr<Shite>& shite, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoShite12" << std::endl;
            #endif
            Shite *s = new Shite;
            s->m_type = Shite::SHITE_EXPR_BAI;
            s->m_shite = shite;
            s->m_expr = expr;
            return shared_ptr<Shite>(s);
        }

        shared_ptr<Shite> DoShite13(shared_ptr<Shite>& shite)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoShite13" << std::endl;
            #endif
            Shite *s = new Shite;
            s->m_type = Shite::SHITE_BAI;
            s->m_shite = shite;
            return shared_ptr<Shite>(s);
        }

        shared_ptr<Shite> DoShite16(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoShite16" << std::endl;
            #endif
            Shite *s = new Shite;
            s->m_type = Shite::MONO_WO_EXPR_SUB;
            s->m_mono = mono;
            s->m_expr = expr;
            return shared_ptr<Shite>(s);
        }

        shared_ptr<Shite> DoShite17(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoShite17" << std::endl;
            #endif
            Shite *s = new Shite;
            s->m_type = Shite::MONO_WO_EXPR_DIV;
            s->m_mono = mono;
            s->m_expr = expr;
            return shared_ptr<Shite>(s);
        }

        shared_ptr<Shite> DoShite18(shared_ptr<Shite>& shite, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoShite18" << std::endl;
            #endif
            Shite *s = new Shite;
            s->m_type = Shite::SHITE_EXPR_JOU;
            s->m_shite = shite;
            s->m_expr = expr;
            return shared_ptr<Shite>(s);
        }

        shared_ptr<Shite> DoShite19(shared_ptr<Shite>& shite)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoShite19" << std::endl;
            #endif
            Shite *s = new Shite;
            s->m_type = Shite::SHITE_HEIHOU;
            s->m_shite = shite;
            return shared_ptr<Shite>(s);
        }

        shared_ptr<Shite> DoShite20(shared_ptr<Shite>& shite)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoShite20" << std::endl;
            #endif
            Shite *s = new Shite;
            s->m_type = Shite::SHITE_RIPPOU;
            s->m_shite = shite;
            return shared_ptr<Shite>(s);
        }

        shared_ptr<Shite> DoShite21(shared_ptr<Shite>& shite)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoShite21" << std::endl;
            #endif
            Shite *s = new Shite;
            s->m_type = Shite::SHOUSUU;
            s->m_shite = shite;
            return shared_ptr<Shite>(s);
        }

        shared_ptr<Mono> DoMono1(shared_ptr<ExprList>& exprlist)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono1" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::EXPRLIST_ADD;
            m->m_exprlist = exprlist;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono2(shared_ptr<ExprList>& exprlist)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono2" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::EXPRLIST_MUL;
            m->m_exprlist = exprlist;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono3(shared_ptr<ExprList>& exprlist)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono3" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::EXPRLIST_SUB;
            m->m_exprlist = exprlist;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono4(shared_ptr<ExprList>& exprlist)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono4" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::EXPRLIST_DIV;
            m->m_exprlist = exprlist;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono5(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono5" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_ADD;
            m->m_mono = mono;
            m->m_expr = expr;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono6(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono6" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_MUL;
            m->m_mono = mono;
            m->m_expr = expr;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono7(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono7" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_SUB;
            m->m_mono = mono;
            m->m_expr = expr;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono8(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono8" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_DIV;
            m->m_mono = mono;
            m->m_expr = expr;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono9(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono9" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_ONLY;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono10(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono10" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_EXPR_BAI;
            m->m_mono = mono;
            m->m_expr = expr;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono11(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono11" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_BAI;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono12(shared_ptr<Shite>& shite, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono12" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SHITE_ADD;
            m->m_shite = shite;
            m->m_expr = expr;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono13(shared_ptr<Shite>& shite, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono13" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SHITE_MUL;
            m->m_shite = shite;
            m->m_expr = expr;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono14(shared_ptr<Shite>& shite, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono14" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SHITE_SUB;
            m->m_shite = shite;
            m->m_expr = expr;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono15(shared_ptr<Shite>& shite, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono15" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SHITE_DIV;
            m->m_shite = shite;
            m->m_expr = expr;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono16(shared_ptr<Shite>& shite, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono16" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SHITE_EXPR_BAI;
            m->m_shite = shite;
            m->m_expr = expr;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono17(shared_ptr<Shite>& shite)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono17" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SHITE_BAI;
            m->m_shite = shite;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono18(shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono18" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = expr;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono19()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono19" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SORE;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono20(shared_ptr<Mono>& mono, shared_ptr<ExprList>& exprlist)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono20" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_TO_EXPRLIST_ADD;
            m->m_mono = mono;
            m->m_exprlist = exprlist;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono21(shared_ptr<Mono>& mono, shared_ptr<ExprList>& exprlist)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono21" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_TO_EXPRLIST_MUL;
            m->m_mono = mono;
            m->m_exprlist = exprlist;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono22(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono22" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_TO_EXPR_ADD;
            m->m_mono = mono;
            m->m_expr = expr;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono23(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono23" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_TO_EXPR_MUL;
            m->m_mono = mono;
            m->m_expr = expr;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono24(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono24" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_TO_EXPR_SUB;
            m->m_mono = mono;
            m->m_expr = expr;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono25(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono25" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_TO_EXPR_DIV;
            m->m_mono = mono;
            m->m_expr = expr;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono26(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono26" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_WO_EXPR_KARA_SUB;
            m->m_mono = mono;
            m->m_expr = expr;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono27(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono27" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_DE_EXPR_WO_DIV;
            m->m_mono = mono;
            m->m_expr = expr;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono28(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono28" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_NO_TASUKAZU;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono29(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono29" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_NO_KAKERUKAZU;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono30(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono30" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_NO_HIKUKAZU;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono31(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono31" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_NO_WARUKAZU;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono32(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono32" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SURUTOKI_TASUKAZU;
            m->m_suruto = suruto;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono33(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono33" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SURUTOKI_KAKERUKAZU;
            m->m_suruto = suruto;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono34(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono34" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SURUTOKI_HIKUKAZU;
            m->m_suruto = suruto;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono35(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono35" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SURUTOKI_WARUKAZU;
            m->m_suruto = suruto;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono36(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono36" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SURUTO_ONLY;
            m->m_suruto = suruto;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono37(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono37" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SURUTOKI_TASARERUKAZU;
            m->m_suruto = suruto;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono38(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono38" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SURUTOKI_KAKERARERUKAZU;
            m->m_suruto = suruto;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono39(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono39" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SURUTOKI_HIKARERUKAZU;
            m->m_suruto = suruto;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono40(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono40" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SURUTOKI_WARARERUKAZU;
            m->m_suruto = suruto;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono41(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono41" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_NO_TASARERUKAZU;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono42(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono42" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_NO_KAKERARERUKAZU;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono43(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono43" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_NO_HIKARERUKAZU;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono44(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono44" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_NO_WARARERUKAZU;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono45()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono45" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SORE_NO_TASUKAZU;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono46()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono46" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SORE_NO_KAKERUKAZU;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono47()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono47" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SORE_NO_HIKUKAZU;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono48()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono48" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SORE_NO_WARUKAZU;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono49()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono49" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SORE_NO_TASARERUKAZU;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono50()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono50" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SORE_NO_KAKERARERUKAZU;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono51()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono51" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SORE_NO_HIKARERUKAZU;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono52()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono52" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SORE_NO_WARARERUKAZU;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono53(shared_ptr<Mono>& mono, shared_ptr<Func1Arg>& func1arg)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono53" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_FUNC1ARG;
            m->m_mono = mono;
            m->m_func1arg = func1arg;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono54(shared_ptr<Shite>& shite, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono54" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SHITE_EXPR_JOU;
            m->m_shite = shite;
            m->m_expr = expr;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono55(shared_ptr<Shite>& shite)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono55" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SHITE_HEIHOU;
            m->m_shite = shite;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono56(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono56" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_EXPR_JOU;
            m->m_mono = mono;
            m->m_expr = expr;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono57(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono57" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_HEIHOU;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono58(shared_ptr<Shite>& shite)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono58" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SHITE_RIPPOU;
            m->m_shite = shite;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono59(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono59" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_RIPPOU;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono60(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono60" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_NO_EXPR_PERCENT;
            m->m_mono = mono;
            m->m_expr = expr;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono61(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono61" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::TASHIZAN;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono62(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono62" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::KAKEZAN;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono63(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono63" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::HIKIZAN;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono64(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono64" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::WARIZAN;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono65(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono65" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SURUTO_WA;
            m->m_suruto = suruto;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono66(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono66" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SURUTO_SEKI;
            m->m_suruto = suruto;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono67(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono67" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SURUTO_SA;
            m->m_suruto = suruto;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono68(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono68" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SURUTO_SHOU;
            m->m_suruto = suruto;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono69(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono69" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SURUTO_AMARI;
            m->m_suruto = suruto;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono70(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono70" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_NO_AMARI;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono71()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono71" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SORE_NO_AMARI;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono72(shared_ptr<Shite>& shite)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono72" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SHOUSUU;
            m->m_shite = shite;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono74(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono74" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_NO_SHOUSUU;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono75(shared_ptr<ExprList>& exprlist, shared_ptr<VecFunc>& vf)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono75" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::EXPRLIST_VECFUNC;
            m->m_exprlist = exprlist;
            m->m_vecfunc = vf;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono76(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono76" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_SHOU_TO_AMARI;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono77(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMono77" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SURUTO_SHOU_TO_AMARI;
            m->m_suruto = suruto;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Expr> DoExpr1(shared_ptr<Expr>& expr, shared_ptr<Term>& term)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoExpr1" << std::endl;
            #endif
            Expr *e = new Expr;
            e->m_type = Expr::ADD;
            e->m_expr = expr;
            e->m_term = term;
            return shared_ptr<Expr>(e);
        }

        shared_ptr<Expr> DoExpr2(shared_ptr<Expr>& expr, shared_ptr<Term>& term)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoExpr2" << std::endl;
            #endif
            Expr *e = new Expr;
            e->m_type = Expr::SUB;
            e->m_expr = expr;
            e->m_term = term;
            return shared_ptr<Expr>(e);
        }

        shared_ptr<Expr> DoExpr3(shared_ptr<Expr>& expr, shared_ptr<Term>& term)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoExpr3" << std::endl;
            #endif
            Expr *e = new Expr;
            e->m_type = Expr::ADD;
            e->m_expr = expr;
            e->m_term = term;
            return shared_ptr<Expr>(e);
        }

        shared_ptr<Expr> DoExpr4(shared_ptr<Expr>& expr, shared_ptr<Term>& term)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoExpr4" << std::endl;
            #endif
            Expr *e = new Expr;
            e->m_type = Expr::SUB;
            e->m_expr = expr;
            e->m_term = term;
            return shared_ptr<Expr>(e);
        }

        shared_ptr<Expr> DoExpr5(shared_ptr<Term>& term)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoExpr5" << std::endl;
            #endif
            Expr *e = new Expr;
            e->m_type = Expr::TERM_ONLY;
            e->m_term = term;
            return shared_ptr<Expr>(e);
        }

        shared_ptr<Term> DoTerm1(shared_ptr<Term>& term, shared_ptr<Fact>& fact)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoTerm1" << std::endl;
            #endif
            Term *t = new Term;
            t->m_type = Term::MUL;
            t->m_term = term;
            t->m_fact = fact;
            return shared_ptr<Term>(t);
        }

        shared_ptr<Term> DoTerm2(shared_ptr<Term>& term, shared_ptr<Fact>& fact)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoTerm2" << std::endl;
            #endif
            Term *t = new Term;
            t->m_type = Term::DIV;
            t->m_term = term;
            t->m_fact = fact;
            return shared_ptr<Term>(t);
        }

        shared_ptr<Term> DoTerm3(shared_ptr<Fact>& fact)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoTerm3" << std::endl;
            #endif
            Term *t = new Term;
            t->m_type = Term::FACT_ONLY;
            t->m_fact = fact;
            return shared_ptr<Term>(t);
        }

        shared_ptr<Fact> DoFact1(shared_ptr<Fact>& fact, shared_ptr<Prim>& prim)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoFact1" << std::endl;
            #endif
            Fact *f = new Fact;
            f->m_type = Fact::POW;
            f->m_fact = fact;
            f->m_prim = prim;
            return shared_ptr<Fact>(f);
        }

        shared_ptr<Fact> DoFact2(shared_ptr<Fact>& fact)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoFact2" << std::endl;
            #endif
            Fact *f = new Fact;
            f->m_type = Fact::POW2;
            f->m_fact = fact;
            return shared_ptr<Fact>(f);
        }

        shared_ptr<Fact> DoFact3(shared_ptr<Prim>& prim)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoFact3" << std::endl;
            #endif
            Fact *f = new Fact;
            f->m_type = Fact::SINGLE;
            f->m_prim = prim;
            return shared_ptr<Fact>(f);
        }

        shared_ptr<Fact> DoFact4(shared_ptr<Fact>& fact)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoFact4" << std::endl;
            #endif
            Fact *f = new Fact;
            f->m_type = Fact::POW3;
            f->m_fact = fact;
            return shared_ptr<Fact>(f);
        }

        shared_ptr<Prim> DoPrim1(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoPrim1" << std::endl;
            #endif
            Prim *p = new Prim;
            p->m_type = Prim::MONO;
            p->m_mono = mono;
            return shared_ptr<Prim>(p);
        }

        shared_ptr<Prim> DoPrim2(shared_ptr<Prim>& prim)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoPrim2" << std::endl;
            #endif
            Prim *p = new Prim;
            p->m_type = Prim::MINUS;
            p->m_prim = prim;
            return shared_ptr<Prim>(p);
        }

        shared_ptr<Prim> DoPrim3(shared_ptr<Prim>& prim, shared_ptr<Num>& num)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoPrim3" << std::endl;
            #endif
            Prim *p = new Prim;
            p->m_type = Prim::BUNSUU;
            p->m_prim = prim;
            p->m_num = num;
            return shared_ptr<Prim>(p);
        }

        shared_ptr<Prim> DoPrim4(shared_ptr<Num>& num)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoPrim4" << std::endl;
            #endif
            Prim *p = new Prim;
            p->m_type = Prim::NUM;
            p->m_num = num;
            return shared_ptr<Prim>(p);
        }

        shared_ptr<Prim> DoPrim5(shared_ptr<Num>& num, shared_ptr<Prim>& prim,
                                 shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoPrim5" << std::endl;
            #endif
            Prim *p = new Prim;
            p->m_type = Prim::TAIBUNSUU;
            p->m_num = num;
            p->m_prim = prim;
            p->m_num2 = num2;
            return shared_ptr<Prim>(p);
        }

        shared_ptr<Prim> DoPrim6(shared_ptr<Func1Arg>& func1arg, shared_ptr<Prim>& prim)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoPrim6" << std::endl;
            #endif
            Prim *p = new Prim;
            p->m_type = Prim::FUNC1ARG;
            p->m_func1arg = func1arg;
            p->m_prim = prim;
            return shared_ptr<Prim>(p);
        }

        shared_ptr<Prim> DoPrim7(shared_ptr<Prim>& prim)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoPrim7" << std::endl;
            #endif
            Prim *p = new Prim;
            p->m_type = Prim::DO;
            p->m_prim = prim;
            return shared_ptr<Prim>(p);
        }

        shared_ptr<Prim> DoPrim8()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoPrim8" << std::endl;
            #endif
            Prim *p = new Prim;
            p->m_type = Prim::PI;
            return shared_ptr<Prim>(p);
        }

        shared_ptr<Prim> DoPrim9()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoPrim9" << std::endl;
            #endif
            Prim *p = new Prim;
            p->m_type = Prim::E;
            return shared_ptr<Prim>(p);
        }

        shared_ptr<Prim> DoPrim10(shared_ptr<Func1Arg>& func1arg, shared_ptr<Prim>& prim)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoPrim10" << std::endl;
            #endif
            Prim *p = new Prim;
            p->m_type = Prim::FUNC1ARG_HEIHOU;
            p->m_func1arg = func1arg;
            p->m_prim = prim;
            return shared_ptr<Prim>(p);
        }

        shared_ptr<Prim> DoPrim11(
            shared_ptr<Func1Arg>& func1arg,
            shared_ptr<Num>& num,
            shared_ptr<Prim>& prim)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoPrim11" << std::endl;
            #endif
            Prim *p = new Prim;
            p->m_type = Prim::FUNC1ARG_JOU;
            p->m_func1arg = func1arg;
            p->m_num = num;
            p->m_prim = prim;
            return shared_ptr<Prim>(p);
        }

        shared_ptr<Prim> DoPrim12(
            shared_ptr<Func1Arg>& func1arg,
            shared_ptr<Prim>& prim)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoPrim12" << std::endl;
            #endif
            Prim *p = new Prim;
            p->m_type = Prim::FUNC1ARG_RIPPOU;
            p->m_func1arg = func1arg;
            p->m_prim = prim;
            return shared_ptr<Prim>(p);
        }

        shared_ptr<ExprList> DoExprList1(shared_ptr<ExprList>& exprlist, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoExprList1" << std::endl;
            #endif
            exprlist.get()->push_back(expr);
            return exprlist;
        }

        shared_ptr<ExprList> DoExprList2(shared_ptr<ExprList>& exprlist, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoExprList2" << std::endl;
            #endif
            exprlist.get()->push_back(expr);
            return exprlist;
        }

        shared_ptr<ExprList> DoExprList3(shared_ptr<Expr>& expr1, shared_ptr<Expr>& expr2)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoExprList3" << std::endl;
            #endif
            ExprList *el = new ExprList;
            el->push_back(expr1);
            el->push_back(expr2);
            return shared_ptr<ExprList>(el);
        }

        shared_ptr<ExprList> DoExprList4(shared_ptr<Expr>& expr1, shared_ptr<Expr>& expr2)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoExprList4" << std::endl;
            #endif
            ExprList *el = new ExprList;
            el->push_back(expr1);
            el->push_back(expr2);
            return shared_ptr<ExprList>(el);
        }

        shared_ptr<Num> DoNum1(shared_ptr<Num>& num, shared_ptr<Digits>& digits)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoNum1" << std::endl;
            #endif
            num.get()->m_value =
                CH_Value(num->m_value.str() + "." + digits->str());
            return num;
        }

        shared_ptr<Num> DoNum2(shared_ptr<Digits>& digits)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoNum2" << std::endl;
            #endif
            Num *num = new Num;
            num->m_value = CH_Value("0." + digits->str());
            return shared_ptr<Num>(num);
        }

        shared_ptr<Num> DoNum3(shared_ptr<Num>& num)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoNum3" << std::endl;
            #endif
            return num;
        }

        shared_ptr<Num> DoNum4()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoNum4" << std::endl;
            #endif
            Num *n = new Num;
            n->m_value = ch_zero;
            return shared_ptr<Num>(n);
        }

        shared_ptr<Num> DoMuryoutaisuuLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMuryoutaisuuLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoMuryoutaisuuLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMuryoutaisuuLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoMuryoutaisuuNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMuryoutaisuuNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_muryoutaisuu;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoMuryoutaisuuNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoMuryoutaisuuNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_muryoutaisuu;
            return num1;
        }

        shared_ptr<Num> DoFukashigiLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoFukashigiLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoFukashigiLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoFukashigiLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoFukashigiNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoFukashigiNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_fukashigi;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoFukashigiNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoFukashigiNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_fukashigi;
            return num1;
        }

        shared_ptr<Num> DoNayutaLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoNayutaLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoNayutaLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoNayutaLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoNayutaNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoNayutaNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_nayuta;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoNayutaNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoNayutaNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_nayuta;
            return num1;
        }

        shared_ptr<Num> DoAsougiLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoAsougiLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoAsougiLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoAsougiLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoAsougiNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoAsougiNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_asougi;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoAsougiNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoAsougiNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_asougi;
            return num1;
        }

        shared_ptr<Num> DoGougashaLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoGougashaLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoGougashaLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoGougashaLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoGougashaNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoGougashaNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_gougasha;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoGougashaNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoGougashaNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_gougasha;
            return num1;
        }

        shared_ptr<Num> DoGokuLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoGokuLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoGokuLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoGokuLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoGokuNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoGokuNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_goku;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoGokuNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoGokuNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_goku;
            return num1;
        }

        shared_ptr<Num> DoSaiLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSaiLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoSaiLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSaiLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoSaiNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSaiNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_sai;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoSaiNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSaiNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_sai;
            return num1;
        }

        shared_ptr<Num> DoSeiLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSeiLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoSeiLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSeiLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoSeiNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSeiNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_sei;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoSeiNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSeiNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_sei;
            return num1;
        }

        shared_ptr<Num> DoKanLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoKanLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoKanLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoKanLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoKanNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoKanNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_kan;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoKanNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoKanNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_kan;
            return num1;
        }

        shared_ptr<Num> DoKouLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoKouLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoKouLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoKouLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoKouNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoKouNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_kou;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoKouNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoKouNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_kou;
            return num1;
        }

        shared_ptr<Num> DoJouLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoJouLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoJouLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoJouLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoJouNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoJouNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_jou;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoJouNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoJouNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_jou;
            return num1;
        }

        shared_ptr<Num> DoJoLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoJoLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoJoLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoJoLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoJoNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoJoNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_jo;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoJoNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoJoNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_jo;
            return num1;
        }

        shared_ptr<Num> DoGaiLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoGaiLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoGaiLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoGaiLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoGaiNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoGaiNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_gai;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoGaiNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoGaiNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_gai;
            return num1;
        }

        shared_ptr<Num> DoKeiLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoKeyNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoKeiLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoKeyNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoKeiNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoKeiNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_kei;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoKeiNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoKeiNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_kei;
            return num1;
        }

        shared_ptr<Num> DoChouLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoChouLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoChouLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoChouLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoChouNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoChouNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_chou;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoChouNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoChouNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_chou;
            return num1;
        }

        shared_ptr<Num> DoOkuLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoOkuLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoOkuLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoOkuLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoOkuNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoOkuNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_oku;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoOkuNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoOkuNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_oku;
            return num1;
        }

        shared_ptr<Num> DoManLeNum1(shared_ptr<Num>& num)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoManLeNum1" << std::endl;
            #endif
            return num;
        }

        shared_ptr<Num> DoManLeNum2(shared_ptr<Num>& num)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoManLeNum2" << std::endl;
            #endif
            return num;
        }

        shared_ptr<Num> DoManNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoManNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_man;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoManNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoManNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_man;
            return num1;
        }

        shared_ptr<Num> DoSenLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSenLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoSenLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSenLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoSenNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSenNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_sen;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoSenNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSenNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_sen;
            return num1;
        }

        shared_ptr<Num> DoSenNum3(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSenNum3" << std::endl;
            #endif
            num1.get()->m_value += ch_sen;
            return num1;
        }

        shared_ptr<Num> DoSenNum4()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoSenNum4" << std::endl;
            #endif
            Num *n = new Num;
            n->m_value = ch_sen;
            return shared_ptr<Num>(n);
        }

        shared_ptr<Num> DoHyakuLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoHyakuLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoHyakuLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoHyakuLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoHyakuNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoHyakuNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_hyaku;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoHyakuNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoHyakuNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_hyaku;
            return num1;
        }

        shared_ptr<Num> DoHyakuNum3(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoHyakuNum3" << std::endl;
            #endif
            num1.get()->m_value += ch_hyaku;
            return num1;
        }

        shared_ptr<Num> DoHyakuNum4()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoHyakuNum4" << std::endl;
            #endif
            Num *n = new Num;
            n->m_value = ch_hyaku;
            return shared_ptr<Num>(n);
        }

        shared_ptr<Num> DoJuuLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoJuuLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoJuuLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoJuuLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoJuuNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoJuuNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_juu;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoJuuNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoJuuNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_juu;
            return num1;
        }

        shared_ptr<Num> DoJuuNum3(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoJuuNum3" << std::endl;
            #endif
            num1.get()->m_value += ch_juu;
            return num1;
        }

        shared_ptr<Num> DoJuuNum4()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoJuuNum4" << std::endl;
            #endif
            Num *n = new Num;
            n->m_value = ch_juu;
            return shared_ptr<Num>(n);
        }

        shared_ptr<Num> DoIchiLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoIchiLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoIchiNum1()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoIchiNum1" << std::endl;
            #endif
            Num *n = new Num;
            n->m_value = 1;
            return shared_ptr<Num>(n);
        }

        shared_ptr<Num> DoIchiNum2()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoIchiNum2" << std::endl;
            #endif
            Num *n = new Num;
            n->m_value = 2;
            return shared_ptr<Num>(n);
        }

        shared_ptr<Num> DoIchiNum3()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoIchiNum3" << std::endl;
            #endif
            Num *n = new Num;
            n->m_value = 3;
            return shared_ptr<Num>(n);
        }

        shared_ptr<Num> DoIchiNum4()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoIchiNum4" << std::endl;
            #endif
            Num *n = new Num;
            n->m_value = 4;
            return shared_ptr<Num>(n);
        }

        shared_ptr<Num> DoIchiNum5()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoIchiNum5" << std::endl;
            #endif
            Num *n = new Num;
            n->m_value = 5;
            return shared_ptr<Num>(n);
        }

        shared_ptr<Num> DoIchiNum6()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoIchiNum6" << std::endl;
            #endif
            Num *n = new Num;
            n->m_value = 6;
            return shared_ptr<Num>(n);
        }

        shared_ptr<Num> DoIchiNum7()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoIchiNum7" << std::endl;
            #endif
            Num *n = new Num;
            n->m_value = 7;
            return shared_ptr<Num>(n);
        }

        shared_ptr<Num> DoIchiNum8()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoIchiNum8" << std::endl;
            #endif
            Num *n = new Num;
            n->m_value = 8;
            return shared_ptr<Num>(n);
        }

        shared_ptr<Num> DoIchiNum9()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoIchiNum9" << std::endl;
            #endif
            Num *n = new Num;
            n->m_value = 9;
            return shared_ptr<Num>(n);
        }

        shared_ptr<Digits> DoDigits1(shared_ptr<Digits>& digits, shared_ptr<Digit>& digit)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoDigits1" << std::endl;
            #endif
            digits.get()->push_back(digit);
            return digits;
        }

        shared_ptr<Digits> DoDigits2(shared_ptr<Digit>& digit)
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoDigits2" << std::endl;
            #endif
            Digits *digits = new Digits;
            digits->push_back(digit);
            return shared_ptr<Digits>(digits);
        }

        shared_ptr<Digit> DoDigit0()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoDigit0" << std::endl;
            #endif
            Digit *d = new Digit;
            d->m_digit = 0;
            return shared_ptr<Digit>(d);
        }

        shared_ptr<Digit> DoDigit1()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoDigit1" << std::endl;
            #endif
            Digit *d = new Digit;
            d->m_digit = 1;
            return shared_ptr<Digit>(d);
        }

        shared_ptr<Digit> DoDigit2()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoDigit2" << std::endl;
            #endif
            Digit *d = new Digit;
            d->m_digit = 2;
            return shared_ptr<Digit>(d);
        }

        shared_ptr<Digit> DoDigit3()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoDigit3" << std::endl;
            #endif
            Digit *d = new Digit;
            d->m_digit = 3;
            return shared_ptr<Digit>(d);
        }

        shared_ptr<Digit> DoDigit4()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoDigit4" << std::endl;
            #endif
            Digit *d = new Digit;
            d->m_digit = 4;
            return shared_ptr<Digit>(d);
        }

        shared_ptr<Digit> DoDigit5()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoDigit5" << std::endl;
            #endif
            Digit *d = new Digit;
            d->m_digit = 5;
            return shared_ptr<Digit>(d);
        }

        shared_ptr<Digit> DoDigit6()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoDigit6" << std::endl;
            #endif
            Digit *d = new Digit;
            d->m_digit = 6;
            return shared_ptr<Digit>(d);
        }

        shared_ptr<Digit> DoDigit7()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoDigit7" << std::endl;
            #endif
            Digit *d = new Digit;
            d->m_digit = 7;
            return shared_ptr<Digit>(d);
        }

        shared_ptr<Digit> DoDigit8()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoDigit8" << std::endl;
            #endif
            Digit *d = new Digit;
            d->m_digit = 8;
            return shared_ptr<Digit>(d);
        }

        shared_ptr<Digit> DoDigit9()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoDigit9" << std::endl;
            #endif
            Digit *d = new Digit;
            d->m_digit = 9;
            return shared_ptr<Digit>(d);
        }

        shared_ptr<Func1Arg> DoFunc1Arg1()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoFunc1Arg1" << std::endl;
            #endif
            Func1Arg *f = new Func1Arg;
            f->m_type = Func1Arg::SIN;
            return shared_ptr<Func1Arg>(f);
        }

        shared_ptr<Func1Arg> DoFunc1Arg2()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoFunc1Arg2" << std::endl;
            #endif
            Func1Arg *f = new Func1Arg;
            f->m_type = Func1Arg::COS;
            return shared_ptr<Func1Arg>(f);
        }

        shared_ptr<Func1Arg> DoFunc1Arg3()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoFunc1Arg3" << std::endl;
            #endif
            Func1Arg *f = new Func1Arg;
            f->m_type = Func1Arg::TAN;
            return shared_ptr<Func1Arg>(f);
        }

        shared_ptr<Func1Arg> DoFunc1Arg4()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoFunc1Arg4" << std::endl;
            #endif
            Func1Arg *f = new Func1Arg;
            f->m_type = Func1Arg::ASIN;
            return shared_ptr<Func1Arg>(f);
        }

        shared_ptr<Func1Arg> DoFunc1Arg5()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoFunc1Arg5" << std::endl;
            #endif
            Func1Arg *f = new Func1Arg;
            f->m_type = Func1Arg::ACOS;
            return shared_ptr<Func1Arg>(f);
        }

        shared_ptr<Func1Arg> DoFunc1Arg6()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoFunc1Arg6" << std::endl;
            #endif
            Func1Arg *f = new Func1Arg;
            f->m_type = Func1Arg::ATAN;
            return shared_ptr<Func1Arg>(f);
        }

        shared_ptr<Func1Arg> DoFunc1Arg7()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoFunc1Arg7" << std::endl;
            #endif
            Func1Arg *f = new Func1Arg;
            f->m_type = Func1Arg::SQRT;
            return shared_ptr<Func1Arg>(f);
        }

        shared_ptr<Func1Arg> DoFunc1Arg8()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoFunc1Arg8" << std::endl;
            #endif
            Func1Arg *f = new Func1Arg;
            f->m_type = Func1Arg::ABS;
            return shared_ptr<Func1Arg>(f);
        }

        shared_ptr<Func1Arg> DoFunc1Arg9()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoFunc1Arg9" << std::endl;
            #endif
            Func1Arg *f = new Func1Arg;
            f->m_type = Func1Arg::INV;
            return shared_ptr<Func1Arg>(f);
        }

        shared_ptr<Func1Arg> DoFunc1Arg10()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoFunc1Arg10" << std::endl;
            #endif
            Func1Arg *f = new Func1Arg;
            f->m_type = Func1Arg::EXP;
            return shared_ptr<Func1Arg>(f);
        }

        shared_ptr<Func1Arg> DoFunc1Arg11()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoFunc1Arg11" << std::endl;
            #endif
            Func1Arg *f = new Func1Arg;
            f->m_type = Func1Arg::LOG;
            return shared_ptr<Func1Arg>(f);
        }

        shared_ptr<Func1Arg> DoFunc1Arg12()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoFunc1Arg12" << std::endl;
            #endif
            Func1Arg *f = new Func1Arg;
            f->m_type = Func1Arg::LOG10;
            return shared_ptr<Func1Arg>(f);
        }

        shared_ptr<Func1Arg> DoFunc1Arg13()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoFunc1Arg13" << std::endl;
            #endif
            Func1Arg *f = new Func1Arg;
            f->m_type = Func1Arg::SINH;
            return shared_ptr<Func1Arg>(f);
        }

        shared_ptr<Func1Arg> DoFunc1Arg14()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoFunc1Arg14" << std::endl;
            #endif
            Func1Arg *f = new Func1Arg;
            f->m_type = Func1Arg::COSH;
            return shared_ptr<Func1Arg>(f);
        }

        shared_ptr<Func1Arg> DoFunc1Arg15()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoFunc1Arg15" << std::endl;
            #endif
            Func1Arg *f = new Func1Arg;
            f->m_type = Func1Arg::TANH;
            return shared_ptr<Func1Arg>(f);
        }

        shared_ptr<VecFunc> DoVecFunc1()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoVecFunc1" << std::endl;
            #endif
            VecFunc *f = new VecFunc;
            f->m_type = VecFunc::COUNT;
            return shared_ptr<VecFunc>(f);
        }

        shared_ptr<VecFunc> DoVecFunc2()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoVecFunc2" << std::endl;
            #endif
            VecFunc *f = new VecFunc;
            f->m_type = VecFunc::MAX;
            return shared_ptr<VecFunc>(f);
        }

        shared_ptr<VecFunc> DoVecFunc3()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoVecFunc3" << std::endl;
            #endif
            VecFunc *f = new VecFunc;
            f->m_type = VecFunc::MIN;
            return shared_ptr<VecFunc>(f);
        }

        shared_ptr<VecFunc> DoVecFunc4()
        {
            #ifdef DEEPDEBUG
                std::cerr << "DoVecFunc4" << std::endl;
            #endif
            VecFunc *f = new VecFunc;
            f->m_type = VecFunc::AVERAGE;
            return shared_ptr<VecFunc>(f);
        }

    public:
              CH_Location& location()       { return m_location; }
        const CH_Location& location() const { return m_location; }

        //
        // errors and warnings
        //
        void message(const std::string& str)
        {
            error() = location().str();
            error() += ": ";
            error() += str;
        }

        void not_supported_yet(const std::string& str)
        {
            message(std::string("エラー: ") + str +
                    " はまだサポートされていません。");
        }

              std::string& error()       { return m_error; }
        const std::string& error() const { return m_error; }

    protected:
        CH_Location                     m_location;
        std::string                     m_error;
    };
} // namespace Calc_H

#endif  // ndef HPARSERSITE_H_
