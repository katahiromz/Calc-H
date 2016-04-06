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
    "Calc-H-CUI --- �Ђ炪�ȓd��Calc-H (CUI)\n"
    "�g����: Calc-H-CUI.exe [\"���̓t�@�C��\" | -e \"�������񂵂�\"]\n"
    "�I�v�V����:\n"
    "   -e \"�������񂵂�\"     �����v�Z�������ʂ�\������B\n"
    "   --help                ���̃��b�Z�[�W��\������B\n"
    "   --version             �o�[�W��������\������B\n" <<
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

        std::cerr << "�uexit�v���u�����v�ł��イ��傤�ł��܂��B" <<
            std::endl << std::endl;

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
        std::cout << "�G���[�F�����̎w�肪�Ԉ���Ă��܂��B" << std::endl;
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
                std::cout << "�ɂイ��傭�F" << input << std::endl;
                std::cout << result << std::endl;
            }

            if (result.find("���イ��傤���܂�") != std::string::npos)
                break;
        }
    }
    else
    {
        std::cerr << "�t�@�C���u" << argv[1] << "�v���J���܂���ł����B" <<
                     std::endl;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////
