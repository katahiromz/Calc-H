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
    typedef b_mp::cpp_dec_float_50      floating_type;
    //typedef b_mp::cpp_dec_float_100     floating_type;

    // returns old value
    bool EnableIntegerDivision(bool enabled = true);

    class Number
    {
    public:
        enum Type
        {
            INTEGER, FLOATING
        };

        Number()                        : m_inner(make_shared<Inner>(0)) { }
        Number(int i)                   : m_inner(make_shared<Inner>(i)) { }
        Number(__int64 i)               : m_inner(make_shared<Inner>(i)) { }
        Number(double f)                : m_inner(make_shared<Inner>(f)) { }
        Number(long double f)           : m_inner(make_shared<Inner>(f)) { }
        Number(const integer_type& i)   : m_inner(make_shared<Inner>(i)) { }
        Number(const floating_type& f)  : m_inner(make_shared<Inner>(f)) { }
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
            *this = *this + 1;
            return *this;
        }

        Number& operator--()
        {
            *this = *this - 1;
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
        bool operator!() const { return is_zero(); }
        std::string str() const;
        std::string str(unsigned precision) const;

        Type get_type() const  { return m_inner->m_type; }

              integer_type&   get_i()        { return *m_inner.get()->m_integer;    }
        const integer_type&   get_i() const  { return *m_inner->m_integer;          }
              floating_type&  get_f()        { return *m_inner.get()->m_floating;   }
        const floating_type&  get_f() const  { return *m_inner->m_floating;         }

        integer_type    to_i() const;   // to integer
        floating_type   to_f() const;   // to floating

        void trim();

        template <typename T>
        T convert_to();

        bool is_i() const { return get_type() == INTEGER; }
        bool is_f() const { return get_type() == FLOATING; }

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

        int compare(const Number& num) const;

        void swap(Number& num)
        {
            m_inner.swap(num.m_inner);
        }

    public: // friend functions
        friend inline Number operator+(const Number& num1)
        {
            return num1;
        }

        friend inline Number operator-(const Number& num1)
        {
            switch (num1.get_type())
            {
            case pmp::Number::INTEGER:
                return Number(static_cast<integer_type>(-(*num1.m_inner->m_integer)));

            case pmp::Number::FLOATING:
                return Number(static_cast<floating_type>(-(*num1.m_inner->m_floating)));

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
            return num1.compare(num2) == 0;
        }

        friend inline bool operator!=(const Number& num1, const Number& num2)
        {
            return num1.compare(num2) != 0;
        }

        friend inline bool operator<(const Number& num1, const Number& num2)
        {
            return num1.compare(num2) < 0;
        }

        friend inline bool operator>(const Number& num1, const Number& num2)
        {
            return num1.compare(num2) > 0;
        }

        friend inline bool operator<=(const Number& num1, const Number& num2)
        {
            return num1.compare(num2) <= 0;
        }

        friend inline bool operator>=(const Number& num1, const Number& num2)
        {
            return num1.compare(num2) >= 0;
        }

    protected:  // inner
        struct Inner
        {
            Type                    m_type;
            integer_type *      m_integer;
            floating_type *     m_floating;

            Inner() : m_type(INTEGER), m_integer(new integer_type())
            {
            }

            Inner(int i) :
                m_type(INTEGER),
                m_integer(new integer_type(i)),
                m_floating(NULL)
            {
            }

            explicit Inner(__int64 i) :
                m_type(INTEGER),
                m_integer(new integer_type(i)),
                m_floating(NULL)
            {
            }

            Inner(double f) :
                m_type(FLOATING),
                m_integer(NULL),
                m_floating(new floating_type(f))
            {
            }

            explicit Inner(long double f) :
                m_type(FLOATING),
                m_integer(NULL),
                m_floating(new floating_type(f))
            {
            }

            Inner(const integer_type& i) :
                m_type(INTEGER),
                m_integer(new integer_type(i)),
                m_floating(NULL)
            {
            }

            Inner(const floating_type& f) :
                m_type(FLOATING),
                m_integer(NULL),
                m_floating(new floating_type(f))
            {
            }

            Inner(const Inner& inner) :
                m_type(inner.m_type),
                m_integer(NULL),
                m_floating(NULL)
            {
                m_integer = (inner.m_integer
                                ? new integer_type(*inner.m_integer)
                                : NULL);
                m_floating = (inner.m_floating
                                ? new floating_type(*inner.m_floating)
                                : NULL);
            }

            Inner(Type type, const std::string& str) :
                m_type(type),
                m_integer(NULL),
                m_floating(NULL)
            {
                switch (type)
                {
                case INTEGER:
                    m_integer = new integer_type(str);
                    break;

                case FLOATING:
                    m_floating = new floating_type(str);
                    break;

                default:
                    assert(0);
                    break;
                }
            }

            ~Inner()
            {
                delete m_integer;
                delete m_floating;
            }
        }; // struct Inner

        shared_ptr<Inner> m_inner;
    }; // class Number

    template <typename T>
    inline T Number::convert_to()
    {
        switch (get_type())
        {
        case INTEGER:
            return m_inner->m_integer->convert_to<T>();

        case FLOATING:
            return m_inner->m_floating->convert_to<T>();

        default:
            assert(0);
            return 0;
        }
    }
} // namespace pmp

/////////////////////////////////////////////////////////////////////////////
// Non-member functions

template <class CharT>
std::basic_ostream<CharT>&
operator<<(std::basic_ostream<CharT>& o, const pmp::Number& num)
{
    switch (num.get_type())
    {
    case pmp::Number::INTEGER:
        o << num.get_i().str();
        break;

    case pmp::Number::FLOATING:
        o << num.get_f().str();
        break;

    default:
        assert(0);
        break;
    }
    return o;
}

namespace pmp
{
    inline Number abs(const Number& num1)
    {
        integer_type i = b_mp::abs(num1.to_i());
        return Number(i);
    }

    inline Number fabs(const Number& num1)
    {
        floating_type f = b_mp::fabs(num1.to_f());
        return Number(f);
    }

    inline Number sqrt(const Number& num1)
    {
        floating_type f = b_mp::sqrt(num1.to_f());
        return Number(f);
    }

    Number floor(const Number& num1);
    Number ceil(const Number& num1);

    inline Number exp(const Number& num1)
    {
        floating_type f = b_mp::exp(num1.to_f());
        return Number(f);
    }

    inline Number log(const Number& num1)
    {
        floating_type f = b_mp::log(num1.to_f());
        return Number(f);
    }

    inline Number log10(const Number& num1)
    {
        floating_type f = b_mp::log10(num1.to_f());
        return Number(f);
    }

    inline Number cos(const Number& num1)
    {
        floating_type f = b_mp::cos(num1.to_f());
        return Number(f);
    }

    inline Number sin(const Number& num1)
    {
        floating_type f = b_mp::sin(num1.to_f());
        return Number(f);
    }

    inline Number tan(const Number& num1)
    {
        floating_type f = b_mp::tan(num1.to_f());
        return Number(f);
    }

    inline Number acos(const Number& num1)
    {
        floating_type f = b_mp::acos(num1.to_f());
        return Number(f);
    }

    inline Number asin(const Number& num1)
    {
        floating_type f = b_mp::asin(num1.to_f());
        return Number(f);
    }

    inline Number atan(const Number& num1)
    {
        floating_type f = b_mp::atan(num1.to_f());
        return Number(f);
    }

    inline Number cosh(const Number& num1)
    {
        floating_type f = b_mp::cosh(num1.to_f());
        return Number(f);
    }

    inline Number sinh(const Number& num1)
    {
        floating_type f = b_mp::sinh(num1.to_f());
        return Number(f);
    }

    inline Number tanh(const Number& num1)
    {
        floating_type f = b_mp::tanh(num1.to_f());
        return Number(f);
    }

    inline Number pow(const Number& num1, const Number& num2)
    {
        floating_type f = b_mp::pow(num1.to_f(), num2.to_f());
        return Number(f);
    }

    inline Number fmod(const Number& num1, const Number& num2)
    {
        floating_type f = b_mp::fmod(num1.to_f(), num2.to_f());
        return Number(f);
    }

    inline Number atan2(const Number& num1, const Number& num2)
    {
        floating_type f = b_mp::atan2(num1.to_f(), num2.to_f());
        return Number(f);
    }
}

namespace std
{
    inline void swap(pmp::Number& num1, pmp::Number& num2)
    {
        num1.swap(num2);
    }
} // namespace std

/////////////////////////////////////////////////////////////////////////////

#endif  // ndef PMPNUMBER_HPP_
