#include "stdafx.h"

////////////////////////////////////////////////////////////////////////////

extern "C"
int main(int argc, char **argv)
{
    std::string query;

    // show logo
    std::cerr << ch_logo << std::endl;

    if (argc <= 1)
    {
        std::cerr << ch_comment << std::endl << std::endl;

        std::cerr << "「exit」か「おわる」でしゅうりょうできます。" << std::endl;
        std::cerr << std::endl;

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
    }
    else
    {
        std::fstream fin(argv[1], std::ios_base::in);
        if (fin.is_open())
        {
            for (;;)
            {
                if (!std::getline(fin, query))
                    break;

                std::string input = query;

                std::string result = ChJustDoIt(query);
                std::cout << "にゅうりょく：" << input << std::endl;
                std::cout << result << std::endl;

                if (result.find("しゅうりょうします") != std::string::npos)
                    break;
            }
        }
        else
        {
            std::cerr << "ファイル「" << argv[1] << "」が開けませんでした。" <<
                         std::endl;
        }
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////
