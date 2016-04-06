////////////////////////////////////////////////////////////////////////////
// stdafx.h -- Hiragana Calculator Calc-H
// This file is part of Calc-H.  See file "ReadMe.txt" and "License.txt".
////////////////////////////////////////////////////////////////////////////

#ifndef CALC_H_STDAFX_H_
#define CALC_H_STDAFX_H_

#include <string>       // for std::string
#include <vector>       // for std::vector
#include <iostream>     // for std::cin, std::cout, std::cerr, std::endl
#include <fstream>      // for std::fstream
#include <sstream>      // for std::stringstream
#include <cassert>      // for assert
#include <algorithm>    // for std::sort, std::unique
#include <cmath>        // for math functions
#include <ctime>            // for std::time
#include <iterator>         // for std::istreambuf_iterator
#include <iostream>         // for std::cerr

#ifdef _WIN32
    #include <windows.h>
    #include <process.h>
#endif

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
using boost::shared_ptr;
using boost::static_pointer_cast;
using boost::dynamic_pointer_cast;
using boost::make_shared;
#define shared_ptr shared_ptr

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

#include "Calc-H.h"

////////////////////////////////////////////////////////////////////////////

#endif  // ndef CALC_H_STDAFX_H_

////////////////////////////////////////////////////////////////////////////
