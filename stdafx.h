////////////////////////////////////////////////////////////////////////////
// stdafx.h -- Hiragana Calculator Calc-H
// This file is part of Calc-H.  See file "ReadMe.txt" and "License.txt".
////////////////////////////////////////////////////////////////////////////

#include <string>       // for std::string
#include <vector>       // for std::vector
#include <iostream>     // for std::cin, std::cout, std::cerr, std::endl
#include <fstream>      // for std::fstream
#include <sstream>      // for std::stringstream
#include <cassert>      // for assert
#include <algorithm>    // for std::sort, std::unique
#include <cmath>        // for math functions

#ifdef _WIN32
    #include <windows.h>
    #include <process.h>
#endif

////////////////////////////////////////////////////////////////////////////

#define EXTENDS_MOBJECT /*empty*/
#define MzcFootmark()
#define MzcFootmarkThis()

////////////////////////////////////////////////////////////////////////////

inline int MzcGetLParamX(LPARAM lParam)
{
    return static_cast<int>(static_cast<short>(LOWORD(lParam)));
}

inline int MzcGetLParamY(LPARAM lParam)
{
    return static_cast<int>(static_cast<short>(HIWORD(lParam)));
}

////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
    #include "WinVoice.h"

    #include "PointSizeRect.h"
    #include "Resizable.h"
#endif

#define DRR1D_USES_PMPNUMBER
#include "PmpNumber.hpp"
#include "Ndrr1D.hpp"

////////////////////////////////////////////////////////////////////////////

#define ch_logo     \
        "       +--------------------------------+\n" \
        "       |  ひらがな電卓 Calc-H ver.0.8.7 |\n" \
        "       |   by 片山博文MZ (katahiromz)   |\n" \
        "       | http://katahiromz.web.fc2.com/ |\n" \
        "       | katayama.hirofumi.mz@gmail.com |\n" \
        "       +--------------------------------+\n"

#define ch_feature  "しんきのう：こえがでるようになりました。\n"

std::string ChJustDoIt(std::string& query);
void CrTrimString(std::string& str);
void ChReplaceString(std::string& str,
                     const std::string& from,
                     const std::string& to);

////////////////////////////////////////////////////////////////////////////
