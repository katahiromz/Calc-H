// stdafx.h

#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <vector>

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

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "PmpNumber.hpp"
typedef pmp::Number CH_Value;

#define ch_precision    5

static const CH_Value ch_kei   = 10000000000000000;
static const CH_Value ch_chou  = 1000000000000;
static const CH_Value ch_oku   = 100000000;
static const CH_Value ch_man   = 10000;
static const CH_Value ch_sen   = 1000;
static const CH_Value ch_hyaku = 100;
static const CH_Value ch_juu   = 10;
static const CH_Value ch_ichi  = 1;
static const CH_Value ch_zero  = 0;

#include "Location.h"
#include "HParserAST.h"
