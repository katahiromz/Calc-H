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
            if (lexeme("｝"))                           return (T_R_PAREN);
            if (lexeme("｛"))                           return (T_L_PAREN);
            if (lexeme("ｔａｎ"))                       return (T_TAN);
            if (lexeme("ｓｉｎ"))                       return (T_SIN);
            if (lexeme("ｃｏｓ"))                       return (T_COS);
            if (lexeme("ａｔａｎ"))                     return (T_ATAN);
            if (lexeme("ａｓｉｎ"))                     return (T_ASIN);
            if (lexeme("ａｒｃｔａｎ"))                 return (T_ATAN);
            if (lexeme("ａｒｃｓｉｎ"))                 return (T_ASIN);
            if (lexeme("ａｒｃｃｏｓ"))                 return (T_ACOS);
            if (lexeme("ａｃｏｓ"))                     return (T_ACOS);
            if (lexeme("］"))                           return (T_R_PAREN);
            if (lexeme("［"))                           return (T_L_PAREN);
            if (lexeme("？"))                           return (T_PERIOD);
            if (lexeme("＝"))                           return (T_HA);
            if (lexeme("．"))                           return (T_PERIOD);
            if (lexeme("－"))                           return (T_MINUS);
            if (lexeme("，"))                           return (T_COMMA);
            if (lexeme("＋"))                           return (T_TASU);
            if (lexeme("）"))                           return (T_R_PAREN);
            if (lexeme("（"))                           return (T_L_PAREN);
            if (lexeme("％"))                           return (T_PERCENT);
            if (lexeme("！"))                           return (T_PERIOD);
            if (lexeme("零"))                           return (T_ZERO);
            if (lexeme("陸"))                           return (T_ROKU);
            if (lexeme("阿僧祗"))                       return (T_ASOUGI);
            if (lexeme("阡"))                           return (T_SEN);
            if (lexeme("那由多"))                       return (T_NAYUTA);
            if (lexeme("那由他"))                       return (T_NAYUTA);
            if (lexeme("載"))                           return (T_SAI);
            if (lexeme("肆"))                           return (T_YON);
            if (lexeme("穣"))                           return (T_JOU2);
            if (lexeme("百"))                           return (T_HYAKU);
            if (lexeme("玖"))                           return (T_KYUU);
            if (lexeme("無量大数"))                     return (T_MURYOUTAISUU);
            if (lexeme("澗"))                           return (T_KAN);
            if (lexeme("漆"))                           return (T_NANA);
            if (lexeme("溝"))                           return (T_KOU);
            if (lexeme("正"))                           return (T_SEI);
            if (lexeme("極"))                           return (T_GOKU);
            if (lexeme("拾"))                           return (T_JUU);
            if (lexeme("恒河沙"))                       return (T_GOUGASHA);
            if (lexeme("弐"))                           return (T_NI);
            if (lexeme("壱"))                           return (T_ICHI);
            if (lexeme("垓"))                           return (T_GAI);
            if (lexeme("四"))                           return (T_YON);
            if (lexeme("参"))                           return (T_SAN);
            if (lexeme("千"))                           return (T_SEN);
            if (lexeme("十"))                           return (T_JUU);
            if (lexeme("六"))                           return (T_ROKU);
            if (lexeme("八"))                           return (T_HACHI);
            if (lexeme("兆"))                           return (T_CHOU);
            if (lexeme("億"))                           return (T_OKU);
            if (lexeme("伍"))                           return (T_GO);
            if (lexeme("仟"))                           return (T_SEN);
            if (lexeme("京"))                           return (T_KEI);
            if (lexeme("五"))                           return (T_GO);
            if (lexeme("二"))                           return (T_NI);
            if (lexeme("九"))                           return (T_KYUU);
            if (lexeme("不可思議"))                     return (T_FUKASHIGI);
            if (lexeme("三"))                           return (T_SAN);
            if (lexeme("万"))                           return (T_MAN);
            if (lexeme("七"))                           return (T_NANA);
            if (lexeme("一"))                           return (T_ICHI);
            if (lexeme("・"))                           return (T_DOT);
            if (lexeme("をわることの"))                 return (T_WARU);
            if (lexeme("をひくことの"))                 return (T_HIKU);
            if (lexeme("を"))                           return (T_WO1);
            if (lexeme("われよ"))                       return (T_WARUTO);
            if (lexeme("われや"))                       return (T_WARUTO);
            if (lexeme("われば"))                       return (T_WARUTO);
            if (lexeme("われ"))                         return (T_WARUTO);
            if (lexeme("わるときの"))                   return (T_WARUTO);
            if (lexeme("わるとき"))                     return (T_WARUTO);
            if (lexeme("わると"))                       return (T_WARUTO);
            if (lexeme("わることの"))                   return (T_WARU);
            if (lexeme("わる"))                         return (T_WARU);
            if (lexeme("わりなさい"))                   return (T_WARUTO);
            if (lexeme("わりざん"))                     return (T_WARIZAN);
            if (lexeme("わり"))                         return (T_WATTE);
            if (lexeme("わられる"))                     return (T_WARARERU);
            if (lexeme("わって"))                       return (T_WATTE);
            if (lexeme("わったら"))                     return (T_WARUTO);
            if (lexeme("わったときの"))                 return (T_WARUTO);
            if (lexeme("わったとき"))                   return (T_WARUTO);
            if (lexeme("わった"))                       return (T_WATTA);
            if (lexeme("わかる"))                       return (T_WAKARAN);
            if (lexeme("わかりません"))                 return (T_WAKARAN);
            if (lexeme("わかります"))                   return (T_WAKARAN);
            if (lexeme("わからんとよ"))                 return (T_WAKARAN);
            if (lexeme("わからん"))                     return (T_WAKARAN);
            if (lexeme("わからないよー"))               return (T_WAKARAN);
            if (lexeme("わからないよお"))               return (T_WAKARAN);
            if (lexeme("わからないよ"))                 return (T_WAKARAN);
            if (lexeme("わからない"))                   return (T_WAKARAN);
            if (lexeme("わ"))                           return (T_WA);
            if (lexeme("ろっ"))                         return (T_ROKU);
            if (lexeme("ろぐ"))                         return (T_LOG);
            if (lexeme("ろく"))                         return (T_ROKU);
            if (lexeme("れい"))                         return (T_ZERO);
            if (lexeme("るーと"))                       return (T_ROOT);
            if (lexeme("るうと"))                       return (T_ROOT);
            if (lexeme("りっぽう"))                     return (T_RIPPOU);
            if (lexeme("よー"))                         return (T_KA);
            if (lexeme("よん"))                         return (T_YON);
            if (lexeme("よげん"))                       return (T_COS);
            if (lexeme("よお"))                         return (T_KA);
            if (lexeme("よぉ"))                         return (T_KA);
            if (lexeme("よう"))                         return (T_KA);
            if (lexeme("よぅ"))                         return (T_KA);
            if (lexeme("よ"))                           return (T_KA);
            if (lexeme("やれよ"))                       return (T_SURUTO);
            if (lexeme("やれや"))                       return (T_SURUTO);
            if (lexeme("やれば"))                       return (T_SURUTO);
            if (lexeme("やれ"))                         return (T_SURUTO);
            if (lexeme("やると"))                       return (T_SURUTO);
            if (lexeme("やりなさい"))                   return (T_SURUTO);
            if (lexeme("やねん"))                       return (T_KA);
            if (lexeme("やね"))                         return (T_KA);
            if (lexeme("やってみろよ"))                 return (T_SURUTO);
            if (lexeme("やってみろや"))                 return (T_SURUTO);
            if (lexeme("やってみろ"))                   return (T_SURUTO);
            if (lexeme("やってみれば"))                 return (T_SURUTO);
            if (lexeme("やってみるとき"))               return (T_SURUTO);
            if (lexeme("やってみると"))                 return (T_SURUTO);
            if (lexeme("やってみよ"))                   return (T_SURUTO);
            if (lexeme("やってみたら"))                 return (T_SURUTO);
            if (lexeme("やってみたとき"))               return (T_SURUTO);
            if (lexeme("やって"))                       return (T_SHITE);
            if (lexeme("やったら"))                     return (T_SURUTO);
            if (lexeme("やったとき"))                   return (T_SURUTO);
            if (lexeme("やった"))                       return (T_SHITA);
            if (lexeme("や"))                           return (T_KA);
            if (lexeme("もの"))                         return (T_MONO);
            if (lexeme("もとめると"))                   return (T_SURUTO);
            if (lexeme("もとめよ"))                     return (T_SURUTO);
            if (lexeme("もとめなさい"))                 return (T_SURUTO);
            if (lexeme("もとめて"))                     return (T_SHITE);
            if (lexeme("もとめたら"))                   return (T_SURUTO);
            if (lexeme("もとめた"))                     return (T_SHITA);
            if (lexeme("もとめ"))                       return (T_SHITE);
            if (lexeme("も"))                           return (T_WO1);
            if (lexeme("むりょうたいすう"))             return (T_MURYOUTAISUU);
            if (lexeme("みんな"))                       return (T_ALL);
            if (lexeme("みれば"))                       return (T_MIRUTO);
            if (lexeme("みると"))                       return (T_MIRUTO);
            if (lexeme("まん"))                         return (T_MAN);
            if (lexeme("まいなすするときの"))           return (T_HIKUTO);
            if (lexeme("まいなすするとき"))             return (T_HIKUTO);
            if (lexeme("まいなすしろよ"))               return (T_HIKUTO);
            if (lexeme("まいなすしろや"))               return (T_HIKUTO);
            if (lexeme("まいなすしろ"))                 return (T_HIKUTO);
            if (lexeme("まいなすしなさい"))             return (T_HIKUTO);
            if (lexeme("まいなすして"))                 return (T_HIITE);
            if (lexeme("まいなすしたら"))               return (T_HIKUTO);
            if (lexeme("まいなすしたとき"))             return (T_HIKUTO);
            if (lexeme("まいなすした"))                 return (T_HIITA);
            if (lexeme("まいなすし"))                   return (T_HIITE);
            if (lexeme("まいなすされる"))               return (T_HIKARERU);
            if (lexeme("まいなす"))                     return (T_MINUS);
            if (lexeme("へいほう"))                     return (T_HEIHOU);
            if (lexeme("へ"))                           return (T_NI);
            if (lexeme("ぷらすするときの"))             return (T_TASUTO);
            if (lexeme("ぷらすするとき"))               return (T_TASUTO);
            if (lexeme("ぷらすしろよ"))                 return (T_TASUTO);
            if (lexeme("ぷらすしろや"))                 return (T_TASUTO);
            if (lexeme("ぷらすしろ"))                   return (T_TASUTO);
            if (lexeme("ぷらすしなさい"))               return (T_TASUTO);
            if (lexeme("ぷらすして"))                   return (T_TASHITE);
            if (lexeme("ぷらすしたら"))                 return (T_TASUTO);
            if (lexeme("ぷらすしたとき"))               return (T_TASUTO);
            if (lexeme("ぷらすした"))                   return (T_TASHITA);
            if (lexeme("ぷらすし"))                     return (T_TASHITE);
            if (lexeme("ぷらすされる"))                 return (T_TASARERU);
            if (lexeme("ぷらす"))                       return (T_PLUS);
            if (lexeme("ぶんの"))                       return (T_BUNNO);
            if (lexeme("ふかしぎ"))                     return (T_FUKASHIGI);
            if (lexeme("ぴゃく"))                       return (T_HYAKU);
            if (lexeme("びゃく"))                       return (T_HYAKU);
            if (lexeme("ひゃく"))                       return (T_HYAKU);
            if (lexeme("ひち"))                         return (T_NANA);
            if (lexeme("ひけよ"))                       return (T_HIKUTO);
            if (lexeme("ひけや"))                       return (T_HIKUTO);
            if (lexeme("ひけば"))                       return (T_HIKUTO);
            if (lexeme("ひけ"))                         return (T_HIKUTO);
            if (lexeme("ひくときの"))                   return (T_HIKUTO);
            if (lexeme("ひくとき"))                     return (T_HIKUTO);
            if (lexeme("ひくと"))                       return (T_HIKUTO);
            if (lexeme("ひくことの"))                   return (T_HIKU);
            if (lexeme("ひく"))                         return (T_HIKU);
            if (lexeme("ひきなさい"))                   return (T_HIKUTO);
            if (lexeme("ひきざん"))                     return (T_HIKIZAN);
            if (lexeme("ひき"))                         return (T_HIITE);
            if (lexeme("ひかれる"))                     return (T_HIKARERU);
            if (lexeme("ひいて"))                       return (T_HIITE);
            if (lexeme("ひいたら"))                     return (T_HIKUTO);
            if (lexeme("ひいたときの"))                 return (T_HIKUTO);
            if (lexeme("ひいたとき"))                   return (T_HIKUTO);
            if (lexeme("ひいた"))                       return (T_HIITA);
            if (lexeme("ぱーせんと"))                   return (T_PERCENT);
            if (lexeme("ぱー"))                         return (T_PERCENT);
            if (lexeme("ぱい"))                         return (T_PI);
            if (lexeme("ばい"))                         return (T_BAI);
            if (lexeme("はっ"))                         return (T_HACHI);
            if (lexeme("はち"))                         return (T_HACHI);
            if (lexeme("はいぱぼりっく・たんぜんと"))   return (T_TANH);
            if (lexeme("はいぱぼりっく・たんじぇんと")) return (T_TANH);
            if (lexeme("はいぱぼりっく・さいん"))       return (T_SINH);
            if (lexeme("はいぱぼりっく・こさいん"))     return (T_COSH);
            if (lexeme("はいぱぼりっくたんぜんと"))     return (T_TANH);
            if (lexeme("はいぱぼりっくたんじぇんと"))   return (T_TANH);
            if (lexeme("はいぱぼりっくさいん"))         return (T_SINH);
            if (lexeme("はいぱぼりっくこさいん"))       return (T_COSH);
            if (lexeme("は"))                           return (T_HA);
            if (lexeme("の"))                           return (T_NO1);
            if (lexeme("ねん"))                         return (T_KA);
            if (lexeme("ねいぴあすう"))                 return (T_E);
            if (lexeme("ね"))                           return (T_KA);
            if (lexeme("にー"))                         return (T_NI);
            if (lexeme("にたすことの"))                 return (T_TASU);
            if (lexeme("にくわえることの"))             return (T_TASU);
            if (lexeme("にかけることの"))               return (T_KAKERU);
            if (lexeme("にぃ"))                         return (T_NI);
            if (lexeme("に"))                           return (T_NI);
            if (lexeme("なーんだ"))                     return (T_IKURA);
            if (lexeme("なーに"))                       return (T_IKURA);
            if (lexeme("なんぼになるんでしょう"))       return (T_IKURA);
            if (lexeme("なんぼになるんだ"))             return (T_IKURA);
            if (lexeme("なんぼになるのでしょう"))       return (T_IKURA);
            if (lexeme("なんぼになるでしょう"))         return (T_IKURA);
            if (lexeme("なんぼになる"))                 return (T_IKURA);
            if (lexeme("なんぼになります"))             return (T_IKURA);
            if (lexeme("なんぼなん"))                   return (T_IKURA);
            if (lexeme("なんぼなの"))                   return (T_IKURA);
            if (lexeme("なんぼでしょう"))               return (T_IKURA);
            if (lexeme("なんぼだろう"))                 return (T_IKURA);
            if (lexeme("なんぼ"))                       return (T_IKURA);
            if (lexeme("なんになるんでしょう"))         return (T_IKURA);
            if (lexeme("なんになるんだ"))               return (T_IKURA);
            if (lexeme("なんになるのでしょう"))         return (T_IKURA);
            if (lexeme("なんになるでしょう"))           return (T_IKURA);
            if (lexeme("なんになる"))                   return (T_IKURA);
            if (lexeme("なんになります"))               return (T_IKURA);
            if (lexeme("なんなんでっ"))                 return (T_IKURA);
            if (lexeme("なんなんです"))                 return (T_IKURA);
            if (lexeme("なんなんでしょう"))             return (T_IKURA);
            if (lexeme("なんなんだろう"))               return (T_IKURA);
            if (lexeme("なんなん"))                     return (T_IKURA);
            if (lexeme("なんなのでっ"))                 return (T_IKURA);
            if (lexeme("なんなのです"))                 return (T_IKURA);
            if (lexeme("なんなのでしょう"))             return (T_IKURA);
            if (lexeme("なんなのだろう"))               return (T_IKURA);
            if (lexeme("なんなの"))                     return (T_IKURA);
            if (lexeme("なんでっ"))                     return (T_IKURA);
            if (lexeme("なんです"))                     return (T_IKURA);
            if (lexeme("なんでしょう"))                 return (T_IKURA);
            if (lexeme("なんだろう"))                   return (T_IKURA);
            if (lexeme("なんだ"))                       return (T_IKURA);
            if (lexeme("なん"))                         return (T_IKURA);
            if (lexeme("なゆた"))                       return (T_NAYUTA);
            if (lexeme("なにになるんです"))             return (T_IKURA);
            if (lexeme("なにになるのです"))             return (T_IKURA);
            if (lexeme("なにになるでしょう"))           return (T_IKURA);
            if (lexeme("なにになる"))                   return (T_IKURA);
            if (lexeme("なにでっ"))                     return (T_IKURA);
            if (lexeme("なにです"))                     return (T_IKURA);
            if (lexeme("なに"))                         return (T_IKURA);
            if (lexeme("なな"))                         return (T_NANA);
            if (lexeme("なおせよ"))                     return (T_SURUTO);
            if (lexeme("なおせや"))                     return (T_SURUTO);
            if (lexeme("なおせ"))                       return (T_SURUTO);
            if (lexeme("なおすときの"))                 return (T_SURUTO);
            if (lexeme("なおすとき"))                   return (T_SURUTO);
            if (lexeme("なおすと"))                     return (T_SURUTO);
            if (lexeme("なおしなさい"))                 return (T_SURUTO);
            if (lexeme("なおしてみろよ"))               return (T_SURUTO);
            if (lexeme("なおしてみろや"))               return (T_SURUTO);
            if (lexeme("なおしてみろ"))                 return (T_SURUTO);
            if (lexeme("なおしてみなさい"))             return (T_SURUTO);
            if (lexeme("なおして"))                     return (T_SURUTO);
            if (lexeme("なおしたら"))                   return (T_SURUTO);
            if (lexeme("なおした"))                     return (T_SHITA);
            if (lexeme("なあに"))                       return (T_IKURA);
            if (lexeme("どんなかずになるんでしょう"))   return (T_IKURA);
            if (lexeme("どんなかずになるのでしょう"))   return (T_IKURA);
            if (lexeme("どんなかずになるでしょう"))     return (T_IKURA);
            if (lexeme("どんなかずになる"))             return (T_IKURA);
            if (lexeme("どんなかずになります"))         return (T_IKURA);
            if (lexeme("どないなるんでしょう"))         return (T_IKURA);
            if (lexeme("どないなるのでしょう"))         return (T_IKURA);
            if (lexeme("どないなる"))                   return (T_IKURA);
            if (lexeme("どないでっ"))                   return (T_IKURA);
            if (lexeme("どないです"))                   return (T_IKURA);
            if (lexeme("どないでしょう"))               return (T_IKURA);
            if (lexeme("どない"))                       return (T_IKURA);
            if (lexeme("どうなんでっ"))                 return (T_IKURA);
            if (lexeme("どうなんです"))                 return (T_IKURA);
            if (lexeme("どうなんでしょう"))             return (T_IKURA);
            if (lexeme("どうなん"))                     return (T_IKURA);
            if (lexeme("どうなる"))                     return (T_IKURA);
            if (lexeme("どうなります"))                 return (T_IKURA);
            if (lexeme("どうなのでっ"))                 return (T_IKURA);
            if (lexeme("どうなのです"))                 return (T_IKURA);
            if (lexeme("どうなのでしょう"))             return (T_IKURA);
            if (lexeme("どうなの"))                     return (T_IKURA);
            if (lexeme("どう"))                         return (T_IKURA);
            if (lexeme("ど"))                           return (T_DO);
            if (lexeme("とね、"))                       return (T_TO1);
            if (lexeme("とな、"))                       return (T_TO1);
            if (lexeme("とじかっこ"))                   return (T_R_PAREN);
            if (lexeme("と"))                           return (T_TO1);
            if (lexeme("でんねん"))                     return (T_KA);
            if (lexeme("で"))                           return (T_DE);
            if (lexeme("てん"))                         return (T_DOT);
            if (lexeme("って"))                         return (T_HA);
            if (lexeme("ちょう"))                       return (T_CHOU);
            if (lexeme("たんぜんと"))                   return (T_TAN);
            if (lexeme("たんじぇんと"))                 return (T_TAN);
            if (lexeme("たせよ"))                       return (T_TASUTO);
            if (lexeme("たせや"))                       return (T_TASUTO);
            if (lexeme("たせば"))                       return (T_TASUTO);
            if (lexeme("たせ"))                         return (T_TASUTO);
            if (lexeme("たすとき"))                     return (T_TASUTO);
            if (lexeme("たすと"))                       return (T_TASUTO);
            if (lexeme("たすことの"))                   return (T_TASU);
            if (lexeme("たす"))                         return (T_TASU);
            if (lexeme("たしなさい"))                   return (T_TASUTO);
            if (lexeme("たして"))                       return (T_TASHITE);
            if (lexeme("たしたら"))                     return (T_TASUTO);
            if (lexeme("たしたときの"))                 return (T_TASUTO);
            if (lexeme("たしたとき"))                   return (T_TASUTO);
            if (lexeme("たした"))                       return (T_TASHITA);
            if (lexeme("たしざん"))                     return (T_TASHIZAN);
            if (lexeme("たしあわせろよ"))               return (T_TASUTO);
            if (lexeme("たしあわせろや"))               return (T_TASUTO);
            if (lexeme("たしあわせろ"))                 return (T_TASUTO);
            if (lexeme("たしあわせれば"))               return (T_TASUTO);
            if (lexeme("たしあわせるとき"))             return (T_TASUTO);
            if (lexeme("たしあわせると"))               return (T_TASUTO);
            if (lexeme("たしあわせる"))                 return (T_TASU);
            if (lexeme("たしあわせよ"))                 return (T_TASUTO);
            if (lexeme("たしあわせなさい"))             return (T_TASUTO);
            if (lexeme("たしあわせて"))                 return (T_TASHITE);
            if (lexeme("たしあわせたときの"))           return (T_TASUTO);
            if (lexeme("たしあわせたとき"))             return (T_TASUTO);
            if (lexeme("たしあわせた"))                 return (T_TASHITA);
            if (lexeme("たしあわせ"))                   return (T_TASUTO);
            if (lexeme("たしあわす"))                   return (T_TASU);
            if (lexeme("たしあわされる"))               return (T_TASARERU);
            if (lexeme("たし"))                         return (T_TASHITE);
            if (lexeme("たされる"))                     return (T_TASARERU);
            if (lexeme("たいすうかんすう"))             return (T_LOG);
            if (lexeme("たいすう"))                     return (T_LOG);
            if (lexeme("それの"))                       return (T_SONO);
            if (lexeme("それがしの"))                   return (T_SONO);
            if (lexeme("それがし"))                     return (T_SORE);
            if (lexeme("それ"))                         return (T_SORE);
            if (lexeme("そやつ"))                       return (T_SORE);
            if (lexeme("そのとき"))                     return (T_SURUTO);
            if (lexeme("そのこたえ"))                   return (T_KOTAE);
            if (lexeme("その"))                         return (T_SONO);
            if (lexeme("そうきょくせんよげん"))         return (T_COSH);
            if (lexeme("そうきょくせんたんぜんと"))     return (T_TANH);
            if (lexeme("そうきょくせんたんじぇんと"))   return (T_TANH);
            if (lexeme("そうきょくせんせいせつ"))       return (T_TANH);
            if (lexeme("そうきょくせんせいげん"))       return (T_SINH);
            if (lexeme("そうきょくせんさいん"))         return (T_SINH);
            if (lexeme("そうきょくせんこさいん"))       return (T_COSH);
            if (lexeme("そいつ"))                       return (T_SORE);
            if (lexeme("ぜんぶ"))                       return (T_ALL);
            if (lexeme("ぜん"))                         return (T_SEN);
            if (lexeme("ぜろ"))                         return (T_ZERO);
            if (lexeme("ぜったいち"))                   return (T_ZETTAICHI);
            if (lexeme("せん"))                         return (T_SEN);
            if (lexeme("せよ"))                         return (T_SURUTO);
            if (lexeme("せき"))                         return (T_SEKI);
            if (lexeme("せいや"))                       return (T_SURUTO);
            if (lexeme("せいせつ"))                     return (T_TAN);
            if (lexeme("せいげん"))                     return (T_SIN);
            if (lexeme("せい"))                         return (T_SEI);
            if (lexeme("すれば"))                       return (T_SURUTO);
            if (lexeme("するときの"))                   return (T_SURUTO);
            if (lexeme("するとき"))                     return (T_SURUTO);
            if (lexeme("すると"))                       return (T_SURUTO);
            if (lexeme("すべて"))                       return (T_ALL);
            if (lexeme("じょー"))                       return (T_JOU1);
            if (lexeme("じょざん"))                     return (T_WARIZAN);
            if (lexeme("じょうようたいすう"))           return (T_LOG10);
            if (lexeme("じょうよ"))                     return (T_AMARI);
            if (lexeme("じょうざん"))                   return (T_KAKEZAN);
            if (lexeme("じょう"))                       return (T_JOU1);
            if (lexeme("じょ"))                         return (T_JO);
            if (lexeme("じゅー"))                       return (T_JUU);
            if (lexeme("じゅっ"))                       return (T_JUU);
            if (lexeme("じゅう"))                       return (T_JUU);
            if (lexeme("じっ"))                         return (T_JUU);
            if (lexeme("じじょう"))                     return (T_HEIHOU);
            if (lexeme("しーや"))                       return (T_SURUTO);
            if (lexeme("しー"))                         return (T_YON);
            if (lexeme("しろよ"))                       return (T_SURUTO);
            if (lexeme("しろや"))                       return (T_SURUTO);
            if (lexeme("しろ"))                         return (T_SURUTO);
            if (lexeme("しらん"))                       return (T_WAKARAN);
            if (lexeme("しらない"))                     return (T_WAKARAN);
            if (lexeme("しょーすー"))                   return (T_SHOUSUU);
            if (lexeme("しょーすう"))                   return (T_SHOUSUU);
            if (lexeme("しょうすー"))                   return (T_SHOUSUU);
            if (lexeme("しょうすう"))                   return (T_SHOUSUU);
            if (lexeme("しょう"))                       return (T_SHOU);
            if (lexeme("しなさい"))                     return (T_SURUTO);
            if (lexeme("してよ"))                       return (T_SHITE);
            if (lexeme("してや"))                       return (T_SHITE);
            if (lexeme("してみろよ"))                   return (T_SURUTO);
            if (lexeme("してみろや"))                   return (T_SURUTO);
            if (lexeme("してみろ"))                     return (T_SURUTO);
            if (lexeme("してみれば"))                   return (T_SURUTO);
            if (lexeme("してみるときの"))               return (T_SURUTO);
            if (lexeme("してみるとき"))                 return (T_SURUTO);
            if (lexeme("してみると"))                   return (T_SURUTO);
            if (lexeme("してみなさい"))                 return (T_SURUTO);
            if (lexeme("して"))                         return (T_SHITE);
            if (lexeme("しち"))                         return (T_NANA);
            if (lexeme("したら"))                       return (T_SURUTO);
            if (lexeme("したときの"))                   return (T_SURUTO);
            if (lexeme("したとき"))                     return (T_SURUTO);
            if (lexeme("した"))                         return (T_SHITA);
            if (lexeme("しぜんたいすうのてい"))         return (T_E);
            if (lexeme("しぜんたいすう"))               return (T_LOG);
            if (lexeme("しすうかんすう"))               return (T_EXP);
            if (lexeme("しすう"))                       return (T_EXP);
            if (lexeme("しいや"))                       return (T_SURUTO);
            if (lexeme("しい"))                         return (T_YON);
            if (lexeme("しぃ"))                         return (T_YON);
            if (lexeme("し"))                           return (T_YON);
            if (lexeme("さん"))                         return (T_SAN);
            if (lexeme("さしひけよ"))                   return (T_HIKUTO);
            if (lexeme("さしひけや"))                   return (T_HIKUTO);
            if (lexeme("さしひけ"))                     return (T_HIKUTO);
            if (lexeme("さしひくとき"))                 return (T_HIKUTO);
            if (lexeme("さしひきなさい"))               return (T_HIKUTO);
            if (lexeme("さしひき"))                     return (T_HIITE);
            if (lexeme("さしひかれる"))                 return (T_HIKARERU);
            if (lexeme("さしひいて"))                   return (T_HIITE);
            if (lexeme("さしひいたら"))                 return (T_HIKUTO);
            if (lexeme("さしひいたときの"))             return (T_HIKUTO);
            if (lexeme("さしひいたとき"))               return (T_HIKUTO);
            if (lexeme("さしひいた"))                   return (T_HIITA);
            if (lexeme("さいん"))                       return (T_SIN);
            if (lexeme("さい"))                         return (T_SAI);
            if (lexeme("さ"))                           return (T_SA);
            if (lexeme("ごく"))                         return (T_GOKU);
            if (lexeme("ごうがしゃ"))                   return (T_GOUGASHA);
            if (lexeme("ごう"))                         return (T_GO);
            if (lexeme("ご"))                           return (T_GO);
            if (lexeme("これの"))                       return (T_SONO);
            if (lexeme("これ"))                         return (T_SORE);
            if (lexeme("こやつ"))                       return (T_SORE);
            if (lexeme("このとき"))                     return (T_SURUTO);
            if (lexeme("こたえろよ"))                   return (T_OSHIETE);
            if (lexeme("こたえろや"))                   return (T_OSHIETE);
            if (lexeme("こたえろ"))                     return (T_OSHIETE);
            if (lexeme("こたえよ"))                     return (T_OSHIETE);
            if (lexeme("こたえなさい"))                 return (T_OSHIETE);
            if (lexeme("こたえてくれるかい"))           return (T_OSHIETE);
            if (lexeme("こたえてくれるか"))             return (T_OSHIETE);
            if (lexeme("こたえてくれる"))               return (T_OSHIETE);
            if (lexeme("こたえてくれよ"))               return (T_OSHIETE);
            if (lexeme("こたえてくれや"))               return (T_OSHIETE);
            if (lexeme("こたえてくれ"))                 return (T_OSHIETE);
            if (lexeme("こたえてくださいよ"))           return (T_OSHIETE);
            if (lexeme("こたえてください"))             return (T_OSHIETE);
            if (lexeme("こたえ"))                       return (T_KOTAE);
            if (lexeme("こさいん"))                     return (T_COS);
            if (lexeme("こう"))                         return (T_KOU);
            if (lexeme("こいつ"))                       return (T_SORE);
            if (lexeme("げんざん"))                     return (T_HIKIZAN);
            if (lexeme("けいさん"))                     return (T_KEISAN);
            if (lexeme("けい"))                         return (T_KEI);
            if (lexeme("くわえろよ"))                   return (T_TASUTO);
            if (lexeme("くわえろや"))                   return (T_TASUTO);
            if (lexeme("くわえろ"))                     return (T_TASUTO);
            if (lexeme("くわえるときの"))               return (T_TASUTO);
            if (lexeme("くわえるとき"))                 return (T_TASUTO);
            if (lexeme("くわえると"))                   return (T_TASUTO);
            if (lexeme("くわえることの"))               return (T_TASU);
            if (lexeme("くわえる"))                     return (T_TASU);
            if (lexeme("くわえられる"))                 return (T_TASARERU);
            if (lexeme("くわえなさい"))                 return (T_TASUTO);
            if (lexeme("くわえて"))                     return (T_TASHITE);
            if (lexeme("くわえたら"))                   return (T_TASUTO);
            if (lexeme("くわえたときの"))               return (T_TASUTO);
            if (lexeme("くわえたとき"))                 return (T_TASUTO);
            if (lexeme("くわえた"))                     return (T_TASHITA);
            if (lexeme("くれる"))                       return (T_KA);
            if (lexeme("くれよー"))                     return (T_KA);
            if (lexeme("くれよぅ"))                     return (T_KA);
            if (lexeme("くれよ"))                       return (T_KA);
            if (lexeme("くれや"))                       return (T_KA);
            if (lexeme("くれ"))                         return (T_KA);
            if (lexeme("くださいよー"))                 return (T_KA);
            if (lexeme("くださいよぅ"))                 return (T_KA);
            if (lexeme("くださいよ"))                   return (T_KA);
            if (lexeme("ください"))                     return (T_KA);
            if (lexeme("く"))                           return (T_KYUU);
            if (lexeme("ぎゃくよげん"))                 return (T_ACOS);
            if (lexeme("ぎゃくたんぜんと"))             return (T_ATAN);
            if (lexeme("ぎゃくたんじぇんと"))           return (T_ATAN);
            if (lexeme("ぎゃくせいせつ"))               return (T_ATAN);
            if (lexeme("ぎゃくせいげん"))               return (T_ASIN);
            if (lexeme("ぎゃくすう"))                   return (T_GYAKUSUU);
            if (lexeme("ぎゃくさいん"))                 return (T_ASIN);
            if (lexeme("ぎゃくこさいん"))               return (T_ACOS);
            if (lexeme("きゅー"))                       return (T_KYUU);
            if (lexeme("きゅう"))                       return (T_KYUU);
            if (lexeme("がい"))                         return (T_GAI);
            if (lexeme("が"))                           return (T_HA);
            if (lexeme("かー"))                         return (T_KA);
            if (lexeme("かん"))                         return (T_KAN);
            if (lexeme("から"))                         return (T_KARA);
            if (lexeme("かねー"))                       return (T_KA);
            if (lexeme("かねえ"))                       return (T_KA);
            if (lexeme("かねぇ"))                       return (T_KA);
            if (lexeme("かね"))                         return (T_KA);
            if (lexeme("かなー"))                       return (T_KA);
            if (lexeme("かなあ"))                       return (T_KA);
            if (lexeme("かなぁ"))                       return (T_KA);
            if (lexeme("かな"))                         return (T_KA);
            if (lexeme("かっこをとじる"))               return (T_R_PAREN);
            if (lexeme("かっこをとじて"))               return (T_R_PAREN);
            if (lexeme("かっことじる"))                 return (T_R_PAREN);
            if (lexeme("かっことじて"))                 return (T_R_PAREN);
            if (lexeme("かっことじ"))                   return (T_R_PAREN);
            if (lexeme("かっこ"))                       return (T_L_PAREN);
            if (lexeme("かず"))                         return (T_MONO);
            if (lexeme("かさん"))                       return (T_TASHIZAN);
            if (lexeme("かけろよ"))                     return (T_KAKERUTO);
            if (lexeme("かけろや"))                     return (T_KAKERUTO);
            if (lexeme("かけろ"))                       return (T_KAKERUTO);
            if (lexeme("かければ"))                     return (T_KAKERUTO);
            if (lexeme("かけるときの"))                 return (T_KAKERUTO);
            if (lexeme("かけるとき"))                   return (T_KAKERUTO);
            if (lexeme("かけると"))                     return (T_KAKERUTO);
            if (lexeme("かけることの"))                 return (T_KAKERU);
            if (lexeme("かける"))                       return (T_KAKERU);
            if (lexeme("かけられる"))                   return (T_KAKERARERU);
            if (lexeme("かけなさい"))                   return (T_KAKERUTO);
            if (lexeme("かけて"))                       return (T_KAKETE);
            if (lexeme("かけたら"))                     return (T_KAKERUTO);
            if (lexeme("かけたときの"))                 return (T_KAKERUTO);
            if (lexeme("かけたとき"))                   return (T_KAKERUTO);
            if (lexeme("かけた"))                       return (T_KAKETA);
            if (lexeme("かけざん"))                     return (T_KAKEZAN);
            if (lexeme("かけあわせろよ"))               return (T_KAKERUTO);
            if (lexeme("かけあわせろや"))               return (T_KAKERUTO);
            if (lexeme("かけあわせろ"))                 return (T_KAKERUTO);
            if (lexeme("かけあわせれば"))               return (T_KAKERUTO);
            if (lexeme("かけあわせるとき"))             return (T_KAKERUTO);
            if (lexeme("かけあわせると"))               return (T_KAKERUTO);
            if (lexeme("かけあわせる"))                 return (T_KAKERU);
            if (lexeme("かけあわせよ"))                 return (T_KAKERUTO);
            if (lexeme("かけあわせなさい"))             return (T_KAKERUTO);
            if (lexeme("かけあわせて"))                 return (T_KAKERUTO);
            if (lexeme("かけあわせたときの"))           return (T_KAKERUTO);
            if (lexeme("かけあわせたとき"))             return (T_KAKERUTO);
            if (lexeme("かけあわせた"))                 return (T_KAKETA);
            if (lexeme("かけあわせ"))                   return (T_KAKERUTO);
            if (lexeme("かけあわすときの"))             return (T_KAKERUTO);
            if (lexeme("かけあわすとき"))               return (T_KAKERUTO);
            if (lexeme("かけあわす"))                   return (T_KAKERU);
            if (lexeme("かけあわしたときの"))           return (T_KAKERUTO);
            if (lexeme("かけあわしたとき"))             return (T_KAKERUTO);
            if (lexeme("かけあわされる"))               return (T_KAKERARERU);
            if (lexeme("かけ"))                         return (T_KAKERU);
            if (lexeme("かい"))                         return (T_KA);
            if (lexeme("かあ"))                         return (T_KA);
            if (lexeme("かぁ"))                         return (T_KA);
            if (lexeme("か"))                           return (T_KA);
            if (lexeme("おねがいよ"))                   return (T_ONEGAI);
            if (lexeme("おねがいやねん"))               return (T_ONEGAI);
            if (lexeme("おねがいする"))                 return (T_ONEGAI);
            if (lexeme("おねがいしますよ"))             return (T_ONEGAI);
            if (lexeme("おねがいしますね"))             return (T_ONEGAI);
            if (lexeme("おねがいします"))               return (T_ONEGAI);
            if (lexeme("おねがいいたしますよ"))         return (T_ONEGAI);
            if (lexeme("おねがいいたしますね"))         return (T_ONEGAI);
            if (lexeme("おねがいいたします"))           return (T_ONEGAI);
            if (lexeme("おねがい"))                     return (T_ONEGAI);
            if (lexeme("おしえろよ"))                   return (T_OSHIETE);
            if (lexeme("おしえろや"))                   return (T_OSHIETE);
            if (lexeme("おしえろ"))                     return (T_OSHIETE);
            if (lexeme("おしえよ"))                     return (T_OSHIETE);
            if (lexeme("おしえなさい"))                 return (T_OSHIETE);
            if (lexeme("おしえてん"))                   return (T_OSHIETE);
            if (lexeme("おしえてよ"))                   return (T_OSHIETE);
            if (lexeme("おしえてや"))                   return (T_OSHIETE);
            if (lexeme("おしえてほしいねん"))           return (T_OSHIETE);
            if (lexeme("おしえてちょうだいよ"))         return (T_OSHIETE);
            if (lexeme("おしえてちょうだい"))           return (T_OSHIETE);
            if (lexeme("おしえてくれるかい"))           return (T_OSHIETE);
            if (lexeme("おしえてくれるか"))             return (T_OSHIETE);
            if (lexeme("おしえてくれる"))               return (T_OSHIETE);
            if (lexeme("おしえてくれ"))                 return (T_OSHIETE);
            if (lexeme("おしえてくださいよ"))           return (T_OSHIETE);
            if (lexeme("おしえてください"))             return (T_OSHIETE);
            if (lexeme("おしえて"))                     return (T_OSHIETE);
            if (lexeme("おく"))                         return (T_OKU);
            if (lexeme("えんしゅうりつ"))               return (T_PI);
            if (lexeme("いー"))                         return (T_E);
            if (lexeme("いっ"))                         return (T_ICHI);
            if (lexeme("いち"))                         return (T_ICHI);
            if (lexeme("いくらになるでしょう"))         return (T_IKURA);
            if (lexeme("いくらになる"))                 return (T_IKURA);
            if (lexeme("いくらになります"))             return (T_IKURA);
            if (lexeme("いくらでっ"))                   return (T_IKURA);
            if (lexeme("いくらです"))                   return (T_IKURA);
            if (lexeme("いくら"))                       return (T_IKURA);
            if (lexeme("いくつや"))                     return (T_IKURA);
            if (lexeme("いくつになるでしょう"))         return (T_IKURA);
            if (lexeme("いくつになる"))                 return (T_IKURA);
            if (lexeme("いくつになります"))             return (T_IKURA);
            if (lexeme("いくつなんでっ"))               return (T_IKURA);
            if (lexeme("いくつなんです"))               return (T_IKURA);
            if (lexeme("いくつなんでしょう"))           return (T_IKURA);
            if (lexeme("いくつなん"))                   return (T_IKURA);
            if (lexeme("いくつなのでっ"))               return (T_IKURA);
            if (lexeme("いくつなのです"))               return (T_IKURA);
            if (lexeme("いくつなのでしょう"))           return (T_IKURA);
            if (lexeme("いくつなの"))                   return (T_IKURA);
            if (lexeme("いくつ"))                       return (T_IKURA);
            if (lexeme("あーくたんぜんと"))             return (T_ATAN);
            if (lexeme("あーくたんじぇんと"))           return (T_ATAN);
            if (lexeme("あーくさいん"))                 return (T_ASIN);
            if (lexeme("あーくこさいん"))               return (T_ACOS);
            if (lexeme("あまり"))                       return (T_AMARI);
            if (lexeme("あそうぎ"))                     return (T_ASOUGI);
            if (lexeme("〕"))                           return (T_R_PAREN);
            if (lexeme("〔"))                           return (T_L_PAREN);
            if (lexeme("】"))                           return (T_R_PAREN);
            if (lexeme("【"))                           return (T_L_PAREN);
            if (lexeme("』"))                           return (T_R_PAREN);
            if (lexeme("『"))                           return (T_L_PAREN);
            if (lexeme("」"))                           return (T_R_PAREN);
            if (lexeme("「"))                           return (T_L_PAREN);
            if (lexeme("》"))                           return (T_R_PAREN);
            if (lexeme("《"))                           return (T_L_PAREN);
            if (lexeme("〉"))                           return (T_R_PAREN);
            if (lexeme("〈"))                           return (T_L_PAREN);
            if (lexeme("〇"))                           return (T_ZERO);
            if (lexeme("。"))                           return (T_PERIOD);
            if (lexeme("、"))                           return (T_COMMA);
            if (lexeme("☆"))                           return (T_PERIOD);
            if (lexeme("★"))                           return (T_PERIOD);
            if (lexeme("●"))                           return (T_PERIOD);
            if (lexeme("◎"))                           return (T_PERIOD);
            if (lexeme("÷"))                           return (T_WARU);
            if (lexeme("×"))                           return (T_KAKERU);
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

            message(str + "がわかりません。");

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
                        message("かっこの対応がおかしいです。");
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
        // T_NO2: 「わ」「さ」「せき」「しょう」の直前の「の」。
        // T_NO3: 「何の何倍」の「の」。「何の（...）」の「の」。
        // T_NO4: 「かけざん」「けいさん」「こたえ」などの直前の「の」。
        // T_NO5: 「のたすかず」「のたされるかず」「のかけるかず」の「の」
        // T_NO6: 「の何乗」「の平方」「の立方」の「の」。
        // T_NO7: 三角関数「正弦」「余弦」「正接」の直前の「の」。
        // T_NO8: 「の何ぱーせんと」の「の」。
        // T_NO9: 「の余り」の「の」。
        // T_NO1: それ以外。
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

        // 特定の語句の後のT_COMMAを削除する。
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

    private:
        // NOTE: Scanner is not copyable.
        Scanner(const Scanner<Iterator, ParserSite>&);
        Scanner& operator=(const Scanner<Iterator, ParserSite>&);
    }; // class Scanner<Iterator, ParserSite>
} // namespace Calc_H

#endif  // ndef HSCANNER_H_
