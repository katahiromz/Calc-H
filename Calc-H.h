////////////////////////////////////////////////////////////////////////////
// Calc-H.h -- Hiragana Calculator Calc-H
// This file is part of Calc-H.  See file "ReadMe.txt" and "License.txt".
////////////////////////////////////////////////////////////////////////////

#define _CRT_SECURE_NO_WARNINGS

#include <string>   // for std::string
#include <vector>   // for std::vector
#include <sstream>  // for std::stringstream
#include <cmath>    // for M_PI

#ifndef shared_ptr
    #if (__cplusplus >= 201103L)
        #include <memory>
        using std::shared_ptr;
        using std::static_pointer_cast;
        using std::dynamic_pointer_cast;
        using std::make_shared;
    #else
        #include <boost/shared_ptr.hpp>
        #include <boost/make_shared.hpp>
        using boost::shared_ptr;
        using boost::static_pointer_cast;
        using boost::dynamic_pointer_cast;
        using boost::make_shared;
    #endif
    #define shared_ptr shared_ptr
#endif

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

void ChReplaceString(std::string& str,
                     const std::string& from,
                     const std::string& to);
