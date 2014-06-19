// HScanner.h
// (Japanese, Shift_JIS)
// Linux�̏ꍇ�́AUTF-8�ɕϊ����ĉ������B

#ifndef HSCANNER_H_
#define HSCANNER_H_

namespace Calc_H
{
    //
    // Scanner<Iterator, ParserSite>
    //
    template <class Iterator, class ParserSite>
    class Scanner
    {
    public:
        typedef ParserSite                  parser_site_type;
        typedef Iterator                    iterator_type;
        typedef TokenInfo<Calc_H::Token>    info_type;
        typedef shared_ptr<info_type>       shared_info_type;

    public:
        Scanner(parser_site_type& parser_site) :
            m_parser_site(parser_site),
            m_index(0)
        {
        }

        void scan(std::vector<info_type>& infos, Iterator begin, Iterator end)
        {
            m_begin = begin;
            m_current = begin;
            m_end = end;

            Token token;
            info_type info;
            do
            {
                token = get_token(info);
                info.set_token(token);
                infos.push_back(info);
            } while (token != Calc_H::eof);

            resynth1(infos);
            if (resynth2(infos))
            {
                resynth3(infos);
                resynth4(infos);
                resynth5(infos);
                resynth6(infos);
                resynth7(infos);
                resynth8(infos);
                resynth9(infos);
                resynth10(infos);
                resynth11(infos);
                resynth12(infos);
            }
        }

        std::string token_to_string(const info_type& info)
        {
            std::string str = token_label(info.m_token);
            return str;
        }

        template <class TokenInfoIt>
        void show_tokens(TokenInfoIt begin, TokenInfoIt end)
        {
            for (TokenInfoIt it = begin; it != end; ++it)
            {
                std::cerr << token_to_string(*it) << " ";
            }
            std::cerr << std::endl;
        }

        Token get_token(TokenValue& info)
        {
            int space_count = 0;
            for (;;)
            {
                bool flag = false;
                while (lexeme(" ") || lexeme("�@") || lexeme("\t"))
                {
                    space_count++;
                    flag = true;
                }

                while (lexeme("������") || lexeme("��������") ||
                       lexeme("�����Ɓ[") || lexeme("�������Ɓ[") ||
                       lexeme("���̂�") || lexeme("���́[") || 
                       lexeme("���A") || lexeme("����") ||
                       lexeme("����ƂˁA") || lexeme("�����ƂˁA") || 
                       lexeme("����Ƃˁ[�A") || lexeme("�����Ƃˁ[�A") || 
                       lexeme("�Ƃ��") || lexeme("��������"))
                {
                    flag = true;
                }
                if (!flag)
                    break;
            }

            info.location() = location();

            if (space_count >= 2)
                return commit_token(T_COMMA);

            if (lexeme("\n"))
            {
                newline();
                return commit_token(T_PERIOD);
            }

            if (lexeme("���񂢂���"))               return (T_1X1);
            if (lexeme("����ɂ�"))                 return (T_1X2);
            if (lexeme("���񂳂�"))               return (T_1X3);
            if (lexeme("���񂵂�"))                 return (T_1X4);
            if (lexeme("���񂲂�"))                 return (T_1X5);
            if (lexeme("����낭��"))               return (T_1X6);
            if (lexeme("����Ђ���"))               return (T_1X7);
            if (lexeme("���񂵂���"))               return (T_1X7);
            if (lexeme("����͂���"))               return (T_1X8);
            if (lexeme("���񂭂�"))                 return (T_1X9);
            if (lexeme("�ɂ�����"))                 return (T_2X1);
            if (lexeme("�ɂɂ�"))                 return (T_2X2);
            if (lexeme("�ɂ���"))                 return (T_2X3);
            if (lexeme("�ɂ���"))                   return (T_2X4);
            //if (lexeme("�ɂ�"))                     return (T_2X5);   // T_NI T_GO
            if (lexeme("�ɂ낭"))                   return (T_2X6);
            if (lexeme("�ɂЂ�"))                   return (T_2X7);
            if (lexeme("�ɂ���"))                   return (T_2X7);
            if (lexeme("�ɂ͂�"))                   return (T_2X8);
            //if (lexeme("�ɂ�"))                     return (T_2X9);   // T_NI T_KYUU
            if (lexeme("���񂢂���"))               return (T_3X1);
            if (lexeme("����ɂ�"))                 return (T_3X2);
            if (lexeme("������"))                 return (T_3X3);
            if (lexeme("����"))                   return (T_3X4);
            if (lexeme("����"))                   return (T_3X5);
            if (lexeme("���Ԃ낭"))                 return (T_3X6);
            if (lexeme("����Ђ�"))                 return (T_3X7);
            if (lexeme("���񂵂�"))                 return (T_3X7);
            if (lexeme("�����"))                   return (T_3X8);
            if (lexeme("����"))                   return (T_3X9);
            if (lexeme("��������"))                 return (T_4X1);
            if (lexeme("���ɂ�"))                   return (T_4X2);
            if (lexeme("������"))                   return (T_4X3);
            if (lexeme("����"))                     return (T_4X4);
            if (lexeme("����"))                     return (T_4X5);
            if (lexeme("���낭"))                   return (T_4X6);
            if (lexeme("���Ђ�"))                   return (T_4X7);
            if (lexeme("������"))                   return (T_4X7);
            if (lexeme("����"))                     return (T_4X8);
            if (lexeme("����"))                     return (T_4X8);
            if (lexeme("����"))                     return (T_4X9);
            if (lexeme("��������"))                 return (T_5X1);
            if (lexeme("����"))                     return (T_5X2);
            if (lexeme("������"))                   return (T_5X3);
            if (lexeme("����"))                     return (T_5X4);
            if (lexeme("����"))                     return (T_5X5);
            if (lexeme("���낭"))                   return (T_5X6);
            if (lexeme("���Ђ�"))                   return (T_5X7);
            if (lexeme("������"))                   return (T_5X7);
            //if (lexeme("����"))                     return (T_5X8);   // T_GO T_HA
            if (lexeme("������"))                   return (T_5X9);
            if (lexeme("�낭������"))               return (T_6X1);
            //if (lexeme("�낭��"))                   return (T_6X2);   // T_ROKU T_NI
            if (lexeme("�낭����"))                 return (T_6X3);
            if (lexeme("�낭��"))                   return (T_6X4);
            if (lexeme("�낭��"))                   return (T_6X5);
            if (lexeme("�낭�낭"))                 return (T_6X6);
            if (lexeme("�낭�Ђ�"))                 return (T_6X7);
            if (lexeme("�낭����"))                 return (T_6X7);
            //if (lexeme("�낭��"))                   return (T_6X8);   // T_ROKU T_HA
            if (lexeme("�낭�͂�"))                 return (T_6X8);
            if (lexeme("�����"))                   return (T_6X9);
            if (lexeme("�Ђ�������"))               return (T_7X1);
            //if (lexeme("�Ђ���"))                   return (T_7X2);   // T_NANA T_NI
            if (lexeme("�Ђ�����"))                 return (T_7X3);
            if (lexeme("�Ђ���"))                   return (T_7X4);
            if (lexeme("�Ђ���"))                   return (T_7X5);
            if (lexeme("�Ђ��낭"))                 return (T_7X6);
            if (lexeme("�Ђ��Ђ�"))                 return (T_7X7);
            //if (lexeme("�Ђ���"))                   return (T_7X8);   // T_NANA T_HA
            if (lexeme("�Ђ��͂�"))                 return (T_7X8);
            if (lexeme("�Ђ���"))                   return (T_7X9);
            if (lexeme("����������"))               return (T_7X1);
            //if (lexeme("������"))                   return (T_7X2);   // T_NANA T_NI
            if (lexeme("��������"))                 return (T_7X3);
            if (lexeme("������"))                   return (T_7X4);
            if (lexeme("������"))                   return (T_7X5);
            if (lexeme("�����낭"))                 return (T_7X6);
            if (lexeme("��������"))                 return (T_7X7);
            //if (lexeme("������"))                   return (T_7X8);   // T_NANA T_HA
            if (lexeme("�����͂�"))                 return (T_7X8);
            if (lexeme("������"))                   return (T_7X9);
            if (lexeme("�͂�������"))               return (T_8X1);
            //if (lexeme("�͂���"))                   return (T_8X2);   // T_HACHI T_NI
            if (lexeme("�͂�����"))                 return (T_8X3);
            if (lexeme("�͂�����"))                 return (T_8X3);
            if (lexeme("�͂���"))                   return (T_8X4);
            if (lexeme("�͂���"))                   return (T_8X5);
            if (lexeme("�͂��낭"))                 return (T_8X6);
            if (lexeme("�͂��Ђ�"))                 return (T_8X7);
            if (lexeme("�͂�����"))                 return (T_8X7);
            if (lexeme("�͂���"))                   return (T_8X8);
            if (lexeme("�͂���"))                   return (T_8X9);
            if (lexeme("��������"))                 return (T_9X1);
            //if (lexeme("����"))                     return (T_9X2);   // T_KYUU T_NI
            if (lexeme("������"))                   return (T_9X3);
            if (lexeme("����"))                     return (T_9X4);
            if (lexeme("����"))                     return (T_9X5);
            if (lexeme("���낭"))                   return (T_9X6);
            if (lexeme("���Ђ�"))                   return (T_9X7);
            if (lexeme("������"))                   return (T_9X7);
            //if (lexeme("����"))                     return (T_9X8);   // T_KYUU T_HA
            if (lexeme("����"))                     return (T_9X9);

            if (!lexeme("�ł͂�", false) && lexeme("�ł�")) return (T_HA);
            if (!lexeme("�ɂ���", false) && lexeme("�ɂ�")) return (T_NI);

            // �Œ���v�@�B
            // �y��������z�s���~���ɕ��ёւ��Ă����B
            if (lexeme("�p"))                           return (T_R_PAREN);
            if (lexeme("�o"))                           return (T_L_PAREN);
            if (lexeme("������"))                       return (T_TAN);
            if (lexeme("������"))                       return (T_SIN);
            if (lexeme("������"))                       return (T_COS);
            if (lexeme("��������"))                     return (T_ATAN);
            if (lexeme("��������"))                     return (T_ASIN);
            if (lexeme("������������"))                 return (T_ATAN);
            if (lexeme("������������"))                 return (T_ASIN);
            if (lexeme("������������"))                 return (T_ACOS);
            if (lexeme("��������"))                     return (T_ACOS);
            if (lexeme("�n"))                           return (T_R_PAREN);
            if (lexeme("�m"))                           return (T_L_PAREN);
            if (lexeme("�H"))                           return (T_PERIOD);
            if (lexeme("��"))                           return (T_HA);
            if (lexeme("�D"))                           return (T_PERIOD);
            if (lexeme("�|"))                           return (T_MINUS);
            if (lexeme("�C"))                           return (T_COMMA);
            if (lexeme("�{"))                           return (T_TASU);
            if (lexeme("�j"))                           return (T_R_PAREN);
            if (lexeme("�i"))                           return (T_L_PAREN);
            if (lexeme("��"))                           return (T_PERCENT);
            if (lexeme("�I"))                           return (T_PERIOD);
            if (lexeme("��"))                           return (T_ZERO);
            if (lexeme("��"))                           return (T_ROKU);
            if (lexeme("���m�L"))                       return (T_ASOUGI);
            if (lexeme("�"))                           return (T_SEN);
            if (lexeme("�ߗR��"))                       return (T_NAYUTA);
            if (lexeme("�ߗR��"))                       return (T_NAYUTA);
            if (lexeme("��"))                           return (T_SAI);
            if (lexeme("��"))                           return (T_YON);
            if (lexeme("��"))                           return (T_JOU2);
            if (lexeme("�S"))                           return (T_HYAKU);
            if (lexeme("��"))                           return (T_KYUU);
            if (lexeme("���ʑ吔"))                     return (T_MURYOUTAISUU);
            if (lexeme("��"))                           return (T_KAN);
            if (lexeme("��"))                           return (T_NANA);
            if (lexeme("�a"))                           return (T_KOU);
            if (lexeme("��"))                           return (T_SEI);
            if (lexeme("��"))                           return (T_GOKU);
            if (lexeme("�E"))                           return (T_JUU);
            if (lexeme("�P�͍�"))                       return (T_GOUGASHA);
            if (lexeme("��"))                           return (T_NI);
            if (lexeme("��"))                           return (T_ICHI);
            if (lexeme("��"))                           return (T_GAI);
            if (lexeme("�l"))                           return (T_YON);
            if (lexeme("�Q"))                           return (T_SAN);
            if (lexeme("��"))                           return (T_SEN);
            if (lexeme("�\"))                           return (T_JUU);
            if (lexeme("�Z"))                           return (T_ROKU);
            if (lexeme("��"))                           return (T_HACHI);
            if (lexeme("��"))                           return (T_CHOU);
            if (lexeme("��"))                           return (T_OKU);
            if (lexeme("��"))                           return (T_GO);
            if (lexeme("��"))                           return (T_SEN);
            if (lexeme("��"))                           return (T_KEI);
            if (lexeme("��"))                           return (T_GO);
            if (lexeme("��"))                           return (T_NI);
            if (lexeme("��"))                           return (T_KYUU);
            if (lexeme("�s�v�c"))                     return (T_FUKASHIGI);
            if (lexeme("�O"))                           return (T_SAN);
            if (lexeme("��"))                           return (T_MAN);
            if (lexeme("��"))                           return (T_NANA);
            if (lexeme("��"))                           return (T_ICHI);
            if (lexeme("�E"))                           return (T_DOT);
            if (lexeme("����邱�Ƃ�"))                 return (T_WARU);
            if (lexeme("���Ђ����Ƃ�"))                 return (T_HIKU);
            if (lexeme("��"))                           return (T_WO1);
            if (lexeme("����"))                       return (T_WARUTO);
            if (lexeme("����"))                       return (T_WARUTO);
            if (lexeme("����"))                       return (T_WARUTO);
            if (lexeme("���"))                         return (T_WARUTO);
            if (lexeme("���Ƃ���"))                   return (T_WARUTO);
            if (lexeme("���Ƃ�"))                     return (T_WARUTO);
            if (lexeme("����"))                       return (T_WARUTO);
            if (lexeme("��邱�Ƃ�"))                   return (T_WARU);
            if (lexeme("���"))                         return (T_WARU);
            if (lexeme("���Ȃ���"))                   return (T_WARUTO);
            if (lexeme("��肴��"))                     return (T_WARIZAN);
            if (lexeme("���"))                         return (T_WATTE);
            if (lexeme("�����"))                     return (T_WARARERU);
            if (lexeme("�����"))                       return (T_WATTE);
            if (lexeme("�������"))                     return (T_WARUTO);
            if (lexeme("������Ƃ���"))                 return (T_WARUTO);
            if (lexeme("������Ƃ�"))                   return (T_WARUTO);
            if (lexeme("�����"))                       return (T_WATTA);
            if (lexeme("�킩��"))                       return (T_WAKARAN);
            if (lexeme("�킩��܂���"))                 return (T_WAKARAN);
            if (lexeme("�킩��܂�"))                   return (T_WAKARAN);
            if (lexeme("�킩���Ƃ�"))                 return (T_WAKARAN);
            if (lexeme("�킩���"))                     return (T_WAKARAN);
            if (lexeme("�킩��Ȃ���["))               return (T_WAKARAN);
            if (lexeme("�킩��Ȃ��您"))               return (T_WAKARAN);
            if (lexeme("�킩��Ȃ���"))                 return (T_WAKARAN);
            if (lexeme("�킩��Ȃ�"))                   return (T_WAKARAN);
            if (lexeme("��"))                           return (T_WA);
            if (lexeme("���"))                         return (T_ROKU);
            if (lexeme("�낮"))                         return (T_LOG);
            if (lexeme("�낭"))                         return (T_ROKU);
            if (lexeme("�ꂢ"))                         return (T_ZERO);
            if (lexeme("��[��"))                       return (T_ROOT);
            if (lexeme("�邤��"))                       return (T_ROOT);
            if (lexeme("����ۂ�"))                     return (T_RIPPOU);
            if (lexeme("��["))                         return (T_KA);
            if (lexeme("���"))                         return (T_YON);
            if (lexeme("�悰��"))                       return (T_COS);
            if (lexeme("�您"))                         return (T_KA);
            if (lexeme("�悧"))                         return (T_KA);
            if (lexeme("�悤"))                         return (T_KA);
            if (lexeme("�患"))                         return (T_KA);
            if (lexeme("��"))                           return (T_KA);
            if (lexeme("����"))                       return (T_SURUTO);
            if (lexeme("����"))                       return (T_SURUTO);
            if (lexeme("����"))                       return (T_SURUTO);
            if (lexeme("���"))                         return (T_SURUTO);
            if (lexeme("����"))                       return (T_SURUTO);
            if (lexeme("���Ȃ���"))                   return (T_SURUTO);
            if (lexeme("��˂�"))                       return (T_KA);
            if (lexeme("���"))                         return (T_KA);
            if (lexeme("����Ă݂��"))                 return (T_SURUTO);
            if (lexeme("����Ă݂��"))                 return (T_SURUTO);
            if (lexeme("����Ă݂�"))                   return (T_SURUTO);
            if (lexeme("����Ă݂��"))                 return (T_SURUTO);
            if (lexeme("����Ă݂�Ƃ�"))               return (T_SURUTO);
            if (lexeme("����Ă݂��"))                 return (T_SURUTO);
            if (lexeme("����Ă݂�"))                   return (T_SURUTO);
            if (lexeme("����Ă݂���"))                 return (T_SURUTO);
            if (lexeme("����Ă݂��Ƃ�"))               return (T_SURUTO);
            if (lexeme("�����"))                       return (T_SHITE);
            if (lexeme("�������"))                     return (T_SURUTO);
            if (lexeme("������Ƃ�"))                   return (T_SURUTO);
            if (lexeme("�����"))                       return (T_SHITA);
            if (lexeme("��"))                           return (T_KA);
            if (lexeme("����"))                         return (T_MONO);
            if (lexeme("���Ƃ߂��"))                   return (T_SURUTO);
            if (lexeme("���Ƃ߂�"))                     return (T_SURUTO);
            if (lexeme("���Ƃ߂Ȃ���"))                 return (T_SURUTO);
            if (lexeme("���Ƃ߂�"))                     return (T_SHITE);
            if (lexeme("���Ƃ߂���"))                   return (T_SURUTO);
            if (lexeme("���Ƃ߂�"))                     return (T_SHITA);
            if (lexeme("���Ƃ�"))                       return (T_SHITE);
            if (lexeme("��"))                           return (T_WO1);
            if (lexeme("�ނ�傤��������"))             return (T_MURYOUTAISUU);
            if (lexeme("�݂��"))                       return (T_ALL);
            if (lexeme("�݂��"))                       return (T_MIRUTO);
            if (lexeme("�݂��"))                       return (T_MIRUTO);
            if (lexeme("�܂�"))                         return (T_MAN);
            if (lexeme("�܂��Ȃ�����Ƃ���"))           return (T_HIKUTO);
            if (lexeme("�܂��Ȃ�����Ƃ�"))             return (T_HIKUTO);
            if (lexeme("�܂��Ȃ������"))               return (T_HIKUTO);
            if (lexeme("�܂��Ȃ������"))               return (T_HIKUTO);
            if (lexeme("�܂��Ȃ�����"))                 return (T_HIKUTO);
            if (lexeme("�܂��Ȃ����Ȃ���"))             return (T_HIKUTO);
            if (lexeme("�܂��Ȃ�����"))                 return (T_HIITE);
            if (lexeme("�܂��Ȃ�������"))               return (T_HIKUTO);
            if (lexeme("�܂��Ȃ������Ƃ�"))             return (T_HIKUTO);
            if (lexeme("�܂��Ȃ�����"))                 return (T_HIITA);
            if (lexeme("�܂��Ȃ���"))                   return (T_HIITE);
            if (lexeme("�܂��Ȃ������"))               return (T_HIKARERU);
            if (lexeme("�܂��Ȃ�"))                     return (T_MINUS);
            if (lexeme("�ւ��ق�"))                     return (T_HEIHOU);
            if (lexeme("��"))                           return (T_NI);
            if (lexeme("�Ղ炷����Ƃ���"))             return (T_TASUTO);
            if (lexeme("�Ղ炷����Ƃ�"))               return (T_TASUTO);
            if (lexeme("�Ղ炷�����"))                 return (T_TASUTO);
            if (lexeme("�Ղ炷�����"))                 return (T_TASUTO);
            if (lexeme("�Ղ炷����"))                   return (T_TASUTO);
            if (lexeme("�Ղ炷���Ȃ���"))               return (T_TASUTO);
            if (lexeme("�Ղ炷����"))                   return (T_TASHITE);
            if (lexeme("�Ղ炷������"))                 return (T_TASUTO);
            if (lexeme("�Ղ炷�����Ƃ�"))               return (T_TASUTO);
            if (lexeme("�Ղ炷����"))                   return (T_TASHITA);
            if (lexeme("�Ղ炷��"))                     return (T_TASHITE);
            if (lexeme("�Ղ炷�����"))                 return (T_TASARERU);
            if (lexeme("�Ղ炷"))                       return (T_PLUS);
            if (lexeme("�Ԃ��"))                       return (T_BUNNO);
            if (lexeme("�ӂ�����"))                     return (T_FUKASHIGI);
            if (lexeme("�҂Ⴍ"))                       return (T_HYAKU);
            if (lexeme("�тႭ"))                       return (T_HYAKU);
            if (lexeme("�ЂႭ"))                       return (T_HYAKU);
            if (lexeme("�Ђ�"))                         return (T_NANA);
            if (lexeme("�Ђ���"))                       return (T_HIKUTO);
            if (lexeme("�Ђ���"))                       return (T_HIKUTO);
            if (lexeme("�Ђ���"))                       return (T_HIKUTO);
            if (lexeme("�Ђ�"))                         return (T_HIKUTO);
            if (lexeme("�Ђ��Ƃ���"))                   return (T_HIKUTO);
            if (lexeme("�Ђ��Ƃ�"))                     return (T_HIKUTO);
            if (lexeme("�Ђ���"))                       return (T_HIKUTO);
            if (lexeme("�Ђ����Ƃ�"))                   return (T_HIKU);
            if (lexeme("�Ђ�"))                         return (T_HIKU);
            if (lexeme("�Ђ��Ȃ���"))                   return (T_HIKUTO);
            if (lexeme("�Ђ�����"))                     return (T_HIKIZAN);
            if (lexeme("�Ђ�"))                         return (T_HIITE);
            if (lexeme("�Ђ����"))                     return (T_HIKARERU);
            if (lexeme("�Ђ���"))                       return (T_HIITE);
            if (lexeme("�Ђ�����"))                     return (T_HIKUTO);
            if (lexeme("�Ђ����Ƃ���"))                 return (T_HIKUTO);
            if (lexeme("�Ђ����Ƃ�"))                   return (T_HIKUTO);
            if (lexeme("�Ђ���"))                       return (T_HIITA);
            if (lexeme("�ρ[�����"))                   return (T_PERCENT);
            if (lexeme("�ρ["))                         return (T_PERCENT);
            if (lexeme("�ς�"))                         return (T_PI);
            if (lexeme("�΂�"))                         return (T_BAI);
            if (lexeme("�͂�"))                         return (T_HACHI);
            if (lexeme("�͂�"))                         return (T_HACHI);
            if (lexeme("�͂��ςڂ�����E���񂺂��"))   return (T_TANH);
            if (lexeme("�͂��ςڂ�����E���񂶂����")) return (T_TANH);
            if (lexeme("�͂��ςڂ�����E������"))       return (T_SINH);
            if (lexeme("�͂��ςڂ�����E��������"))     return (T_COSH);
            if (lexeme("�͂��ςڂ�������񂺂��"))     return (T_TANH);
            if (lexeme("�͂��ςڂ�������񂶂����"))   return (T_TANH);
            if (lexeme("�͂��ςڂ����������"))         return (T_SINH);
            if (lexeme("�͂��ςڂ������������"))       return (T_COSH);
            if (lexeme("��"))                           return (T_HA);
            if (lexeme("��"))                           return (T_NO1);
            if (lexeme("�˂�"))                         return (T_KA);
            if (lexeme("�˂��҂�����"))                 return (T_E);
            if (lexeme("��"))                           return (T_KA);
            if (lexeme("�Ɂ["))                         return (T_NI);
            if (lexeme("�ɂ������Ƃ�"))                 return (T_TASU);
            if (lexeme("�ɂ��킦�邱�Ƃ�"))             return (T_TASU);
            if (lexeme("�ɂ����邱�Ƃ�"))               return (T_KAKERU);
            if (lexeme("�ɂ�"))                         return (T_NI);
            if (lexeme("��"))                           return (T_NI);
            if (lexeme("�ȁ[��"))                     return (T_IKURA);
            if (lexeme("�ȁ[��"))                       return (T_IKURA);
            if (lexeme("�Ȃ�ڂɂȂ��ł��傤"))       return (T_IKURA);
            if (lexeme("�Ȃ�ڂɂȂ��"))             return (T_IKURA);
            if (lexeme("�Ȃ�ڂɂȂ�̂ł��傤"))       return (T_IKURA);
            if (lexeme("�Ȃ�ڂɂȂ�ł��傤"))         return (T_IKURA);
            if (lexeme("�Ȃ�ڂɂȂ�"))                 return (T_IKURA);
            if (lexeme("�Ȃ�ڂɂȂ�܂�"))             return (T_IKURA);
            if (lexeme("�Ȃ�ڂȂ�"))                   return (T_IKURA);
            if (lexeme("�Ȃ�ڂȂ�"))                   return (T_IKURA);
            if (lexeme("�Ȃ�ڂł��傤"))               return (T_IKURA);
            if (lexeme("�Ȃ�ڂ��낤"))                 return (T_IKURA);
            if (lexeme("�Ȃ��"))                       return (T_IKURA);
            if (lexeme("�Ȃ�ɂȂ��ł��傤"))         return (T_IKURA);
            if (lexeme("�Ȃ�ɂȂ��"))               return (T_IKURA);
            if (lexeme("�Ȃ�ɂȂ�̂ł��傤"))         return (T_IKURA);
            if (lexeme("�Ȃ�ɂȂ�ł��傤"))           return (T_IKURA);
            if (lexeme("�Ȃ�ɂȂ�"))                   return (T_IKURA);
            if (lexeme("�Ȃ�ɂȂ�܂�"))               return (T_IKURA);
            if (lexeme("�Ȃ�Ȃ�ł�"))                 return (T_IKURA);
            if (lexeme("�Ȃ�Ȃ�ł�"))                 return (T_IKURA);
            if (lexeme("�Ȃ�Ȃ�ł��傤"))             return (T_IKURA);
            if (lexeme("�Ȃ�Ȃ񂾂낤"))               return (T_IKURA);
            if (lexeme("�Ȃ�Ȃ�"))                     return (T_IKURA);
            if (lexeme("�Ȃ�Ȃ̂ł�"))                 return (T_IKURA);
            if (lexeme("�Ȃ�Ȃ̂ł�"))                 return (T_IKURA);
            if (lexeme("�Ȃ�Ȃ̂ł��傤"))             return (T_IKURA);
            if (lexeme("�Ȃ�Ȃ̂��낤"))               return (T_IKURA);
            if (lexeme("�Ȃ�Ȃ�"))                     return (T_IKURA);
            if (lexeme("�Ȃ�ł�"))                     return (T_IKURA);
            if (lexeme("�Ȃ�ł�"))                     return (T_IKURA);
            if (lexeme("�Ȃ�ł��傤"))                 return (T_IKURA);
            if (lexeme("�Ȃ񂾂낤"))                   return (T_IKURA);
            if (lexeme("�Ȃ�"))                       return (T_IKURA);
            if (lexeme("�Ȃ�"))                         return (T_IKURA);
            if (lexeme("�Ȃ䂽"))                       return (T_NAYUTA);
            if (lexeme("�ȂɂɂȂ��ł�"))             return (T_IKURA);
            if (lexeme("�ȂɂɂȂ�̂ł�"))             return (T_IKURA);
            if (lexeme("�ȂɂɂȂ�ł��傤"))           return (T_IKURA);
            if (lexeme("�ȂɂɂȂ�"))                   return (T_IKURA);
            if (lexeme("�Ȃɂł�"))                     return (T_IKURA);
            if (lexeme("�Ȃɂł�"))                     return (T_IKURA);
            if (lexeme("�Ȃ�"))                         return (T_IKURA);
            if (lexeme("�Ȃ�"))                         return (T_NANA);
            if (lexeme("�Ȃ�����"))                     return (T_SURUTO);
            if (lexeme("�Ȃ�����"))                     return (T_SURUTO);
            if (lexeme("�Ȃ���"))                       return (T_SURUTO);
            if (lexeme("�Ȃ����Ƃ���"))                 return (T_SURUTO);
            if (lexeme("�Ȃ����Ƃ�"))                   return (T_SURUTO);
            if (lexeme("�Ȃ�����"))                     return (T_SURUTO);
            if (lexeme("�Ȃ����Ȃ���"))                 return (T_SURUTO);
            if (lexeme("�Ȃ����Ă݂��"))               return (T_SURUTO);
            if (lexeme("�Ȃ����Ă݂��"))               return (T_SURUTO);
            if (lexeme("�Ȃ����Ă݂�"))                 return (T_SURUTO);
            if (lexeme("�Ȃ����Ă݂Ȃ���"))             return (T_SURUTO);
            if (lexeme("�Ȃ�����"))                     return (T_SURUTO);
            if (lexeme("�Ȃ�������"))                   return (T_SURUTO);
            if (lexeme("�Ȃ�����"))                     return (T_SHITA);
            if (lexeme("�Ȃ���"))                       return (T_IKURA);
            if (lexeme("�ǂ�Ȃ����ɂȂ��ł��傤"))   return (T_IKURA);
            if (lexeme("�ǂ�Ȃ����ɂȂ�̂ł��傤"))   return (T_IKURA);
            if (lexeme("�ǂ�Ȃ����ɂȂ�ł��傤"))     return (T_IKURA);
            if (lexeme("�ǂ�Ȃ����ɂȂ�"))             return (T_IKURA);
            if (lexeme("�ǂ�Ȃ����ɂȂ�܂�"))         return (T_IKURA);
            if (lexeme("�ǂȂ��Ȃ��ł��傤"))         return (T_IKURA);
            if (lexeme("�ǂȂ��Ȃ�̂ł��傤"))         return (T_IKURA);
            if (lexeme("�ǂȂ��Ȃ�"))                   return (T_IKURA);
            if (lexeme("�ǂȂ��ł�"))                   return (T_IKURA);
            if (lexeme("�ǂȂ��ł�"))                   return (T_IKURA);
            if (lexeme("�ǂȂ��ł��傤"))               return (T_IKURA);
            if (lexeme("�ǂȂ�"))                       return (T_IKURA);
            if (lexeme("�ǂ��Ȃ�ł�"))                 return (T_IKURA);
            if (lexeme("�ǂ��Ȃ�ł�"))                 return (T_IKURA);
            if (lexeme("�ǂ��Ȃ�ł��傤"))             return (T_IKURA);
            if (lexeme("�ǂ��Ȃ�"))                     return (T_IKURA);
            if (lexeme("�ǂ��Ȃ�"))                     return (T_IKURA);
            if (lexeme("�ǂ��Ȃ�܂�"))                 return (T_IKURA);
            if (lexeme("�ǂ��Ȃ̂ł�"))                 return (T_IKURA);
            if (lexeme("�ǂ��Ȃ̂ł�"))                 return (T_IKURA);
            if (lexeme("�ǂ��Ȃ̂ł��傤"))             return (T_IKURA);
            if (lexeme("�ǂ��Ȃ�"))                     return (T_IKURA);
            if (lexeme("�ǂ�"))                         return (T_IKURA);
            if (lexeme("��"))                           return (T_DO);
            if (lexeme("�ƂˁA"))                       return (T_TO1);
            if (lexeme("�ƂȁA"))                       return (T_TO1);
            if (lexeme("�Ƃ�������"))                   return (T_R_PAREN);
            if (lexeme("��"))                           return (T_TO1);
            if (lexeme("�ł�˂�"))                     return (T_KA);
            if (lexeme("��"))                           return (T_DE);
            if (lexeme("�Ă�"))                         return (T_DOT);
            if (lexeme("����"))                         return (T_HA);
            if (lexeme("���傤"))                       return (T_CHOU);
            if (lexeme("���񂺂��"))                   return (T_TAN);
            if (lexeme("���񂶂����"))                 return (T_TAN);
            if (lexeme("������"))                       return (T_TASUTO);
            if (lexeme("������"))                       return (T_TASUTO);
            if (lexeme("������"))                       return (T_TASUTO);
            if (lexeme("����"))                         return (T_TASUTO);
            if (lexeme("�����Ƃ�"))                     return (T_TASUTO);
            if (lexeme("������"))                       return (T_TASUTO);
            if (lexeme("�������Ƃ�"))                   return (T_TASU);
            if (lexeme("����"))                         return (T_TASU);
            if (lexeme("�����Ȃ���"))                   return (T_TASUTO);
            if (lexeme("������"))                       return (T_TASHITE);
            if (lexeme("��������"))                     return (T_TASUTO);
            if (lexeme("�������Ƃ���"))                 return (T_TASUTO);
            if (lexeme("�������Ƃ�"))                   return (T_TASUTO);
            if (lexeme("������"))                       return (T_TASHITA);
            if (lexeme("��������"))                     return (T_TASHIZAN);
            if (lexeme("�������킹���"))               return (T_TASUTO);
            if (lexeme("�������킹���"))               return (T_TASUTO);
            if (lexeme("�������킹��"))                 return (T_TASUTO);
            if (lexeme("�������킹���"))               return (T_TASUTO);
            if (lexeme("�������킹��Ƃ�"))             return (T_TASUTO);
            if (lexeme("�������킹���"))               return (T_TASUTO);
            if (lexeme("�������킹��"))                 return (T_TASU);
            if (lexeme("�������킹��"))                 return (T_TASUTO);
            if (lexeme("�������킹�Ȃ���"))             return (T_TASUTO);
            if (lexeme("�������킹��"))                 return (T_TASHITE);
            if (lexeme("�������킹���Ƃ���"))           return (T_TASUTO);
            if (lexeme("�������킹���Ƃ�"))             return (T_TASUTO);
            if (lexeme("�������킹��"))                 return (T_TASHITA);
            if (lexeme("�������킹"))                   return (T_TASUTO);
            if (lexeme("�������킷"))                   return (T_TASU);
            if (lexeme("�������킳���"))               return (T_TASARERU);
            if (lexeme("����"))                         return (T_TASHITE);
            if (lexeme("�������"))                     return (T_TASARERU);
            if (lexeme("�����������񂷂�"))             return (T_LOG);
            if (lexeme("��������"))                     return (T_LOG);
            if (lexeme("�����"))                       return (T_SONO);
            if (lexeme("���ꂪ����"))                   return (T_SONO);
            if (lexeme("���ꂪ��"))                     return (T_SORE);
            if (lexeme("����"))                         return (T_SORE);
            if (lexeme("�����"))                       return (T_SORE);
            if (lexeme("���̂Ƃ�"))                     return (T_SURUTO);
            if (lexeme("���̂�����"))                   return (T_KOTAE);
            if (lexeme("����"))                         return (T_SONO);
            if (lexeme("�������傭����悰��"))         return (T_COSH);
            if (lexeme("�������傭���񂽂񂺂��"))     return (T_TANH);
            if (lexeme("�������傭���񂽂񂶂����"))   return (T_TANH);
            if (lexeme("�������傭���񂹂�����"))       return (T_TANH);
            if (lexeme("�������傭���񂹂�����"))       return (T_SINH);
            if (lexeme("�������傭���񂳂���"))         return (T_SINH);
            if (lexeme("�������傭���񂱂�����"))       return (T_COSH);
            if (lexeme("������"))                       return (T_SORE);
            if (lexeme("�����"))                       return (T_ALL);
            if (lexeme("����"))                         return (T_SEN);
            if (lexeme("����"))                         return (T_ZERO);
            if (lexeme("����������"))                   return (T_ZETTAICHI);
            if (lexeme("����"))                         return (T_SEN);
            if (lexeme("����"))                         return (T_SURUTO);
            if (lexeme("����"))                         return (T_SEKI);
            if (lexeme("������"))                       return (T_SURUTO);
            if (lexeme("��������"))                     return (T_TAN);
            if (lexeme("��������"))                     return (T_SIN);
            if (lexeme("����"))                         return (T_SEI);
            if (lexeme("�����"))                       return (T_SURUTO);
            if (lexeme("����Ƃ���"))                   return (T_SURUTO);
            if (lexeme("����Ƃ�"))                     return (T_SURUTO);
            if (lexeme("�����"))                       return (T_SURUTO);
            if (lexeme("���ׂ�"))                       return (T_ALL);
            if (lexeme("����["))                       return (T_JOU1);
            if (lexeme("���傴��"))                     return (T_WARIZAN);
            if (lexeme("���傤�悤��������"))           return (T_LOG10);
            if (lexeme("���傤��"))                     return (T_AMARI);
            if (lexeme("���傤����"))                   return (T_KAKEZAN);
            if (lexeme("���傤"))                       return (T_JOU1);
            if (lexeme("����"))                         return (T_JO);
            if (lexeme("����["))                       return (T_JUU);
            if (lexeme("�����"))                       return (T_JUU);
            if (lexeme("���イ"))                       return (T_JUU);
            if (lexeme("����"))                         return (T_JUU);
            if (lexeme("�����傤"))                     return (T_HEIHOU);
            if (lexeme("���[��"))                       return (T_SURUTO);
            if (lexeme("���["))                         return (T_YON);
            if (lexeme("�����"))                       return (T_SURUTO);
            if (lexeme("�����"))                       return (T_SURUTO);
            if (lexeme("����"))                         return (T_SURUTO);
            if (lexeme("�����"))                       return (T_WAKARAN);
            if (lexeme("����Ȃ�"))                     return (T_WAKARAN);
            if (lexeme("����[���["))                   return (T_SHOUSUU);
            if (lexeme("����[����"))                   return (T_SHOUSUU);
            if (lexeme("���傤���["))                   return (T_SHOUSUU);
            if (lexeme("���傤����"))                   return (T_SHOUSUU);
            if (lexeme("���傤"))                       return (T_SHOU);
            if (lexeme("���Ȃ���"))                     return (T_SURUTO);
            if (lexeme("���Ă�"))                       return (T_SHITE);
            if (lexeme("���Ă�"))                       return (T_SHITE);
            if (lexeme("���Ă݂��"))                   return (T_SURUTO);
            if (lexeme("���Ă݂��"))                   return (T_SURUTO);
            if (lexeme("���Ă݂�"))                     return (T_SURUTO);
            if (lexeme("���Ă݂��"))                   return (T_SURUTO);
            if (lexeme("���Ă݂�Ƃ���"))               return (T_SURUTO);
            if (lexeme("���Ă݂�Ƃ�"))                 return (T_SURUTO);
            if (lexeme("���Ă݂��"))                   return (T_SURUTO);
            if (lexeme("���Ă݂Ȃ���"))                 return (T_SURUTO);
            if (lexeme("����"))                         return (T_SHITE);
            if (lexeme("����"))                         return (T_NANA);
            if (lexeme("������"))                       return (T_SURUTO);
            if (lexeme("�����Ƃ���"))                   return (T_SURUTO);
            if (lexeme("�����Ƃ�"))                     return (T_SURUTO);
            if (lexeme("����"))                         return (T_SHITA);
            if (lexeme("�����񂽂������̂Ă�"))         return (T_E);
            if (lexeme("�����񂽂�����"))               return (T_LOG);
            if (lexeme("���������񂷂�"))               return (T_EXP);
            if (lexeme("������"))                       return (T_EXP);
            if (lexeme("������"))                       return (T_SURUTO);
            if (lexeme("����"))                         return (T_YON);
            if (lexeme("����"))                         return (T_YON);
            if (lexeme("��"))                           return (T_YON);
            if (lexeme("����"))                         return (T_SAN);
            if (lexeme("�����Ђ���"))                   return (T_HIKUTO);
            if (lexeme("�����Ђ���"))                   return (T_HIKUTO);
            if (lexeme("�����Ђ�"))                     return (T_HIKUTO);
            if (lexeme("�����Ђ��Ƃ�"))                 return (T_HIKUTO);
            if (lexeme("�����Ђ��Ȃ���"))               return (T_HIKUTO);
            if (lexeme("�����Ђ�"))                     return (T_HIITE);
            if (lexeme("�����Ђ����"))                 return (T_HIKARERU);
            if (lexeme("�����Ђ���"))                   return (T_HIITE);
            if (lexeme("�����Ђ�����"))                 return (T_HIKUTO);
            if (lexeme("�����Ђ����Ƃ���"))             return (T_HIKUTO);
            if (lexeme("�����Ђ����Ƃ�"))               return (T_HIKUTO);
            if (lexeme("�����Ђ���"))                   return (T_HIITA);
            if (lexeme("������"))                       return (T_SIN);
            if (lexeme("����"))                         return (T_SAI);
            if (lexeme("��"))                           return (T_SA);
            if (lexeme("����"))                         return (T_GOKU);
            if (lexeme("����������"))                   return (T_GOUGASHA);
            if (lexeme("����"))                         return (T_GO);
            if (lexeme("��"))                           return (T_GO);
            if (lexeme("�����"))                       return (T_SONO);
            if (lexeme("����"))                         return (T_SORE);
            if (lexeme("�����"))                       return (T_SORE);
            if (lexeme("���̂Ƃ�"))                     return (T_SURUTO);
            if (lexeme("���������"))                   return (T_OSHIETE);
            if (lexeme("���������"))                   return (T_OSHIETE);
            if (lexeme("��������"))                     return (T_OSHIETE);
            if (lexeme("��������"))                     return (T_OSHIETE);
            if (lexeme("�������Ȃ���"))                 return (T_OSHIETE);
            if (lexeme("�������Ă���邩��"))           return (T_OSHIETE);
            if (lexeme("�������Ă���邩"))             return (T_OSHIETE);
            if (lexeme("�������Ă����"))               return (T_OSHIETE);
            if (lexeme("�������Ă����"))               return (T_OSHIETE);
            if (lexeme("�������Ă����"))               return (T_OSHIETE);
            if (lexeme("�������Ă���"))                 return (T_OSHIETE);
            if (lexeme("�������Ă���������"))           return (T_OSHIETE);
            if (lexeme("�������Ă�������"))             return (T_OSHIETE);
            if (lexeme("������"))                       return (T_KOTAE);
            if (lexeme("��������"))                     return (T_COS);
            if (lexeme("����"))                         return (T_KOU);
            if (lexeme("������"))                       return (T_SORE);
            if (lexeme("���񂴂�"))                     return (T_HIKIZAN);
            if (lexeme("��������"))                     return (T_KEISAN);
            if (lexeme("����"))                         return (T_KEI);
            if (lexeme("���킦���"))                   return (T_TASUTO);
            if (lexeme("���킦���"))                   return (T_TASUTO);
            if (lexeme("���킦��"))                     return (T_TASUTO);
            if (lexeme("���킦��Ƃ���"))               return (T_TASUTO);
            if (lexeme("���킦��Ƃ�"))                 return (T_TASUTO);
            if (lexeme("���킦���"))                   return (T_TASUTO);
            if (lexeme("���킦�邱�Ƃ�"))               return (T_TASU);
            if (lexeme("���킦��"))                     return (T_TASU);
            if (lexeme("���킦����"))                 return (T_TASARERU);
            if (lexeme("���킦�Ȃ���"))                 return (T_TASUTO);
            if (lexeme("���킦��"))                     return (T_TASHITE);
            if (lexeme("���킦����"))                   return (T_TASUTO);
            if (lexeme("���킦���Ƃ���"))               return (T_TASUTO);
            if (lexeme("���킦���Ƃ�"))                 return (T_TASUTO);
            if (lexeme("���킦��"))                     return (T_TASHITA);
            if (lexeme("�����"))                       return (T_KA);
            if (lexeme("�����["))                     return (T_KA);
            if (lexeme("����患"))                     return (T_KA);
            if (lexeme("�����"))                       return (T_KA);
            if (lexeme("�����"))                       return (T_KA);
            if (lexeme("����"))                         return (T_KA);
            if (lexeme("����������["))                 return (T_KA);
            if (lexeme("���������患"))                 return (T_KA);
            if (lexeme("����������"))                   return (T_KA);
            if (lexeme("��������"))                     return (T_KA);
            if (lexeme("��"))                           return (T_KYUU);
            if (lexeme("���Ⴍ�悰��"))                 return (T_ACOS);
            if (lexeme("���Ⴍ���񂺂��"))             return (T_ATAN);
            if (lexeme("���Ⴍ���񂶂����"))           return (T_ATAN);
            if (lexeme("���Ⴍ��������"))               return (T_ATAN);
            if (lexeme("���Ⴍ��������"))               return (T_ASIN);
            if (lexeme("���Ⴍ����"))                   return (T_GYAKUSUU);
            if (lexeme("���Ⴍ������"))                 return (T_ASIN);
            if (lexeme("���Ⴍ��������"))               return (T_ACOS);
            if (lexeme("����["))                       return (T_KYUU);
            if (lexeme("���イ"))                       return (T_KYUU);
            if (lexeme("����"))                         return (T_GAI);
            if (lexeme("��"))                           return (T_HA);
            if (lexeme("���["))                         return (T_KA);
            if (lexeme("����"))                         return (T_KAN);
            if (lexeme("����"))                         return (T_KARA);
            if (lexeme("���ˁ["))                       return (T_KA);
            if (lexeme("���˂�"))                       return (T_KA);
            if (lexeme("���˂�"))                       return (T_KA);
            if (lexeme("����"))                         return (T_KA);
            if (lexeme("���ȁ["))                       return (T_KA);
            if (lexeme("���Ȃ�"))                       return (T_KA);
            if (lexeme("���Ȃ�"))                       return (T_KA);
            if (lexeme("����"))                         return (T_KA);
            if (lexeme("���������Ƃ���"))               return (T_R_PAREN);
            if (lexeme("���������Ƃ���"))               return (T_R_PAREN);
            if (lexeme("�������Ƃ���"))                 return (T_R_PAREN);
            if (lexeme("�������Ƃ���"))                 return (T_R_PAREN);
            if (lexeme("�������Ƃ�"))                   return (T_R_PAREN);
            if (lexeme("������"))                       return (T_L_PAREN);
            if (lexeme("����"))                         return (T_MONO);
            if (lexeme("������"))                       return (T_TASHIZAN);
            if (lexeme("�������"))                     return (T_KAKERUTO);
            if (lexeme("�������"))                     return (T_KAKERUTO);
            if (lexeme("������"))                       return (T_KAKERUTO);
            if (lexeme("�������"))                     return (T_KAKERUTO);
            if (lexeme("������Ƃ���"))                 return (T_KAKERUTO);
            if (lexeme("������Ƃ�"))                   return (T_KAKERUTO);
            if (lexeme("�������"))                     return (T_KAKERUTO);
            if (lexeme("�����邱�Ƃ�"))                 return (T_KAKERU);
            if (lexeme("������"))                       return (T_KAKERU);
            if (lexeme("��������"))                   return (T_KAKERARERU);
            if (lexeme("�����Ȃ���"))                   return (T_KAKERUTO);
            if (lexeme("������"))                       return (T_KAKETE);
            if (lexeme("��������"))                     return (T_KAKERUTO);
            if (lexeme("�������Ƃ���"))                 return (T_KAKERUTO);
            if (lexeme("�������Ƃ�"))                   return (T_KAKERUTO);
            if (lexeme("������"))                       return (T_KAKETA);
            if (lexeme("��������"))                     return (T_KAKEZAN);
            if (lexeme("�������킹���"))               return (T_KAKERUTO);
            if (lexeme("�������킹���"))               return (T_KAKERUTO);
            if (lexeme("�������킹��"))                 return (T_KAKERUTO);
            if (lexeme("�������킹���"))               return (T_KAKERUTO);
            if (lexeme("�������킹��Ƃ�"))             return (T_KAKERUTO);
            if (lexeme("�������킹���"))               return (T_KAKERUTO);
            if (lexeme("�������킹��"))                 return (T_KAKERU);
            if (lexeme("�������킹��"))                 return (T_KAKERUTO);
            if (lexeme("�������킹�Ȃ���"))             return (T_KAKERUTO);
            if (lexeme("�������킹��"))                 return (T_KAKERUTO);
            if (lexeme("�������킹���Ƃ���"))           return (T_KAKERUTO);
            if (lexeme("�������킹���Ƃ�"))             return (T_KAKERUTO);
            if (lexeme("�������킹��"))                 return (T_KAKETA);
            if (lexeme("�������킹"))                   return (T_KAKERUTO);
            if (lexeme("�������킷�Ƃ���"))             return (T_KAKERUTO);
            if (lexeme("�������킷�Ƃ�"))               return (T_KAKERUTO);
            if (lexeme("�������킷"))                   return (T_KAKERU);
            if (lexeme("�������킵���Ƃ���"))           return (T_KAKERUTO);
            if (lexeme("�������킵���Ƃ�"))             return (T_KAKERUTO);
            if (lexeme("�������킳���"))               return (T_KAKERARERU);
            if (lexeme("����"))                         return (T_KAKERU);
            if (lexeme("����"))                         return (T_KA);
            if (lexeme("����"))                         return (T_KA);
            if (lexeme("����"))                         return (T_KA);
            if (lexeme("��"))                           return (T_KA);
            if (lexeme("���˂�����"))                   return (T_ONEGAI);
            if (lexeme("���˂�����˂�"))               return (T_ONEGAI);
            if (lexeme("���˂�������"))                 return (T_ONEGAI);
            if (lexeme("���˂������܂���"))             return (T_ONEGAI);
            if (lexeme("���˂������܂���"))             return (T_ONEGAI);
            if (lexeme("���˂������܂�"))               return (T_ONEGAI);
            if (lexeme("���˂����������܂���"))         return (T_ONEGAI);
            if (lexeme("���˂����������܂���"))         return (T_ONEGAI);
            if (lexeme("���˂����������܂�"))           return (T_ONEGAI);
            if (lexeme("���˂���"))                     return (T_ONEGAI);
            if (lexeme("���������"))                   return (T_OSHIETE);
            if (lexeme("���������"))                   return (T_OSHIETE);
            if (lexeme("��������"))                     return (T_OSHIETE);
            if (lexeme("��������"))                     return (T_OSHIETE);
            if (lexeme("�������Ȃ���"))                 return (T_OSHIETE);
            if (lexeme("�������Ă�"))                   return (T_OSHIETE);
            if (lexeme("�������Ă�"))                   return (T_OSHIETE);
            if (lexeme("�������Ă�"))                   return (T_OSHIETE);
            if (lexeme("�������Ăق����˂�"))           return (T_OSHIETE);
            if (lexeme("�������Ă��傤������"))         return (T_OSHIETE);
            if (lexeme("�������Ă��傤����"))           return (T_OSHIETE);
            if (lexeme("�������Ă���邩��"))           return (T_OSHIETE);
            if (lexeme("�������Ă���邩"))             return (T_OSHIETE);
            if (lexeme("�������Ă����"))               return (T_OSHIETE);
            if (lexeme("�������Ă���"))                 return (T_OSHIETE);
            if (lexeme("�������Ă���������"))           return (T_OSHIETE);
            if (lexeme("�������Ă�������"))             return (T_OSHIETE);
            if (lexeme("��������"))                     return (T_OSHIETE);
            if (lexeme("����"))                         return (T_OKU);
            if (lexeme("���񂵂イ���"))               return (T_PI);
            if (lexeme("���["))                         return (T_E);
            if (lexeme("����"))                         return (T_ICHI);
            if (lexeme("����"))                         return (T_ICHI);
            if (lexeme("������ɂȂ�ł��傤"))         return (T_IKURA);
            if (lexeme("������ɂȂ�"))                 return (T_IKURA);
            if (lexeme("������ɂȂ�܂�"))             return (T_IKURA);
            if (lexeme("������ł�"))                   return (T_IKURA);
            if (lexeme("������ł�"))                   return (T_IKURA);
            if (lexeme("������"))                       return (T_IKURA);
            if (lexeme("������"))                     return (T_IKURA);
            if (lexeme("�����ɂȂ�ł��傤"))         return (T_IKURA);
            if (lexeme("�����ɂȂ�"))                 return (T_IKURA);
            if (lexeme("�����ɂȂ�܂�"))             return (T_IKURA);
            if (lexeme("�����Ȃ�ł�"))               return (T_IKURA);
            if (lexeme("�����Ȃ�ł�"))               return (T_IKURA);
            if (lexeme("�����Ȃ�ł��傤"))           return (T_IKURA);
            if (lexeme("�����Ȃ�"))                   return (T_IKURA);
            if (lexeme("�����Ȃ̂ł�"))               return (T_IKURA);
            if (lexeme("�����Ȃ̂ł�"))               return (T_IKURA);
            if (lexeme("�����Ȃ̂ł��傤"))           return (T_IKURA);
            if (lexeme("�����Ȃ�"))                   return (T_IKURA);
            if (lexeme("������"))                       return (T_IKURA);
            if (lexeme("���[�����񂺂��"))             return (T_ATAN);
            if (lexeme("���[�����񂶂����"))           return (T_ATAN);
            if (lexeme("���[��������"))                 return (T_ASIN);
            if (lexeme("���[����������"))               return (T_ACOS);
            if (lexeme("���܂�"))                       return (T_AMARI);
            if (lexeme("��������"))                     return (T_ASOUGI);
            if (lexeme("�l"))                           return (T_R_PAREN);
            if (lexeme("�k"))                           return (T_L_PAREN);
            if (lexeme("�z"))                           return (T_R_PAREN);
            if (lexeme("�y"))                           return (T_L_PAREN);
            if (lexeme("�x"))                           return (T_R_PAREN);
            if (lexeme("�w"))                           return (T_L_PAREN);
            if (lexeme("�v"))                           return (T_R_PAREN);
            if (lexeme("�u"))                           return (T_L_PAREN);
            if (lexeme("�t"))                           return (T_R_PAREN);
            if (lexeme("�s"))                           return (T_L_PAREN);
            if (lexeme("�r"))                           return (T_R_PAREN);
            if (lexeme("�q"))                           return (T_L_PAREN);
            if (lexeme("�Z"))                           return (T_ZERO);
            if (lexeme("�B"))                           return (T_PERIOD);
            if (lexeme("�A"))                           return (T_COMMA);
            if (lexeme("��"))                           return (T_PERIOD);
            if (lexeme("��"))                           return (T_PERIOD);
            if (lexeme("��"))                           return (T_PERIOD);
            if (lexeme("��"))                           return (T_PERIOD);
            if (lexeme("��"))                           return (T_WARU);
            if (lexeme("�~"))                           return (T_KAKERU);
            if (lexeme("}"))                            return (T_R_PAREN);
            if (lexeme("{"))                            return (T_L_PAREN);
            if (lexeme("tanh"))                         return (T_TANH);
            if (lexeme("tan"))                          return (T_TAN);
            if (lexeme("sinh"))                         return (T_SINH);
            if (lexeme("sin"))                          return (T_SIN);
            if (lexeme("log10"))                        return (T_LOG10);
            if (lexeme("log"))                          return (T_LOG);
            if (lexeme("ln"))                           return (T_LOG);
            if (lexeme("fabs"))                         return (T_ZETTAICHI);
            if (lexeme("exp"))                          return (T_EXP);
            if (lexeme("cosh"))                         return (T_COSH);
            if (lexeme("cos"))                          return (T_COS);
            if (lexeme("atan"))                         return (T_ATAN);
            if (lexeme("asin"))                         return (T_ASIN);
            if (lexeme("arctan"))                       return (T_ATAN);
            if (lexeme("arcsin"))                       return (T_ASIN);
            if (lexeme("arccos"))                       return (T_ACOS);
            if (lexeme("acos"))                         return (T_ACOS);
            if (lexeme("abs"))                          return (T_ZETTAICHI);
            if (lexeme("]"))                            return (T_R_PAREN);
            if (lexeme("["))                            return (T_L_PAREN);
            if (lexeme("?"))                            return (T_PERIOD);
            if (lexeme("=="))                           return (T_HA);
            if (lexeme("="))                            return (T_HA);
            if (lexeme("/"))                            return (T_WARU);
            if (lexeme("."))                            return (T_PERIOD);
            if (lexeme("-"))                            return (T_MINUS);
            if (lexeme(","))                            return (T_COMMA);
            if (lexeme("+"))                            return (T_TASU);
            if (lexeme("*"))                            return (T_KAKERU);
            if (lexeme(")"))                            return (T_R_PAREN);
            if (lexeme("("))                            return (T_L_PAREN);
            if (lexeme("%"))                            return (T_PERCENT);
            if (lexeme("!"))                            return (T_PERIOD);
            // �y�����܂Łz�s���~���ɕ��ёւ��Ă����B

            char ch = getch();
            if (ch == EOF)
                return commit_token(eof);

            std::string str;
            do
            {
                str += ch;
                ch = getch();
            } while (ch != EOF);

            message(str + "���킩��܂���B");

            return commit_token(eof);
        }   // get_token

    protected:
        char getch()
        {
            char c;

            if (m_buff.size() == m_index)
            {
                if (m_current == m_end)
                {
                    c = EOF;
                }
                else
                {
                    c = *m_current++;
                    m_buff.push_back(c);
                    m_index++;
                }
            }
            else
            {
                c = m_buff[m_index++];
            }

            return c;
        }   // getch

        void ungetch()
        {
            assert(m_index);
            if (0 < m_index)
                m_index--;
        }

        void commit()
        {
            using namespace std;
            if (m_index)
            {
                m_buff.erase(m_buff.begin(), m_buff.begin() + m_index);
                m_index = 0;
            }
        }

        Token commit_token(Token token)
        {
            commit();
            return token;
        }

        bool lexeme(const char* s, bool do_commit = true)
        {
            char c;
            std::size_t n = 1;
            while (*s)
            {
                c = getch();
                if (c == *s)
                {
                    s++; n++;
                }
                else if (c == EOF)
                {
                    return false;
                }
                else
                {
                    assert(n <= m_index);
                    m_index -= n;
                    return false;   // closed
                }
            }
            if (*s == 0)
            {
                if (do_commit)
                {
                    commit();
                }
                else
                {
                    --n;
                    assert(n <= m_index);
                    m_index -= n;
                }
                return true;    // closed
            }
            return false;   // closed
        }   // lexeme

        void newline()
        {
            ++location();
        }

    protected:
        parser_site_type&       m_parser_site;
        std::size_t             m_index;
        std::vector<char>       m_buff;

        iterator_type           m_begin;
        iterator_type           m_current;
        iterator_type           m_end;

        CH_Location& location()
        {
            return m_parser_site.location();
        }

        const CH_Location& location() const
        {
            return m_parser_site.location();
        }

        void message(const std::string& str)
        {
            m_parser_site.message(str);
        }

        // T_PERIOD, T_COMMA�𐮗����āAT_MIRUTO, T_ALL���폜����B
        // T_SONO���uT_SORE, T_NO1�v�ɒu��������B
        void resynth1(std::vector<info_type>& infos)
        {
            std::vector<info_type> newinfos;
            std::vector<info_type>::iterator it = infos.begin();
            std::vector<info_type>::iterator end = infos.end();

            while (it->get_token() == T_PERIOD ||
                   it->get_token() == T_COMMA ||
                   it->get_token() == T_HA)
            {
                ++it;
            }

            for (; it != end; ++it)
            {
                switch (it->get_token())
                {
                case T_SONO:
                    it->set_token(T_SORE);
                    newinfos.push_back(*it);
                    it->set_token(T_NO1);
                    newinfos.push_back(*it);
                    break;

                case T_MIRUTO:
                case T_ALL:
                    continue;

                case T_PERIOD:
                    newinfos.push_back(*it);
                    do
                    {
                        ++it;
                    } while (it->get_token() == T_PERIOD);
                    --it;
                    continue;

                case T_COMMA:
                    newinfos.push_back(*it);
                    do
                    {
                        ++it;
                    } while (it->get_token() == T_COMMA);
                    --it;
                    continue;

                default:
                    newinfos.push_back(*it);
                }
            }
            infos = newinfos;
        }

        // ���ʂ̑Ή����C������B
        bool resynth2(std::vector<info_type>& infos)
        {
            std::vector<info_type> newinfos;
            std::vector<info_type>::iterator it = infos.begin();
            std::vector<info_type>::iterator end = infos.end();
            info_type info;
            int nest = 0;
            for (; it != end; ++it)
            {
                switch (it->get_token())
                {
                case T_L_PAREN:
                    ++nest;
                    assert(nest >= 0);
                    break;

                case T_R_PAREN:
                    --nest;
                    if (nest < 0)
                    {
                        message("�������̑Ή������������ł��B");
                        return false;
                    }
                    break;

                case T_HA:
                case T_PERIOD:
                case eof:
                    info = *it;
                    for (info.set_token(T_R_PAREN); nest > 0; --nest)
                        newinfos.push_back(info);
                    break;

                default:
                    break;
                }
                newinfos.push_back(*it);
            }
            infos = newinfos;
            return true;
        }

        // �u�́v��T_NO1, T_NO2, ..., T_NO7�ɕ��ނ���B
        // ���̒P�ʁu���v��T_JOU2�ɕ��ނ���B
        // T_NO2: �u��v�u���v�u�����v�u���傤�v�̒��O�́u�́v�B
        // T_NO3: �u���̉��{�v�́u�́v�B�u���́i...�j�v�́u�́v�B
        // T_NO4: �u��������v�u��������v�u�������v�Ȃǂ̒��O�́u�́v�B
        // T_NO5: �u�̂��������v�u�̂�����邩���v�u�̂����邩���v�́u�́v
        // T_NO6: �u�̉���v�u�̕����v�u�̗����v�́u�́v�B
        // T_NO7: �O�p�֐��u�����v�u�]���v�u���ځv�̒��O�́u�́v�B
        // T_NO8: �u�̉��ρ[����Ɓv�́u�́v�B
        // T_NO9: �u�̗]��v�́u�́v�B
        // T_NO1: ����ȊO�B
        void resynth3(std::vector<info_type>& infos)
        {
            std::vector<info_type> newinfos;
            std::vector<info_type> subinfos;
            std::vector<info_type>::iterator it = infos.begin();
            std::vector<info_type>::iterator end = infos.end();
            int nest;
            std::size_t no1 = std::string::npos;    // position of T_NO1
            std::size_t jou1 = std::string::npos;   // position of T_JOU1
            std::size_t wo1 = std::string::npos;    // position of T_WO1
            bool flag = false;
            for (; it != end; ++it)
            {
                switch (it->get_token())
                {
                case T_L_PAREN:
                    newinfos.push_back(*it);
                    subinfos.clear();
                    ++it;
                    for (nest = 1; it != end; ++it)
                    {
                        switch (it->get_token())
                        {
                        case T_L_PAREN:
                            ++nest;
                            subinfos.push_back(*it);
                            continue;

                        case T_R_PAREN:
                            --nest;
                            if (nest == 0)
                                break;
                            subinfos.push_back(*it);
                            continue;

                        default:
                            subinfos.push_back(*it);
                            continue;
                        }
                        break;
                    }
                    //show_tokens(subinfos.begin(), subinfos.end());
                    resynth2(subinfos);
                    //show_tokens(subinfos.begin(), subinfos.end());
                    newinfos.insert(newinfos.end(), subinfos.begin(), subinfos.end());
                    break;

                case T_NO1:
                    no1 = std::distance(infos.begin(), it);
                    jou1 = std::string::npos;
                    flag = false;
                    break;

                case T_WO1:
                    wo1 = std::distance(infos.begin(), it);
                    jou1 = std::string::npos;
                    flag = false;
                    break;

                case T_WA:
                case T_SEKI:
                case T_SA:
                case T_SHOU:
                    if (no1 != std::string::npos)
                    {
                        (newinfos.begin() + no1)->set_token(T_NO2);
                        no1 = std::string::npos;
                    }
                    jou1 = std::string::npos;
                    break;

                case T_BAI:
                    if (no1 != std::string::npos)
                    {
                        (newinfos.begin() + no1)->set_token(T_NO3);
                        no1 = std::string::npos;
                        if (jou1 != std::string::npos &&
                            newinfos[jou1].get_token() == T_JOU1)
                        {
                            newinfos[jou1].set_token(T_JOU2);
                            jou1 = std::string::npos;
                        }
                    }
                    flag = false;
                    break;

                case T_TASHIZAN:
                case T_HIKIZAN:
                case T_KAKEZAN:
                case T_WARIZAN:
                case T_KEISAN:
                case T_KOTAE:
                    if (no1 != std::string::npos)
                    {
                        (newinfos.begin() + no1)->set_token(T_NO4);
                        no1 = std::string::npos;
                    }
                    jou1 = std::string::npos;
                    flag = false;
                    break;

                case T_TASU:
                case T_KAKERU:
                case T_HIKU:
                case T_WARU:
                case T_TASARERU:
                case T_KAKERARERU:
                case T_HIKARERU:
                case T_WARARERU:
                    if (no1 != std::string::npos)
                    {
                        (newinfos.begin() + no1)->set_token(T_NO5);
                        no1 = std::string::npos;
                    }
                    jou1 = std::string::npos;
                    flag = false;
                    break;

                case T_PERCENT:
                    if (no1 != std::string::npos)
                    {
                        (newinfos.begin() + no1)->set_token(T_NO8);
                        no1 = std::string::npos;
                    }
                    flag = false;
                    break;

                case T_JOU1:
                    if (no1 != std::string::npos)
                    {
                        if (jou1 != std::string::npos &&
                            newinfos[jou1].get_token() == T_JOU1)
                        {
                            newinfos[jou1].set_token(T_JOU2);
                        }
                        (newinfos.begin() + no1)->set_token(T_NO6);
                        jou1 = std::distance(infos.begin(), it);
                    }
                    else if (wo1 != std::string::npos)
                    {
                        if (jou1 != std::string::npos &&
                            newinfos[jou1].get_token() == T_JOU1)
                        {
                            newinfos[jou1].set_token(T_JOU2);
                        }
                        wo1 = std::string::npos;
                        jou1 = std::distance(infos.begin(), it);
                    }
                    else if (jou1 != std::string::npos)
                    {
                        (newinfos.begin() + jou1)->set_token(T_JOU2);
                    }
                    else if (!flag)
                    {
                        it->set_token(T_JOU2);
                    }
                    flag = false;
                    break;

                case T_HEIHOU:
                case T_RIPPOU:
                    if (no1 != std::string::npos)
                    {
                        (newinfos.begin() + no1)->set_token(T_NO6);
                        no1 = std::string::npos;
                    }
                    jou1 = std::string::npos;
                    flag = false;
                    break;

                case T_AMARI:
                    if (no1 != std::string::npos)
                    {
                        (newinfos.begin() + no1)->set_token(T_NO9);
                        no1 = std::string::npos;
                    }
                    jou1 = std::string::npos;
                    flag = false;
                    break;

                case T_SIN:
                case T_COS:
                case T_TAN:
                case T_ASIN:
                case T_ACOS:
                case T_ATAN:
                case T_SINH:
                case T_COSH:
                case T_TANH:
                case T_ROOT:
                case T_ZETTAICHI:
                case T_GYAKUSUU:
                    if (no1 != std::string::npos)
                    {
                        (newinfos.begin() + no1)->set_token(T_NO7);
                    }
                    jou1 = std::string::npos;
                    flag = true;
                    break;

                default:
                    break;
                }
                newinfos.push_back(*it);
            }
            infos = newinfos;
        }

        // �����_�̌�̐����ɂ��邵������B
        void resynth4(std::vector<info_type>& infos)
        {
            std::vector<info_type>::iterator it = infos.begin();
            std::vector<info_type>::iterator end = infos.end();
            bool dot = false;
            for (; it != end; ++it)
            {
                switch (it->get_token())
                {
                case T_DOT:
                    dot = true;
                    break;

                case T_ZERO:    if (dot) it->set_token(T_DIGIT0); break;
                case T_ICHI:    if (dot) it->set_token(T_DIGIT1); break;
                case T_NI:      if (dot) it->set_token(T_DIGIT2); break;
                case T_SAN:     if (dot) it->set_token(T_DIGIT3); break;
                case T_YON:     if (dot) it->set_token(T_DIGIT4); break;
                case T_GO:      if (dot) it->set_token(T_DIGIT5); break;
                case T_ROKU:    if (dot) it->set_token(T_DIGIT6); break;
                case T_NANA:    if (dot) it->set_token(T_DIGIT7); break;
                case T_HACHI:   if (dot) it->set_token(T_DIGIT8); break;
                case T_KYUU:    if (dot) it->set_token(T_DIGIT9); break;

                default:
                    dot = false;
                    break;
                }
            }
        }

        // �u�Ɓv��T_TO1, T_TO2�ɕ��ނ���B
        // T_TO2: �u�Ȃ�ƂȂ�Ԃ�̂ȂɁv�́u�Ɓv�B
        // T_TO1: ����ȊO�B
        void resynth5(std::vector<info_type>& infos)
        {
            std::vector<info_type>::iterator it = infos.begin();
            std::vector<info_type>::iterator end = infos.end();
            std::vector<info_type>::iterator it_save = end;
            int flag = 0;
            for (; it != end; ++it)
            {
                switch (it->get_token())
                {
                case T_TO1:
                    if (flag != 2)
                        it_save = it;
                    flag = 0;
                    break;

                case T_ICHI:
                case T_NI:
                case T_SAN:
                case T_YON:
                case T_GO:
                case T_ROKU:
                case T_NANA:
                case T_HACHI:
                case T_KYUU:
                case T_JUU:
                case T_HYAKU:
                case T_SEN:
                case T_MAN:
                case T_OKU:
                case T_CHOU:
                case T_KEI:
                    if (flag == 1)
                        flag = 2;
                    break;

                case T_BUNNO:
                    if (it_save != end)
                    {
                        it_save->set_token(T_TO2);
                        it_save = end;
                    }
                    flag = 1;
                    break;

                default:
                    it_save = end;
                    flag = 0;
                    break;
                }
            }
        }

        // ����̌��̌��T_COMMA���폜����B
        void resynth6(std::vector<info_type>& infos)
        {
            std::vector<info_type> newinfos;
            std::vector<info_type>::iterator it = infos.begin();
            std::vector<info_type>::iterator end = infos.end();
            std::vector<info_type>::iterator it2;
            for (; it != end; ++it)
            {
                switch (it->get_token())
                {
                case T_AMARI:
                case T_BAI:
                case T_BUNNO:
                case T_DE:
                case T_HA:
                case T_HEIHOU:
                case T_HIITE:
                case T_HIKIZAN:
                case T_HIKU:
                case T_HIKUTO:
                case T_IKURA:
                case T_KA:
                case T_KAKERU:
                case T_KAKERUTO:
                case T_KAKETE:
                case T_KAKEZAN:
                case T_KARA:
                case T_KEISAN:
                case T_KOTAE:
                case T_L_PAREN:
                case T_MINUS:
                case T_NO1:
                case T_NO2:
                case T_NO3:
                case T_NO4:
                case T_NO5:
                case T_NO6:
                case T_NO7:
                case T_NO8:
                case T_NO9:
                case T_OSHIETE:
                case T_PERCENT:
                case T_PLUS:
                case T_SA:
                case T_SEKI:
                case T_SHITA:
                case T_SHITE:
                case T_SHOU:
                case T_SURUTO:
                case T_TASHITE:
                case T_TASHIZAN:
                case T_TASU:
                case T_TASUTO:
                case T_TO1:
                case T_WA:
                case T_WARIZAN:
                case T_WARU:
                case T_WARUTO:
                case T_WATTE:
                case T_WO1:
                case T_WO2:
                //case T_NI:    // NOTE: T_NI�́A������������Ȃ��B
                    it2 = it;
                    it2++;
                    if (it2->get_token() == T_COMMA)
                    {
                        newinfos.push_back(*it);
                        it = it2;
                        continue;
                    }
                    break;

                default:
                    break;
                }
                newinfos.push_back(*it);
            }
            infos = newinfos;
        }

        // �u�v�Z���v�u�����v�Ȃǂ́u���v���C������B
        void resynth7(std::vector<info_type>& infos)
        {
            std::vector<info_type>::iterator it = infos.begin();
            std::vector<info_type>::iterator end = infos.end();
            bool flag = false;
            for (; it != end; ++it)
            {
                switch (it->get_token())
                {
                case T_TASHIZAN:
                case T_HIKIZAN:
                case T_KAKEZAN:
                case T_WARIZAN:
                case T_KEISAN:
                case T_PLUS:
                case T_MINUS:
                case T_WO1:
                case T_WO2:
                    flag = true;
                    break;

                case T_YON:
                    if (flag)
                    {
                        flag = false;
                        it->set_token(T_SHITE);
                    }
                    break;

                default:
                    flag = false;
                    break;
                }
            }
        }

        // �u���v��T_WO1��T_WO2�ɕ��ނ���B
        // T_WO2: �u��������v�u��������v�Ȃǂ̒���́u���v�B
        // T_WO1: ���̂ق��́u���v�B
        void resynth8(std::vector<info_type>& infos)
        {
            std::vector<info_type>::iterator it = infos.begin();
            std::vector<info_type>::iterator end = infos.end();
            bool flag = false;
            for (; it != end; ++it)
            {
                switch (it->get_token())
                {
                case T_TASHIZAN:
                case T_HIKIZAN:
                case T_KAKEZAN:
                case T_WARIZAN:
                case T_KEISAN:
                    flag = true;
                    break;

                case T_WO1:
                    if (flag)
                    {
                        flag = false;
                        it->set_token(T_WO2);
                    }
                    break;

                default:
                    flag = false;
                    break;
                }
            }
        }

        // �u���v�̒���Ɂu��������v�u��������v�Ȃǂ�����΁A
        // �u���v���u�́v�ɕς���B
        void resynth9(std::vector<info_type>& infos)
        {
            std::vector<info_type>::iterator it = infos.begin();
            std::vector<info_type>::iterator end = infos.end();
            std::vector<info_type>::iterator it_save = end;
            for (; it != end; ++it)
            {
                switch (it->get_token())
                {
                case T_TASHIZAN:
                case T_HIKIZAN:
                case T_KAKEZAN:
                case T_WARIZAN:
                case T_KEISAN:
                    if (it_save != end)
                    {
                        it_save->set_token(T_NO4);
                    }
                    break;

                case T_WO1:
                case T_WO2:
                    it_save = it;
                    break;

                default:
                    it_save = end;
                    break;
                }
            }
        }

        // �u�������B�v�u�Ђ����B�v�u�������B�v�u������B�v��
        // �u�B�v���u�A�v�ɕς���B
        // �u�������`�v�u�Ђ����`�v�u�������`�v�u������`�v��
        // �u��������`�v�u�Ђ�����`�v�u��������`�v�u�������`�v�ɕς���B
        void resynth10(std::vector<info_type>& infos)
        {
            std::vector<info_type>::iterator it = infos.begin();
            std::vector<info_type>::iterator end = infos.end();
            for (; it != end; ++it)
            {
                if ((it->get_token() == T_TASHITA ||
                     it->get_token() == T_HIITA ||
                     it->get_token() == T_KAKETA ||
                     it->get_token() == T_WATTA) &&
                    (it + 1)->get_token() == T_PERIOD)
                {
                    (it + 1)->set_token(T_COMMA);
                }
                if ((it + 1)->get_token() == T_COMMA ||
                    (it + 1)->get_token() == T_KOTAE ||
                    (it + 1)->get_token() == T_AMARI ||
                    (it + 1)->get_token() == T_SORE)
                {
                    if (it->get_token() == T_TASHITA)
                        it->set_token(T_TASUTO);
                    else if (it->get_token() == T_HIITA)
                        it->set_token(T_HIKUTO);
                    else if (it->get_token() == T_KAKETA)
                        it->set_token(T_KAKERUTO);
                    else if (it->get_token() == T_WATTA)
                        it->set_token(T_WARUTO);
                }
            }
        }

        // ����̌�咼�O�́u�A�v����菜���B
        void resynth11(std::vector<info_type>& infos)
        {
            std::vector<info_type> newinfos;
            std::vector<info_type>::iterator it = infos.begin();
            std::vector<info_type>::iterator end = infos.end();
            for (; it != end; ++it)
            {
                if (it->get_token() == T_COMMA)
                {
                    switch ((it + 1)->get_token())
                    {
                    case T_AMARI:
                    case T_BAI:
                    case T_DE:
                    case T_HIITA:
                    case T_HIITE:
                    case T_HIKIZAN:
                    case T_HIKU:
                    case T_HIKUTO:
                    case T_IKURA:
                    case T_KA:
                    case T_KAKERU:
                    case T_KAKERUTO:
                    case T_KAKETA:
                    case T_KAKETE:
                    case T_KAKEZAN:
                    case T_KARA:
                    case T_KEISAN:
                    case T_KOTAE:
                    case T_ONEGAI:
                    case T_OSHIETE:
                    case T_PERCENT:
                    case T_R_PAREN:
                    case T_SA:
                    case T_SEKI:
                    case T_SHITA:
                    case T_SHITE:
                    case T_SHOU:
                    case T_SURUTO:
                    case T_TASHITA:
                    case T_TASHITE:
                    case T_TASHIZAN:
                    case T_TASU:
                    case T_TASUTO:
                    case T_WA:
                    case T_WARIZAN:
                    case T_WARU:
                    case T_WARUTO:
                    case T_WATTA:
                    case T_WATTE:
                    case T_WO1:
                    case T_WO2:
                        ++it;
                        break;

                    default:
                        break;
                    }
                }
                newinfos.push_back(*it);
            }
            infos = newinfos;
        }

        // �u����̂����v���u����v�ɕϊ�����B
        void resynth12(std::vector<info_type>& infos)
        {
            std::vector<info_type> newinfos;
            std::vector<info_type>::iterator it = infos.begin();
            std::vector<info_type>::iterator end = infos.end();
            std::vector<info_type>::iterator it_save = end;
            info_type info;
            for (; it != end; ++it)
            {
                if (it->get_token() == T_SORE &&
                    (it + 1)->get_token() == T_NO4 &&
                    (it + 2)->get_token() == T_MONO)
                {
                    newinfos.push_back(*it);
                    ++it;
                    ++it;
                }
                else
                {
                    newinfos.push_back(*it);
                }
            }
            infos = newinfos;
        }

    private:
        // NOTE: Scanner is not copyable.
        Scanner(const Scanner<Iterator, ParserSite>&);
        Scanner& operator=(const Scanner<Iterator, ParserSite>&);
    }; // class Scanner<Iterator, ParserSite>
} // namespace Calc_H

#endif  // ndef HSCANNER_H_
