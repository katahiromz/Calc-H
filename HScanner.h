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
                resynth13(infos);
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

            if (!lexeme("ではち", false) && lexeme("では")) return (T_HA);
            if (!lexeme("にいち", false) && lexeme("にい")) return (T_NI);

            // 最長一致法。
            // 【ここから】行を降順に並び替えておく。
            if (lexeme("～"))                           return set_info(info, T_KARA);
            if (lexeme("｝"))                           return set_info(info, T_R_PAREN);
            if (lexeme("｛"))                           return set_info(info, T_L_PAREN);
            if (lexeme("ｔａｎｈ"))                     return set_info(info, T_TANH);
            if (lexeme("ｔａｎ"))                       return set_info(info, T_TAN);
            if (lexeme("ｓｉｎｈ"))                     return set_info(info, T_SINH);
            if (lexeme("ｓｉｎ"))                       return set_info(info, T_SIN);
            if (lexeme("ｌｏｇ１０"))                   return set_info(info, T_LOG10);
            if (lexeme("ｌｏｇ"))                       return set_info(info, T_LOG);
            if (lexeme("ｌｎ"))                         return set_info(info, T_LOG);
            if (lexeme("ｆａｂｓ"))                     return set_info(info, T_ZETTAICHI);
            if (lexeme("ｅｘｐ"))                       return set_info(info, T_EXP);
            if (lexeme("ｃｏｓｈ"))                     return set_info(info, T_COSH);
            if (lexeme("ｃｏｓ"))                       return set_info(info, T_COS);
            if (lexeme("ａｔａｎ"))                     return set_info(info, T_ATAN);
            if (lexeme("ａｓｉｎ"))                     return set_info(info, T_ASIN);
            if (lexeme("ａｒｃｔａｎ"))                 return set_info(info, T_ATAN);
            if (lexeme("ａｒｃｓｉｎ"))                 return set_info(info, T_ASIN);
            if (lexeme("ａｒｃｃｏｓ"))                 return set_info(info, T_ACOS);
            if (lexeme("ａｃｏｓ"))                     return set_info(info, T_ACOS);
            if (lexeme("ａｂｓ"))                       return set_info(info, T_ZETTAICHI);
            if (lexeme("］"))                           return set_info(info, T_R_PAREN);
            if (lexeme("［"))                           return set_info(info, T_L_PAREN);
            if (lexeme("？"))                           return set_info(info, T_PERIOD);
            if (lexeme("＝"))                           return set_info(info, T_HA);
            if (lexeme("．"))                           return set_info(info, T_PERIOD);
            if (lexeme("－"))                           return set_info(info, T_MINUS);
            if (lexeme("，"))                           return set_info(info, T_COMMA);
            if (lexeme("＋"))                           return set_info(info, T_TASU);
            if (lexeme("）"))                           return set_info(info, T_R_PAREN);
            if (lexeme("（"))                           return set_info(info, T_L_PAREN);
            if (lexeme("％"))                           return set_info(info, T_PERCENT);
            if (lexeme("！"))                           return set_info(info, T_PERIOD);
            if (lexeme("零"))                           return set_info(info, T_ZERO);
            if (lexeme("陸"))                           return set_info(info, T_ROKU);
            if (lexeme("阿僧祗"))                       return set_info(info, T_ASOUGI);
            if (lexeme("阡"))                           return set_info(info, T_SEN);
            if (lexeme("那由多"))                       return set_info(info, T_NAYUTA);
            if (lexeme("那由他"))                       return set_info(info, T_NAYUTA);
            if (lexeme("載"))                           return set_info(info, T_SAI);
            if (lexeme("肆"))                           return set_info(info, T_YON);
            if (lexeme("穣"))                           return set_info(info, T_JOU2);
            if (lexeme("百"))                           return set_info(info, T_HYAKU);
            if (lexeme("玖"))                           return set_info(info, T_KYUU);
            if (lexeme("無量大数"))                     return set_info(info, T_MURYOUTAISUU);
            if (lexeme("澗"))                           return set_info(info, T_KAN);
            if (lexeme("漆"))                           return set_info(info, T_NANA);
            if (lexeme("溝"))                           return set_info(info, T_KOU);
            if (lexeme("正"))                           return set_info(info, T_SEI);
            if (lexeme("極"))                           return set_info(info, T_GOKU);
            if (lexeme("拾"))                           return set_info(info, T_JUU);
            if (lexeme("恒河沙"))                       return set_info(info, T_GOUGASHA);
            if (lexeme("弐"))                           return set_info(info, T_NI);
            if (lexeme("壱"))                           return set_info(info, T_ICHI);
            if (lexeme("垓"))                           return set_info(info, T_GAI);
            if (lexeme("四"))                           return set_info(info, T_YON);
            if (lexeme("参"))                           return set_info(info, T_SAN);
            if (lexeme("千"))                           return set_info(info, T_SEN);
            if (lexeme("十"))                           return set_info(info, T_JUU);
            if (lexeme("六"))                           return set_info(info, T_ROKU);
            if (lexeme("八"))                           return set_info(info, T_HACHI);
            if (lexeme("兆"))                           return set_info(info, T_CHOU);
            if (lexeme("億"))                           return set_info(info, T_OKU);
            if (lexeme("伍"))                           return set_info(info, T_GO);
            if (lexeme("仟"))                           return set_info(info, T_SEN);
            if (lexeme("京"))                           return set_info(info, T_KEI);
            if (lexeme("五"))                           return set_info(info, T_GO);
            if (lexeme("二"))                           return set_info(info, T_NI);
            if (lexeme("九"))                           return set_info(info, T_KYUU);
            if (lexeme("不可思議"))                     return set_info(info, T_FUKASHIGI);
            if (lexeme("三"))                           return set_info(info, T_SAN);
            if (lexeme("万"))                           return set_info(info, T_MAN);
            if (lexeme("七"))                           return set_info(info, T_NANA);
            if (lexeme("一"))                           return set_info(info, T_ICHI);
            if (lexeme("・"))                           return set_info(info, T_DOT);
            if (lexeme("んや"))                         return set_info(info, T_KANA);
            if (lexeme("をわることの"))                 return set_info(info, T_WARU);
            if (lexeme("をひくことの"))                 return set_info(info, T_HIKU);
            if (lexeme("を"))                           return set_info(info, T_WO1);
            if (lexeme("われよ"))                       return set_info(info, T_WARUTO);
            if (lexeme("われや"))                       return set_info(info, T_WARUTO);
            if (lexeme("われば"))                       return set_info(info, T_WARUTO);
            if (lexeme("われ"))                         return set_info(info, T_WARUTO);
            if (lexeme("わるときの"))                   return set_info(info, T_WARUTO);
            if (lexeme("わるとき"))                     return set_info(info, T_WARUTO);
            if (lexeme("わると"))                       return set_info(info, T_WARUTO);
            if (lexeme("わることの"))                   return set_info(info, T_WARU);
            if (lexeme("わる"))                         return set_info(info, T_WARU);
            if (lexeme("わりなさい"))                   return set_info(info, T_WARUTO);
            if (lexeme("わりざん"))                     return set_info(info, T_WARIZAN);
            if (lexeme("わり"))                         return set_info(info, T_WATTE);
            if (lexeme("わられる"))                     return set_info(info, T_WARARERU);
            if (lexeme("わってみよ"))                   return set_info(info, T_WARUTO);
            if (lexeme("わってみい"))                   return set_info(info, T_WARUTO);
            if (lexeme("わってみぃ"))                   return set_info(info, T_WARUTO);
            if (lexeme("わってみ"))                     return set_info(info, T_WARUTO);
            if (lexeme("わって"))                       return set_info(info, T_WATTE);
            if (lexeme("わったら"))                     return set_info(info, T_WARUTO);
            if (lexeme("わったときの"))                 return set_info(info, T_WARUTO);
            if (lexeme("わったとき"))                   return set_info(info, T_WARUTO);
            if (lexeme("わった"))                       return set_info(info, T_WATTA);
            if (lexeme("わかる"))                       return set_info(info, T_WAKARAN);
            if (lexeme("わかりません"))                 return set_info(info, T_WAKARAN);
            if (lexeme("わかります"))                   return set_info(info, T_WAKARAN);
            if (lexeme("わからんとよ"))                 return set_info(info, T_WAKARAN);
            if (lexeme("わからん"))                     return set_info(info, T_WAKARAN);
            if (lexeme("わからないよー"))               return set_info(info, T_WAKARAN);
            if (lexeme("わからないよお"))               return set_info(info, T_WAKARAN);
            if (lexeme("わからないよ"))                 return set_info(info, T_WAKARAN);
            if (lexeme("わからない"))                   return set_info(info, T_WAKARAN);
            if (lexeme("わ"))                           return set_info(info, T_SUM);
            if (lexeme("ろっ"))                         return set_info(info, T_ROKU);
            if (lexeme("ろぐ"))                         return set_info(info, T_LOG);
            if (lexeme("ろく"))                         return set_info(info, T_ROKU);
            if (lexeme("れい"))                         return set_info(info, T_ZERO);
            if (lexeme("るーと"))                       return set_info(info, T_ROOT);
            if (lexeme("るうと"))                       return set_info(info, T_ROOT);
            if (lexeme("りっぽう"))                     return set_info(info, T_RIPPOU);
            if (lexeme("よー"))                         return set_info(info, T_KANA);
            if (lexeme("よん"))                         return set_info(info, T_YON);
            if (lexeme("よげん"))                       return set_info(info, T_COS);
            if (lexeme("よお"))                         return set_info(info, T_KANA);
            if (lexeme("よぉ"))                         return set_info(info, T_KANA);
            if (lexeme("よう"))                         return set_info(info, T_KANA);
            if (lexeme("よぅ"))                         return set_info(info, T_KANA);
            if (lexeme("よ"))                           return set_info(info, T_KANA);
            if (lexeme("やれよ"))                       return set_info(info, T_SURUTO);
            if (lexeme("やれや"))                       return set_info(info, T_SURUTO);
            if (lexeme("やれば"))                       return set_info(info, T_SURUTO);
            if (lexeme("やれ"))                         return set_info(info, T_SURUTO);
            if (lexeme("やると"))                       return set_info(info, T_SURUTO);
            if (lexeme("やりなさい"))                   return set_info(info, T_SURUTO);
            if (lexeme("やねん"))                       return set_info(info, T_KANA);
            if (lexeme("やね"))                         return set_info(info, T_KANA);
            if (lexeme("やってみろよ"))                 return set_info(info, T_SURUTO);
            if (lexeme("やってみろや"))                 return set_info(info, T_SURUTO);
            if (lexeme("やってみろ"))                   return set_info(info, T_SURUTO);
            if (lexeme("やってみれば"))                 return set_info(info, T_SURUTO);
            if (lexeme("やってみるとき"))               return set_info(info, T_SURUTO);
            if (lexeme("やってみると"))                 return set_info(info, T_SURUTO);
            if (lexeme("やってみよ"))                   return set_info(info, T_SURUTO);
            if (lexeme("やってみたら"))                 return set_info(info, T_SURUTO);
            if (lexeme("やってみたとき"))               return set_info(info, T_SURUTO);
            if (lexeme("やってみい"))                   return set_info(info, T_SURUTO);
            if (lexeme("やってみぃ"))                   return set_info(info, T_SURUTO);
            if (lexeme("やってみ"))                     return set_info(info, T_SURUTO);
            if (lexeme("やって"))                       return set_info(info, T_SHITE);
            if (lexeme("やったら"))                     return set_info(info, T_SURUTO);
            if (lexeme("やったとき"))                   return set_info(info, T_SURUTO);
            if (lexeme("やった"))                       return set_info(info, T_SHITA);
            if (lexeme("や"))                           return set_info(info, T_KANA);
            if (lexeme("もの"))                         return set_info(info, T_MONO);
            if (lexeme("もとめると"))                   return set_info(info, T_SURUTO);
            if (lexeme("もとめよ"))                     return set_info(info, T_SURUTO);
            if (lexeme("もとめなさい"))                 return set_info(info, T_SURUTO);
            if (lexeme("もとめてみよ"))                 return set_info(info, T_SURUTO);
            if (lexeme("もとめてみい"))                 return set_info(info, T_SURUTO);
            if (lexeme("もとめてみぃ"))                 return set_info(info, T_SURUTO);
            if (lexeme("もとめてみ"))                   return set_info(info, T_SURUTO);
            if (lexeme("もとめて"))                     return set_info(info, T_SHITE);
            if (lexeme("もとめたら"))                   return set_info(info, T_SURUTO);
            if (lexeme("もとめた"))                     return set_info(info, T_SHITA);
            if (lexeme("もとめ"))                       return set_info(info, T_SHITE);
            if (lexeme("も"))                           return set_info(info, T_WO1);
            if (lexeme("むりょうたいすう"))             return set_info(info, T_MURYOUTAISUU);
            if (lexeme("みんな"))                       return set_info(info, T_ALL);
            if (lexeme("みれば"))                       return set_info(info, T_MIRUTO);
            if (lexeme("みると"))                       return set_info(info, T_MIRUTO);
            if (lexeme("まん"))                         return set_info(info, T_MAN);
            if (lexeme("まいなすするときの"))           return set_info(info, T_HIKUTO);
            if (lexeme("まいなすするとき"))             return set_info(info, T_HIKUTO);
            if (lexeme("まいなすしろよ"))               return set_info(info, T_HIKUTO);
            if (lexeme("まいなすしろや"))               return set_info(info, T_HIKUTO);
            if (lexeme("まいなすしろ"))                 return set_info(info, T_HIKUTO);
            if (lexeme("まいなすしなさい"))             return set_info(info, T_HIKUTO);
            if (lexeme("まいなすしてみよ"))             return set_info(info, T_HIKUTO);
            if (lexeme("まいなすしてみい"))             return set_info(info, T_HIKUTO);
            if (lexeme("まいなすしてみぃ"))             return set_info(info, T_HIKUTO);
            if (lexeme("まいなすしてみ"))               return set_info(info, T_HIKUTO);
            if (lexeme("まいなすして"))                 return set_info(info, T_HIITE);
            if (lexeme("まいなすしたら"))               return set_info(info, T_HIKUTO);
            if (lexeme("まいなすしたとき"))             return set_info(info, T_HIKUTO);
            if (lexeme("まいなすした"))                 return set_info(info, T_HIITA);
            if (lexeme("まいなすし"))                   return set_info(info, T_HIITE);
            if (lexeme("まいなすされる"))               return set_info(info, T_HIKARERU);
            if (lexeme("まいなす"))                     return set_info(info, T_MINUS);
            if (lexeme("へいほう"))                     return set_info(info, T_HEIHOU);
            if (lexeme("へいきんち"))                   return set_info(info, T_AVERAGE);
            if (lexeme("へいきん"))                     return set_info(info, T_AVERAGE);
            if (lexeme("へ"))                           return set_info(info, T_NI);
            if (lexeme("ぷらすするときの"))             return set_info(info, T_TASUTO);
            if (lexeme("ぷらすするとき"))               return set_info(info, T_TASUTO);
            if (lexeme("ぷらすしろよ"))                 return set_info(info, T_TASUTO);
            if (lexeme("ぷらすしろや"))                 return set_info(info, T_TASUTO);
            if (lexeme("ぷらすしろ"))                   return set_info(info, T_TASUTO);
            if (lexeme("ぷらすしなさい"))               return set_info(info, T_TASUTO);
            if (lexeme("ぷらすしてみよ"))               return set_info(info, T_TASUTO);
            if (lexeme("ぷらすしてみい"))               return set_info(info, T_TASUTO);
            if (lexeme("ぷらすしてみぃ"))               return set_info(info, T_TASUTO);
            if (lexeme("ぷらすしてみ"))                 return set_info(info, T_TASUTO);
            if (lexeme("ぷらすして"))                   return set_info(info, T_TASHITE);
            if (lexeme("ぷらすしたら"))                 return set_info(info, T_TASUTO);
            if (lexeme("ぷらすしたとき"))               return set_info(info, T_TASUTO);
            if (lexeme("ぷらすした"))                   return set_info(info, T_TASHITA);
            if (lexeme("ぷらすし"))                     return set_info(info, T_TASHITE);
            if (lexeme("ぷらすされる"))                 return set_info(info, T_TASARERU);
            if (lexeme("ぷらす"))                       return set_info(info, T_PLUS);
            if (lexeme("ぶんの"))                       return set_info(info, T_BUNNO);
            if (lexeme("ふかしぎ"))                     return set_info(info, T_FUKASHIGI);
            if (lexeme("ぴゃく"))                       return set_info(info, T_HYAKU);
            if (lexeme("びゃく"))                       return set_info(info, T_HYAKU);
            if (lexeme("ひゃく"))                       return set_info(info, T_HYAKU);
            if (lexeme("ひち"))                         return set_info(info, T_NANA);
            if (lexeme("ひけよ"))                       return set_info(info, T_HIKUTO);
            if (lexeme("ひけや"))                       return set_info(info, T_HIKUTO);
            if (lexeme("ひけば"))                       return set_info(info, T_HIKUTO);
            if (lexeme("ひけ"))                         return set_info(info, T_HIKUTO);
            if (lexeme("ひくときの"))                   return set_info(info, T_HIKUTO);
            if (lexeme("ひくとき"))                     return set_info(info, T_HIKUTO);
            if (lexeme("ひくと"))                       return set_info(info, T_HIKUTO);
            if (lexeme("ひくことの"))                   return set_info(info, T_HIKU);
            if (lexeme("ひく"))                         return set_info(info, T_HIKU);
            if (lexeme("ひきなさい"))                   return set_info(info, T_HIKUTO);
            if (lexeme("ひきざん"))                     return set_info(info, T_HIKIZAN);
            if (lexeme("ひき"))                         return set_info(info, T_HIITE);
            if (lexeme("ひかれる"))                     return set_info(info, T_HIKARERU);
            if (lexeme("ひいてみよ"))                   return set_info(info, T_HIKUTO);
            if (lexeme("ひいてみい"))                   return set_info(info, T_HIKUTO);
            if (lexeme("ひいてみぃ"))                   return set_info(info, T_HIKUTO);
            if (lexeme("ひいてみ"))                     return set_info(info, T_HIKUTO);
            if (lexeme("ひいて"))                       return set_info(info, T_HIITE);
            if (lexeme("ひいたら"))                     return set_info(info, T_HIKUTO);
            if (lexeme("ひいたときの"))                 return set_info(info, T_HIKUTO);
            if (lexeme("ひいたとき"))                   return set_info(info, T_HIKUTO);
            if (lexeme("ひいた"))                       return set_info(info, T_HIITA);
            if (lexeme("ぱーせんと"))                   return set_info(info, T_PERCENT);
            if (lexeme("ぱー"))                         return set_info(info, T_PERCENT);
            if (lexeme("ぱい"))                         return set_info(info, T_PI);
            if (lexeme("ばい"))                         return set_info(info, T_BAI);
            if (lexeme("はっ"))                         return set_info(info, T_HACHI);
            if (lexeme("はち"))                         return set_info(info, T_HACHI);
            if (lexeme("はいぱーぼりっく"))             return set_info(info, T_HYPERBOLIC);
            if (lexeme("はいぱぼりっく"))               return set_info(info, T_HYPERBOLIC);
            if (lexeme("は"))                           return set_info(info, T_HA);
            if (lexeme("のや"))                         return set_info(info, T_KANA);
            if (lexeme("のなかで"))                     return set_info(info, T_NOUCHI);
            if (lexeme("のうちで"))                     return set_info(info, T_NOUCHI);
            if (lexeme("のうち"))                       return set_info(info, T_NOUCHI);
            if (lexeme("の"))                           return set_info(info, T_NO1);
            if (lexeme("ねん"))                         return set_info(info, T_KANA);
            if (lexeme("ねいぴあすう"))                 return set_info(info, T_E);
            if (lexeme("ね"))                           return set_info(info, T_KANA);
            if (lexeme("にー"))                         return set_info(info, T_NI);
            if (lexeme("にたすことの"))                 return set_info(info, T_TASU);
            if (lexeme("にくわえることの"))             return set_info(info, T_TASU);
            if (lexeme("にかけることの"))               return set_info(info, T_KAKERU);
            if (lexeme("にぃ"))                         return set_info(info, T_NI);
            if (lexeme("に"))                           return set_info(info, T_NI);
            if (lexeme("なーんだ"))                     return set_info(info, T_IKURA);
            if (lexeme("なーに"))                       return set_info(info, T_IKURA);
            if (lexeme("なんぼになるんでしょう"))       return set_info(info, T_IKURA);
            if (lexeme("なんぼになるんだ"))             return set_info(info, T_IKURA);
            if (lexeme("なんぼになるのでしょう"))       return set_info(info, T_IKURA);
            if (lexeme("なんぼになるでしょう"))         return set_info(info, T_IKURA);
            if (lexeme("なんぼになる"))                 return set_info(info, T_IKURA);
            if (lexeme("なんぼになります"))             return set_info(info, T_IKURA);
            if (lexeme("なんぼなん"))                   return set_info(info, T_IKURA);
            if (lexeme("なんぼなの"))                   return set_info(info, T_IKURA);
            if (lexeme("なんぼでしょう"))               return set_info(info, T_IKURA);
            if (lexeme("なんぼだろう"))                 return set_info(info, T_IKURA);
            if (lexeme("なんぼ"))                       return set_info(info, T_IKURA);
            if (lexeme("なんになるんでしょう"))         return set_info(info, T_IKURA);
            if (lexeme("なんになるんだ"))               return set_info(info, T_IKURA);
            if (lexeme("なんになるのでしょう"))         return set_info(info, T_IKURA);
            if (lexeme("なんになるでしょう"))           return set_info(info, T_IKURA);
            if (lexeme("なんになる"))                   return set_info(info, T_IKURA);
            if (lexeme("なんになります"))               return set_info(info, T_IKURA);
            if (lexeme("なんなんでっ"))                 return set_info(info, T_IKURA);
            if (lexeme("なんなんです"))                 return set_info(info, T_IKURA);
            if (lexeme("なんなんでしょう"))             return set_info(info, T_IKURA);
            if (lexeme("なんなんだろう"))               return set_info(info, T_IKURA);
            if (lexeme("なんなん"))                     return set_info(info, T_IKURA);
            if (lexeme("なんなのでっ"))                 return set_info(info, T_IKURA);
            if (lexeme("なんなのです"))                 return set_info(info, T_IKURA);
            if (lexeme("なんなのでしょう"))             return set_info(info, T_IKURA);
            if (lexeme("なんなのだろう"))               return set_info(info, T_IKURA);
            if (lexeme("なんなの"))                     return set_info(info, T_IKURA);
            if (lexeme("なんでっ"))                     return set_info(info, T_IKURA);
            if (lexeme("なんです"))                     return set_info(info, T_IKURA);
            if (lexeme("なんでしょう"))                 return set_info(info, T_IKURA);
            if (lexeme("なんだろう"))                   return set_info(info, T_IKURA);
            if (lexeme("なんだ"))                       return set_info(info, T_IKURA);
            if (lexeme("なん"))                         return set_info(info, T_IKURA);
            if (lexeme("なゆた"))                       return set_info(info, T_NAYUTA);
            if (lexeme("なにになるんです"))             return set_info(info, T_IKURA);
            if (lexeme("なにになるのです"))             return set_info(info, T_IKURA);
            if (lexeme("なにになるでしょう"))           return set_info(info, T_IKURA);
            if (lexeme("なにになる"))                   return set_info(info, T_IKURA);
            if (lexeme("なにでっ"))                     return set_info(info, T_IKURA);
            if (lexeme("なにです"))                     return set_info(info, T_IKURA);
            if (lexeme("なに"))                         return set_info(info, T_IKURA);
            if (lexeme("なな"))                         return set_info(info, T_NANA);
            if (lexeme("なおせよ"))                     return set_info(info, T_SURUTO);
            if (lexeme("なおせや"))                     return set_info(info, T_SURUTO);
            if (lexeme("なおせ"))                       return set_info(info, T_SURUTO);
            if (lexeme("なおすときの"))                 return set_info(info, T_SURUTO);
            if (lexeme("なおすとき"))                   return set_info(info, T_SURUTO);
            if (lexeme("なおすと"))                     return set_info(info, T_SURUTO);
            if (lexeme("なおしなさい"))                 return set_info(info, T_SURUTO);
            if (lexeme("なおしてみろよ"))               return set_info(info, T_SURUTO);
            if (lexeme("なおしてみろや"))               return set_info(info, T_SURUTO);
            if (lexeme("なおしてみろ"))                 return set_info(info, T_SURUTO);
            if (lexeme("なおしてみよ"))                 return set_info(info, T_SURUTO);
            if (lexeme("なおしてみなさい"))             return set_info(info, T_SURUTO);
            if (lexeme("なおしてみい"))                 return set_info(info, T_SURUTO);
            if (lexeme("なおしてみぃ"))                 return set_info(info, T_SURUTO);
            if (lexeme("なおしてみ"))                   return set_info(info, T_SURUTO);
            if (lexeme("なおして"))                     return set_info(info, T_SURUTO);
            if (lexeme("なおしたら"))                   return set_info(info, T_SURUTO);
            if (lexeme("なおした"))                     return set_info(info, T_SHITA);
            if (lexeme("なあに"))                       return set_info(info, T_IKURA);
            if (lexeme("どんなかずになるんでしょう"))   return set_info(info, T_IKURA);
            if (lexeme("どんなかずになるのでしょう"))   return set_info(info, T_IKURA);
            if (lexeme("どんなかずになるでしょう"))     return set_info(info, T_IKURA);
            if (lexeme("どんなかずになる"))             return set_info(info, T_IKURA);
            if (lexeme("どんなかずになります"))         return set_info(info, T_IKURA);
            if (lexeme("どないなるんでしょう"))         return set_info(info, T_IKURA);
            if (lexeme("どないなるのでしょう"))         return set_info(info, T_IKURA);
            if (lexeme("どないなる"))                   return set_info(info, T_IKURA);
            if (lexeme("どないでっ"))                   return set_info(info, T_IKURA);
            if (lexeme("どないです"))                   return set_info(info, T_IKURA);
            if (lexeme("どないでしょう"))               return set_info(info, T_IKURA);
            if (lexeme("どない"))                       return set_info(info, T_IKURA);
            if (lexeme("どうなんでっ"))                 return set_info(info, T_IKURA);
            if (lexeme("どうなんです"))                 return set_info(info, T_IKURA);
            if (lexeme("どうなんでしょう"))             return set_info(info, T_IKURA);
            if (lexeme("どうなん"))                     return set_info(info, T_IKURA);
            if (lexeme("どうなる"))                     return set_info(info, T_IKURA);
            if (lexeme("どうなります"))                 return set_info(info, T_IKURA);
            if (lexeme("どうなのでっ"))                 return set_info(info, T_IKURA);
            if (lexeme("どうなのです"))                 return set_info(info, T_IKURA);
            if (lexeme("どうなのでしょう"))             return set_info(info, T_IKURA);
            if (lexeme("どうなの"))                     return set_info(info, T_IKURA);
            if (lexeme("どう"))                         return set_info(info, T_IKURA);
            if (lexeme("ど"))                           return set_info(info, T_DO);
            if (lexeme("とね、"))                       return set_info(info, T_TO1);
            if (lexeme("とな、"))                       return set_info(info, T_TO1);
            if (lexeme("とじかっこ"))                   return set_info(info, T_R_PAREN);
            if (lexeme("と"))                           return set_info(info, T_TO1);
            if (lexeme("でんねん"))                     return set_info(info, T_KANA);
            if (lexeme("で"))                           return set_info(info, T_DE);
            if (lexeme("てん"))                         return set_info(info, T_DOT);
            if (lexeme("って"))                         return set_info(info, T_HA);
            if (lexeme("ちょう"))                       return set_info(info, T_CHOU);
            if (lexeme("たんぜんと"))                   return set_info(info, T_TAN);
            if (lexeme("たんじぇんと"))                 return set_info(info, T_TAN);
            if (lexeme("たせよ"))                       return set_info(info, T_TASUTO);
            if (lexeme("たせや"))                       return set_info(info, T_TASUTO);
            if (lexeme("たせば"))                       return set_info(info, T_TASUTO);
            if (lexeme("たせ"))                         return set_info(info, T_TASUTO);
            if (lexeme("たすとき"))                     return set_info(info, T_TASUTO);
            if (lexeme("たすと"))                       return set_info(info, T_TASUTO);
            if (lexeme("たすことの"))                   return set_info(info, T_TASU);
            if (lexeme("たす"))                         return set_info(info, T_TASU);
            if (lexeme("たしなさい"))                   return set_info(info, T_TASUTO);
            if (lexeme("たしてみよ"))                   return set_info(info, T_TASUTO);
            if (lexeme("たしてみい"))                   return set_info(info, T_TASUTO);
            if (lexeme("たしてみぃ"))                   return set_info(info, T_TASUTO);
            if (lexeme("たしてみ"))                     return set_info(info, T_TASUTO);
            if (lexeme("たして"))                       return set_info(info, T_TASHITE);
            if (lexeme("たしたら"))                     return set_info(info, T_TASUTO);
            if (lexeme("たしたときの"))                 return set_info(info, T_TASUTO);
            if (lexeme("たしたとき"))                   return set_info(info, T_TASUTO);
            if (lexeme("たした"))                       return set_info(info, T_TASHITA);
            if (lexeme("たしざん"))                     return set_info(info, T_TASHIZAN);
            if (lexeme("たしあわせろよ"))               return set_info(info, T_TASUTO);
            if (lexeme("たしあわせろや"))               return set_info(info, T_TASUTO);
            if (lexeme("たしあわせろ"))                 return set_info(info, T_TASUTO);
            if (lexeme("たしあわせれば"))               return set_info(info, T_TASUTO);
            if (lexeme("たしあわせるとき"))             return set_info(info, T_TASUTO);
            if (lexeme("たしあわせると"))               return set_info(info, T_TASUTO);
            if (lexeme("たしあわせる"))                 return set_info(info, T_TASU);
            if (lexeme("たしあわせよ"))                 return set_info(info, T_TASUTO);
            if (lexeme("たしあわせなさい"))             return set_info(info, T_TASUTO);
            if (lexeme("たしあわせてみよ"))             return set_info(info, T_TASUTO);
            if (lexeme("たしあわせてみい"))             return set_info(info, T_TASUTO);
            if (lexeme("たしあわせてみぃ"))             return set_info(info, T_TASUTO);
            if (lexeme("たしあわせてみ"))               return set_info(info, T_TASUTO);
            if (lexeme("たしあわせて"))                 return set_info(info, T_TASHITE);
            if (lexeme("たしあわせたときの"))           return set_info(info, T_TASUTO);
            if (lexeme("たしあわせたとき"))             return set_info(info, T_TASUTO);
            if (lexeme("たしあわせた"))                 return set_info(info, T_TASHITA);
            if (lexeme("たしあわせ"))                   return set_info(info, T_TASUTO);
            if (lexeme("たしあわす"))                   return set_info(info, T_TASU);
            if (lexeme("たしあわされる"))               return set_info(info, T_TASARERU);
            if (lexeme("たし"))                         return set_info(info, T_TASHITE);
            if (lexeme("たされる"))                     return set_info(info, T_TASARERU);
            if (lexeme("たいすうかんすう"))             return set_info(info, T_LOG);
            if (lexeme("たいすう"))                     return set_info(info, T_LOG);
            if (lexeme("それの"))                       return set_info(info, T_SONO);
            if (lexeme("それがしの"))                   return set_info(info, T_SONO);
            if (lexeme("それがし"))                     return set_info(info, T_SORE);
            if (lexeme("それ"))                         return set_info(info, T_SORE);
            if (lexeme("そやつ"))                       return set_info(info, T_SORE);
            if (lexeme("そのとき"))                     return set_info(info, T_SURUTO);
            if (lexeme("そのこたえ"))                   return set_info(info, T_KOTAE);
            if (lexeme("その"))                         return set_info(info, T_SONO);
            if (lexeme("そうきょくせん"))               return set_info(info, T_HYPERBOLIC);
            if (lexeme("そいつ"))                       return set_info(info, T_SORE);
            if (lexeme("ぜんぶ"))                       return set_info(info, T_ALL);
            if (lexeme("ぜん"))                         return set_info(info, T_SEN);
            if (lexeme("ぜろ"))                         return set_info(info, T_ZERO);
            if (lexeme("ぜったいち"))                   return set_info(info, T_ZETTAICHI);
            if (lexeme("せん"))                         return set_info(info, T_SEN);
            if (lexeme("せよ"))                         return set_info(info, T_SURUTO);
            if (lexeme("せき"))                         return set_info(info, T_PROD);
            if (lexeme("せいや"))                       return set_info(info, T_SURUTO);
            if (lexeme("せいせつ"))                     return set_info(info, T_TAN);
            if (lexeme("せいげん"))                     return set_info(info, T_SIN);
            if (lexeme("せい"))                         return set_info(info, T_SEI);
            if (lexeme("すれば"))                       return set_info(info, T_SURUTO);
            if (lexeme("するときの"))                   return set_info(info, T_SURUTO);
            if (lexeme("するとき"))                     return set_info(info, T_SURUTO);
            if (lexeme("すると"))                       return set_info(info, T_SURUTO);
            if (lexeme("すべて"))                       return set_info(info, T_ALL);
            if (lexeme("じょー"))                       return set_info(info, T_JOU1);
            if (lexeme("じょざん"))                     return set_info(info, T_WARIZAN);
            if (lexeme("じょうようたいすう"))           return set_info(info, T_LOG10);
            if (lexeme("じょうよ"))                     return set_info(info, T_AMARI);
            if (lexeme("じょうざん"))                   return set_info(info, T_KAKEZAN);
            if (lexeme("じょう"))                       return set_info(info, T_JOU1);
            if (lexeme("じょ"))                         return set_info(info, T_JO);
            if (lexeme("じゅー"))                       return set_info(info, T_JUU);
            if (lexeme("じゅっ"))                       return set_info(info, T_JUU);
            if (lexeme("じゅう"))                       return set_info(info, T_JUU);
            if (lexeme("じっ"))                         return set_info(info, T_JUU);
            if (lexeme("じじょう"))                     return set_info(info, T_HEIHOU);
            if (lexeme("しーや"))                       return set_info(info, T_SURUTO);
            if (lexeme("しー"))                         return set_info(info, T_YON);
            if (lexeme("しろよ"))                       return set_info(info, T_SURUTO);
            if (lexeme("しろや"))                       return set_info(info, T_SURUTO);
            if (lexeme("しろ"))                         return set_info(info, T_SURUTO);
            if (lexeme("しらん"))                       return set_info(info, T_WAKARAN);
            if (lexeme("しらない"))                     return set_info(info, T_WAKARAN);
            if (lexeme("しょーすー"))                   return set_info(info, T_SHOUSUU);
            if (lexeme("しょーすう"))                   return set_info(info, T_SHOUSUU);
            if (lexeme("しょー"))                       return set_info(info, T_QUOT);
            if (lexeme("しょうすー"))                   return set_info(info, T_SHOUSUU);
            if (lexeme("しょうすう"))                   return set_info(info, T_SHOUSUU);
            if (lexeme("しょう"))                       return set_info(info, T_QUOT);
            if (lexeme("しなさい"))                     return set_info(info, T_SURUTO);
            if (lexeme("してよ"))                       return set_info(info, T_SHITE);
            if (lexeme("してや"))                       return set_info(info, T_SHITE);
            if (lexeme("してみろよ"))                   return set_info(info, T_SURUTO);
            if (lexeme("してみろや"))                   return set_info(info, T_SURUTO);
            if (lexeme("してみろ"))                     return set_info(info, T_SURUTO);
            if (lexeme("してみれば"))                   return set_info(info, T_SURUTO);
            if (lexeme("してみるときの"))               return set_info(info, T_SURUTO);
            if (lexeme("してみるとき"))                 return set_info(info, T_SURUTO);
            if (lexeme("してみると"))                   return set_info(info, T_SURUTO);
            if (lexeme("してみよ"))                     return set_info(info, T_SURUTO);
            if (lexeme("してみなさい"))                 return set_info(info, T_SURUTO);
            if (lexeme("してみい"))                     return set_info(info, T_SURUTO);
            if (lexeme("してみぃ"))                     return set_info(info, T_SURUTO);
            if (lexeme("してみ"))                       return set_info(info, T_SURUTO);
            if (lexeme("して"))                         return set_info(info, T_SHITE);
            if (lexeme("しち"))                         return set_info(info, T_NANA);
            if (lexeme("したら"))                       return set_info(info, T_SURUTO);
            if (lexeme("したときの"))                   return set_info(info, T_SURUTO);
            if (lexeme("したとき"))                     return set_info(info, T_SURUTO);
            if (lexeme("した"))                         return set_info(info, T_SHITA);
            if (lexeme("しぜんたいすうのてい"))         return set_info(info, T_E);
            if (lexeme("しぜんたいすう"))               return set_info(info, T_LOG);
            if (lexeme("しすうかんすう"))               return set_info(info, T_EXP);
            if (lexeme("しすう"))                       return set_info(info, T_EXP);
            if (lexeme("しいや"))                       return set_info(info, T_SURUTO);
            if (lexeme("しい"))                         return set_info(info, T_YON);
            if (lexeme("しぃ"))                         return set_info(info, T_YON);
            if (lexeme("し"))                           return set_info(info, T_YON);
            if (lexeme("さん"))                         return set_info(info, T_SAN);
            if (lexeme("さしひけよ"))                   return set_info(info, T_HIKUTO);
            if (lexeme("さしひけや"))                   return set_info(info, T_HIKUTO);
            if (lexeme("さしひけ"))                     return set_info(info, T_HIKUTO);
            if (lexeme("さしひくとき"))                 return set_info(info, T_HIKUTO);
            if (lexeme("さしひきなさい"))               return set_info(info, T_HIKUTO);
            if (lexeme("さしひき"))                     return set_info(info, T_HIITE);
            if (lexeme("さしひかれる"))                 return set_info(info, T_HIKARERU);
            if (lexeme("さしひいてみよ"))               return set_info(info, T_HIKUTO);
            if (lexeme("さしひいてみい"))               return set_info(info, T_HIKUTO);
            if (lexeme("さしひいてみぃ"))               return set_info(info, T_HIKUTO);
            if (lexeme("さしひいてみ"))                 return set_info(info, T_HIKUTO);
            if (lexeme("さしひいて"))                   return set_info(info, T_HIITE);
            if (lexeme("さしひいたら"))                 return set_info(info, T_HIKUTO);
            if (lexeme("さしひいたときの"))             return set_info(info, T_HIKUTO);
            if (lexeme("さしひいたとき"))               return set_info(info, T_HIKUTO);
            if (lexeme("さしひいた"))                   return set_info(info, T_HIITA);
            if (lexeme("さいん"))                       return set_info(info, T_SIN);
            if (lexeme("さいだいち"))                   return set_info(info, T_MAX);
            if (lexeme("さいしょーち"))                 return set_info(info, T_MIN);
            if (lexeme("さいしょうち"))                 return set_info(info, T_MIN);
            if (lexeme("さい"))                         return set_info(info, T_SAI);
            if (lexeme("さ"))                           return set_info(info, T_DIFF);
            if (lexeme("ごく"))                         return set_info(info, T_GOKU);
            if (lexeme("ごうけい"))                     return set_info(info, T_SUM);
            if (lexeme("ごうがしゃ"))                   return set_info(info, T_GOUGASHA);
            if (lexeme("ごう"))                         return set_info(info, T_GO);
            if (lexeme("ご"))                           return set_info(info, T_GO);
            if (lexeme("これの"))                       return set_info(info, T_SONO);
            if (lexeme("これ"))                         return set_info(info, T_SORE);
            if (lexeme("こやつ"))                       return set_info(info, T_SORE);
            if (lexeme("このとき"))                     return set_info(info, T_SURUTO);
            if (lexeme("こたえろよ"))                   return set_info(info, T_OSHIETE);
            if (lexeme("こたえろや"))                   return set_info(info, T_OSHIETE);
            if (lexeme("こたえろ"))                     return set_info(info, T_OSHIETE);
            if (lexeme("こたえよ"))                     return set_info(info, T_OSHIETE);
            if (lexeme("こたえなさい"))                 return set_info(info, T_OSHIETE);
            if (lexeme("こたえてみよ"))                 return set_info(info, T_OSHIETE);
            if (lexeme("こたえてみい"))                 return set_info(info, T_OSHIETE);
            if (lexeme("こたえてみぃ"))                 return set_info(info, T_OSHIETE);
            if (lexeme("こたえてみ"))                   return set_info(info, T_OSHIETE);
            if (lexeme("こたえてくれるかい"))           return set_info(info, T_OSHIETE);
            if (lexeme("こたえてくれるか"))             return set_info(info, T_OSHIETE);
            if (lexeme("こたえてくれる"))               return set_info(info, T_OSHIETE);
            if (lexeme("こたえてくれよ"))               return set_info(info, T_OSHIETE);
            if (lexeme("こたえてくれや"))               return set_info(info, T_OSHIETE);
            if (lexeme("こたえてくれ"))                 return set_info(info, T_OSHIETE);
            if (lexeme("こたえてくださいよ"))           return set_info(info, T_OSHIETE);
            if (lexeme("こたえてください"))             return set_info(info, T_OSHIETE);
            if (lexeme("こたえて"))                     return set_info(info, T_OSHIETE);
            if (lexeme("こたえ"))                       return set_info(info, T_KOTAE);
            if (lexeme("こすう"))                       return set_info(info, T_COUNT);
            if (lexeme("こさいん"))                     return set_info(info, T_COS);
            if (lexeme("こう"))                         return set_info(info, T_KOU);
            if (lexeme("こいつ"))                       return set_info(info, T_SORE);
            if (lexeme("げんざん"))                     return set_info(info, T_HIKIZAN);
            if (lexeme("けいさん"))                     return set_info(info, T_KEISAN);
            if (lexeme("けい"))                         return set_info(info, T_KEI);
            if (lexeme("くわえろよ"))                   return set_info(info, T_TASUTO);
            if (lexeme("くわえろや"))                   return set_info(info, T_TASUTO);
            if (lexeme("くわえろ"))                     return set_info(info, T_TASUTO);
            if (lexeme("くわえるときの"))               return set_info(info, T_TASUTO);
            if (lexeme("くわえるとき"))                 return set_info(info, T_TASUTO);
            if (lexeme("くわえると"))                   return set_info(info, T_TASUTO);
            if (lexeme("くわえることの"))               return set_info(info, T_TASU);
            if (lexeme("くわえる"))                     return set_info(info, T_TASU);
            if (lexeme("くわえられる"))                 return set_info(info, T_TASARERU);
            if (lexeme("くわえなさい"))                 return set_info(info, T_TASUTO);
            if (lexeme("くわえてみよ"))                 return set_info(info, T_TASUTO);
            if (lexeme("くわえてみい"))                 return set_info(info, T_TASUTO);
            if (lexeme("くわえてみぃ"))                 return set_info(info, T_TASUTO);
            if (lexeme("くわえてみ"))                   return set_info(info, T_TASUTO);
            if (lexeme("くわえて"))                     return set_info(info, T_TASHITE);
            if (lexeme("くわえたら"))                   return set_info(info, T_TASUTO);
            if (lexeme("くわえたときの"))               return set_info(info, T_TASUTO);
            if (lexeme("くわえたとき"))                 return set_info(info, T_TASUTO);
            if (lexeme("くわえた"))                     return set_info(info, T_TASHITA);
            if (lexeme("くれるや"))                     return set_info(info, T_KANA);
            if (lexeme("くれるかなあ"))                 return set_info(info, T_KANA);
            if (lexeme("くれるかなぁ"))                 return set_info(info, T_KANA);
            if (lexeme("くれるかな"))                   return set_info(info, T_KANA);
            if (lexeme("くれるかい"))                   return set_info(info, T_KANA);
            if (lexeme("くれる"))                       return set_info(info, T_KANA);
            if (lexeme("くれよー"))                     return set_info(info, T_KANA);
            if (lexeme("くれよぅ"))                     return set_info(info, T_KANA);
            if (lexeme("くれよ"))                       return set_info(info, T_KANA);
            if (lexeme("くれや"))                       return set_info(info, T_KANA);
            if (lexeme("くれ"))                         return set_info(info, T_KANA);
            if (lexeme("くださいよー"))                 return set_info(info, T_KANA);
            if (lexeme("くださいよぅ"))                 return set_info(info, T_KANA);
            if (lexeme("くださいよ"))                   return set_info(info, T_KANA);
            if (lexeme("ください"))                     return set_info(info, T_KANA);
            if (lexeme("く"))                           return set_info(info, T_KYUU);
            if (lexeme("ぎゃくよげん"))                 return set_info(info, T_ACOS);
            if (lexeme("ぎゃくたんぜんと"))             return set_info(info, T_ATAN);
            if (lexeme("ぎゃくたんじぇんと"))           return set_info(info, T_ATAN);
            if (lexeme("ぎゃくせいせつ"))               return set_info(info, T_ATAN);
            if (lexeme("ぎゃくせいげん"))               return set_info(info, T_ASIN);
            if (lexeme("ぎゃくすう"))                   return set_info(info, T_GYAKUSUU);
            if (lexeme("ぎゃくさいん"))                 return set_info(info, T_ASIN);
            if (lexeme("ぎゃくこさいん"))               return set_info(info, T_ACOS);
            if (lexeme("きゅー"))                       return set_info(info, T_KYUU);
            if (lexeme("きゅう"))                       return set_info(info, T_KYUU);
            if (lexeme("がい"))                         return set_info(info, T_GAI);
            if (lexeme("が"))                           return set_info(info, T_HA);
            if (lexeme("かー"))                         return set_info(info, T_KANA);
            if (lexeme("かん"))                         return set_info(info, T_KAN);
            if (lexeme("から"))                         return set_info(info, T_KARA);
            if (lexeme("かねー"))                       return set_info(info, T_KANA);
            if (lexeme("かねえ"))                       return set_info(info, T_KANA);
            if (lexeme("かねぇ"))                       return set_info(info, T_KANA);
            if (lexeme("かね"))                         return set_info(info, T_KANA);
            if (lexeme("かなー"))                       return set_info(info, T_KANA);
            if (lexeme("かなあ"))                       return set_info(info, T_KANA);
            if (lexeme("かなぁ"))                       return set_info(info, T_KANA);
            if (lexeme("かな"))                         return set_info(info, T_KANA);
            if (lexeme("かっこをとじる"))               return set_info(info, T_R_PAREN);
            if (lexeme("かっこをとじて"))               return set_info(info, T_R_PAREN);
            if (lexeme("かっことじる"))                 return set_info(info, T_R_PAREN);
            if (lexeme("かっことじて"))                 return set_info(info, T_R_PAREN);
            if (lexeme("かっことじ"))                   return set_info(info, T_R_PAREN);
            if (lexeme("かっこ"))                       return set_info(info, T_L_PAREN);
            if (lexeme("かず"))                         return set_info(info, T_MONO);
            if (lexeme("かさん"))                       return set_info(info, T_TASHIZAN);
            if (lexeme("かけろよ"))                     return set_info(info, T_KAKERUTO);
            if (lexeme("かけろや"))                     return set_info(info, T_KAKERUTO);
            if (lexeme("かけろ"))                       return set_info(info, T_KAKERUTO);
            if (lexeme("かければ"))                     return set_info(info, T_KAKERUTO);
            if (lexeme("かけるときの"))                 return set_info(info, T_KAKERUTO);
            if (lexeme("かけるとき"))                   return set_info(info, T_KAKERUTO);
            if (lexeme("かけると"))                     return set_info(info, T_KAKERUTO);
            if (lexeme("かけることの"))                 return set_info(info, T_KAKERU);
            if (lexeme("かける"))                       return set_info(info, T_KAKERU);
            if (lexeme("かけられる"))                   return set_info(info, T_KAKERARERU);
            if (lexeme("かけなさい"))                   return set_info(info, T_KAKERUTO);
            if (lexeme("かけてみよ"))                   return set_info(info, T_KAKERUTO);
            if (lexeme("かけてみい"))                   return set_info(info, T_KAKERUTO);
            if (lexeme("かけてみぃ"))                   return set_info(info, T_KAKERUTO);
            if (lexeme("かけてみ"))                     return set_info(info, T_KAKERUTO);
            if (lexeme("かけて"))                       return set_info(info, T_KAKETE);
            if (lexeme("かけたら"))                     return set_info(info, T_KAKERUTO);
            if (lexeme("かけたときの"))                 return set_info(info, T_KAKERUTO);
            if (lexeme("かけたとき"))                   return set_info(info, T_KAKERUTO);
            if (lexeme("かけた"))                       return set_info(info, T_KAKETA);
            if (lexeme("かけざん"))                     return set_info(info, T_KAKEZAN);
            if (lexeme("かけあわせろよ"))               return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわせろや"))               return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわせろ"))                 return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわせれば"))               return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわせるとき"))             return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわせると"))               return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわせる"))                 return set_info(info, T_KAKERU);
            if (lexeme("かけあわせよ"))                 return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわせなさい"))             return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわせてみよ"))             return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわせてみい"))             return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわせてみぃ"))             return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわせてみ"))               return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわせて"))                 return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわせたときの"))           return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわせたとき"))             return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわせた"))                 return set_info(info, T_KAKETA);
            if (lexeme("かけあわせ"))                   return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわすときの"))             return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわすとき"))               return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわす"))                   return set_info(info, T_KAKERU);
            if (lexeme("かけあわしたときの"))           return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわしたとき"))             return set_info(info, T_KAKERUTO);
            if (lexeme("かけあわされる"))               return set_info(info, T_KAKERARERU);
            if (lexeme("かけ"))                         return set_info(info, T_KAKERU);
            if (lexeme("かい"))                         return set_info(info, T_KANA);
            if (lexeme("かあ"))                         return set_info(info, T_KANA);
            if (lexeme("かぁ"))                         return set_info(info, T_KANA);
            if (lexeme("か"))                           return set_info(info, T_KA);
            if (lexeme("おねがいよ"))                   return set_info(info, T_ONEGAI);
            if (lexeme("おねがいやねん"))               return set_info(info, T_ONEGAI);
            if (lexeme("おねがいする"))                 return set_info(info, T_ONEGAI);
            if (lexeme("おねがいしますよ"))             return set_info(info, T_ONEGAI);
            if (lexeme("おねがいしますね"))             return set_info(info, T_ONEGAI);
            if (lexeme("おねがいします"))               return set_info(info, T_ONEGAI);
            if (lexeme("おねがいいたしますよ"))         return set_info(info, T_ONEGAI);
            if (lexeme("おねがいいたしますね"))         return set_info(info, T_ONEGAI);
            if (lexeme("おねがいいたします"))           return set_info(info, T_ONEGAI);
            if (lexeme("おねがい"))                     return set_info(info, T_ONEGAI);
            if (lexeme("おしえろよ"))                   return set_info(info, T_OSHIETE);
            if (lexeme("おしえろや"))                   return set_info(info, T_OSHIETE);
            if (lexeme("おしえろ"))                     return set_info(info, T_OSHIETE);
            if (lexeme("おしえよ"))                     return set_info(info, T_OSHIETE);
            if (lexeme("おしえなさい"))                 return set_info(info, T_OSHIETE);
            if (lexeme("おしえてん"))                   return set_info(info, T_OSHIETE);
            if (lexeme("おしえてよ"))                   return set_info(info, T_OSHIETE);
            if (lexeme("おしえてや"))                   return set_info(info, T_OSHIETE);
            if (lexeme("おしえてみよ"))                 return set_info(info, T_OSHIETE);
            if (lexeme("おしえてみい"))                 return set_info(info, T_OSHIETE);
            if (lexeme("おしえてみぃ"))                 return set_info(info, T_OSHIETE);
            if (lexeme("おしえてみ"))                   return set_info(info, T_OSHIETE);
            if (lexeme("おしえてほしいねん"))           return set_info(info, T_OSHIETE);
            if (lexeme("おしえてちょうだいよ"))         return set_info(info, T_OSHIETE);
            if (lexeme("おしえてちょうだい"))           return set_info(info, T_OSHIETE);
            if (lexeme("おしえてくれるかい"))           return set_info(info, T_OSHIETE);
            if (lexeme("おしえてくれるか"))             return set_info(info, T_OSHIETE);
            if (lexeme("おしえてくれる"))               return set_info(info, T_OSHIETE);
            if (lexeme("おしえてくれ"))                 return set_info(info, T_OSHIETE);
            if (lexeme("おしえてくださいよ"))           return set_info(info, T_OSHIETE);
            if (lexeme("おしえてください"))             return set_info(info, T_OSHIETE);
            if (lexeme("おしえて"))                     return set_info(info, T_OSHIETE);
            if (lexeme("おく"))                         return set_info(info, T_OKU);
            if (lexeme("えんしゅうりつ"))               return set_info(info, T_PI);
            if (lexeme("いー"))                         return set_info(info, T_E);
            if (lexeme("いっ"))                         return set_info(info, T_ICHI);
            if (lexeme("いち"))                         return set_info(info, T_ICHI);
            if (lexeme("いくらになるでしょう"))         return set_info(info, T_IKURA);
            if (lexeme("いくらになる"))                 return set_info(info, T_IKURA);
            if (lexeme("いくらになります"))             return set_info(info, T_IKURA);
            if (lexeme("いくらでっ"))                   return set_info(info, T_IKURA);
            if (lexeme("いくらです"))                   return set_info(info, T_IKURA);
            if (lexeme("いくらである"))                 return set_info(info, T_IKURA);
            if (lexeme("いくらだろう"))                 return set_info(info, T_IKURA);
            if (lexeme("いくら"))                       return set_info(info, T_IKURA);
            if (lexeme("いくつや"))                     return set_info(info, T_IKURA);
            if (lexeme("いくつになるでしょう"))         return set_info(info, T_IKURA);
            if (lexeme("いくつになる"))                 return set_info(info, T_IKURA);
            if (lexeme("いくつになります"))             return set_info(info, T_IKURA);
            if (lexeme("いくつなんでっ"))               return set_info(info, T_IKURA);
            if (lexeme("いくつなんです"))               return set_info(info, T_IKURA);
            if (lexeme("いくつなんでしょう"))           return set_info(info, T_IKURA);
            if (lexeme("いくつなん"))                   return set_info(info, T_IKURA);
            if (lexeme("いくつなのでっ"))               return set_info(info, T_IKURA);
            if (lexeme("いくつなのです"))               return set_info(info, T_IKURA);
            if (lexeme("いくつなのでしょう"))           return set_info(info, T_IKURA);
            if (lexeme("いくつなの"))                   return set_info(info, T_IKURA);
            if (lexeme("いくつである"))                 return set_info(info, T_IKURA);
            if (lexeme("いくつだろう"))                 return set_info(info, T_IKURA);
            if (lexeme("いくつ"))                       return set_info(info, T_IKURA);
            if (lexeme("あーくたんぜんと"))             return set_info(info, T_ATAN);
            if (lexeme("あーくたんじぇんと"))           return set_info(info, T_ATAN);
            if (lexeme("あーくさいん"))                 return set_info(info, T_ASIN);
            if (lexeme("あーくこさいん"))               return set_info(info, T_ACOS);
            if (lexeme("あまり"))                       return set_info(info, T_AMARI);
            if (lexeme("あそうぎ"))                     return set_info(info, T_ASOUGI);
            if (lexeme("〕"))                           return set_info(info, T_R_PAREN);
            if (lexeme("〔"))                           return set_info(info, T_L_PAREN);
            if (lexeme("】"))                           return set_info(info, T_R_PAREN);
            if (lexeme("【"))                           return set_info(info, T_L_PAREN);
            if (lexeme("』"))                           return set_info(info, T_R_PAREN);
            if (lexeme("『"))                           return set_info(info, T_L_PAREN);
            if (lexeme("」"))                           return set_info(info, T_R_PAREN);
            if (lexeme("「"))                           return set_info(info, T_L_PAREN);
            if (lexeme("》"))                           return set_info(info, T_R_PAREN);
            if (lexeme("《"))                           return set_info(info, T_L_PAREN);
            if (lexeme("〉"))                           return set_info(info, T_R_PAREN);
            if (lexeme("〈"))                           return set_info(info, T_L_PAREN);
            if (lexeme("〇"))                           return set_info(info, T_ZERO);
            if (lexeme("。"))                           return set_info(info, T_PERIOD);
            if (lexeme("、"))                           return set_info(info, T_COMMA);
            if (lexeme("☆"))                           return set_info(info, T_PERIOD);
            if (lexeme("★"))                           return set_info(info, T_PERIOD);
            if (lexeme("●"))                           return set_info(info, T_PERIOD);
            if (lexeme("◎"))                           return set_info(info, T_PERIOD);
            if (lexeme("÷"))                           return set_info(info, T_WARU);
            if (lexeme("×"))                           return set_info(info, T_KAKERU);
            if (lexeme("~"))                            return set_info(info, T_KARA);
            if (lexeme("}"))                            return set_info(info, T_R_PAREN);
            if (lexeme("{"))                            return set_info(info, T_L_PAREN);
            if (lexeme("tanh"))                         return set_info(info, T_TANH);
            if (lexeme("tan"))                          return set_info(info, T_TAN);
            if (lexeme("sinh"))                         return set_info(info, T_SINH);
            if (lexeme("sin"))                          return set_info(info, T_SIN);
            if (lexeme("log10"))                        return set_info(info, T_LOG10);
            if (lexeme("log"))                          return set_info(info, T_LOG);
            if (lexeme("ln"))                           return set_info(info, T_LOG);
            if (lexeme("fabs"))                         return set_info(info, T_ZETTAICHI);
            if (lexeme("exp"))                          return set_info(info, T_EXP);
            if (lexeme("cosh"))                         return set_info(info, T_COSH);
            if (lexeme("cos"))                          return set_info(info, T_COS);
            if (lexeme("atan"))                         return set_info(info, T_ATAN);
            if (lexeme("asin"))                         return set_info(info, T_ASIN);
            if (lexeme("arctan"))                       return set_info(info, T_ATAN);
            if (lexeme("arcsin"))                       return set_info(info, T_ASIN);
            if (lexeme("arccos"))                       return set_info(info, T_ACOS);
            if (lexeme("acos"))                         return set_info(info, T_ACOS);
            if (lexeme("abs"))                          return set_info(info, T_ZETTAICHI);
            if (lexeme("]"))                            return set_info(info, T_R_PAREN);
            if (lexeme("["))                            return set_info(info, T_L_PAREN);
            if (lexeme("?"))                            return set_info(info, T_PERIOD);
            if (lexeme("=="))                           return set_info(info, T_HA);
            if (lexeme("="))                            return set_info(info, T_HA);
            if (lexeme("/"))                            return set_info(info, T_WARU);
            if (lexeme("."))                            return set_info(info, T_PERIOD);
            if (lexeme("-"))                            return set_info(info, T_MINUS);
            if (lexeme(","))                            return set_info(info, T_COMMA);
            if (lexeme("+"))                            return set_info(info, T_TASU);
            if (lexeme("*"))                            return set_info(info, T_KAKERU);
            if (lexeme(")"))                            return set_info(info, T_R_PAREN);
            if (lexeme("("))                            return set_info(info, T_L_PAREN);
            if (lexeme("%"))                            return set_info(info, T_PERCENT);
            if (lexeme("!"))                            return set_info(info, T_PERIOD);
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

                case T_COUNT:
                case T_SUM:
                case T_DIFF:
                case T_PROD:
                case T_QUOT:
                case T_MAX:
                case T_MIN:
                case T_AVERAGE:
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
                case T_DE:
                case T_DIFF:
                case T_HA:
                case T_HEIHOU:
                case T_HIITE:
                case T_HIKIZAN:
                case T_HIKU:
                case T_HIKUTO:
                case T_IKURA:
                case T_KANA:
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
                    case T_DIFF:
                    case T_HIITA:
                    case T_HIITE:
                    case T_HIKIZAN:
                    case T_HIKU:
                    case T_HIKUTO:
                    case T_IKURA:
                    case T_KANA:
                    case T_KAKERU:
                    case T_KAKERUTO:
                    case T_KAKETA:
                    case T_KAKETE:
                    case T_KAKEZAN:
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

        // 文末の「か」を「かな」にする。
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
                    if ((it + 1)->get_token() == T_KA)
                        (it + 1)->set_token(T_KANA);
                    break;

                default:
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
