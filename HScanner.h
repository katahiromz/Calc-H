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

            resynth0(infos);
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
                resynth13(infos);
                resynth14(infos);
                resynth15(infos);
                resynth16(infos);
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
                       lexeme("�Ƃ��"))
                {
                    flag = true;
                }
                if (!flag)
                    break;
            }

            info.location() = location();

            if (space_count >= 2)
                return commit_token(T_COMMA);

            // arabic numeral
            std::string strNum;
            if (lexeme("0") || lexeme("1") || lexeme("2") || lexeme("3") || lexeme("4") ||
                lexeme("5") || lexeme("6") || lexeme("7") || lexeme("8") || lexeme("9") ||
                lexeme("�O") || lexeme("�P") || lexeme("�Q") || lexeme("�R") || lexeme("�S") ||
                lexeme("�T") || lexeme("�U") || lexeme("�V") || lexeme("�W") || lexeme("�X"))
            {
                bool has_dot = false;
                strNum = m_saved_str;
                while (lexeme("0") || lexeme("1") || lexeme("2") || lexeme("3") || lexeme("4") ||
                       lexeme("5") || lexeme("6") || lexeme("7") || lexeme("8") || lexeme("9") ||
                       lexeme("�O") || lexeme("�P") || lexeme("�Q") || lexeme("�R") || lexeme("�S") ||
                       lexeme("�T") || lexeme("�U") || lexeme("�V") || lexeme("�W") || lexeme("�X") ||
                       (!has_dot && (lexeme(".") || lexeme("�D"))))
                {
                    strNum += m_saved_str;
                    if (m_saved_str == "." || m_saved_str == "�D")
                    {
                        has_dot = true;
                    }
                }
                ChReplaceString(strNum, "�O", "0");
                ChReplaceString(strNum, "�P", "1");
                ChReplaceString(strNum, "�Q", "2");
                ChReplaceString(strNum, "�R", "3");
                ChReplaceString(strNum, "�S", "4");
                ChReplaceString(strNum, "�T", "5");
                ChReplaceString(strNum, "�U", "6");
                ChReplaceString(strNum, "�V", "7");
                ChReplaceString(strNum, "�W", "8");
                ChReplaceString(strNum, "�X", "9");
                ChReplaceString(strNum, "�D", ".");
                m_saved_str = strNum;
                return set_info(info, T_NUMBER);
            }

            if (lexeme("\n"))
            {
                newline();
                return set_info(info, T_PERIOD);
            }

            if (!lexeme("�ł͂�", false) && lexeme("�ł�")) return set_info(info, T_HA);
            if (!lexeme("�ɂ���", false) && lexeme("�ɂ�")) return set_info(info, T_NI);
            if (!lexeme("���Ȃ�", false) && lexeme("����")) return set_info(info, T_WO1);
            if (!lexeme("�ɂȂ�", false) && lexeme("�ɂ�")) return set_info(info, T_NI);
            if (!lexeme("�̂Ȃ�", false) && lexeme("�̂�")) return set_info(info, T_NO1);

            // �Œ���v�@�B
            // �y��������z�s���~���ɕ��ёւ��Ă����B
            if (lexeme("�`"))                               return set_info(info, T_KARA);
            if (lexeme("�p"))                               return set_info(info, T_R_PAREN);
            if (lexeme("�o"))                               return set_info(info, T_L_PAREN);
            if (lexeme("��������"))                         return set_info(info, T_TANH);
            if (lexeme("������"))                           return set_info(info, T_TAN);
            if (lexeme("��������"))                         return set_info(info, T_SINH);
            if (lexeme("������"))                           return set_info(info, T_SIN);
            if (lexeme("�������P�O"))                       return set_info(info, T_LOG10);
            if (lexeme("������"))                           return set_info(info, T_LOG);
            if (lexeme("����"))                             return set_info(info, T_LOG);
            if (lexeme("��������"))                         return set_info(info, T_ZETTAICHI);
            if (lexeme("������"))                           return set_info(info, T_EXP);
            if (lexeme("��������"))                         return set_info(info, T_COSH);
            if (lexeme("������"))                           return set_info(info, T_COS);
            if (lexeme("��������"))                         return set_info(info, T_ATAN);
            if (lexeme("��������"))                         return set_info(info, T_ASIN);
            if (lexeme("������������"))                     return set_info(info, T_ATAN);
            if (lexeme("������������"))                     return set_info(info, T_ASIN);
            if (lexeme("������������"))                     return set_info(info, T_ACOS);
            if (lexeme("��������"))                         return set_info(info, T_ACOS);
            if (lexeme("������"))                           return set_info(info, T_ZETTAICHI);
            if (lexeme("�n"))                               return set_info(info, T_R_PAREN);
            if (lexeme("�m"))                               return set_info(info, T_L_PAREN);
            if (lexeme("�H"))                               return set_info(info, T_PERIOD);
            if (lexeme("��"))                               return set_info(info, T_HA);
            if (lexeme("�D"))                               return set_info(info, T_PERIOD);
            if (lexeme("�|"))                               return set_info(info, T_MINUS);
            if (lexeme("�C"))                               return set_info(info, T_COMMA);
            if (lexeme("�{"))                               return set_info(info, T_TASU);
            if (lexeme("�j"))                               return set_info(info, T_R_PAREN);
            if (lexeme("�i"))                               return set_info(info, T_L_PAREN);
            if (lexeme("��"))                               return set_info(info, T_PERCENT);
            if (lexeme("�I"))                               return set_info(info, T_FACTORIAL);
            if (lexeme("��"))                               return set_info(info, T_ZERO);
            if (lexeme("��"))                               return set_info(info, T_ROKU);
            if (lexeme("���m�L"))                           return set_info(info, T_ASOUGI);
            if (lexeme("�"))                               return set_info(info, T_SEN);
            if (lexeme("�ߗR��"))                           return set_info(info, T_NAYUTA);
            if (lexeme("�ߗR��"))                           return set_info(info, T_NAYUTA);
            if (lexeme("��"))                               return set_info(info, T_SAI);
            if (lexeme("��"))                               return set_info(info, T_YON);
            if (lexeme("��"))                               return set_info(info, T_JOU2);
            if (lexeme("�S"))                               return set_info(info, T_HYAKU);
            if (lexeme("��"))                               return set_info(info, T_KYUU);
            if (lexeme("���ʑ吔"))                         return set_info(info, T_MURYOUTAISUU);
            if (lexeme("��"))                               return set_info(info, T_KAN);
            if (lexeme("��"))                               return set_info(info, T_NANA);
            if (lexeme("�a"))                               return set_info(info, T_KOU);
            if (lexeme("��"))                               return set_info(info, T_SEI);
            if (lexeme("��"))                               return set_info(info, T_GOKU);
            if (lexeme("�E"))                               return set_info(info, T_JUU);
            if (lexeme("�P�͍�"))                           return set_info(info, T_GOUGASHA);
            if (lexeme("��"))                               return set_info(info, T_NI);
            if (lexeme("��"))                               return set_info(info, T_HATA);
            if (lexeme("��"))                               return set_info(info, T_ICHI);
            if (lexeme("��"))                               return set_info(info, T_GAI);
            if (lexeme("�l"))                               return set_info(info, T_YON);
            if (lexeme("�Q"))                               return set_info(info, T_SAN);
            if (lexeme("��"))                               return set_info(info, T_SEN);
            if (lexeme("�\"))                               return set_info(info, T_JUU);
            if (lexeme("�Z"))                               return set_info(info, T_ROKU);
            if (lexeme("��"))                               return set_info(info, T_HACHI);
            if (lexeme("��"))                               return set_info(info, T_CHOU);
            if (lexeme("��"))                               return set_info(info, T_OKU);
            if (lexeme("��"))                               return set_info(info, T_GO);
            if (lexeme("��"))                               return set_info(info, T_SEN);
            if (lexeme("��"))                               return set_info(info, T_KEI);
            if (lexeme("��"))                               return set_info(info, T_GO);
            if (lexeme("��"))                               return set_info(info, T_NI);
            if (lexeme("��"))                               return set_info(info, T_KYUU);
            if (lexeme("�s�v�c"))                         return set_info(info, T_FUKASHIGI);
            if (lexeme("�O"))                               return set_info(info, T_SAN);
            if (lexeme("��"))                               return set_info(info, T_MAN);
            if (lexeme("��"))                               return set_info(info, T_NANA);
            if (lexeme("��"))                               return set_info(info, T_ICHI);
            if (lexeme("�E"))                               return set_info(info, T_DOT);
            if (lexeme("���"))                             return set_info(info, T_KANA);
            if (lexeme("����邱�Ƃ�"))                     return set_info(info, T_WARU);
            if (lexeme("���Ђ����Ƃ�"))                     return set_info(info, T_HIKU);
            if (lexeme("����"))                             return set_info(info, T_WO1);
            if (lexeme("��"))                               return set_info(info, T_WO1);
            if (lexeme("����"))                           return set_info(info, T_WARUTO);
            if (lexeme("����"))                           return set_info(info, T_WARUTO);
            if (lexeme("����"))                           return set_info(info, T_WARUTO);
            if (lexeme("���"))                             return set_info(info, T_WARUTO);
            if (lexeme("���Ƃ���"))                       return set_info(info, T_WARUTO);
            if (lexeme("���Ƃ�"))                         return set_info(info, T_WARUTO);
            if (lexeme("����"))                           return set_info(info, T_WARUTO);
            if (lexeme("��邱�Ƃ�"))                       return set_info(info, T_WARU);
            if (lexeme("���"))                             return set_info(info, T_WARU);
            if (lexeme("���Ȃ���"))                       return set_info(info, T_WARUTO);
            if (lexeme("��肴��"))                         return set_info(info, T_WARIZAN);
            if (lexeme("��肫����ł�"))                 return set_info(info, T_WARIKIRU);
            if (lexeme("��肫����ł��傤"))             return set_info(info, T_WARIKIRU);
            if (lexeme("��肫����"))                     return set_info(info, T_WARIKIRU);
            if (lexeme("��肫���̂ł�"))                 return set_info(info, T_WARIKIRU);
            if (lexeme("��肫���̂ł��傤"))             return set_info(info, T_WARIKIRU);
            if (lexeme("��肫���ł��傤"))               return set_info(info, T_WARIKIRU);
            if (lexeme("��肫���"))                       return set_info(info, T_WARIKIRU);
            if (lexeme("��肫��܂���"))                   return set_info(info, T_WARIKIRENAI);
            if (lexeme("��肫��܂�"))                     return set_info(info, T_WARIKIRU);
            if (lexeme("��肫��ւ��"))                   return set_info(info, T_WARIKIRENAI);
            if (lexeme("��肫��ւ�"))                     return set_info(info, T_WARIKIRENAI);
            if (lexeme("��肫��Ȃ���ł�"))               return set_info(info, T_WARIKIRENAI);
            if (lexeme("��肫��Ȃ���ł��傤"))           return set_info(info, T_WARIKIRENAI);
            if (lexeme("��肫��Ȃ��̂ł�"))               return set_info(info, T_WARIKIRENAI);
            if (lexeme("��肫��Ȃ��̂ł��傤"))           return set_info(info, T_WARIKIRENAI);
            if (lexeme("��肫��Ȃ��ł��傤"))             return set_info(info, T_WARIKIRENAI);
            if (lexeme("��肫��Ȃ�"))                     return set_info(info, T_WARIKIRENAI);
            if (lexeme("��肫���ł�"))                   return set_info(info, T_WARIKIRU);
            if (lexeme("��肫���ł��傤"))               return set_info(info, T_WARIKIRU);
            if (lexeme("��肫���"))                       return set_info(info, T_WARIKIRU);
            if (lexeme("��肫��̂ł�"))                   return set_info(info, T_WARIKIRU);
            if (lexeme("��肫��̂ł��傤"))               return set_info(info, T_WARIKIRU);
            if (lexeme("��肫���"))                       return set_info(info, T_WARIKIRU);
            if (lexeme("��肫��ł��傤"))                 return set_info(info, T_WARIKIRU);
            if (lexeme("��肫�邱�Ƃ��ł���̂ł��傤"))   return set_info(info, T_WARIKIRU);
            if (lexeme("��肫�邱�Ƃ��ł���ł��傤"))     return set_info(info, T_WARIKIRU);
            if (lexeme("��肫�邱�Ƃ��ł���"))             return set_info(info, T_WARIKIRU);
            if (lexeme("��肫�邱�Ƃ��ł��܂���"))         return set_info(info, T_WARIKIRENAI);
            if (lexeme("��肫�邱�Ƃ��ł��܂�"))           return set_info(info, T_WARIKIRU);
            if (lexeme("��肫�邱�Ƃ��ł��ւ��"))         return set_info(info, T_WARIKIRENAI);
            if (lexeme("��肫�邱�Ƃ��ł��ւ�"))           return set_info(info, T_WARIKIRENAI);
            if (lexeme("��肫�邱�Ƃ��ł��Ȃ���ł��傤")) return set_info(info, T_WARIKIRENAI);
            if (lexeme("��肫�邱�Ƃ��ł��Ȃ��̂ł��傤")) return set_info(info, T_WARIKIRENAI);
            if (lexeme("��肫�邱�Ƃ��ł��Ȃ�"))           return set_info(info, T_WARIKIRENAI);
            if (lexeme("��肫��"))                         return set_info(info, T_WARIKIRU);
            if (lexeme("���"))                             return set_info(info, T_WATTE);
            if (lexeme("�����"))                         return set_info(info, T_WARARERU);
            if (lexeme("����Ă݂�"))                       return set_info(info, T_WARUTO);
            if (lexeme("����Ă݂�"))                       return set_info(info, T_WARUTO);
            if (lexeme("����Ă݂�"))                       return set_info(info, T_WARUTO);
            if (lexeme("����Ă�"))                         return set_info(info, T_WARUTO);
            if (lexeme("�����"))                           return set_info(info, T_WATTE);
            if (lexeme("�������"))                         return set_info(info, T_WARUTO);
            if (lexeme("������Ƃ���"))                     return set_info(info, T_WARUTO);
            if (lexeme("������Ƃ�"))                       return set_info(info, T_WARUTO);
            if (lexeme("�����"))                           return set_info(info, T_WATTA);
            if (lexeme("�킩���"))                         return set_info(info, T_WAKARAN);
            if (lexeme("�킩��"))                           return set_info(info, T_WAKARAN);
            if (lexeme("�킩��܂���"))                     return set_info(info, T_WAKARAN);
            if (lexeme("�킩��܂�"))                       return set_info(info, T_WAKARAN);
            if (lexeme("�킩����"))                       return set_info(info, T_WAKARAN);
            if (lexeme("�킩����"))                       return set_info(info, T_WAKARAN);
            if (lexeme("�킩���"))                         return set_info(info, T_WAKARAN);
            if (lexeme("�킩��ւ��"))                     return set_info(info, T_WAKARAN);
            if (lexeme("�킩��ւ�"))                       return set_info(info, T_WAKARAN);
            if (lexeme("�킩��Ȃ���"))                     return set_info(info, T_WAKARAN);
            if (lexeme("�킩��Ȃ�"))                       return set_info(info, T_WAKARAN);
            if (lexeme("��"))                               return set_info(info, T_SUM);
            if (lexeme("���"))                             return set_info(info, T_ROKU);
            if (lexeme("�낮"))                             return set_info(info, T_LOG);
            if (lexeme("�낭"))                             return set_info(info, T_ROKU);
            if (lexeme("�ꂢ"))                             return set_info(info, T_ZERO);
            if (lexeme("��[��"))                           return set_info(info, T_ROOT);
            if (lexeme("�邤��"))                           return set_info(info, T_ROOT);
            if (lexeme("����ۂ�"))                         return set_info(info, T_RIPPOU);
            if (lexeme("��["))                             return set_info(info, T_KANA);
            if (lexeme("���"))                             return set_info(info, T_YON);
            if (lexeme("��낸"))                           return set_info(info, T_MAN);
            if (lexeme("���"))                             return set_info(info, T_YON);
            if (lexeme("�悻"))                             return set_info(info, T_YOSO);
            if (lexeme("�悰��"))                           return set_info(info, T_COS);
            if (lexeme("�您"))                             return set_info(info, T_KANA);
            if (lexeme("�悧"))                             return set_info(info, T_KANA);
            if (lexeme("�悤"))                             return set_info(info, T_KANA);
            if (lexeme("�患"))                             return set_info(info, T_KANA);
            if (lexeme("��"))                               return set_info(info, T_KANA);
            if (lexeme("��["))                             return set_info(info, T_HACHI);
            if (lexeme("����"))                           return set_info(info, T_SURUTO);
            if (lexeme("����"))                           return set_info(info, T_SURUTO);
            if (lexeme("����"))                           return set_info(info, T_SURUTO);
            if (lexeme("���"))                             return set_info(info, T_SURUTO);
            if (lexeme("����"))                           return set_info(info, T_SURUTO);
            if (lexeme("���Ȃ���"))                       return set_info(info, T_SURUTO);
            if (lexeme("��˂�"))                           return set_info(info, T_KANA);
            if (lexeme("���"))                             return set_info(info, T_KANA);
            if (lexeme("����Ă݂��"))                     return set_info(info, T_SURUTO);
            if (lexeme("����Ă݂��"))                     return set_info(info, T_SURUTO);
            if (lexeme("����Ă݂�"))                       return set_info(info, T_SURUTO);
            if (lexeme("����Ă݂��"))                     return set_info(info, T_SURUTO);
            if (lexeme("����Ă݂�Ƃ�"))                   return set_info(info, T_SURUTO);
            if (lexeme("����Ă݂��"))                     return set_info(info, T_SURUTO);
            if (lexeme("����Ă݂�"))                       return set_info(info, T_SURUTO);
            if (lexeme("����Ă݂���"))                     return set_info(info, T_SURUTO);
            if (lexeme("����Ă݂��Ƃ�"))                   return set_info(info, T_SURUTO);
            if (lexeme("����Ă݂�"))                       return set_info(info, T_SURUTO);
            if (lexeme("����Ă݂�"))                       return set_info(info, T_SURUTO);
            if (lexeme("����Ă�"))                         return set_info(info, T_SURUTO);
            if (lexeme("�����"))                           return set_info(info, T_SHITE);
            if (lexeme("�������"))                         return set_info(info, T_SURUTO);
            if (lexeme("������Ƃ�"))                       return set_info(info, T_SURUTO);
            if (lexeme("�����"))                           return set_info(info, T_SHITA);
            if (lexeme("���"))                             return set_info(info, T_HACHI);
            if (lexeme("�₨"))                             return set_info(info, T_YAO);
            if (lexeme("�₠"))                             return set_info(info, T_HACHI);
            if (lexeme("�₟"))                             return set_info(info, T_HACHI);
            if (lexeme("��"))                               return set_info(info, T_KANA);
            if (lexeme("����"))                             return set_info(info, T_HYAKU);
            if (lexeme("����"))                             return set_info(info, T_MONO);
            if (lexeme("���Ƃ߂��"))                       return set_info(info, T_SURUTO);
            if (lexeme("���Ƃ߂�"))                         return set_info(info, T_SURUTO);
            if (lexeme("���Ƃ߂Ȃ���"))                     return set_info(info, T_SURUTO);
            if (lexeme("���Ƃ߂Ă݂�"))                     return set_info(info, T_SURUTO);
            if (lexeme("���Ƃ߂Ă݂�"))                     return set_info(info, T_SURUTO);
            if (lexeme("���Ƃ߂Ă݂�"))                     return set_info(info, T_SURUTO);
            if (lexeme("���Ƃ߂Ă�"))                       return set_info(info, T_SURUTO);
            if (lexeme("���Ƃ߂�"))                         return set_info(info, T_SHITE);
            if (lexeme("���Ƃ߂���"))                       return set_info(info, T_SURUTO);
            if (lexeme("���Ƃ߂�"))                         return set_info(info, T_SHITA);
            if (lexeme("���Ƃ�"))                           return set_info(info, T_SHITE);
            if (lexeme("��"))                               return set_info(info, T_WO1);
            if (lexeme("�ނ�傤��������"))                 return set_info(info, T_MURYOUTAISUU);
            if (lexeme("�ނ�"))                             return set_info(info, T_ROKU);
            if (lexeme("�ނ�"))                             return set_info(info, T_ROKU);
            if (lexeme("��"))                               return set_info(info, T_ROKU);
            if (lexeme("�݂��"))                           return set_info(info, T_ALL);
            if (lexeme("�݂��"))                           return set_info(info, T_MIRUTO);
            if (lexeme("�݂��"))                           return set_info(info, T_MIRUTO);
            if (lexeme("�݂�"))                             return set_info(info, T_SAN);
            if (lexeme("�݂�"))                             return set_info(info, T_SAN);
            if (lexeme("��"))                               return set_info(info, T_SAN);
            if (lexeme("�܂�"))                             return set_info(info, T_MAN);
            if (lexeme("�܂��Ȃ�����Ƃ���"))               return set_info(info, T_HIKUTO);
            if (lexeme("�܂��Ȃ�����Ƃ�"))                 return set_info(info, T_HIKUTO);
            if (lexeme("�܂��Ȃ������"))                   return set_info(info, T_HIKUTO);
            if (lexeme("�܂��Ȃ������"))                   return set_info(info, T_HIKUTO);
            if (lexeme("�܂��Ȃ�����"))                     return set_info(info, T_HIKUTO);
            if (lexeme("�܂��Ȃ����Ȃ���"))                 return set_info(info, T_HIKUTO);
            if (lexeme("�܂��Ȃ����Ă݂�"))                 return set_info(info, T_HIKUTO);
            if (lexeme("�܂��Ȃ����Ă݂�"))                 return set_info(info, T_HIKUTO);
            if (lexeme("�܂��Ȃ����Ă݂�"))                 return set_info(info, T_HIKUTO);
            if (lexeme("�܂��Ȃ����Ă�"))                   return set_info(info, T_HIKUTO);
            if (lexeme("�܂��Ȃ�����"))                     return set_info(info, T_HIITE);
            if (lexeme("�܂��Ȃ�������"))                   return set_info(info, T_HIKUTO);
            if (lexeme("�܂��Ȃ������Ƃ�"))                 return set_info(info, T_HIKUTO);
            if (lexeme("�܂��Ȃ�����"))                     return set_info(info, T_HIITA);
            if (lexeme("�܂��Ȃ���"))                       return set_info(info, T_HIITE);
            if (lexeme("�܂��Ȃ������"))                   return set_info(info, T_HIKARERU);
            if (lexeme("�܂��Ȃ�"))                         return set_info(info, T_MINUS);
            if (lexeme("�ւ点��"))                         return set_info(info, T_HIKUTO);
            if (lexeme("�ւ点��"))                         return set_info(info, T_HIKUTO);
            if (lexeme("�ւ点��"))                         return set_info(info, T_HIKUTO);
            if (lexeme("�ւ点"))                           return set_info(info, T_HIKUTO);
            if (lexeme("�ւ炷�Ƃ���"))                     return set_info(info, T_HIKUTO);
            if (lexeme("�ւ炷�Ƃ�"))                       return set_info(info, T_HIKUTO);
            if (lexeme("�ւ炷��"))                         return set_info(info, T_HIKUTO);
            if (lexeme("�ւ炷"))                           return set_info(info, T_HIKU);
            if (lexeme("�ւ炵�Ȃ���"))                     return set_info(info, T_HIKUTO);
            if (lexeme("�ւ炵�Ă݂�"))                     return set_info(info, T_HIKUTO);
            if (lexeme("�ւ炵�Ă݂�"))                     return set_info(info, T_HIKUTO);
            if (lexeme("�ւ炵�Ă݂�"))                     return set_info(info, T_HIKUTO);
            if (lexeme("�ւ炵�Ă�"))                       return set_info(info, T_HIKUTO);
            if (lexeme("�ւ炵��"))                         return set_info(info, T_HIITE);
            if (lexeme("�ւ炵����"))                       return set_info(info, T_HIKUTO);
            if (lexeme("�ւ炵���Ƃ���"))                   return set_info(info, T_HIKUTO);
            if (lexeme("�ւ炵���Ƃ�"))                     return set_info(info, T_HIKUTO);
            if (lexeme("�ւ炵��"))                         return set_info(info, T_HIITA);
            if (lexeme("�ւ炵"))                           return set_info(info, T_HIITE);
            if (lexeme("�ւ��ق�����"))                     return set_info(info, T_HEIHOUKON);
            if (lexeme("�ւ��ق�"))                         return set_info(info, T_HEIHOU);
            if (lexeme("�ւ�����"))                       return set_info(info, T_AVERAGE);
            if (lexeme("�ւ�����"))                         return set_info(info, T_AVERAGE);
            if (lexeme("��"))                               return set_info(info, T_NI);
            if (lexeme("�Ղ炷����Ƃ���"))                 return set_info(info, T_TASUTO);
            if (lexeme("�Ղ炷����Ƃ�"))                   return set_info(info, T_TASUTO);
            if (lexeme("�Ղ炷�����"))                     return set_info(info, T_TASUTO);
            if (lexeme("�Ղ炷�����"))                     return set_info(info, T_TASUTO);
            if (lexeme("�Ղ炷����"))                       return set_info(info, T_TASUTO);
            if (lexeme("�Ղ炷���Ȃ���"))                   return set_info(info, T_TASUTO);
            if (lexeme("�Ղ炷���Ă݂�"))                   return set_info(info, T_TASUTO);
            if (lexeme("�Ղ炷���Ă݂�"))                   return set_info(info, T_TASUTO);
            if (lexeme("�Ղ炷���Ă݂�"))                   return set_info(info, T_TASUTO);
            if (lexeme("�Ղ炷���Ă�"))                     return set_info(info, T_TASUTO);
            if (lexeme("�Ղ炷����"))                       return set_info(info, T_TASHITE);
            if (lexeme("�Ղ炷������"))                     return set_info(info, T_TASUTO);
            if (lexeme("�Ղ炷�����Ƃ�"))                   return set_info(info, T_TASUTO);
            if (lexeme("�Ղ炷����"))                       return set_info(info, T_TASHITA);
            if (lexeme("�Ղ炷��"))                         return set_info(info, T_TASHITE);
            if (lexeme("�Ղ炷�����"))                     return set_info(info, T_TASARERU);
            if (lexeme("�Ղ炷"))                           return set_info(info, T_PLUS);
            if (lexeme("�Ԃ��"))                           return set_info(info, T_BUNNO);
            if (lexeme("�ӂ₹��"))                         return set_info(info, T_TASUTO);
            if (lexeme("�ӂ₹��"))                         return set_info(info, T_TASUTO);
            if (lexeme("�ӂ₹��"))                         return set_info(info, T_TASUTO);
            if (lexeme("�ӂ₹"))                           return set_info(info, T_TASUTO);
            if (lexeme("�ӂ₷�Ƃ�"))                       return set_info(info, T_TASUTO);
            if (lexeme("�ӂ₷��"))                         return set_info(info, T_TASUTO);
            if (lexeme("�ӂ₷"))                           return set_info(info, T_TASU);
            if (lexeme("�ӂ₵�Ȃ���"))                     return set_info(info, T_TASUTO);
            if (lexeme("�ӂ₵�Ă݂�"))                     return set_info(info, T_TASUTO);
            if (lexeme("�ӂ₵�Ă݂�"))                     return set_info(info, T_TASUTO);
            if (lexeme("�ӂ₵�Ă݂�"))                     return set_info(info, T_TASUTO);
            if (lexeme("�ӂ₵�Ă�"))                       return set_info(info, T_TASUTO);
            if (lexeme("�ӂ₵��"))                         return set_info(info, T_TASHITE);
            if (lexeme("�ӂ₵����"))                       return set_info(info, T_TASUTO);
            if (lexeme("�ӂ₵���Ƃ���"))                   return set_info(info, T_TASUTO);
            if (lexeme("�ӂ₵���Ƃ�"))                     return set_info(info, T_TASUTO);
            if (lexeme("�ӂ₵��"))                         return set_info(info, T_TASHITA);
            if (lexeme("�ӂ�"))                             return set_info(info, T_NI);
            if (lexeme("�ӂ�����"))                         return set_info(info, T_FUKASHIGI);
            if (lexeme("�҂Ⴍ"))                           return set_info(info, T_HYAKU);
            if (lexeme("�тႭ"))                           return set_info(info, T_HYAKU);
            if (lexeme("�Ё["))                             return set_info(info, T_ICHI);
            if (lexeme("�ЂႭ"))                           return set_info(info, T_HYAKU);
            if (lexeme("�Ђ�"))                             return set_info(info, T_ICHI);
            if (lexeme("�Ђ�"))                             return set_info(info, T_NANA);
            if (lexeme("�Ђ��傷��"))                       return set_info(info, T_HIJOSUU);
            if (lexeme("�Ђ��傤����"))                     return set_info(info, T_HIJOUSUU);
            if (lexeme("�Ђ��񂷂�"))                       return set_info(info, T_HIGENSUU);
            if (lexeme("�Ђ���"))                           return set_info(info, T_HIKUTO);
            if (lexeme("�Ђ���"))                           return set_info(info, T_HIKUTO);
            if (lexeme("�Ђ���"))                           return set_info(info, T_HIKUTO);
            if (lexeme("�Ђ�"))                             return set_info(info, T_HIKUTO);
            if (lexeme("�Ђ��Ƃ���"))                       return set_info(info, T_HIKUTO);
            if (lexeme("�Ђ��Ƃ�"))                         return set_info(info, T_HIKUTO);
            if (lexeme("�Ђ���"))                           return set_info(info, T_HIKUTO);
            if (lexeme("�Ђ����Ƃ�"))                       return set_info(info, T_HIKU);
            if (lexeme("�Ђ�"))                             return set_info(info, T_HIKU);
            if (lexeme("�Ђ��Ȃ���"))                       return set_info(info, T_HIKUTO);
            if (lexeme("�Ђ�����"))                         return set_info(info, T_HIKIZAN);
            if (lexeme("�Ђ�"))                             return set_info(info, T_HIITE);
            if (lexeme("�Ђ����"))                         return set_info(info, T_HIKARERU);
            if (lexeme("�Ђ�����"))                         return set_info(info, T_HIKASUU);
            if (lexeme("�Ђ��Ă݂�"))                       return set_info(info, T_HIKUTO);
            if (lexeme("�Ђ��Ă݂�"))                       return set_info(info, T_HIKUTO);
            if (lexeme("�Ђ��Ă݂�"))                       return set_info(info, T_HIKUTO);
            if (lexeme("�Ђ��Ă�"))                         return set_info(info, T_HIKUTO);
            if (lexeme("�Ђ���"))                           return set_info(info, T_HIITE);
            if (lexeme("�Ђ�����"))                         return set_info(info, T_HIKUTO);
            if (lexeme("�Ђ����Ƃ���"))                     return set_info(info, T_HIKUTO);
            if (lexeme("�Ђ����Ƃ�"))                       return set_info(info, T_HIKUTO);
            if (lexeme("�Ђ���"))                           return set_info(info, T_HIITA);
            if (lexeme("�Ђ�"))                             return set_info(info, T_ICHI);
            if (lexeme("�Ђ�"))                             return set_info(info, T_ICHI);
            if (lexeme("�ρ[�����"))                       return set_info(info, T_PERCENT);
            if (lexeme("�ρ["))                             return set_info(info, T_PERCENT);
            if (lexeme("�ς�"))                             return set_info(info, T_HACHI);
            if (lexeme("�ς�"))                             return set_info(info, T_PI);
            if (lexeme("�΂�"))                             return set_info(info, T_MAN);
            if (lexeme("�΂�"))                             return set_info(info, T_BAI);
            if (lexeme("�͂�"))                             return set_info(info, T_HACHI);
            if (lexeme("�͂�"))                             return set_info(info, T_HACHI);
            if (lexeme("�͂�"))                             return set_info(info, T_HATA);
            if (lexeme("�͂��ρ[�ڂ����"))                 return set_info(info, T_HYPERBOLIC);
            if (lexeme("�͂��ςڂ����"))                   return set_info(info, T_HYPERBOLIC);
            if (lexeme("��"))                               return set_info(info, T_HA);
            if (lexeme("�̂�"))                             return set_info(info, T_KANA);
            if (lexeme("�̂�"))                             return set_info(info, T_NO1);
            if (lexeme("�̂Ȃ���"))                         return set_info(info, T_NOUCHI);
            if (lexeme("�̂�����"))                         return set_info(info, T_NOUCHI);
            if (lexeme("�̂���"))                           return set_info(info, T_NOUCHI);
            if (lexeme("��"))                               return set_info(info, T_NO1);
            if (lexeme("�˂�"))                             return set_info(info, T_KANA);
            if (lexeme("�˂��҂�����"))                     return set_info(info, T_E);
            if (lexeme("��"))                               return set_info(info, T_KANA);
            if (lexeme("�Ɂ["))                             return set_info(info, T_NI);
            if (lexeme("�ɂ�"))                             return set_info(info, T_NI);
            if (lexeme("�ɂ������Ƃ�"))                     return set_info(info, T_TASU);
            if (lexeme("�ɂ��킦�邱�Ƃ�"))                 return set_info(info, T_TASU);
            if (lexeme("�ɂ����邱�Ƃ�"))                   return set_info(info, T_KAKERU);
            if (lexeme("�ɂ�"))                             return set_info(info, T_NI);
            if (lexeme("��"))                               return set_info(info, T_NI);
            if (lexeme("�ȁ[��"))                         return set_info(info, T_IKURA);
            if (lexeme("�ȁ[��"))                           return set_info(info, T_IKURA);
            if (lexeme("�Ȃ�ڂɂȂ��ł��傤"))           return set_info(info, T_IKURA);
            if (lexeme("�Ȃ�ڂɂȂ��"))                 return set_info(info, T_IKURA);
            if (lexeme("�Ȃ�ڂɂȂ�̂ł��傤"))           return set_info(info, T_IKURA);
            if (lexeme("�Ȃ�ڂɂȂ�ł��傤"))             return set_info(info, T_IKURA);
            if (lexeme("�Ȃ�ڂɂȂ�"))                     return set_info(info, T_IKURA);
            if (lexeme("�Ȃ�ڂɂȂ�܂�"))                 return set_info(info, T_IKURA);
            if (lexeme("�Ȃ�ڂȂ�"))                       return set_info(info, T_IKURA);
            if (lexeme("�Ȃ�ڂȂ�"))                       return set_info(info, T_IKURA);
            if (lexeme("�Ȃ�ڂł��傤"))                   return set_info(info, T_IKURA);
            if (lexeme("�Ȃ�ڂ��낤"))                     return set_info(info, T_IKURA);
            if (lexeme("�Ȃ��"))                           return set_info(info, T_IKURA);
            if (lexeme("�Ȃ�ɂȂ��ł��傤"))             return set_info(info, T_IKURA);
            if (lexeme("�Ȃ�ɂȂ��"))                   return set_info(info, T_IKURA);
            if (lexeme("�Ȃ�ɂȂ�̂ł��傤"))             return set_info(info, T_IKURA);
            if (lexeme("�Ȃ�ɂȂ�ł��傤"))               return set_info(info, T_IKURA);
            if (lexeme("�Ȃ�ɂȂ�"))                       return set_info(info, T_IKURA);
            if (lexeme("�Ȃ�ɂȂ�܂�"))                   return set_info(info, T_IKURA);
            if (lexeme("�Ȃ�Ȃ�ł�"))                     return set_info(info, T_IKURA);
            if (lexeme("�Ȃ�Ȃ�ł�"))                     return set_info(info, T_IKURA);
            if (lexeme("�Ȃ�Ȃ�ł��傤"))                 return set_info(info, T_IKURA);
            if (lexeme("�Ȃ�Ȃ񂾂낤"))                   return set_info(info, T_IKURA);
            if (lexeme("�Ȃ�Ȃ�"))                         return set_info(info, T_IKURA);
            if (lexeme("�Ȃ�Ȃ̂ł�"))                     return set_info(info, T_IKURA);
            if (lexeme("�Ȃ�Ȃ̂ł�"))                     return set_info(info, T_IKURA);
            if (lexeme("�Ȃ�Ȃ̂ł��傤"))                 return set_info(info, T_IKURA);
            if (lexeme("�Ȃ�Ȃ̂��낤"))                   return set_info(info, T_IKURA);
            if (lexeme("�Ȃ�Ȃ�"))                         return set_info(info, T_IKURA);
            if (lexeme("�Ȃ�ł�"))                         return set_info(info, T_IKURA);
            if (lexeme("�Ȃ�ł�"))                         return set_info(info, T_IKURA);
            if (lexeme("�Ȃ�ł��傤"))                     return set_info(info, T_IKURA);
            if (lexeme("�Ȃ񂾂낤"))                       return set_info(info, T_IKURA);
            if (lexeme("�Ȃ�"))                           return set_info(info, T_IKURA);
            if (lexeme("�Ȃ�"))                             return set_info(info, T_IKURA);
            if (lexeme("�Ȃ䂽"))                           return set_info(info, T_NAYUTA);
            if (lexeme("�Ȃ�"))                             return set_info(info, T_NANA);
            if (lexeme("�ȂɂɂȂ��ł�"))                 return set_info(info, T_IKURA);
            if (lexeme("�ȂɂɂȂ�̂ł�"))                 return set_info(info, T_IKURA);
            if (lexeme("�ȂɂɂȂ�ł��傤"))               return set_info(info, T_IKURA);
            if (lexeme("�ȂɂɂȂ�"))                       return set_info(info, T_IKURA);
            if (lexeme("�Ȃɂł�"))                         return set_info(info, T_IKURA);
            if (lexeme("�Ȃɂł�"))                         return set_info(info, T_IKURA);
            if (lexeme("�Ȃ�"))                             return set_info(info, T_IKURA);
            if (lexeme("�Ȃ�"))                             return set_info(info, T_NANA);
            if (lexeme("�Ȃ�����"))                         return set_info(info, T_SURUTO);
            if (lexeme("�Ȃ�����"))                         return set_info(info, T_SURUTO);
            if (lexeme("�Ȃ���"))                           return set_info(info, T_SURUTO);
            if (lexeme("�Ȃ����Ƃ���"))                     return set_info(info, T_SURUTO);
            if (lexeme("�Ȃ����Ƃ�"))                       return set_info(info, T_SURUTO);
            if (lexeme("�Ȃ�����"))                         return set_info(info, T_SURUTO);
            if (lexeme("�Ȃ����Ȃ���"))                     return set_info(info, T_SURUTO);
            if (lexeme("�Ȃ����Ă݂��"))                   return set_info(info, T_SURUTO);
            if (lexeme("�Ȃ����Ă݂��"))                   return set_info(info, T_SURUTO);
            if (lexeme("�Ȃ����Ă݂�"))                     return set_info(info, T_SURUTO);
            if (lexeme("�Ȃ����Ă݂�"))                     return set_info(info, T_SURUTO);
            if (lexeme("�Ȃ����Ă݂Ȃ���"))                 return set_info(info, T_SURUTO);
            if (lexeme("�Ȃ����Ă݂�"))                     return set_info(info, T_SURUTO);
            if (lexeme("�Ȃ����Ă݂�"))                     return set_info(info, T_SURUTO);
            if (lexeme("�Ȃ����Ă�"))                       return set_info(info, T_SURUTO);
            if (lexeme("�Ȃ�����"))                         return set_info(info, T_SURUTO);
            if (lexeme("�Ȃ�������"))                       return set_info(info, T_SURUTO);
            if (lexeme("�Ȃ�����"))                         return set_info(info, T_SHITA);
            if (lexeme("�Ȃ���"))                           return set_info(info, T_IKURA);
            if (lexeme("�ǂ�Ȃ����ɂȂ��ł��傤"))       return set_info(info, T_IKURA);
            if (lexeme("�ǂ�Ȃ����ɂȂ�̂ł��傤"))       return set_info(info, T_IKURA);
            if (lexeme("�ǂ�Ȃ����ɂȂ�ł��傤"))         return set_info(info, T_IKURA);
            if (lexeme("�ǂ�Ȃ����ɂȂ�"))                 return set_info(info, T_IKURA);
            if (lexeme("�ǂ�Ȃ����ɂȂ�܂�"))             return set_info(info, T_IKURA);
            if (lexeme("�ǂȂ��Ȃ��ł��傤"))             return set_info(info, T_IKURA);
            if (lexeme("�ǂȂ��Ȃ�̂ł��傤"))             return set_info(info, T_IKURA);
            if (lexeme("�ǂȂ��Ȃ�"))                       return set_info(info, T_IKURA);
            if (lexeme("�ǂȂ��ł�"))                       return set_info(info, T_IKURA);
            if (lexeme("�ǂȂ��ł�"))                       return set_info(info, T_IKURA);
            if (lexeme("�ǂȂ��ł��傤"))                   return set_info(info, T_IKURA);
            if (lexeme("�ǂȂ�"))                           return set_info(info, T_IKURA);
            if (lexeme("�ǂ��Ȃ�ł�"))                     return set_info(info, T_IKURA);
            if (lexeme("�ǂ��Ȃ�ł�"))                     return set_info(info, T_IKURA);
            if (lexeme("�ǂ��Ȃ�ł��傤"))                 return set_info(info, T_IKURA);
            if (lexeme("�ǂ��Ȃ�"))                         return set_info(info, T_IKURA);
            if (lexeme("�ǂ��Ȃ�"))                         return set_info(info, T_IKURA);
            if (lexeme("�ǂ��Ȃ�܂�"))                     return set_info(info, T_IKURA);
            if (lexeme("�ǂ��Ȃ̂ł�"))                     return set_info(info, T_IKURA);
            if (lexeme("�ǂ��Ȃ̂ł�"))                     return set_info(info, T_IKURA);
            if (lexeme("�ǂ��Ȃ̂ł��傤"))                 return set_info(info, T_IKURA);
            if (lexeme("�ǂ��Ȃ�"))                         return set_info(info, T_IKURA);
            if (lexeme("�ǂ�"))                             return set_info(info, T_IKURA);
            if (lexeme("��"))                               return set_info(info, T_DO);
            if (lexeme("�ƂˁA"))                           return set_info(info, T_TO1);
            if (lexeme("�ƂȁA"))                           return set_info(info, T_TO1);
            if (lexeme("�Ƃ�������"))                       return set_info(info, T_R_PAREN);
            if (lexeme("�Ƃ�"))                             return set_info(info, T_JUU);
            if (lexeme("��"))                               return set_info(info, T_TO1);
            if (lexeme("�ł�˂�"))                         return set_info(info, T_KANA);
            if (lexeme("��"))                               return set_info(info, T_DE);
            if (lexeme("�Ă�"))                             return set_info(info, T_DOT);
            if (lexeme("�Ԃ�"))                           return set_info(info, T_BAI);
            if (lexeme("����"))                             return set_info(info, T_HA);
            if (lexeme("���傤�ЂႭ"))                     return set_info(info, T_HYAKU);
            if (lexeme("���傤�͂�"))                       return set_info(info, T_GO);
            if (lexeme("���傤"))                           return set_info(info, T_CHOU);
            if (lexeme("��"))                               return set_info(info, T_SEN);
            if (lexeme("���񂺂��"))                       return set_info(info, T_TAN);
            if (lexeme("���񂶂����"))                     return set_info(info, T_TAN);
            if (lexeme("������"))                           return set_info(info, T_TASUTO);
            if (lexeme("������"))                           return set_info(info, T_TASUTO);
            if (lexeme("������"))                           return set_info(info, T_TASUTO);
            if (lexeme("����"))                             return set_info(info, T_TASUTO);
            if (lexeme("�����Ƃ�"))                         return set_info(info, T_TASUTO);
            if (lexeme("������"))                           return set_info(info, T_TASUTO);
            if (lexeme("�������Ƃ�"))                       return set_info(info, T_TASU);
            if (lexeme("����"))                             return set_info(info, T_TASU);
            if (lexeme("�����Ȃ���"))                       return set_info(info, T_TASUTO);
            if (lexeme("�����Ă݂�"))                       return set_info(info, T_TASUTO);
            if (lexeme("�����Ă݂�"))                       return set_info(info, T_TASUTO);
            if (lexeme("�����Ă݂�"))                       return set_info(info, T_TASUTO);
            if (lexeme("�����Ă�"))                         return set_info(info, T_TASUTO);
            if (lexeme("������"))                           return set_info(info, T_TASHITE);
            if (lexeme("��������"))                         return set_info(info, T_TASUTO);
            if (lexeme("�������Ƃ���"))                     return set_info(info, T_TASUTO);
            if (lexeme("�������Ƃ�"))                       return set_info(info, T_TASUTO);
            if (lexeme("������"))                           return set_info(info, T_TASHITA);
            if (lexeme("��������"))                         return set_info(info, T_TASHIZAN);
            if (lexeme("�������킹���"))                   return set_info(info, T_TASUTO);
            if (lexeme("�������킹���"))                   return set_info(info, T_TASUTO);
            if (lexeme("�������킹��"))                     return set_info(info, T_TASUTO);
            if (lexeme("�������킹���"))                   return set_info(info, T_TASUTO);
            if (lexeme("�������킹��Ƃ�"))                 return set_info(info, T_TASUTO);
            if (lexeme("�������킹���"))                   return set_info(info, T_TASUTO);
            if (lexeme("�������킹��"))                     return set_info(info, T_TASU);
            if (lexeme("�������킹��"))                     return set_info(info, T_TASUTO);
            if (lexeme("�������킹�Ȃ���"))                 return set_info(info, T_TASUTO);
            if (lexeme("�������킹�Ă݂�"))                 return set_info(info, T_TASUTO);
            if (lexeme("�������킹�Ă݂�"))                 return set_info(info, T_TASUTO);
            if (lexeme("�������킹�Ă݂�"))                 return set_info(info, T_TASUTO);
            if (lexeme("�������킹�Ă�"))                   return set_info(info, T_TASUTO);
            if (lexeme("�������킹��"))                     return set_info(info, T_TASHITE);
            if (lexeme("�������킹���Ƃ���"))               return set_info(info, T_TASUTO);
            if (lexeme("�������킹���Ƃ�"))                 return set_info(info, T_TASUTO);
            if (lexeme("�������킹��"))                     return set_info(info, T_TASHITA);
            if (lexeme("�������킹"))                       return set_info(info, T_TASUTO);
            if (lexeme("�������킷"))                       return set_info(info, T_TASU);
            if (lexeme("�������킳���"))                   return set_info(info, T_TASARERU);
            if (lexeme("����"))                             return set_info(info, T_TASHITE);
            if (lexeme("�������"))                         return set_info(info, T_TASARERU);
            if (lexeme("�����������񂷂�"))                 return set_info(info, T_LOG);
            if (lexeme("��������"))                         return set_info(info, T_LOG);
            if (lexeme("�����"))                           return set_info(info, T_SONO);
            if (lexeme("���ꂪ����"))                       return set_info(info, T_SONO);
            if (lexeme("���ꂪ��"))                         return set_info(info, T_SORE);
            if (lexeme("����"))                             return set_info(info, T_SORE);
            if (lexeme("�����"))                           return set_info(info, T_SORE);
            if (lexeme("���̂Ƃ�"))                         return set_info(info, T_SURUTO);
            if (lexeme("���̂�����"))                       return set_info(info, T_KOTAE);
            if (lexeme("����"))                             return set_info(info, T_SONO);
            if (lexeme("������"))                           return set_info(info, T_SUM);
            if (lexeme("�������傭����"))                   return set_info(info, T_HYPERBOLIC);
            if (lexeme("������"))                           return set_info(info, T_SORE);
            if (lexeme("��"))                               return set_info(info, T_JUU);
            if (lexeme("�����"))                           return set_info(info, T_ALL);
            if (lexeme("����"))                             return set_info(info, T_SEN);
            if (lexeme("����"))                             return set_info(info, T_ZERO);
            if (lexeme("����������"))                       return set_info(info, T_ZETTAICHI);
            if (lexeme("����"))                             return set_info(info, T_SEN);
            if (lexeme("����"))                             return set_info(info, T_SURUTO);
            if (lexeme("����"))                             return set_info(info, T_PROD);
            if (lexeme("������"))                           return set_info(info, T_SURUTO);
            if (lexeme("��������"))                         return set_info(info, T_TAN);
            if (lexeme("��������"))                         return set_info(info, T_SIN);
            if (lexeme("����"))                             return set_info(info, T_SEI);
            if (lexeme("�����"))                           return set_info(info, T_SURUTO);
            if (lexeme("����Ƃ���"))                       return set_info(info, T_SURUTO);
            if (lexeme("����Ƃ�"))                         return set_info(info, T_SURUTO);
            if (lexeme("�����"))                           return set_info(info, T_SURUTO);
            if (lexeme("����"))                             return set_info(info, T_SURU);
            if (lexeme("���ׂ�"))                           return set_info(info, T_ALL);
            if (lexeme("����["))                           return set_info(info, T_JOU1);
            if (lexeme("���傷��"))                         return set_info(info, T_JOSUU);
            if (lexeme("���傴��"))                         return set_info(info, T_WARIZAN);
            if (lexeme("���傤�悤��������"))               return set_info(info, T_LOG10);
            if (lexeme("���傤��"))                         return set_info(info, T_AMARI);
            if (lexeme("���傤����"))                       return set_info(info, T_JOUSUU);
            if (lexeme("���傤����"))                       return set_info(info, T_KAKEZAN);
            if (lexeme("���傤"))                           return set_info(info, T_JOU1);
            if (lexeme("����"))                             return set_info(info, T_JO);
            if (lexeme("����["))                           return set_info(info, T_JUU);
            if (lexeme("�����"))                           return set_info(info, T_JUU);
            if (lexeme("���イ"))                           return set_info(info, T_JUU);
            if (lexeme("����"))                             return set_info(info, T_JUU);
            if (lexeme("�����傤"))                         return set_info(info, T_HEIHOU);
            if (lexeme("���[��"))                           return set_info(info, T_SURUTO);
            if (lexeme("���["))                             return set_info(info, T_YON);
            if (lexeme("�����"))                           return set_info(info, T_SURUTO);
            if (lexeme("�����"))                           return set_info(info, T_SURUTO);
            if (lexeme("����"))                             return set_info(info, T_SURUTO);
            if (lexeme("�����"))                           return set_info(info, T_WAKARAN);
            if (lexeme("����Ȃ�"))                         return set_info(info, T_WAKARAN);
            if (lexeme("����[���["))                       return set_info(info, T_SHOUSUU);
            if (lexeme("����[����"))                       return set_info(info, T_SHOUSUU);
            if (lexeme("����["))                           return set_info(info, T_QUOT);
            if (lexeme("���傤���["))                       return set_info(info, T_SHOUSUU);
            if (lexeme("���傤����"))                       return set_info(info, T_SHOUSUU);
            if (lexeme("���傤"))                           return set_info(info, T_QUOT);
            if (lexeme("���Ȃ���"))                         return set_info(info, T_SURUTO);
            if (lexeme("���Ă�"))                           return set_info(info, T_SHITE);
            if (lexeme("���Ă�"))                           return set_info(info, T_SHITE);
            if (lexeme("���Ă݂��"))                       return set_info(info, T_SURUTO);
            if (lexeme("���Ă݂��"))                       return set_info(info, T_SURUTO);
            if (lexeme("���Ă݂�"))                         return set_info(info, T_SURUTO);
            if (lexeme("���Ă݂��"))                       return set_info(info, T_SURUTO);
            if (lexeme("���Ă݂�Ƃ���"))                   return set_info(info, T_SURUTO);
            if (lexeme("���Ă݂�Ƃ�"))                     return set_info(info, T_SURUTO);
            if (lexeme("���Ă݂��"))                       return set_info(info, T_SURUTO);
            if (lexeme("���Ă݂�"))                         return set_info(info, T_SURUTO);
            if (lexeme("���Ă݂Ȃ���"))                     return set_info(info, T_SURUTO);
            if (lexeme("���Ă݂�"))                         return set_info(info, T_SURUTO);
            if (lexeme("���Ă݂�"))                         return set_info(info, T_SURUTO);
            if (lexeme("���Ă�"))                           return set_info(info, T_SURUTO);
            if (lexeme("����"))                             return set_info(info, T_SHITE);
            if (lexeme("����"))                             return set_info(info, T_NANA);
            if (lexeme("������"))                           return set_info(info, T_SURUTO);
            if (lexeme("�����Ƃ���"))                       return set_info(info, T_SURUTO);
            if (lexeme("�����Ƃ�"))                         return set_info(info, T_SURUTO);
            if (lexeme("����"))                             return set_info(info, T_SHITA);
            if (lexeme("�����񂽂������̂Ă�"))             return set_info(info, T_E);
            if (lexeme("�����񂽂�����"))                   return set_info(info, T_LOG);
            if (lexeme("���������񂷂�"))                   return set_info(info, T_EXP);
            if (lexeme("������"))                           return set_info(info, T_EXP);
            if (lexeme("������"))                           return set_info(info, T_SURUTO);
            if (lexeme("����"))                             return set_info(info, T_YON);
            if (lexeme("����"))                             return set_info(info, T_YON);
            if (lexeme("��"))                               return set_info(info, T_YON);
            if (lexeme("����"))                             return set_info(info, T_SAN);
            if (lexeme("�����Ђ���"))                       return set_info(info, T_HIKUTO);
            if (lexeme("�����Ђ���"))                       return set_info(info, T_HIKUTO);
            if (lexeme("�����Ђ�"))                         return set_info(info, T_HIKUTO);
            if (lexeme("�����Ђ��Ƃ�"))                     return set_info(info, T_HIKUTO);
            if (lexeme("�����Ђ��Ȃ���"))                   return set_info(info, T_HIKUTO);
            if (lexeme("�����Ђ�"))                         return set_info(info, T_HIITE);
            if (lexeme("�����Ђ����"))                     return set_info(info, T_HIKARERU);
            if (lexeme("�����Ђ��Ă݂�"))                   return set_info(info, T_HIKUTO);
            if (lexeme("�����Ђ��Ă݂�"))                   return set_info(info, T_HIKUTO);
            if (lexeme("�����Ђ��Ă݂�"))                   return set_info(info, T_HIKUTO);
            if (lexeme("�����Ђ��Ă�"))                     return set_info(info, T_HIKUTO);
            if (lexeme("�����Ђ���"))                       return set_info(info, T_HIITE);
            if (lexeme("�����Ђ�����"))                     return set_info(info, T_HIKUTO);
            if (lexeme("�����Ђ����Ƃ���"))                 return set_info(info, T_HIKUTO);
            if (lexeme("�����Ђ����Ƃ�"))                   return set_info(info, T_HIKUTO);
            if (lexeme("�����Ђ���"))                       return set_info(info, T_HIITA);
            if (lexeme("������"))                           return set_info(info, T_SIN);
            if (lexeme("���������ƂȂ����"))               return set_info(info, T_MAX);
            if (lexeme("���������ƂȂ邩��"))               return set_info(info, T_MAX);
            if (lexeme("����������"))                       return set_info(info, T_MAX);
            if (lexeme("��������[�ƂȂ邩��"))             return set_info(info, T_MIN);
            if (lexeme("��������[��"))                     return set_info(info, T_MIN);
            if (lexeme("�������傤�ƂȂ邩��"))             return set_info(info, T_MIN);
            if (lexeme("�������傤��"))                     return set_info(info, T_MIN);
            if (lexeme("����"))                             return set_info(info, T_SAI);
            if (lexeme("��"))                               return set_info(info, T_DIFF);
            if (lexeme("����"))                             return set_info(info, T_GOKU);
            if (lexeme("��������"))                         return set_info(info, T_SUM);
            if (lexeme("����������"))                       return set_info(info, T_GOUGASHA);
            if (lexeme("����"))                             return set_info(info, T_GO);
            if (lexeme("��"))                               return set_info(info, T_GO);
            if (lexeme("�����"))                           return set_info(info, T_SONO);
            if (lexeme("����"))                             return set_info(info, T_SORE);
            if (lexeme("�����"))                           return set_info(info, T_SORE);
            if (lexeme("���Ԃ�"))                           return set_info(info, T_BAI);
            if (lexeme("���̂Ƃ�"))                         return set_info(info, T_SURUTO);
            if (lexeme("���������"))                       return set_info(info, T_OSHIETE);
            if (lexeme("���������"))                       return set_info(info, T_OSHIETE);
            if (lexeme("��������"))                         return set_info(info, T_OSHIETE);
            if (lexeme("��������"))                         return set_info(info, T_OSHIETE);
            if (lexeme("�������Ȃ���"))                     return set_info(info, T_OSHIETE);
            if (lexeme("�������Ă݂�"))                     return set_info(info, T_OSHIETE);
            if (lexeme("�������Ă݂�"))                     return set_info(info, T_OSHIETE);
            if (lexeme("�������Ă݂�"))                     return set_info(info, T_OSHIETE);
            if (lexeme("�������Ă�"))                       return set_info(info, T_OSHIETE);
            if (lexeme("�������Ă���邩��"))               return set_info(info, T_OSHIETE);
            if (lexeme("�������Ă���邩"))                 return set_info(info, T_OSHIETE);
            if (lexeme("�������Ă����"))                   return set_info(info, T_OSHIETE);
            if (lexeme("�������Ă����"))                   return set_info(info, T_OSHIETE);
            if (lexeme("�������Ă����"))                   return set_info(info, T_OSHIETE);
            if (lexeme("�������Ă���"))                     return set_info(info, T_OSHIETE);
            if (lexeme("�������Ă���������"))               return set_info(info, T_OSHIETE);
            if (lexeme("�������Ă�������"))                 return set_info(info, T_OSHIETE);
            if (lexeme("��������"))                         return set_info(info, T_OSHIETE);
            if (lexeme("������"))                           return set_info(info, T_KOTAE);
            if (lexeme("������"))                           return set_info(info, T_COUNT);
            if (lexeme("��������"))                         return set_info(info, T_COS);
            if (lexeme("������"))                           return set_info(info, T_KYUU);
            if (lexeme("����"))                             return set_info(info, T_KYUU);
            if (lexeme("����"))                             return set_info(info, T_KOU);
            if (lexeme("������"))                           return set_info(info, T_SORE);
            if (lexeme("���񂷂�"))                         return set_info(info, T_GENSUU);
            if (lexeme("���񂴂�"))                         return set_info(info, T_HIKIZAN);
            if (lexeme("��������"))                         return set_info(info, T_KEISAN);
            if (lexeme("����"))                             return set_info(info, T_KEI);
            if (lexeme("���킦���"))                       return set_info(info, T_TASUTO);
            if (lexeme("���킦���"))                       return set_info(info, T_TASUTO);
            if (lexeme("���킦��"))                         return set_info(info, T_TASUTO);
            if (lexeme("���킦��Ƃ���"))                   return set_info(info, T_TASUTO);
            if (lexeme("���킦��Ƃ�"))                     return set_info(info, T_TASUTO);
            if (lexeme("���킦���"))                       return set_info(info, T_TASUTO);
            if (lexeme("���킦�邱�Ƃ�"))                   return set_info(info, T_TASU);
            if (lexeme("���킦��"))                         return set_info(info, T_TASU);
            if (lexeme("���킦����"))                     return set_info(info, T_TASARERU);
            if (lexeme("���킦�Ȃ���"))                     return set_info(info, T_TASUTO);
            if (lexeme("���킦�Ă݂�"))                     return set_info(info, T_TASUTO);
            if (lexeme("���킦�Ă݂�"))                     return set_info(info, T_TASUTO);
            if (lexeme("���킦�Ă݂�"))                     return set_info(info, T_TASUTO);
            if (lexeme("���킦�Ă�"))                       return set_info(info, T_TASUTO);
            if (lexeme("���킦��"))                         return set_info(info, T_TASHITE);
            if (lexeme("���킦����"))                       return set_info(info, T_TASUTO);
            if (lexeme("���킦���Ƃ���"))                   return set_info(info, T_TASUTO);
            if (lexeme("���킦���Ƃ�"))                     return set_info(info, T_TASUTO);
            if (lexeme("���킦��"))                         return set_info(info, T_TASHITA);
            if (lexeme("������"))                         return set_info(info, T_KANA);
            if (lexeme("����邩�Ȃ�"))                     return set_info(info, T_KANA);
            if (lexeme("����邩�Ȃ�"))                     return set_info(info, T_KANA);
            if (lexeme("����邩��"))                       return set_info(info, T_KANA);
            if (lexeme("����邩��"))                       return set_info(info, T_KANA);
            if (lexeme("�����"))                           return set_info(info, T_KANA);
            if (lexeme("�����["))                         return set_info(info, T_KANA);
            if (lexeme("����患"))                         return set_info(info, T_KANA);
            if (lexeme("�����"))                           return set_info(info, T_KANA);
            if (lexeme("�����"))                           return set_info(info, T_KANA);
            if (lexeme("����"))                             return set_info(info, T_KANA);
            if (lexeme("����������["))                     return set_info(info, T_KANA);
            if (lexeme("���������患"))                     return set_info(info, T_KANA);
            if (lexeme("����������"))                       return set_info(info, T_KANA);
            if (lexeme("��������"))                         return set_info(info, T_KANA);
            if (lexeme("��"))                               return set_info(info, T_KYUU);
            if (lexeme("���Ⴍ�悰��"))                     return set_info(info, T_ACOS);
            if (lexeme("���Ⴍ���񂺂��"))                 return set_info(info, T_ATAN);
            if (lexeme("���Ⴍ���񂶂����"))               return set_info(info, T_ATAN);
            if (lexeme("���Ⴍ��������"))                   return set_info(info, T_ATAN);
            if (lexeme("���Ⴍ��������"))                   return set_info(info, T_ASIN);
            if (lexeme("���Ⴍ����"))                       return set_info(info, T_GYAKUSUU);
            if (lexeme("���Ⴍ������"))                     return set_info(info, T_ASIN);
            if (lexeme("���Ⴍ��������"))                   return set_info(info, T_ACOS);
            if (lexeme("����["))                           return set_info(info, T_KYUU);
            if (lexeme("���イ"))                           return set_info(info, T_KYUU);
            if (lexeme("����"))                             return set_info(info, T_GAI);
            if (lexeme("��"))                               return set_info(info, T_HA);
            if (lexeme("���["))                             return set_info(info, T_KANA);
            if (lexeme("����"))                             return set_info(info, T_KAN);
            if (lexeme("����"))                             return set_info(info, T_KARA);
            if (lexeme("���ˁ["))                           return set_info(info, T_KANA);
            if (lexeme("���˂�"))                           return set_info(info, T_KANA);
            if (lexeme("���˂�"))                           return set_info(info, T_KANA);
            if (lexeme("����"))                             return set_info(info, T_KANA);
            if (lexeme("���ȁ["))                           return set_info(info, T_KANA);
            if (lexeme("���Ȃ�"))                           return set_info(info, T_KANA);
            if (lexeme("���Ȃ�"))                           return set_info(info, T_KANA);
            if (lexeme("����"))                             return set_info(info, T_KANA);
            if (lexeme("���������Ƃ���"))                   return set_info(info, T_R_PAREN);
            if (lexeme("���������Ƃ���"))                   return set_info(info, T_R_PAREN);
            if (lexeme("�������Ƃ���"))                     return set_info(info, T_R_PAREN);
            if (lexeme("�������Ƃ���"))                     return set_info(info, T_R_PAREN);
            if (lexeme("�������Ƃ�"))                       return set_info(info, T_R_PAREN);
            if (lexeme("������"))                           return set_info(info, T_L_PAREN);
            if (lexeme("����"))                             return set_info(info, T_MONO);
            if (lexeme("������"))                           return set_info(info, T_KASUU);
            if (lexeme("������"))                           return set_info(info, T_TASHIZAN);
            if (lexeme("�������"))                         return set_info(info, T_KAKERUTO);
            if (lexeme("�������"))                         return set_info(info, T_KAKERUTO);
            if (lexeme("������"))                           return set_info(info, T_KAKERUTO);
            if (lexeme("�������"))                         return set_info(info, T_KAKERUTO);
            if (lexeme("������Ƃ���"))                     return set_info(info, T_KAKERUTO);
            if (lexeme("������Ƃ�"))                       return set_info(info, T_KAKERUTO);
            if (lexeme("�������"))                         return set_info(info, T_KAKERUTO);
            if (lexeme("�����邱�Ƃ�"))                     return set_info(info, T_KAKERU);
            if (lexeme("������"))                           return set_info(info, T_KAKERU);
            if (lexeme("��������"))                       return set_info(info, T_KAKERARERU);
            if (lexeme("�����Ȃ���"))                       return set_info(info, T_KAKERUTO);
            if (lexeme("�����Ă݂�"))                       return set_info(info, T_KAKERUTO);
            if (lexeme("�����Ă݂�"))                       return set_info(info, T_KAKERUTO);
            if (lexeme("�����Ă݂�"))                       return set_info(info, T_KAKERUTO);
            if (lexeme("�����Ă�"))                         return set_info(info, T_KAKERUTO);
            if (lexeme("������"))                           return set_info(info, T_KAKETE);
            if (lexeme("��������"))                         return set_info(info, T_KAKERUTO);
            if (lexeme("�������Ƃ���"))                     return set_info(info, T_KAKERUTO);
            if (lexeme("�������Ƃ�"))                       return set_info(info, T_KAKERUTO);
            if (lexeme("������"))                           return set_info(info, T_KAKETA);
            if (lexeme("��������"))                         return set_info(info, T_KAKEZAN);
            if (lexeme("�������킹���"))                   return set_info(info, T_KAKERUTO);
            if (lexeme("�������킹���"))                   return set_info(info, T_KAKERUTO);
            if (lexeme("�������킹��"))                     return set_info(info, T_KAKERUTO);
            if (lexeme("�������킹���"))                   return set_info(info, T_KAKERUTO);
            if (lexeme("�������킹��Ƃ�"))                 return set_info(info, T_KAKERUTO);
            if (lexeme("�������킹���"))                   return set_info(info, T_KAKERUTO);
            if (lexeme("�������킹��"))                     return set_info(info, T_KAKERU);
            if (lexeme("�������킹��"))                     return set_info(info, T_KAKERUTO);
            if (lexeme("�������킹�Ȃ���"))                 return set_info(info, T_KAKERUTO);
            if (lexeme("�������킹�Ă݂�"))                 return set_info(info, T_KAKERUTO);
            if (lexeme("�������킹�Ă݂�"))                 return set_info(info, T_KAKERUTO);
            if (lexeme("�������킹�Ă݂�"))                 return set_info(info, T_KAKERUTO);
            if (lexeme("�������킹�Ă�"))                   return set_info(info, T_KAKERUTO);
            if (lexeme("�������킹��"))                     return set_info(info, T_KAKERUTO);
            if (lexeme("�������킹���Ƃ���"))               return set_info(info, T_KAKERUTO);
            if (lexeme("�������킹���Ƃ�"))                 return set_info(info, T_KAKERUTO);
            if (lexeme("�������킹��"))                     return set_info(info, T_KAKETA);
            if (lexeme("�������킹"))                       return set_info(info, T_KAKERUTO);
            if (lexeme("�������킷�Ƃ���"))                 return set_info(info, T_KAKERUTO);
            if (lexeme("�������킷�Ƃ�"))                   return set_info(info, T_KAKERUTO);
            if (lexeme("�������킷"))                       return set_info(info, T_KAKERU);
            if (lexeme("�������킵���Ƃ���"))               return set_info(info, T_KAKERUTO);
            if (lexeme("�������킵���Ƃ�"))                 return set_info(info, T_KAKERUTO);
            if (lexeme("�������킳���"))                   return set_info(info, T_KAKERARERU);
            if (lexeme("����"))                             return set_info(info, T_KAKERU);
            if (lexeme("�������傤"))                       return set_info(info, T_KAIJOU);
            if (lexeme("����"))                             return set_info(info, T_KANA);
            if (lexeme("����"))                             return set_info(info, T_KANA);
            if (lexeme("����"))                             return set_info(info, T_KANA);
            if (lexeme("��"))                               return set_info(info, T_KA);
            if (lexeme("���˂�����"))                       return set_info(info, T_ONEGAI);
            if (lexeme("���˂�����˂�"))                   return set_info(info, T_ONEGAI);
            if (lexeme("���˂�������"))                     return set_info(info, T_ONEGAI);
            if (lexeme("���˂������܂���"))                 return set_info(info, T_ONEGAI);
            if (lexeme("���˂������܂���"))                 return set_info(info, T_ONEGAI);
            if (lexeme("���˂������܂�"))                   return set_info(info, T_ONEGAI);
            if (lexeme("���˂����������܂���"))             return set_info(info, T_ONEGAI);
            if (lexeme("���˂����������܂���"))             return set_info(info, T_ONEGAI);
            if (lexeme("���˂����������܂�"))               return set_info(info, T_ONEGAI);
            if (lexeme("���˂���"))                         return set_info(info, T_ONEGAI);
            if (lexeme("���������"))                       return set_info(info, T_OSHIETE);
            if (lexeme("���������"))                       return set_info(info, T_OSHIETE);
            if (lexeme("��������"))                         return set_info(info, T_OSHIETE);
            if (lexeme("��������"))                         return set_info(info, T_OSHIETE);
            if (lexeme("�������Ȃ���"))                     return set_info(info, T_OSHIETE);
            if (lexeme("�������Ă�"))                       return set_info(info, T_OSHIETE);
            if (lexeme("�������Ă�"))                       return set_info(info, T_OSHIETE);
            if (lexeme("�������Ă�"))                       return set_info(info, T_OSHIETE);
            if (lexeme("�������Ă݂�"))                     return set_info(info, T_OSHIETE);
            if (lexeme("�������Ă݂�"))                     return set_info(info, T_OSHIETE);
            if (lexeme("�������Ă݂�"))                     return set_info(info, T_OSHIETE);
            if (lexeme("�������Ă�"))                       return set_info(info, T_OSHIETE);
            if (lexeme("�������Ăق����˂�"))               return set_info(info, T_OSHIETE);
            if (lexeme("�������Ă��傤������"))             return set_info(info, T_OSHIETE);
            if (lexeme("�������Ă��傤����"))               return set_info(info, T_OSHIETE);
            if (lexeme("�������Ă���邩��"))               return set_info(info, T_OSHIETE);
            if (lexeme("�������Ă���邩"))                 return set_info(info, T_OSHIETE);
            if (lexeme("�������Ă����"))                   return set_info(info, T_OSHIETE);
            if (lexeme("�������Ă���"))                     return set_info(info, T_OSHIETE);
            if (lexeme("�������Ă���������"))               return set_info(info, T_OSHIETE);
            if (lexeme("�������Ă�������"))                 return set_info(info, T_OSHIETE);
            if (lexeme("��������"))                         return set_info(info, T_OSHIETE);
            if (lexeme("����"))                             return set_info(info, T_OKU);
            if (lexeme("���񂵂イ���"))                   return set_info(info, T_PI);
            if (lexeme("���["))                             return set_info(info, T_E);
            if (lexeme("����"))                             return set_info(info, T_GO);
            if (lexeme("����"))                             return set_info(info, T_ICHI);
            if (lexeme("�����΂񂿂���������"))             return set_info(info, T_MIN);
            if (lexeme("�����΂񂿂���������"))             return set_info(info, T_MIN);
            if (lexeme("�����΂񂨂���������"))             return set_info(info, T_MAX);
            if (lexeme("�����΂񂨂���������"))             return set_info(info, T_MAX);
            if (lexeme("����"))                             return set_info(info, T_ICHI);
            if (lexeme("����"))                             return set_info(info, T_ISO);
            if (lexeme("������ɂȂ�ł��傤"))             return set_info(info, T_IKURA);
            if (lexeme("������ɂȂ�"))                     return set_info(info, T_IKURA);
            if (lexeme("������ɂȂ�܂�"))                 return set_info(info, T_IKURA);
            if (lexeme("������ł�"))                       return set_info(info, T_IKURA);
            if (lexeme("������ł�"))                       return set_info(info, T_IKURA);
            if (lexeme("������ł���"))                     return set_info(info, T_IKURA);
            if (lexeme("�����炾�낤"))                     return set_info(info, T_IKURA);
            if (lexeme("������"))                           return set_info(info, T_IKURA);
            if (lexeme("������"))                         return set_info(info, T_IKURA);
            if (lexeme("�����ɂȂ�ł��傤"))             return set_info(info, T_IKURA);
            if (lexeme("�����ɂȂ�"))                     return set_info(info, T_IKURA);
            if (lexeme("�����ɂȂ�܂�"))                 return set_info(info, T_IKURA);
            if (lexeme("�����Ȃ�ł�"))                   return set_info(info, T_IKURA);
            if (lexeme("�����Ȃ�ł�"))                   return set_info(info, T_IKURA);
            if (lexeme("�����Ȃ�ł��傤"))               return set_info(info, T_IKURA);
            if (lexeme("�����Ȃ�"))                       return set_info(info, T_IKURA);
            if (lexeme("�����Ȃ̂ł�"))                   return set_info(info, T_IKURA);
            if (lexeme("�����Ȃ̂ł�"))                   return set_info(info, T_IKURA);
            if (lexeme("�����Ȃ̂ł��傤"))               return set_info(info, T_IKURA);
            if (lexeme("�����Ȃ�"))                       return set_info(info, T_IKURA);
            if (lexeme("�����ł���"))                     return set_info(info, T_IKURA);
            if (lexeme("�������낤"))                     return set_info(info, T_IKURA);
            if (lexeme("������"))                           return set_info(info, T_IKURA);
            if (lexeme("����"))                             return set_info(info, T_IO);
            if (lexeme("����"))                             return set_info(info, T_GO);
            if (lexeme("���[�����񂺂��"))                 return set_info(info, T_ATAN);
            if (lexeme("���[�����񂶂����"))               return set_info(info, T_ATAN);
            if (lexeme("���[��������"))                     return set_info(info, T_ASIN);
            if (lexeme("���[����������"))                   return set_info(info, T_ACOS);
            if (lexeme("���܂�"))                           return set_info(info, T_AMARI);
            if (lexeme("��������"))                         return set_info(info, T_ASOUGI);
            if (lexeme("�l"))                               return set_info(info, T_R_PAREN);
            if (lexeme("�k"))                               return set_info(info, T_L_PAREN);
            if (lexeme("�z"))                               return set_info(info, T_R_PAREN);
            if (lexeme("�y"))                               return set_info(info, T_L_PAREN);
            if (lexeme("�x"))                               return set_info(info, T_R_PAREN);
            if (lexeme("�w"))                               return set_info(info, T_L_PAREN);
            if (lexeme("�v"))                               return set_info(info, T_R_PAREN);
            if (lexeme("�u"))                               return set_info(info, T_L_PAREN);
            if (lexeme("�t"))                               return set_info(info, T_R_PAREN);
            if (lexeme("�s"))                               return set_info(info, T_L_PAREN);
            if (lexeme("�r"))                               return set_info(info, T_R_PAREN);
            if (lexeme("�q"))                               return set_info(info, T_L_PAREN);
            if (lexeme("�Z"))                               return set_info(info, T_ZERO);
            if (lexeme("�B"))                               return set_info(info, T_PERIOD);
            if (lexeme("�A"))                               return set_info(info, T_COMMA);
            if (lexeme("��"))                               return set_info(info, T_PERIOD);
            if (lexeme("��"))                               return set_info(info, T_PERIOD);
            if (lexeme("��"))                               return set_info(info, T_PERIOD);
            if (lexeme("��"))                               return set_info(info, T_PERIOD);
            if (lexeme("��"))                               return set_info(info, T_WARU);
            if (lexeme("�~"))                               return set_info(info, T_KAKERU);
            if (lexeme("~"))                                return set_info(info, T_KARA);
            if (lexeme("}"))                                return set_info(info, T_R_PAREN);
            if (lexeme("{"))                                return set_info(info, T_L_PAREN);
            if (lexeme("tanh"))                             return set_info(info, T_TANH);
            if (lexeme("tan"))                              return set_info(info, T_TAN);
            if (lexeme("sinh"))                             return set_info(info, T_SINH);
            if (lexeme("sin"))                              return set_info(info, T_SIN);
            if (lexeme("log10"))                            return set_info(info, T_LOG10);
            if (lexeme("log"))                              return set_info(info, T_LOG);
            if (lexeme("ln"))                               return set_info(info, T_LOG);
            if (lexeme("fabs"))                             return set_info(info, T_ZETTAICHI);
            if (lexeme("exp"))                              return set_info(info, T_EXP);
            if (lexeme("cosh"))                             return set_info(info, T_COSH);
            if (lexeme("cos"))                              return set_info(info, T_COS);
            if (lexeme("atan"))                             return set_info(info, T_ATAN);
            if (lexeme("asin"))                             return set_info(info, T_ASIN);
            if (lexeme("arctan"))                           return set_info(info, T_ATAN);
            if (lexeme("arcsin"))                           return set_info(info, T_ASIN);
            if (lexeme("arccos"))                           return set_info(info, T_ACOS);
            if (lexeme("acos"))                             return set_info(info, T_ACOS);
            if (lexeme("abs"))                              return set_info(info, T_ZETTAICHI);
            if (lexeme("]"))                                return set_info(info, T_R_PAREN);
            if (lexeme("["))                                return set_info(info, T_L_PAREN);
            if (lexeme("?"))                                return set_info(info, T_PERIOD);
            if (lexeme("=="))                               return set_info(info, T_HA);
            if (lexeme("="))                                return set_info(info, T_HA);
            if (lexeme("/"))                                return set_info(info, T_WARU);
            if (lexeme("."))                                return set_info(info, T_PERIOD);
            if (lexeme("-"))                                return set_info(info, T_MINUS);
            if (lexeme(","))                                return set_info(info, T_COMMA);
            if (lexeme("+"))                                return set_info(info, T_TASU);
            if (lexeme("*"))                                return set_info(info, T_KAKERU);
            if (lexeme(")"))                                return set_info(info, T_R_PAREN);
            if (lexeme("("))                                return set_info(info, T_L_PAREN);
            if (lexeme("%"))                                return set_info(info, T_PERCENT);
            if (lexeme("!"))                                return set_info(info, T_FACTORIAL);
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
                m_saved_str.assign(m_buff.begin(), m_buff.begin() + m_index);
                m_buff.erase(m_buff.begin(), m_buff.begin() + m_index);
                m_index = 0;
            }
        }

        Token commit_token(Token token)
        {
            commit();
            return token;
        }

        Token set_info(info_type& info, Token token)
        {
            info.text() = m_saved_str;
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
        std::string             m_saved_str;

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

        // �̂̐��̐������������ɕϊ�����B
        // �u�������v�u�Ђ������v�Ȃǂ��u���������v�u������邩���v�Ȃǂɕϊ�����B
        void resynth0(std::vector<info_type>& infos)
        {
            std::vector<info_type> newinfos;
            std::vector<info_type>::iterator it = infos.begin();
            std::vector<info_type>::iterator end = infos.end();
            for (; it != end; ++it)
            {
                switch (it->get_token())
                {
                case T_HATA:
                    it->set_token(T_NI);
                    newinfos.push_back(*it);
                    it->set_token(T_JUU);
                    newinfos.push_back(*it);
                    break;

                case T_YAO:
                    it->set_token(T_HACHI);
                    newinfos.push_back(*it);
                    it->set_token(T_HYAKU);
                    newinfos.push_back(*it);
                    break;

                case T_IO:
                    it->set_token(T_GO);
                    newinfos.push_back(*it);
                    it->set_token(T_HYAKU);
                    newinfos.push_back(*it);
                    break;

                case T_ISO:
                    it->set_token(T_GO);
                    newinfos.push_back(*it);
                    it->set_token(T_JUU);
                    newinfos.push_back(*it);
                    break;

                case T_YOSO:
                    it->set_token(T_YON);
                    newinfos.push_back(*it);
                    it->set_token(T_JUU);
                    newinfos.push_back(*it);
                    break;

                case T_KANA:
                    if (it->text() == "��")
                    {
                        if ((it + 1)->get_token() == T_JUU ||
                            (it + 1)->get_token() == T_MAN)
                        {
                            it->set_token(T_HACHI);
                        }
                    }
                    newinfos.push_back(*it);
                    break;

                case T_KASUU:
                    it->set_token(T_TASU);
                    newinfos.push_back(*it);
                    it->set_token(T_MONO);
                    newinfos.push_back(*it);
                    break;

                case T_HIKASUU:
                    it->set_token(T_TASARERU);
                    newinfos.push_back(*it);
                    it->set_token(T_MONO);
                    newinfos.push_back(*it);
                    break;

                case T_GENSUU:
                    it->set_token(T_HIKU);
                    newinfos.push_back(*it);
                    it->set_token(T_MONO);
                    newinfos.push_back(*it);
                    break;

                case T_HIGENSUU:
                    it->set_token(T_HIKARERU);
                    newinfos.push_back(*it);
                    it->set_token(T_MONO);
                    newinfos.push_back(*it);
                    break;

                case T_JOUSUU:
                    it->set_token(T_KAKERU);
                    newinfos.push_back(*it);
                    it->set_token(T_MONO);
                    newinfos.push_back(*it);
                    break;

                case T_HIJOUSUU:
                    it->set_token(T_KAKERARERU);
                    newinfos.push_back(*it);
                    it->set_token(T_MONO);
                    newinfos.push_back(*it);
                    break;

                case T_JOSUU:
                    it->set_token(T_WARU);
                    newinfos.push_back(*it);
                    it->set_token(T_MONO);
                    newinfos.push_back(*it);
                    break;

                case T_HIJOSUU:
                    it->set_token(T_WARARERU);
                    newinfos.push_back(*it);
                    it->set_token(T_MONO);
                    newinfos.push_back(*it);
                    break;

                default:
                    newinfos.push_back(*it);
                }
            }
            infos = newinfos;
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
        // T_NO2: �u��v�u���v�u�����v�u���傤�v�́u�́v�B
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

                case T_COUNT:
                case T_SUM:
                case T_DIFF:
                case T_PROD:
                case T_QUOT:
                case T_MAX:
                case T_MIN:
                case T_AVERAGE:
                case T_HEIHOUKON:
                    if (no1 != std::string::npos)
                    {
                        (newinfos.begin() + no1)->set_token(T_NO2);
                        no1 = std::string::npos;
                    }
                    jou1 = std::string::npos;
                    break;

                case T_KAIJOU:
                    if (no1 != std::string::npos)
                    {
                        (newinfos.begin() + no1)->set_token(T_NO6);
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

        // ����̌��̒����T_COMMA���폜����B
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
                case T_DIFF:
                case T_HA:
                case T_HEIHOU:
                case T_HEIHOUKON:
                case T_KAIJOU:
                case T_HIITE:
                case T_HIKIZAN:
                case T_HIKU:
                case T_HIKUTO:
                case T_IKURA:
                case T_KAKERU:
                case T_KAKERUTO:
                case T_KAKETE:
                case T_KAKEZAN:
                case T_KANA:
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
                case T_NOUCHI:
                case T_OSHIETE:
                case T_PERCENT:
                case T_PLUS:
                case T_PROD:
                case T_QUOT:
                case T_SHITA:
                case T_SHITE:
                case T_SUM:
                case T_SURUTO:
                case T_TASHITE:
                case T_TASHIZAN:
                case T_TASU:
                case T_TASUTO:
                case T_TO1:
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
                    if (flag && it->text() == "��")
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
                else if (it->get_token() == T_TASU &&
                         (it + 1)->get_token() == T_PERIOD)
                {
                    it->set_token(T_TASUTO);
                    (it + 1)->set_token(T_COMMA);
                }
                else if (it->get_token() == T_HIKU &&
                         (it + 1)->get_token() == T_PERIOD)
                {
                    it->set_token(T_HIKUTO);
                    (it + 1)->set_token(T_COMMA);
                }
                else if (it->get_token() == T_KAKERU &&
                         (it + 1)->get_token() == T_PERIOD)
                {
                    it->set_token(T_KAKERUTO);
                    (it + 1)->set_token(T_COMMA);
                }
                else if (it->get_token() == T_WARU &&
                         (it + 1)->get_token() == T_PERIOD)
                {
                    it->set_token(T_WARUTO);
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
                    case T_DIFF:
                    case T_HEIHOUKON:
                    case T_KAIJOU:
                    case T_HIITA:
                    case T_HIITE:
                    case T_HIKIZAN:
                    case T_HIKU:
                    case T_HIKUTO:
                    case T_IKURA:
                    case T_KAKERU:
                    case T_KAKERUTO:
                    case T_KAKETA:
                    case T_KAKETE:
                    case T_KAKEZAN:
                    case T_KANA:
                    case T_KARA:
                    case T_KEISAN:
                    case T_KOTAE:
                    case T_NOUCHI:
                    case T_ONEGAI:
                    case T_OSHIETE:
                    case T_PERCENT:
                    case T_PROD:
                    case T_QUOT:
                    case T_R_PAREN:
                    case T_SHITA:
                    case T_SHITE:
                    case T_SUM:
                    case T_SURUTO:
                    case T_TASHITA:
                    case T_TASHITE:
                    case T_TASHIZAN:
                    case T_TASU:
                    case T_TASUTO:
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

        // �����́u���v���u���ȁv�ɂ���B
        void resynth13(std::vector<info_type>& infos)
        {
            std::vector<info_type>::iterator it = infos.begin();
            std::vector<info_type>::iterator end = infos.end();
            for (; it != end; ++it)
            {
                switch (it->get_token())
                {
                case T_IKURA:
                case T_WAKARAN:
                case T_OSHIETE:
                case T_SURUTO:
                case T_TASUTO:
                case T_KAKERUTO:
                case T_HIKUTO:
                case T_WARUTO:
                case T_WARIKIRU:
                case T_WARIKIRENAI:
                    if ((it + 1)->get_token() == T_KA)
                        (it + 1)->set_token(T_KANA);
                    break;

                default:
                    break;
                }
            }
        }

        // �����́u�����v�u�Ђ��v�u������v�u���v��
        // �u�����Ɓv�u�Ђ��Ɓv�u������Ɓv�u���Ɓv�ɂ���B
        void resynth14(std::vector<info_type>& infos)
        {
            std::vector<info_type>::iterator it = infos.begin();
            std::vector<info_type>::iterator end = infos.end();
            for (; it != end; ++it)
            {
                switch (it->get_token())
                {
                case T_TASU:
                    if ((it + 1)->get_token() == T_PERIOD)
                        it->set_token(T_TASUTO);
                    break;

                case T_HIKU:
                    if ((it + 1)->get_token() == T_PERIOD)
                        it->set_token(T_HIKUTO);
                    break;

                case T_KAKERU:
                    if ((it + 1)->get_token() == T_PERIOD)
                        it->set_token(T_KAKERUTO);
                    break;

                case T_WARU:
                    if ((it + 1)->get_token() == T_PERIOD)
                        it->set_token(T_WARUTO);
                    break;

                case T_SURU:
                    if ((it + 1)->get_token() == T_PERIOD)
                        it->set_token(T_SURUTO);
                    break;

                default:
                    break;
                }
            }
        }

        // ���̒��ł͂Ȃ��A���͂̒��́u�̂������傤�v�́u�́v(T_NO6)��T_NO4�ɂ���B
        void resynth15(std::vector<info_type>& infos)
        {
            std::vector<info_type>::iterator it = infos.begin();
            std::vector<info_type>::iterator end = infos.end();
            for (; it != end; ++it)
            {
                switch (it->get_token())
                {
                case T_ACOS:
                case T_AMARI:
                case T_ASIN:
                case T_ATAN:
                case T_AVERAGE:
                case T_BAI:
                case T_COS:
                case T_COUNT:
                case T_DIFF:
                case T_EXP:
                case T_GYAKUSUU:
                case T_HEIHOU:
                case T_HEIHOUKON:
                case T_HIKIZAN:
                case T_KAIJOU:
                case T_KAKEZAN:
                case T_KEISAN:
                case T_KOTAE:
                case T_LOG:
                case T_LOG10:
                case T_MAX:
                case T_MIN:
                case T_MONO:
                case T_PROD:
                case T_QUOT:
                case T_RIPPOU:
                case T_ROOT:
                case T_SIN:
                case T_SINH:
                case T_SUM:
                case T_TAN:
                case T_TASHIZAN:
                case T_WARIZAN:
                case T_ZETTAICHI:
                    if ((it + 1)->get_token() == T_NO6 &&
                        (it + 2)->get_token() == T_KAIJOU)
                    {
                        (it + 1)->set_token(T_NO4);
                    }
                    break;

                default:
                    break;
                }
            }
        }

        // T_TASUTO, T_HIKUTO, T_KAKERUTO, T_WARUTO�̌��T_SURUTO����菜���B
        void resynth16(std::vector<info_type>& infos)
        {
            std::vector<info_type> newinfos;
            std::vector<info_type>::iterator it = infos.begin();
            std::vector<info_type>::iterator end = infos.end();
            for (; it != end; ++it)
            {
                switch (it->get_token())
                {
                case T_TASUTO:
                case T_HIKUTO:
                case T_KAKERUTO:
                case T_WARUTO:
                    newinfos.push_back(*it);
                    if ((it + 1)->get_token() == T_SURUTO)
                        it++;
                    break;

                default:
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
