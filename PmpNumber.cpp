/////////////////////////////////////////////////////////////////////////////
// Number --- polymorphic, multiprecision number class
// See file "ReadMe.txt" and "License.txt".
/////////////////////////////////////////////////////////////////////////////

#include "PmpNumber.hpp"

namespace pmp
{
    static bool s_integer_division_enabled = false;
    bool EnableIntegerDivision(bool enabled/* = true*/)
    {
        bool was_enabled = s_integer_division_enabled;
        s_integer_division_enabled = enabled;
        return was_enabled;
    }
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
        switch(get_type())
        {
        case Number::INTEGER:
            switch(num.get_type())
            {
            case Number::INTEGER:
                i = get_i();
                i += num.get_i();
                assign(i);
                break;

            case Number::FLOATING:
                f = floating_type(get_i());
                f += num.get_f();
                assign(f);
                break;

            default:
                assert(0);
                break;
            }
            break;

        case Number::FLOATING:
            switch(num.get_type())
            {
            case Number::INTEGER:
                f = get_f();
                f += static_cast<floating_type>(num.get_i());
                assign(f);
                break;

            case Number::FLOATING:
                f = get_f();
                f += num.get_f();
                assign(f);
                break;

            default:
                assert(0);
                break;
            }
            break;
        }
        return *this;
    }

    Number& Number::operator-=(const Number& num)
    {
        integer_type    i;
        floating_type   f;
        switch(get_type())
        {
        case Number::INTEGER:
            switch(num.get_type())
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

            default:
                assert(0);
                break;
            }
            break;

        case Number::FLOATING:
            switch(num.get_type())
            {
            case Number::INTEGER:
                f = get_f();
                f -= static_cast<floating_type>(num.get_i());
                assign(f);
                break;

            case Number::FLOATING:
                f = get_f();
                f -= num.get_f();
                assign(f);
                break;

            default:
                assert(0);
                break;
            }
            break;
        }
        return *this;
    }

    Number& Number::operator*=(const Number& num)
    {
        integer_type    i;
        floating_type   f;
        switch(get_type())
        {
        case Number::INTEGER:
            switch(num.get_type())
            {
            case Number::INTEGER:
                i = get_i();
                i *= num.get_i();
                assign(i);
                break;

            case Number::FLOATING:
                f = floating_type(get_i());
                f *= num.get_f();
                assign(f);
                break;

            default:
                assert(0);
                break;
            }
            break;

        case Number::FLOATING:
            switch(num.get_type())
            {
            case Number::INTEGER:
                f = get_f();
                f *= static_cast<floating_type>(num.get_i());
                assign(f);
                break;

            case Number::FLOATING:
                f = get_f();
                f *= num.get_f();
                assign(f);
                break;

            default:
                assert(0);
                break;
            }
            break;
        }
        return *this;
    }

    Number& Number::operator/=(const Number& num)
    {
        integer_type    i;
        floating_type   f;
        switch(get_type())
        {
        case Number::INTEGER:
            switch(num.get_type())
            {
            case Number::INTEGER:
                if (s_integer_division_enabled)
                {
                    i = get_i();
                    i /= num.get_i();
                    assign(i);
                }
                else
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
                break;

            case Number::FLOATING:
                f = static_cast<floating_type>(get_i());
                f /= num.get_f();
                assign(f);
                break;

            default:
                assert(0);
                break;
            }
            break;

        case Number::FLOATING:
            switch(num.get_type())
            {
            case Number::INTEGER:
                f = get_f();
                f /= static_cast<floating_type>(num.get_i());
                assign(f);
                break;

            case Number::FLOATING:
                f = get_f();
                f /= num.get_f();
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

    Number& Number::operator%=(const Number& num)
    {
        integer_type    i;
        floating_type   f;
        switch(get_type())
        {
        case Number::INTEGER:
            switch(num.get_type())
            {
            case Number::INTEGER:
                i = get_i();
                i %= num.get_i();
                assign(i);
                break;

            case Number::FLOATING:
                f = floating_type(get_i());
                f = b_mp::fmod(f, num.get_f());
                assign(f);
                break;

            default:
                assert(0);
                break;
            }
            break;

        case Number::FLOATING:
            switch(num.get_type())
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
        switch (get_type())
        {
        case Number::INTEGER:
            return get_i().is_zero();

        case Number::FLOATING:
            return get_f().is_zero();

        default:
            assert(0);
            return false;
        }
    }

    std::string Number::str() const
    {
        switch (get_type())
        {
        case Number::INTEGER:
            return get_i().str();

        case Number::FLOATING:
            return get_f().str();

        default:
            assert(0);
            return "";
        }
    }

    std::string Number::str(unsigned precision) const
    {
        switch (get_type())
        {
        case Number::INTEGER:
            return get_i().str(precision);

        case Number::FLOATING:
            return get_f().str(precision);

        default:
            assert(0);
            return "";
        }
    }

    integer_type Number::to_i() const
    {
		std::string str;
		std::size_t i;
        switch (get_type())
        {
        case Number::INTEGER:
            return get_i();

        case Number::FLOATING:
			str = get_f().str();
			i = str.find('.');
			if (i != std::string::npos)
				return integer_type(str.substr(0, i));
			else
				return integer_type(str);

        default:
            assert(0);
            return 0;
        }
    }

    floating_type Number::to_f() const
    {
        switch (get_type())
        {
        case Number::INTEGER:
            return floating_type(get_i());

        case Number::FLOATING:
            return get_f();

        default:
            assert(0);
            return 0;
        }
    }

    int Number::compare(const Number& num) const
    {
        floating_type f;
        switch (get_type())
        {
        case Number::INTEGER:
            switch (num.get_type())
            {
            case Number::INTEGER:
                return get_i().compare(num.get_i());

            case Number::FLOATING:
                f = static_cast<floating_type>(get_i());
                return f.compare(num.get_f());

            default:
                assert(0);
                return 0;
            }

        case Number::FLOATING:
            switch (num.get_type())
            {
            case Number::INTEGER:
                f = static_cast<floating_type>(num.get_i());
                return get_f().compare(f);

            case Number::FLOATING:
                return get_f().compare(num.get_f());

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

        switch (get_type())
        {
        case Number::INTEGER:
            break;

        case Number::FLOATING:
            i = integer_type(get_f().str());
            f = static_cast<floating_type>(i);
            if (f == get_f())
                assign(i);
            break;

        default:
            assert(0);
            break;
        }
    }
} // namespace pmp

/////////////////////////////////////////////////////////////////////////////
// Non-member functions

namespace pmp
{
    Number floor(const Number& num1)
    {
        floating_type f;
        switch (num1.get_type())
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
        switch (num1.get_type())
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
        std::cout << n4.convert_to<int>() << std::endl;
        std::cout << n4.convert_to<__int64>() << std::endl;
        std::cout << n4.convert_to<float>() << std::endl;
        std::cout << n4.convert_to<double>() << std::endl;
        std::cout << n4.convert_to<long double>() << std::endl;

        return 0;
    }
#endif  // def UNITTEST

/////////////////////////////////////////////////////////////////////////////
