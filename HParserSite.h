////////////////////////////////////////////////////////////////////////////
// HParserSite.h -- Hiragana Calculator Calc-H
// This file is part of Calc-H.  See file "ReadMe.txt" and "License.txt".
////////////////////////////////////////////////////////////////////////////

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
                std::cout << "DoSent1" << std::endl;
            #endif
            Sentence *s = new Sentence;
            s->m_type = Sentence::MONO;
            s->m_mono = mono;
            return shared_ptr<Sentence>(s);
        }

        shared_ptr<Sentence> DoSent3(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSent3" << std::endl;
            #endif
            Sentence *s = new Sentence;
            s->m_type = Sentence::SURUTO;
            s->m_suruto = suruto;
            return shared_ptr<Sentence>(s);
        }

        shared_ptr<Sentence> DoSent4(shared_ptr<Shite>& shite)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSent4" << std::endl;
            #endif
            Sentence *s = new Sentence;
            s->m_type = Sentence::SHITE;
            s->m_shite = shite;
            return shared_ptr<Sentence>(s);
        }

        shared_ptr<Sentence> DoSent6()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSent6" << std::endl;
            #endif
            Sentence *s = new Sentence;
            s->m_type = Sentence::EMPTY;
            return shared_ptr<Sentence>(s);
        }

        shared_ptr<Sentence> DoSent8(shared_ptr<Doms>& doms1, shared_ptr<Doms>& doms2)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSent8" << std::endl;
            #endif
            Sentence *s = new Sentence;
            s->m_type = Sentence::DOMS_IS_DOMS;
            s->m_doms1 = doms1;
            s->m_doms2 = doms2;
            return shared_ptr<Sentence>(s);
        }

        shared_ptr<Sentence> DoSent9(shared_ptr<Doms>& doms1, shared_ptr<Cnstr>& cnstr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSent9" << std::endl;
            #endif
            Sentence *s = new Sentence;
            s->m_type = Sentence::DOMS_IS_CNSTR;
            s->m_doms1 = doms1;
            s->m_cnstr = cnstr;
            return shared_ptr<Sentence>(s);
        }

        shared_ptr<Sentence> DoSent10(shared_ptr<Mono>& mono, shared_ptr<Doms>& doms2)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSent10" << std::endl;
            #endif
            Sentence *s = new Sentence;
            s->m_type = Sentence::MONO_IS_DOMS;
            s->m_mono = mono;
            s->m_doms2 = doms2;
            return shared_ptr<Sentence>(s);
        }

        shared_ptr<Sentence> DoSent11(shared_ptr<Mono>& mono, shared_ptr<Cnstr>& cnstr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSent11" << std::endl;
            #endif
            Sentence *s = new Sentence;
            s->m_type = Sentence::MONO_IS_CNSTR;
            s->m_mono = mono;
            s->m_cnstr = cnstr;
            return shared_ptr<Sentence>(s);
        }

        shared_ptr<Sentence> DoSent12(shared_ptr<Mono>& mono, shared_ptr<Cnstr>& cnstr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSent12" << std::endl;
            #endif
            Sentence *s = new Sentence;
            s->m_type = Sentence::MONO_IS_CNSTRED_BUNSUU;
            s->m_mono = mono;
            s->m_cnstr = cnstr;
            return shared_ptr<Sentence>(s);
        }

        shared_ptr<Sentence> DoSent13(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSent13" << std::endl;
            #endif
            Sentence *s = new Sentence;
            s->m_type = Sentence::MONO_IS_BUNSUU;
            s->m_mono = mono;
            return shared_ptr<Sentence>(s);
        }

        shared_ptr<Sentence> DoSent14(shared_ptr<Mono>& mono, shared_ptr<Cnstr>& cnstr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSent14" << std::endl;
            #endif
            Sentence *s = new Sentence;
            s->m_type = Sentence::MONO_IS_CNSTRED_SHOUSUU;
            s->m_mono = mono;
            s->m_cnstr = cnstr;
            return shared_ptr<Sentence>(s);
        }

        shared_ptr<Sentence> DoSent15(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSent15" << std::endl;
            #endif
            Sentence *s = new Sentence;
            s->m_type = Sentence::MONO_IS_SHOUSUU;
            s->m_mono = mono;
            return shared_ptr<Sentence>(s);
        }

        shared_ptr<Sentence> DoSent16(shared_ptr<Mono>& mono1, shared_ptr<Mono>& mono2)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSent16" << std::endl;
            #endif
            Sentence *s = new Sentence;
            s->m_type = Sentence::MONO_IS_MONO;
            s->m_mono = mono1;
            s->m_mono2 = mono2;
            return shared_ptr<Sentence>(s);
        }

        shared_ptr<Sentence> DoSent17(shared_ptr<Sentence>& sentence)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSent17" << std::endl;
            #endif
            return sentence;
        }

        shared_ptr<Sentence> DoSent18(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSent18" << std::endl;
            #endif
            Sentence *s = new Sentence;
            s->m_type = Sentence::MONO_IS_TASHIZAN;
            s->m_mono = mono;
            return shared_ptr<Sentence>(s);
        }

        shared_ptr<Sentence> DoSent19(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSent19" << std::endl;
            #endif
            Sentence *s = new Sentence;
            s->m_type = Sentence::MONO_IS_HIKIZAN;
            s->m_mono = mono;
            return shared_ptr<Sentence>(s);
        }

        shared_ptr<Sentence> DoSent20(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSent20" << std::endl;
            #endif
            Sentence *s = new Sentence;
            s->m_type = Sentence::MONO_IS_KAKEZAN;
            s->m_mono = mono;
            return shared_ptr<Sentence>(s);
        }

        shared_ptr<Sentence> DoSent21(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSent21" << std::endl;
            #endif
            Sentence *s = new Sentence;
            s->m_type = Sentence::MONO_IS_WARIZAN;
            s->m_mono = mono;
            return shared_ptr<Sentence>(s);
        }

        shared_ptr<Sentence> DoSent22(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSent22" << std::endl;
            #endif
            Sentence *s = new Sentence;
            s->m_type = Sentence::SURUTO_TASHIZAN;
            s->m_suruto = suruto;
            return shared_ptr<Sentence>(s);
        }

        shared_ptr<Sentence> DoSent23(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSent23" << std::endl;
            #endif
            Sentence *s = new Sentence;
            s->m_type = Sentence::SURUTO_HIKIZAN;
            s->m_suruto = suruto;
            return shared_ptr<Sentence>(s);
        }

        shared_ptr<Sentence> DoSent24(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSent24" << std::endl;
            #endif
            Sentence *s = new Sentence;
            s->m_type = Sentence::SURUTO_KAKEZAN;
            s->m_suruto = suruto;
            return shared_ptr<Sentence>(s);
        }

        shared_ptr<Sentence> DoSent25(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSent25" << std::endl;
            #endif
            Sentence *s = new Sentence;
            s->m_type = Sentence::SURUTO_WARIZAN;
            s->m_suruto = suruto;
            return shared_ptr<Sentence>(s);
        }

        shared_ptr<Sentence> DoSent26(shared_ptr<Doms>& doms1, shared_ptr<ExprList>& exprlist)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSent26" << std::endl;
            #endif
            Sentence *s = new Sentence;
            s->m_type = Sentence::DOMS_IS_EXPRLIST;
            s->m_doms1 = doms1;
            s->m_exprlist = exprlist;
            return shared_ptr<Sentence>(s);
        }

        shared_ptr<Sentence> DoSent27(shared_ptr<Doms>& doms1, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSent27" << std::endl;
            #endif
            Sentence *s = new Sentence;
            s->m_type = Sentence::DOMS_IS_EXPR;
            s->m_doms1 = doms1;
            s->m_expr = expr;
            return shared_ptr<Sentence>(s);
        }

        shared_ptr<Sentence> DoSent28(shared_ptr<Doms>& doms1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSent28" << std::endl;
            #endif
            Sentence *s = new Sentence;
            s->m_type = Sentence::DOMS_IS_WHAT;
            s->m_doms1 = doms1;
            return shared_ptr<Sentence>(s);
        }

        shared_ptr<Sentence> DoSent29(shared_ptr<ExprList>& exprlist)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSent29" << std::endl;
            #endif
            Sentence *s = new Sentence;
            s->m_type = Sentence::TAGAINISO;
            s->m_exprlist = exprlist;
            return shared_ptr<Sentence>(s);
        }

        shared_ptr<Sentence> DoSent30(shared_ptr<Doms>& doms1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSent30" << std::endl;
            #endif
            Sentence *s = new Sentence;
            s->m_type = Sentence::DOES_DOMS_EXIST;
            s->m_doms1 = doms1;
            return shared_ptr<Sentence>(s);
        }

        shared_ptr<Sentence> DoSent31(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSent31" << std::endl;
            #endif
            Sentence *s = new Sentence;
            s->m_type = Sentence::MONO_SOINSUUBUNKAI;
            s->m_mono = mono;
            return shared_ptr<Sentence>(s);
        }

        shared_ptr<Node> DoWakaran1()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoWakaran1" << std::endl;
            #endif
            return shared_ptr<Node>();
        }

        shared_ptr<Node> DoOshiete1()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoOshiete1" << std::endl;
            #endif
            return shared_ptr<Node>();
        }

        shared_ptr<Suruto> DoSuruto1(shared_ptr<ExprList>& exprlist)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSuruto1" << std::endl;
            #endif
            Suruto *s = new Suruto;
            s->m_type = Suruto::EXPRLIST_ADD;
            s->m_exprlist = exprlist;
            return shared_ptr<Suruto>(s);
        }

        shared_ptr<Suruto> DoSuruto2(shared_ptr<ExprList>& exprlist)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSuruto2" << std::endl;
            #endif
            Suruto *s = new Suruto;
            s->m_type = Suruto::EXPRLIST_MUL;
            s->m_exprlist = exprlist;
            return shared_ptr<Suruto>(s);
        }

        shared_ptr<Suruto> DoSuruto3(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSuruto3" << std::endl;
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
                std::cout << "DoSuruto4" << std::endl;
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
                std::cout << "DoSuruto5" << std::endl;
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
                std::cout << "DoSuruto6" << std::endl;
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
                std::cout << "DoSuruto7" << std::endl;
            #endif
            Suruto *s = new Suruto;
            s->m_type = Suruto::MONO_ONLY;
            s->m_mono = mono;
            return shared_ptr<Suruto>(s);
        }

        shared_ptr<Suruto> DoSuruto8(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSuruto8" << std::endl;
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
                std::cout << "DoSuruto9" << std::endl;
            #endif
            Suruto *s = new Suruto;
            s->m_type = Suruto::MONO_WO_BAI;
            s->m_mono = mono;
            return shared_ptr<Suruto>(s);
        }

        shared_ptr<Suruto> DoSuruto10(shared_ptr<Shite>& shite, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSuruto10" << std::endl;
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
                std::cout << "DoSuruto11" << std::endl;
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
                std::cout << "DoSuruto12" << std::endl;
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
                std::cout << "DoSuruto13" << std::endl;
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
                std::cout << "DoSuruto16" << std::endl;
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
                std::cout << "DoSuruto17" << std::endl;
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
                std::cout << "DoSuruto18" << std::endl;
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
                std::cout << "DoSuruto19" << std::endl;
            #endif
            Suruto *s = new Suruto;
            s->m_type = Suruto::MONO_WO_HEIHOU;
            s->m_mono = mono;
            return shared_ptr<Suruto>(s);
        }

        shared_ptr<Suruto> DoSuruto20(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSuruto20" << std::endl;
            #endif
            Suruto *s = new Suruto;
            s->m_type = Suruto::MONO_WO_RIPPOU;
            s->m_mono = mono;
            return shared_ptr<Suruto>(s);
        }

        shared_ptr<Suruto> DoSuruto21(shared_ptr<Shite>& shite)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSuruto21" << std::endl;
            #endif
            Suruto *s = new Suruto;
            s->m_type = Suruto::SHOUSUU;
            s->m_shite = shite;
            return shared_ptr<Suruto>(s);
        }

        shared_ptr<Suruto> DoSuruto22(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSuruto22" << std::endl;
            #endif
            Suruto *s = new Suruto;
            s->m_type = Suruto::MONO_WO_SHOUSUU;
            s->m_mono = mono;
            return shared_ptr<Suruto>(s);
        }

        shared_ptr<Shite> DoShite1(shared_ptr<ExprList>& exprlist)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoShite1" << std::endl;
            #endif
            Shite *s = new Shite;
            s->m_type = Shite::EXPRLIST_ADD;
            s->m_exprlist = exprlist;
            return shared_ptr<Shite>(s);
        }

        shared_ptr<Shite> DoShite2(shared_ptr<ExprList>& exprlist)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoShite2" << std::endl;
            #endif
            Shite *s = new Shite;
            s->m_type = Shite::EXPRLIST_MUL;
            s->m_exprlist = exprlist;
            return shared_ptr<Shite>(s);
        }

        shared_ptr<Shite> DoShite3(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoShite3" << std::endl;
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
                std::cout << "DoShite4" << std::endl;
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
                std::cout << "DoShite5" << std::endl;
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
                std::cout << "DoShite6" << std::endl;
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
                std::cout << "DoShite7" << std::endl;
            #endif
            Shite *s = new Shite;
            s->m_type = Shite::MONO_ONLY;
            s->m_mono = mono;
            return shared_ptr<Shite>(s);
        }

        shared_ptr<Shite> DoShite8(shared_ptr<Shite>& shite, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoShite8" << std::endl;
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
                std::cout << "DoShite9" << std::endl;
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
                std::cout << "DoShite10" << std::endl;
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
                std::cout << "DoShite11" << std::endl;
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
                std::cout << "DoShite12" << std::endl;
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
                std::cout << "DoShite13" << std::endl;
            #endif
            Shite *s = new Shite;
            s->m_type = Shite::SHITE_BAI;
            s->m_shite = shite;
            return shared_ptr<Shite>(s);
        }

        shared_ptr<Shite> DoShite16(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoShite16" << std::endl;
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
                std::cout << "DoShite17" << std::endl;
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
                std::cout << "DoShite18" << std::endl;
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
                std::cout << "DoShite19" << std::endl;
            #endif
            Shite *s = new Shite;
            s->m_type = Shite::SHITE_HEIHOU;
            s->m_shite = shite;
            return shared_ptr<Shite>(s);
        }

        shared_ptr<Shite> DoShite20(shared_ptr<Shite>& shite)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoShite20" << std::endl;
            #endif
            Shite *s = new Shite;
            s->m_type = Shite::SHITE_RIPPOU;
            s->m_shite = shite;
            return shared_ptr<Shite>(s);
        }

        shared_ptr<Shite> DoShite21(shared_ptr<Shite>& shite)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoShite21" << std::endl;
            #endif
            Shite *s = new Shite;
            s->m_type = Shite::SHOUSUU;
            s->m_shite = shite;
            return shared_ptr<Shite>(s);
        }

        shared_ptr<Mono> DoMono1(shared_ptr<ExprList>& exprlist)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono1" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::EXPRLIST_ADD;
            m->m_exprlist = exprlist;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono2(shared_ptr<ExprList>& exprlist)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono2" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::EXPRLIST_MUL;
            m->m_exprlist = exprlist;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono3(shared_ptr<ExprList>& exprlist)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono3" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::EXPRLIST_SUB;
            m->m_exprlist = exprlist;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono4(shared_ptr<ExprList>& exprlist)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono4" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::EXPRLIST_DIV;
            m->m_exprlist = exprlist;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono5(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono5" << std::endl;
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
                std::cout << "DoMono6" << std::endl;
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
                std::cout << "DoMono7" << std::endl;
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
                std::cout << "DoMono8" << std::endl;
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
                std::cout << "DoMono9" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_ONLY;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono10(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono10" << std::endl;
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
                std::cout << "DoMono11" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_BAI;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono12(shared_ptr<Shite>& shite, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono12" << std::endl;
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
                std::cout << "DoMono13" << std::endl;
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
                std::cout << "DoMono14" << std::endl;
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
                std::cout << "DoMono15" << std::endl;
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
                std::cout << "DoMono16" << std::endl;
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
                std::cout << "DoMono17" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SHITE_BAI;
            m->m_shite = shite;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono18(shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono18" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::EXPR_ONLY;
            m->m_expr = expr;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono19()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono19" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SORE;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono20(shared_ptr<Mono>& mono, shared_ptr<ExprList>& exprlist)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono20" << std::endl;
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
                std::cout << "DoMono21" << std::endl;
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
                std::cout << "DoMono22" << std::endl;
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
                std::cout << "DoMono23" << std::endl;
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
                std::cout << "DoMono24" << std::endl;
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
                std::cout << "DoMono25" << std::endl;
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
                std::cout << "DoMono26" << std::endl;
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
                std::cout << "DoMono27" << std::endl;
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
                std::cout << "DoMono28" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_NO_TASUKAZU;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono29(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono29" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_NO_KAKERUKAZU;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono30(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono30" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_NO_HIKUKAZU;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono31(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono31" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_NO_WARUKAZU;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono32(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono32" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SURUTOKI_TASUKAZU;
            m->m_suruto = suruto;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono33(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono33" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SURUTOKI_KAKERUKAZU;
            m->m_suruto = suruto;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono34(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono34" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SURUTOKI_HIKUKAZU;
            m->m_suruto = suruto;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono35(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono35" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SURUTOKI_WARUKAZU;
            m->m_suruto = suruto;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono36(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono36" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SURUTO_ONLY;
            m->m_suruto = suruto;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono37(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono37" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SURUTOKI_TASARERUKAZU;
            m->m_suruto = suruto;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono38(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono38" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SURUTOKI_KAKERARERUKAZU;
            m->m_suruto = suruto;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono39(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono39" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SURUTOKI_HIKARERUKAZU;
            m->m_suruto = suruto;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono40(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono40" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SURUTOKI_WARARERUKAZU;
            m->m_suruto = suruto;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono41(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono41" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_NO_TASARERUKAZU;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono42(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono42" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_NO_KAKERARERUKAZU;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono43(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono43" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_NO_HIKARERUKAZU;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono44(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono44" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_NO_WARARERUKAZU;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono45()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono45" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SORE_NO_TASUKAZU;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono46()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono46" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SORE_NO_KAKERUKAZU;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono47()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono47" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SORE_NO_HIKUKAZU;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono48()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono48" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SORE_NO_WARUKAZU;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono49()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono49" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SORE_NO_TASARERUKAZU;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono50()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono50" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SORE_NO_KAKERARERUKAZU;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono51()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono51" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SORE_NO_HIKARERUKAZU;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono52()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono52" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SORE_NO_WARARERUKAZU;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono53(shared_ptr<Mono>& mono, shared_ptr<Func1Arg>& func1arg)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono53" << std::endl;
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
                std::cout << "DoMono54" << std::endl;
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
                std::cout << "DoMono55" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SHITE_HEIHOU;
            m->m_shite = shite;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono56(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono56" << std::endl;
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
                std::cout << "DoMono57" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_HEIHOU;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono58(shared_ptr<Shite>& shite)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono58" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SHITE_RIPPOU;
            m->m_shite = shite;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono59(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono59" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_RIPPOU;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono60(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono60" << std::endl;
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
                std::cout << "DoMono61" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::TASHIZAN;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono62(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono62" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::KAKEZAN;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono63(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono63" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::HIKIZAN;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono64(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono64" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::WARIZAN;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono65(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono65" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SURUTO_WA;
            m->m_suruto = suruto;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono66(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono66" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SURUTO_SEKI;
            m->m_suruto = suruto;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono67(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono67" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SURUTO_SA;
            m->m_suruto = suruto;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono68(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono68" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SURUTO_SHOU;
            m->m_suruto = suruto;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono69(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono69" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SURUTO_AMARI;
            m->m_suruto = suruto;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono70(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono70" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_NO_AMARI;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono71()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono71" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SORE_NO_AMARI;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono72(shared_ptr<Shite>& shite)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono72" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SHOUSUU;
            m->m_shite = shite;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono74(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono74" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_NO_SHOUSUU;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono75(shared_ptr<ExprList>& exprlist, shared_ptr<VecFunc>& vf)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono75" << std::endl;
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
                std::cout << "DoMono76" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_SHOU_TO_AMARI;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono77(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono77" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SURUTO_SHOU_TO_AMARI;
            m->m_suruto = suruto;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono78(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono78" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_HEIHOUKON;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono79(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono79" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_KAIJOU;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono80(shared_ptr<Doms>& doms)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono80" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::DOMS_SUM;
            m->m_doms = doms;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono81(shared_ptr<Doms>& doms)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono81" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::DOMS_PROD;
            m->m_doms = doms;
            return shared_ptr<Mono>(m);
        }

       shared_ptr<Mono> DoMono82(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono82" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_EXPR_WARIBIKI;
            m->m_mono = mono;
            m->m_expr = expr;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono83(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono83" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_EXPR_WARIMASHI;
            m->m_mono = mono;
            m->m_expr = expr;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono84(shared_ptr<ExprList>& exprlist, shared_ptr<Doms>& doms)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono84" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::EXPRLIST_NOUCHI_DOMS;
            m->m_exprlist = exprlist;
            m->m_doms = doms;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono85(shared_ptr<ExprList>& exprlist)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono85" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::EXPRLIST_INTDIV;
            m->m_exprlist = exprlist;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono86(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono64" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SEISHOU;
            m->m_mono = mono;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono87(shared_ptr<Mono>& mono, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono87" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::MONO_TO_EXPR_INTDIV;
            m->m_mono = mono;
            m->m_expr = expr;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Mono> DoMono88(shared_ptr<Suruto>& suruto)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMono88" << std::endl;
            #endif
            Mono *m = new Mono;
            m->m_type = Mono::SURUTO_SEISHOU;
            m->m_suruto = suruto;
            return shared_ptr<Mono>(m);
        }

        shared_ptr<Expr> DoExpr1(shared_ptr<Expr>& expr, shared_ptr<Term>& term)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoExpr1" << std::endl;
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
                std::cout << "DoExpr2" << std::endl;
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
                std::cout << "DoExpr3" << std::endl;
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
                std::cout << "DoExpr4" << std::endl;
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
                std::cout << "DoExpr5" << std::endl;
            #endif
            Expr *e = new Expr;
            e->m_type = Expr::TERM_ONLY;
            e->m_term = term;
            return shared_ptr<Expr>(e);
        }

        shared_ptr<Term> DoTerm1(shared_ptr<Term>& term, shared_ptr<Fact>& fact)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoTerm1" << std::endl;
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
                std::cout << "DoTerm2" << std::endl;
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
                std::cout << "DoTerm3" << std::endl;
            #endif
            Term *t = new Term;
            t->m_type = Term::FACT_ONLY;
            t->m_fact = fact;
            return shared_ptr<Term>(t);
        }

        shared_ptr<Fact> DoFact1(shared_ptr<Fact>& fact, shared_ptr<Prim>& prim)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoFact1" << std::endl;
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
                std::cout << "DoFact2" << std::endl;
            #endif
            Fact *f = new Fact;
            f->m_type = Fact::POW2;
            f->m_fact = fact;
            return shared_ptr<Fact>(f);
        }

        shared_ptr<Fact> DoFact3(shared_ptr<Prim>& prim)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoFact3" << std::endl;
            #endif
            Fact *f = new Fact;
            f->m_type = Fact::SINGLE;
            f->m_prim = prim;
            return shared_ptr<Fact>(f);
        }

        shared_ptr<Fact> DoFact4(shared_ptr<Fact>& fact)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoFact4" << std::endl;
            #endif
            Fact *f = new Fact;
            f->m_type = Fact::POW3;
            f->m_fact = fact;
            return shared_ptr<Fact>(f);
        }

        shared_ptr<Fact> DoFact5(shared_ptr<Fact>& fact)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoFact5" << std::endl;
            #endif
            Fact *f = new Fact;
            f->m_type = Fact::KAIJOU;
            f->m_fact = fact;
            return shared_ptr<Fact>(f);
        }

        shared_ptr<Fact> DoFact6(shared_ptr<Fact>& fact, shared_ptr<Prim>& prim)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoFact6" << std::endl;
            #endif
            Fact *f = new Fact;
            f->m_type = Fact::PERCENT;
            f->m_fact = fact;
            f->m_prim = prim;
            return shared_ptr<Fact>(f);
        }

        shared_ptr<Fact> DoFact7(shared_ptr<Fact>& fact, shared_ptr<Prim>& prim)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoFact7" << std::endl;
            #endif
            Fact *f = new Fact;
            f->m_type = Fact::WARIBIKI;
            f->m_fact = fact;
            f->m_prim = prim;
            return shared_ptr<Fact>(f);
        }

        shared_ptr<Fact> DoFact8(shared_ptr<Fact>& fact, shared_ptr<Prim>& prim)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoFact8" << std::endl;
            #endif
            Fact *f = new Fact;
            f->m_type = Fact::WARIMASHI;
            f->m_fact = fact;
            f->m_prim = prim;
            return shared_ptr<Fact>(f);
        }

        shared_ptr<Fact> DoFact9(shared_ptr<Fact>& fact)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoFact9" << std::endl;
            #endif
            Fact *f = new Fact;
            f->m_type = Fact::HEIHOUKON;
            f->m_fact = fact;
            return shared_ptr<Fact>(f);
        }

        shared_ptr<Fact> DoFact10(shared_ptr<Fact>& fact, shared_ptr<Prim>& prim)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoFact10" << std::endl;
            #endif
            Fact *f = new Fact;
            f->m_type = Fact::POWER;
            f->m_fact = fact;
            f->m_prim = prim;
            return shared_ptr<Fact>(f);
        }

        shared_ptr<Prim> DoPrim1(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoPrim1" << std::endl;
            #endif
            Prim *p = new Prim;
            p->m_type = Prim::MONO;
            p->m_mono = mono;
            return shared_ptr<Prim>(p);
        }

        shared_ptr<Prim> DoPrim2(shared_ptr<Prim>& prim)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoPrim2" << std::endl;
            #endif
            Prim *p = new Prim;
            p->m_type = Prim::MINUS;
            p->m_prim = prim;
            return shared_ptr<Prim>(p);
        }

        shared_ptr<Prim> DoPrim3(shared_ptr<Prim>& prim, shared_ptr<Num>& num)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoPrim3" << std::endl;
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
                std::cout << "DoPrim4" << std::endl;
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
                std::cout << "DoPrim5" << std::endl;
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
                std::cout << "DoPrim6" << std::endl;
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
                std::cout << "DoPrim7" << std::endl;
            #endif
            Prim *p = new Prim;
            p->m_type = Prim::DO;
            p->m_prim = prim;
            return shared_ptr<Prim>(p);
        }

        shared_ptr<Prim> DoPrim8()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoPrim8" << std::endl;
            #endif
            Prim *p = new Prim;
            p->m_type = Prim::PI;
            return shared_ptr<Prim>(p);
        }

        shared_ptr<Prim> DoPrim9()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoPrim9" << std::endl;
            #endif
            Prim *p = new Prim;
            p->m_type = Prim::E;
            return shared_ptr<Prim>(p);
        }

        shared_ptr<Prim> DoPrim10(shared_ptr<Func1Arg>& func1arg, shared_ptr<Prim>& prim)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoPrim10" << std::endl;
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
                std::cout << "DoPrim11" << std::endl;
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
                std::cout << "DoPrim12" << std::endl;
            #endif
            Prim *p = new Prim;
            p->m_type = Prim::FUNC1ARG_RIPPOU;
            p->m_func1arg = func1arg;
            p->m_prim = prim;
            return shared_ptr<Prim>(p);
        }

        shared_ptr<Prim> DoPrim13(shared_ptr<Prim>& prim)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoPrim13" << std::endl;
            #endif
            Prim *p = new Prim;
            p->m_type = Prim::FACTORIAL;
            p->m_prim = prim;
            return shared_ptr<Prim>(p);
        }

        shared_ptr<Prim> DoPrim14(shared_ptr<Prim>& prim)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoPrim14" << std::endl;
            #endif
            Prim *p = new Prim;
            p->m_type = Prim::NTH_PRIME;
            p->m_prim = prim;
            return shared_ptr<Prim>(p);
        }

        shared_ptr<ExprList> DoExprList1(shared_ptr<ExprList>& exprlist, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoExprList1" << std::endl;
            #endif
            exprlist.get()->push_back(expr);
            return exprlist;
        }

        shared_ptr<ExprList> DoExprList2(shared_ptr<ExprList>& exprlist, shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoExprList2" << std::endl;
            #endif
            exprlist.get()->push_back(expr);
            return exprlist;
        }

        shared_ptr<ExprList> DoExprList3(shared_ptr<Expr>& expr1, shared_ptr<Expr>& expr2)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoExprList3" << std::endl;
            #endif
            ExprList *el = new ExprList;
            el->push_back(expr1);
            el->push_back(expr2);
            return shared_ptr<ExprList>(el);
        }

        shared_ptr<ExprList> DoExprList4(shared_ptr<Expr>& expr1, shared_ptr<Expr>& expr2)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoExprList4" << std::endl;
            #endif
            ExprList *el = new ExprList;
            el->push_back(expr1);
            el->push_back(expr2);
            return shared_ptr<ExprList>(el);
        }

        shared_ptr<Num> DoNum1(shared_ptr<Num>& num, shared_ptr<Digits>& digits)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoNum1" << std::endl;
            #endif
            num.get()->m_value =
                CH_Value(num->m_value.str() + "." + digits->str());
            return num;
        }

        shared_ptr<Num> DoNum2(shared_ptr<Digits>& digits)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoNum2" << std::endl;
            #endif
            Num *num = new Num;
            num->m_value = CH_Value("0." + digits->str());
            return shared_ptr<Num>(num);
        }

        shared_ptr<Num> DoNum3(shared_ptr<Num>& num)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoNum3" << std::endl;
            #endif
            return num;
        }

        shared_ptr<Num> DoNum4()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoNum4" << std::endl;
            #endif
            Num *n = new Num;
            n->m_value = ch_zero;
            return shared_ptr<Num>(n);
        }

        shared_ptr<Num> DoMuryoutaisuuLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMuryoutaisuuLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoMuryoutaisuuLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMuryoutaisuuLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoMuryoutaisuuNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMuryoutaisuuNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_muryoutaisuu;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoMuryoutaisuuNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoMuryoutaisuuNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_muryoutaisuu;
            return num1;
        }

        shared_ptr<Num> DoFukashigiLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoFukashigiLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoFukashigiLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoFukashigiLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoFukashigiNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoFukashigiNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_fukashigi;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoFukashigiNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoFukashigiNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_fukashigi;
            return num1;
        }

        shared_ptr<Num> DoNayutaLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoNayutaLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoNayutaLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoNayutaLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoNayutaNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoNayutaNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_nayuta;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoNayutaNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoNayutaNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_nayuta;
            return num1;
        }

        shared_ptr<Num> DoAsougiLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoAsougiLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoAsougiLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoAsougiLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoAsougiNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoAsougiNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_asougi;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoAsougiNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoAsougiNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_asougi;
            return num1;
        }

        shared_ptr<Num> DoGougashaLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoGougashaLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoGougashaLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoGougashaLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoGougashaNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoGougashaNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_gougasha;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoGougashaNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoGougashaNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_gougasha;
            return num1;
        }

        shared_ptr<Num> DoGokuLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoGokuLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoGokuLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoGokuLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoGokuNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoGokuNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_goku;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoGokuNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoGokuNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_goku;
            return num1;
        }

        shared_ptr<Num> DoSaiLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSaiLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoSaiLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSaiLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoSaiNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSaiNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_sai;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoSaiNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSaiNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_sai;
            return num1;
        }

        shared_ptr<Num> DoSeiLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSeiLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoSeiLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSeiLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoSeiNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSeiNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_sei;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoSeiNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSeiNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_sei;
            return num1;
        }

        shared_ptr<Num> DoKanLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoKanLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoKanLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoKanLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoKanNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoKanNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_kan;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoKanNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoKanNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_kan;
            return num1;
        }

        shared_ptr<Num> DoKouLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoKouLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoKouLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoKouLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoKouNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoKouNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_kou;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoKouNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoKouNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_kou;
            return num1;
        }

        shared_ptr<Num> DoJouLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoJouLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoJouLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoJouLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoJouNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoJouNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_jou;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoJouNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoJouNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_jou;
            return num1;
        }

        shared_ptr<Num> DoJoLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoJoLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoJoLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoJoLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoJoNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoJoNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_jo;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoJoNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoJoNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_jo;
            return num1;
        }

        shared_ptr<Num> DoGaiLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoGaiLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoGaiLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoGaiLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoGaiNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoGaiNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_gai;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoGaiNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoGaiNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_gai;
            return num1;
        }

        shared_ptr<Num> DoKeiLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoKeyNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoKeiLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoKeyNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoKeiNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoKeiNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_kei;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoKeiNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoKeiNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_kei;
            return num1;
        }

        shared_ptr<Num> DoChouLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoChouLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoChouLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoChouLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoChouNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoChouNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_chou;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoChouNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoChouNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_chou;
            return num1;
        }

        shared_ptr<Num> DoOkuLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoOkuLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoOkuLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoOkuLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoOkuNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoOkuNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_oku;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoOkuNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoOkuNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_oku;
            return num1;
        }

        shared_ptr<Num> DoManLeNum1(shared_ptr<Num>& num)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoManLeNum1" << std::endl;
            #endif
            return num;
        }

        shared_ptr<Num> DoManLeNum2(shared_ptr<Num>& num)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoManLeNum2" << std::endl;
            #endif
            return num;
        }

        shared_ptr<Num> DoManNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoManNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_man;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoManNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoManNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_man;
            return num1;
        }

        shared_ptr<Num> DoSenLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSenLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoSenLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSenLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoSenNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSenNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_sen;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoSenNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSenNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_sen;
            return num1;
        }

        shared_ptr<Num> DoSenNum3(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSenNum3" << std::endl;
            #endif
            num1.get()->m_value += ch_sen;
            return num1;
        }

        shared_ptr<Num> DoSenNum4()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoSenNum4" << std::endl;
            #endif
            Num *n = new Num;
            n->m_value = ch_sen;
            return shared_ptr<Num>(n);
        }

        shared_ptr<Num> DoHyakuLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoHyakuLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoHyakuLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoHyakuLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoHyakuNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoHyakuNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_hyaku;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoHyakuNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoHyakuNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_hyaku;
            return num1;
        }

        shared_ptr<Num> DoHyakuNum3(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoHyakuNum3" << std::endl;
            #endif
            num1.get()->m_value += ch_hyaku;
            return num1;
        }

        shared_ptr<Num> DoHyakuNum4()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoHyakuNum4" << std::endl;
            #endif
            Num *n = new Num;
            n->m_value = ch_hyaku;
            return shared_ptr<Num>(n);
        }

        shared_ptr<Num> DoJuuLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoJuuLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoJuuLeNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoJuuLeNum2" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoJuuNum1(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoJuuNum1" << std::endl;
            #endif
            num1.get()->m_value *= ch_juu;
            num1.get()->m_value += num2->m_value;
            return num1;
        }

        shared_ptr<Num> DoJuuNum2(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoJuuNum2" << std::endl;
            #endif
            num1.get()->m_value *= ch_juu;
            return num1;
        }

        shared_ptr<Num> DoJuuNum3(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoJuuNum3" << std::endl;
            #endif
            num1.get()->m_value += ch_juu;
            return num1;
        }

        shared_ptr<Num> DoJuuNum4()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoJuuNum4" << std::endl;
            #endif
            Num *n = new Num;
            n->m_value = ch_juu;
            return shared_ptr<Num>(n);
        }

        shared_ptr<Num> DoIchiLeNum1(shared_ptr<Num>& num1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoIchiLeNum1" << std::endl;
            #endif
            return num1;
        }

        shared_ptr<Num> DoIchiNum1()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoIchiNum1" << std::endl;
            #endif
            Num *n = new Num;
            n->m_value = 1;
            return shared_ptr<Num>(n);
        }

        shared_ptr<Num> DoIchiNum2()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoIchiNum2" << std::endl;
            #endif
            Num *n = new Num;
            n->m_value = 2;
            return shared_ptr<Num>(n);
        }

        shared_ptr<Num> DoIchiNum3()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoIchiNum3" << std::endl;
            #endif
            Num *n = new Num;
            n->m_value = 3;
            return shared_ptr<Num>(n);
        }

        shared_ptr<Num> DoIchiNum4()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoIchiNum4" << std::endl;
            #endif
            Num *n = new Num;
            n->m_value = 4;
            return shared_ptr<Num>(n);
        }

        shared_ptr<Num> DoIchiNum5()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoIchiNum5" << std::endl;
            #endif
            Num *n = new Num;
            n->m_value = 5;
            return shared_ptr<Num>(n);
        }

        shared_ptr<Num> DoIchiNum6()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoIchiNum6" << std::endl;
            #endif
            Num *n = new Num;
            n->m_value = 6;
            return shared_ptr<Num>(n);
        }

        shared_ptr<Num> DoIchiNum7()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoIchiNum7" << std::endl;
            #endif
            Num *n = new Num;
            n->m_value = 7;
            return shared_ptr<Num>(n);
        }

        shared_ptr<Num> DoIchiNum8()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoIchiNum8" << std::endl;
            #endif
            Num *n = new Num;
            n->m_value = 8;
            return shared_ptr<Num>(n);
        }

        shared_ptr<Num> DoIchiNum9()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoIchiNum9" << std::endl;
            #endif
            Num *n = new Num;
            n->m_value = 9;
            return shared_ptr<Num>(n);
        }

        shared_ptr<Num> DoIchiNum10(shared_ptr<ChTokenInfo>& num)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoIchiNum10" << std::endl;
            #endif
            Num *n = new Num;
            n->m_value = CH_Value(num->text());
            return shared_ptr<Num>(n);
        }

        shared_ptr<Digits> DoDigits1(shared_ptr<Digits>& digits, shared_ptr<Digit>& digit)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoDigits1" << std::endl;
            #endif
            digits.get()->push_back(digit);
            return digits;
        }

        shared_ptr<Digits> DoDigits2(shared_ptr<Digit>& digit)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoDigits2" << std::endl;
            #endif
            Digits *digits = new Digits;
            digits->push_back(digit);
            return shared_ptr<Digits>(digits);
        }

        shared_ptr<Digit> DoDigit0()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoDigit0" << std::endl;
            #endif
            Digit *d = new Digit;
            d->m_digit = 0;
            return shared_ptr<Digit>(d);
        }

        shared_ptr<Digit> DoDigit1()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoDigit1" << std::endl;
            #endif
            Digit *d = new Digit;
            d->m_digit = 1;
            return shared_ptr<Digit>(d);
        }

        shared_ptr<Digit> DoDigit2()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoDigit2" << std::endl;
            #endif
            Digit *d = new Digit;
            d->m_digit = 2;
            return shared_ptr<Digit>(d);
        }

        shared_ptr<Digit> DoDigit3()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoDigit3" << std::endl;
            #endif
            Digit *d = new Digit;
            d->m_digit = 3;
            return shared_ptr<Digit>(d);
        }

        shared_ptr<Digit> DoDigit4()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoDigit4" << std::endl;
            #endif
            Digit *d = new Digit;
            d->m_digit = 4;
            return shared_ptr<Digit>(d);
        }

        shared_ptr<Digit> DoDigit5()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoDigit5" << std::endl;
            #endif
            Digit *d = new Digit;
            d->m_digit = 5;
            return shared_ptr<Digit>(d);
        }

        shared_ptr<Digit> DoDigit6()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoDigit6" << std::endl;
            #endif
            Digit *d = new Digit;
            d->m_digit = 6;
            return shared_ptr<Digit>(d);
        }

        shared_ptr<Digit> DoDigit7()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoDigit7" << std::endl;
            #endif
            Digit *d = new Digit;
            d->m_digit = 7;
            return shared_ptr<Digit>(d);
        }

        shared_ptr<Digit> DoDigit8()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoDigit8" << std::endl;
            #endif
            Digit *d = new Digit;
            d->m_digit = 8;
            return shared_ptr<Digit>(d);
        }

        shared_ptr<Digit> DoDigit9()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoDigit9" << std::endl;
            #endif
            Digit *d = new Digit;
            d->m_digit = 9;
            return shared_ptr<Digit>(d);
        }

        shared_ptr<Func1Arg> DoFunc1Arg1()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoFunc1Arg1" << std::endl;
            #endif
            Func1Arg *f = new Func1Arg;
            f->m_type = Func1Arg::SIN;
            return shared_ptr<Func1Arg>(f);
        }

        shared_ptr<Func1Arg> DoFunc1Arg2()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoFunc1Arg2" << std::endl;
            #endif
            Func1Arg *f = new Func1Arg;
            f->m_type = Func1Arg::COS;
            return shared_ptr<Func1Arg>(f);
        }

        shared_ptr<Func1Arg> DoFunc1Arg3()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoFunc1Arg3" << std::endl;
            #endif
            Func1Arg *f = new Func1Arg;
            f->m_type = Func1Arg::TAN;
            return shared_ptr<Func1Arg>(f);
        }

        shared_ptr<Func1Arg> DoFunc1Arg4()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoFunc1Arg4" << std::endl;
            #endif
            Func1Arg *f = new Func1Arg;
            f->m_type = Func1Arg::ASIN;
            return shared_ptr<Func1Arg>(f);
        }

        shared_ptr<Func1Arg> DoFunc1Arg5()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoFunc1Arg5" << std::endl;
            #endif
            Func1Arg *f = new Func1Arg;
            f->m_type = Func1Arg::ACOS;
            return shared_ptr<Func1Arg>(f);
        }

        shared_ptr<Func1Arg> DoFunc1Arg6()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoFunc1Arg6" << std::endl;
            #endif
            Func1Arg *f = new Func1Arg;
            f->m_type = Func1Arg::ATAN;
            return shared_ptr<Func1Arg>(f);
        }

        shared_ptr<Func1Arg> DoFunc1Arg7()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoFunc1Arg7" << std::endl;
            #endif
            Func1Arg *f = new Func1Arg;
            f->m_type = Func1Arg::SQRT;
            return shared_ptr<Func1Arg>(f);
        }

        shared_ptr<Func1Arg> DoFunc1Arg8()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoFunc1Arg8" << std::endl;
            #endif
            Func1Arg *f = new Func1Arg;
            f->m_type = Func1Arg::ABS;
            return shared_ptr<Func1Arg>(f);
        }

        shared_ptr<Func1Arg> DoFunc1Arg9()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoFunc1Arg9" << std::endl;
            #endif
            Func1Arg *f = new Func1Arg;
            f->m_type = Func1Arg::INV;
            return shared_ptr<Func1Arg>(f);
        }

        shared_ptr<Func1Arg> DoFunc1Arg10()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoFunc1Arg10" << std::endl;
            #endif
            Func1Arg *f = new Func1Arg;
            f->m_type = Func1Arg::EXP;
            return shared_ptr<Func1Arg>(f);
        }

        shared_ptr<Func1Arg> DoFunc1Arg11()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoFunc1Arg11" << std::endl;
            #endif
            Func1Arg *f = new Func1Arg;
            f->m_type = Func1Arg::LOG;
            return shared_ptr<Func1Arg>(f);
        }

        shared_ptr<Func1Arg> DoFunc1Arg12()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoFunc1Arg12" << std::endl;
            #endif
            Func1Arg *f = new Func1Arg;
            f->m_type = Func1Arg::LOG10;
            return shared_ptr<Func1Arg>(f);
        }

        shared_ptr<Func1Arg> DoFunc1Arg13()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoFunc1Arg13" << std::endl;
            #endif
            Func1Arg *f = new Func1Arg;
            f->m_type = Func1Arg::SINH;
            return shared_ptr<Func1Arg>(f);
        }

        shared_ptr<Func1Arg> DoFunc1Arg14()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoFunc1Arg14" << std::endl;
            #endif
            Func1Arg *f = new Func1Arg;
            f->m_type = Func1Arg::COSH;
            return shared_ptr<Func1Arg>(f);
        }

        shared_ptr<Func1Arg> DoFunc1Arg15()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoFunc1Arg15" << std::endl;
            #endif
            Func1Arg *f = new Func1Arg;
            f->m_type = Func1Arg::TANH;
            return shared_ptr<Func1Arg>(f);
        }

        shared_ptr<VecFunc> DoVecFunc1()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoVecFunc1" << std::endl;
            #endif
            VecFunc *f = new VecFunc;
            f->m_type = VecFunc::COUNT;
            return shared_ptr<VecFunc>(f);
        }

        shared_ptr<VecFunc> DoVecFunc2()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoVecFunc2" << std::endl;
            #endif
            VecFunc *f = new VecFunc;
            f->m_type = VecFunc::MAX;
            return shared_ptr<VecFunc>(f);
        }

        shared_ptr<VecFunc> DoVecFunc3()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoVecFunc3" << std::endl;
            #endif
            VecFunc *f = new VecFunc;
            f->m_type = VecFunc::MIN;
            return shared_ptr<VecFunc>(f);
        }

        shared_ptr<VecFunc> DoVecFunc4()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoVecFunc4" << std::endl;
            #endif
            VecFunc *f = new VecFunc;
            f->m_type = VecFunc::AVERAGE;
            return shared_ptr<VecFunc>(f);
        }

        shared_ptr<VecFunc> DoVecFunc5()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoVecFunc5" << std::endl;
            #endif
            VecFunc *f = new VecFunc;
            f->m_type = VecFunc::GCD;
            return shared_ptr<VecFunc>(f);
        }

        shared_ptr<VecFunc> DoVecFunc6()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoVecFunc6" << std::endl;
            #endif
            VecFunc *f = new VecFunc;
            f->m_type = VecFunc::LCM;
            return shared_ptr<VecFunc>(f);
        }

        shared_ptr<Doms> DoDoms1(shared_ptr<Doms>& doms, shared_ptr<Dom>& dom)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoDoms1" << std::endl;
            #endif
            doms.get()->push_back(dom);
            return doms;
        }

        shared_ptr<Doms> DoDoms2(shared_ptr<Dom>& dom)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoDoms2" << std::endl;
            #endif
            Doms *d = new Doms;
            d->push_back(dom);
            return shared_ptr<Doms>(d);
        }

        shared_ptr<Doms> DoDoms3(shared_ptr<Doms>& doms, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoDoms3" << std::endl;
            #endif
            Dom *dom = new Dom;
            dom->m_type = Dom::NUM_ONLY;
            dom->m_num = num2;
            doms.get()->push_back(shared_ptr<Dom>(dom));
            return doms;
        }

        shared_ptr<Doms> DoDoms4(shared_ptr<Num>& num1, shared_ptr<Num>& num2)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoDoms4" << std::endl;
            #endif
            Dom *dom1 = new Dom;
            dom1->m_type = Dom::NUM_ONLY;
            dom1->m_num = num1;
            Dom *dom2 = new Dom;
            dom2->m_type = Dom::NUM_ONLY;
            dom2->m_num = num2;
            Doms *doms = new Doms;
            doms->push_back(shared_ptr<Dom>(dom1));
            doms->push_back(shared_ptr<Dom>(dom2));
            return shared_ptr<Doms>(doms);
        }

        shared_ptr<Dom> DoDom1(shared_ptr<Cnstr>& cnstr, shared_ptr<PrimDom>& primdom)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoDom1" << std::endl;
            #endif
            Dom *d = new Dom;
            d->m_type = Dom::CNSTRED_PRIMDOM;
            d->m_cnstr = cnstr;
            d->m_primdom = primdom;
            return shared_ptr<Dom>(d);
        }

        shared_ptr<Dom> DoDom2(shared_ptr<PrimDom>& primdom)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoDom2" << std::endl;
            #endif
            Dom *d = new Dom;
            d->m_type = Dom::PRIMDOM_ONLY;
            d->m_primdom = primdom;
            return shared_ptr<Dom>(d);
        }

        shared_ptr<Dom> DoDom3(shared_ptr<Dom>& dom, shared_ptr<PrimDom>& primdom)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoDom3" << std::endl;
            #endif
            Dom *d = new Dom;
            d->m_type = Dom::DOM_OF_DOM;
            d->m_dom = dom;
            d->m_primdom = primdom;
            return shared_ptr<Dom>(d);
        }

        shared_ptr<Dom> DoDom4(shared_ptr<Expr>& expr1, shared_ptr<Expr>& expr2,
                               shared_ptr<PrimDom>& primdom)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoDom4" << std::endl;
            #endif
            Dom *d = new Dom;
            d->m_type = Dom::EXPR_KARA_EXPR_MADE;
            d->m_expr1 = expr1;
            d->m_expr2 = expr2;
            d->m_primdom = primdom;
            return shared_ptr<Dom>(d);
        }

        shared_ptr<Dom> DoDom5(shared_ptr<Dom>& dom, shared_ptr<Dom>& dom2)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoDom5" << std::endl;
            #endif
            Dom *d = new Dom;
            d->m_type = Dom::DOM_NOUCHI_DOM;
            d->m_dom = dom;
            d->m_dom2 = dom2;
            return shared_ptr<Dom>(d);
        }

        shared_ptr<Dom> DoDom6(shared_ptr<Dom>& dom, shared_ptr<Expr>& expr1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoDom6" << std::endl;
            #endif
            Dom *d = new Dom;
            d->m_type = Dom::DOM_ADD;
            d->m_dom = dom;
            d->m_expr1 = expr1;
            return shared_ptr<Dom>(d);
        }

        shared_ptr<Dom> DoDom7(shared_ptr<Dom>& dom, shared_ptr<Expr>& expr1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoDom7" << std::endl;
            #endif
            Dom *d = new Dom;
            d->m_type = Dom::DOM_SUB;
            d->m_dom = dom;
            d->m_expr1 = expr1;
            return shared_ptr<Dom>(d);
        }

        shared_ptr<Dom> DoDom8(shared_ptr<Dom>& dom, shared_ptr<Expr>& expr1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoDom8" << std::endl;
            #endif
            Dom *d = new Dom;
            d->m_type = Dom::DOM_MUL;
            d->m_dom = dom;
            d->m_expr1 = expr1;
            return shared_ptr<Dom>(d);
        }

        shared_ptr<Dom> DoDom9(shared_ptr<Dom>& dom, shared_ptr<Expr>& expr1)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoDom9" << std::endl;
            #endif
            Dom *d = new Dom;
            d->m_type = Dom::DOM_DIV;
            d->m_dom = dom;
            d->m_expr1 = expr1;
            return shared_ptr<Dom>(d);
        }

        shared_ptr<PrimDom> DoPrimDom1(shared_ptr<PrimDom>& primdom)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoPrimDom1" << std::endl;
            #endif
            PrimDom *pm = new PrimDom;
            pm->m_type = PrimDom::POSITIVE;
            pm->m_primdom = primdom;
            return shared_ptr<PrimDom>(pm);
        }

        shared_ptr<PrimDom> DoPrimDom2(shared_ptr<PrimDom>& primdom)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoPrimDom2" << std::endl;
            #endif
            PrimDom *pm = new PrimDom;
            pm->m_type = PrimDom::NEGATIVE;
            pm->m_primdom = primdom;
            return shared_ptr<PrimDom>(pm);
        }

        shared_ptr<PrimDom> DoPrimDom3()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoPrimDom3" << std::endl;
            #endif
            PrimDom *pm = new PrimDom;
            pm->m_type = PrimDom::SHIZENSUU;
            return shared_ptr<PrimDom>(pm);
        }

        shared_ptr<PrimDom> DoPrimDom4()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoPrimDom4" << std::endl;
            #endif
            PrimDom *pm = new PrimDom;
            pm->m_type = PrimDom::SEISUU;
            return shared_ptr<PrimDom>(pm);
        }

        shared_ptr<PrimDom> DoPrimDom5()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoPrimDom5" << std::endl;
            #endif
            PrimDom *pm = new PrimDom;
            pm->m_type = PrimDom::GUUSUU;
            return shared_ptr<PrimDom>(pm);
        }

        shared_ptr<PrimDom> DoPrimDom6()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoPrimDom6" << std::endl;
            #endif
            PrimDom *pm = new PrimDom;
            pm->m_type = PrimDom::KISUU;
            return shared_ptr<PrimDom>(pm);
        }

        shared_ptr<PrimDom> DoPrimDom7()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoPrimDom7" << std::endl;
            #endif
            PrimDom *pm = new PrimDom;
            pm->m_type = PrimDom::JISSUU;
            return shared_ptr<PrimDom>(pm);
        }

        shared_ptr<PrimDom> DoPrimDom8()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoPrimDom8" << std::endl;
            #endif
            PrimDom *pm = new PrimDom;
            pm->m_type = PrimDom::SOSUU;
            return shared_ptr<PrimDom>(pm);
        }

        shared_ptr<PrimDom> DoPrimDom9(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoPrimDom9" << std::endl;
            #endif
            PrimDom *pm = new PrimDom;
            pm->m_type = PrimDom::BAISUU;
            pm->m_mono = mono;
            return shared_ptr<PrimDom>(pm);
        }

        shared_ptr<PrimDom> DoPrimDom10(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoPrimDom10" << std::endl;
            #endif
            PrimDom *pm = new PrimDom;
            pm->m_type = PrimDom::YAKUSUU;
            pm->m_mono = mono;
            return shared_ptr<PrimDom>(pm);
        }

        shared_ptr<PrimDom> DoPrimDom11(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoPrimDom11" << std::endl;
            #endif
            PrimDom *pm = new PrimDom;
            pm->m_type = PrimDom::SOINSUU;
            pm->m_mono = mono;
            return shared_ptr<PrimDom>(pm);
        }

        shared_ptr<PrimDom> DoPrimDom12()
        {
            #ifdef DEEPDEBUG
                std::cout << "DoPrimDom12" << std::endl;
            #endif
            PrimDom *pm = new PrimDom;
            pm->m_type = PrimDom::GOUSEISUU;
            return shared_ptr<PrimDom>(pm);
        }

        shared_ptr<PrimDom> DoPrimDom13(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoPrimDom13" << std::endl;
            #endif
            PrimDom *pm = new PrimDom;
            pm->m_type = PrimDom::JIMEINAYAKUSUU;
            pm->m_mono = mono;
            return shared_ptr<PrimDom>(pm);
        }

        shared_ptr<PrimDom> DoPrimDom14(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoPrimDom14" << std::endl;
            #endif
            PrimDom *pm = new PrimDom;
            pm->m_type = PrimDom::SHINNOYAKUSUU;
            pm->m_mono = mono;
            return shared_ptr<PrimDom>(pm);
        }

        shared_ptr<PrimDom> DoPrimDom15(shared_ptr<ExprList>& exprlist)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoPrimDom15" << std::endl;
            #endif
            PrimDom *pm = new PrimDom;
            pm->m_type = PrimDom::KOUBAISUU;
            pm->m_exprlist = exprlist;
            return shared_ptr<PrimDom>(pm);
        }

        shared_ptr<PrimDom> DoPrimDom16(shared_ptr<ExprList>& exprlist)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoPrimDom16" << std::endl;
            #endif
            PrimDom *pm = new PrimDom;
            pm->m_type = PrimDom::KOUYAKUSUU;
            pm->m_exprlist = exprlist;
            return shared_ptr<PrimDom>(pm);
        }

        shared_ptr<Cnstr> DoCnstr1(shared_ptr<Cnstr>& cnstr, shared_ptr<AndCnstr>& andcnstr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoCnstr1" << std::endl;
            #endif
            Cnstr *c = new Cnstr;
            c->m_type = Cnstr::MATAHA;
            c->m_cnstr = cnstr;
            c->m_andcnstr = andcnstr;
            return shared_ptr<Cnstr>(c);
        }

        shared_ptr<Cnstr> DoCnstr2(shared_ptr<AndCnstr>& andcnstr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoCnstr2" << std::endl;
            #endif
            Cnstr *c = new Cnstr;
            c->m_type = Cnstr::SINGLE;
            c->m_andcnstr = andcnstr;
            return shared_ptr<Cnstr>(c);
        }

        shared_ptr<AndCnstr> DoAndCnstr1(shared_ptr<Expr>& expr, shared_ptr<AndCnstr>& andcnstr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoAndCnstr1" << std::endl;
            #endif
            AndCnstr *ac = new AndCnstr;
            ac->m_type = AndCnstr::IJOU;
            ac->m_expr = expr;
            ac->m_andcnstr = andcnstr;
            return shared_ptr<AndCnstr>(ac);
        }

        shared_ptr<AndCnstr> DoAndCnstr2(shared_ptr<Expr>& expr, shared_ptr<AndCnstr>& andcnstr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoAndCnstr2" << std::endl;
            #endif
            AndCnstr *ac = new AndCnstr;
            ac->m_type = AndCnstr::IKA;
            ac->m_expr = expr;
            ac->m_andcnstr = andcnstr;
            return shared_ptr<AndCnstr>(ac);
        }

        shared_ptr<AndCnstr> DoAndCnstr3(shared_ptr<Expr>& expr, shared_ptr<AndCnstr>& andcnstr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoAndCnstr3" << std::endl;
            #endif
            AndCnstr *ac = new AndCnstr;
            ac->m_type = AndCnstr::CHIISAI;
            ac->m_expr = expr;
            ac->m_andcnstr = andcnstr;
            return shared_ptr<AndCnstr>(ac);
        }

        shared_ptr<AndCnstr> DoAndCnstr4(shared_ptr<Expr>& expr, shared_ptr<AndCnstr>& andcnstr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoAndCnstr4" << std::endl;
            #endif
            AndCnstr *ac = new AndCnstr;
            ac->m_type = AndCnstr::OOKII;
            ac->m_expr = expr;
            ac->m_andcnstr = andcnstr;
            return shared_ptr<AndCnstr>(ac);
        }

        shared_ptr<AndCnstr> DoAndCnstr5(shared_ptr<Cnstr>& cnstr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoAndCnstr5" << std::endl;
            #endif
            AndCnstr *ac = new AndCnstr;
            ac->m_type = AndCnstr::CNSTR_ONLY;
            ac->m_cnstr = cnstr;
            return shared_ptr<AndCnstr>(ac);
        }

        shared_ptr<AndCnstr> DoAndCnstr6(shared_ptr<PrimCnstr>& primcnstr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoAndCnstr6" << std::endl;
            #endif
            AndCnstr *ac = new AndCnstr;
            ac->m_type = AndCnstr::PRIMCNSTR_ONLY;
            ac->m_primcnstr = primcnstr;
            return shared_ptr<AndCnstr>(ac);
        }

        shared_ptr<AndCnstr> DoAndCnstr7(shared_ptr<Expr>& expr, shared_ptr<AndCnstr>& andcnstr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoAndCnstr7" << std::endl;
            #endif
            AndCnstr *ac = new AndCnstr;
            ac->m_type = AndCnstr::HITOSHIKU;
            ac->m_expr = expr;
            ac->m_andcnstr = andcnstr;
            return shared_ptr<AndCnstr>(ac);
        }

        shared_ptr<AndCnstr> DoAndCnstr8(shared_ptr<Expr>& expr, shared_ptr<AndCnstr>& andcnstr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoAndCnstr8" << std::endl;
            #endif
            AndCnstr *ac = new AndCnstr;
            ac->m_type = AndCnstr::SEIJO;
            ac->m_expr = expr;
            ac->m_andcnstr = andcnstr;
            return shared_ptr<AndCnstr>(ac);
        }

        shared_ptr<PrimCnstr> DoPrimCnstr1(shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoPrimCnstr1" << std::endl;
            #endif
            PrimCnstr *pc = new PrimCnstr;
            pc->m_type = PrimCnstr::IJOU;
            pc->m_expr = expr;
            return shared_ptr<PrimCnstr>(pc);
        }

        shared_ptr<PrimCnstr> DoPrimCnstr2(shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoPrimCnstr2" << std::endl;
            #endif
            PrimCnstr *pc = new PrimCnstr;
            pc->m_type = PrimCnstr::IKA;
            pc->m_expr = expr;
            return shared_ptr<PrimCnstr>(pc);
        }

        shared_ptr<PrimCnstr> DoPrimCnstr3(shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoPrimCnstr3" << std::endl;
            #endif
            PrimCnstr *pc = new PrimCnstr;
            pc->m_type = PrimCnstr::CHIISAI;
            pc->m_expr = expr;
            return shared_ptr<PrimCnstr>(pc);
        }

        shared_ptr<PrimCnstr> DoPrimCnstr4(shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoPrimCnstr4" << std::endl;
            #endif
            PrimCnstr *pc = new PrimCnstr;
            pc->m_type = PrimCnstr::OOKII;
            pc->m_expr = expr;
            return shared_ptr<PrimCnstr>(pc);
        }

        shared_ptr<PrimCnstr> DoPrimCnstr5(shared_ptr<Mono>& mono)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoPrimCnstr5" << std::endl;
            #endif
            PrimCnstr *pc = new PrimCnstr;
            pc->m_type = PrimCnstr::WARIKIRU;
            pc->m_mono = mono;
            return shared_ptr<PrimCnstr>(pc);
        }

        shared_ptr<PrimCnstr> DoPrimCnstr6(shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoPrimCnstr6" << std::endl;
            #endif
            PrimCnstr *pc = new PrimCnstr;
            pc->m_type = PrimCnstr::HITOSHII;
            pc->m_expr = expr;
            return shared_ptr<PrimCnstr>(pc);
        }

        shared_ptr<PrimCnstr> DoPrimCnstr7(shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoPrimCnstr7" << std::endl;
            #endif
            PrimCnstr *pc = new PrimCnstr;
            pc->m_type = PrimCnstr::SEIJO;
            pc->m_expr = expr;
            return shared_ptr<PrimCnstr>(pc);
        }

        shared_ptr<PrimCnstr> DoPrimCnstr8(
            shared_ptr<Expr>& expr, shared_ptr<Expr>& expr2)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoPrimCnstr8" << std::endl;
            #endif
            PrimCnstr *pc = new PrimCnstr;
            pc->m_type = PrimCnstr::WARUTO_AMARU;
            pc->m_expr = expr;
            pc->m_expr2 = expr2;
            return shared_ptr<PrimCnstr>(pc);
        }

        shared_ptr<PrimCnstr> DoPrimCnstr9(shared_ptr<Expr>& expr)
        {
            #ifdef DEEPDEBUG
                std::cout << "DoPrimCnstr9" << std::endl;
            #endif
            PrimCnstr *pc = new PrimCnstr;
            pc->m_type = PrimCnstr::WARUTO_AMARANAI;
            pc->m_expr = expr;
            return shared_ptr<PrimCnstr>(pc);
        }

    public:
              CH_Location& location()       { return m_location; }
        const CH_Location& location() const { return m_location; }

        //
        // errors and warnings
        //
        void message(const std::string& str)
        {
            //error() = location().str();
            //error() += ": ";
            error() += str;
        }

        void not_supported_yet(const std::string& str)
        {
            message(std::string("エラー: ") + str +
                    " {は/わ}まだサポートされていません。");
        }

              std::string& error()       { return m_error; }
        const std::string& error() const { return m_error; }

    protected:
        CH_Location                     m_location;
        std::string                     m_error;
    };
} // namespace Calc_H

#endif  // ndef HPARSERSITE_H_
