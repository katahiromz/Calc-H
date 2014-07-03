/////////////////////////////////////////////////////////////////////////////
// Number --- polymorphic, multiprecision number class
// See file "ReadMe.txt" and "License.txt".
/////////////////////////////////////////////////////////////////////////////

#ifndef PMPNUMBER_HPP_
#define PMPNUMBER_HPP_

#ifndef __cplusplus
    #error You must use C++ compiler to compile this source.  You lose!
#endif

#include <algorithm>    // for std::swap
#include <iostream>     // for std::basic_ostream
#include <string>       // for std::string
#include <vector>       // for std::vector
#include <cmath>        // for math functions
#include <cassert>      // for assert

/////////////////////////////////////////////////////////////////////////////
// smart pointers

#ifndef shared_ptr
    #if (__cplusplus >= 201103L)
        #include <memory>
        using std::shared_ptr;
        using std::make_shared;
    #else
        // boost::shared_ptr
        #include <Boost/shared_ptr.hpp>
        using boost::shared_ptr;

        // boost::make_shared
        #include <boost/make_shared.hpp>
        using boost::make_shared;
    #endif
#endif  // ndef shared_ptr

/////////////////////////////////////////////////////////////////////////////
// Boost.Multiprecision

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>

#define b_mp        boost::multiprecision

/////////////////////////////////////////////////////////////////////////////

namespace pmp
{
    typedef b_mp::cpp_int               integer_type;
    //typedef b_mp::cpp_dec_float_50      floating_type;
    typedef b_mp::cpp_dec_float_100     floating_type;
    typedef b_mp::cpp_rational          rational_type;
    static const unsigned s_default_precision = 100;

    inline floating_type i_to_f(const integer_type& i)
    {
        return floating_type(i);
    }

    inline rational_type i_to_r(const integer_type& i)
    {
        return rational_type(i, integer_type(1));
    }

    integer_type  f_to_i(const floating_type& f);
    rational_type f_to_r(const floating_type& f);

    inline integer_type  r_to_i(const rational_type& r)
    {
        integer_type i = b_mp::numerator(r);
        i /= b_mp::denominator(r);
        return i;
    }

    inline floating_type r_to_f(const rational_type& r)
    {
        return floating_type(r);
    }

    inline void split(const std::string& str, char sep, std::vector<std::string>& vec)
    {
        int i = 0, j = str.find_first_of(sep);
        vec.clear();
        while (j != -1)
        {
            vec.push_back(str.substr(i, j - i));
            i = j + 1;
            j = str.find_first_of(sep, i);
        }
        vec.push_back(str.substr(i, -1));
    }

    //
    // pmp::Number
    //
    class Number
    {
    public:
        enum Type
        {
            INTEGER, FLOATING, RATIONAL
#ifndef PMP_DISABLE_VECTOR
            , VECTOR
#endif
        };
        typedef std::vector<Number> vector_type;

        Number()                        : m_inner(make_shared<Inner>(0)) { }
        Number(int i)                   : m_inner(make_shared<Inner>(i)) { }
        Number(__int64 i)               : m_inner(make_shared<Inner>(i)) { }
        Number(double f)                : m_inner(make_shared<Inner>(f)) { }
        Number(long double f)           : m_inner(make_shared<Inner>(f)) { }
        Number(const integer_type& i)   : m_inner(make_shared<Inner>(i)) { }
        Number(const floating_type& f)  : m_inner(make_shared<Inner>(f)) { }
        Number(const rational_type& r)  : m_inner(make_shared<Inner>(r)) { }

        Number(int num, int denom) : m_inner(make_shared<Inner>(num, denom))
        {
        }

        Number(__int64 num, __int64 denom) :
            m_inner(make_shared<Inner>(num, denom))
        {
        }

        Number(const integer_type& num, const integer_type& denom) :
            m_inner(make_shared<Inner>(num, denom))
        {
        }

        Number(const Number& num, const Number& denom) :
            m_inner(make_shared<Inner>(num.to_i(), denom.to_i()))
        {
        }

        Number(const std::string& num, const std::string& denom) :
            m_inner(make_shared<Inner>(integer_type(num), integer_type(denom)))
        {
        }

#ifndef PMP_DISABLE_VECTOR
        Number(const vector_type& vec) :
            m_inner(make_shared<Inner>(vector_type(vec)))
        {
        }
#endif

        Number(Type type, const std::string& str);
        Number(const std::string& str);
        Number(const Number& num) : m_inner(num.m_inner) { }

        Number& operator=(const Number& num)
        {
            m_inner = num.m_inner;
            return *this;
        }

        void assign(const integer_type& i)
        {
            m_inner = make_shared<Inner>(i);
        }

        void assign(const floating_type& f)
        {
            m_inner = make_shared<Inner>(f);
        }

        void assign(const rational_type& r)
        {
            m_inner = make_shared<Inner>(r);
        }

        void assign(const std::string& str)
        {
            m_inner = make_shared<Inner>(str);
        }

        void assign(const Number& num)
        {
            m_inner = num.m_inner;
        }

        Number& operator+=(const Number& num);
        Number& operator-=(const Number& num);
        Number& operator*=(const Number& num);
        Number& operator/=(const Number& num);
        Number& operator%=(const Number& num);

        Number& operator++()
        {
            *this += 1;
            return *this;
        }

        Number& operator--()
        {
            *this -= 1;
            return *this;
        }

        Number operator++(int)
        {
            Number num(*this);
            this->operator++();
            return num;
        }

        Number operator--(int)
        {
            Number num(*this);
            this->operator--();
            return num;
        }

        bool is_zero() const;
        int sign() const;
        bool operator!() const { return is_zero(); }
        std::string str() const;
        std::string str(unsigned precision) const;
        std::string str(unsigned precision, std::ios_base::fmtflags flags) const;

        Type type() const  { return m_inner->m_type; }

              integer_type&   get_i()       { assert(is_i()); return *m_inner.get()->m_integer;  }
        const integer_type&   get_i() const { assert(is_i()); return *m_inner->m_integer;        }
              floating_type&  get_f()       { assert(is_f()); return *m_inner.get()->m_floating; }
        const floating_type&  get_f() const { assert(is_f()); return *m_inner->m_floating;       }
              rational_type&  get_r()       { assert(is_r()); return *m_inner.get()->m_rational; }
        const rational_type&  get_r() const { assert(is_r()); return *m_inner->m_rational;       }
#ifndef PMP_DISABLE_VECTOR
                 vector_type& get_v()       { assert(is_v()); return *m_inner.get()->m_vector;   }
           const vector_type& get_v() const { assert(is_v()); return *m_inner->m_vector;         }
#endif

        integer_type    to_i() const;   // to integer
        floating_type   to_f() const;   // to floating
        rational_type   to_r() const;   // to rational
#ifndef PMP_DISABLE_VECTOR
        vector_type     to_v() const;   // to vector
#endif

        floating_type   i_to_f() const    { return pmp::i_to_f(get_i()); }
        rational_type   i_to_r() const    { return pmp::i_to_r(get_i()); }
        integer_type    f_to_i() const    { return pmp::f_to_i(get_f()); }
        rational_type   f_to_r() const    { return pmp::f_to_r(get_f()); }
        integer_type    r_to_i() const    { return pmp::r_to_i(get_r()); }
        floating_type   r_to_f() const    { return pmp::r_to_f(get_r()); }

        void trim(unsigned precision = s_default_precision);

        template <typename T>
        T convert_to();

        bool is_i() const { return type() == INTEGER; }
        bool is_f() const { return type() == FLOATING; }
        bool is_r() const { return type() == RATIONAL; }
#ifndef PMP_DISABLE_VECTOR
        bool is_v() const { return type() == VECTOR; }
#endif

        int compare(int n) const
        {
            return compare(static_cast<double>(n));
        }

        int compare(__int64 n) const
        {
            return compare(static_cast<double>(n));
        }

        int compare(double d) const
        {
            return compare(static_cast<Number>(d));
        }

        int compare(long double d) const
        {
            return compare(static_cast<Number>(d));
        }

        void compare(const Number& num, bool comparisons[3]) const;
        int compare(const Number& num) const;

        void swap(Number& num)
        {
            m_inner.swap(num.m_inner);
        }

#ifndef PMP_DISABLE_VECTOR
        Number& operator[](std::size_t index)
        {
            if (type() == Number::VECTOR)
                return get_v()[index];
            else
                return *this;
        }

        const Number& operator[](std::size_t index) const
        {
            if (type() == Number::VECTOR)
                return get_v()[index];
            else
                return *this;
        }

        void push_back(const Number& num)
        {
            if (type() == Number::VECTOR)
            {
                vector_type v = get_v();
                v.push_back(num);
                assign(v);
            }
        }
#endif  // ndef PMP_DISABLE_VECTOR

        std::size_t size() const
        {
#ifndef PMP_DISABLE_VECTOR
            if (is_v())
                return get_v().size();
            else
#endif
                return 1;
        }

        bool empty() const
        {
#ifndef PMP_DISABLE_VECTOR
            if (is_v())
                return size() == 0;
            else
#endif
                return false;
        }

    public: // friend functions
        friend inline Number operator+(const Number& num1)
        {
            return num1;
        }

        friend inline Number operator-(const Number& num1)
        {
            switch (num1.type())
            {
            case Number::INTEGER:
                return Number(static_cast<integer_type>(-(*num1.m_inner->m_integer)));

            case Number::FLOATING:
                return Number(static_cast<floating_type>(-(*num1.m_inner->m_floating)));

            case Number::RATIONAL:
                return Number(static_cast<rational_type>(-(*num1.m_inner->m_rational)));

#ifndef PMP_DISABLE_VECTOR
            case Number::VECTOR:
                {
                    vector_type vec;
                    for (std::size_t i = 0; i < num1.size(); ++i)
                        vec.push_back(-num1.get_v()[i]);
                    return Number(vec);
                }
#endif

            default:
                assert(0);
                return 0;
            }
        }

        friend inline Number operator+(const Number& num1, const Number& num2)
        {
            Number num(num1);
            num += num2;
            return num;
        }

        friend inline Number operator-(const Number& num1, const Number& num2)
        {
            Number num(num1);
            num -= num2;
            return num;
        }

        friend inline Number operator*(const Number& num1, const Number& num2)
        {
            Number num(num1);
            num *= num2;
            return num;
        }

        friend inline Number operator/(const Number& num1, const Number& num2)
        {
            Number num(num1);
            num /= num2;
            return num;
        }

        friend inline Number operator%(const Number& num1, const Number& num2)
        {
            Number num(num1);
            num %= num2;
            return num;
        }

        friend inline bool operator==(const Number& num1, const Number& num2)
        {
            int comp = num1.compare(num2);
            return comp == 0;
        }

        friend inline bool operator!=(const Number& num1, const Number& num2)
        {
            int comp = num1.compare(num2);
            return comp == -1 || comp == 1;
        }

        friend inline bool operator<(const Number& num1, const Number& num2)
        {
            int comp = num1.compare(num2);
            return comp == -1;
        }

        friend inline bool operator>(const Number& num1, const Number& num2)
        {
            int comp = num1.compare(num2);
            return comp == 1;
        }

        friend inline bool operator<=(const Number& num1, const Number& num2)
        {
            int comp = num1.compare(num2);
            return comp == -1 || comp == 0;
        }

        friend inline bool operator>=(const Number& num1, const Number& num2)
        {
            int comp = num1.compare(num2);
            return comp == 0 || comp == 1;
        }

    public:
        static unsigned default_precision()
        {
            return s_default_precision;
        }

    protected:  // inner
        struct Inner
        {
            Type                m_type;
            integer_type *      m_integer;
            floating_type *     m_floating;
            rational_type *     m_rational;
#ifndef PMP_DISABLE_VECTOR
            vector_type *       m_vector;
#endif

            Inner() :
                m_type(INTEGER),
                m_integer(new integer_type()),
                m_floating(NULL),
                m_rational(NULL)
#ifndef PMP_DISABLE_VECTOR
                , m_vector(NULL)
#endif
            {
            }

            Inner(int i) :
                m_type(INTEGER),
                m_integer(new integer_type(i)),
                m_floating(NULL),
                m_rational(NULL)
#ifndef PMP_DISABLE_VECTOR
                , m_vector(NULL)
#endif
            {
            }

            explicit Inner(__int64 i) :
                m_type(INTEGER),
                m_integer(new integer_type(i)),
                m_floating(NULL),
                m_rational(NULL)
#ifndef PMP_DISABLE_VECTOR
                , m_vector(NULL)
#endif
            {
            }

            Inner(double f) :
                m_type(FLOATING),
                m_integer(NULL),
                m_floating(new floating_type(f)),
                m_rational(NULL)
#ifndef PMP_DISABLE_VECTOR
                , m_vector(NULL)
#endif
            {
            }

            Inner(int num, int denom) :
                m_type(RATIONAL),
                m_integer(NULL),
                m_floating(NULL),
                m_rational(new rational_type(num, denom))
#ifndef PMP_DISABLE_VECTOR
                , m_vector(NULL)
#endif
            {
            }

            explicit Inner(__int64 num, __int64 denom) :
                m_type(RATIONAL),
                m_integer(NULL),
                m_floating(NULL),
                m_rational(new rational_type(num, denom))
#ifndef PMP_DISABLE_VECTOR
                , m_vector(NULL)
#endif
            {
            }

            explicit Inner(long double f) :
                m_type(FLOATING),
                m_integer(NULL),
                m_floating(new floating_type(f)),
                m_rational(NULL)
#ifndef PMP_DISABLE_VECTOR
                , m_vector(NULL)
#endif
            {
            }

            Inner(const integer_type& i) :
                m_type(INTEGER),
                m_integer(new integer_type(i)),
                m_floating(NULL),
                m_rational(NULL)
#ifndef PMP_DISABLE_VECTOR
                , m_vector(NULL)
#endif
            {
            }

            Inner(const floating_type& f) :
                m_type(FLOATING),
                m_integer(NULL),
                m_floating(new floating_type(f)),
                m_rational(NULL)
#ifndef PMP_DISABLE_VECTOR
                , m_vector(NULL)
#endif
            {
            }

            Inner(const rational_type& r) :
                m_type(RATIONAL),
                m_integer(NULL),
                m_floating(NULL),
                m_rational(new rational_type(r))
#ifndef PMP_DISABLE_VECTOR
                , m_vector(NULL)
#endif
            {
            }

            Inner(const Inner& inner) :
                m_type(inner.m_type),
                m_integer(NULL),
                m_floating(NULL),
                m_rational(NULL)
#ifndef PMP_DISABLE_VECTOR
                , m_vector(NULL)
#endif
            {
                m_integer = (inner.m_integer
                              ? new integer_type(*inner.m_integer)
                              : NULL);
                m_floating = (inner.m_floating
                              ? new floating_type(*inner.m_floating)
                              : NULL);
                m_rational = (inner.m_rational
                              ? new rational_type(*inner.m_rational)
                              : NULL);
#ifndef PMP_DISABLE_VECTOR
                m_vector = (inner.m_vector
                            ? new vector_type(*inner.m_vector)
                            : NULL);
#endif
            }

            Inner(Type type, const std::string& str) :
                m_type(type),
                m_integer(NULL),
                m_floating(NULL),
                m_rational(NULL)
#ifndef PMP_DISABLE_VECTOR
                , m_vector(NULL)
#endif
            {
                switch (type)
                {
                case INTEGER:
                    m_integer = new integer_type(str);
                    break;

                case FLOATING:
                    m_floating = new floating_type(str);
                    break;

                case RATIONAL:
                    m_rational = new rational_type(str);
                    break;

#ifndef PMP_DISABLE_VECTOR
                case VECTOR:
                    {
                        std::vector<std::string> strs;
                        pmp::split(str, ',', strs);
                        m_vector = new vector_type;
                        for (std::size_t i = 0; i < strs.size(); ++i)
                            m_vector->push_back(Number(strs[i]));
                    }
                    break;
#endif

                default:
                    assert(0);
                    break;
                }
            }

            Inner(const std::string& str) :
                m_type(INTEGER),
                m_integer(NULL),
                m_floating(NULL),
                m_rational(NULL)
#ifndef PMP_DISABLE_VECTOR
                , m_vector(NULL)
#endif
            {
#ifndef PMP_DISABLE_VECTOR
                if (str.find(',') != std::string::npos)
                {
                    m_type = VECTOR;
                    std::vector<std::string> strs;
                    pmp::split(str, ',', strs);
                    m_vector = new vector_type;
                    for (std::size_t i = 0; i < strs.size(); ++i)
                        m_vector->push_back(Number(strs[i]));
                }
                else
#endif
                if (str.find('.') != std::string::npos ||
                    str.find("e+") != std::string::npos ||
                    str.find("e-") != std::string::npos)
                {
                    m_type = FLOATING;
                    m_floating = new floating_type(str);
                }
                else if (str.find('/') != std::string::npos)
                {
                    m_type = RATIONAL;
                    m_rational = new rational_type(str);
                }
                else
                {
                    m_integer = new integer_type(str);
                }
            }

            Inner(const integer_type& num, const integer_type& denom) :
                m_type(RATIONAL),
                m_integer(NULL),
                m_floating(NULL),
                m_rational(new rational_type(num, denom))
#ifndef PMP_DISABLE_VECTOR
                , m_vector(NULL)
#endif
            {
            }

#ifndef PMP_DISABLE_VECTOR
            Inner(const vector_type& vec) :
                m_type(VECTOR),
                m_integer(NULL),
                m_floating(NULL),
                m_rational(NULL),
                m_vector(new vector_type(vec))
            {
            }
#endif

            ~Inner()
            {
                delete m_integer;
                delete m_floating;
                delete m_rational;
#ifndef PMP_DISABLE_VECTOR
                delete m_vector;
#endif
            }
        }; // struct Inner

        shared_ptr<Inner> m_inner;
    }; // class Number

    #ifdef PMP_INTDIV_INTEGER
    #elif defined(PMP_INTDIV_FLOATING)
    #elif defined(PMP_INTDIV_RATIONAL)
    #else
        Number::Type SetIntDivType(Number::Type type);
    #endif

    template <typename T>
    inline T Number::convert_to()
    {
        switch (type())
        {
        case INTEGER:
            return m_inner->m_integer->convert_to<T>();

        case FLOATING:
            return m_inner->m_floating->convert_to<T>();

        case RATIONAL:
            return m_inner->m_rational->convert_to<T>();

        default:
            assert(0);
            return 0;
        }
    }

    typedef Number::vector_type vector_type;
} // namespace pmp

/////////////////////////////////////////////////////////////////////////////
// Non-member functions

template <class CharT>
std::basic_ostream<CharT>&
operator<<(std::basic_ostream<CharT>& o, const pmp::Number& num)
{
    switch (num.type())
    {
    case pmp::Number::INTEGER:
        o << num.get_i().str();
        break;

    case pmp::Number::FLOATING:
        o << num.get_f().str();
        break;

    case pmp::Number::RATIONAL:
        o << num.get_r().str();
        break;

#ifndef PMP_DISABLE_VECTOR
    case pmp::Number::VECTOR:
        if (!num.empty())
        {
            o << num.get_v()[0];
            for (std::size_t i = 1; i < num.size(); ++i)
            {
                o << ',';
                o << num.get_v()[i];
            }
        }
        break;
#endif  // ndef PMP_DISABLE_VECTOR

    default:
        assert(0);
        break;
    }
    return o;
}

namespace pmp
{
    Number abs(const Number& num1);
    Number fabs(const Number& num1);

    inline Number sqrt(const Number& num1)
    {
#ifndef PMP_DISABLE_VECTOR
        if (num1.is_v())
        {
            vector_type vec;
            for (std::size_t i = 0; i < num1.size(); ++i)
                vec.push_back(pmp::sqrt(num1[i]));
            return Number(vec);
        }
#endif
        floating_type f = b_mp::sqrt(num1.to_f());
        return Number(f);
    }

    Number floor(const Number& num1);
    Number ceil(const Number& num1);

    inline Number exp(const Number& num1)
    {
#ifndef PMP_DISABLE_VECTOR
        if (num1.is_v())
        {
            vector_type vec;
            for (std::size_t i = 0; i < num1.size(); ++i)
                vec.push_back(pmp::exp(num1[i]));
            return Number(vec);
        }
#endif
        floating_type f = b_mp::exp(num1.to_f());
        return Number(f);
    }

    inline Number log(const Number& num1)
    {
#ifndef PMP_DISABLE_VECTOR
        if (num1.is_v())
        {
            vector_type vec;
            for (std::size_t i = 0; i < num1.size(); ++i)
                vec.push_back(pmp::log(num1[i]));
            return Number(vec);
        }
#endif
        floating_type f = b_mp::log(num1.to_f());
        return Number(f);
    }

    inline Number log10(const Number& num1)
    {
#ifndef PMP_DISABLE_VECTOR
        if (num1.is_v())
        {
            vector_type vec;
            for (std::size_t i = 0; i < num1.size(); ++i)
                vec.push_back(pmp::log10(num1[i]));
            return Number(vec);
        }
#endif
        floating_type f = b_mp::log10(num1.to_f());
        return Number(f);
    }

    inline Number cos(const Number& num1)
    {
#ifndef PMP_DISABLE_VECTOR
        if (num1.is_v())
        {
            vector_type vec;
            for (std::size_t i = 0; i < num1.size(); ++i)
                vec.push_back(pmp::cos(num1[i]));
            return Number(vec);
        }
#endif
        floating_type f = b_mp::cos(num1.to_f());
        return Number(f);
    }

    inline Number sin(const Number& num1)
    {
#ifndef PMP_DISABLE_VECTOR
        if (num1.is_v())
        {
            vector_type vec;
            for (std::size_t i = 0; i < num1.size(); ++i)
                vec.push_back(pmp::sin(num1[i]));
            return Number(vec);
        }
#endif
        floating_type f = b_mp::sin(num1.to_f());
        return Number(f);
    }

    inline Number tan(const Number& num1)
    {
#ifndef PMP_DISABLE_VECTOR
        if (num1.is_v())
        {
            vector_type vec;
            for (std::size_t i = 0; i < num1.size(); ++i)
                vec.push_back(pmp::tan(num1[i]));
            return Number(vec);
        }
#endif
        floating_type f = b_mp::tan(num1.to_f());
        return Number(f);
    }

    inline Number acos(const Number& num1)
    {
#ifndef PMP_DISABLE_VECTOR
        if (num1.is_v())
        {
            vector_type vec;
            for (std::size_t i = 0; i < num1.size(); ++i)
                vec.push_back(pmp::acos(num1[i]));
            return Number(vec);
        }
#endif
        floating_type f = b_mp::acos(num1.to_f());
        return Number(f);
    }

    inline Number asin(const Number& num1)
    {
#ifndef PMP_DISABLE_VECTOR
        if (num1.is_v())
        {
            vector_type vec;
            for (std::size_t i = 0; i < num1.size(); ++i)
                vec.push_back(pmp::asin(num1[i]));
            return Number(vec);
        }
#endif
        floating_type f = b_mp::asin(num1.to_f());
        return Number(f);
    }

    inline Number atan(const Number& num1)
    {
#ifndef PMP_DISABLE_VECTOR
        if (num1.is_v())
        {
            vector_type vec;
            for (std::size_t i = 0; i < num1.size(); ++i)
                vec.push_back(pmp::atan(num1[i]));
            return Number(vec);
        }
#endif
        floating_type f = b_mp::atan(num1.to_f());
        return Number(f);
    }

    inline Number cosh(const Number& num1)
    {
#ifndef PMP_DISABLE_VECTOR
        if (num1.is_v())
        {
            vector_type vec;
            for (std::size_t i = 0; i < num1.size(); ++i)
                vec.push_back(pmp::cosh(num1[i]));
            return Number(vec);
        }
#endif
        floating_type f = b_mp::cosh(num1.to_f());
        return Number(f);
    }

    inline Number sinh(const Number& num1)
    {
#ifndef PMP_DISABLE_VECTOR
        if (num1.is_v())
        {
            vector_type vec;
            for (std::size_t i = 0; i < num1.size(); ++i)
                vec.push_back(pmp::sinh(num1[i]));
            return Number(vec);
        }
#endif
        floating_type f = b_mp::sinh(num1.to_f());
        return Number(f);
    }

    inline Number tanh(const Number& num1)
    {
#ifndef PMP_DISABLE_VECTOR
        if (num1.is_v())
        {
            vector_type vec;
            for (std::size_t i = 0; i < num1.size(); ++i)
                vec.push_back(pmp::tanh(num1[i]));
            return Number(vec);
        }
#endif
        floating_type f = b_mp::tanh(num1.to_f());
        return Number(f);
    }

    inline Number pow(const Number& num1, const Number& num2)
    {
#ifndef PMP_DISABLE_VECTOR
        assert(num2.type() != Number::VECTOR);
        if (num1.is_v())
        {
            vector_type vec;
            for (std::size_t i = 0; i < num1.size(); ++i)
                vec.push_back(pmp::pow(num1[i], num2));
            return Number(vec);
        }
#endif
        floating_type f = b_mp::pow(num1.to_f(), num2.to_f());
        return Number(f);
    }

    inline Number fmod(const Number& num1, const Number& num2)
    {
#ifndef PMP_DISABLE_VECTOR
        assert(num2.type() != Number::VECTOR);
        if (num1.is_v())
        {
            vector_type vec;
            for (std::size_t i = 0; i < num1.size(); ++i)
                vec.push_back(pmp::fmod(num1[i], num2));
            return Number(vec);
        }
#endif
        floating_type f = b_mp::fmod(num1.to_f(), num2.to_f());
        return Number(f);
    }

    inline Number atan2(const Number& num1, const Number& num2)
    {
#ifndef PMP_DISABLE_VECTOR
        assert(num2.type() != Number::VECTOR);
        if (num1.is_v())
        {
            vector_type vec;
            for (std::size_t i = 0; i < num1.size(); ++i)
                vec.push_back(pmp::atan2(num1[i], num2));
            return Number(vec);
        }
#endif
        floating_type f = b_mp::atan2(num1.to_f(), num2.to_f());
        return Number(f);
    }

    inline Number numerator(const Number& num1)
    {
#ifndef PMP_DISABLE_VECTOR
        if (num1.is_v())
        {
            vector_type vec;
            for (std::size_t i = 0; i < num1.size(); ++i)
                vec.push_back(pmp::numerator(num1[i]));
            return Number(vec);
        }
#endif
        if (num1.type() == Number::RATIONAL)
            return b_mp::numerator(num1.get_r());
        else
            return num1;
    }

    inline Number denominator(const Number& num1)
    {
#ifndef PMP_DISABLE_VECTOR
        if (num1.is_v())
        {
            vector_type vec;
            for (std::size_t i = 0; i < num1.size(); ++i)
                vec.push_back(pmp::denominator(num1[i]));
            return Number(vec);
        }
#endif
        if (num1.type() == Number::RATIONAL)
            return b_mp::denominator(num1.get_r());
        else
            return 1;
    }

    inline Number count(const Number& num1)
    {
#ifndef PMP_DISABLE_VECTOR
        Number n = 0;
        if (num1.is_v())
        {
            for (std::size_t i = 0; i < num1.size(); ++i)
            {
                n += pmp::count(num1[i]);
            }
            return n;
        }
#endif
        return 1;
    }

    inline Number sum(const Number& num1)
    {
#ifndef PMP_DISABLE_VECTOR
        Number n(0);
        if (num1.is_v())
        {
            for (std::size_t i = 0; i < num1.size(); ++i)
            {
                n += pmp::sum(num1[i]);
            }
            return n;
        }
#endif
        return num1;
    }

    // product
    inline Number prod(const Number& num1)
    {
#ifndef PMP_DISABLE_VECTOR
        Number n(1);
        if (num1.is_v())
        {
            for (std::size_t i = 0; i < num1.size(); ++i)
            {
                n *= pmp::prod(num1[i]);
            }
            return n;
        }
#endif
        return num1;
    }

    // difference
    inline Number diff(const Number& num1)
    {
#ifndef PMP_DISABLE_VECTOR
        Number n;
        if (num1.is_v())
        {
            n = num1[0];
            for (std::size_t i = 1; i < num1.size(); ++i)
            {
                n -= num1[i];
            }
            return n;
        }
#endif
        return num1;
    }

    // quotient
    inline Number quot(const Number& num1)
    {
#ifndef PMP_DISABLE_VECTOR
        Number n;
        if (num1.is_v())
        {
            n = num1[0];
            for (std::size_t i = 1; i < num1.size(); ++i)
            {
                n /= num1[i];
            }
            return n;
        }
#endif
        return num1;
    }

    inline Number average(const Number& num1)
    {
        Number n = pmp::sum(num1);
        n += 0.0;
        n /= pmp::count(num1);
        return n;
    }

    #undef max
    inline Number max(const Number& num1)
    {
#ifndef PMP_DISABLE_VECTOR
        if (num1.is_v() && !num1.empty())
        {
            Number m = num1[0];
            for (std::size_t i = 1; i < num1.size(); ++i)
            {
                if (m < num1[i])
                    m = num1[i];
            }
            return m;
        }
#endif
        return num1;
    }

    #undef min
    inline Number min(const Number& num1)
    {
#ifndef PMP_DISABLE_VECTOR
        if (num1.is_v() && !num1.empty())
        {
            Number m = num1[0];
            for (std::size_t i = 1; i < num1.size(); ++i)
            {
                if (m > num1[i])
                    m = num1[i];
            }
            return m;
        }
#endif
        return num1;
    }
} // namespace pmp

namespace std
{
    inline void swap(pmp::Number& num1, pmp::Number& num2)
    {
        num1.swap(num2);
    }
} // namespace std

/////////////////////////////////////////////////////////////////////////////

#endif  // ndef PMPNUMBER_HPP_
