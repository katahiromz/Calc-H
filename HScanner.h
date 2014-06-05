// HScanner.h
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

            resynth1(infos);
            resynth2(infos);
            resynth3(infos);
            resynth4(infos);
            resynth5(infos);
            resynth6(infos);
            resynth7(infos);
            resynth8(infos);
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
        }

        Token get_token(TokenValue& info)
        {
            int space_count = 0;
            for (;;)
            {
                bool flag = false;
                while (lexeme(" ") || lexeme("　") || lexeme("\t"))
                {
                    space_count++;
                    flag = true;
                }

                while (lexeme("えっと") || lexeme("ええっと") ||
                       lexeme("えっとー") || lexeme("ええっとー") ||
                       lexeme("あのね") || lexeme("あのー") || 
                       lexeme("あ、") || lexeme("あっ") ||
                       lexeme("うんとね、") || lexeme("えっとね、") || 
                       lexeme("うんとねー、") || lexeme("えっとねー、") || 
                       lexeme("とんで") || lexeme("そうそう"))
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

            if (lexeme("これ"))                     return (T_SORE);
            if (lexeme("それ"))                     return (T_SORE);

            if (lexeme("いんいちが"))               return (T_1X1);
            if (lexeme("いんにが"))                 return (T_1X2);
            if (lexeme("いんさんが"))               return (T_1X3);
            if (lexeme("いんしが"))                 return (T_1X4);
            if (lexeme("いんごが"))                 return (T_1X5);
            if (lexeme("いんろくが"))               return (T_1X6);
            if (lexeme("いんひちが"))               return (T_1X7);
            if (lexeme("いんしちが"))               return (T_1X7);
            if (lexeme("いんはちが"))               return (T_1X8);
            if (lexeme("いんくが"))                 return (T_1X9);
            if (lexeme("にいちが"))                 return (T_2X1);
            if (lexeme("ににんが"))                 return (T_2X2);
            if (lexeme("にさんが"))                 return (T_2X3);
            if (lexeme("にしが"))                   return (T_2X4);
            //if (lexeme("にご"))                     return (T_2X5);   // T_NI T_GO
            if (lexeme("にろく"))                   return (T_2X6);
            if (lexeme("にひち"))                   return (T_2X7);
            if (lexeme("にしち"))                   return (T_2X7);
            if (lexeme("にはち"))                   return (T_2X8);
            //if (lexeme("にく"))                     return (T_2X9);   // T_NI T_KYUU
            if (lexeme("さんいちが"))               return (T_3X1);
            if (lexeme("さんにが"))                 return (T_3X2);
            if (lexeme("さざんが"))                 return (T_3X3);
            if (lexeme("さんし"))                   return (T_3X4);
            if (lexeme("さんご"))                   return (T_3X5);
            if (lexeme("さぶろく"))                 return (T_3X6);
            if (lexeme("さんひち"))                 return (T_3X7);
            if (lexeme("さんしち"))                 return (T_3X7);
            if (lexeme("さんぱ"))                   return (T_3X8);
            if (lexeme("さんく"))                   return (T_3X9);
            if (lexeme("しいちが"))                 return (T_4X1);
            if (lexeme("しにが"))                   return (T_4X2);
            if (lexeme("しさん"))                   return (T_4X3);
            if (lexeme("しし"))                     return (T_4X4);
            if (lexeme("しご"))                     return (T_4X5);
            if (lexeme("しろく"))                   return (T_4X6);
            if (lexeme("しひち"))                   return (T_4X7);
            if (lexeme("ししち"))                   return (T_4X7);
            if (lexeme("しわ"))                     return (T_4X8);
            if (lexeme("しは"))                     return (T_4X8);
            if (lexeme("しく"))                     return (T_4X9);
            if (lexeme("ごいちが"))                 return (T_5X1);
            if (lexeme("ごに"))                     return (T_5X2);
            if (lexeme("ごさん"))                   return (T_5X3);
            if (lexeme("ごし"))                     return (T_5X4);
            if (lexeme("ごご"))                     return (T_5X5);
            if (lexeme("ごろく"))                   return (T_5X6);
            if (lexeme("ごひち"))                   return (T_5X7);
            if (lexeme("ごしち"))                   return (T_5X7);
            //if (lexeme("ごは"))                     return (T_5X8);   // T_GO T_HA
            if (lexeme("ごっく"))                   return (T_5X9);
            if (lexeme("ろくいちが"))               return (T_6X1);
            //if (lexeme("ろくに"))                   return (T_6X2);   // T_ROKU T_NI
            if (lexeme("ろくさん"))                 return (T_6X3);
            if (lexeme("ろくし"))                   return (T_6X4);
            if (lexeme("ろくご"))                   return (T_6X5);
            if (lexeme("ろくろく"))                 return (T_6X6);
            if (lexeme("ろくひち"))                 return (T_6X7);
            if (lexeme("ろくしち"))                 return (T_6X7);
            //if (lexeme("ろくは"))                   return (T_6X8);   // T_ROKU T_HA
            if (lexeme("ろくはち"))                 return (T_6X8);
            if (lexeme("ろっく"))                   return (T_6X9);
            if (lexeme("ひちいちが"))               return (T_7X1);
            //if (lexeme("ひちに"))                   return (T_7X2);   // T_NANA T_NI
            if (lexeme("ひちさん"))                 return (T_7X3);
            if (lexeme("ひちし"))                   return (T_7X4);
            if (lexeme("ひちご"))                   return (T_7X5);
            if (lexeme("ひちろく"))                 return (T_7X6);
            if (lexeme("ひちひち"))                 return (T_7X7);
            //if (lexeme("ひちは"))                   return (T_7X8);   // T_NANA T_HA
            if (lexeme("ひちはち"))                 return (T_7X8);
            if (lexeme("ひちく"))                   return (T_7X9);
            if (lexeme("しちいちが"))               return (T_7X1);
            //if (lexeme("しちに"))                   return (T_7X2);   // T_NANA T_NI
            if (lexeme("しちさん"))                 return (T_7X3);
            if (lexeme("しちし"))                   return (T_7X4);
            if (lexeme("しちご"))                   return (T_7X5);
            if (lexeme("しちろく"))                 return (T_7X6);
            if (lexeme("しちしち"))                 return (T_7X7);
            //if (lexeme("しちは"))                   return (T_7X8);   // T_NANA T_HA
            if (lexeme("しちはち"))                 return (T_7X8);
            if (lexeme("しちく"))                   return (T_7X9);
            if (lexeme("はちいちが"))               return (T_8X1);
            //if (lexeme("はちに"))                   return (T_8X2);   // T_HACHI T_NI
            if (lexeme("はちさん"))                 return (T_8X3);
            if (lexeme("はっさん"))                 return (T_8X3);
            if (lexeme("はっし"))                   return (T_8X4);
            if (lexeme("はちご"))                   return (T_8X5);
            if (lexeme("はちろく"))                 return (T_8X6);
            if (lexeme("はちひち"))                 return (T_8X7);
            if (lexeme("はちしち"))                 return (T_8X7);
            if (lexeme("はっぱ"))                   return (T_8X8);
            if (lexeme("はっく"))                   return (T_8X9);
            if (lexeme("くいちが"))                 return (T_9X1);
            //if (lexeme("くに"))                     return (T_9X2);   // T_KYUU T_NI
            if (lexeme("くさん"))                   return (T_9X3);
            if (lexeme("くし"))                     return (T_9X4);
            if (lexeme("くご"))                     return (T_9X5);
            if (lexeme("くろく"))                   return (T_9X6);
            if (lexeme("くひち"))                   return (T_9X7);
            if (lexeme("くしち"))                   return (T_9X7);
            //if (lexeme("くは"))                     return (T_9X8);   // T_KYUU T_HA
            if (lexeme("くく"))                     return (T_9X9);

            if (!lexeme("ではち", false) && lexeme("では")) return (T_HA);
            if (!lexeme("にいち", false) && lexeme("にい")) return (T_NI);

            // 最長一致法。
            // 【ここから】行を降順に並び替えておく。
            if (lexeme("｝"))                       return (T_R_PAREN);
            if (lexeme("｛"))                       return (T_L_PAREN);
            if (lexeme("］"))                       return (T_R_PAREN);
            if (lexeme("［"))                       return (T_L_PAREN);
            if (lexeme("？"))                       return (T_PERIOD);
            if (lexeme("＝"))                       return (T_HA);
            if (lexeme("．"))                       return (T_PERIOD);
            if (lexeme("－"))                       return (T_MINUS);
            if (lexeme("，"))                       return (T_COMMA);
            if (lexeme("＋"))                       return (T_TASU);
            if (lexeme("）"))                       return (T_R_PAREN);
            if (lexeme("（"))                       return (T_L_PAREN);
            if (lexeme("！"))                       return (T_PERIOD);
            if (lexeme("・"))                       return (T_DOT);
            if (lexeme("をわることの"))             return (T_WARU);
            if (lexeme("をひくことの"))             return (T_HIKU);
            if (lexeme("を"))                       return (T_WO1);
            if (lexeme("われば"))                   return (T_WARUTO);
            if (lexeme("わると"))                   return (T_WARUTO);
            if (lexeme("わることの"))               return (T_WARU);
            if (lexeme("わる"))                     return (T_WARU);
            if (lexeme("わりざん"))                 return (T_WARIZAN);
            if (lexeme("わり"))                     return (T_WATTE);
            if (lexeme("わって"))                   return (T_WATTE);
            if (lexeme("わったら"))                 return (T_WARUTO);
            if (lexeme("わった"))                   return (T_WATTA);
            if (lexeme("わかる"))                   return (T_WAKARAN);
            if (lexeme("わかりません"))             return (T_WAKARAN);
            if (lexeme("わかります"))               return (T_WAKARAN);
            if (lexeme("わからんとよ"))             return (T_WAKARAN);
            if (lexeme("わからん"))                 return (T_WAKARAN);
            if (lexeme("わからないよー"))           return (T_WAKARAN);
            if (lexeme("わからないよお"))           return (T_WAKARAN);
            if (lexeme("わからないよ"))             return (T_WAKARAN);
            if (lexeme("わからない"))               return (T_WAKARAN);
            if (lexeme("わ"))                       return (T_WA);
            if (lexeme("ろっ"))                     return (T_ROKU);
            if (lexeme("ろく"))                     return (T_ROKU);
            if (lexeme("れい"))                     return (T_ZERO);
            if (lexeme("よー"))                     return (T_KA);
            if (lexeme("よん"))                     return (T_YON);
            if (lexeme("よお"))                     return (T_KA);
            if (lexeme("よぉ"))                     return (T_KA);
            if (lexeme("よう"))                     return (T_KA);
            if (lexeme("よぅ"))                     return (T_KA);
            if (lexeme("よ"))                       return (T_KA);
            if (lexeme("やれば"))                   return (T_SURUTO);
            if (lexeme("やると"))                   return (T_SURUTO);
            if (lexeme("やねん"))                   return (T_KA);
            if (lexeme("やね"))                     return (T_KA);
            if (lexeme("やってみれば"))             return (T_SURUTO);
            if (lexeme("やってみると"))             return (T_SURUTO);
            if (lexeme("やって"))                   return (T_SHITE);
            if (lexeme("やったら"))                 return (T_SURUTO);
            if (lexeme("やった"))                   return (T_SHITA);
            if (lexeme("や"))                       return (T_KA);
            if (lexeme("もの"))                     return (T_MONO);
            if (lexeme("もとめると"))               return (T_SURUTO);
            if (lexeme("もとめる"))                 return (T_SURU);
            if (lexeme("もとめて"))                 return (T_SHITE);
            if (lexeme("もとめたら"))               return (T_SURUTO);
            if (lexeme("もとめた"))                 return (T_SHITA);
            if (lexeme("もとめ"))                   return (T_SHITE);
            if (lexeme("も"))                       return (T_WO1);
            if (lexeme("みんな"))                   return (T_ALL);
            if (lexeme("みれば"))                   return (T_MIRUTO);
            if (lexeme("みると"))                   return (T_MIRUTO);
            if (lexeme("まん"))                     return (T_MAN);
            if (lexeme("まいなすして"))             return (T_HIITE);
            if (lexeme("まいなすしたら"))           return (T_HIKUTO);
            if (lexeme("まいなすした"))             return (T_HIITA);
            if (lexeme("まいなすし"))               return (T_HIITE);
            if (lexeme("まいなす"))                 return (T_MINUS);
            if (lexeme("へ"))                       return (T_NI);
            if (lexeme("ぷらすして"))               return (T_TASHITE);
            if (lexeme("ぷらすしたら"))             return (T_TASUTO);
            if (lexeme("ぷらすした"))               return (T_TASHITA);
            if (lexeme("ぷらすし"))                 return (T_TASHITE);
            if (lexeme("ぷらす"))                   return (T_PLUS);
            if (lexeme("ぶんの"))                   return (T_BUNNO);
            if (lexeme("ぴゃく"))                   return (T_HYAKU);
            if (lexeme("びゃく"))                   return (T_HYAKU);
            if (lexeme("ひゃく"))                   return (T_HYAKU);
            if (lexeme("ひち"))                     return (T_NANA);
            if (lexeme("ひけば"))                   return (T_HIKUTO);
            if (lexeme("ひくと"))                   return (T_HIKUTO);
            if (lexeme("ひくことの"))               return (T_HIKU);
            if (lexeme("ひく"))                     return (T_HIKU);
            if (lexeme("ひきざん"))                 return (T_HIKIZAN);
            if (lexeme("ひき"))                     return (T_HIITE);
            if (lexeme("ひいて"))                   return (T_HIITE);
            if (lexeme("ひいたら"))                 return (T_HIKUTO);
            if (lexeme("ひいた"))                   return (T_HIITA);
            if (lexeme("ばい"))                     return (T_BAI);
            if (lexeme("はっ"))                     return (T_HACHI);
            if (lexeme("はち"))                     return (T_HACHI);
            if (lexeme("は"))                       return (T_HA);
            if (lexeme("の"))                       return (T_NO1);
            if (lexeme("ねん"))                     return (T_KA);
            if (lexeme("ね"))                       return (T_KA);
            if (lexeme("にー"))                     return (T_NI);
            if (lexeme("にたすことの"))             return (T_TASU);
            if (lexeme("にくわえることの"))         return (T_TASU);
            if (lexeme("にかけることの"))           return (T_KAKERU);
            if (lexeme("にぃ"))                     return (T_NI);
            if (lexeme("に"))                       return (T_NI);
            if (lexeme("なーんだ"))                 return (T_IKURA);
            if (lexeme("なーに"))                   return (T_IKURA);
            if (lexeme("なんぼなん"))               return (T_IKURA);
            if (lexeme("なんぼなの"))               return (T_IKURA);
            if (lexeme("なんぼ"))                   return (T_IKURA);
            if (lexeme("なんになるでしょう"))       return (T_IKURA);
            if (lexeme("なんになる"))               return (T_IKURA);
            if (lexeme("なんになります"))           return (T_IKURA);
            if (lexeme("なんなんでっ"))             return (T_IKURA);
            if (lexeme("なんなんです"))             return (T_IKURA);
            if (lexeme("なんなんでしょう"))         return (T_IKURA);
            if (lexeme("なんなん"))                 return (T_IKURA);
            if (lexeme("なんなのでっ"))             return (T_IKURA);
            if (lexeme("なんなのです"))             return (T_IKURA);
            if (lexeme("なんなのでしょう"))         return (T_IKURA);
            if (lexeme("なんなの"))                 return (T_IKURA);
            if (lexeme("なんでっ"))                 return (T_IKURA);
            if (lexeme("なんです"))                 return (T_IKURA);
            if (lexeme("なんでしょう"))             return (T_IKURA);
            if (lexeme("なんだ"))                   return (T_IKURA);
            if (lexeme("なん"))                     return (T_IKURA);
            if (lexeme("なにになる"))               return (T_IKURA);
            if (lexeme("なにでっ"))                 return (T_IKURA);
            if (lexeme("なにです"))                 return (T_IKURA);
            if (lexeme("なに"))                     return (T_IKURA);
            if (lexeme("なな"))                     return (T_NANA);
            if (lexeme("なあに"))                   return (T_IKURA);
            if (lexeme("どんなかずになるでしょう")) return (T_IKURA);
            if (lexeme("どんなかずになる"))         return (T_IKURA);
            if (lexeme("どんなかずになります"))     return (T_IKURA);
            if (lexeme("どないなるんでしょう"))     return (T_IKURA);
            if (lexeme("どないなるのでしょう"))     return (T_IKURA);
            if (lexeme("どないなる"))               return (T_IKURA);
            if (lexeme("どないでっ"))               return (T_IKURA);
            if (lexeme("どないです"))               return (T_IKURA);
            if (lexeme("どないでしょう"))           return (T_IKURA);
            if (lexeme("どない"))                   return (T_IKURA);
            if (lexeme("どうなんでっ"))             return (T_IKURA);
            if (lexeme("どうなんです"))             return (T_IKURA);
            if (lexeme("どうなんでしょう"))         return (T_IKURA);
            if (lexeme("どうなん"))                 return (T_IKURA);
            if (lexeme("どうなる"))                 return (T_IKURA);
            if (lexeme("どうなります"))             return (T_IKURA);
            if (lexeme("どうなのでっ"))             return (T_IKURA);
            if (lexeme("どうなのです"))             return (T_IKURA);
            if (lexeme("どうなのでしょう"))         return (T_IKURA);
            if (lexeme("どうなの"))                 return (T_IKURA);
            if (lexeme("どう"))                     return (T_IKURA);
            if (lexeme("とね、"))                   return (T_TO1);
            if (lexeme("とな、"))                   return (T_TO1);
            if (lexeme("とじかっこ"))               return (T_R_PAREN);
            if (lexeme("と"))                       return (T_TO1);
            if (lexeme("でんねん"))                 return (T_KA);
            if (lexeme("で"))                       return (T_DE);
            if (lexeme("てん"))                     return (T_DOT);
            if (lexeme("ちょう"))                   return (T_CHOU);
            if (lexeme("たせば"))                   return (T_TASUTO);
            if (lexeme("たすと"))                   return (T_TASUTO);
            if (lexeme("たすことの"))               return (T_TASU);
            if (lexeme("たす"))                     return (T_TASU);
            if (lexeme("たして"))                   return (T_TASHITE);
            if (lexeme("たしたら"))                 return (T_TASUTO);
            if (lexeme("たした"))                   return (T_TASHITA);
            if (lexeme("たしざん"))                 return (T_TASHIZAN);
            if (lexeme("たしあわせれば"))           return (T_TASUTO);
            if (lexeme("たしあわせると"))           return (T_TASUTO);
            if (lexeme("たしあわせる"))             return (T_TASU);
            if (lexeme("たしあわせて"))             return (T_TASHITE);
            if (lexeme("たしあわせた"))             return (T_TASHITA);
            if (lexeme("たしあわす"))               return (T_TASU);
            if (lexeme("たし"))                     return (T_TASHITE);
            if (lexeme("ぜんぶ"))                   return (T_ALL);
            if (lexeme("ぜん"))                     return (T_SEN);
            if (lexeme("ぜろ"))                     return (T_ZERO);
            if (lexeme("せん"))                     return (T_SEN);
            if (lexeme("せき"))                     return (T_SEKI);
            if (lexeme("すれば"))                   return (T_SURUTO);
            if (lexeme("すると"))                   return (T_SURUTO);
            if (lexeme("する"))                     return (T_SURU);
            if (lexeme("すべて"))                   return (T_ALL);
            if (lexeme("じょー"))                   return (T_JOU);
            if (lexeme("じょざん"))                 return (T_WARIZAN);
            if (lexeme("じょうざん"))               return (T_KAKEZAN);
            if (lexeme("じょう"))                   return (T_JOU);
            if (lexeme("じゅー"))                   return (T_JUU);
            if (lexeme("じゅう"))                   return (T_JUU);
            if (lexeme("じじょう"))                 return (T_JIJOU);
            if (lexeme("しー"))                     return (T_YON);
            if (lexeme("しらん"))                   return (T_WAKARAN);
            if (lexeme("しらない"))                 return (T_WAKARAN);
            if (lexeme("しょう"))                   return (T_SHOU);
            if (lexeme("してみれば"))               return (T_SURUTO);
            if (lexeme("してみると"))               return (T_SURUTO);
            if (lexeme("して"))                     return (T_SHITE);
            if (lexeme("しち"))                     return (T_NANA);
            if (lexeme("したら"))                   return (T_SURUTO);
            if (lexeme("した"))                     return (T_SHITA);
            if (lexeme("しい"))                     return (T_YON);
            if (lexeme("しぃ"))                     return (T_YON);
            if (lexeme("し"))                       return (T_YON);
            if (lexeme("さん"))                     return (T_SAN);
            if (lexeme("さしひき"))                 return (T_HIITE);
            if (lexeme("さしひいて"))               return (T_HIITE);
            if (lexeme("さしひいたら"))             return (T_HIKUTO);
            if (lexeme("さしひいた"))               return (T_HIITA);
            if (lexeme("さ"))                       return (T_SA);
            if (lexeme("ご"))                       return (T_GO);
            if (lexeme("こたえろよ"))               return (T_OSHIETE);
            if (lexeme("こたえろ"))                 return (T_OSHIETE);
            if (lexeme("こたえてくれるかい"))       return (T_OSHIETE);
            if (lexeme("こたえてくれるか"))         return (T_OSHIETE);
            if (lexeme("こたえてくれる"))           return (T_OSHIETE);
            if (lexeme("こたえてくれよ"))           return (T_OSHIETE);
            if (lexeme("こたえてくれ"))             return (T_OSHIETE);
            if (lexeme("こたえてくださいよ"))       return (T_OSHIETE);
            if (lexeme("こたえてください"))         return (T_OSHIETE);
            if (lexeme("げんざん"))                 return (T_HIKIZAN);
            if (lexeme("けいさん"))                 return (T_KEISAN);
            if (lexeme("けい"))                     return (T_KEI);
            if (lexeme("くわえると"))               return (T_TASUTO);
            if (lexeme("くわえることの"))           return (T_TASU);
            if (lexeme("くわえる"))                 return (T_TASU);
            if (lexeme("くわえて"))                 return (T_TASHITE);
            if (lexeme("くわえたら"))               return (T_TASUTO);
            if (lexeme("くわえた"))                 return (T_TASHITA);
            if (lexeme("くれよー"))                 return (T_KA);
            if (lexeme("くれよぅ"))                 return (T_KA);
            if (lexeme("くれよ"))                   return (T_KA);
            if (lexeme("くれ"))                     return (T_KA);
            if (lexeme("くださいよー"))             return (T_KA);
            if (lexeme("くださいよぅ"))             return (T_KA);
            if (lexeme("くださいよ"))               return (T_KA);
            if (lexeme("ください"))                 return (T_KA);
            if (lexeme("く"))                       return (T_KYUU);
            if (lexeme("きゅー"))                   return (T_KYUU);
            if (lexeme("きゅう"))                   return (T_KYUU);
            if (lexeme("が"))                       return (T_HA);
            if (lexeme("かー"))                     return (T_KA);
            if (lexeme("から"))                     return (T_KARA);
            if (lexeme("かねー"))                   return (T_KA);
            if (lexeme("かねえ"))                   return (T_KA);
            if (lexeme("かねぇ"))                   return (T_KA);
            if (lexeme("かね"))                     return (T_KA);
            if (lexeme("かなー"))                   return (T_KA);
            if (lexeme("かなあ"))                   return (T_KA);
            if (lexeme("かなぁ"))                   return (T_KA);
            if (lexeme("かな"))                     return (T_KA);
            if (lexeme("かっこをとじる"))           return (T_R_PAREN);
            if (lexeme("かっこをとじて"))           return (T_R_PAREN);
            if (lexeme("かっことじる"))             return (T_R_PAREN);
            if (lexeme("かっことじて"))             return (T_R_PAREN);
            if (lexeme("かっことじ"))               return (T_R_PAREN);
            if (lexeme("かっこ"))                   return (T_L_PAREN);
            if (lexeme("かさん"))                   return (T_TASHIZAN);
            if (lexeme("かければ"))                 return (T_KAKERUTO);
            if (lexeme("かけると"))                 return (T_KAKERUTO);
            if (lexeme("かけることの"))             return (T_KAKERU);
            if (lexeme("かける"))                   return (T_KAKERU);
            if (lexeme("かけて"))                   return (T_KAKETE);
            if (lexeme("かけたら"))                 return (T_KAKERUTO);
            if (lexeme("かけた"))                   return (T_KAKETA);
            if (lexeme("かけざん"))                 return (T_KAKEZAN);
            if (lexeme("かけあわせれば"))           return (T_KAKERUTO);
            if (lexeme("かけあわせると"))           return (T_KAKERUTO);
            if (lexeme("かけあわせる"))             return (T_KAKERU);
            if (lexeme("かけあわせて"))             return (T_KAKERUTO);
            if (lexeme("かけあわせた"))             return (T_KAKETA);
            if (lexeme("かけあわせ"))               return (T_KAKEZAN);
            if (lexeme("かけあわす"))               return (T_KAKERU);
            if (lexeme("かけ"))                     return (T_KAKERU);
            if (lexeme("かあ"))                     return (T_KA);
            if (lexeme("かぁ"))                     return (T_KA);
            if (lexeme("か"))                       return (T_KA);
            if (lexeme("おしえろよ"))               return (T_OSHIETE);
            if (lexeme("おしえろ"))                 return (T_OSHIETE);
            if (lexeme("おしえてん"))               return (T_OSHIETE);
            if (lexeme("おしえてよ"))               return (T_OSHIETE);
            if (lexeme("おしえてほしいねん"))       return (T_OSHIETE);
            if (lexeme("おしえてちょうだいよ"))     return (T_OSHIETE);
            if (lexeme("おしえてちょうだい"))       return (T_OSHIETE);
            if (lexeme("おしえてくれるかい"))       return (T_OSHIETE);
            if (lexeme("おしえてくれるか"))         return (T_OSHIETE);
            if (lexeme("おしえてくれる"))           return (T_OSHIETE);
            if (lexeme("おしえてくれ"))             return (T_OSHIETE);
            if (lexeme("おしえてくださいよ"))       return (T_OSHIETE);
            if (lexeme("おしえてください"))         return (T_OSHIETE);
            if (lexeme("おしえて"))                 return (T_OSHIETE);
            if (lexeme("おく"))                     return (T_OKU);
            if (lexeme("いっ"))                     return (T_ICHI);
            if (lexeme("いち"))                     return (T_ICHI);
            if (lexeme("いくらになるでしょう"))     return (T_IKURA);
            if (lexeme("いくらになる"))             return (T_IKURA);
            if (lexeme("いくらになります"))         return (T_IKURA);
            if (lexeme("いくらでっ"))               return (T_IKURA);
            if (lexeme("いくらです"))               return (T_IKURA);
            if (lexeme("いくら"))                   return (T_IKURA);
            if (lexeme("いくつや"))                 return (T_IKURA);
            if (lexeme("いくつになるでしょう"))     return (T_IKURA);
            if (lexeme("いくつになる"))             return (T_IKURA);
            if (lexeme("いくつになります"))         return (T_IKURA);
            if (lexeme("いくつなん"))               return (T_IKURA);
            if (lexeme("いくつなのでっ"))           return (T_IKURA);
            if (lexeme("いくつなのです"))           return (T_IKURA);
            if (lexeme("いくつなのでしょう"))       return (T_IKURA);
            if (lexeme("いくつなの"))               return (T_IKURA);
            if (lexeme("いくつ"))                   return (T_IKURA);
            if (lexeme("〕"))                       return (T_R_PAREN);
            if (lexeme("〔"))                       return (T_L_PAREN);
            if (lexeme("】"))                       return (T_R_PAREN);
            if (lexeme("【"))                       return (T_L_PAREN);
            if (lexeme("』"))                       return (T_R_PAREN);
            if (lexeme("『"))                       return (T_L_PAREN);
            if (lexeme("」"))                       return (T_R_PAREN);
            if (lexeme("「"))                       return (T_L_PAREN);
            if (lexeme("》"))                       return (T_R_PAREN);
            if (lexeme("《"))                       return (T_L_PAREN);
            if (lexeme("〉"))                       return (T_R_PAREN);
            if (lexeme("〈"))                       return (T_L_PAREN);
            if (lexeme("。"))                       return (T_PERIOD);
            if (lexeme("、"))                       return (T_COMMA);
            if (lexeme("☆"))                       return (T_PERIOD);
            if (lexeme("★"))                       return (T_PERIOD);
            if (lexeme("●"))                       return (T_PERIOD);
            if (lexeme("◎"))                       return (T_PERIOD);
            if (lexeme("÷"))                       return (T_WARU);
            if (lexeme("×"))                       return (T_KAKERU);
            if (lexeme("}"))                        return (T_R_PAREN);
            if (lexeme("{"))                        return (T_L_PAREN);
            if (lexeme("]"))                        return (T_R_PAREN);
            if (lexeme("["))                        return (T_L_PAREN);
            if (lexeme("?"))                        return (T_PERIOD);
            if (lexeme("=="))                       return (T_HA);
            if (lexeme("="))                        return (T_HA);
            if (lexeme("/"))                        return (T_WARU);
            if (lexeme("."))                        return (T_PERIOD);
            if (lexeme("-"))                        return (T_MINUS);
            if (lexeme(","))                        return (T_COMMA);
            if (lexeme("+"))                        return (T_TASU);
            if (lexeme("*"))                        return (T_KAKERU);
            if (lexeme(")"))                        return (T_R_PAREN);
            if (lexeme("("))                        return (T_L_PAREN);
            if (lexeme("!"))                        return (T_PERIOD);
            // 【ここまで】行を降順に並び替えておく。

            char ch = getch();
            if (ch == EOF)
                return commit_token(eof);

            std::string str;
            do
            {
                str += ch;
                ch = getch();
            } while (ch != EOF);

            message(std::string("エラー：文字列「") +
                    str + "」がわかりません。\n");

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

        // T_PERIOD, T_COMMAを整理して、T_MIRUTO, T_ALLを削除する。
        void resynth1(std::vector<info_type>& infos)
        {
            std::vector<info_type> newinfos;
            info_type info;

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

        // 「の」をT_NO1, T_NO2, T_NO3に分類する。
        // T_NO2: 「わ」「さ」「せき」「しょう」の直前の「の」。
        // T_NO3: 「何の何倍」の「の」。
        // T_NO4: 「かけざん」「けいさん」などの直前の「の」。
        // T_NO1: それ以外。
        void resynth2(std::vector<info_type>& infos)
        {
            std::vector<info_type>::iterator it = infos.begin();
            std::vector<info_type>::iterator end = infos.end();
            std::vector<info_type>::iterator it_save = end;
            for (; it != end; ++it)
            {
                switch (it->get_token())
                {
                case T_NO1:
                    it_save = it;
                    break;

                case T_WA:
                case T_SEKI:
                case T_SA:
                case T_SHOU:
                    if (it_save != end)
                    {
                        it_save->set_token(T_NO2);
                        it_save = end;
                    }
                    break;

                case T_BAI:
                    if (it_save != end)
                    {
                        it_save->set_token(T_NO3);
                        it_save = end;
                    }
                    break;

                case T_TASHIZAN:
                case T_HIKIZAN:
                case T_KAKEZAN:
                case T_WARIZAN:
                case T_KEISAN:
                    if (it_save != end)
                    {
                        it_save->set_token(T_NO4);
                        it_save = end;
                    }
                    break;

                case T_JOU:
                case T_JIJOU:
                    it_save = end;

                default:
                    break;
                }
            }
        }

        // 特定の語句の後のT_COMMAを削除する。
        void resynth3(std::vector<info_type>& infos)
        {
            std::vector<info_type> newinfos;
            info_type info;

            std::vector<info_type>::iterator it = infos.begin();
            std::vector<info_type>::iterator end = infos.end();
            std::vector<info_type>::iterator it2;
            for (; it != end; ++it)
            {
                switch (it->get_token())
                {
                case T_DE:
                case T_HA:
                case T_HIITE:
                case T_HIKU:
                case T_IKURA:
                case T_KA:
                case T_KAKERU:
                case T_KAKETE:
                case T_KARA:
                case T_MINUS:
                case T_NI:
                case T_NO1:
                case T_NO2:
                case T_NO3:
                case T_PLUS:
                case T_SHITA:
                case T_SHITE:
                case T_SURUTO:
                case T_TASHITE:
                case T_TASU:
                case T_TO1:
                case T_WARU:
                case T_WATTE:
                case T_WO1:
                case T_WO2:
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
            for (; it != end; ++it)
            {
                switch (it->get_token())
                {
                case T_TO1:
                    it_save = it;
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
                    break;

                case T_BUNNO:
                    if (it_save != end)
                    {
                        it_save->set_token(T_TO2);
                        it_save = end;
                    }
                    break;

                default:
                    it_save = end;
                    break;
                }
            }
        }

        // 「計算し」「をし」などの「し」を修正する。
        void resynth6(std::vector<info_type>& infos)
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

        // 「を」をT_WO1とT_WO2に分類する。
        // T_WO2: 「たしざん」「けいさん」などの直後の「を」。
        // T_WO1: そのほかの「を」。
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

        // 「を」の直後に「けいさん」「たしざん」などがあれば、
        // 「を」を「の」に変える。
        void resynth8(std::vector<info_type>& infos)
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
                    it_save = it;
                    break;

                default:
                    it_save = end;
                    break;
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
