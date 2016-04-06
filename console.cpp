////////////////////////////////////////////////////////////////////////////
// console.cpp -- Hiragana Calculator Calc-H
// This file is part of Calc-H.  See file "ReadMe.txt" and "License.txt".
////////////////////////////////////////////////////////////////////////////
// (Japanese, Shift_JIS)
#include "stdafx.h"

////////////////////////////////////////////////////////////////////////////

void ChShowHelp(void)
{
    std::cout <<
    "Calc-H-CUI --- ひらがな電卓Calc-H (CUI)\n"
    "使い方: Calc-H-CUI.exe [\"入力ファイル\" | -e \"けいさんしき\"]\n"
    "オプション:\n"
    "   -e \"けいさんしき\"     式を計算した結果を表示する。\n"
    "   --help                このメッセージを表示する。\n"
    "   --version             バージョン情報を表示する。\n" <<
    std::endl;
}

void ChShowVersion(void)
{
    // show logo
    std::cout << ch_logo << std::endl;
}

extern "C"
int main(int argc, char **argv)
{
    std::string query;

    // interactive mode
    if (argc <= 1)
    {
        ChShowVersion();

        std::cerr << "「exit」か「おわる」でしゅうりょうできます。" <<
            std::endl << std::endl;

        for (;;)
        {
            std::cout << "にゅうりょく：";

            if (!std::getline(std::cin, query))
                break;

            std::string result = ChJustDoIt(query);
            std::cout << result << std::endl;

            if (result.find("しゅうりょうします") != std::string::npos)
                break;
        }
        return 0;
    }

    // one-liner mode
    if (argc == 3 &&
        (strcmp(argv[1], "-e") == 0 || strcmp(argv[1], "-E") == 0))
    {
        query = argv[2];
        std::string result = ChJustDoIt(query);
        std::cout << result << std::endl;
        return 0;
    }

    // show help
    if (argc == 2 &&
        (strcmp(argv[1], "--help") == 0 ||
         strcmp(argv[1], "--HELP") == 0 ||
         strcmp(argv[1], "/?") == 0))
    {
        ChShowHelp();
        return 0;
    }

    // show version
    if (argc == 2 &&
        (strcmp(argv[1], "--version") == 0 ||
         strcmp(argv[1], "--VERSION") == 0 ||
         strcmp(argv[1], "-V") == 0))
    {
        ChShowVersion();
        return 0;
    }

    if (argc != 2)
    {
        std::cout << "エラー：引数の指定が間違っています。" << std::endl;
        ChShowHelp();
        return 1;
    }

    // file input mode
    std::fstream fin(argv[1], std::ios_base::in);
    if (fin.is_open())
    {
        for (;;)
        {
            if (!std::getline(fin, query))
                break;

            std::string input = query;
            std::string result = ChJustDoIt(query);
            if (!result.empty())
            {
                std::cout << "にゅうりょく：" << input << std::endl;
                std::cout << result << std::endl;
            }

            if (result.find("しゅうりょうします") != std::string::npos)
                break;
        }
    }
    else
    {
        std::cerr << "ファイル「" << argv[1] << "」が開けませんでした。" <<
                     std::endl;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////
