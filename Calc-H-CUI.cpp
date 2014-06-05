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

        std::cerr << "�uexit�v���u�����v�ł��イ��傤�ł��܂��B" << std::endl;
        std::cerr << std::endl;

        for (;;)
        {
            std::cout << "�ɂイ��傭�F";

            if (!std::getline(std::cin, query))
                break;

            std::string result = ChJustDoIt(query);
            std::cout << result << std::endl;

            if (result.find("���イ��傤���܂�") != std::string::npos)
                break;
        }
    }
    else
    {
        std::fstream fin(argv[1], std::ios_base::in);
        for (;;)
        {
            if (!std::getline(fin, query))
                break;

            std::string result = ChJustDoIt(query);
            std::cout << result << std::endl;

            if (result.find("���イ��傤���܂�") != std::string::npos)
                break;
        }
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////
