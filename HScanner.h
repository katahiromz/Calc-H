////////////////////////////////////////////////////////////////////////////
// HScanner.h -- Hiragana Calculator Calc-H
// This file is part of Calc-H.  See file "ReadMe.txt" and "License.txt".
////////////////////////////////////////////////////////////////////////////
// (Japanese, Shift_JIS)
// Linuxの場合は、UTF-8に変換して下さい。

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
        }

        void resynth(std::vector<info_type>& infos)
        {
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
                resynth17(infos);
                resynth18(infos);
                resynth19(infos);
                resynth20(infos);
                resynth21(infos);
                resynth22(infos);
                resynth23(infos);
                resynth24(infos);
                resynth25(infos);
                resynth26(infos);
                resynth27(infos);
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
                std::cout << token_to_string(*it) << " ";
            }
            std::cout << std::endl;
        }

        Token get_token(ChTokenInfo& info)
        {
            int space_count = 0;
            while (lexeme(" ") || lexeme("　") || lexeme("\t"))
            {
                space_count++;
            }

            info.location() = location();

            if (space_count >= 2)
                return commit_token(T_COMMA);

            // arabic numeral
            std::string strNum;
            if (lexeme("0") || lexeme("1") || lexeme("2") || lexeme("3") || lexeme("4") ||
                lexeme("5") || lexeme("6") || lexeme("7") || lexeme("8") || lexeme("9") ||
                lexeme("０") || lexeme("１") || lexeme("２") || lexeme("３") || lexeme("４") ||
                lexeme("５") || lexeme("６") || lexeme("７") || lexeme("８") || lexeme("９"))
            {
                bool has_dot = false;
                strNum = m_saved_str;
                while (lexeme("0") || lexeme("1") || lexeme("2") || lexeme("3") || lexeme("4") ||
                       lexeme("5") || lexeme("6") || lexeme("7") || lexeme("8") || lexeme("9") ||
                       lexeme("０") || lexeme("１") || lexeme("２") || lexeme("３") || lexeme("４") ||
                       lexeme("５") || lexeme("６") || lexeme("７") || lexeme("８") || lexeme("９") ||
                       (!has_dot && (lexeme(".") || lexeme("．"))))
                {
                    strNum += m_saved_str;
                    if (m_saved_str == "." || m_saved_str == "．")
                    {
                        has_dot = true;
                    }
                }
                ChReplaceString(strNum, "０", "0");
                ChReplaceString(strNum, "１", "1");
                ChReplaceString(strNum, "２", "2");
                ChReplaceString(strNum, "３", "3");
                ChReplaceString(strNum, "４", "4");
                ChReplaceString(strNum, "５", "5");
                ChReplaceString(strNum, "６", "6");
                ChReplaceString(strNum, "７", "7");
                ChReplaceString(strNum, "８", "8");
                ChReplaceString(strNum, "９", "9");
                ChReplaceString(strNum, "．", ".");
                m_saved_str = strNum;
                return set_info(info, T_NUMBER);
            }

            if (lexeme("\n"))
            {
                newline();
                return set_info(info, T_PERIOD);
            }

            if (!lexeme("にいち", false) && !lexeme("にいじょう", false) &&
                !lexeme("にいか", false) && lexeme("にい"))
            {
                return set_info(info, T_NI);
            }
            if (!lexeme("をなな", false) && !lexeme("をなゆ", false) &&
                !lexeme("をなお", false) &&lexeme("をな")) return set_info(info, T_WO1);
            if (!lexeme("になな", false) && !lexeme("になり", false) &&
                !lexeme("になる", false) && lexeme("にな")) return set_info(info, T_NI);
            if (!lexeme("のなな", false) && lexeme("のな")) return set_info(info, T_NO1);
            if (!lexeme("いってん", false) &&
                (lexeme("いってみ") || lexeme("いって"))) return set_info(info, T_OSHIETE);

            if (!lexeme("かいち", false) && !lexeme("かいう", false) && !lexeme("かいな", false) &&
                !lexeme("かいい", false) && !lexeme("かいじ", false) && lexeme("かい"))
            {
                return set_info(info, T_KANA);
            }

            if (!lexeme("のやくすう", false) && lexeme("のや"))
            {
                return set_info(info, T_KANA);
            }

            if (!lexeme("がいち", false) && !lexeme("がいっ", false) && !lexeme("がいく", false) &&
                !lexeme("がいお", false) && !lexeme("がいそ", false) &&  lexeme("がい"))
                return set_info(info, T_GAI);

            // 最長一致法。
            // 【ここから】行を降順に並び替えておく。
            if (lexeme("～"))                               return set_info(info, T_KARA1);
            if (lexeme("｝"))                               return set_info(info, T_R_PAREN);
            if (lexeme("｛"))                               return set_info(info, T_L_PAREN);
            if (lexeme("ｔａｎｈ"))                         return set_info(info, T_TANH);
            if (lexeme("ｔａｎ"))                           return set_info(info, T_TAN);
            if (lexeme("ｓｉｎｈ"))                         return set_info(info, T_SINH);
            if (lexeme("ｓｉｎ"))                           return set_info(info, T_SIN);
            if (lexeme("ｌｏｇ１０"))                       return set_info(info, T_LOG10);
            if (lexeme("ｌｏｇ"))                           return set_info(info, T_LOG);
            if (lexeme("ｌｎ"))                             return set_info(info, T_LOG);
            if (lexeme("ｌｃｍ"))                           return set_info(info, T_LCM);
            if (lexeme("ｇｃｄ"))                           return set_info(info, T_GCD);
            if (lexeme("ｆａｂｓ"))                         return set_info(info, T_ZETTAICHI);
            if (lexeme("ｅｘｐ"))                           return set_info(info, T_EXP);
            if (lexeme("ｅ"))                               return set_info(info, T_E);
            if (lexeme("ｃｏｓｈ"))                         return set_info(info, T_COSH);
            if (lexeme("ｃｏｓ"))                           return set_info(info, T_COS);
            if (lexeme("ａｔａｎ"))                         return set_info(info, T_ATAN);
            if (lexeme("ａｓｉｎ"))                         return set_info(info, T_ASIN);
            if (lexeme("ａｒｃｔａｎ"))                     return set_info(info, T_ATAN);
            if (lexeme("ａｒｃｓｉｎ"))                     return set_info(info, T_ASIN);
            if (lexeme("ａｒｃｃｏｓ"))                     return set_info(info, T_ACOS);
            if (lexeme("ａｃｏｓ"))                         return set_info(info, T_ACOS);
            if (lexeme("ａｂｓ"))                           return set_info(info, T_ZETTAICHI);
            if (lexeme("＾"))                               return set_info(info, T_POWER);
            if (lexeme("］"))                               return set_info(info, T_R_PAREN);
            if (lexeme("［"))                               return set_info(info, T_L_PAREN);
            if (lexeme("ＬＣＭ"))                           return set_info(info, T_LCM);
            if (lexeme("ＧＣＤ"))                           return set_info(info, T_GCD);
            if (lexeme("？"))                               return set_info(info, T_PERIOD);
            if (lexeme("＝"))                               return set_info(info, T_HA);
            if (lexeme("．"))                               return set_info(info, T_PERIOD);
            if (lexeme("－"))                               return set_info(info, T_MINUS);
            if (lexeme("，"))                               return set_info(info, T_COMMA);
            if (lexeme("＋"))                               return set_info(info, T_TASU);
            if (lexeme("＊＊"))                             return set_info(info, T_POWER);
            if (lexeme("）"))                               return set_info(info, T_R_PAREN);
            if (lexeme("（"))                               return set_info(info, T_L_PAREN);
            if (lexeme("％"))                               return set_info(info, T_PERCENT);
            if (lexeme("！"))                               return set_info(info, T_FACTORIAL);
            if (lexeme("零"))                               return set_info(info, T_ZERO);
            if (lexeme("陸"))                               return set_info(info, T_ROKU);
            if (lexeme("阿僧祗"))                           return set_info(info, T_ASOUGI);
            if (lexeme("阡"))                               return set_info(info, T_SEN);
            if (lexeme("那由多"))                           return set_info(info, T_NAYUTA);
            if (lexeme("那由他"))                           return set_info(info, T_NAYUTA);
            if (lexeme("載"))                               return set_info(info, T_SAI);
            if (lexeme("肆"))                               return set_info(info, T_YON);
            if (lexeme("穣"))                               return set_info(info, T_JOU2);
            if (lexeme("百"))                               return set_info(info, T_HYAKU);
            if (lexeme("玖"))                               return set_info(info, T_KYUU);
            if (lexeme("無量大数"))                         return set_info(info, T_MURYOUTAISUU);
            if (lexeme("澗"))                               return set_info(info, T_KAN);
            if (lexeme("漆"))                               return set_info(info, T_NANA);
            if (lexeme("溝"))                               return set_info(info, T_KOU);
            if (lexeme("正"))                               return set_info(info, T_SEI);
            if (lexeme("極"))                               return set_info(info, T_GOKU);
            if (lexeme("拾"))                               return set_info(info, T_JUU);
            if (lexeme("恒河沙"))                           return set_info(info, T_GOUGASHA);
            if (lexeme("弐"))                               return set_info(info, T_NI);
            if (lexeme("廿"))                               return set_info(info, T_HATA);
            if (lexeme("壱"))                               return set_info(info, T_ICHI);
            if (lexeme("垓"))                               return set_info(info, T_GAI);
            if (lexeme("四"))                               return set_info(info, T_YON);
            if (lexeme("参"))                               return set_info(info, T_SAN);
            if (lexeme("千"))                               return set_info(info, T_SEN);
            if (lexeme("十"))                               return set_info(info, T_JUU);
            if (lexeme("六"))                               return set_info(info, T_ROKU);
            if (lexeme("八"))                               return set_info(info, T_HACHI);
            if (lexeme("兆"))                               return set_info(info, T_CHOU);
            if (lexeme("億"))                               return set_info(info, T_OKU);
            if (lexeme("伍"))                               return set_info(info, T_GO);
            if (lexeme("仟"))                               return set_info(info, T_SEN);
            if (lexeme("京"))                               return set_info(info, T_KEI);
            if (lexeme("五"))                               return set_info(info, T_GO);
            if (lexeme("二"))                               return set_info(info, T_NI);
            if (lexeme("九"))                               return set_info(info, T_KYUU);
            if (lexeme("不可思議"))                         return set_info(info, T_FUKASHIGI);
            if (lexeme("三"))                               return set_info(info, T_SAN);
            if (lexeme("万"))                               return set_info(info, T_MAN);
            if (lexeme("七"))                               return set_info(info, T_NANA);
            if (lexeme("一"))                               return set_info(info, T_ICHI);
            if (lexeme("・"))                               return set_info(info, T_DOT);
            if (lexeme("んや"))                             return set_info(info, T_KANA);
            if (lexeme("をわることの"))                     return set_info(info, T_WARU);
            if (lexeme("をひくことの"))                     return set_info(info, T_HIKU);
            if (lexeme("をね"))                             return set_info(info, T_WO1);
            if (lexeme("を"))                               return set_info(info, T_WO1);
            if (lexeme("われば"))                           return set_info(info, T_WARUTO);
            if (lexeme("われ"))                             return set_info(info, T_WARUTO);
            if (lexeme("わるときの"))                       return set_info(info, T_WARUTO);
            if (lexeme("わるとき"))                         return set_info(info, T_WARUTO);
            if (lexeme("わると"))                           return set_info(info, T_WARUTO);
            if (lexeme("わることの"))                       return set_info(info, T_WARU);
            if (lexeme("わる"))                             return set_info(info, T_WARU);
            if (lexeme("わります"))                         return set_info(info, T_WARU);
            if (lexeme("わりまし"))                         return set_info(info, T_WARIMASHI);
            if (lexeme("わりびき"))                         return set_info(info, T_WARIBIKI);
            if (lexeme("わりなさい"))                       return set_info(info, T_WARUTO);
            if (lexeme("わりざん"))                         return set_info(info, T_WARIZAN);
            if (lexeme("わりきれるん"))                     return set_info(info, T_WARIKIRU);
            if (lexeme("わりきれるの"))                     return set_info(info, T_WARIKIRU);
            if (lexeme("わりきれる"))                       return set_info(info, T_WARIKIRU);
            if (lexeme("わりきれます"))                     return set_info(info, T_WARIKIRU);
            if (lexeme("わりきるん"))                       return set_info(info, T_WARIKIRU);
            if (lexeme("わりきるの"))                       return set_info(info, T_WARIKIRU);
            if (lexeme("わりきることができるの"))           return set_info(info, T_WARIKIRU);
            if (lexeme("わりきることができる"))             return set_info(info, T_WARIKIRU);
            if (lexeme("わりきることができます"))           return set_info(info, T_WARIKIRU);
            if (lexeme("わりきる"))                         return set_info(info, T_WARIKIRU);
            if (lexeme("わり"))                             return set_info(info, T_WATTE);
            if (lexeme("わられる"))                         return set_info(info, T_WARARERU);
            if (lexeme("わってみー"))                       return set_info(info, T_WARUTO);
            if (lexeme("わってみよ"))                       return set_info(info, T_WARUTO);
            if (lexeme("わってみい"))                       return set_info(info, T_WARUTO);
            if (lexeme("わってみぃ"))                       return set_info(info, T_WARUTO);
            if (lexeme("わってみ"))                         return set_info(info, T_WARUTO);
            if (lexeme("わって"))                           return set_info(info, T_WATTE);
            if (lexeme("わったら"))                         return set_info(info, T_WARUTO);
            if (lexeme("わったときの"))                     return set_info(info, T_WARUTO);
            if (lexeme("わったとき"))                       return set_info(info, T_WARUTO);
            if (lexeme("わった"))                           return set_info(info, T_WATTA);
            if (lexeme("わかるん"))                         return set_info(info, T_WAKARAN);
            if (lexeme("わかるの"))                         return set_info(info, T_WAKARAN);
            if (lexeme("わかる"))                           return set_info(info, T_WAKARAN);
            if (lexeme("わかりません"))                     return set_info(info, T_WAKARAN);
            if (lexeme("わかります"))                       return set_info(info, T_WAKARAN);
            if (lexeme("わからんの"))                       return set_info(info, T_WAKARAN);
            if (lexeme("わからんと"))                       return set_info(info, T_WAKARAN);
            if (lexeme("わからん"))                         return set_info(info, T_WAKARAN);
            if (lexeme("わからへんの"))                     return set_info(info, T_WAKARAN);
            if (lexeme("わからへん"))                       return set_info(info, T_WAKARAN);
            if (lexeme("わからねーの"))                     return set_info(info, T_WAKARAN);
            if (lexeme("わからねー"))                       return set_info(info, T_WAKARAN);
            if (lexeme("わからないん"))                     return set_info(info, T_WAKARAN);
            if (lexeme("わからないの"))                     return set_info(info, T_WAKARAN);
            if (lexeme("わからない"))                       return set_info(info, T_WAKARAN);
            if (lexeme("わかったぞー"))                     return set_info(info, T_ETTO);
            if (lexeme("わかったぞお"))                     return set_info(info, T_ETTO);
            if (lexeme("わかったぞう"))                     return set_info(info, T_ETTO);
            if (lexeme("わかったぞ"))                       return set_info(info, T_ETTO);
            if (lexeme("わかった"))                         return set_info(info, T_ETTO);
            if (lexeme("わ"))                               return set_info(info, T_SUM);
            if (lexeme("ろっ"))                             return set_info(info, T_ROKU);
            if (lexeme("ろぐ"))                             return set_info(info, T_LOG);
            if (lexeme("ろく"))                             return set_info(info, T_ROKU);
            if (lexeme("れー"))                             return set_info(info, T_ZERO);
            if (lexeme("れい"))                             return set_info(info, T_ZERO);
            if (lexeme("るーと"))                           return set_info(info, T_ROOT);
            if (lexeme("るうと"))                           return set_info(info, T_ROOT);
            if (lexeme("りょーかい"))                       return set_info(info, T_ETTO);
            if (lexeme("りょうかい"))                       return set_info(info, T_ETTO);
            if (lexeme("りっぽー"))                         return set_info(info, T_RIPPOU);
            if (lexeme("りっぽう"))                         return set_info(info, T_RIPPOU);
            if (lexeme("よー"))                             return set_info(info, T_KANA);
            if (lexeme("よん"))                             return set_info(info, T_YON);
            if (lexeme("よろず"))                           return set_info(info, T_MAN);
            if (lexeme("より"))                             return set_info(info, T_YORI);
            if (lexeme("よね"))                             return set_info(info, T_KANA);
            if (lexeme("よっ"))                             return set_info(info, T_YON);
            if (lexeme("よそ"))                             return set_info(info, T_YOSO);
            if (lexeme("よげん"))                           return set_info(info, T_COS);
            if (lexeme("よお"))                             return set_info(info, T_KANA);
            if (lexeme("よぉ"))                             return set_info(info, T_KANA);
            if (lexeme("よう"))                             return set_info(info, T_KANA);
            if (lexeme("よぅ"))                             return set_info(info, T_KANA);
            if (lexeme("よ"))                               return set_info(info, T_KANA);
            if (lexeme("ゆーて"))                           return set_info(info, T_OSHIETE);
            if (lexeme("ゆってみー"))                       return set_info(info, T_OSHIETE);
            if (lexeme("ゆってみい"))                       return set_info(info, T_OSHIETE);
            if (lexeme("ゆってみぃ"))                       return set_info(info, T_OSHIETE);
            if (lexeme("ゆってみ"))                         return set_info(info, T_OSHIETE);
            if (lexeme("ゆって"))                           return set_info(info, T_OSHIETE);
            if (lexeme("ゆうて"))                           return set_info(info, T_OSHIETE);
            if (lexeme("やー"))                             return set_info(info, T_HACHI);
            if (lexeme("やろか"))                           return set_info(info, T_KANA);
            if (lexeme("やろ"))                             return set_info(info, T_KANA);
            if (lexeme("やれば"))                           return set_info(info, T_SURUTO);
            if (lexeme("やれ"))                             return set_info(info, T_SURUTO);
            if (lexeme("やると"))                           return set_info(info, T_SURUTO);
            if (lexeme("やりなさい"))                       return set_info(info, T_SURUTO);
            if (lexeme("やねん"))                           return set_info(info, T_KANA);
            if (lexeme("やね"))                             return set_info(info, T_KANA);
            if (lexeme("やってみー"))                       return set_info(info, T_SURUTO);
            if (lexeme("やってみろ"))                       return set_info(info, T_SURUTO);
            if (lexeme("やってみれば"))                     return set_info(info, T_SURUTO);
            if (lexeme("やってみるとき"))                   return set_info(info, T_SURUTO);
            if (lexeme("やってみると"))                     return set_info(info, T_SURUTO);
            if (lexeme("やってみよ"))                       return set_info(info, T_SURUTO);
            if (lexeme("やってみたら"))                     return set_info(info, T_SURUTO);
            if (lexeme("やってみたとき"))                   return set_info(info, T_SURUTO);
            if (lexeme("やってみい"))                       return set_info(info, T_SURUTO);
            if (lexeme("やってみぃ"))                       return set_info(info, T_SURUTO);
            if (lexeme("やってみ"))                         return set_info(info, T_SURUTO);
            if (lexeme("やって"))                           return set_info(info, T_SHITE);
            if (lexeme("やったら"))                         return set_info(info, T_SURUTO);
            if (lexeme("やったとき"))                       return set_info(info, T_SURUTO);
            if (lexeme("やった"))                           return set_info(info, T_SHITA);
            if (lexeme("やっ"))                             return set_info(info, T_HACHI);
            if (lexeme("やくすー"))                         return set_info(info, T_YAKUSUU);
            if (lexeme("やくすう"))                         return set_info(info, T_YAKUSUU);
            if (lexeme("やお"))                             return set_info(info, T_YAO);
            if (lexeme("やあ"))                             return set_info(info, T_HACHI);
            if (lexeme("やぁ"))                             return set_info(info, T_HACHI);
            if (lexeme("や"))                               return set_info(info, T_KANA);
            if (lexeme("もんだいやで"))                     return set_info(info, T_ETTO);
            if (lexeme("もんだいや"))                       return set_info(info, T_ETTO);
            if (lexeme("もんだいです"))                     return set_info(info, T_ETTO);
            if (lexeme("もんだいがあります"))               return set_info(info, T_ETTO);
            if (lexeme("もんだい"))                         return set_info(info, T_ETTO);
            if (lexeme("もも"))                             return set_info(info, T_HYAKU);
            if (lexeme("もの"))                             return set_info(info, T_MONO);
            if (lexeme("もとめると"))                       return set_info(info, T_SURUTO);
            if (lexeme("もとめよ"))                         return set_info(info, T_SURUTO);
            if (lexeme("もとめなさい"))                     return set_info(info, T_SURUTO);
            if (lexeme("もとめてみー"))                     return set_info(info, T_SURUTO);
            if (lexeme("もとめてみよ"))                     return set_info(info, T_SURUTO);
            if (lexeme("もとめてみい"))                     return set_info(info, T_SURUTO);
            if (lexeme("もとめてみぃ"))                     return set_info(info, T_SURUTO);
            if (lexeme("もとめてみ"))                       return set_info(info, T_SURUTO);
            if (lexeme("もとめて"))                         return set_info(info, T_SHITE);
            if (lexeme("もとめたら"))                       return set_info(info, T_SURUTO);
            if (lexeme("もとめた"))                         return set_info(info, T_SHITA);
            if (lexeme("もとめ"))                           return set_info(info, T_SHITE);
            if (lexeme("もしもし"))                         return set_info(info, T_ETTO);
            if (lexeme("も"))                               return set_info(info, T_WO1);
            if (lexeme("むー"))                             return set_info(info, T_ROKU);
            if (lexeme("むりょーたいすー"))                 return set_info(info, T_MURYOUTAISUU);
            if (lexeme("むりょーたいすう"))                 return set_info(info, T_MURYOUTAISUU);
            if (lexeme("むりょうたいすー"))                 return set_info(info, T_MURYOUTAISUU);
            if (lexeme("むりょうたいすう"))                 return set_info(info, T_MURYOUTAISUU);
            if (lexeme("むっ"))                             return set_info(info, T_ROKU);
            if (lexeme("むう"))                             return set_info(info, T_ROKU);
            if (lexeme("む"))                               return set_info(info, T_ROKU);
            if (lexeme("みー"))                             return set_info(info, T_KURE);
            if (lexeme("みんな"))                           return set_info(info, T_ALL);
            if (lexeme("みろ"))                             return set_info(info, T_KURE);
            if (lexeme("みれば"))                           return set_info(info, T_MIRUTO);
            if (lexeme("みると"))                           return set_info(info, T_MIRUTO);
            if (lexeme("みまん"))                           return set_info(info, T_MIMAN);
            if (lexeme("みっ"))                             return set_info(info, T_SAN);
            if (lexeme("みたまへ"))                         return set_info(info, T_KURE);
            if (lexeme("みたまえ"))                         return set_info(info, T_KURE);
            if (lexeme("みい"))                             return set_info(info, T_KURE);
            if (lexeme("みぃ"))                             return set_info(info, T_KURE);
            if (lexeme("み"))                               return set_info(info, T_KURE);
            if (lexeme("まん"))                             return set_info(info, T_MAN);
            if (lexeme("まで"))                             return set_info(info, T_MADE);
            if (lexeme("または"))                           return set_info(info, T_MATAHA);
            if (lexeme("まいなすするときの"))               return set_info(info, T_HIKUTO);
            if (lexeme("まいなすするとき"))                 return set_info(info, T_HIKUTO);
            if (lexeme("まいなすしろ"))                     return set_info(info, T_HIKUTO);
            if (lexeme("まいなすしなさい"))                 return set_info(info, T_HIKUTO);
            if (lexeme("まいなすしてみー"))                 return set_info(info, T_HIKUTO);
            if (lexeme("まいなすしてみよ"))                 return set_info(info, T_HIKUTO);
            if (lexeme("まいなすしてみい"))                 return set_info(info, T_HIKUTO);
            if (lexeme("まいなすしてみぃ"))                 return set_info(info, T_HIKUTO);
            if (lexeme("まいなすしてみ"))                   return set_info(info, T_HIKUTO);
            if (lexeme("まいなすして"))                     return set_info(info, T_HIITE);
            if (lexeme("まいなすしたら"))                   return set_info(info, T_HIKUTO);
            if (lexeme("まいなすしたとき"))                 return set_info(info, T_HIKUTO);
            if (lexeme("まいなすした"))                     return set_info(info, T_HIITA);
            if (lexeme("まいなすし"))                       return set_info(info, T_HIITE);
            if (lexeme("まいなすされる"))                   return set_info(info, T_HIKARERU);
            if (lexeme("まいなす"))                         return set_info(info, T_MINUS);
            if (lexeme("ほしー"))                           return set_info(info, T_KURE);
            if (lexeme("ほしい"))                           return set_info(info, T_KURE);
            if (lexeme("ほしぃ"))                           return set_info(info, T_KURE);
            if (lexeme("へーほーこん"))                     return set_info(info, T_HEIHOUKON);
            if (lexeme("へーほー"))                         return set_info(info, T_HEIHOU);
            if (lexeme("へーほうこん"))                     return set_info(info, T_HEIHOUKON);
            if (lexeme("へーほう"))                         return set_info(info, T_HEIHOU);
            if (lexeme("へーきんち"))                       return set_info(info, T_AVERAGE);
            if (lexeme("へーきん"))                         return set_info(info, T_AVERAGE);
            if (lexeme("へらせば"))                         return set_info(info, T_HIKUTO);
            if (lexeme("へらせ"))                           return set_info(info, T_HIKUTO);
            if (lexeme("へらすときの"))                     return set_info(info, T_HIKUTO);
            if (lexeme("へらすとき"))                       return set_info(info, T_HIKUTO);
            if (lexeme("へらすと"))                         return set_info(info, T_HIKUTO);
            if (lexeme("へらす"))                           return set_info(info, T_HIKU);
            if (lexeme("へらしなさい"))                     return set_info(info, T_HIKUTO);
            if (lexeme("へらしてみー"))                     return set_info(info, T_HIKUTO);
            if (lexeme("へらしてみよ"))                     return set_info(info, T_HIKUTO);
            if (lexeme("へらしてみい"))                     return set_info(info, T_HIKUTO);
            if (lexeme("へらしてみぃ"))                     return set_info(info, T_HIKUTO);
            if (lexeme("へらしてみ"))                       return set_info(info, T_HIKUTO);
            if (lexeme("へらして"))                         return set_info(info, T_HIITE);
            if (lexeme("へらしたら"))                       return set_info(info, T_HIKUTO);
            if (lexeme("へらしたときの"))                   return set_info(info, T_HIKUTO);
            if (lexeme("へらしたとき"))                     return set_info(info, T_HIKUTO);
            if (lexeme("へらした"))                         return set_info(info, T_HIITA);
            if (lexeme("へらし"))                           return set_info(info, T_HIITE);
            if (lexeme("へいほーこん"))                     return set_info(info, T_HEIHOUKON);
            if (lexeme("へいほー"))                         return set_info(info, T_HEIHOU);
            if (lexeme("へいほうこん"))                     return set_info(info, T_HEIHOUKON);
            if (lexeme("へいほう"))                         return set_info(info, T_HEIHOU);
            if (lexeme("へいきんち"))                       return set_info(info, T_AVERAGE);
            if (lexeme("へいきん"))                         return set_info(info, T_AVERAGE);
            if (lexeme("へ"))                               return set_info(info, T_NI);
            if (lexeme("ぷらすするときの"))                 return set_info(info, T_TASUTO);
            if (lexeme("ぷらすするとき"))                   return set_info(info, T_TASUTO);
            if (lexeme("ぷらすしろ"))                       return set_info(info, T_TASUTO);
            if (lexeme("ぷらすしなさい"))                   return set_info(info, T_TASUTO);
            if (lexeme("ぷらすしてみー"))                   return set_info(info, T_TASUTO);
            if (lexeme("ぷらすしてみよ"))                   return set_info(info, T_TASUTO);
            if (lexeme("ぷらすしてみい"))                   return set_info(info, T_TASUTO);
            if (lexeme("ぷらすしてみぃ"))                   return set_info(info, T_TASUTO);
            if (lexeme("ぷらすしてみ"))                     return set_info(info, T_TASUTO);
            if (lexeme("ぷらすして"))                       return set_info(info, T_TASHITE);
            if (lexeme("ぷらすしたら"))                     return set_info(info, T_TASUTO);
            if (lexeme("ぷらすしたとき"))                   return set_info(info, T_TASUTO);
            if (lexeme("ぷらすした"))                       return set_info(info, T_TASHITA);
            if (lexeme("ぷらすし"))                         return set_info(info, T_TASHITE);
            if (lexeme("ぷらすされる"))                     return set_info(info, T_TASARERU);
            if (lexeme("ぷらす"))                           return set_info(info, T_PLUS);
            if (lexeme("ぶんの"))                           return set_info(info, T_BUNNO);
            if (lexeme("ぶんすー"))                         return set_info(info, T_BUNSUU);
            if (lexeme("ぶんすう"))                         return set_info(info, T_BUNSUU);
            if (lexeme("ふやせば"))                         return set_info(info, T_TASUTO);
            if (lexeme("ふやせ"))                           return set_info(info, T_TASUTO);
            if (lexeme("ふやすとき"))                       return set_info(info, T_TASUTO);
            if (lexeme("ふやすと"))                         return set_info(info, T_TASUTO);
            if (lexeme("ふやす"))                           return set_info(info, T_TASU);
            if (lexeme("ふやしなさいよ"))                   return set_info(info, T_TASUTO);
            if (lexeme("ふやしなさいね"))                   return set_info(info, T_TASUTO);
            if (lexeme("ふやしなさい"))                     return set_info(info, T_TASUTO);
            if (lexeme("ふやしてみー"))                     return set_info(info, T_TASUTO);
            if (lexeme("ふやしてみよ"))                     return set_info(info, T_TASUTO);
            if (lexeme("ふやしてみい"))                     return set_info(info, T_TASUTO);
            if (lexeme("ふやしてみぃ"))                     return set_info(info, T_TASUTO);
            if (lexeme("ふやしてみ"))                       return set_info(info, T_TASUTO);
            if (lexeme("ふやして"))                         return set_info(info, T_TASHITE);
            if (lexeme("ふやしたら"))                       return set_info(info, T_TASUTO);
            if (lexeme("ふやしたときの"))                   return set_info(info, T_TASUTO);
            if (lexeme("ふやしたとき"))                     return set_info(info, T_TASUTO);
            if (lexeme("ふやした"))                         return set_info(info, T_TASHITA);
            if (lexeme("ふの"))                             return set_info(info, T_FUNO);
            if (lexeme("ふた"))                             return set_info(info, T_NI);
            if (lexeme("ふかしぎ"))                         return set_info(info, T_FUKASHIGI);
            if (lexeme("ぴゃく"))                           return set_info(info, T_HYAKU);
            if (lexeme("びゃく"))                           return set_info(info, T_HYAKU);
            if (lexeme("ひー"))                             return set_info(info, T_ICHI);
            if (lexeme("ひゃく"))                           return set_info(info, T_HYAKU);
            if (lexeme("ひと"))                             return set_info(info, T_ICHI);
            if (lexeme("ひち"))                             return set_info(info, T_NANA);
            if (lexeme("ひじょーすー"))                     return set_info(info, T_HIJOUSUU);
            if (lexeme("ひじょーすう"))                     return set_info(info, T_HIJOUSUU);
            if (lexeme("ひじょすー"))                       return set_info(info, T_HIJOSUU);
            if (lexeme("ひじょすう"))                       return set_info(info, T_HIJOSUU);
            if (lexeme("ひじょうすー"))                     return set_info(info, T_HIJOUSUU);
            if (lexeme("ひじょうすう"))                     return set_info(info, T_HIJOUSUU);
            if (lexeme("ひげんすー"))                       return set_info(info, T_HIGENSUU);
            if (lexeme("ひげんすう"))                       return set_info(info, T_HIGENSUU);
            if (lexeme("ひけば"))                           return set_info(info, T_HIKUTO);
            if (lexeme("ひけ"))                             return set_info(info, T_HIKUTO);
            if (lexeme("ひくときの"))                       return set_info(info, T_HIKUTO);
            if (lexeme("ひくとき"))                         return set_info(info, T_HIKUTO);
            if (lexeme("ひくと"))                           return set_info(info, T_HIKUTO);
            if (lexeme("ひくことの"))                       return set_info(info, T_HIKU);
            if (lexeme("ひく"))                             return set_info(info, T_HIKU);
            if (lexeme("ひきます"))                         return set_info(info, T_HIKU);
            if (lexeme("ひきなさい"))                       return set_info(info, T_HIKUTO);
            if (lexeme("ひきざん"))                         return set_info(info, T_HIKIZAN);
            if (lexeme("ひき"))                             return set_info(info, T_HIITE);
            if (lexeme("ひかれる"))                         return set_info(info, T_HIKARERU);
            if (lexeme("ひかすー"))                         return set_info(info, T_HIKASUU);
            if (lexeme("ひかすう"))                         return set_info(info, T_HIKASUU);
            if (lexeme("ひいてみー"))                       return set_info(info, T_HIKUTO);
            if (lexeme("ひいてみよ"))                       return set_info(info, T_HIKUTO);
            if (lexeme("ひいてみい"))                       return set_info(info, T_HIKUTO);
            if (lexeme("ひいてみぃ"))                       return set_info(info, T_HIKUTO);
            if (lexeme("ひいてみ"))                         return set_info(info, T_HIKUTO);
            if (lexeme("ひいて"))                           return set_info(info, T_HIITE);
            if (lexeme("ひいたら"))                         return set_info(info, T_HIKUTO);
            if (lexeme("ひいたときの"))                     return set_info(info, T_HIKUTO);
            if (lexeme("ひいたとき"))                       return set_info(info, T_HIKUTO);
            if (lexeme("ひいた"))                           return set_info(info, T_HIITA);
            if (lexeme("ひい"))                             return set_info(info, T_ICHI);
            if (lexeme("ひぃ"))                             return set_info(info, T_ICHI);
            if (lexeme("ぱーせんと"))                       return set_info(info, T_PERCENT);
            if (lexeme("ぱー"))                             return set_info(info, T_PERCENT);
            if (lexeme("ぱま"))                             return set_info(info, T_HACHI);
            if (lexeme("ぱい"))                             return set_info(info, T_PI);
            if (lexeme("ばんめの"))                         return set_info(info, T_BANMENO);
            if (lexeme("ばん"))                             return set_info(info, T_MAN);
            if (lexeme("ばいすー"))                         return set_info(info, T_BAISUU);
            if (lexeme("ばいすう"))                         return set_info(info, T_BAISUU);
            if (lexeme("ばい"))                             return set_info(info, T_BAI);
            if (lexeme("はろー"))                           return set_info(info, T_ETTO);
            if (lexeme("はっ"))                             return set_info(info, T_HACHI);
            if (lexeme("はち"))                             return set_info(info, T_HACHI);
            if (lexeme("はいぱーぼりっく"))                 return set_info(info, T_HYPERBOLIC);
            if (lexeme("はいぱぼりっく"))                   return set_info(info, T_HYPERBOLIC);
            if (lexeme("は"))                               return set_info(info, T_HA);
            if (lexeme("のね"))                             return set_info(info, T_NO1);
            if (lexeme("のなかで"))                         return set_info(info, T_NOUCHI);
            if (lexeme("のうちで"))                         return set_info(info, T_NOUCHI);
            if (lexeme("のうち"))                           return set_info(info, T_NOUCHI);
            if (lexeme("の"))                               return set_info(info, T_NO1);
            if (lexeme("ねーぴあすー"))                     return set_info(info, T_E);
            if (lexeme("ねーぴあすう"))                     return set_info(info, T_E);
            if (lexeme("ねーの"))                           return set_info(info, T_NAI);
            if (lexeme("ねー"))                             return set_info(info, T_NAI);
            if (lexeme("ねん"))                             return set_info(info, T_KANA);
            if (lexeme("ねいぴあすー"))                     return set_info(info, T_E);
            if (lexeme("ねいぴあすう"))                     return set_info(info, T_E);
            if (lexeme("ね"))                               return set_info(info, T_KANA);
            if (lexeme("にー"))                             return set_info(info, T_NI);
            if (lexeme("にひとしー"))                       return set_info(info, T_NIHITOSHII);
            if (lexeme("にひとしくなる"))                   return set_info(info, T_NIHITOSHII);
            if (lexeme("にひとしくて"))                     return set_info(info, T_NIHITOSHIKU);
            if (lexeme("にひとしく"))                       return set_info(info, T_NIHITOSHIKU);
            if (lexeme("にひとしい"))                       return set_info(info, T_NIHITOSHII);
            if (lexeme("にね"))                             return set_info(info, T_NI);
            if (lexeme("になるんでしょー"))                 return set_info(info, T_DEARU);
            if (lexeme("になるんでしょう"))                 return set_info(info, T_DEARU);
            if (lexeme("になるんでござる"))                 return set_info(info, T_DEARU);
            if (lexeme("になるんでございます"))             return set_info(info, T_DEARU);
            if (lexeme("になるんでございましょー"))         return set_info(info, T_DEARU);
            if (lexeme("になるんでございましょう"))         return set_info(info, T_DEARU);
            if (lexeme("になるん"))                         return set_info(info, T_DEARU);
            if (lexeme("になるのでしょー"))                 return set_info(info, T_DEARU);
            if (lexeme("になるのでしょう"))                 return set_info(info, T_DEARU);
            if (lexeme("になるのでござる"))                 return set_info(info, T_DEARU);
            if (lexeme("になるのでございます"))             return set_info(info, T_DEARU);
            if (lexeme("になるのでございましょー"))         return set_info(info, T_DEARU);
            if (lexeme("になるのでございましょう"))         return set_info(info, T_DEARU);
            if (lexeme("になるの"))                         return set_info(info, T_DEARU);
            if (lexeme("になるでしょー"))                   return set_info(info, T_DEARU);
            if (lexeme("になるでしょう"))                   return set_info(info, T_DEARU);
            if (lexeme("になる"))                           return set_info(info, T_DEARU);
            if (lexeme("になります"))                       return set_info(info, T_DEARU);
            if (lexeme("にたすことの"))                     return set_info(info, T_TASU);
            if (lexeme("にくわえることの"))                 return set_info(info, T_TASU);
            if (lexeme("にかけることの"))                   return set_info(info, T_KAKERU);
            if (lexeme("にぃ"))                             return set_info(info, T_NI);
            if (lexeme("に"))                               return set_info(info, T_NI);
            if (lexeme("なーんだ"))                         return set_info(info, T_IKURA);
            if (lexeme("なーに"))                           return set_info(info, T_IKURA);
            if (lexeme("なんぼになるんでしょー"))           return set_info(info, T_IKURA);
            if (lexeme("なんぼになるんでしょう"))           return set_info(info, T_IKURA);
            if (lexeme("なんぼになるんでござる"))           return set_info(info, T_IKURA);
            if (lexeme("なんぼになるんでございます"))       return set_info(info, T_IKURA);
            if (lexeme("なんぼになるんでございましょー"))   return set_info(info, T_IKURA);
            if (lexeme("なんぼになるんでございましょう"))   return set_info(info, T_IKURA);
            if (lexeme("なんぼになるん"))                   return set_info(info, T_IKURA);
            if (lexeme("なんぼになるのー"))                 return set_info(info, T_IKURA);
            if (lexeme("なんぼになるのでしょー"))           return set_info(info, T_IKURA);
            if (lexeme("なんぼになるのでしょう"))           return set_info(info, T_IKURA);
            if (lexeme("なんぼになるのでござる"))           return set_info(info, T_IKURA);
            if (lexeme("なんぼになるのでございます"))       return set_info(info, T_IKURA);
            if (lexeme("なんぼになるのでございましょー"))   return set_info(info, T_IKURA);
            if (lexeme("なんぼになるのでございましょう"))   return set_info(info, T_IKURA);
            if (lexeme("なんぼになるの"))                   return set_info(info, T_IKURA);
            if (lexeme("なんぼになるでしょー"))             return set_info(info, T_IKURA);
            if (lexeme("なんぼになるでしょう"))             return set_info(info, T_IKURA);
            if (lexeme("なんぼになる"))                     return set_info(info, T_IKURA);
            if (lexeme("なんぼになります"))                 return set_info(info, T_IKURA);
            if (lexeme("なんぼなん"))                       return set_info(info, T_IKURA);
            if (lexeme("なんぼなの"))                       return set_info(info, T_IKURA);
            if (lexeme("なんぼ"))                           return set_info(info, T_IKURA);
            if (lexeme("なんになるんでしょー"))             return set_info(info, T_IKURA);
            if (lexeme("なんになるんでしょう"))             return set_info(info, T_IKURA);
            if (lexeme("なんになるんでござる"))             return set_info(info, T_IKURA);
            if (lexeme("なんになるんでございます"))         return set_info(info, T_IKURA);
            if (lexeme("なんになるんでございましょー"))     return set_info(info, T_IKURA);
            if (lexeme("なんになるんでございましょう"))     return set_info(info, T_IKURA);
            if (lexeme("なんになるん"))                     return set_info(info, T_IKURA);
            if (lexeme("なんになるのでしょー"))             return set_info(info, T_IKURA);
            if (lexeme("なんになるのでしょう"))             return set_info(info, T_IKURA);
            if (lexeme("なんになるのでござる"))             return set_info(info, T_IKURA);
            if (lexeme("なんになるのでございます"))         return set_info(info, T_IKURA);
            if (lexeme("なんになるのでございましょー"))     return set_info(info, T_IKURA);
            if (lexeme("なんになるのでございましょう"))     return set_info(info, T_IKURA);
            if (lexeme("なんになるの"))                     return set_info(info, T_IKURA);
            if (lexeme("なんになるでしょー"))               return set_info(info, T_IKURA);
            if (lexeme("なんになるでしょう"))               return set_info(info, T_IKURA);
            if (lexeme("なんになる"))                       return set_info(info, T_IKURA);
            if (lexeme("なんになります"))                   return set_info(info, T_IKURA);
            if (lexeme("なんなんでしょー"))                 return set_info(info, T_IKURA);
            if (lexeme("なんなんでしょう"))                 return set_info(info, T_IKURA);
            if (lexeme("なんなん"))                         return set_info(info, T_IKURA);
            if (lexeme("なんなのでしょー"))                 return set_info(info, T_IKURA);
            if (lexeme("なんなのでしょう"))                 return set_info(info, T_IKURA);
            if (lexeme("なんなの"))                         return set_info(info, T_IKURA);
            if (lexeme("なんでやんす"))                     return set_info(info, T_DEARU);
            if (lexeme("なんでやす"))                       return set_info(info, T_DEARU);
            if (lexeme("なんでっしゃろ"))                   return set_info(info, T_DEARU);
            if (lexeme("なんでっ"))                         return set_info(info, T_DEARU);
            if (lexeme("なんですやろ"))                     return set_info(info, T_DEARU);
            if (lexeme("なんです"))                         return set_info(info, T_DEARU);
            if (lexeme("なんでしょー"))                     return set_info(info, T_DEARU);
            if (lexeme("なんでしょう"))                     return set_info(info, T_DEARU);
            if (lexeme("なんでござる"))                     return set_info(info, T_DEARU);
            if (lexeme("なんでございますのでしょー"))       return set_info(info, T_DEARU);
            if (lexeme("なんでございますのでしょう"))       return set_info(info, T_DEARU);
            if (lexeme("なんでございますの"))               return set_info(info, T_DEARU);
            if (lexeme("なんでございますでしょー"))         return set_info(info, T_DEARU);
            if (lexeme("なんでございますでしょう"))         return set_info(info, T_DEARU);
            if (lexeme("なんでございます"))                 return set_info(info, T_DEARU);
            if (lexeme("なんでございましょー"))             return set_info(info, T_DEARU);
            if (lexeme("なんでございましょう"))             return set_info(info, T_DEARU);
            if (lexeme("なんである"))                       return set_info(info, T_DEARU);
            if (lexeme("なんでありんす"))                   return set_info(info, T_DEARU);
            if (lexeme("なんであります"))                   return set_info(info, T_DEARU);
            if (lexeme("なんだろー"))                       return set_info(info, T_DEARU);
            if (lexeme("なんだろう"))                       return set_info(info, T_DEARU);
            if (lexeme("なんだ"))                           return set_info(info, T_DEARU);
            if (lexeme("なん"))                             return set_info(info, T_DEARU);
            if (lexeme("ならば"))                           return set_info(info, T_ETTO);
            if (lexeme("なゆた"))                           return set_info(info, T_NAYUTA);
            if (lexeme("なのは"))                           return set_info(info, T_NANOHA);
            if (lexeme("なのでやんす"))                     return set_info(info, T_DEARU);
            if (lexeme("なのでやす"))                       return set_info(info, T_DEARU);
            if (lexeme("なのでっしゃろ"))                   return set_info(info, T_DEARU);
            if (lexeme("なのでっ"))                         return set_info(info, T_DEARU);
            if (lexeme("なのですやろ"))                     return set_info(info, T_DEARU);
            if (lexeme("なのです"))                         return set_info(info, T_DEARU);
            if (lexeme("なのでしょー"))                     return set_info(info, T_DEARU);
            if (lexeme("なのでしょう"))                     return set_info(info, T_DEARU);
            if (lexeme("なのでござる"))                     return set_info(info, T_DEARU);
            if (lexeme("なのでございますのでしょー"))       return set_info(info, T_DEARU);
            if (lexeme("なのでございますのでしょう"))       return set_info(info, T_DEARU);
            if (lexeme("なのでございますの"))               return set_info(info, T_DEARU);
            if (lexeme("なのでございますでしょー"))         return set_info(info, T_DEARU);
            if (lexeme("なのでございますでしょう"))         return set_info(info, T_DEARU);
            if (lexeme("なのでございます"))                 return set_info(info, T_DEARU);
            if (lexeme("なのでございましょー"))             return set_info(info, T_DEARU);
            if (lexeme("なのでございましょう"))             return set_info(info, T_DEARU);
            if (lexeme("なのである"))                       return set_info(info, T_DEARU);
            if (lexeme("なのでありんす"))                   return set_info(info, T_DEARU);
            if (lexeme("なのであります"))                   return set_info(info, T_DEARU);
            if (lexeme("なのだろー"))                       return set_info(info, T_DEARU);
            if (lexeme("なのだろう"))                       return set_info(info, T_DEARU);
            if (lexeme("なのだ"))                           return set_info(info, T_DEARU);
            if (lexeme("なの"))                             return set_info(info, T_DEARU);
            if (lexeme("なぬ"))                             return set_info(info, T_NANA);
            if (lexeme("なにになるんでしょー"))             return set_info(info, T_IKURA);
            if (lexeme("なにになるんでしょう"))             return set_info(info, T_IKURA);
            if (lexeme("なにになるんでござる"))             return set_info(info, T_IKURA);
            if (lexeme("なにになるんでございます"))         return set_info(info, T_IKURA);
            if (lexeme("なにになるん"))                     return set_info(info, T_IKURA);
            if (lexeme("なにになるのでしょー"))             return set_info(info, T_IKURA);
            if (lexeme("なにになるのでしょう"))             return set_info(info, T_IKURA);
            if (lexeme("なにになるのでござる"))             return set_info(info, T_IKURA);
            if (lexeme("なにになるのでございます"))         return set_info(info, T_IKURA);
            if (lexeme("なにになるの"))                     return set_info(info, T_IKURA);
            if (lexeme("なにになるでしょー"))               return set_info(info, T_IKURA);
            if (lexeme("なにになるでしょう"))               return set_info(info, T_IKURA);
            if (lexeme("なにになる"))                       return set_info(info, T_IKURA);
            if (lexeme("なに"))                             return set_info(info, T_IKURA);
            if (lexeme("なな"))                             return set_info(info, T_NANA);
            if (lexeme("なし"))                             return set_info(info, T_NAI);
            if (lexeme("なおせ"))                           return set_info(info, T_SURUTO);
            if (lexeme("なおすときの"))                     return set_info(info, T_SURUTO);
            if (lexeme("なおすとき"))                       return set_info(info, T_SURUTO);
            if (lexeme("なおすと"))                         return set_info(info, T_SURUTO);
            if (lexeme("なおしなさい"))                     return set_info(info, T_SURUTO);
            if (lexeme("なおしてみー"))                     return set_info(info, T_SURUTO);
            if (lexeme("なおしてみろ"))                     return set_info(info, T_SURUTO);
            if (lexeme("なおしてみよ"))                     return set_info(info, T_SURUTO);
            if (lexeme("なおしてみなさい"))                 return set_info(info, T_SURUTO);
            if (lexeme("なおしてみい"))                     return set_info(info, T_SURUTO);
            if (lexeme("なおしてみぃ"))                     return set_info(info, T_SURUTO);
            if (lexeme("なおしてみ"))                       return set_info(info, T_SURUTO);
            if (lexeme("なおして"))                         return set_info(info, T_SURUTO);
            if (lexeme("なおしたら"))                       return set_info(info, T_SURUTO);
            if (lexeme("なおした"))                         return set_info(info, T_SHITA);
            if (lexeme("ないん"))                           return set_info(info, T_NAI);
            if (lexeme("ないの"))                           return set_info(info, T_NAI);
            if (lexeme("ない"))                             return set_info(info, T_NAI);
            if (lexeme("なあんだ"))                         return set_info(info, T_IKURA);
            if (lexeme("なあに"))                           return set_info(info, T_IKURA);
            if (lexeme("なぁんだ"))                         return set_info(info, T_IKURA);
            if (lexeme("なぁに"))                           return set_info(info, T_IKURA);
            if (lexeme("どーなん"))                         return set_info(info, T_IKURA);
            if (lexeme("どーなる"))                         return set_info(info, T_IKURA);
            if (lexeme("どーなります"))                     return set_info(info, T_IKURA);
            if (lexeme("どーなの"))                         return set_info(info, T_IKURA);
            if (lexeme("どー"))                             return set_info(info, T_IKURA);
            if (lexeme("どんなかずになるんでしょー"))       return set_info(info, T_IKURA);
            if (lexeme("どんなかずになるんでしょう"))       return set_info(info, T_IKURA);
            if (lexeme("どんなかずになるんでござる"))       return set_info(info, T_IKURA);
            if (lexeme("どんなかずになるんでございます"))   return set_info(info, T_IKURA);
            if (lexeme("どんなかずになるんでございましょー")) return set_info(info, T_IKURA);
            if (lexeme("どんなかずになるんでございましょう")) return set_info(info, T_IKURA);
            if (lexeme("どんなかずになるん"))               return set_info(info, T_IKURA);
            if (lexeme("どんなかずになるのでしょー"))       return set_info(info, T_IKURA);
            if (lexeme("どんなかずになるのでしょう"))       return set_info(info, T_IKURA);
            if (lexeme("どんなかずになるのでござる"))       return set_info(info, T_IKURA);
            if (lexeme("どんなかずになるのでございます"))   return set_info(info, T_IKURA);
            if (lexeme("どんなかずになるのでございましょー")) return set_info(info, T_IKURA);
            if (lexeme("どんなかずになるのでございましょう")) return set_info(info, T_IKURA);
            if (lexeme("どんなかずになるの"))               return set_info(info, T_IKURA);
            if (lexeme("どんなかずになる"))                 return set_info(info, T_IKURA);
            if (lexeme("どんなかずになります"))             return set_info(info, T_IKURA);
            if (lexeme("どれになるんでしょー"))             return set_info(info, T_IKURA);
            if (lexeme("どれになるんでしょう"))             return set_info(info, T_IKURA);
            if (lexeme("どれになるんでござる"))             return set_info(info, T_IKURA);
            if (lexeme("どれになるんでございます"))         return set_info(info, T_IKURA);
            if (lexeme("どれになるん"))                     return set_info(info, T_IKURA);
            if (lexeme("どれになるのでしょー"))             return set_info(info, T_IKURA);
            if (lexeme("どれになるのでしょう"))             return set_info(info, T_IKURA);
            if (lexeme("どれになるのでござる"))             return set_info(info, T_IKURA);
            if (lexeme("どれになるのでございます"))         return set_info(info, T_IKURA);
            if (lexeme("どれになるの"))                     return set_info(info, T_IKURA);
            if (lexeme("どれになるでしょー"))               return set_info(info, T_IKURA);
            if (lexeme("どれになるでしょう"))               return set_info(info, T_IKURA);
            if (lexeme("どれになる"))                       return set_info(info, T_IKURA);
            if (lexeme("どれ"))                             return set_info(info, T_IKURA);
            if (lexeme("どないなるんでしょー"))             return set_info(info, T_IKURA);
            if (lexeme("どないなるんでしょう"))             return set_info(info, T_IKURA);
            if (lexeme("どないなるん"))                     return set_info(info, T_IKURA);
            if (lexeme("どないなるのでしょー"))             return set_info(info, T_IKURA);
            if (lexeme("どないなるのでしょう"))             return set_info(info, T_IKURA);
            if (lexeme("どないなるの"))                     return set_info(info, T_IKURA);
            if (lexeme("どないなるでしょー"))               return set_info(info, T_IKURA);
            if (lexeme("どないなるでしょう"))               return set_info(info, T_IKURA);
            if (lexeme("どないなる"))                       return set_info(info, T_IKURA);
            if (lexeme("どない"))                           return set_info(info, T_IKURA);
            if (lexeme("どうなん"))                         return set_info(info, T_IKURA);
            if (lexeme("どうなる"))                         return set_info(info, T_IKURA);
            if (lexeme("どうなります"))                     return set_info(info, T_IKURA);
            if (lexeme("どうなの"))                         return set_info(info, T_IKURA);
            if (lexeme("どう"))                             return set_info(info, T_IKURA);
            if (lexeme("ど"))                               return set_info(info, T_DO);
            if (lexeme("とんでー"))                         return set_info(info, T_ETTO);
            if (lexeme("とんで"))                           return set_info(info, T_ETTO);
            if (lexeme("とひとしー"))                       return set_info(info, T_NIHITOSHII);
            if (lexeme("とひとしくなる"))                   return set_info(info, T_NIHITOSHII);
            if (lexeme("とひとしくて"))                     return set_info(info, T_NIHITOSHIKU);
            if (lexeme("とひとしく"))                       return set_info(info, T_NIHITOSHIKU);
            if (lexeme("とひとしい"))                       return set_info(info, T_NIHITOSHII);
            if (lexeme("とね、"))                           return set_info(info, T_TO1);
            if (lexeme("となる"))                           return set_info(info, T_NIHITOSHII);
            if (lexeme("とな、"))                           return set_info(info, T_TO1);
            if (lexeme("とじかっこ"))                       return set_info(info, T_R_PAREN);
            if (lexeme("とお"))                             return set_info(info, T_JUU);
            if (lexeme("と"))                               return set_info(info, T_TO1);
            if (lexeme("でん"))                             return set_info(info, T_DEARU);
            if (lexeme("でやんす"))                         return set_info(info, T_DEARU);
            if (lexeme("でやす"))                           return set_info(info, T_DEARU);
            if (lexeme("では"))                             return set_info(info, T_ETTO);
            if (lexeme("でっしゃろ"))                       return set_info(info, T_DEARU);
            if (lexeme("でっ"))                             return set_info(info, T_DEARU);
            if (lexeme("ですやろ"))                         return set_info(info, T_DEARU);
            if (lexeme("です"))                             return set_info(info, T_DEARU);
            if (lexeme("でしょー"))                         return set_info(info, T_DEARU);
            if (lexeme("でしょう"))                         return set_info(info, T_DEARU);
            if (lexeme("でござる"))                         return set_info(info, T_DEARU);
            if (lexeme("でございますのでしょー"))           return set_info(info, T_DEARU);
            if (lexeme("でございますのでしょう"))           return set_info(info, T_DEARU);
            if (lexeme("でございますの"))                   return set_info(info, T_DEARU);
            if (lexeme("でございますでしょー"))             return set_info(info, T_DEARU);
            if (lexeme("でございますでしょう"))             return set_info(info, T_DEARU);
            if (lexeme("でございます"))                     return set_info(info, T_DEARU);
            if (lexeme("でございましょー"))                 return set_info(info, T_DEARU);
            if (lexeme("でございましょう"))                 return set_info(info, T_DEARU);
            if (lexeme("である"))                           return set_info(info, T_DEARU);
            if (lexeme("でありんす"))                       return set_info(info, T_DEARU);
            if (lexeme("であります"))                       return set_info(info, T_DEARU);
            if (lexeme("であり"))                           return set_info(info, T_DE1);
            if (lexeme("で"))                               return set_info(info, T_DE1);
            if (lexeme("てん"))                             return set_info(info, T_DOT);
            if (lexeme("つぶん"))                           return set_info(info, T_BAI);
            if (lexeme("って"))                             return set_info(info, T_HA);
            if (lexeme("ちーさく"))                         return set_info(info, T_CHIISAKU);
            if (lexeme("ちーさい"))                         return set_info(info, T_CHIISAI);
            if (lexeme("ちょーひゃく"))                     return set_info(info, T_HYAKU);
            if (lexeme("ちょーはん"))                       return set_info(info, T_GO);
            if (lexeme("ちょーだい"))                       return set_info(info, T_KURE);
            if (lexeme("ちょー"))                           return set_info(info, T_CHOU);
            if (lexeme("ちょうひゃく"))                     return set_info(info, T_HYAKU);
            if (lexeme("ちょうはん"))                       return set_info(info, T_GO);
            if (lexeme("ちょうだい"))                       return set_info(info, T_KURE);
            if (lexeme("ちょう"))                           return set_info(info, T_CHOU);
            if (lexeme("ちいさく"))                         return set_info(info, T_CHIISAKU);
            if (lexeme("ちいさい"))                         return set_info(info, T_CHIISAI);
            if (lexeme("ち"))                               return set_info(info, T_SEN);
            if (lexeme("だろー"))                           return set_info(info, T_DEARU);
            if (lexeme("だろう"))                           return set_info(info, T_DEARU);
            if (lexeme("だよねー"))                         return set_info(info, T_KANA);
            if (lexeme("だよねえ"))                         return set_info(info, T_KANA);
            if (lexeme("だよねぇ"))                         return set_info(info, T_KANA);
            if (lexeme("だよね"))                           return set_info(info, T_KANA);
            if (lexeme("だったら"))                         return set_info(info, T_ETTO);
            if (lexeme("だ"))                               return set_info(info, T_DEARU);
            if (lexeme("たんぜんと"))                       return set_info(info, T_TAN);
            if (lexeme("たんじぇんと"))                     return set_info(info, T_TAN);
            if (lexeme("たん"))                             return set_info(info, T_TAN);
            if (lexeme("たせば"))                           return set_info(info, T_TASUTO);
            if (lexeme("たせ"))                             return set_info(info, T_TASUTO);
            if (lexeme("たすとき"))                         return set_info(info, T_TASUTO);
            if (lexeme("たすと"))                           return set_info(info, T_TASUTO);
            if (lexeme("たすことの"))                       return set_info(info, T_TASU);
            if (lexeme("たす"))                             return set_info(info, T_TASU);
            if (lexeme("たします"))                         return set_info(info, T_TASU);
            if (lexeme("たしなさい"))                       return set_info(info, T_TASUTO);
            if (lexeme("たしてみー"))                       return set_info(info, T_TASUTO);
            if (lexeme("たしてみよ"))                       return set_info(info, T_TASUTO);
            if (lexeme("たしてみい"))                       return set_info(info, T_TASUTO);
            if (lexeme("たしてみぃ"))                       return set_info(info, T_TASUTO);
            if (lexeme("たしてみ"))                         return set_info(info, T_TASUTO);
            if (lexeme("たして"))                           return set_info(info, T_TASHITE);
            if (lexeme("たしたら"))                         return set_info(info, T_TASUTO);
            if (lexeme("たしたときの"))                     return set_info(info, T_TASUTO);
            if (lexeme("たしたとき"))                       return set_info(info, T_TASUTO);
            if (lexeme("たした"))                           return set_info(info, T_TASHITA);
            if (lexeme("たしざん"))                         return set_info(info, T_TASHIZAN);
            if (lexeme("たしあわせろ"))                     return set_info(info, T_TASUTO);
            if (lexeme("たしあわせれば"))                   return set_info(info, T_TASUTO);
            if (lexeme("たしあわせるとき"))                 return set_info(info, T_TASUTO);
            if (lexeme("たしあわせると"))                   return set_info(info, T_TASUTO);
            if (lexeme("たしあわせる"))                     return set_info(info, T_TASU);
            if (lexeme("たしあわせよ"))                     return set_info(info, T_TASUTO);
            if (lexeme("たしあわせます"))                   return set_info(info, T_TASU);
            if (lexeme("たしあわせなさい"))                 return set_info(info, T_TASUTO);
            if (lexeme("たしあわせてみー"))                 return set_info(info, T_TASUTO);
            if (lexeme("たしあわせてみよ"))                 return set_info(info, T_TASUTO);
            if (lexeme("たしあわせてみい"))                 return set_info(info, T_TASUTO);
            if (lexeme("たしあわせてみぃ"))                 return set_info(info, T_TASUTO);
            if (lexeme("たしあわせてみ"))                   return set_info(info, T_TASUTO);
            if (lexeme("たしあわせて"))                     return set_info(info, T_TASHITE);
            if (lexeme("たしあわせたときの"))               return set_info(info, T_TASUTO);
            if (lexeme("たしあわせたとき"))                 return set_info(info, T_TASUTO);
            if (lexeme("たしあわせた"))                     return set_info(info, T_TASHITA);
            if (lexeme("たしあわせ"))                       return set_info(info, T_TASUTO);
            if (lexeme("たしあわす"))                       return set_info(info, T_TASU);
            if (lexeme("たしあわされる"))                   return set_info(info, T_TASARERU);
            if (lexeme("たし"))                             return set_info(info, T_TASHITE);
            if (lexeme("たされる"))                         return set_info(info, T_TASARERU);
            if (lexeme("たがいにそ"))                       return set_info(info, T_TAGAINISO);
            if (lexeme("たいすーかんすー"))                 return set_info(info, T_LOG);
            if (lexeme("たいすーかんすう"))                 return set_info(info, T_LOG);
            if (lexeme("たいすー"))                         return set_info(info, T_LOG);
            if (lexeme("たいすうかんすー"))                 return set_info(info, T_LOG);
            if (lexeme("たいすうかんすう"))                 return set_info(info, T_LOG);
            if (lexeme("たいすう"))                         return set_info(info, T_LOG);
            if (lexeme("そーわ"))                           return set_info(info, T_SUM);
            if (lexeme("そーや"))                           return set_info(info, T_ETTO);
            if (lexeme("そーだねー"))                       return set_info(info, T_ETTO);
            if (lexeme("そーだねえ"))                       return set_info(info, T_ETTO);
            if (lexeme("そーだねぇ"))                       return set_info(info, T_ETTO);
            if (lexeme("そーだね"))                         return set_info(info, T_ETTO);
            if (lexeme("そーだ"))                           return set_info(info, T_ETTO);
            if (lexeme("そーきょくせん"))                   return set_info(info, T_HYPERBOLIC);
            if (lexeme("そー"))                             return set_info(info, T_ETTO);
            if (lexeme("そんざいするん"))                   return set_info(info, T_ARI);
            if (lexeme("そんざいするの"))                   return set_info(info, T_ARI);
            if (lexeme("そんざいする"))                     return set_info(info, T_ARI);
            if (lexeme("そんざいしませんの"))               return set_info(info, T_NAI);
            if (lexeme("そんざいしません"))                 return set_info(info, T_NAI);
            if (lexeme("そんざいしますの"))                 return set_info(info, T_ARI);
            if (lexeme("そんざいします"))                   return set_info(info, T_ARI);
            if (lexeme("そんざいしねーの"))                 return set_info(info, T_NAI);
            if (lexeme("そんざいしねー"))                   return set_info(info, T_NAI);
            if (lexeme("そんざいしないん"))                 return set_info(info, T_NAI);
            if (lexeme("そんざいしないの"))                 return set_info(info, T_NAI);
            if (lexeme("そんざいしない"))                   return set_info(info, T_NAI);
            if (lexeme("それの"))                           return set_info(info, T_SONO);
            if (lexeme("それでは"))                         return set_info(info, T_ETTO);
            if (lexeme("それだねー"))                       return set_info(info, T_ETTO);
            if (lexeme("それだねぇ"))                       return set_info(info, T_ETTO);
            if (lexeme("それだね"))                         return set_info(info, T_ETTO);
            if (lexeme("それだったら"))                     return set_info(info, T_ETTO);
            if (lexeme("それだ"))                           return set_info(info, T_ETTO);
            if (lexeme("それじゃねー"))                     return set_info(info, T_ETTO);
            if (lexeme("それじゃねえ"))                     return set_info(info, T_ETTO);
            if (lexeme("それじゃねぇ"))                     return set_info(info, T_ETTO);
            if (lexeme("それじゃね"))                       return set_info(info, T_ETTO);
            if (lexeme("それがしの"))                       return set_info(info, T_SONO);
            if (lexeme("それがし"))                         return set_info(info, T_SORE);
            if (lexeme("それ"))                             return set_info(info, T_SORE);
            if (lexeme("そやつ"))                           return set_info(info, T_SORE);
            if (lexeme("そのとき"))                         return set_info(info, T_SURUTO);
            if (lexeme("そのこたえ"))                       return set_info(info, T_KOTAE);
            if (lexeme("その"))                             return set_info(info, T_SONO);
            if (lexeme("そすー"))                           return set_info(info, T_SOSUU);
            if (lexeme("そすう"))                           return set_info(info, T_SOSUU);
            if (lexeme("そうわ"))                           return set_info(info, T_SUM);
            if (lexeme("そうや"))                           return set_info(info, T_ETTO);
            if (lexeme("そうだねー"))                       return set_info(info, T_ETTO);
            if (lexeme("そうだねえ"))                       return set_info(info, T_ETTO);
            if (lexeme("そうだねぇ"))                       return set_info(info, T_ETTO);
            if (lexeme("そうだね"))                         return set_info(info, T_ETTO);
            if (lexeme("そうだ"))                           return set_info(info, T_ETTO);
            if (lexeme("そうきょくせん"))                   return set_info(info, T_HYPERBOLIC);
            if (lexeme("そう"))                             return set_info(info, T_ETTO);
            if (lexeme("そいんすーぶんかい"))               return set_info(info, T_SOINSUUBUNKAI);
            if (lexeme("そいんすー"))                       return set_info(info, T_SOINSUU);
            if (lexeme("そいんすうぶんかい"))               return set_info(info, T_SOINSUUBUNKAI);
            if (lexeme("そいんすう"))                       return set_info(info, T_SOINSUU);
            if (lexeme("そいつ"))                           return set_info(info, T_SORE);
            if (lexeme("そ"))                               return set_info(info, T_JUU);
            if (lexeme("ぜんぶ"))                           return set_info(info, T_ALL);
            if (lexeme("ぜん"))                             return set_info(info, T_SEN);
            if (lexeme("ぜろ"))                             return set_info(info, T_ZERO);
            if (lexeme("ぜったいち"))                       return set_info(info, T_ZETTAICHI);
            if (lexeme("せーの"))                           return set_info(info, T_SEINO);
            if (lexeme("せーせつ"))                         return set_info(info, T_TAN);
            if (lexeme("せーすー"))                         return set_info(info, T_SEISUU);
            if (lexeme("せーすう"))                         return set_info(info, T_SEISUU);
            if (lexeme("せーじょ"))                         return set_info(info, T_SEIJO);
            if (lexeme("せーしょー"))                       return set_info(info, T_INTQUOT);
            if (lexeme("せーしょう"))                       return set_info(info, T_INTQUOT);
            if (lexeme("せーげん"))                         return set_info(info, T_SIN);
            if (lexeme("せー"))                             return set_info(info, T_SEI);
            if (lexeme("せん"))                             return set_info(info, T_SEN);
            if (lexeme("せよ"))                             return set_info(info, T_SURUTO);
            if (lexeme("せき"))                             return set_info(info, T_PROD);
            if (lexeme("せいの"))                           return set_info(info, T_SEINO);
            if (lexeme("せいせつ"))                         return set_info(info, T_TAN);
            if (lexeme("せいすー"))                         return set_info(info, T_SEISUU);
            if (lexeme("せいすう"))                         return set_info(info, T_SEISUU);
            if (lexeme("せいじょ"))                         return set_info(info, T_SEIJO);
            if (lexeme("せいしょー"))                       return set_info(info, T_INTQUOT);
            if (lexeme("せいしょう"))                       return set_info(info, T_INTQUOT);
            if (lexeme("せいげん"))                         return set_info(info, T_SIN);
            if (lexeme("せい"))                             return set_info(info, T_SEI);
            if (lexeme("すれば"))                           return set_info(info, T_SURUTO);
            if (lexeme("するときの"))                       return set_info(info, T_SURUTO);
            if (lexeme("するとき"))                         return set_info(info, T_SURUTO);
            if (lexeme("すると"))                           return set_info(info, T_SURUTO);
            if (lexeme("する"))                             return set_info(info, T_SURU);
            if (lexeme("すべて"))                           return set_info(info, T_ALL);
            if (lexeme("じょーよーたいすー"))               return set_info(info, T_LOG10);
            if (lexeme("じょーよーたいすう"))               return set_info(info, T_LOG10);
            if (lexeme("じょーようたいすー"))               return set_info(info, T_LOG10);
            if (lexeme("じょーようたいすう"))               return set_info(info, T_LOG10);
            if (lexeme("じょーよ"))                         return set_info(info, T_AMARI);
            if (lexeme("じょーほー"))                       return set_info(info, T_KAKEZAN);
            if (lexeme("じょーほう"))                       return set_info(info, T_KAKEZAN);
            if (lexeme("じょーすー"))                       return set_info(info, T_JOUSUU);
            if (lexeme("じょーすう"))                       return set_info(info, T_JOUSUU);
            if (lexeme("じょーざん"))                       return set_info(info, T_KAKEZAN);
            if (lexeme("じょー"))                           return set_info(info, T_JOU1);
            if (lexeme("じょほー"))                         return set_info(info, T_WARIZAN);
            if (lexeme("じょほう"))                         return set_info(info, T_WARIZAN);
            if (lexeme("じょすー"))                         return set_info(info, T_JOSUU);
            if (lexeme("じょすう"))                         return set_info(info, T_JOSUU);
            if (lexeme("じょざん"))                         return set_info(info, T_WARIZAN);
            if (lexeme("じょうよーたいすー"))               return set_info(info, T_LOG10);
            if (lexeme("じょうよーたいすう"))               return set_info(info, T_LOG10);
            if (lexeme("じょうようたいすー"))               return set_info(info, T_LOG10);
            if (lexeme("じょうようたいすう"))               return set_info(info, T_LOG10);
            if (lexeme("じょうよ"))                         return set_info(info, T_AMARI);
            if (lexeme("じょうほー"))                       return set_info(info, T_KAKEZAN);
            if (lexeme("じょうほう"))                       return set_info(info, T_KAKEZAN);
            if (lexeme("じょうすー"))                       return set_info(info, T_JOUSUU);
            if (lexeme("じょうすう"))                       return set_info(info, T_JOUSUU);
            if (lexeme("じょうざん"))                       return set_info(info, T_KAKEZAN);
            if (lexeme("じょう"))                           return set_info(info, T_JOU1);
            if (lexeme("じょ"))                             return set_info(info, T_JO);
            if (lexeme("じゅー"))                           return set_info(info, T_JUU);
            if (lexeme("じゅっ"))                           return set_info(info, T_JUU);
            if (lexeme("じゅう"))                           return set_info(info, T_JUU);
            if (lexeme("じめいなやくすー"))                 return set_info(info, T_JIMEINAYAKUSUU);
            if (lexeme("じめいなやくすう"))                 return set_info(info, T_JIMEINAYAKUSUU);
            if (lexeme("じめいないんすー"))                 return set_info(info, T_JIMEINAYAKUSUU);
            if (lexeme("じめいないんすう"))                 return set_info(info, T_JIMEINAYAKUSUU);
            if (lexeme("じっすー"))                         return set_info(info, T_JISSUU);
            if (lexeme("じっすう"))                         return set_info(info, T_JISSUU);
            if (lexeme("じっ"))                             return set_info(info, T_JUU);
            if (lexeme("じじょー"))                         return set_info(info, T_HEIHOU);
            if (lexeme("じじょう"))                         return set_info(info, T_HEIHOU);
            if (lexeme("しーや"))                           return set_info(info, T_SURUTO);
            if (lexeme("しー"))                             return set_info(info, T_YON);
            if (lexeme("しんのやくすー"))                   return set_info(info, T_SHINNOYAKUSUU);
            if (lexeme("しんのやくすう"))                   return set_info(info, T_SHINNOYAKUSUU);
            if (lexeme("しんのいんすー"))                   return set_info(info, T_SHINNOYAKUSUU);
            if (lexeme("しんのいんすう"))                   return set_info(info, T_SHINNOYAKUSUU);
            if (lexeme("しん"))                             return set_info(info, T_SIN);
            if (lexeme("しろ"))                             return set_info(info, T_SURUTO);
            if (lexeme("しらん"))                           return set_info(info, T_WAKARAN);
            if (lexeme("しらねー"))                         return set_info(info, T_WAKARAN);
            if (lexeme("しらない"))                         return set_info(info, T_WAKARAN);
            if (lexeme("しょーすー"))                       return set_info(info, T_SHOUSUU);
            if (lexeme("しょーすう"))                       return set_info(info, T_SHOUSUU);
            if (lexeme("しょー"))                           return set_info(info, T_QUOT);
            if (lexeme("しょうすー"))                       return set_info(info, T_SHOUSUU);
            if (lexeme("しょうすう"))                       return set_info(info, T_SHOUSUU);
            if (lexeme("しょう"))                           return set_info(info, T_QUOT);
            if (lexeme("しなさい"))                         return set_info(info, T_SURUTO);
            if (lexeme("してみー"))                         return set_info(info, T_SURUTO);
            if (lexeme("してみろ"))                         return set_info(info, T_SURUTO);
            if (lexeme("してみれば"))                       return set_info(info, T_SURUTO);
            if (lexeme("してみるときの"))                   return set_info(info, T_SURUTO);
            if (lexeme("してみるとき"))                     return set_info(info, T_SURUTO);
            if (lexeme("してみると"))                       return set_info(info, T_SURUTO);
            if (lexeme("してみなさい"))                     return set_info(info, T_SURUTO);
            if (lexeme("してみい"))                         return set_info(info, T_SURUTO);
            if (lexeme("してみぃ"))                         return set_info(info, T_SURUTO);
            if (lexeme("してみ"))                           return set_info(info, T_SURUTO);
            if (lexeme("して"))                             return set_info(info, T_SHITE);
            if (lexeme("しつもんやで"))                     return set_info(info, T_ETTO);
            if (lexeme("しつもんや"))                       return set_info(info, T_ETTO);
            if (lexeme("しつもんです"))                     return set_info(info, T_ETTO);
            if (lexeme("しつもんがあります"))               return set_info(info, T_ETTO);
            if (lexeme("しつもん"))                         return set_info(info, T_ETTO);
            if (lexeme("しち"))                             return set_info(info, T_NANA);
            if (lexeme("したら"))                           return set_info(info, T_SURUTO);
            if (lexeme("したときの"))                       return set_info(info, T_SURUTO);
            if (lexeme("したとき"))                         return set_info(info, T_SURUTO);
            if (lexeme("した"))                             return set_info(info, T_SHITA);
            if (lexeme("しぜんたいすーのてー"))             return set_info(info, T_E);
            if (lexeme("しぜんたいすーのてい"))             return set_info(info, T_E);
            if (lexeme("しぜんたいすー"))                   return set_info(info, T_LOG);
            if (lexeme("しぜんたいすうのてー"))             return set_info(info, T_E);
            if (lexeme("しぜんたいすうのてい"))             return set_info(info, T_E);
            if (lexeme("しぜんたいすう"))                   return set_info(info, T_LOG);
            if (lexeme("しぜんすー"))                       return set_info(info, T_SHIZENSUU);
            if (lexeme("しぜんすう"))                       return set_info(info, T_SHIZENSUU);
            if (lexeme("しすーかんすー"))                   return set_info(info, T_EXP);
            if (lexeme("しすーかんすう"))                   return set_info(info, T_EXP);
            if (lexeme("しすー"))                           return set_info(info, T_EXP);
            if (lexeme("しすうかんすう"))                   return set_info(info, T_EXP);
            if (lexeme("しすう"))                           return set_info(info, T_EXP);
            if (lexeme("しいや"))                           return set_info(info, T_SURUTO);
            if (lexeme("しい"))                             return set_info(info, T_YON);
            if (lexeme("しぃ"))                             return set_info(info, T_YON);
            if (lexeme("し"))                               return set_info(info, T_YON);
            if (lexeme("さん"))                             return set_info(info, T_SAN);
            if (lexeme("さしひーてみー"))                   return set_info(info, T_HIKUTO);
            if (lexeme("さしひーてみよ"))                   return set_info(info, T_HIKUTO);
            if (lexeme("さしひーてみい"))                   return set_info(info, T_HIKUTO);
            if (lexeme("さしひーてみぃ"))                   return set_info(info, T_HIKUTO);
            if (lexeme("さしひーてみ"))                     return set_info(info, T_HIKUTO);
            if (lexeme("さしひーて"))                       return set_info(info, T_HIITE);
            if (lexeme("さしひーたら"))                     return set_info(info, T_HIKUTO);
            if (lexeme("さしひーたときの"))                 return set_info(info, T_HIKUTO);
            if (lexeme("さしひーたとき"))                   return set_info(info, T_HIKUTO);
            if (lexeme("さしひーた"))                       return set_info(info, T_HIITA);
            if (lexeme("さしひけ"))                         return set_info(info, T_HIKUTO);
            if (lexeme("さしひくとき"))                     return set_info(info, T_HIKUTO);
            if (lexeme("さしひきます"))                     return set_info(info, T_HIKU);
            if (lexeme("さしひきなさい"))                   return set_info(info, T_HIKUTO);
            if (lexeme("さしひき"))                         return set_info(info, T_HIITE);
            if (lexeme("さしひかれる"))                     return set_info(info, T_HIKARERU);
            if (lexeme("さしひいてみー"))                   return set_info(info, T_HIKUTO);
            if (lexeme("さしひいてみよ"))                   return set_info(info, T_HIKUTO);
            if (lexeme("さしひいてみい"))                   return set_info(info, T_HIKUTO);
            if (lexeme("さしひいてみぃ"))                   return set_info(info, T_HIKUTO);
            if (lexeme("さしひいてみ"))                     return set_info(info, T_HIKUTO);
            if (lexeme("さしひいて"))                       return set_info(info, T_HIITE);
            if (lexeme("さしひいたら"))                     return set_info(info, T_HIKUTO);
            if (lexeme("さしひいたときの"))                 return set_info(info, T_HIKUTO);
            if (lexeme("さしひいたとき"))                   return set_info(info, T_HIKUTO);
            if (lexeme("さしひいた"))                       return set_info(info, T_HIITA);
            if (lexeme("さいん"))                           return set_info(info, T_SIN);
            if (lexeme("さいだいのもの"))                   return set_info(info, T_MAX);
            if (lexeme("さいだいなもの"))                   return set_info(info, T_MAX);
            if (lexeme("さいだいなの"))                     return set_info(info, T_MAX);
            if (lexeme("さいだいとなるもの"))               return set_info(info, T_MAX);
            if (lexeme("さいだいとなるかず"))               return set_info(info, T_MAX);
            if (lexeme("さいだいち"))                       return set_info(info, T_MAX);
            if (lexeme("さいだいこーやくすー"))             return set_info(info, T_GCD);
            if (lexeme("さいだいこーやくすう"))             return set_info(info, T_GCD);
            if (lexeme("さいだいこうやくすー"))             return set_info(info, T_GCD);
            if (lexeme("さいだいこうやくすう"))             return set_info(info, T_GCD);
            if (lexeme("さいしょーのもの"))                 return set_info(info, T_MIN);
            if (lexeme("さいしょーなもの"))                 return set_info(info, T_MIN);
            if (lexeme("さいしょーなの"))                   return set_info(info, T_MIN);
            if (lexeme("さいしょーとなるもの"))             return set_info(info, T_MIN);
            if (lexeme("さいしょーとなるかず"))             return set_info(info, T_MIN);
            if (lexeme("さいしょーち"))                     return set_info(info, T_MIN);
            if (lexeme("さいしょーこーばいすー"))           return set_info(info, T_LCM);
            if (lexeme("さいしょーこーばいすう"))           return set_info(info, T_LCM);
            if (lexeme("さいしょーこうばいすー"))           return set_info(info, T_LCM);
            if (lexeme("さいしょーこうばいすう"))           return set_info(info, T_LCM);
            if (lexeme("さいしょうのもの"))                 return set_info(info, T_MIN);
            if (lexeme("さいしょうなもの"))                 return set_info(info, T_MIN);
            if (lexeme("さいしょうなの"))                   return set_info(info, T_MIN);
            if (lexeme("さいしょうとなるもの"))             return set_info(info, T_MIN);
            if (lexeme("さいしょうとなるかず"))             return set_info(info, T_MIN);
            if (lexeme("さいしょうち"))                     return set_info(info, T_MIN);
            if (lexeme("さいしょうこうばいすう"))           return set_info(info, T_LCM);
            if (lexeme("さい"))                             return set_info(info, T_SAI);
            if (lexeme("さ"))                               return set_info(info, T_DIFF);
            if (lexeme("ごーせーすー"))                     return set_info(info, T_GOUSEISUU);
            if (lexeme("ごーせーすう"))                     return set_info(info, T_GOUSEISUU);
            if (lexeme("ごーせいすー"))                     return set_info(info, T_GOUSEISUU);
            if (lexeme("ごーせいすう"))                     return set_info(info, T_GOUSEISUU);
            if (lexeme("ごーけー"))                         return set_info(info, T_SUM);
            if (lexeme("ごーけい"))                         return set_info(info, T_SUM);
            if (lexeme("ごーがしゃ"))                       return set_info(info, T_GOUGASHA);
            if (lexeme("ごー"))                             return set_info(info, T_GO);
            if (lexeme("ごらんなさい"))                     return set_info(info, T_KURE);
            if (lexeme("ごらん"))                           return set_info(info, T_KURE);
            if (lexeme("ござるの"))                         return set_info(info, T_ARI);
            if (lexeme("ござる"))                           return set_info(info, T_ARI);
            if (lexeme("ございませんの"))                   return set_info(info, T_NAI);
            if (lexeme("ございません"))                     return set_info(info, T_NAI);
            if (lexeme("ございますの"))                     return set_info(info, T_ARI);
            if (lexeme("ございます"))                       return set_info(info, T_ARI);
            if (lexeme("ごく"))                             return set_info(info, T_GOKU);
            if (lexeme("ごうせーすー"))                     return set_info(info, T_GOUSEISUU);
            if (lexeme("ごうせーすう"))                     return set_info(info, T_GOUSEISUU);
            if (lexeme("ごうせいすー"))                     return set_info(info, T_GOUSEISUU);
            if (lexeme("ごうせいすう"))                     return set_info(info, T_GOUSEISUU);
            if (lexeme("ごうけー"))                         return set_info(info, T_SUM);
            if (lexeme("ごうけい"))                         return set_info(info, T_SUM);
            if (lexeme("ごうがしゃ"))                       return set_info(info, T_GOUGASHA);
            if (lexeme("ごう"))                             return set_info(info, T_GO);
            if (lexeme("ご"))                               return set_info(info, T_GO);
            if (lexeme("こーやくすー"))                     return set_info(info, T_KOUYAKUSUU);
            if (lexeme("こーやくすう"))                     return set_info(info, T_KOUYAKUSUU);
            if (lexeme("こーばいすー"))                     return set_info(info, T_KOUBAISUU);
            if (lexeme("こーばいすう"))                     return set_info(info, T_KOUBAISUU);
            if (lexeme("こー"))                             return set_info(info, T_KOU);
            if (lexeme("こんばんわ"))                       return set_info(info, T_ETTO);
            if (lexeme("こんばんは"))                       return set_info(info, T_ETTO);
            if (lexeme("こんにちわ"))                       return set_info(info, T_ETTO);
            if (lexeme("こんにちは"))                       return set_info(info, T_ETTO);
            if (lexeme("これの"))                           return set_info(info, T_SONO);
            if (lexeme("これ"))                             return set_info(info, T_SORE);
            if (lexeme("こらー"))                           return set_info(info, T_ETTO);
            if (lexeme("こら"))                             return set_info(info, T_ETTO);
            if (lexeme("こやつ"))                           return set_info(info, T_SORE);
            if (lexeme("こぶん"))                           return set_info(info, T_BAI);
            if (lexeme("このとき"))                         return set_info(info, T_SURUTO);
            if (lexeme("こたえろ"))                         return set_info(info, T_OSHIETE);
            if (lexeme("こたえられる"))                     return set_info(info, T_OSHIETE);
            if (lexeme("こたえよ"))                         return set_info(info, T_OSHIETE);
            if (lexeme("こたえなさい"))                     return set_info(info, T_OSHIETE);
            if (lexeme("こたえて"))                         return set_info(info, T_OSHIETE);
            if (lexeme("こたえたまへ"))                     return set_info(info, T_OSHIETE);
            if (lexeme("こたえたまえ"))                     return set_info(info, T_OSHIETE);
            if (lexeme("こたえ"))                           return set_info(info, T_KOTAE);
            if (lexeme("こすー"))                           return set_info(info, T_COUNT);
            if (lexeme("こすう"))                           return set_info(info, T_COUNT);
            if (lexeme("こす"))                             return set_info(info, T_COS);
            if (lexeme("こさいん"))                         return set_info(info, T_COS);
            if (lexeme("ここの"))                           return set_info(info, T_KYUU);
            if (lexeme("ここ"))                             return set_info(info, T_KYUU);
            if (lexeme("こうやくすう"))                     return set_info(info, T_KOUYAKUSUU);
            if (lexeme("こうばいすう"))                     return set_info(info, T_KOUBAISUU);
            if (lexeme("こう"))                             return set_info(info, T_KOU);
            if (lexeme("こいつ"))                           return set_info(info, T_SORE);
            if (lexeme("げんほー"))                         return set_info(info, T_HIKIZAN);
            if (lexeme("げんほう"))                         return set_info(info, T_HIKIZAN);
            if (lexeme("げんすー"))                         return set_info(info, T_GENSUU);
            if (lexeme("げんすう"))                         return set_info(info, T_GENSUU);
            if (lexeme("げんざん"))                         return set_info(info, T_HIKIZAN);
            if (lexeme("けーさん"))                         return set_info(info, T_KEISAN);
            if (lexeme("けー"))                             return set_info(info, T_KEI);
            if (lexeme("けいさん"))                         return set_info(info, T_KEISAN);
            if (lexeme("けい"))                             return set_info(info, T_KEI);
            if (lexeme("ぐーすー"))                         return set_info(info, T_GUUSUU);
            if (lexeme("ぐーすう"))                         return set_info(info, T_GUUSUU);
            if (lexeme("ぐうすー"))                         return set_info(info, T_GUUSUU);
            if (lexeme("ぐうすう"))                         return set_info(info, T_GUUSUU);
            if (lexeme("くわえろ"))                         return set_info(info, T_TASUTO);
            if (lexeme("くわえるときの"))                   return set_info(info, T_TASUTO);
            if (lexeme("くわえるとき"))                     return set_info(info, T_TASUTO);
            if (lexeme("くわえると"))                       return set_info(info, T_TASUTO);
            if (lexeme("くわえることの"))                   return set_info(info, T_TASU);
            if (lexeme("くわえる"))                         return set_info(info, T_TASU);
            if (lexeme("くわえられる"))                     return set_info(info, T_TASARERU);
            if (lexeme("くわえなさい"))                     return set_info(info, T_TASUTO);
            if (lexeme("くわえてみー"))                     return set_info(info, T_TASUTO);
            if (lexeme("くわえてみよ"))                     return set_info(info, T_TASUTO);
            if (lexeme("くわえてみい"))                     return set_info(info, T_TASUTO);
            if (lexeme("くわえてみぃ"))                     return set_info(info, T_TASUTO);
            if (lexeme("くわえてみ"))                       return set_info(info, T_TASUTO);
            if (lexeme("くわえて"))                         return set_info(info, T_TASHITE);
            if (lexeme("くわえたら"))                       return set_info(info, T_TASUTO);
            if (lexeme("くわえたときの"))                   return set_info(info, T_TASUTO);
            if (lexeme("くわえたとき"))                     return set_info(info, T_TASUTO);
            if (lexeme("くわえた"))                         return set_info(info, T_TASHITA);
            if (lexeme("くれるん"))                         return set_info(info, T_KURE);
            if (lexeme("くれるの"))                         return set_info(info, T_KURE);
            if (lexeme("くれる"))                           return set_info(info, T_KURE);
            if (lexeme("くれません"))                       return set_info(info, T_KURE);
            if (lexeme("くれ"))                             return set_info(info, T_KURE);
            if (lexeme("ください"))                         return set_info(info, T_KURE);
            if (lexeme("く"))                               return set_info(info, T_KYUU);
            if (lexeme("ぎゃくよげん"))                     return set_info(info, T_ACOS);
            if (lexeme("ぎゃくたんぜんと"))                 return set_info(info, T_ATAN);
            if (lexeme("ぎゃくたんじぇんと"))               return set_info(info, T_ATAN);
            if (lexeme("ぎゃくせーせつ"))                   return set_info(info, T_ATAN);
            if (lexeme("ぎゃくせーげん"))                   return set_info(info, T_ASIN);
            if (lexeme("ぎゃくせいせつ"))                   return set_info(info, T_ATAN);
            if (lexeme("ぎゃくせいげん"))                   return set_info(info, T_ASIN);
            if (lexeme("ぎゃくすー"))                       return set_info(info, T_GYAKUSUU);
            if (lexeme("ぎゃくすう"))                       return set_info(info, T_GYAKUSUU);
            if (lexeme("ぎゃくさいん"))                     return set_info(info, T_ASIN);
            if (lexeme("ぎゃくこさいん"))                   return set_info(info, T_ACOS);
            if (lexeme("きゅー"))                           return set_info(info, T_KYUU);
            if (lexeme("きゅう"))                           return set_info(info, T_KYUU);
            if (lexeme("きすー"))                           return set_info(info, T_KISUU);
            if (lexeme("きすう"))                           return set_info(info, T_KISUU);
            if (lexeme("が"))                               return set_info(info, T_HA);
            if (lexeme("かー"))                             return set_info(info, T_KANA);
            if (lexeme("かん"))                             return set_info(info, T_KAN);
            if (lexeme("から"))                             return set_info(info, T_KARA1);
            if (lexeme("かほー"))                           return set_info(info, T_TASHIZAN);
            if (lexeme("かほう"))                           return set_info(info, T_TASHIZAN);
            if (lexeme("かねー"))                           return set_info(info, T_KANA);
            if (lexeme("かねえ"))                           return set_info(info, T_KANA);
            if (lexeme("かねぇ"))                           return set_info(info, T_KANA);
            if (lexeme("かね"))                             return set_info(info, T_KANA);
            if (lexeme("かなー"))                           return set_info(info, T_KANA);
            if (lexeme("かなあ"))                           return set_info(info, T_KANA);
            if (lexeme("かなぁ"))                           return set_info(info, T_KANA);
            if (lexeme("かな"))                             return set_info(info, T_KANA);
            if (lexeme("かつ"))                             return set_info(info, T_KATSU);
            if (lexeme("かっこをひらく"))                   return set_info(info, T_L_PAREN);
            if (lexeme("かっこをひらいて"))                 return set_info(info, T_L_PAREN);
            if (lexeme("かっこをとじる"))                   return set_info(info, T_R_PAREN);
            if (lexeme("かっこをとじて"))                   return set_info(info, T_R_PAREN);
            if (lexeme("かっこひらく"))                     return set_info(info, T_L_PAREN);
            if (lexeme("かっこひらいて"))                   return set_info(info, T_L_PAREN);
            if (lexeme("かっことじる"))                     return set_info(info, T_R_PAREN);
            if (lexeme("かっことじて"))                     return set_info(info, T_R_PAREN);
            if (lexeme("かっことじ"))                       return set_info(info, T_R_PAREN);
            if (lexeme("かっこ"))                           return set_info(info, T_L_PAREN);
            if (lexeme("かず"))                             return set_info(info, T_MONO);
            if (lexeme("かすー"))                           return set_info(info, T_KASUU);
            if (lexeme("かすう"))                           return set_info(info, T_KASUU);
            if (lexeme("かしら"))                           return set_info(info, T_KA);
            if (lexeme("かさん"))                           return set_info(info, T_TASHIZAN);
            if (lexeme("かけろ"))                           return set_info(info, T_KAKERUTO);
            if (lexeme("かければ"))                         return set_info(info, T_KAKERUTO);
            if (lexeme("かけるときの"))                     return set_info(info, T_KAKERUTO);
            if (lexeme("かけるとき"))                       return set_info(info, T_KAKERUTO);
            if (lexeme("かけると"))                         return set_info(info, T_KAKERUTO);
            if (lexeme("かけることの"))                     return set_info(info, T_KAKERU);
            if (lexeme("かける"))                           return set_info(info, T_KAKERU);
            if (lexeme("かけられる"))                       return set_info(info, T_KAKERARERU);
            if (lexeme("かけます"))                         return set_info(info, T_KAKERU);
            if (lexeme("かけなさい"))                       return set_info(info, T_KAKERUTO);
            if (lexeme("かけてみー"))                       return set_info(info, T_KAKERUTO);
            if (lexeme("かけてみよ"))                       return set_info(info, T_KAKERUTO);
            if (lexeme("かけてみい"))                       return set_info(info, T_KAKERUTO);
            if (lexeme("かけてみぃ"))                       return set_info(info, T_KAKERUTO);
            if (lexeme("かけてみ"))                         return set_info(info, T_KAKERUTO);
            if (lexeme("かけて"))                           return set_info(info, T_KAKETE);
            if (lexeme("かけたら"))                         return set_info(info, T_KAKERUTO);
            if (lexeme("かけたときの"))                     return set_info(info, T_KAKERUTO);
            if (lexeme("かけたとき"))                       return set_info(info, T_KAKERUTO);
            if (lexeme("かけた"))                           return set_info(info, T_KAKETA);
            if (lexeme("かけざん"))                         return set_info(info, T_KAKEZAN);
            if (lexeme("かけあわせろ"))                     return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわせれば"))                   return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわせるとき"))                 return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわせると"))                   return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわせる"))                     return set_info(info, T_KAKERU);
            if (lexeme("かけあわせよ"))                     return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわせます"))                   return set_info(info, T_KAKERU);
            if (lexeme("かけあわせなさいよ"))               return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわせなさい"))                 return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわせてみー"))                 return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわせてみよ"))                 return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわせてみい"))                 return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわせてみぃ"))                 return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわせてみ"))                   return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわせて"))                     return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわせたときの"))               return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわせたとき"))                 return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわせた"))                     return set_info(info, T_KAKETA);
            if (lexeme("かけあわせ"))                       return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわすときの"))                 return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわすとき"))                   return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわす"))                       return set_info(info, T_KAKERU);
            if (lexeme("かけあわしたときの"))               return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわしたとき"))                 return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわされる"))                   return set_info(info, T_KAKERARERU);
            if (lexeme("かけ"))                             return set_info(info, T_KAKERU);
            if (lexeme("かいな"))                           return set_info(info, T_KANA);
            if (lexeme("かいじょー"))                       return set_info(info, T_KAIJOU);
            if (lexeme("かいじょう"))                       return set_info(info, T_KAIJOU);
            if (lexeme("かあ"))                             return set_info(info, T_KANA);
            if (lexeme("かぁ"))                             return set_info(info, T_KANA);
            if (lexeme("か"))                               return set_info(info, T_KA);
            if (lexeme("おーい"))                           return set_info(info, T_ETTO);
            if (lexeme("おやおや"))                         return set_info(info, T_ETTO);
            if (lexeme("おや"))                             return set_info(info, T_ETTO);
            if (lexeme("おはよーございます"))               return set_info(info, T_ETTO);
            if (lexeme("おはよー"))                         return set_info(info, T_ETTO);
            if (lexeme("おはようございます"))               return set_info(info, T_ETTO);
            if (lexeme("おはよう"))                         return set_info(info, T_ETTO);
            if (lexeme("おはよ"))                           return set_info(info, T_ETTO);
            if (lexeme("おねがいよ"))                       return set_info(info, T_ONEGAI);
            if (lexeme("おねがいやねん"))                   return set_info(info, T_ONEGAI);
            if (lexeme("おねがいね"))                       return set_info(info, T_ONEGAI);
            if (lexeme("おねがいする"))                     return set_info(info, T_ONEGAI);
            if (lexeme("おねがいしますよ"))                 return set_info(info, T_ONEGAI);
            if (lexeme("おねがいしますね"))                 return set_info(info, T_ONEGAI);
            if (lexeme("おねがいします"))                   return set_info(info, T_ONEGAI);
            if (lexeme("おねがいいたしますよ"))             return set_info(info, T_ONEGAI);
            if (lexeme("おねがいいたしますね"))             return set_info(info, T_ONEGAI);
            if (lexeme("おねがいいたします"))               return set_info(info, T_ONEGAI);
            if (lexeme("おねがい"))                         return set_info(info, T_ONEGAI);
            if (lexeme("おしえろ"))                         return set_info(info, T_OSHIETE);
            if (lexeme("おしえよ"))                         return set_info(info, T_OSHIETE);
            if (lexeme("おしえてん"))                       return set_info(info, T_OSHIETE);
            if (lexeme("おしえてみー"))                     return set_info(info, T_OSHIETE);
            if (lexeme("おしえてみい"))                     return set_info(info, T_OSHIETE);
            if (lexeme("おしえてみぃ"))                     return set_info(info, T_OSHIETE);
            if (lexeme("おしえてみ"))                       return set_info(info, T_OSHIETE);
            if (lexeme("おしえて"))                         return set_info(info, T_OSHIETE);
            if (lexeme("おしえたまへ"))                     return set_info(info, T_OSHIETE);
            if (lexeme("おしえたまえ"))                     return set_info(info, T_OSHIETE);
            if (lexeme("おく"))                             return set_info(info, T_OKU);
            if (lexeme("おおきー"))                         return set_info(info, T_OOKII);
            if (lexeme("おおきく"))                         return set_info(info, T_OOKIKU);
            if (lexeme("おおきい"))                         return set_info(info, T_OOKII);
            if (lexeme("おい"))                             return set_info(info, T_ETTO);
            if (lexeme("えー"))                             return set_info(info, T_ETTO);
            if (lexeme("えんしゅーりつ"))                   return set_info(info, T_PI);
            if (lexeme("えんしゅうりつ"))                   return set_info(info, T_PI);
            if (lexeme("えとー"))                           return set_info(info, T_ETTO);
            if (lexeme("えとねー"))                         return set_info(info, T_ETTO);
            if (lexeme("えとねえ"))                         return set_info(info, T_ETTO);
            if (lexeme("えとねぇ"))                         return set_info(info, T_ETTO);
            if (lexeme("えとね"))                           return set_info(info, T_ETTO);
            if (lexeme("えと"))                             return set_info(info, T_ETTO);
            if (lexeme("えっとー"))                         return set_info(info, T_ETTO);
            if (lexeme("えっとねー"))                       return set_info(info, T_ETTO);
            if (lexeme("えっとねえ"))                       return set_info(info, T_ETTO);
            if (lexeme("えっとねぇ"))                       return set_info(info, T_ETTO);
            if (lexeme("えっとね"))                         return set_info(info, T_ETTO);
            if (lexeme("えっとお"))                         return set_info(info, T_ETTO);
            if (lexeme("えっと"))                           return set_info(info, T_ETTO);
            if (lexeme("ええっとー"))                       return set_info(info, T_ETTO);
            if (lexeme("ええっとお"))                       return set_info(info, T_ETTO);
            if (lexeme("ええっと"))                         return set_info(info, T_ETTO);
            if (lexeme("ええ"))                             return set_info(info, T_ETTO);
            if (lexeme("うんとねー"))                       return set_info(info, T_ETTO);
            if (lexeme("うんとねえ"))                       return set_info(info, T_ETTO);
            if (lexeme("うんとねぇ"))                       return set_info(info, T_ETTO);
            if (lexeme("うんとね"))                         return set_info(info, T_ETTO);
            if (lexeme("いーなさい"))                       return set_info(info, T_OSHIETE);
            if (lexeme("いーたまへ"))                       return set_info(info, T_OSHIETE);
            if (lexeme("いーたまえ"))                       return set_info(info, T_OSHIETE);
            if (lexeme("いー"))                             return set_info(info, T_GO);
            if (lexeme("いー"))                             return set_info(info, T_E);
            if (lexeme("いんすー"))                         return set_info(info, T_YAKUSUU);
            if (lexeme("いんすう"))                         return set_info(info, T_YAKUSUU);
            if (lexeme("いや"))                             return set_info(info, T_ETTO);
            if (lexeme("いつ"))                             return set_info(info, T_GO);
            if (lexeme("いっ"))                             return set_info(info, T_ICHI);
            if (lexeme("いちばんちいさいもの"))             return set_info(info, T_MIN);
            if (lexeme("いちばんちいさいかず"))             return set_info(info, T_MIN);
            if (lexeme("いちばんおーきーもの"))             return set_info(info, T_MAX);
            if (lexeme("いちばんおーきーかず"))             return set_info(info, T_MAX);
            if (lexeme("いちばんおおきーもの"))             return set_info(info, T_MAX);
            if (lexeme("いちばんおおきーかず"))             return set_info(info, T_MAX);
            if (lexeme("いちばんおーきいもの"))             return set_info(info, T_MAX);
            if (lexeme("いちばんおーきいかず"))             return set_info(info, T_MAX);
            if (lexeme("いちばんおおきいもの"))             return set_info(info, T_MAX);
            if (lexeme("いちばんおおきいかず"))             return set_info(info, T_MAX);
            if (lexeme("いち"))                             return set_info(info, T_ICHI);
            if (lexeme("いそ"))                             return set_info(info, T_ISO);
            if (lexeme("いじょー"))                         return set_info(info, T_IJOU);
            if (lexeme("いじょう"))                         return set_info(info, T_IJOU);
            if (lexeme("いくらになるんでしょー"))           return set_info(info, T_IKURA);
            if (lexeme("いくらになるんでしょう"))           return set_info(info, T_IKURA);
            if (lexeme("いくらになるんでござる"))           return set_info(info, T_IKURA);
            if (lexeme("いくらになるんでございます"))       return set_info(info, T_IKURA);
            if (lexeme("いくらになるんでございましょー"))   return set_info(info, T_IKURA);
            if (lexeme("いくらになるんでございましょう"))   return set_info(info, T_IKURA);
            if (lexeme("いくらになるん"))                   return set_info(info, T_IKURA);
            if (lexeme("いくらになるのー"))                 return set_info(info, T_IKURA);
            if (lexeme("いくらになるのでしょー"))           return set_info(info, T_IKURA);
            if (lexeme("いくらになるのでしょう"))           return set_info(info, T_IKURA);
            if (lexeme("いくらになるのでござる"))           return set_info(info, T_IKURA);
            if (lexeme("いくらになるのでございます"))       return set_info(info, T_IKURA);
            if (lexeme("いくらになるのでございましょー"))   return set_info(info, T_IKURA);
            if (lexeme("いくらになるのでございましょう"))   return set_info(info, T_IKURA);
            if (lexeme("いくらになるの"))                   return set_info(info, T_IKURA);
            if (lexeme("いくらになるでしょー"))             return set_info(info, T_IKURA);
            if (lexeme("いくらになるでしょう"))             return set_info(info, T_IKURA);
            if (lexeme("いくらになる"))                     return set_info(info, T_IKURA);
            if (lexeme("いくらになります"))                 return set_info(info, T_IKURA);
            if (lexeme("いくらなん"))                       return set_info(info, T_IKURA);
            if (lexeme("いくらなのー"))                     return set_info(info, T_IKURA);
            if (lexeme("いくらなの"))                       return set_info(info, T_IKURA);
            if (lexeme("いくら"))                           return set_info(info, T_IKURA);
            if (lexeme("いくつー"))                         return set_info(info, T_IKURA);
            if (lexeme("いくつになるんでしょー"))           return set_info(info, T_IKURA);
            if (lexeme("いくつになるんでしょう"))           return set_info(info, T_IKURA);
            if (lexeme("いくつになるんでござる"))           return set_info(info, T_IKURA);
            if (lexeme("いくつになるんでございます"))       return set_info(info, T_IKURA);
            if (lexeme("いくつになるんでございましょー"))   return set_info(info, T_IKURA);
            if (lexeme("いくつになるんでございましょう"))   return set_info(info, T_IKURA);
            if (lexeme("いくつになるん"))                   return set_info(info, T_IKURA);
            if (lexeme("いくつになるのでしょー"))           return set_info(info, T_IKURA);
            if (lexeme("いくつになるのでしょう"))           return set_info(info, T_IKURA);
            if (lexeme("いくつになるのでござる"))           return set_info(info, T_IKURA);
            if (lexeme("いくつになるのでございます"))       return set_info(info, T_IKURA);
            if (lexeme("いくつになるのでございましょー"))   return set_info(info, T_IKURA);
            if (lexeme("いくつになるのでございましょう"))   return set_info(info, T_IKURA);
            if (lexeme("いくつになるの"))                   return set_info(info, T_IKURA);
            if (lexeme("いくつになるでしょー"))             return set_info(info, T_IKURA);
            if (lexeme("いくつになるでしょう"))             return set_info(info, T_IKURA);
            if (lexeme("いくつになる"))                     return set_info(info, T_IKURA);
            if (lexeme("いくつになります"))                 return set_info(info, T_IKURA);
            if (lexeme("いくつなん"))                       return set_info(info, T_IKURA);
            if (lexeme("いくつなのー"))                     return set_info(info, T_IKURA);
            if (lexeme("いくつなの"))                       return set_info(info, T_IKURA);
            if (lexeme("いくつ"))                           return set_info(info, T_IKURA);
            if (lexeme("いか"))                             return set_info(info, T_IKA);
            if (lexeme("いお"))                             return set_info(info, T_IO);
            if (lexeme("いうて"))                           return set_info(info, T_OSHIETE);
            if (lexeme("いいなさい"))                       return set_info(info, T_OSHIETE);
            if (lexeme("いいたまへ"))                       return set_info(info, T_OSHIETE);
            if (lexeme("いいたまえ"))                       return set_info(info, T_OSHIETE);
            if (lexeme("いい"))                             return set_info(info, T_GO);
            if (lexeme("あーくたんぜんと"))                 return set_info(info, T_ATAN);
            if (lexeme("あーくたんじぇんと"))               return set_info(info, T_ATAN);
            if (lexeme("あーくさいん"))                     return set_info(info, T_ASIN);
            if (lexeme("あーくこさいん"))                   return set_info(info, T_ACOS);
            if (lexeme("あれー"))                           return set_info(info, T_ETTO);
            if (lexeme("あれえ"))                           return set_info(info, T_ETTO);
            if (lexeme("あれぇ"))                           return set_info(info, T_ETTO);
            if (lexeme("あるの"))                           return set_info(info, T_ARI);
            if (lexeme("ある"))                             return set_info(info, T_ARI);
            if (lexeme("ありんす"))                         return set_info(info, T_ARI);
            if (lexeme("ありませんの"))                     return set_info(info, T_NAI);
            if (lexeme("ありません"))                       return set_info(info, T_NAI);
            if (lexeme("ありますの"))                       return set_info(info, T_ARI);
            if (lexeme("あります"))                         return set_info(info, T_ARI);
            if (lexeme("あり"))                             return set_info(info, T_ARI);
            if (lexeme("あまる"))                           return set_info(info, T_AMARU);
            if (lexeme("あまります"))                       return set_info(info, T_AMARU);
            if (lexeme("あまり"))                           return set_info(info, T_AMARI);
            if (lexeme("あのー"))                           return set_info(info, T_ETTO);
            if (lexeme("あのねー"))                         return set_info(info, T_ETTO);
            if (lexeme("あのねえ"))                         return set_info(info, T_ETTO);
            if (lexeme("あのねぇ"))                         return set_info(info, T_ETTO);
            if (lexeme("あのね"))                           return set_info(info, T_ETTO);
            if (lexeme("あのお"))                           return set_info(info, T_ETTO);
            if (lexeme("あのう"))                           return set_info(info, T_ETTO);
            if (lexeme("あっ"))                             return set_info(info, T_ETTO);
            if (lexeme("あたい"))                           return set_info(info, T_KOTAE);
            if (lexeme("あそーぎ"))                         return set_info(info, T_ASOUGI);
            if (lexeme("あそうぎ"))                         return set_info(info, T_ASOUGI);
            if (lexeme("あ"))                               return set_info(info, T_ETTO);
            if (lexeme("〕"))                               return set_info(info, T_R_PAREN);
            if (lexeme("〔"))                               return set_info(info, T_L_PAREN);
            if (lexeme("】"))                               return set_info(info, T_R_PAREN);
            if (lexeme("【"))                               return set_info(info, T_L_PAREN);
            if (lexeme("』"))                               return set_info(info, T_R_PAREN);
            if (lexeme("『"))                               return set_info(info, T_L_PAREN);
            if (lexeme("」"))                               return set_info(info, T_R_PAREN);
            if (lexeme("「"))                               return set_info(info, T_L_PAREN);
            if (lexeme("》"))                               return set_info(info, T_R_PAREN);
            if (lexeme("《"))                               return set_info(info, T_L_PAREN);
            if (lexeme("〉"))                               return set_info(info, T_R_PAREN);
            if (lexeme("〈"))                               return set_info(info, T_L_PAREN);
            if (lexeme("〇"))                               return set_info(info, T_ZERO);
            if (lexeme("。"))                               return set_info(info, T_PERIOD);
            if (lexeme("、"))                               return set_info(info, T_COMMA);
            if (lexeme("☆"))                               return set_info(info, T_PERIOD);
            if (lexeme("★"))                               return set_info(info, T_PERIOD);
            if (lexeme("●"))                               return set_info(info, T_PERIOD);
            if (lexeme("◎"))                               return set_info(info, T_PERIOD);
            if (lexeme("√"))                               return set_info(info, T_ROOT);
            if (lexeme("÷"))                               return set_info(info, T_WARU);
            if (lexeme("×"))                               return set_info(info, T_KAKERU);
            if (lexeme("~"))                                return set_info(info, T_KARA1);
            if (lexeme("}"))                                return set_info(info, T_R_PAREN);
            if (lexeme("{"))                                return set_info(info, T_L_PAREN);
            if (lexeme("tanh"))                             return set_info(info, T_TANH);
            if (lexeme("tan"))                              return set_info(info, T_TAN);
            if (lexeme("sinh"))                             return set_info(info, T_SINH);
            if (lexeme("sin"))                              return set_info(info, T_SIN);
            if (lexeme("log10"))                            return set_info(info, T_LOG10);
            if (lexeme("log"))                              return set_info(info, T_LOG);
            if (lexeme("ln"))                               return set_info(info, T_LOG);
            if (lexeme("lcm"))                              return set_info(info, T_LCM);
            if (lexeme("gcd"))                              return set_info(info, T_GCD);
            if (lexeme("fabs"))                             return set_info(info, T_ZETTAICHI);
            if (lexeme("exp"))                              return set_info(info, T_EXP);
            if (lexeme("e"))                                return set_info(info, T_E);
            if (lexeme("cosh"))                             return set_info(info, T_COSH);
            if (lexeme("cos"))                              return set_info(info, T_COS);
            if (lexeme("atan"))                             return set_info(info, T_ATAN);
            if (lexeme("asin"))                             return set_info(info, T_ASIN);
            if (lexeme("arctan"))                           return set_info(info, T_ATAN);
            if (lexeme("arcsin"))                           return set_info(info, T_ASIN);
            if (lexeme("arccos"))                           return set_info(info, T_ACOS);
            if (lexeme("acos"))                             return set_info(info, T_ACOS);
            if (lexeme("abs"))                              return set_info(info, T_ZETTAICHI);
            if (lexeme("^"))                                return set_info(info, T_POWER);
            if (lexeme("]"))                                return set_info(info, T_R_PAREN);
            if (lexeme("["))                                return set_info(info, T_L_PAREN);
            if (lexeme("LCM"))                              return set_info(info, T_LCM);
            if (lexeme("GCD"))                              return set_info(info, T_GCD);
            if (lexeme("?"))                                return set_info(info, T_PERIOD);
            if (lexeme("=="))                               return set_info(info, T_HA);
            if (lexeme("="))                                return set_info(info, T_HA);
            if (lexeme("/"))                                return set_info(info, T_WARU);
            if (lexeme("."))                                return set_info(info, T_PERIOD);
            if (lexeme("-"))                                return set_info(info, T_MINUS);
            if (lexeme(","))                                return set_info(info, T_COMMA);
            if (lexeme("+"))                                return set_info(info, T_TASU);
            if (lexeme("**"))                               return set_info(info, T_POWER);
            if (lexeme("*"))                                return set_info(info, T_KAKERU);
            if (lexeme(")"))                                return set_info(info, T_R_PAREN);
            if (lexeme("("))                                return set_info(info, T_L_PAREN);
            if (lexeme("%"))                                return set_info(info, T_PERCENT);
            if (lexeme("!"))                                return set_info(info, T_FACTORIAL);
            // 【ここまで】行を降順に並び替えておく。

            char ch = getch();
            if (ch == EOF)
                return commit_token(eof);

            message("ことばがわかりません。");

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
            size_t n = 1;
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
        size_t             m_index;
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

        // 昔の数の数え方を今風に変換する。
        // 「かすう」「ひかすう」などを「たすかず」「たされるかず」などに変換する。
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
                    if (it->text() == "や")
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

        // T_PERIOD, T_COMMAを整理して、T_MIRUTO, T_ALLを削除する。
        // T_SONOを「T_SORE, T_NO1」に置き換える。
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

        // 括弧の対応を修正する。
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
                        message("かっこのたいおうがおかしいです。");
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

        // 「の」をT_NO1, T_NO2, ..., T_NO7に分類する。
        // 数の単位「穣」をT_JOU2に分類する。
        // T_NO2: 「わ」「さ」「せき」「しょう」の「の」。
        // T_NO3: 「何の何倍」の「の」。「何の（...）」の「の」。
        // T_NO4: 「かけざん」「けいさん」「こたえ」などの直前の「の」。
        // T_NO5: 「のたすかず」「のたされるかず」「のかけるかず」の「の」
        // T_NO6: 「の何乗」「の平方」「の立方」の「の」。
        // T_NO7: 三角関数「正弦」「余弦」「正接」の直前の「の」。
        // T_NO8: 「の何ぱーせんと」の「の」。
        // T_NO9: 「の余り」の「の」。
        // T_NO10: 倍数、約数、素因数の直前の「の」。
        // T_NO11: 素因数分解の直前の「の」。
        // T_NO1: それ以外。
        void resynth3(std::vector<info_type>& infos)
        {
            std::vector<info_type> newinfos;
            std::vector<info_type> subinfos;
            std::vector<info_type>::iterator it = infos.begin();
            std::vector<info_type>::iterator end = infos.end();
            int nest;
            size_t no1 = std::string::npos;    // position of T_NO1
            size_t jou1 = std::string::npos;   // position of T_JOU1
            size_t wo1 = std::string::npos;    // position of T_WO1
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
                case T_INTQUOT:
                case T_MAX:
                case T_MIN:
                case T_AVERAGE:
                case T_GCD:
                case T_LCM:
                    if (no1 != std::string::npos)
                    {
                        (newinfos.begin() + no1)->set_token(T_NO2);
                        no1 = std::string::npos;
                    }
                    jou1 = std::string::npos;
                    break;

                case T_BAISUU:
                case T_YAKUSUU:
                case T_SOINSUU:
                case T_JIMEINAYAKUSUU:
                case T_SHINNOYAKUSUU:
                case T_KOUBAISUU:
                case T_KOUYAKUSUU:
                    if (no1 != std::string::npos)
                    {
                        (newinfos.begin() + no1)->set_token(T_NO10);
                        no1 = std::string::npos;
                    }
                    jou1 = std::string::npos;
                    break;

                case T_SOINSUUBUNKAI:
                    if (no1 != std::string::npos)
                    {
                        (newinfos.begin() + no1)->set_token(T_NO11);
                        no1 = std::string::npos;
                    }
                    jou1 = std::string::npos;
                    break;

                case T_KAIJOU:
                case T_HEIHOUKON:
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
                case T_WARIBIKI:
                case T_WARIMASHI:
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

        // 小数点の後の数字にしるしをつける。
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

        // 「と」をT_TO1, T_TO2に分類する。
        // T_TO2: 「なんとなんぶんのなに」の「と」。
        // T_TO1: それ以外。
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

        // 特定の語句の直後のT_COMMAを削除する。
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
                case T_DE1:
                case T_DE2:
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
                case T_KARA1:
                case T_KARA2:
                case T_KEISAN:
                case T_KOTAE:
                case T_L_PAREN:
                case T_MADE:
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
                case T_NO10:
                case T_NOUCHI:
                case T_OSHIETE:
                case T_PERCENT:
                case T_PLUS:
                case T_PROD:
                case T_QUOT:
                case T_INTQUOT:
                case T_SHITA:
                case T_SHITE:
                case T_SUM:
                case T_SURUTO:
                case T_TAGAINISO:
                case T_TASHITE:
                case T_TASHIZAN:
                case T_TASU:
                case T_TASUTO:
                case T_TO1:
                case T_WARIBIKI:
                case T_WARIMASHI:
                case T_WARIZAN:
                case T_WARU:
                case T_WARUTO:
                case T_WATTE:
                case T_WO1:
                case T_WO2:
                //case T_NI:    // NOTE: T_NIは、数字かもしれない。
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

        // 「計算し」「をし」などの「し」を修正する。
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
                case T_SEIJO:
                    flag = true;
                    break;

                case T_YON:
                    if (flag && it->text() == "し")
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

        // 「を」をT_WO1とT_WO2に分類する。
        // T_WO2: 「たしざん」「けいさん」などの直後の「を」、または
        //        「せいじょ」の前の「を」。
        // T_WO1: そのほかの「を」。
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
                    if (flag || (it + 1)->get_token() == T_SEIJO)
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

        // 「を」の直後に「けいさん」「たしざん」などがあれば、
        // 「を」を「の」に変える。
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

        // 「たした。」「ひいた。」「かけた。」「わった。」の
        // 「。」を「、」に変える。
        // 「たした～」「ひいた～」「かけた～」「わった～」を
        // 「たしたら～」「ひいたら～」「かけたら～」「わったら～」に変える。
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
				if (it->get_token() != eof &&
					((it + 1)->get_token() == T_COMMA ||
                     (it + 1)->get_token() == T_KOTAE ||
                     (it + 1)->get_token() == T_AMARI ||
                     (it + 1)->get_token() == T_SORE))
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

        // 特定の語句直前の「、」を取り除く。
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
                    case T_DE1:
                    case T_DE2:
                    case T_DIFF:
                    case T_HEIHOUKON:
                    case T_HIITA:
                    case T_HIITE:
                    case T_HIKIZAN:
                    case T_HIKU:
                    case T_HIKUTO:
                    case T_IKURA:
                    case T_KAIJOU:
                    case T_KAKERU:
                    case T_KAKERUTO:
                    case T_KAKETA:
                    case T_KAKETE:
                    case T_KAKEZAN:
                    case T_KANA:
                    case T_KARA1:
                    case T_KARA2:
                    case T_KEISAN:
                    case T_KOTAE:
                    case T_MADE:
                    case T_NOUCHI:
                    case T_ONEGAI:
                    case T_OSHIETE:
                    case T_PERCENT:
                    case T_PROD:
                    case T_INTQUOT:
                    case T_QUOT:
                    case T_R_PAREN:
                    case T_SHITA:
                    case T_SHITE:
                    case T_SUM:
                    case T_SURUTO:
                    case T_TAGAINISO:
                    case T_TASHITA:
                    case T_TASHITE:
                    case T_TASHIZAN:
                    case T_TASU:
                    case T_TASUTO:
                    case T_WARIBIKI:
                    case T_WARIMASHI:
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

        // 「それのかず」を「それ」に変換する。
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

        // T_OSHIETE/T_WAKARAN/T_KURE/T_IKURA/T_WARIKIRU
        // の後のT_KAをT_KANAにする。
        // 文末の「か」を「かな」にする。
        void resynth13(std::vector<info_type>& infos)
        {
            std::vector<info_type>::iterator it = infos.begin();
            std::vector<info_type>::iterator end = infos.end();
            for (; it != end; ++it)
            {
                switch (it->get_token())
                {
                case T_OSHIETE:
                case T_WAKARAN:
                case T_KURE:
                case T_IKURA:
                case T_WARIKIRU:
                    if ((it + 1)->get_token() == T_KA)
                        (it + 1)->set_token(T_KANA);
                    break;

                case T_KA:
                    if ((it + 1)->get_token() == T_PERIOD)
                        it->set_token(T_KANA);
                    break;

                default:
                    break;
                }
            }
        }

        // 文末の「たす」「ひく」「かける」「わる」を
        // 「たすと」「ひくと」「かけると」「わると」にする。
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

        // 式の中ではない、文章の中の「のかいじょう」の「の」(T_NO6)をT_NO4にする。
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
                case T_BAISUU:
                case T_COS:
                case T_COUNT:
                case T_DIFF:
                case T_EXP:
                case T_GCD:
                case T_GOUSEISUU:
                case T_GYAKUSUU:
                case T_HEIHOU:
                case T_HEIHOUKON:
                case T_HIKIZAN:
                case T_KAIJOU:
                case T_KAKEZAN:
                case T_KEISAN:
                case T_KOTAE:
                case T_KOUBAISUU:
                case T_KOUYAKUSUU:
                case T_LCM:
                case T_LOG10:
                case T_LOG:
                case T_MAX:
                case T_MIN:
                case T_MONO:
                case T_PROD:
                case T_INTQUOT:
                case T_QUOT:
                case T_RIPPOU:
                case T_ROOT:
                case T_SHIZENSUU:
                case T_SIN:
                case T_SINH:
                case T_SOINSUU:
                case T_SOINSUUBUNKAI:
                case T_SUM:
                case T_TAN:
                case T_TASHIZAN:
                case T_WARIZAN:
                case T_YAKUSUU:
                case T_ZETTAICHI:
                    if ((it + 1)->get_token() == T_NO6 &&
                        ((it + 2)->get_token() == T_KAIJOU ||
                         (it + 2)->get_token() == T_HEIHOUKON))
                    {
                        (it + 1)->set_token(T_NO4);
                    }
                    break;

                default:
                    break;
                }
            }
        }

        // T_TASUTO, T_HIKUTO, T_KAKERUTO, T_WARUTOの後のT_SURUTOを取り除く。
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

        // T_HA T_DEARUをT_HA T_IKURAに変換。
        void resynth17(std::vector<info_type>& infos)
        {
            std::vector<info_type>::iterator it = infos.begin();
            std::vector<info_type>::iterator end = infos.end();
            for (; it != end; ++it)
            {
                if (it->get_token() == T_HA &&
                    (it + 1)->get_token() == T_DEARU)
                {
                    (it + 1)->set_token(T_IKURA);
                }
            }
        }

        // T_DEARU T_PERIODをT_KANA T_PERIODに変換。
        void resynth18(std::vector<info_type>& infos)
        {
            std::vector<info_type>::iterator it = infos.begin();
            std::vector<info_type>::iterator end = infos.end();
            for (; it != end; ++it)
            {
                if (it->get_token() == T_DEARU &&
                    (it + 1)->get_token() == T_PERIOD)
                {
                    it->set_token(T_KANA);
                }
            }
        }

        // T_DEARU T_KANAをT_KANAに変換。
        void resynth19(std::vector<info_type>& infos)
        {
            std::vector<info_type> newinfos;
            std::vector<info_type>::iterator it = infos.begin();
            std::vector<info_type>::iterator end = infos.end();
            for (; it != end; ++it)
            {
                if (it->get_token() == T_DEARU &&
                    (it + 1)->get_token() == T_KANA)
                {
                    continue;
                }
                newinfos.push_back(*it);
            }
            infos = newinfos;
        }

        // T_MADEの前のT_KARA1をT_KARA2に変換。
        void resynth20(std::vector<info_type>& infos)
        {
            std::vector<info_type>::iterator it = infos.begin();
            std::vector<info_type>::iterator end = infos.end();
            std::vector<info_type>::iterator kara = infos.end();
            for (; it != end; ++it)
            {
                switch (it->get_token())
                {
                case T_KARA1:
                    kara = it;
                    break;

                case T_MADE:
                    if (kara != infos.end())
                    {
                        kara->set_token(T_KARA2);
                        kara = infos.end();
                    }
                    break;

                default:
                    break;
                }
            }
        }

        // T_SHIZENSUU, T_SEISUU, T_GUUSUU, T_KISUU, T_JISSUU,
        // T_MONO, T_SOSUU, T_GOUSEISUU, T_JIMEINAYAKUSUUの前後のT_TO1をT_KAに変換する。
        void resynth21(std::vector<info_type>& infos)
        {
            std::vector<info_type>::iterator it = infos.begin();
            std::vector<info_type>::iterator end = infos.end();
            for (; it != end; ++it)
            {
                switch (it->get_token())
                {
                case T_SHIZENSUU:
                case T_SEISUU:
                case T_GUUSUU:
                case T_KISUU:
                case T_JISSUU:
                case T_SOSUU:
                case T_GOUSEISUU:
                case T_JIMEINAYAKUSUU:
                    if ((it + 1)->get_token() == T_TO1)
                        (it + 1)->set_token(T_KA);
                    break;

                case T_TO1:
                    switch ((it + 1)->get_token())
                    {
                    case T_SHIZENSUU:
                    case T_SEISUU:
                    case T_GUUSUU:
                    case T_KISUU:
                    case T_JISSUU:
                    case T_SOSUU:
                    case T_GOUSEISUU:
                    case T_JIMEINAYAKUSUU:
                        it->set_token(T_KA);
                        break;

                    default:
                        break;
                    }
                    break;

                default:
                    break;
                }
            }
        }

        // T_NANOHA --> T_DEARU T_MONO T_HA.
        // T_WARUTO T_WARIKIRU --> T_WARIKIRU.
        // T_MADE T_NO2 T_SUM -->T_MADE T_NO1 T_SEISUU T_NO2 T_SUM.
        // T_MADE T_NO2 T_PROD -->T_MADE T_NO1 T_SEISUU T_NO2 T_PROD.
        // ... T_NO1 T_HA -> ... T_MONO T_HA.
        void resynth22(std::vector<info_type>& infos)
        {
            std::vector<info_type> newinfos;
            std::vector<info_type>::iterator it = infos.begin();
            std::vector<info_type>::iterator end = infos.end();
            for (; it != end; ++it)
            {
                Token token = it->get_token();
                if (token == T_MADE &&
                    (it + 1)->get_token() == T_NO2 &&
                    ((it + 2)->get_token() == T_SUM ||
                     (it + 2)->get_token() == T_PROD))
                {
                    newinfos.push_back(*it);
                    ++it;   // T_NO2
                    it->set_token(T_NO1);       newinfos.push_back(*it);
                    it->set_token(T_SEISUU);    newinfos.push_back(*it);
                    it->set_token(T_NO2);       newinfos.push_back(*it);
                    ++it;   // T_SUM or T_PROD
                }
                else if ((token == T_CHIISAI || token == T_OOKII ||
                          token == T_DEARU || token == T_NIHITOSHII) &&
                          (it + 1)->get_token() == T_NO1 &&
                          (it + 2)->get_token() == T_HA)
                {
                    newinfos.push_back(*it);
                    ++it;
                    it->set_token(T_MONO);      newinfos.push_back(*it);
                    ++it;
                }
                else if ((token == T_CHIISAI || token == T_OOKII ||
                          token == T_DEARU || token == T_NIHITOSHII) &&
                          (it + 1)->get_token() == T_NO1 &&
                          (it + 2)->get_token() == T_SUM &&
                          (it + 2)->text() == "わ")
                {
                    newinfos.push_back(*it);
                    ++it;
                    it->set_token(T_MONO);      newinfos.push_back(*it);
                    ++it;
                }
                else if (token == T_WARUTO &&
                         (it + 1)->get_token() == T_WARIKIRU)
                {
                    ++it;
                }
                else if (token == T_NANOHA)
                {
                    it->set_token(T_DEARU);
                    newinfos.push_back(*it);
                    it->set_token(T_MONO);
                    newinfos.push_back(*it);
                    it->set_token(T_HA);
                }
                newinfos.push_back(*it);
            }
            infos = newinfos;
        }

        // 助詞「は」が「わ」になっていたときに修正。
        void resynth23(std::vector<info_type>& infos)
        {
            std::vector<info_type>::iterator it = infos.begin();
            std::vector<info_type>::iterator end = infos.end();
            std::vector<info_type>::iterator prev = end;
            for (; it != end; ++it)
            {
                switch (it->get_token())
                {
                case T_SUM:
                    if (prev != end && it->text() == "わ")
                    {
                        switch (prev->get_token())
                        {
                        case T_NO2:
                        case T_TASUTO:
                        case T_SURUTO:
                            break;

                        default:
                            it->set_token(T_HA);
                            break;
                        }
                    }
                    break;

                default:
                    break;
                }
                prev = it;
            }
        }

        // 「えっと」などの感動詞を取り除く。
        void resynth24(std::vector<info_type>& infos)
        {
            std::vector<info_type> newinfos;
            std::vector<info_type>::iterator it = infos.begin();
            std::vector<info_type>::iterator end = infos.end();
            for (; it != end; ++it)
            {
                if (it->get_token() == T_ETTO)
                {
                    if ((it + 1)->get_token() == T_PERIOD)
                        ++it;
                    else if ((it + 1)->get_token() == T_COMMA)
                        ++it;
                    continue;
                }

                newinfos.push_back(*it);
            }
            infos = newinfos;
        }

        // 「たします。すると」などを「たすと」などに変換する。
        void resynth25(std::vector<info_type>& infos)
        {
            std::vector<info_type> newinfos;
            std::vector<info_type>::iterator it = infos.begin();
            std::vector<info_type>::iterator end = infos.end();
            for (; it != end; ++it)
            {
                if (it->get_token() != eof &&
                    ((it + 1)->get_token() == T_PERIOD ||
                     (it + 1)->get_token() == T_COMMA))
                {
                    switch (it->get_token())
                    {
                    case T_TASU:
                        if ((it + 2)->get_token() == T_SURUTO)
                        {
                            it->set_token(T_TASUTO);
                            newinfos.push_back(*it);
                            ++it;
                            ++it;
                            continue;
                        }
                        break;

                    case T_HIKU:
                        if ((it + 2)->get_token() == T_SURUTO)
                        {
                            it->set_token(T_HIKUTO);
                            newinfos.push_back(*it);
                            ++it;
                            ++it;
                            continue;
                        }
                        break;

                    case T_KAKERU:
                        if ((it + 2)->get_token() == T_SURUTO)
                        {
                            it->set_token(T_KAKERUTO);
                            newinfos.push_back(*it);
                            ++it;
                            ++it;
                            continue;
                        }
                        break;

                    case T_WARU:
                        if ((it + 2)->get_token() == T_SURUTO)
                        {
                            it->set_token(T_WARUTO);
                            newinfos.push_back(*it);
                            ++it;
                            ++it;
                            continue;
                        }
                        break;

                    default:
                        break;
                    }
                }
                newinfos.push_back(*it);
            }
            infos = newinfos;
        }

        // 「ぐうすう」「きすう」「しぜんすう」などの後の「を」を
        // 「は」に変える。
        void resynth26(std::vector<info_type>& infos)
        {
            std::vector<info_type>::iterator it = infos.begin();
            std::vector<info_type>::iterator end = infos.end();
            for (; it != end; ++it)
            {
                switch (it->get_token())
                {
                case T_SHIZENSUU:
                case T_SEISUU:
                case T_GUUSUU:
                case T_KISUU:
                case T_JISSUU:
                case T_SOSUU:
                case T_GOUSEISUU:
                case T_KOUBAISUU:
                case T_KOUYAKUSUU:
                case T_BAISUU:
                case T_YAKUSUU:
                case T_SOINSUU:
                case T_JIMEINAYAKUSUU:
                case T_SHINNOYAKUSUU:
                    if ((it + 1)->get_token() == T_WO1)
                    {
                        bool flag = false;
                        std::vector<info_type>::iterator it2 = it;
                        for (; it2 != end; ++it2)
                        {
                            if (it2->get_token() == T_DE1 &&
                                (it2 + 1)->get_token() == T_WATTA &&
                                (it2 + 2)->get_token() == T_MONO)
                            {
                                flag = true;
                                break;
                            }
                        }
                        if (!flag)
                            (it + 1)->set_token(T_HA);
                    }
                    break;

                default:
                    break;
                }
            }
        }

        // 「わりきる」の前の「で」をT_DE2に変える。
        // T_WO1 T_WARUTOの前のT_DE1をT_DE3に変える。
        void resynth27(std::vector<info_type>& infos)
        {
            std::vector<info_type>::iterator it = infos.begin();
            std::vector<info_type>::iterator end = infos.end();
            for (; it != end; ++it)
            {
                switch (it->get_token())
                {
                case T_DE1:
                    if ((it + 1)->get_token() == T_WARIKIRU)
                    {
                        it->set_token(T_DE2);
                    }
                    break;

                default:
                    break;
                }
            }
            for (it = infos.begin(); it != end; ++it)
            {
                if (it->get_token() == T_WO1 &&
                    (it + 1)->get_token() == T_WARUTO)
                {
                    std::vector<info_type>::iterator it2 = it;
                    for (; it2 != infos.begin(); --it2)
                    {
                        if (it2->get_token() == T_DE1)
                        {
                            it2->set_token(T_DE3);
                            break;
                        }
                    }
                }
            }
        }

    private:
        // NOTE: Scanner is not copyable.
        Scanner(const Scanner<Iterator, ParserSite>&);
        Scanner& operator=(const Scanner<Iterator, ParserSite>&);
    }; // class Scanner<Iterator, ParserSite>
} // namespace Calc_H

#endif  // ndef HSCANNER_H_
