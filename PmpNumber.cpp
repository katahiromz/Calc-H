/////////////////////////////////////////////////////////////////////////////
// Number --- polymorphic, multiprecision number class
// See file "ReadMe.txt" and "License.txt".
/////////////////////////////////////////////////////////////////////////////

#include "PmpNumber.hpp"

namespace pmp
{
    #ifdef PMP_INTDIV_INTEGER
        static const Number::Type s_intdiv_type = Number::INTEGER;
    #elif defined(PMP_INTDIV_FLOATING)
        static const Number::Type s_intdiv_type = Number::FLOATING;
    #elif defined(PMP_INTDIV_RATIONAL)
        static const Number::Type s_intdiv_type = Number::RATIONAL;
    #else
        static Number::Type s_intdiv_type = Number::INTEGER;
        Number::Type SetIntDivType(Number::Type type)
        {
            Number::Type old_type = s_intdiv_type;
            s_intdiv_type = type;
            return old_type;
        }
    #endif
}

/////////////////////////////////////////////////////////////////////////////

namespace pmp
{
    Number::Number(Type type, const std::string& str) :
        m_inner(shared_ptr<Inner>(new Inner(type, str)))
    {
    }

    Number::Number(const std::string& str)
    {
        if (str.find('.')  == std::string::npos &&
            str.find("e+") == std::string::npos &&
            str.find("e-") == std::string::npos)
        {
            m_inner = shared_ptr<Inner>(new Inner(INTEGER, str));
        }
        else
            m_inner = shared_ptr<Inner>(new Inner(FLOATING, str));
    }

    Number& Number::operator+=(const Number& num)
    {
        integer_type    i;
        floating_type   f;
        rational_type   r;
        switch (type())
        {
        case Number::INTEGER:
            switch (num.type())
            {
            case Number::INTEGER:
                i = get_i();
                i += num.get_i();
                assign(i);
                break;

            case Number::FLOATING:
                f = to_f();
                f += num.get_f();
                assign(f);
                break;

            case Number::RATIONAL:
                r = to_r();
                r += num.get_r();
                assign(r);
                break;

            default:
                assert(0);
                break;
            }
            break;

        case Number::FLOATING:
            switch (num.type())
            {
            case Number::INTEGER:
                f = get_f();
                f += num.to_f();
                assign(f);
                break;

            case Number::FLOATING:
                f = get_f();
                f += num.get_f();
                assign(f);
                break;

            case Number::RATIONAL:
                f = get_f();
                f += num.to_f();
                assign(f);
                break;

            default:
                assert(0);
                break;
            }
            break;

        case Number::RATIONAL:
            switch (num.type())
            {
            case Number::INTEGER:
                r = get_r();
                r += num.to_r();
                assign(r);
                break;

            case Number::FLOATING:
                f = to_f();
                f += num.get_f();
                assign(f);
                break;

            case Number::RATIONAL:
                r = get_r();
                r += num.get_r();
                assign(r);
                break;

            default:
                assert(0);
                break;
            }
            break;

        default:
            assert(0);
            break;
        }
        return *this;
    }

    Number& Number::operator-=(const Number& num)
    {
        integer_type    i;
        floating_type   f;
        rational_type   r;
        switch (type())
        {
        case Number::INTEGER:
            switch (num.type())
            {
            case Number::INTEGER:
                i = get_i();
                i -= num.get_i();
                assign(i);
                break;

            case Number::FLOATING:
                f = floating_type(get_i());
                f -= num.get_f();
                assign(f);
                break;

            case Number::RATIONAL:
                r = to_r();
                r -= num.get_r();
                assign(r);
                break;

            default:
                assert(0);
                break;
            }
            break;

        case Number::FLOATING:
            switch (num.type())
            {
            case Number::INTEGER:
                f = get_f();
                f -= num.to_f();
                assign(f);
                break;

            case Number::FLOATING:
                f = get_f();
                f -= num.get_f();
                assign(f);
                break;

            case Number::RATIONAL:
                f = get_f();
                f -= num.to_f();
                assign(f);
                break;

            default:
                assert(0);
                break;
            }
            break;

        case Number::RATIONAL:
            switch (num.type())
            {
            case Number::INTEGER:
                r = get_r();
                r -= num.to_r();
                assign(r);
                break;

            case Number::FLOATING:
                f = to_f();
                f -= num.to_f();
                assign(f);
                break;

            case Number::RATIONAL:
                r = get_r();
                r -= num.get_r();
                assign(r);
                break;

            default:
                assert(0);
                break;
            }
            break;

        default:
            assert(0);
            break;
        }
        return *this;
    }

    Number& Number::operator*=(const Number& num)
    {
        integer_type    i;
        floating_type   f;
        rational_type   r;
        switch (type())
        {
        case Number::INTEGER:
            switch (num.type())
            {
            case Number::INTEGER:
                i = get_i();
                i *= num.get_i();
                assign(i);
                break;

            case Number::FLOATING:
                f = to_f();
                f *= num.get_f();
                assign(f);
                break;

            case Number::RATIONAL:
                r = to_r();
                r *= num.get_r();
                assign(r);
                break;

            default:
                assert(0);
                break;
            }
            break;

        case Number::FLOATING:
            switch (num.type())
            {
            case Number::INTEGER:
                f = get_f();
                f *= num.to_f();
                assign(f);
                break;

            case Number::FLOATING:
                f = get_f();
                f *= num.get_f();
                assign(f);
                break;

            case Number::RATIONAL:
                f = get_f();
                f *= num.to_f();
                assign(f);
                break;

            default:
                assert(0);
                break;
            }
            break;

        case Number::RATIONAL:
            switch (num.type())
            {
            case Number::INTEGER:
                r = get_r();
                r *= num.to_r();
                assign(r);
                break;

            case Number::FLOATING:
                f = to_f();
                f *= num.get_f();
                assign(f);
                break;

            case Number::RATIONAL:
                r = get_r();
                r *= num.get_r();
                assign(r);
                break;

            default:
                assert(0);
                break;
            }
            break;

        default:
            assert(0);
            break;
        }
        return *this;
    }

    Number& Number::operator/=(const Number& num)
    {
        integer_type    i;
        floating_type   f;
        rational_type   r;
        switch (type())
        {
        case Number::INTEGER:
            switch (num.type())
            {
            case Number::INTEGER:
                if (s_intdiv_type == Number::INTEGER)
                {
                    i = get_i();
                    i /= num.get_i();
                    assign(i);
                }
                else if (s_intdiv_type == Number::FLOATING)
                {
                    if (b_mp::fmod(to_f(), num.to_f()) != 0)
                    {
                        f = to_f();
                        f /= num.to_f();
                        assign(f);
                    }
                    else
                    {
                        i = get_i();
                        i /= num.get_i();
                        assign(i);
                    }
                }
                else if (s_intdiv_type == Number::RATIONAL)
                {
                    r = rational_type(get_i(), num.get_i());
                    assign(r);
                }
                else
                {
                    assert(0);
                }
                break;

            case Number::FLOATING:
                f = to_f();
                f /= num.get_f();
                assign(f);
                break;

            case Number::RATIONAL:
                r = to_r();
                r /= num.get_r();
                assign(r);
                break;

            default:
                assert(0);
                break;
            }
            break;

        case Number::FLOATING:
            switch (num.type())
            {
            case Number::INTEGER:
                f = get_f();
                f /= num.to_f();
                assign(f);
                break;

            case Number::FLOATING:
                f = get_f();
                f /= num.get_f();
                assign(f);
                break;

            case Number::RATIONAL:
                f = get_f();
                f /= num.to_f();
                assign(f);
                break;

            default:
                assert(0);
                break;
            }
            break;

        case Number::RATIONAL:
            switch (num.type())
            {
            case Number::INTEGER:
                r = get_r();
                r /= num.to_r();
                assign(r);
                break;

            case Number::FLOATING:
                f = to_f();
                f /= num.get_f();
                assign(f);
                break;

            case Number::RATIONAL:
                r = get_r();
                r /= num.get_r();
                assign(r);
                break;

            default:
                assert(0);
                break;
            }
            break;

        default:
            assert(0);
            break;
        }
        return *this;
    }

    Number& Number::operator%=(const Number& num)
    {
        integer_type    i;
        floating_type   f;
        rational_type   r;
        switch (type())
        {
        case Number::INTEGER:
            switch (num.type())
            {
            case Number::INTEGER:
                i = get_i();
                i %= num.get_i();
                assign(i);
                break;

            case Number::FLOATING:
                f = to_f();
                f = b_mp::fmod(f, num.get_f());
                assign(f);
                break;

            default:
                assert(0);
                break;
            }
            break;

        case Number::FLOATING:
            switch (num.type())
            {
            case Number::INTEGER:
                f = get_f();
                f = b_mp::fmod(f, static_cast<floating_type>(num.get_i()));
                assign(f);
                break;

            case Number::FLOATING:
                f = get_f();
                f = b_mp::fmod(f, num.get_f());
                assign(f);
                break;

            default:
                assert(0);
                break;
            }
            break;

        default:
            assert(0);
            break;
        }
        return *this;
    }

    bool Number::is_zero() const
    {
        switch (type())
        {
        case Number::INTEGER:
            return get_i().is_zero();

        case Number::FLOATING:
            return get_f().is_zero();

        case Number::RATIONAL:
            return get_r().is_zero();

        default:
            assert(0);
            return false;
        }
    }

    std::string Number::str() const
    {
        switch (type())
        {
        case Number::INTEGER:
            return get_i().str();

        case Number::FLOATING:
            return get_f().str();

        case Number::RATIONAL:
            return get_r().str();

        default:
            assert(0);
            return "";
        }
    }

    std::string Number::str(unsigned precision) const
    {
        switch (type())
        {
        case Number::INTEGER:
            return get_i().str(precision);

        case Number::FLOATING:
            return get_f().str(precision);

        case Number::RATIONAL:
            return get_r().str();

        default:
            assert(0);
            return "";
        }
    }

    int Number::sign() const
    {
        switch (type())
        {
        case Number::INTEGER:
            return get_i().sign();

        case Number::FLOATING:
            return get_f().sign();;

        case Number::RATIONAL:
            return get_r().sign();

        default:
            assert(0);
            return 0;
        }
    }

    std::string Number::str(unsigned precision,
                            std::ios_base::fmtflags flags) const
    {
        switch (type())
        {
        case Number::INTEGER:
            return get_i().str(precision, flags);

        case Number::FLOATING:
            return get_f().str(precision, flags);

        case Number::RATIONAL:
            return get_r().str();

        default:
            assert(0);
            return "";
        }
    }

    integer_type Number::to_i() const
    {
        switch (type())
        {
        case Number::INTEGER:
            return get_i();

        case Number::FLOATING:
            return f_to_i();

        case Number::RATIONAL:
            return r_to_i();

        default:
            assert(0);
            return 0;
        }
    }

    floating_type Number::to_f() const
    {
        switch (type())
        {
        case Number::INTEGER:
            return i_to_f();

        case Number::FLOATING:
            return get_f();

        case Number::RATIONAL:
            return r_to_f();

        default:
            assert(0);
            return 0;
        }
    }

    rational_type Number::to_r() const
    {
        switch (type())
        {
        case Number::INTEGER:
            return i_to_r();

        case Number::FLOATING:
            return f_to_r();

        case Number::RATIONAL:
            return get_r();

        default:
            assert(0);
            return 0;
        }
    }

    int Number::compare(const Number& num) const
    {
        floating_type f;
        switch (type())
        {
        case Number::INTEGER:
            switch (num.type())
            {
            case Number::INTEGER:
                return get_i().compare(num.get_i());

            case Number::FLOATING:
                f = to_f();
                return f.compare(num.get_f());

            case Number::RATIONAL:
                f = to_f();
                return f.compare(num.to_f());

            default:
                assert(0);
                return 0;
            }

        case Number::FLOATING:
            switch (num.type())
            {
            case Number::INTEGER:
                f = num.to_f();
                return get_f().compare(f);

            case Number::FLOATING:
                return get_f().compare(num.get_f());

            case Number::RATIONAL:
                return get_f().compare(num.to_f());

            default:
                assert(0);
                return 0;
            }

        case Number::RATIONAL:
            switch (num.type())
            {
            case Number::INTEGER:
                f = num.to_f();
                return to_f().compare(f);

            case Number::FLOATING:
                return to_f().compare(num.get_f());

            case Number::RATIONAL:
                return to_f().compare(num.to_f());

            default:
                assert(0);
                return 0;
            }

        default:
            assert(0);
            return 0;
        }
    }

    void Number::trim()
    {
        integer_type    i;
        floating_type   f;
        rational_type   r;

        switch (type())
        {
        case Number::INTEGER:
            break;

        case Number::FLOATING:
            i = to_i();
            f = static_cast<floating_type>(i);
            if (f == get_f())
                assign(i);
            break;

        case Number::RATIONAL:
            r = get_r();
            if (b_mp::denominator(r) == 1)
                assign(b_mp::numerator(r));
            break;

        default:
            assert(0);
            break;
        }
    }

    /*static*/ integer_type f_to_i(const floating_type& f)
    {
        std::string str = f.str(0, std::ios_base::fixed);
        std::size_t i = str.find('.');
        if (i != std::string::npos)
            str = str.substr(0, i);
        return integer_type(str);
    }

    /*static*/ rational_type f_to_r(const floating_type& f)
    {
        integer_type n = f_to_i(f);
        floating_type m = f - i_to_f(n);
        if (m.is_zero())
            return rational_type(n, integer_type(1));

        integer_type k("620448401733239439360000"); // 24!
        m *= floating_type(k);
        integer_type j = f_to_i(m + 0.5);
        j += n * k + 1;
        return rational_type(j, k);
    }
} // namespace pmp

/////////////////////////////////////////////////////////////////////////////
// Non-member functions

namespace pmp
{
    Number abs(const Number& num1)
    {
        integer_type    i;
        floating_type   f;
        rational_type   r;
        switch (num1.type())
        {
        case Number::INTEGER:
            i = b_mp::abs(num1.get_i());
            return Number(i);

        case Number::FLOATING:
            f = b_mp::fabs(num1.to_f());
            return Number(f);

        case Number::RATIONAL:
            r = b_mp::abs(num1.get_r());
            return Number(r);

        default:
            assert(0);
            return 0;
        }
    }

    Number fabs(const Number& num1)
    {
        integer_type    i;
        floating_type   f;
        rational_type   r;
        switch (num1.type())
        {
        case Number::INTEGER:
            i = b_mp::abs(num1.get_i());
            return Number(i);

        case Number::FLOATING:
            f = b_mp::fabs(num1.to_f());
            return Number(f);

        case Number::RATIONAL:
            r = b_mp::abs(num1.get_r());
            return Number(r);

        default:
            assert(0);
            return 0;
        }
    }

    Number floor(const Number& num1)
    {
        floating_type f;
        switch (num1.type())
        {
        case Number::INTEGER:
            return num1;

        case Number::FLOATING:
            f = b_mp::floor(num1.get_f());
            return Number(f);

        default:
            assert(0);
            return 0;
        }
    }

    Number ceil(const Number& num1)
    {
        floating_type f;
        switch (num1.type())
        {
        case Number::INTEGER:
            return num1;

        case Number::FLOATING:
            f = b_mp::ceil(num1.get_f());
            return Number(f);

        default:
            assert(0);
            return 0;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
// unit test and example

#ifdef UNITTEST
    using namespace pmp;
    int main(void)
    {
        Number n1(100);
        Number n2(200.0);
        Number n3;

        n3 = n1;
        n3 += n2;
        assert(n3 == 300.0);
        std::cout << n3 << std::endl;
        n3 = n1;
        n3 -= n2;
        assert(n3 == -100.0);
        std::cout << n3 << std::endl;
        n3 = n1;
        n3 *= n2;
        assert(n3 == 20000.0);
        std::cout << n3 << std::endl;
        n3 = n1;
        n3 /= n2;
        assert(n3 == 0.5);
        std::cout << n3 << std::endl;
        n3 = n1;
        n3 %= n2;
        assert(n3 == 100.0);
        std::cout << n3 << std::endl;

        n3 = n1;
        n3 += 20;
        assert(n3 == 120);
        std::cout << n3 << std::endl;
        n3 = n1;
        n3 -= 20;
        assert(n3 == 80);
        std::cout << n3 << std::endl;
        n3 = n1;
        n3 *= 20;
        assert(n3 == 2000);
        std::cout << n3 << std::endl;
        n3 = n1;
        n3 /= 20;
        assert(n3 == 5);
        std::cout << n3 << std::endl;
        n3 = n1;
        n3 %= 20;
        assert(n3 == 0);
        std::cout << n3 << std::endl;

        n3 = n1;
        n3 += 20.0;
        assert(n3 == 120.0);
        std::cout << n3 << std::endl;
        n3 = n1;
        n3 -= 20.0;
        assert(n3 == 80.0);
        std::cout << n3 << std::endl;
        n3 = n1;
        n3 *= 20.0;
        assert(n3 == 2000.0);
        std::cout << n3 << std::endl;
        n3 = n1;
        n3 /= 20.0;
        assert(n3 == 5.0);
        std::cout << n3 << std::endl;
        n3 = n1;
        n3 %= 20.0;
        assert(n3 == 0.0);
        std::cout << n3 << std::endl;

        n3 = n1 + n2;
        assert(n3 == 300.0);
        std::cout << n3 << std::endl;
        n3 = n1 - n2;
        assert(n3 == -100.0);
        std::cout << n3 << std::endl;
        n3 = n1 * n2;
        assert(n3 == 20000.0);
        std::cout << n3 << std::endl;
        n3 = n1 / n2;
        assert(n3 == 0.5);
        std::cout << n3 << std::endl;
        n3 = n1 % n2;
        assert(n3 == 100.0);
        std::cout << n3 << std::endl;

        n3 = n1 + 20;
        assert(n3 == 120);
        std::cout << n3 << std::endl;
        n3 = n1 - 20;
        assert(n3 == 80);
        std::cout << n3 << std::endl;
        n3 = n1 * 20;
        assert(n3 == 2000);
        std::cout << n3 << std::endl;
        n3 = n1 / 20;
        assert(n3 == 5);
        std::cout << n3 << std::endl;
        n3 = n1 % 20;
        assert(n3 == 0);
        std::cout << n3 << std::endl;

        n3 = n1 + 20.0;
        assert(n3 == 120.0);
        std::cout << n3 << std::endl;
        n3 = n1 - 20.0;
        assert(n3 == 80.0);
        std::cout << n3 << std::endl;
        n3 = n1 * 20.0;
        assert(n3 == 2000.0);
        std::cout << n3 << std::endl;
        n3 = n1 / 20.0;
        assert(n3 == 5.0);
        std::cout << n3 << std::endl;
        n3 = n1 % 20.0;
        assert(n3 == 0.0);
        std::cout << n3 << std::endl;

        n3 = pmp::abs(Number("-1111111111111111111111111111111111111111111111111111111"));
        std::cout << n3 << std::endl;
        n3 = pmp::fabs(Number("-1111111111111111111111111111111111111111111111111111111.0"));
        std::cout << n3 << std::endl;
        n3 = pmp::sin(n1);
        std::cout << n3 << std::endl;
        n3 = pmp::sin(n2);
        std::cout << n3 << std::endl;
        n3 = pmp::cos(n1);
        std::cout << n3 << std::endl;
        n3 = pmp::cos(n2);
        std::cout << n3 << std::endl;
        n3 = pmp::tan(n1);
        std::cout << n3 << std::endl;
        n3 = pmp::tan(n2);
        std::cout << n3 << std::endl;
        n3 = pmp::pow(n1, n2);
        std::cout << n3 << std::endl;

        Number n4(1.2);
        std::cout << "n4" << std::endl;
        std::cout << n4 << std::endl;
        std::cout << n4.convert_to<int>() << std::endl;
        std::cout << n4.convert_to<__int64>() << std::endl;
        std::cout << n4.convert_to<float>() << std::endl;
        std::cout << n4.convert_to<double>() << std::endl;
        std::cout << n4.convert_to<long double>() << std::endl;
        std::cout << n4.to_i() << std::endl;
        std::cout << n4.to_f() << std::endl;
        std::cout << pmp::r_to_f(n4.to_r()) << std::endl;

        Number n5("1.000000000000000000000000000000000000000000000000000000001");
        std::cout << "n5" << std::endl;
        std::cout << n5 << std::endl;
        std::cout << n5.convert_to<int>() << std::endl;
        std::cout << n5.convert_to<__int64>() << std::endl;
        std::cout << n5.convert_to<float>() << std::endl;
        std::cout << n5.convert_to<double>() << std::endl;
        std::cout << n5.convert_to<long double>() << std::endl;
        std::cout << n5.to_i() << std::endl;
        std::cout << n5.to_f() << std::endl;
        std::cout << pmp::r_to_f(n5.to_r()) << std::endl;

        Number n6("0.000000000000000000000000000000000000000000000000000000001");
        std::cout << "n6" << std::endl;
        std::cout << n6 << std::endl;
        std::cout << n6.convert_to<int>() << std::endl;
        std::cout << n6.convert_to<__int64>() << std::endl;
        std::cout << n6.convert_to<float>() << std::endl;
        std::cout << n6.convert_to<double>() << std::endl;
        std::cout << n6.convert_to<long double>() << std::endl;
        std::cout << n6.to_i() << std::endl;
        std::cout << n6.to_f() << std::endl;
        std::cout << pmp::r_to_f(n6.to_r()) << std::endl;

        Number n7("1000000000000000000000000000000000000000000000000000000001");
        std::cout << "n7" << std::endl;
        std::cout << n7 << std::endl;
        std::cout << n7.convert_to<int>() << std::endl;
        std::cout << n7.convert_to<__int64>() << std::endl;
        std::cout << n7.convert_to<float>() << std::endl;
        std::cout << n7.convert_to<double>() << std::endl;
        std::cout << n7.convert_to<long double>() << std::endl;
        std::cout << n7.to_i() << std::endl;
        std::cout << n7.to_f() << std::endl;
        std::cout << pmp::r_to_f(n7.to_r()) << std::endl;

        Number n8("99999999999999999999999999999999999999999999999999999.01");
        std::cout << "n8" << std::endl;
        std::cout << n8 << std::endl;
        std::cout << n8.convert_to<int>() << std::endl;
        std::cout << n8.convert_to<__int64>() << std::endl;
        std::cout << n8.convert_to<float>() << std::endl;
        std::cout << n8.convert_to<double>() << std::endl;
        std::cout << n8.convert_to<long double>() << std::endl;
        std::cout << n8.to_i() << std::endl;
        std::cout << n8.to_f() << std::endl;
        std::cout << pmp::r_to_f(n8.to_r()) << std::endl;

        Number n9("100000000", "200000000000000");
        std::cout << "n9" << std::endl;
        std::cout << n9 << std::endl;
        std::cout << n9.convert_to<int>() << std::endl;
        std::cout << n9.convert_to<__int64>() << std::endl;
        std::cout << n9.convert_to<float>() << std::endl;
        std::cout << n9.convert_to<double>() << std::endl;
        std::cout << n9.convert_to<long double>() << std::endl;
        std::cout << n9.to_i() << std::endl;
        std::cout << n9.to_f() << std::endl;
        std::cout << pmp::r_to_f(n9.to_r()) << std::endl;

        Number n10("100000000", "200000");
        std::cout << "n10" << std::endl;
        std::cout << n10 << std::endl;
        std::cout << n10.convert_to<int>() << std::endl;
        std::cout << n10.convert_to<__int64>() << std::endl;
        std::cout << n10.convert_to<float>() << std::endl;
        std::cout << n10.convert_to<double>() << std::endl;
        std::cout << n10.convert_to<long double>() << std::endl;
        std::cout << n10.to_i() << std::endl;
        std::cout << n10.to_f() << std::endl;
        std::cout << pmp::r_to_f(n10.to_r()) << std::endl;

        return 0;
    }
#endif  // def UNITTEST

/////////////////////////////////////////////////////////////////////////////
