#include <string>   // for std::string
#include <vector>   // for std::vector
#include <iostream> // for std::cin, std::cout, std::cerr, std::endl
#include <fstream>  // for std::fstream
#include <sstream>  // for std::stringstream
#include <cassert>  // for assert

#ifdef _WIN32
	#include <windows.h>
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
    #include "PointSizeRect.h"
    #include "Resizable.h"
#endif

////////////////////////////////////////////////////////////////////////////

const char * const ch_logo = 
        "       +--------------------------------+\n"
        "       |  ひらがな電卓 Calc-H ver.0.4.2 |\n"
        "       |   by 片山博文MZ (katahiromz)   |\n"
        "       | http://katahiromz.web.fc2.com/ |\n"
        "       | katayama.hirofumi.mz@gmail.com |\n"
        "       +--------------------------------+\n";

const char * const ch_comment =
    "コメント：ぶんすうもけいさんできるようになりました。";

std::string ChJustDoIt(std::string& query);
void CrTrimString(std::string& str);
void ChReplaceString(std::string& str,
                     const std::string& from,
                     const std::string& to);

////////////////////////////////////////////////////////////////////////////
