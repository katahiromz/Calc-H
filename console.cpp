////////////////////////////////////////////////////////////////////////////
// console.cpp -- Hiragana Calculator Calc-H
// This file is part of Calc-H.  See file "ReadMe.txt" and "License.txt".
////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

////////////////////////////////////////////////////////////////////////////

void ChShowHelp(void)
{
    std::cout << ch_cui_help << std::endl;
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

        std::cerr << ch_type_exit_to_quit << std::endl;

        for (;;)
        {
            std::cout << ch_do_input;

            if (!std::getline(std::cin, query))
                break;

            std::string result = ChJustDoIt(query);
            std::cout << result << std::endl;

            if (result.find(ch_quitting) != std::string::npos)
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
        std::cout << ch_error_num_of_args << std::endl;
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
                std::cout << ch_do_input << input << std::endl;
                std::cout << result << std::endl;
            }

            if (result.find(ch_quitting) != std::string::npos)
                break;
        }
    }
    else
    {
        std::cerr << ch_file_paren << argv[1] << ch_paren_cannot_open <<
                     std::endl;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////
