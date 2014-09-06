
// Ndrr1D.cpp --- The one-dimensional Numeric Domains/Ranges Resolver
// See file "ReadMe.txt" and "License.txt".
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

////////////////////////////////////////////////////////////////////////////
// compare two aspects

bool operator==(const Ndrr1D::Aspect& a1, const Ndrr1D::Aspect& a2)
{
    if (a1.m_pnModulus && a2.m_pnModulus)
    {
        if (*a1.m_pnModulus == *a2.m_pnModulus &&
            *a1.m_pnResidue == *a2.m_pnResidue)
        {
            if (a1.MustBePrime() == a2.MustBePrime())
            {
                if (a1.MustBeComposite() == a2.MustBeComposite())
                {
                    return true;
                }
            }
        }
    }
    else if (a1.m_pnModulus == NULL && a2.m_pnModulus == NULL)
    {
        if (a1.MustBePrime() == a2.MustBePrime())
        {
            if (a1.MustBeComposite() == a2.MustBeComposite())
            {
                return true;
            }
        }
    }
    return false;
}

bool operator!=(const Ndrr1D::Aspect& a1, const Ndrr1D::Aspect& a2)
{
    return !(a1 == a2);
}

////////////////////////////////////////////////////////////////////////////

namespace Ndrr1D
{

////////////////////////////////////////////////////////////////////////////
// public functions

bool IsPositiveNumber(const number_type& value)
{
    return value > 0;
}

bool IsNegativeNumber(const number_type& value)
{
    return value < 0;
}

bool IsNaturalNumber(const number_type& value)
{
    #ifdef DRR1D_USES_PMPNUMBER
        return value >= 1 && (value % 1).is_zero();
    #else
        return value >= 1 && std::fmod(value, 1) == 0;
    #endif
}

bool IsRegularNumber(const number_type& value)
{
    #ifdef DRR1D_USES_PMPNUMBER
        return (value % 1).is_zero();
    #else
        return std::fmod(value, 1) == 0;
    #endif
}

bool IsEvenNumber(const number_type& value)
{
    #ifdef DRR1D_USES_PMPNUMBER
        return (value % 2).is_zero();
    #else
        return std::fmod(value, 2) == 0;
    #endif
}

bool IsOddNumber(const number_type& value)
{
    #ifdef DRR1D_USES_PMPNUMBER
        number_type v = value % 2;
        return v == 1 || v == -1;
    #else
        number_type v = std::fmod(value, 2);
        return v == 1 || v == -1;
    #endif
}

bool IsPrimeNumber(const number_type& value)
{
    if (!IsRegularNumber(value))
        return false;

    if (value < 2)
        return false;

    if (value == 2)
        return true;

    #ifdef DRR1D_USES_PMPNUMBER
        if ((value % 2).is_zero())
            return false;

        for (number_type i = 3; i < value; i += 2)
        {
            if ((value % i).is_zero())
                return false;
        }
    #else
        if (std::fmod(value, 2) == 0)
            return false;

        for (int i = 3; i < value; i += 2)
        {
            if (std::fmod(value, i) == 0)
                return false;
        }
    #endif

    return true;
}

bool IsCompositeNumber(const number_type& value)
{
    if (!IsRegularNumber(value))
        return false;

    if (value > 1)
        return !IsPrimeNumber(value);

    if (value < -1)
        return !IsPrimeNumber(-value);

    return false;
}

number_type ModFloor(number_type d, number_type mod)
{
    if (mod < 0)
        mod = -mod;

    number_type v;
    #ifdef DRR1D_USES_PMPNUMBER
        v = (d % mod);
        if (v.is_zero())
            return d;
        if (v < 0)
            v += mod;
        return (d - v);
    #else
        v = std::fmod(d, mod);
        if (v == 0)
            return d;
        if (v < 0)
            v += mod;
        return (d - v);
    #endif
}

number_type ModCeil(number_type d, number_type mod)
{
    if (mod < 0)
        mod = -mod;

    number_type v;
    #ifdef DRR1D_USES_PMPNUMBER
        v = (d % mod);
        if (v.is_zero())
            return d;
        if (v < 0)
            v += mod;
        return (d - v) + mod;
    #else
        v = std::fmod(d, mod);
        if (v == 0)
            return d;
        if (v < 0)
            v += mod;
        return (d - v) + mod;
    #endif
}

integer_type gcd(integer_type x, integer_type y)
{
    integer_type z;

    if (x < 0)
        x = -x;
    if (y < 0)
        y = -y;

    assert(x != 0 || y != 0);
    #ifdef DRR1D_USES_PMPNUMBER
        if (y.is_zero())
            return x;

        for (;;)
        {
            z = x % y;
            if (z.is_zero())
                break;
            x = y;
            y = z;
        }
    #else
        if (y == 0)
            return x;

        for (;;)
        {
            z = x % y;
            if (z == 0)
                break;
            x = y;
            y = z;
        }
    #endif

    return y;
}

integer_type lcm(integer_type x, integer_type y)
{
    assert(x != 0 && y != 0);

    if (x < 0)
        x = -x;
    if (y < 0)
        y = -y;

    return x * y / gcd(x, y);
}

integer_type gcdx(integer_type& x, integer_type& y,
                  const integer_type& a, const integer_type& b)
{
    assert(a > 0 || b > 0);
    integer_type q1, r2, x2, y2;
    integer_type r0 = a, r1 = b, x0 = 1, x1 = 0, y0 = 0, y1 = 1;
    while (r1 > 0)
    {
        q1 = r0 / r1;
        r2 = r0 % r1;
        x2 = x0 - q1 * x1;
        y2 = y0 - q1 * y1;
        r0 = r1; r1 = r2; x0 = x1; x1 = x2; y0 = y1; y1 = y2;
    }
    x = x0; y = y0;
    return r0;
}

integer_type cnrem(
    const integer_type& a1, const integer_type& m1,
    const integer_type& a2, const integer_type& m2)
{
    assert(m1 > 0 && m2 > 0);
    integer_type x, y;
    integer_type d = gcdx(x, y, m1, m2);
    assert(d == 1);
    return a1 + (a2 - a1) * x * m1;
}

bool katayama_qz(
    integer_type& y, integer_type& M, integer_type & N,
    const integer_type& a1, integer_type m1,
    const integer_type& a2, integer_type m2)
{
    N = gcd(m1, m2);
    M = m1 * m2 / N;
    if ((a2 - a1) % N != 0)
        return false;

    if (N != 1)
    {
        m1 /= N;
        m2 /= N;
    }
    y = cnrem(a1, m1, a2, m2);

    while (y < 0)
        y += M;
    y %= M;

    return true;
}

////////////////////////////////////////////////////////////////////////////
// Ndrr1D::Range

Range::Range() :
    m_has_min(false), m_has_max(false), m_pnLBound(NULL), m_pnUBound(NULL)
{
}

Range::Range(bool has_min, bool has_max,
             number_type *pnLBound, number_type *pnUBound) :
    m_has_min(has_min), m_has_max(has_max),
    m_pnLBound(pnLBound ? new number_type(*pnLBound) : NULL),
    m_pnUBound(pnUBound ? new number_type(*pnUBound) : NULL)
{
    #ifdef DRR1D_USES_PMPNUMBER
        if (m_pnLBound)
            m_pnLBound->trim();
        if (m_pnUBound)
            m_pnUBound->trim();
    #endif
}

Range::Range(const number_type& value) :
    m_has_min(true), m_has_max(true),
    m_pnLBound(new number_type(value)),
    m_pnUBound(new number_type(value))
{
    #ifdef DRR1D_USES_PMPNUMBER
        if (m_pnLBound)
            m_pnLBound->trim();
        if (m_pnUBound)
            m_pnUBound->trim();
    #endif
}

Range::Range(const Range& s) :
    m_has_min(s.m_has_min), m_has_max(s.m_has_max),
    m_pnLBound(s.m_pnLBound ? new number_type(*s.m_pnLBound) : NULL),
    m_pnUBound(s.m_pnUBound ? new number_type(*s.m_pnUBound) : NULL)
{
}

/*virtual*/ Range::~Range()
{
    delete m_pnLBound;
    delete m_pnUBound;
}

Range& Range::operator=(const Range& s)
{
    m_has_min = s.m_has_min;
    m_has_max = s.m_has_max;
    if (m_pnLBound)
        delete m_pnLBound;
    m_pnLBound = (s.m_pnLBound ? new number_type(*s.m_pnLBound) : NULL);
    if (m_pnUBound)
        delete m_pnUBound;
    m_pnUBound = (s.m_pnUBound ? new number_type(*s.m_pnUBound) : NULL);
    return *this;
}

bool Range::empty() const
{
    if (m_pnLBound && m_pnUBound)
    {
        if (*m_pnLBound > *m_pnUBound)
            return true;
        if (*m_pnLBound == *m_pnUBound && (!m_has_min || !m_has_max))
            return true;
    }
    return false;
}

bool Range::entire() const
{
    return m_pnLBound == NULL && m_pnUBound == NULL;
}

void Range::clear()
{
    delete m_pnLBound;
    m_pnLBound = new number_type(1);
    delete m_pnUBound;
    m_pnUBound = new number_type(0);
}

bool Range::Includes(const Range& r) const
{
    if (r.empty())
        return true;

    if (empty())
        return false;

    if (m_pnLBound && r.m_pnLBound)
    {
        if (*m_pnLBound > *r.m_pnLBound)
            return false;
        if (*m_pnLBound == *r.m_pnLBound && m_has_min < r.m_has_min)
            return false;
    }
    else if (m_pnLBound)
    {
        return false;
    }

    if (m_pnUBound && r.m_pnUBound)
    {
        if (*m_pnUBound < *r.m_pnUBound)
            return false;
        if (*m_pnUBound == *r.m_pnUBound && m_has_max < r.m_has_max)
            return false;
    }
    else if (m_pnUBound)
    {
        return false;
    }

    return true;
}

bool Range::Contains(const number_type& value) const
{
    if (m_pnLBound)
    {
        if (*m_pnLBound > value)
            return false;
        if (*m_pnLBound == value && !m_has_min)
            return false;
    }

    if (m_pnUBound)
    {
        if (*m_pnUBound < value)
            return false;
        if (*m_pnUBound == value && !m_has_max)
            return false;
    }

    return true;
}

void Range::Intersect(const Range& r)
{
    Range *r0 = Range::Intersect(this, &r);
    if (r0)
    {
        *this = *r0;
        delete r0;
    }
    else
        clear();
}

/*static*/ Range *Range::Intersect(const Range *r1, const Range *r2)
{
    if (r1->empty() || r2->empty())
        return NULL;

    bool has_min = false;
    number_type *pnLBound = NULL;
    if (r1->m_pnLBound && r2->m_pnLBound)
    {
        if (*r1->m_pnLBound < *r2->m_pnLBound)
        {
            pnLBound = r2->m_pnLBound;
            has_min = r2->m_has_min;
        }
        else if (*r1->m_pnLBound > *r2->m_pnLBound)
        {
            pnLBound = r1->m_pnLBound;
            has_min = r1->m_has_min;
        }
        else
        {
            pnLBound = r1->m_pnLBound;
            has_min = r1->m_has_min && r2->m_has_min;
        }
    }
    else if (r1->m_pnLBound)
    {
        pnLBound = r1->m_pnLBound;
        has_min = r1->m_has_min;
    }
    else if (r2->m_pnLBound)
    {
        pnLBound = r2->m_pnLBound;
        has_min = r2->m_has_min;
    }

    bool has_max = false;
    number_type *pnUBound = NULL;
    if (r1->m_pnUBound && r2->m_pnUBound)
    {
        if (*r1->m_pnUBound < *r2->m_pnUBound)
        {
            pnUBound = r1->m_pnUBound;
            has_max = r1->m_has_max;
        }
        else if (*r1->m_pnUBound > *r2->m_pnUBound)
        {
            pnUBound = r2->m_pnUBound;
            has_max = r2->m_has_max;
        }
        else
        {
            pnUBound = r2->m_pnUBound;
            has_max = r1->m_has_max && r2->m_has_max;
        }
    }
    else if (r1->m_pnUBound)
    {
        pnUBound = r1->m_pnUBound;
        has_max = r1->m_has_max;
    }
    else if (r2->m_pnUBound)
    {
        pnUBound = r2->m_pnUBound;
        has_max = r2->m_has_max;
    }

    Range *r = new Range(has_min, has_max, pnLBound, pnUBound);
    if (r->empty())
    {
        delete r;
        return NULL;
    }
    return r;
}

bool Range::Equal(const Range& r) const
{
    return Includes(r) && r.Includes(*this);
}

////////////////////////////////////////////////////////////////////////////
// Ndrr1D::Ranges

Ranges::Ranges(const Range& r)
{
    Range *r0 = new Range(r);
    add(r0);
}

Ranges::Ranges(bool has_min, bool has_max,
               number_type *pnLBound, number_type *pnUBound)
{
    Range *r = new Range(has_min, has_max, pnLBound, pnUBound);
    add(r);
}

Ranges::Ranges(const number_type& value)
{
    Range *r = new Range(value);
    add(r);
}

Ranges::Ranges(const std::vector<shared_ptr<Range> >& ranges) :
    std::vector<shared_ptr<Range> >(ranges)
{
}

Ranges& Ranges::operator=(const std::vector<shared_ptr<Range> >& ranges)
{
    assign(ranges.begin(), ranges.end());
    return *this;
}

void Ranges::add(Range *r)
{
    push_back(shared_ptr<Range>(r));
}

bool Ranges::empty() const
{
    size_t i, siz = size();
    if (siz == 0)
    {
        return true;
    }

    for (i = 0; i < siz; ++i)
    {
        if (!((*this)[i])->empty())
        {
            return false;
        }
    }
    return true;
}

bool Ranges::entire() const
{
    size_t i, siz = size();
    if (siz == 0)
        return false;

    for (i = 0; i < siz; ++i)
    {
        if (((*this)[i])->entire())
            return true;
    }
    return false;
}

bool Ranges::Includes(const Range& r) const
{
    size_t i, siz = size();
    for (i = 0; i < siz; ++i)
    {
        if ((*this)[i]->Includes(r))
            return true;
    }
    return false;
}

bool Ranges::Includes(const Ranges& r) const
{
    size_t i, siz = r.size();
    for (i = 0; i < siz; ++i)
    {
        if (!Includes(*r[i].get()))
            return false;
    }
    return true;
}

bool Ranges::Contains(const number_type& value) const
{
    size_t i, siz = size();
    for (i = 0; i < siz; ++i)
    {
        if ((*this)[i]->Contains(value))
            return true;
    }
    return false;
}

void Ranges::Intersect(const Range& r)
{
    Ranges *ranges = Ranges::Intersect(this, &r);
    *this = *ranges;
    delete ranges;
}

void Ranges::Intersect(const Ranges& ranges)
{
    Ranges *r = Ranges::Intersect(this, &ranges);
    *this = *r;
    delete r;
}

void Ranges::Union(const Ranges& ranges)
{
    Ranges *r = Ranges::Union(this, &ranges);
    *this = *r;
    delete r;
}

number_type *Ranges::GetLBound(bool& has_min) const
{
    size_t i, siz = size();
    has_min = false;
    if (siz == 0 || empty())
    {
        return NULL;
    }
    for (i = 0; i < siz; ++i)
    {
        if (((*this)[i])->m_pnLBound == NULL)
            return NULL;
    }
    number_type *value = ((*this)[0])->m_pnLBound;
    has_min = ((*this)[0])->m_has_min;
    for (i = 1; i < siz; ++i)
    {
        Range& r = *((*this)[i]).get();
        if (*r.m_pnLBound < *value)
        {
            value = r.m_pnLBound;
            has_min = r.m_has_min;
        }
        else if (*value == *r.m_pnLBound)
        {
            if (r.m_has_min)
                has_min = true;
        }
    }
    return value;
}

number_type *Ranges::GetUBound(bool& has_max) const
{
    size_t i, siz = size();
    has_max = false;
    if (siz == 0 || empty())
    {
        return NULL;
    }
    for (i = 0; i < siz; ++i)
    {
        if (((*this)[i])->m_pnUBound == NULL)
        {
            return NULL;
        }
    }
    number_type *value = ((*this)[0])->m_pnUBound;
    has_max = ((*this)[0])->m_has_max;
    for (i = 1; i < siz; ++i)
    {
        Range& r = *((*this)[i]).get();
        if (*value < *r.m_pnUBound)
        {
            value = r.m_pnUBound;
            has_max = r.m_has_max;
        }
        else if (*value == *r.m_pnUBound)
        {
            if (r.m_has_max)
                has_max = true;
        }
    }
    return value;
}

void Ranges::Optimize()
{
    size_t i, j, siz;

    // remove empty ranges
retry1:
    siz = size();
    for (i = 0; i < siz; ++i)
    {
        if ((*this)[i]->empty())
        {
            erase(begin() + i);
            goto retry1;
        }
    }

retry2:
    siz = size();
    if (siz == 0)
        return;

    for (i = 0; i < siz - 1; ++i)
    {
        for (j = i + 1; j < siz; ++j)
        {
            Range *r0 =
                Range::Intersect((*this)[i].get(), (*this)[j].get());
            if (r0)
            {
                delete r0;

                bool has_min = false, has_max = false;;
                number_type *pnLBound = NULL, *pnUBound = NULL;

                Range *r1 = (*this)[i].get();
                Range *r2 = (*this)[j].get();
                if (r1->m_pnLBound && r2->m_pnLBound)
                {
                    if (*r1->m_pnLBound < *r2->m_pnLBound)
                    {
                        pnLBound = r1->m_pnLBound;
                        has_min = r1->m_has_min;
                    }
                    else if (*r1->m_pnLBound > *r2->m_pnLBound)
                    {
                        pnLBound = r2->m_pnLBound;
                        has_min = r2->m_has_min;
                    }
                    else
                    {
                        pnLBound = r2->m_pnLBound;
                        has_min = r1->m_has_min || r2->m_has_min;
                    }
                }
                if (r1->m_pnUBound && r2->m_pnUBound)
                {
                    if (*r1->m_pnUBound < *r2->m_pnUBound)
                    {
                        pnUBound = r2->m_pnUBound;
                        has_max = r2->m_has_max;
                    }
                    else if (*r1->m_pnUBound > *r2->m_pnUBound)
                    {
                        pnUBound = r1->m_pnUBound;
                        has_max = r1->m_has_max;
                    }
                    else
                    {
                        pnUBound = r2->m_pnUBound;
                        has_max = r1->m_has_max || r2->m_has_max;
                    }
                }

                r0 = new Range(has_min, has_max, pnLBound, pnUBound);
                (*this)[i] = shared_ptr<Range>(r0);
                erase(begin() + j);
                goto retry2;
            }
            else if (
                (*this)[i]->m_pnUBound && (*this)[j]->m_pnLBound &&
                *(*this)[i]->m_pnUBound == *(*this)[j]->m_pnLBound &&
                ((*this)[i]->m_has_max || (*this)[j]->m_has_min))
            {
                bool has_min, has_max;
                number_type *pnLBound, *pnUBound;
                has_min = (*this)[i]->m_has_min;
                has_max = (*this)[j]->m_has_max;
                pnLBound = (*this)[i]->m_pnLBound;
                pnUBound = (*this)[j]->m_pnUBound;
                Range *r = new Range(has_min, has_max, pnLBound, pnUBound);
                (*this)[i] = shared_ptr<Range>(r);
                erase(begin() + j);
                goto retry2;
            }
            else if (
                (*this)[j]->m_pnUBound && (*this)[i]->m_pnLBound &&
                *(*this)[j]->m_pnUBound == *(*this)[i]->m_pnLBound &&
                ((*this)[j]->m_has_max || (*this)[i]->m_has_min))
            {
                bool has_min, has_max;
                number_type *pnLBound, *pnUBound;
                has_min = (*this)[j]->m_has_min;
                has_max = (*this)[i]->m_has_max;
                pnLBound = (*this)[j]->m_pnLBound;
                pnUBound = (*this)[i]->m_pnUBound;
                Range *r = new Range(has_min, has_max, pnLBound, pnUBound);
                (*this)[i] = shared_ptr<Range>(r);
                erase(begin() + j);
                goto retry2;
            }
        }
    }

    // check existence of the whole range
    for (i = 0; i < siz; ++i)
    {
        if ((*this)[i]->entire())
        {
            Range *r = Range::Whole();
            clear();
            add(r);
            break;
        }
    }
}

/*static*/ Ranges *Ranges::Union(const Ranges *r1, const Ranges *r2)
{
    Ranges *r = new Ranges(*r1);
    r->insert(r->end(), r2->begin(), r2->end());
    r->Optimize();
    return r;
}

/*static*/ Ranges *Ranges::Intersect(const Ranges *ranges, const Range *r)
{
    Ranges *pRanges  = new Ranges();
    size_t i, siz = ranges->size();
    for (i = 0; i < siz; ++i)
    {
        Range *r0 = Range::Intersect((*ranges)[i].get(), r);
        if (r0)
            pRanges->add(r0);
    }
    return pRanges;
}

/*static*/ Ranges *Ranges::Intersect(const Ranges *r1, const Ranges *r2)
{
    Ranges *pRanges  = new Ranges();
    size_t i, siz = r2->size();
    for (i = 0; i < siz; ++i)
    {
        Ranges *r = Ranges::Intersect(r1, ((*r2)[i]).get());
        if (!r->empty())
            pRanges->insert(pRanges->end(), r->begin(), r->end());
        delete r;
    }
    return pRanges;
}

/*static*/ Ranges *Ranges::Whole()
{
    Ranges *ranges = new Ranges();
    Range *r = Range::Whole();
    ranges->add(r);
    return ranges;
}

bool Ranges::Equal(const Ranges& r) const
{
    return Includes(r) && r.Includes(*this);
}

////////////////////////////////////////////////////////////////////////////
// Ndrr1D::Aspect

Aspect::Aspect() :
    m_pnModulus(NULL), m_pnResidue(NULL),
    m_must_be_prime(false), m_must_be_composite(false)
{
}

Aspect::Aspect(integer_type *pnModulus, integer_type *pnResidue/* = NULL*/,
               bool must_be_prime/* = false*/,
               bool must_be_composite/* = false*/) :
    m_pnModulus(pnModulus ? new integer_type(*pnModulus) : NULL),
    m_pnResidue(pnResidue ? new integer_type(*pnResidue) : NULL),
    m_must_be_prime(must_be_prime),
    m_must_be_composite(must_be_composite)
{
    assert(m_pnModulus == NULL || *m_pnModulus > 0);
    assert(m_pnResidue == NULL || *m_pnResidue >= 0);
    assert(m_pnModulus == NULL || *m_pnResidue < *m_pnModulus);
    assert((m_pnModulus != NULL) == (m_pnResidue != NULL));
}

Aspect::Aspect(const Aspect& a) :
    m_pnModulus(a.m_pnModulus ? new integer_type(*a.m_pnModulus) : NULL),
    m_pnResidue(a.m_pnResidue ? new integer_type(*a.m_pnResidue) : NULL),
    m_must_be_prime(a.m_must_be_prime),
    m_must_be_composite(a.m_must_be_composite)
{
    assert(m_pnModulus == NULL || *m_pnModulus > 0);
    assert(m_pnResidue == NULL || *m_pnResidue >= 0);
    assert(m_pnModulus == NULL || *m_pnResidue < *m_pnModulus);
    assert((m_pnModulus != NULL) == (m_pnResidue != NULL));
}

Aspect& Aspect::operator=(const Aspect& a)
{
    if (m_pnModulus)
        delete m_pnModulus;
    m_pnModulus = (a.m_pnModulus ? new integer_type(*a.m_pnModulus) : NULL);
    if (m_pnResidue)
        delete m_pnResidue;
    m_pnResidue = (a.m_pnResidue ? new integer_type(*a.m_pnResidue) : NULL);
    m_must_be_prime = a.m_must_be_prime;
    m_must_be_composite = a.m_must_be_composite;
    assert(m_pnModulus == NULL || *m_pnModulus > 0);
    assert(m_pnResidue == NULL || *m_pnResidue >= 0);
    assert(m_pnModulus == NULL || *m_pnResidue < *m_pnModulus);
    assert((m_pnModulus != NULL) == (m_pnResidue != NULL));
    return *this;
}

/*virtual*/ Aspect::~Aspect()
{
    delete m_pnModulus;
    delete m_pnResidue;
}

bool Aspect::empty() const
{
    if (MustBePrime() && MustBeComposite())
        return true;

    if (MustBePrime() && m_pnModulus)
    {
        assert(m_pnResidue);
        if (IsCompositeNumber(*m_pnModulus))
        {
            #ifdef DRR1D_USES_PMPNUMBER
                if (m_pnResidue->is_zero())
                    return true;
            #else
                if (*m_pnResidue == 0)
                    return true;
            #endif
            if (IsCompositeNumber(*m_pnResidue))
                return true;
        }
        if (IsEvenNumber(*m_pnModulus))
        {
            if (*m_pnModulus > 2 && IsEvenNumber(*m_pnResidue))
            {
                if (*m_pnResidue != 2)
                    return true;
            }
        }
    }
    return false;
}

bool Aspect::entire() const
{
    if (MustBePrime() || MustBeComposite())
        return false;

    return m_pnModulus == NULL;
}

void Aspect::clear()
{
    delete m_pnModulus;
    m_pnModulus = NULL;
    delete m_pnResidue;
    m_pnResidue = NULL;
    m_must_be_prime = true;
    m_must_be_composite = true;
}

bool Aspect::Includes(const Aspect& a) const
{
    if (a.empty())
        return true;

    if (empty())
        return false;

    if (MustBePrime() && !a.MustBePrime())
        return false;

    if (MustBeComposite() && !a.MustBeComposite())
        return false;

    if (m_pnModulus)
    {
        if (a.m_pnModulus == NULL)
            return false;

        assert(m_pnResidue);
        assert(a.m_pnResidue);

        const integer_type& m1 = *m_pnModulus;
        const integer_type& r1 = *m_pnResidue;
        const integer_type& m2 = *a.m_pnModulus;
        const integer_type& r2 = *a.m_pnResidue;
        if (m1 == m2 && r1 == r2)
        {
            return true;
        }

        if (m2 % m1 != 0)
        {
            return false;
        }

        integer_type N(gcd(m1, m2));
        if ((r2 - r1) % N != 0)
        {
            return false;
        }
    }

    return true;
}

bool Aspect::Contains(const number_type& value) const
{
    if (MustBePrime() && MustBeComposite())
        return false;

    if (m_pnModulus)
    {
        assert(m_pnResidue);
        #ifdef DRR1D_USES_PMPNUMBER
            number_type r = value % *m_pnModulus;
            r.trim();
        #else
            number_type r = std::fmod(value, *m_pnModulus);
        #endif
        if (r != *m_pnResidue)
            return false;
    }

    if (MustBePrime() && !IsPrimeNumber(value))
        return false;

    if (MustBeComposite() && !IsCompositeNumber(value))
        return false;

    return true;
}

void Aspect::Intersect(const Aspect& a)
{
    Aspect *aspect = Aspect::Intersect(this, &a);
    if (aspect)
    {
        *this = *aspect;
        delete aspect;
    }
    else
        clear();
}

/*static*/ Aspect *Aspect::Intersect(const Aspect *a1, const Aspect *a2)
{
    if (a1->empty() || a2->empty())
        return NULL;

    integer_type *pnModulus = NULL, *pnResidue = NULL;
    if (a1->m_pnModulus && a2->m_pnModulus)
    {
        integer_type m1 = *a1->m_pnModulus;
        integer_type m2 = *a2->m_pnModulus;
        assert(a1->m_pnResidue);
        assert(a2->m_pnResidue);
        integer_type r1 = *a1->m_pnResidue;
        integer_type r2 = *a2->m_pnResidue;
        assert(0 <= r1 && r1 < m1);
        assert(0 <= r2 && r2 < m2);

        if (m1 == m2)
        {
            if (r1 == r2)
            {
                pnModulus = new integer_type(m1);
                pnResidue = new integer_type(r1);
            }
            else
            {
                return NULL;
            }
        }
        else
        {
            integer_type y, M, N;
            if (katayama_qz(y, M, N, r1, m1, r2, m2))
            {
                pnModulus = new integer_type(M);
                pnResidue = new integer_type(y);
            }
            else
            {
                return NULL;
            }
        }
    }
    else if (a1->m_pnModulus)
    {
        integer_type m1 = *a1->m_pnModulus;
        assert(a1->m_pnResidue);
        integer_type r1 = *a1->m_pnResidue;
        assert(m1 > 0);
        assert(0 <= r1 && r1 < m1);

        pnModulus = new integer_type(m1);
        pnResidue = new integer_type(r1);
    }
    else if (a2->m_pnModulus)
    {
        integer_type m2 = *a2->m_pnModulus;
        assert(a2->m_pnResidue);
        integer_type r2 = *a2->m_pnResidue;
        assert(m2 > 0);
        assert(0 <= r2 && r2 < m2);

        pnModulus = new integer_type(m2);
        pnResidue = new integer_type(r2);
    }

    bool must_be_prime = a1->MustBePrime() || a2->MustBePrime();
    bool must_be_composite = a1->MustBeComposite() || a2->MustBeComposite();
    Aspect *a = new Aspect(pnModulus, pnResidue,
                           must_be_prime, must_be_composite);
    delete pnModulus;
    delete pnResidue;
    return a;
}

bool Aspect::Equal(const Aspect& a) const
{
    if (MustBePrime() != a.MustBePrime())
        return false;

    if (MustBeComposite() != a.MustBeComposite())
        return false;

    if (m_pnModulus && a.m_pnModulus)
    {
        if (*m_pnModulus == *a.m_pnModulus)
        {
            if (*m_pnResidue == *a.m_pnResidue)
                return true;
        }
    }
    else if (m_pnModulus == NULL && a.m_pnModulus == NULL)
    {
        return true;
    }

    return false;
}

bool Aspect::MustBe2() const
{
    integer_type *pnModulus = GetModulus();
    integer_type *pnResidue = GetResidue();
    if (!MustBePrime() || pnModulus == NULL)
    {
        return false;
    }

    assert(pnResidue);
    if (*pnModulus == 2 && *pnResidue == 0)
    {
        return true;
    }
    else if (*pnModulus > 2 && IsEvenNumber(*pnModulus))
    {
        if (IsEvenNumber(*pnResidue))
        {
            return true;
        }
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////
// Ndrr1D::Domain

Domain::Domain(
    bool has_min, bool has_max,
    number_type *pnLBound, number_type *pnUBound)
{
    Range *r = new Range(has_min, has_max, pnLBound, pnUBound);
    m_ranges.add(r);
}

Domain::Domain(
    const Aspect& a,
    bool has_min, bool has_max,
    number_type *pnLBound, number_type *pnUBound) :
    m_aspect(a)
{
    Range *r = new Range(has_min, has_max, pnLBound, pnUBound);
    m_ranges.add(r);
}

Domain::Domain(const Aspect& a, const number_type& value) :
    m_aspect(a), m_ranges(value)
{
}

Domain::Domain(const Aspect& a) : m_aspect(a)
{
    m_ranges.add(new Range());
}

bool Domain::Includes(const Domain& d) const
{
    if (d.empty())
        return true;

    if (empty())
        return false;

    return m_aspect.Includes(d.m_aspect) && m_ranges.Includes(d.m_ranges);
}

/*static*/ Domain *Domain::Intersect(const Domain *d1, const Domain *d2)
{
    if (d1->empty() || d2->empty())
    {
        return NULL;
    }

    Domain *d = NULL;
    Aspect *a = Aspect::Intersect(&d1->m_aspect, &d2->m_aspect);
    Ranges *r = Ranges::Intersect(&d1->m_ranges, &d2->m_ranges);
    if (a && r)
        d = new Domain(*a, *r);
    delete r;
    delete a;

    if (d)
    {
        d->FixNarrower();
        if (d->empty())
        {
            delete d;
            d = NULL;
        }
    }
    return d;
}

void Domain::Intersect(const Range& r)
{
    size_t i, siz = m_ranges.size();
    for (i = 0; i < siz; ++i)
    {
        m_ranges[i].get()->Intersect(r);
    }
}

void Domain::Intersect(const Domain& d)
{
    Domain *domain = Domain::Intersect(this, &d);
    if (domain)
    {
        *this = *domain;
        delete domain;
    }
    else
    {
        clear();
    }
}

bool Domain::GetValues(std::vector<number_type>& values) const
{
    values.clear();
    if (empty())
        return true;

    bool has_min, has_max;
    number_type *pnLBound = GetLBound(has_min);
    number_type *pnUBound = GetUBound(has_max);
    if (pnLBound == NULL || pnUBound == NULL)
    {
        return false;
    }

    integer_type *pnModulus = GetModulus();
    if (pnModulus)
    {
        integer_type *pnResidue = GetResidue();
        assert(pnResidue);

        if (*pnLBound > *pnUBound)
        {
            return true;
        }

        if (*pnLBound == *pnUBound)
        {
            if (Contains(*pnLBound))
                values.push_back(*pnLBound);
            return true;
        }

        number_type m0 = ModCeil(*pnLBound - *pnResidue, *pnModulus) + *pnResidue;
        number_type m1 = ModFloor(*pnUBound - *pnResidue, *pnModulus) + *pnResidue;
        #ifdef DRR1D_USES_PMPNUMBER
            integer_type n0 = m0.to_i();
            integer_type n1 = m1.to_i();
        #else
            integer_type n0 = (integer_type)m0;
            integer_type n1 = (integer_type)m1;
        #endif
        for (integer_type i = n0; i <= n1; i += *pnModulus)
        {
            if (m_aspect.Contains(i))
            {
                values.push_back(i);
            }
        }
        std::sort(values.begin(), values.end());
        std::unique(values.begin(), values.end());
        return true;
    }
    else
    {
        if (*pnLBound == *pnUBound && has_min && has_max)
        {
            if (!m_aspect.Contains(*pnLBound))
            {
                return true;
            }

            values.push_back(*pnLBound);
            return true;
        }
        return false;
    }
}

void Domain::FixWider()
{
    if (empty())
    {
        clear();
        return;
    }

    integer_type *pnModulus = GetModulus();
    if (pnModulus == NULL)
    {
        return;
    }
    integer_type *pnResidue = GetResidue();
    assert(pnResidue);

    if (MustBe2())
    {
        if (Contains(2))
        {
            *this = Domain(2);
        }
        else
        {
            clear();
        }
        return;
    }

    size_t i, siz = m_ranges.size();
    for (i = 0; i < siz; ++i)
    {
        Range *r0 = m_ranges[i].get();
        bool has_min, has_max;
        number_type *pnLBound = r0->GetLBound(has_min);
        if (pnLBound)
        {
            number_type v1 = *pnLBound;
            number_type v2 = ModFloor(v1 - *pnResidue, *pnModulus) + *pnResidue;
            *pnLBound = v2;
            if (v1 == v2 && has_min)
            {
                *pnLBound -= *pnModulus;
            }
            r0->m_has_min = false;
            if (MustBePrime() && !IsPrimeNumber(*pnLBound))
            {
                while (*pnLBound >= 2 && !IsPrimeNumber(*pnLBound))
                {
                    *pnLBound -= *pnModulus;
                }
            }
            if (MustBeComposite() && !IsCompositeNumber(*pnLBound))
            {
                while (!IsCompositeNumber(*pnLBound))
                {
                    *pnLBound -= *pnModulus;
                }
            }
        }
        number_type *pnUBound = r0->GetUBound(has_max);
        if (pnUBound)
        {
            number_type v1 = *pnUBound;
            number_type v2 = ModCeil(v1 - *pnResidue, *pnModulus) + *pnResidue;
            *pnUBound = v2;
            if (v1 == v2 && has_max)
            {
                *pnUBound += *pnModulus;
            }
            r0->m_has_max = false;
            if (MustBePrime() && !IsPrimeNumber(*pnUBound))
            {
                do
                {
                    *pnUBound += *pnModulus;
                } while (!IsPrimeNumber(*pnUBound));
            }
            if (MustBeComposite() && !IsCompositeNumber(*pnUBound))
            {
                do
                {
                    *pnUBound += *pnModulus;
                } while (!IsCompositeNumber(*pnUBound));
            }
        }
    }
}

void Domain::FixNarrower()
{
    if (empty())
    {
        clear();
        return;
    }

    integer_type *pnModulus = GetModulus();
    if (pnModulus == NULL)
    {
        return;
    }
    integer_type *pnResidue = GetResidue();
    assert(pnResidue);

    if (MustBe2())
    {
        if (Contains(2))
        {
            *this = Domain(2);
        }
        else
        {
            clear();
        }
        return;
    }

    size_t i, siz = m_ranges.size();
    for (i = 0; i < siz; ++i)
    {
        Range *r0 = m_ranges[i].get();
        bool has_min, has_max;
        number_type *pnLBound = r0->GetLBound(has_min);
        if (pnLBound)
        {
            number_type v1 = *pnLBound;
            number_type v2 = ModCeil(v1 - *pnResidue, *pnModulus) + *pnResidue;
            *pnLBound = v2;
            if (v1 == v2)
            {
                if (!r0->m_has_min)
                {
                    *pnLBound += *pnModulus;
                    r0->m_has_min = true;
                }
            }
            else
            {
                r0->m_has_min = true;
            }
            if (MustBePrime() && !IsPrimeNumber(*pnLBound))
            {
                do
                {
                    *pnLBound += *pnModulus;
                } while (!IsPrimeNumber(*pnLBound));
                r0->m_has_min = true;
            }
            if (MustBeComposite() && !IsCompositeNumber(*pnLBound))
            {
                do
                {
                    *pnLBound += *pnModulus;
                } while (!IsCompositeNumber(*pnLBound));
                r0->m_has_min = true;
            }
        }
        number_type *pnUBound = r0->GetUBound(has_max);
        if (pnUBound)
        {
            number_type v1 = *pnUBound;
            number_type v2 = ModFloor(v1 - *pnResidue, *pnModulus) + *pnResidue;
            *pnUBound = v2;
            if (v1 == v2)
            {
                if (!r0->m_has_max)
                {
                    *pnUBound -= *pnModulus;
                    r0->m_has_max = true;
                }
            }
            else
            {
                r0->m_has_max = true;
            }
            if (MustBePrime() && !IsPrimeNumber(*pnUBound))
            {
                while (*pnUBound >= 2 && !IsPrimeNumber(*pnUBound))
                {
                    *pnUBound -= *pnModulus;
                }
                r0->m_has_max = true;
            }
            if (MustBeComposite() && !IsCompositeNumber(*pnUBound))
            {
                while (!IsCompositeNumber(*pnUBound))
                {
                    *pnUBound -= *pnModulus;
                }
                r0->m_has_max = true;
            }
        }
    }
}

bool Domain::empty() const
{
    if (m_aspect.empty() || m_ranges.empty())
        return true;

    if (MustBePrime())
    {
        integer_type *pnModulus = GetModulus();
        integer_type *pnResidue = GetResidue();
        if (pnModulus)
        {
            assert(pnResidue);
            if (*pnModulus == 2 && *pnResidue == 0)
            {
                return !Contains(*pnModulus);
            }
            else if (*pnModulus > 2 && IsEvenNumber(*pnModulus))
            {
                if (*pnResidue == 2)
                {
                    return !Contains(*pnResidue);
                }
                else
                {
                    return IsEvenNumber(*pnResidue);
                }
            }
        }
    }
    return false;
}

bool Domain::entire() const
{
    return m_aspect.entire() && m_ranges.entire();
}

void Domain::clear()
{
    m_aspect.clear();
    m_ranges.clear();
}

/*static*/ Domain *Domain::Whole(Domain::Type type/* = Domain::REAL*/)
{
    Domain *d = new Domain();
    Range *r0;
    number_type n1;
    integer_type i1, i2;
    switch (type)
    {
    case Domain::EMPTY:
        d->clear();
        return d;

    case Domain::POSITIVE:
        n1 = 0;
        d->m_ranges = Ranges(false, false, &n1, NULL);
        return d;

    case Domain::NEGATIVE:
        n1 = 0;
        d->m_ranges = Ranges(false, false, NULL, &n1);
        return d;

    case Domain::NATURAL:
        n1 = 1;
        d->m_ranges = Ranges(true, false, &n1, NULL);
        i1 = 1;
        i2 = 0;
        d->m_aspect = Aspect(&i1, &i2);
        return d;

    case Domain::REGULAR:
        d->m_ranges = Ranges(Range());
        i1 = 1;
        i2 = 0;
        d->m_aspect = Aspect(&i1, &i2);
        return d;

    case Domain::EVEN:
        d->m_ranges = Ranges(Range());
        i1 = 2;
        i2 = 0;
        d->m_aspect = Aspect(&i1, &i2);
        return d;

    case Domain::ODD:
        d->m_ranges = Ranges(Range());
        i1 = 2;
        i2 = 1;
        d->m_aspect = Aspect(&i1, &i2);
        return d;

    case Domain::REAL:
        r0 = Range::Whole();
        d->m_ranges = Ranges(*r0);
        delete r0;
        return d;

    case Domain::PRIME:
        n1 = 2;
        d->m_ranges = Ranges(true, false, &n1, NULL);
        i1 = 1;
        i2 = 0;
        d->m_aspect = Aspect(&i1, &i2, true, false);
        return d;

    case Domain::COMPOSITE:
        r0 = Range::Whole();
        d->m_ranges = Ranges(*r0);
        delete r0;
        i1 = 1;
        i2 = 0;
        d->m_aspect = Aspect(&i1, &i2, false, true);
        return d;

    default:
        assert(0);
    }
    delete d;
    return NULL;
}

void Domain::Optimize()
{
    m_ranges.Optimize();
}

bool Domain::Equal(const Domain& d) const
{
    Domain d1(*this);
    Domain d2(d);
    d1.FixBeforeIncludes(d2);
    if (!d1.Includes(d2))
        return false;
    d2.FixBeforeIncludes(d1);
    if (!d2.Includes(d1))
        return false;
    return true;
}

void Domain::FixBeforeIncludes(Domain& d)
{
    FixWider();
    d.FixNarrower();
}

////////////////////////////////////////////////////////////////////////////
// Domains

void Domains::add(Domain *d)
{
    push_back(shared_ptr<Domain>(d));
}

bool Domains::empty() const
{
    if (size() == 0)
        return true;

    size_t i, siz = size();
    for (i = 0; i < siz; ++i)
    {
        if (!((*this)[i])->empty())
            return false;
    }
    return true;
}

bool Domains::entire() const
{
    if (size() == 0)
        return false;

    size_t i, siz = size();
    for (i = 0; i < siz; ++i)
    {
        if (((*this)[i])->entire())
            return true;
    }
    return false;
}

bool Domains::Includes(const Domain& domain) const
{
    if (domain.empty())
        return true;

    if (empty())
        return false;

    size_t i, siz = size();
    for (i = 0; i < siz; ++i)
    {
        if ((*this)[i]->Includes(domain))
            return true;
    }

    integer_type *pnModulus1 = domain.m_aspect.m_pnModulus;
    integer_type *pnResidue1 = domain.m_aspect.m_pnResidue;
    if (pnModulus1 == NULL)
    {
        return false;
    }

    for (i = 0; i < siz; ++i)
    {
        Domain& d_i = *(*this)[i].get();
        Aspect& a_i = d_i.m_aspect;
        if (a_i.MustBePrime() && !domain.MustBePrime())
        {
            continue;
        }

        if (a_i.MustBeComposite() && !domain.MustBeComposite())
        {
            continue;
        }

        if (!d_i.m_ranges.Includes(domain.m_ranges))
        {
            continue;
        }

        integer_type *pnModulus2 = a_i.m_pnModulus;
        integer_type *pnResidue2 = a_i.m_pnResidue;

        integer_type m1 = *pnModulus1;
        integer_type r1 = *pnResidue1;
        integer_type m2 = *pnModulus2;
        integer_type r2 = *pnResidue2;
        integer_type y, M, N;
        if (!katayama_qz(y, M, N, r1, m1, r2, m2))
        {
            continue;
        }

        bool flag = false;
        integer_type ysave = y % m2;
        do
        {
            flag = false;
            for (size_t j = 0; j < siz; ++j)
            {
                Domain& d_j = *(*this)[j].get();
                Aspect& a_j = d_j.m_aspect;
                if (a_j.MustBePrime() && !domain.MustBePrime())
                {
                    continue;
                }

                if (a_j.MustBeComposite() && !domain.MustBeComposite())
                {
                    continue;
                }

                if (a_j.m_pnModulus &&
                    *a_j.m_pnModulus == m2 &&
                    *a_j.m_pnResidue == y)
                {
                    if (d_j.m_ranges.Includes(domain.m_ranges))
                    {
                        flag = true;
                        break;
                    }
                }
            }
            if (!flag)
            {
                Aspect a(&m2, &y, domain.m_aspect.MustBePrime(),
                                  domain.m_aspect.MustBeComposite());
                Domain d(a, domain.m_ranges);
                if (!Includes(d))
                {
                    return false;
                }
                flag = true;
            }
            y += m1;
            y %= m2;
        } while (ysave != y);
        if (flag)
            return true;
    }

    return false;
}

bool Domains::Includes(const Domains& domains) const
{
    if (domains.empty())
        return true;

    if (empty())
        return false;

    size_t i, siz = domains.size();
    for (i = 0; i < siz; ++i)
    {
        if (Includes(*domains[i].get()))
            return true;
    }
    return false;
}

bool Domains::Contains(const number_type& value) const
{
    size_t i, siz = size();
    for (i = 0; i < siz; ++i)
    {
        if ((*this)[i]->Contains(value))
            return true;
    }
    return false;
}

void Domains::Intersect(const Range& r)
{
    size_t i, siz = size();
    for (i = 0; i < siz; ++i)
    {
        ((*this)[i]).get()->Intersect(r);
    }
}

void Domains::Intersect(const Domain& domain)
{
    Domains *d = Domains::Intersect(this, &domain);
    *this = *d;
    delete d;
}

void Domains::Intersect(const Domains& domains)
{
    Domains *d = Domains::Intersect(this, &domains);
    *this = *d;
    delete d;
}

void Domains::Union(const Domain& domain)
{
    Domains *d = Domains::Union(this, &domain);
    *this = *d;
    delete d;
}

void Domains::Union(const Domains& domains)
{
    Domains *d = Domains::Union(this, &domains);
    *this = *d;
    delete d;
}

bool Domains::GetValues(std::vector<number_type>& values) const
{
    values.clear();
    if (empty())
        return true;

    std::vector<number_type> v;
    size_t i, siz = size();
    for (i = 0; i < siz; ++i)
    {
        if ((*this)[i]->GetValues(v))
        {
            values.insert(values.end(), v.begin(), v.end());
        }
        else
        {
            values.clear();
            return false;
        }
    }

    std::sort(values.begin(), values.end());
    std::unique(values.begin(), values.end());
    return true;
}

void Domains::FixWider()
{
    size_t i, siz = size();
    for (i = 0; i < siz; ++i)
    {
        Domain& domain = *((*this)[i]).get();
        domain.FixWider();
        domain.Optimize();
    }

    bool flag = false;
    for (i = 0; i < siz; ++i)
    {
        Domain& domain1 = *((*this)[i]).get();
        for (size_t j = 0; j < siz; ++j)
        {
            if (i == j)
                continue;

            Domain& domain2 = *((*this)[j]).get();
            if (domain1.m_aspect.Includes(domain2.m_aspect))
            {
                domain2.m_ranges.Union(domain1.m_ranges);
                flag = true;
            }
        }
    }

    if (flag)
    {
        for (i = 0; i < siz; ++i)
        {
            Domain& domain = *((*this)[i]).get();
            domain.FixWider();
            domain.Optimize();
        }
    }
}

void Domains::FixNarrower()
{
    size_t i, j, siz;

retry:
    siz = size();
    for (i = 0; i < siz; ++i)
    {
        Domain& domain1 = *((*this)[i]).get();
        for (j = 0; j < siz; ++j)
        {
            if (i == j)
                continue;

            Domain& domain2 = *((*this)[j]).get();
            domain1.FixBeforeIncludes(domain2);
            if (domain1.Includes(domain2))
            {
                erase(begin() + j);
                goto retry;
            }
        }
    }

    siz = size();
    for (i = 0; i < siz; ++i)
    {
        Domain& domain = *((*this)[i]).get();
        domain.FixNarrower();
    }
}

void Domains::FixBeforeIncludes(Domain& domain)
{
    FixWider();
    domain.FixNarrower();
}

void Domains::FixBeforeIncludes(Domains& domains)
{
    FixWider();
    domains.FixNarrower();
}

number_type *Domains::GetLBound(bool& has_min) const
{
    size_t i, siz = size();
    has_min = false;
    if (siz == 0)
    {
        return NULL;
    }

    number_type *value;
    for (i = 0; i < siz; ++i)
    {
        bool flag;
        value = ((*this)[i])->GetLBound(flag);
        if (value == NULL)
            return NULL;
    }

    bool flag;
    value = ((*this)[0])->GetLBound(flag);
    has_min = flag;
    for (i = 1; i < siz; ++i)
    {
        number_type *pnLBound = ((*this)[i])->GetLBound(flag);
        if (*value > *pnLBound)
        {
            value = pnLBound;
            has_min = flag;
        }
        else if (*value == *pnLBound)
        {
            if (flag)
                has_min = true;
        }
    }
    return value;
}

number_type *Domains::GetUBound(bool& has_max) const
{
    size_t i, siz = size();
    has_max = false;
    if (siz == 0)
    {
        return NULL;
    }

    number_type *value;
    for (i = 0; i < siz; ++i)
    {
        bool flag;
        value = ((*this)[i])->GetUBound(flag);
        if (value == NULL)
            return NULL;
    }

    bool flag;
    value = ((*this)[0])->GetUBound(flag);
    has_max = flag;
    for (i = 1; i < siz; ++i)
    {
        number_type *pnUBound = ((*this)[i])->GetUBound(flag);
        if (*value < *pnUBound)
        {
            value = pnUBound;
            has_max = flag;
        }
        else if (*value == *pnUBound)
        {
            if (flag)
                has_max = true;
        }
    }
    return value;
}

void Domains::Optimize()
{
    if (empty())
    {
        clear();
        return;
    }

retry1:
    // remove empty domain
    size_t siz = size();
    for (size_t i = 0; i < siz; ++i)
    {
        if ((*this)[i]->empty())
        {
            erase(begin() + i);
            goto retry1;
        }
    }

retry2:
    siz = size();
    for (size_t i = 0; i < siz; ++i)
    {
        for (size_t j = 0; j < siz; ++j)
        {
            if (i == j)
                continue;

            Domain& d1 = *(*this)[i].get();
            Domain& d2 = *(*this)[j].get();
            Aspect& a1 = d1.m_aspect;
            Aspect& a2 = d2.m_aspect;
            if (a1 != a2)
                continue;

            // calculate the union
            number_type *pnLBound = NULL, *pnUBound = NULL;
            bool has_min = false, has_max = false;
            bool has_min1, has_max1, has_min2, has_max2;

            number_type *pnLBound1 = d1.GetLBound(has_min1);
            number_type *pnLBound2 = d2.GetLBound(has_min2);
            if (pnLBound1 && pnLBound2)
            {
                if (*pnLBound1 < *pnLBound2)
                {
                    pnLBound = pnLBound1;
                    has_min = has_min1;
                }
                else if (*pnLBound1 > *pnLBound2)
                {
                    pnLBound = pnLBound2;
                    has_min = has_min2;
                }
                else
                {
                    pnLBound = pnLBound2;
                    has_min = has_min1 || has_min2;
                }
            }

            number_type *pnUBound1 = d1.GetUBound(has_max1);
            number_type *pnUBound2 = d2.GetUBound(has_max2);
            if (pnUBound1 && pnUBound2)
            {
                if (*pnUBound1 < *pnUBound2)
                {
                    pnUBound = pnUBound2;
                    has_max = has_max2;
                }
                else if (*pnUBound1 > *pnUBound2)
                {
                    pnUBound = pnUBound1;
                    has_max = has_max1;
                }
                else
                {
                    pnUBound = pnUBound2;
                    has_max = has_max1 || has_max2;
                }
            }

            // unite
            if (pnLBound && pnUBound)
            {
                if (*pnLBound < *pnUBound)
                {
                    Domain *d = new Domain(a1, has_min, has_max, pnLBound, pnUBound);
                    (*this)[i] = shared_ptr<Domain>(d);
                    erase(begin() + j);
                    goto retry2;
                }
                else if (*pnLBound == *pnUBound && has_min && has_max)
                {
                    Domain *d = new Domain(a1, *pnLBound);
                    (*this)[i] = shared_ptr<Domain>(d);
                    erase(begin() + j);
                    goto retry2;
                }
            }
            else if (pnLBound || pnUBound)
            {
                Domain *d = new Domain(a1, has_min, has_max, pnLBound, pnUBound);
                (*this)[i] = shared_ptr<Domain>(d);
                erase(begin() + j);
                goto retry2;
            }
            else
            {
                continue;
            }

            // unite
            if (pnLBound1 && pnUBound2 && *pnLBound1 == *pnUBound2 &&
                (has_min1 || has_max2))
            {
                Domain *d = new Domain(a1, has_min2, has_max1, pnLBound2, pnUBound1);
                (*this)[i] = shared_ptr<Domain>(d);
                erase(begin() + j);
                goto retry2;
            }
            else if (pnUBound1 && pnLBound2 && *pnUBound1 == *pnLBound2 &&
                     (has_max1 || has_min2))
            {
                Domain *d = new Domain(a1, has_min1, has_max2, pnLBound1, pnUBound2);
                (*this)[i] = shared_ptr<Domain>(d);
                erase(begin() + j);
                goto retry2;
            }
        }
    }

    if (entire())
    {
        Domains *d = Domains::Whole();
        *this = *d;
        delete d;
    }
}

/*static*/ Domains *Domains::Intersect(const Domains *domains, const Domain *domain)
{
    Domains *pDomains = new Domains();
    size_t i, siz = domains->size();
    for (i = 0; i < siz; ++i)
    {
        Domain *d = Domain::Intersect((*domains)[i].get(), domain);
        if (d)
            pDomains->add(d);
    }
    return pDomains;
}

/*static*/ Domains *Domains::Intersect(const Domains *d1, const Domains *d2)
{
    Domains *pDomains  = new Domains();
    size_t i, siz1 = d1->size();
    for (i = 0; i < siz1; ++i)
    {
        Domains *d = Domains::Intersect(d2, ((*d1)[i]).get());
        if (d->size())
            pDomains->insert(pDomains->end(), d->begin(), d->end());
        delete d;
    }
    pDomains->Optimize();
    return pDomains;
}

/*static*/ Domains *Domains::Union(const Domains *domains, const Domain *domain)
{
    Domains *d = new Domains(*domains);
    d->add(new Domain(*domain));
    d->FixWider();
    d->Optimize();
    d->FixNarrower();
    return d;
}

/*static*/ Domains *Domains::Union(const Domains *d1, const Domains *d2)
{
    Domains *d = new Domains(*d1);
    d->insert(d->end(), d2->begin(), d2->end());
    d->FixWider();
    d->Optimize();
    d->FixNarrower();
    return d;
}

/*static*/ Domains *Domains::Whole(Domain::Type type/* = Domain::REAL*/)
{
    Domains *domains = new Domains;
    Domain *d = Domain::Whole(type);
    domains->add(d);
    return domains;
}

bool Domains::Equal(const Domains& d) const
{
    Domains d1(*this);
    Domains d2(d);
    d1.FixBeforeIncludes(d2);
    if (!d1.Includes(d2))
        return false;
    d2.FixBeforeIncludes(d1);
    if (!d2.Includes(d1))
        return false;
    return true;
}

////////////////////////////////////////////////////////////////////////////

} // namespace Ndrr1D

////////////////////////////////////////////////////////////////////////////

#ifdef UNITTEST
    int main(void)
    {
        using namespace Ndrr1D;
        number_type n1;
        number_type n2;
        integer_type i1;
        integer_type i2;
        number_type *pnLBound, *pnUBound;
        bool has_min, has_max;

        //
        // FixWider test
        //
        i1 = 2;
        i2 = 1;
        n1 = 1;
        n2 = 5;
        Domain domain1(Aspect(&i1, &i2),
                       Ranges(true, true, &n1, &n2));
        domain1.FixWider();
        std::cout << domain1 << std::endl;
        pnLBound = domain1.GetLBound(has_min);
        assert(*pnLBound == -1);
        assert(!has_min);
        pnUBound = domain1.GetUBound(has_max);
        assert(*pnUBound == 7);
        assert(!has_max);

        Domain domain2(Aspect(&i1, &i2),
                       Ranges(false, false, &n1, &n2));
        domain2.FixWider();
        std::cout << domain2 << std::endl;
        pnLBound = domain2.GetLBound(has_min);
        assert(*pnLBound == 1);
        assert(!has_min);
        pnUBound = domain2.GetUBound(has_max);
        assert(*pnUBound == 5);
        assert(!has_max);

        n1 = 2;
        n2 = 6;
        Domain domain3(Aspect(&i1, &i2),
                       Ranges(true, true, &n1, &n2));
        domain3.FixWider();
        std::cout << domain3 << std::endl;
        pnLBound = domain3.GetLBound(has_min);
        assert(*pnLBound == 1);
        assert(!has_min);
        pnUBound = domain3.GetUBound(has_max);
        assert(*pnUBound == 7);
        assert(!has_max);

        Domain domain4(Aspect(&i1, &i2),
                       Ranges(false, false, &n1, &n2));
        domain4.FixWider();
        std::cout << domain4 << std::endl;
        pnLBound = domain4.GetLBound(has_min);
        assert(*pnLBound == 1);
        assert(!has_min);
        pnUBound = domain4.GetUBound(has_max);
        assert(*pnUBound == 7);
        assert(!has_max);

        i1 = 2;
        i2 = 0;
        n1 = 1;
        n2 = 5;
        Domain domain5(Aspect(&i1, &i2),
                       Ranges(true, true, &n1, &n2));
        domain5.FixWider();
        std::cout << domain5 << std::endl;
        pnLBound = domain5.GetLBound(has_min);
        assert(*pnLBound == 0);
        assert(!has_min);
        pnUBound = domain5.GetUBound(has_max);
        assert(*pnUBound == 6);
        assert(!has_max);

        Domain domain6(Aspect(&i1, &i2),
                       Ranges(false, false, &n1, &n2));
        domain6.FixWider();
        std::cout << domain6 << std::endl;
        pnLBound = domain6.GetLBound(has_min);
        assert(*pnLBound == 0);
        assert(!has_min);
        pnUBound = domain6.GetUBound(has_max);
        assert(*pnUBound == 6);
        assert(!has_max);

        n1 = 2;
        n2 = 6;
        Domain domain7(Aspect(&i1, &i2),
                       Ranges(true, true, &n1, &n2));
        domain7.FixWider();
        std::cout << domain7 << std::endl;
        pnLBound = domain7.GetLBound(has_min);
        assert(*pnLBound == 0);
        assert(!has_min);
        pnUBound = domain7.GetUBound(has_max);
        assert(*pnUBound == 8);
        assert(!has_max);

        Domain domain8(Aspect(&i1, &i2),
                       Ranges(false, false, &n1, &n2));
        domain8.FixWider();
        std::cout << domain8 << std::endl;
        pnLBound = domain8.GetLBound(has_min);
        assert(*pnLBound == 2);
        assert(!has_min);
        pnUBound = domain8.GetUBound(has_max);
        assert(*pnUBound == 6);
        assert(!has_max);

        //
        // FixNarrower test
        //
        i1 = 2;
        i2 = 1;
        n1 = 1;
        n2 = 5;
        Domain domain9(Aspect(&i1, &i2),
                       Ranges(true, true, &n1, &n2));
        domain9.FixNarrower();
        std::cout << domain9 << std::endl;
        pnLBound = domain9.GetLBound(has_min);
        assert(*pnLBound == 1);
        assert(has_min);
        pnUBound = domain9.GetUBound(has_max);
        assert(*pnUBound == 5);
        assert(has_max);

        Domain domain10(Aspect(&i1, &i2),
                       Ranges(false, false, &n1, &n2));
        domain10.FixNarrower();
        std::cout << domain10 << std::endl;
        pnLBound = domain10.GetLBound(has_min);
        assert(*pnLBound == 3);
        assert(has_min);
        pnUBound = domain10.GetUBound(has_max);
        assert(*pnUBound == 3);
        assert(has_max);

        n1 = 2;
        n2 = 6;
        Domain domain11(Aspect(&i1, &i2),
                        Ranges(true, true, &n1, &n2));
        domain11.FixNarrower();
        std::cout << domain11 << std::endl;
        pnLBound = domain11.GetLBound(has_min);
        assert(*pnLBound == 3);
        assert(has_min);
        pnUBound = domain11.GetUBound(has_max);
        assert(*pnUBound == 5);
        assert(has_max);

        Domain domain12(Aspect(&i1, &i2),
                        Ranges(false, false, &n1, &n2));
        domain12.FixNarrower();
        std::cout << domain12 << std::endl;
        pnLBound = domain12.GetLBound(has_min);
        assert(*pnLBound == 3);
        assert(has_min);
        pnUBound = domain12.GetUBound(has_max);
        assert(*pnUBound == 5);
        assert(has_max);

        i1 = 2;
        i2 = 0;
        n1 = 1;
        n2 = 5;
        Domain domain13(Aspect(&i1, &i2),
                        Ranges(true, true, &n1, &n2));
        domain13.FixNarrower();
        std::cout << domain13 << std::endl;
        pnLBound = domain13.GetLBound(has_min);
        assert(*pnLBound == 2);
        assert(has_min);
        pnUBound = domain13.GetUBound(has_max);
        assert(*pnUBound == 4);
        assert(has_max);

        Domain domain14(Aspect(&i1, &i2),
                        Ranges(false, false, &n1, &n2));
        domain14.FixNarrower();
        std::cout << domain14 << std::endl;
        pnLBound = domain14.GetLBound(has_min);
        assert(*pnLBound == 2);
        assert(has_min);
        pnUBound = domain14.GetUBound(has_max);
        assert(*pnUBound == 4);
        assert(has_max);

        n1 = 2;
        n2 = 6;
        Domain domain15(Aspect(&i1, &i2),
                        Ranges(true, true, &n1, &n2));
        domain15.FixNarrower();
        std::cout << domain15 << std::endl;
        pnLBound = domain15.GetLBound(has_min);
        assert(*pnLBound == 2);
        assert(has_min);
        pnUBound = domain15.GetUBound(has_max);
        assert(*pnUBound == 6);
        assert(has_max);

        Domain domain16(Aspect(&i1, &i2),
                        Ranges(false, false, &n1, &n2));
        domain16.FixNarrower();
        std::cout << domain16 << std::endl;
        pnLBound = domain16.GetLBound(has_min);
        assert(*pnLBound == 4);
        assert(has_min);
        pnUBound = domain16.GetUBound(has_max);
        assert(*pnUBound == 4);
        assert(has_max);

        //
        // Range::Includes test
        //
        {
            n1 = 0;
            n2 = 0;
            Range r1(n1);
            n1 = 0;
            n2 = 0;
            Range r2(n1);
            assert(r1.Equal(r2));

            n1 = 0;
            n2 = 1;
            Range r3(true, true, &n1, &n2);
            Range r4(false, true, &n1, &n2);
            Range r5(true, false, &n1, &n2);
            assert(r3.Includes(r1));
            assert(r3.Includes(r4));
            assert(r3.Includes(r5));
            assert(!r1.Includes(r3));
            assert(!r4.Includes(r3));
            assert(!r5.Includes(r3));
        }
        {
            n1 = 0;
            n2 = 1;
            Range r1(false, true, &n1, &n2);
            n1 = 1;
            n2 = 2;
            Range r2(true, false, &n1, &n2);
            Ranges ranges1;
            ranges1.add(new Range(r1));
            ranges1.add(new Range(r2));
            ranges1.Optimize();
            n1 = 0;
            n2 = 2;
            Range r3(false, false, &n1, &n2);
            assert(ranges1.Includes(r3));
        }
        {
            n1 = 0;
            n2 = 1;
            Range r1(false, true, &n1, &n2);
            n1 = 1;
            n2 = 2;
            Range r2(false, false, &n1, &n2);
            Ranges ranges1;
            ranges1.add(new Range(r1));
            ranges1.add(new Range(r2));
            ranges1.Optimize();
            n1 = 0;
            n2 = 2;
            Ranges ranges2(true, true, &n1, &n2);
            assert(ranges2.Includes(ranges1));
            assert(!ranges1.Includes(ranges2));
        }

        //
        // Domain::Includes and Domains::Includes test
        //
        {
            Range *r;

            i1 = 2;
            i2 = 1;
            n1 = 0;
            Domain *domain1_1 = new Domain(Aspect(&i1, &i2));
            r = new Range(true, false, &n1, NULL);
            domain1_1->m_ranges.Intersect(*r);
            delete r;

            i1 = 4;
            i2 = 1;
            n1 = 1;
            Domain *domain2_1 = new Domain(Aspect(&i1, &i2));
            r = new Range(true, false, &n1, NULL);
            domain2_1->m_ranges.Intersect(*r);
            delete r;

            i1 = 4;
            i2 = 3;
            n1 = 3;
            Domain *domain2_2 = new Domain(Aspect(&i1, &i2));
            r = new Range(true, false, &n1, NULL);
            domain2_2->m_ranges.Intersect(*r);
            delete r;

            Domains domains1, domains2;
            domains1.add(domain1_1);
            domains2.add(domain2_1);
            domains2.add(domain2_2);

            domains1.FixBeforeIncludes(domains2);
            std::cout << domains1 << std::endl;
            assert(domains1.Includes(domains2));

            domains2.FixBeforeIncludes(domains1);
            std::cout << domains2 << std::endl;
            assert(domains2.Includes(domains1));
        }
        {
            Range *r;

            i1 = 3;
            i2 = 1;
            n1 = 0;
            Domain *domain1_1 = new Domain(Aspect(&i1, &i2, true));
            r = new Range(true, false, &n1, NULL);
            domain1_1->m_ranges.Intersect(*r);
            delete r;

            i1 = 9;
            i2 = 1;
            n1 = 0;
            Domain *domain2_1 = new Domain(Aspect(&i1, &i2, true));
            r = new Range(true, false, &n1, NULL);
            domain2_1->m_ranges.Intersect(*r);
            delete r;

            i1 = 9;
            i2 = 4;
            n1 = 0;
            Domain *domain2_2 = new Domain(Aspect(&i1, &i2, true));
            r = new Range(true, false, &n1, NULL);
            domain2_2->m_ranges.Intersect(*r);
            delete r;

            i1 = 9;
            i2 = 7;
            n1 = 0;
            Domain *domain2_3 = new Domain(Aspect(&i1, &i2, true));
            r = new Range(true, false, &n1, NULL);
            domain2_3->m_ranges.Intersect(*r);
            delete r;

            Domains domains1, domains2;
            domains1.add(domain1_1);
            domains2.add(domain2_1);
            domains2.add(domain2_2);
            domains2.add(domain2_3);

            domains1.FixBeforeIncludes(domains2);
            std::cout << domains1 << std::endl;
            assert(domains1.Includes(domains2));

            domains2.FixBeforeIncludes(domains1);
            std::cout << domains2 << std::endl;
            assert(domains2.Includes(domains1));
        }
        {
            Range *r;

            i1 = 2;
            i2 = 0;
            n1 = 0;
            Domain *domain1_1 = new Domain(Aspect(&i1, &i2));
            r = new Range(true, false, &n1, NULL);
            domain1_1->m_ranges.Intersect(*r);
            delete r;

            i1 = 4;
            i2 = 0;
            n1 = 0;
            Domain *domain2_1 = new Domain(Aspect(&i1, &i2));
            r = new Range(true, false, &n1, NULL);
            domain2_1->m_ranges.Intersect(*r);
            delete r;

            i1 = 8;
            i2 = 2;
            n1 = 0;
            Domain *domain3_1 = new Domain(Aspect(&i1, &i2));
            r = new Range(true, false, &n1, NULL);
            domain3_1->m_ranges.Intersect(*r);
            delete r;

            i1 = 8;
            i2 = 6;
            n1 = 0;
            Domain *domain3_2 = new Domain(Aspect(&i1, &i2));
            r = new Range(true, false, &n1, NULL);
            domain3_2->m_ranges.Intersect(*r);
            delete r;

            Domains domains1, domains2;
            domains1.add(domain1_1);
            domains2.add(domain2_1);
            domains2.add(domain3_1);
            domains2.add(domain3_2);

            domains1.FixBeforeIncludes(domains2);
            std::cout << domains1 << std::endl;
            std::cout << domains2 << std::endl;
            assert(domains1.Includes(domains2));
            domains2.FixBeforeIncludes(domains1);
            std::cout << domains1 << std::endl;
            std::cout << domains2 << std::endl;
            assert(domains2.Includes(domains1));
            assert(!domains1.empty());
            assert(!domains2.empty());
        }

        return 0;
    }
#endif  // def UNITTEST

////////////////////////////////////////////////////////////////////////////
