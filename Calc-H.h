////////////////////////////////////////////////////////////////////////////
// Calc-H.h -- Hiragana Calculator Calc-H
// This file is part of Calc-H.  See file "ReadMe.txt" and "License.txt".
////////////////////////////////////////////////////////////////////////////

#ifndef CALC_H_H_
#define CALC_H_H_

#define _CRT_SECURE_NO_WARNINGS

#ifdef USE_GTKMM
    #include <gtkmm.h>
    #include <gtkmm/main.h>
#else
    #include <windows.h>
    #include <process.h>
#endif

#include <string>       // for std::string
#include <vector>       // for std::vector
#include <sstream>      // for std::stringstream
#include <cmath>        // for math functions and M_PI

#include <iostream>     // for std::cin, std::cout, std::cerr, std::endl
#include <fstream>      // for std::fstream
#include <cassert>      // for assert
#include <algorithm>    // for std::sort, std::unique
#include <ctime>            // for std::time
#include <iterator>         // for std::istreambuf_iterator
#include <iostream>         // for std::cerr

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
using boost::shared_ptr;
using boost::static_pointer_cast;
using boost::dynamic_pointer_cast;
using boost::make_shared;
#define shared_ptr shared_ptr

#include <cstdlib>
#include <cstdio>
#include <cstring>

#define DRR1D_USES_PMPNUMBER
#include "PmpNumber.hpp"
#include "Ndrr1D.hpp"
typedef pmp::Number CH_Value;

#define ch_precision    15

static const CH_Value ch_zero  =          0;
static const CH_Value ch_ichi  =          1;
static const CH_Value ch_juu   =          10;
static const CH_Value ch_hyaku =          100;
static const CH_Value ch_sen   =          1000;
static const CH_Value ch_man   =          10000;
static const CH_Value ch_oku   =          100000000;
static const CH_Value ch_chou  =          1000000000000;
static const CH_Value ch_kei   =          10000000000000000;
static const CH_Value ch_gai            ("100000000000000000000");
static const CH_Value ch_jo             ("1000000000000000000000000");
static const CH_Value ch_jou            ("10000000000000000000000000000");
static const CH_Value ch_kou            ("100000000000000000000000000000000");
static const CH_Value ch_kan            ("1000000000000000000000000000000000000");
static const CH_Value ch_sei            ("10000000000000000000000000000000000000000");
static const CH_Value ch_sai            ("100000000000000000000000000000000000000000000");
static const CH_Value ch_goku           ("1000000000000000000000000000000000000000000000000");
static const CH_Value ch_gougasha       ("10000000000000000000000000000000000000000000000000000");
static const CH_Value ch_asougi         ("100000000000000000000000000000000000000000000000000000000");
static const CH_Value ch_nayuta         ("1000000000000000000000000000000000000000000000000000000000000");
static const CH_Value ch_fukashigi      ("10000000000000000000000000000000000000000000000000000000000000000");
static const CH_Value ch_muryoutaisuu   ("100000000000000000000000000000000000000000000000000000000000000000000");

#include "Location.h"
#include "HParserAST.h"

std::string ChJustDoIt(std::string& query);
void CrTrimString(std::string& str);
void ChReplaceString(std::string& str,
                     const std::string& from,
                     const std::string& to);
void ChFixResultForDisplay(std::string& result);
void ChFixResultForVoice(std::string& result);

extern std::string ch_cui_help;
extern std::string ch_logo;
extern std::string ch_feature;
extern std::string ch_type_exit_to_quit;
extern std::string ch_do_input;
extern std::string ch_quitting;
extern std::string ch_error_num_of_args;
extern std::string ch_file_paren;
extern std::string ch_paren_cannot_open;
extern std::string ch_error_colon;
extern std::string ch_not_supported_yet;

#ifndef USE_GTKMM
    #define EXTENDS_MOBJECT /*empty*/
    #define MzcFootmark()
    #define MzcFootmarkThis()

    inline int MzcGetLParamX(LPARAM lParam) {
        return static_cast<int>(static_cast<short>(LOWORD(lParam)));
    }

    inline int MzcGetLParamY(LPARAM lParam) {
        return static_cast<int>(static_cast<short>(HIWORD(lParam)));
    }

    #include "WinVoice.h"
    #include "PointSizeRect.h"
    #include "Resizable.h"
#endif

#endif  // ndef CALC_H_H_
