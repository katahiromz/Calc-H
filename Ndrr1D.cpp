/////////////////////////////////////////////////////////////////////////////
// Ndrr1D.cpp --- The one-dimensional Numeric Domains/Ranges Resolver
// See file "ReadMe.txt" and "License.txt".
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

namespace Ndrr1D
{

////////////////////////////////////////////////////////////////////////////
// public functions

bool IsPositiveNumber(const DRR1D_VALUE& value)
{
    return value > 0;
}

bool IsNegativeNumber(const DRR1D_VALUE& value)
{
    return value < 0;
}

bool IsNaturalNumber(const DRR1D_VALUE& value)
{
    #ifdef DRR1D_USES_PMPNUMBER
        return value >= 0 && (value % 1).is_zero();
    #else
        return value >= 0 && std::fmod(value, 1) == 0;
    #endif
}

bool IsRegularNumber(const DRR1D_VALUE& value)
{
    #ifdef DRR1D_USES_PMPNUMBER
        return (value % 1).is_zero();
    #else
        return std::fmod(value, 1) == 0;
    #endif
}

bool IsEvenNumber(const DRR1D_VALUE& value)
{
    #ifdef DRR1D_USES_PMPNUMBER
        return (value % 2).is_zero();
    #else
        return std::fmod(value, 2) == 0;
    #endif
}

bool IsOddNumber(const DRR1D_VALUE& value)
{
    #ifdef DRR1D_USES_PMPNUMBER
        DRR1D_VALUE v = value % 2;
        return v == 1 || v == -1;
    #else
        DRR1D_VALUE v = std::fmod(value, 2);
        return v == 1 || v == -1;
    #endif
}

bool IsPrimeNumber(const DRR1D_VALUE& value)
{
    if (value < 2)
        return false;

    if (value == 2)
        return true;

    #ifdef DRR1D_USES_PMPNUMBER
        if ((value % 2).is_zero())
            return false;

        for (DRR1D_VALUE i = 3; i < value; i += 2)
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

DRR1D_VALUE GauseModFloor(DRR1D_VALUE d, DRR1D_VALUE mod)
{
    if (mod < 0)
        mod = -mod;

    DRR1D_VALUE v;
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

DRR1D_VALUE GauseModCeil(DRR1D_VALUE d, DRR1D_VALUE mod)
{
    if (mod < 0)
        mod = -mod;

    DRR1D_VALUE v;
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

////////////////////////////////////////////////////////////////////////////
// Range

/*static*/ Range *Range::Intersect(const Range *r1, const Range *r2)
{
    if (r1->empty() || r2->empty())
        return NULL;

    bool has_min = false;
    DRR1D_VALUE *pnLBound = NULL;
    if (r1->m_pnLBound && r2->m_pnLBound)
    {
        if (*r1->m_pnLBound < *r2->m_pnLBound)
        {
            pnLBound = new DRR1D_VALUE(*r2->m_pnLBound);
            has_min = r2->m_has_min;
        }
        else if (*r1->m_pnLBound > *r2->m_pnLBound)
        {
            pnLBound = new DRR1D_VALUE(*r1->m_pnLBound);
            has_min = r1->m_has_min;
        }
        else
        {
            pnLBound = new DRR1D_VALUE(*r1->m_pnLBound);
            has_min = r1->m_has_min && r2->m_has_min;
        }
    }
    else if (r1->m_pnLBound)
    {
        pnLBound = new DRR1D_VALUE(*r1->m_pnLBound);
        has_min = r1->m_has_min;
    }
    else if (r2->m_pnLBound)
    {
        pnLBound = new DRR1D_VALUE(*r2->m_pnLBound);
        has_min = r2->m_has_min;
    }

    bool has_max = false;
    DRR1D_VALUE *pnUBound = NULL;
    if (r1->m_pnUBound && r2->m_pnUBound)
    {
        if (*r1->m_pnUBound < *r2->m_pnUBound)
        {
            pnUBound = new DRR1D_VALUE(*r1->m_pnUBound);
            has_max = r1->m_has_max;
        }
        else if (*r1->m_pnUBound > *r2->m_pnUBound)
        {
            pnUBound = new DRR1D_VALUE(*r2->m_pnUBound);
            has_max = r2->m_has_max;
        }
        else
        {
            pnUBound = new DRR1D_VALUE(*r2->m_pnUBound);
            has_max = r1->m_has_max && r2->m_has_max;
        }
    }
    else if (r1->m_pnUBound)
    {
        pnUBound = new DRR1D_VALUE(*r1->m_pnUBound);
        has_max = r1->m_has_max;
    }
    else if (r2->m_pnUBound)
    {
        pnUBound = new DRR1D_VALUE(*r2->m_pnUBound);
        has_max = r2->m_has_max;
    }

    Range *r = new Range;
    r->m_has_min = has_min;
    r->m_has_max = has_max;
    r->m_pnLBound = pnLBound;
    r->m_pnUBound = pnUBound;
    if (r->empty())
    {
        delete r;
        r = NULL;
    }
    return r;
}

bool Range::Includes(const Range& range) const
{
    if (range.empty())
        return true;

    if (m_pnLBound && range.m_pnLBound)
    {
        if (*m_pnLBound > *range.m_pnLBound)
            return false;
        if (*m_pnLBound == *range.m_pnLBound && m_has_min < range.m_has_min)
            return false;
    }
    else if (m_pnLBound)
    {
        return false;
    }

    if (m_pnUBound && range.m_pnUBound)
    {
        if (*m_pnUBound < *range.m_pnUBound)
            return false;
        if (*m_pnUBound == *range.m_pnUBound && m_has_max < range.m_has_max)
            return false;
    }
    else if (m_pnUBound)
    {
        return false;
    }

    return true;
}

bool Range::Contains(DRR1D_VALUE value) const
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

void Range::clear()
{
    delete m_pnLBound;
    m_pnLBound = new DRR1D_VALUE(1);
    delete m_pnUBound;
    m_pnUBound = new DRR1D_VALUE(0);
}

void Range::Intersect(const Range& r)
{
    Range *range = Range::Intersect(this, &r);
    if (range)
    {
        *this = *range;
        delete range;
    }
    else
        clear();
}

////////////////////////////////////////////////////////////////////////////
// Ranges

/*static*/ Ranges *Ranges::Optimize(const Ranges *ranges)
{
    Ranges *pRanges = new Ranges(*ranges);

    // remove empty ranges
retry1:
    std::size_t siz = pRanges->size();
    for (std::size_t i = 0; i < siz; ++i)
    {
        if ((*pRanges)[i]->empty())
        {
            pRanges->erase(pRanges->begin() + i);
            goto retry1;
        }
    }

retry2:
    siz = pRanges->size();
    for (std::size_t i = 0; i < siz - 1; ++i)
    {
        for (std::size_t j = i + 1; j < siz; ++j)
        {
            Range *range = Range::Intersect((*pRanges)[i].get(), (*pRanges)[j].get());
            if (range)
            {
                delete range;

                bool has_min = false, has_max = false;;
                DRR1D_VALUE *pnLBound = NULL, *pnUBound = NULL;

                shared_ptr<Range> r1 = (*pRanges)[i];
                shared_ptr<Range> r2 = (*pRanges)[j];
                if (r1->m_pnLBound && r2->m_pnLBound)
                {
                    if (*r1->m_pnLBound < *r2->m_pnLBound)
                    {
                        pnLBound = new DRR1D_VALUE(*r1->m_pnLBound);
                        has_min = r1->m_has_min;
                    }
                    else if (*r1->m_pnLBound > *r2->m_pnLBound)
                    {
                        pnLBound = new DRR1D_VALUE(*r2->m_pnLBound);
                        has_min = r2->m_has_min;
                    }
                    else
                    {
                        pnLBound = new DRR1D_VALUE(*r2->m_pnLBound);
                        has_min = r1->m_has_min || r2->m_has_min;
                    }
                }
                if (r1->m_pnUBound && r2->m_pnUBound)
                {
                    if (*r1->m_pnUBound < *r2->m_pnUBound)
                    {
                        pnUBound = new DRR1D_VALUE(*r2->m_pnUBound);
                        has_max = r2->m_has_max;
                    }
                    else if (*r1->m_pnUBound > *r2->m_pnUBound)
                    {
                        pnUBound = new DRR1D_VALUE(*r1->m_pnUBound);
                        has_max = r1->m_has_max;
                    }
                    else
                    {
                        pnUBound = new DRR1D_VALUE(*r2->m_pnUBound);
                        has_max = r1->m_has_max || r2->m_has_max;
                    }
                }

                Range *r = new Range;
                r->m_has_min = has_min;
                r->m_has_max = has_max;
                r->m_pnLBound = pnLBound;
                r->m_pnUBound = pnUBound;
                (*pRanges)[i] = shared_ptr<Range>(r);
                pRanges->erase(pRanges->begin() + j);
                goto retry2;
            }
            else if (
                (*pRanges)[i]->m_pnUBound &&
                (*pRanges)[j]->m_pnLBound &&
                *(*pRanges)[i]->m_pnUBound == *(*pRanges)[j]->m_pnLBound &&
                ((*pRanges)[i]->m_has_max || (*pRanges)[j]->m_has_min))
            {
                Range *r = new Range;
                r->m_has_min = (*pRanges)[i]->m_has_min;
                r->m_has_max = (*pRanges)[j]->m_has_max;
                if ((*pRanges)[i]->m_pnLBound)
                    r->m_pnLBound = new DRR1D_VALUE(*(*pRanges)[i]->m_pnLBound);
                if ((*pRanges)[j]->m_pnUBound)
                    r->m_pnUBound = new DRR1D_VALUE(*(*pRanges)[j]->m_pnUBound);
                (*pRanges)[i] = shared_ptr<Range>(r);
                pRanges->erase(pRanges->begin() + j);
                goto retry2;
            }
            else if (
                (*pRanges)[j]->m_pnUBound &&
                (*pRanges)[i]->m_pnLBound &&
                *(*pRanges)[j]->m_pnUBound == *(*pRanges)[i]->m_pnLBound &&
                ((*pRanges)[j]->m_has_max || (*pRanges)[i]->m_has_min))
            {
                Range *r = new Range;
                r->m_has_min = (*pRanges)[j]->m_has_min;
                r->m_has_max = (*pRanges)[i]->m_has_max;
                if ((*pRanges)[j]->m_pnLBound)
                    r->m_pnLBound = new DRR1D_VALUE(*(*pRanges)[j]->m_pnLBound);
                if ((*pRanges)[i]->m_pnUBound)
                    r->m_pnUBound = new DRR1D_VALUE(*(*pRanges)[i]->m_pnUBound);
                (*pRanges)[i] = shared_ptr<Range>(r);
                pRanges->erase(pRanges->begin() + j);
                goto retry2;
            }
        }
    }

    // check existence of the whole range
    for (std::size_t i = 0; i < siz; ++i)
    {
        if ((*pRanges)[i]->m_pnLBound == NULL &&
            (*pRanges)[i]->m_pnUBound == NULL)
        {
            Range *r = Range::Whole();
            pRanges->clear();
            pRanges->push_back(shared_ptr<Range>(r));
            break;
        }
    }

    return pRanges;
}

/*static*/ Ranges *Ranges::Intersect(const Ranges *ranges, const Range *range)
{
    Ranges *pRanges  = new Ranges();
    std::size_t i, siz = ranges->size();
    for (i = 0; i < siz; ++i)
    {
        Range *r = Range::Intersect((*ranges)[i].get(), range);
        if (r)
            pRanges->push_back(shared_ptr<Range>(r));
    }
    return pRanges;
}

/*static*/ Ranges *Ranges::Intersect(const Ranges *r1, const Ranges *r2)
{
    Ranges *pRanges  = new Ranges();
    std::size_t i, siz1 = r1->size();
    for (i = 0; i < siz1; ++i)
    {
        Ranges *r = Ranges::Intersect(r2, ((*r1)[i]).get());
        if (r->size())
            pRanges->insert(pRanges->end(), r->begin(), r->end());
        delete r;
    }
    return pRanges;
}

bool Ranges::Includes(const Ranges& ranges) const
{
    std::size_t i, j, siz1 = size(), siz2 = ranges.size();
    for (j = 0; j < siz2; ++j)
    {
        bool flag = false;
        for (i = 0; i < siz1; ++i)
        {
            if ((*this)[i]->Includes(*ranges[j].get()))
            {
                flag = true;
                break;
            }
        }
        if (!flag)
            return false;
    }
    return true;
}

bool Ranges::Contains(DRR1D_VALUE value) const
{
    std::size_t i, siz = size();
    for (i = 0; i < siz; ++i)
    {
        if ((*this)[i]->Contains(value))
            return true;
    }
    return false;
}

void Ranges::Optimize()
{
    Ranges *ranges = Ranges::Optimize(this);
    *this = *ranges;
    delete ranges;
}

/*static*/ Ranges *Ranges::Union(const Ranges *r1, const Ranges *r2)
{
    Ranges *r = new Ranges(*r1);
    r->insert(r->end(), r2->begin(), r2->end());
    Ranges *ranges = Ranges::Optimize(r);
    delete r;
    return ranges;
}

/*static*/ Ranges *Ranges::Whole()
{
    Ranges *ranges = new Ranges;
    Range *r = Range::Whole();
    ranges->push_back(shared_ptr<Range>(r));
    return ranges;
}

void Ranges::Union(const Ranges& ranges)
{
    Ranges *r = Ranges::Union(this, &ranges);
    *this = *r;
    delete r;
}

void Ranges::Intersect(const Range& range)
{
    Ranges *ranges = Ranges::Intersect(this, &range);
    *this = *ranges;
    delete ranges;
}

void Ranges::Intersect(const Ranges& ranges)
{
    Ranges *r = Ranges::Intersect(this, &ranges);
    *this = *r;
    delete r;
}

bool Ranges::empty() const
{
    if (size() == 0)
        return true;

    std::size_t i, siz = size();
    for (i = 0; i < siz; ++i)
    {
        if (!((*this)[i])->empty())
            return false;
    }
    return true;
}

DRR1D_VALUE *Ranges::GetLBound(bool& has_min) const
{
    std::size_t i, siz = size();
    has_min = false;
    if (siz == 0)
        return NULL;
    for (i = 0; i < siz; ++i)
    {
        if (((*this)[i])->m_pnLBound == NULL)
            return NULL;
    }
    DRR1D_VALUE *value = ((*this)[0])->m_pnLBound;
    has_min = ((*this)[0])->m_has_min;
    for (i = 1; i < siz; ++i)
    {
        if (*value > *((*this)[i])->m_pnLBound)
        {
            value = ((*this)[i])->m_pnLBound;
            has_min = ((*this)[i])->m_has_min;
        }
        else if (*value == *((*this)[i])->m_pnLBound)
        {
            if (((*this)[i])->m_has_min)
                has_min = true;
        }
    }
    return value;
}

DRR1D_VALUE *Ranges::GetUBound(bool& has_max) const
{
    std::size_t i, siz = size();
    has_max = false;
    if (siz == 0)
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
    DRR1D_VALUE *value = ((*this)[0])->m_pnUBound;
    has_max = ((*this)[0])->m_has_max;
    for (i = 1; i < siz; ++i)
    {
        if (*value < *((*this)[i])->m_pnUBound)
        {
            value = ((*this)[i])->m_pnUBound;
            has_max = ((*this)[i])->m_has_max;
        }
        else if (*value == *((*this)[i])->m_pnUBound)
        {
            if (((*this)[i])->m_has_max)
                has_max = true;
        }
    }
    return value;
}

////////////////////////////////////////////////////////////////////////////
// Domain

bool Domain::Contains(const DRR1D_VALUE& value) const
{
    if (!m_ranges->Contains(value))
        return false;

    std::size_t i, siz = m_afnContains.size();
    for (i = 0; i < siz; ++i)
    {
        if (!(*m_afnContains[i])(value))
            return false;
    }
    return true;
}

bool Domain::Includes(Domain::Type type) const
{
    switch (m_dom_type)
    {
    case Domain::EMPTY:
        return false;

    case Domain::REGULAR:
        switch (type)
        {
        case Domain::REGULAR:
        case Domain::EVEN:
        case Domain::ODD:
        case Domain::PRIME:
            return true;

        case Domain::REAL:
            return false;

        default:
            assert(0);
        }
        break;

    case Domain::EVEN:
        switch (type)
        {
        case Domain::REGULAR:
        case Domain::ODD:
        case Domain::REAL:
        case Domain::PRIME:
            return false;

        case Domain::EVEN:
            return true;

        default:
            assert(0);
        }
        break;

    case Domain::ODD:
        switch (type)
        {
        case Domain::REGULAR:
        case Domain::EVEN:
        case Domain::REAL:
        case Domain::PRIME:
            return false;

        case Domain::ODD:
            return true;

        default:
            assert(0);
        }
        break;

    case Domain::REAL:
        return true;

    case Domain::PRIME:
        switch (type)
        {
        case Domain::REGULAR:
        case Domain::EVEN:
        case Domain::ODD:
        case Domain::REAL:
            return false;

        case Domain::PRIME:
            return true;

        default:
            assert(0);
        }
        break;

    default:
        assert(0);
    }
    return false;
}

bool Domain::Includes(const Domain& domain) const
{
    if (domain.m_dom_type == Domain::EMPTY)
        return true;

    if (!m_ranges->Includes(*domain.m_ranges.get()))
        return false;

    return Includes(domain.m_dom_type);
}

bool Domain::empty() const
{
    return m_dom_type == Domain::EMPTY || m_ranges->empty();
}

void Domain::RestrictTo(Domain::Type type)
{
    switch (type)
    {
    case Domain::EMPTY:
        m_dom_type = type;
        break;

    case Domain::REGULAR:
        if (m_dom_type == Domain::REAL)
            m_dom_type = type;
        break;

    case Domain::EVEN:
        switch (m_dom_type)
        {
        case Domain::EMPTY:
        case Domain::EVEN:
            break;

        case Domain::REGULAR:
        case Domain::REAL:
            m_dom_type = type;
            break;

        case Domain::ODD:
            m_dom_type = Domain::EMPTY;
            break;

        case Domain::PRIME:
            {
                Range *r = new Range;
                r->m_has_min = r->m_has_max = true;
                r->m_pnLBound = new DRR1D_VALUE(2);
                r->m_pnUBound = new DRR1D_VALUE(2);
                Intersect(*r);
                delete r;
            }
            break;

        default:
            assert(0);
        }
        break;

    case Domain::ODD:
        switch (m_dom_type)
        {
        case Domain::EMPTY:
        case Domain::ODD:
            break;

        case Domain::REGULAR:
        case Domain::REAL:
            m_dom_type = type;
            break;

        case Domain::EVEN:
            m_dom_type = Domain::EMPTY;
            break;

        case Domain::PRIME:
            {
                Range *r = new Range;
                r->m_has_min = true;
                r->m_has_max = false;
                r->m_pnLBound = new DRR1D_VALUE(3);
                r->m_pnUBound = NULL;
                Intersect(*r);
                delete r;
            }
            break;

        default:
            assert(0);
        }
        break;

    case Domain::REAL:
        break;

    case Domain::PRIME:
        switch (m_dom_type)
        {
        case Domain::EMPTY:
            break;

        case Domain::EVEN:
            m_dom_type = type;
            {
                Range *r = new Range;
                r->m_has_min = r->m_has_max = true;
                r->m_pnLBound = new DRR1D_VALUE(2);
                r->m_pnUBound = new DRR1D_VALUE(2);
                Intersect(*r);
                delete r;
            }
            break;

        case Domain::ODD:
            m_dom_type = type;
            {
                Range *r = new Range;
                r->m_has_min = true;
                r->m_has_max = false;
                r->m_pnLBound = new DRR1D_VALUE(3);
                r->m_pnUBound = NULL;
                Intersect(*r);
                delete r;
            }
            break;

        case Domain::REGULAR:
        case Domain::REAL:
            m_dom_type = type;
            break;

        default:
            assert(0);
        }
        break;

    default:
        assert(0);
    }
}

void Domain::clear()
{
    m_dom_type = Domain::EMPTY;
    m_ranges.get()->clear();
    m_afnContains.clear();
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
        clear();
}

void Domain::Intersect(const Range& r)
{
    std::size_t i, siz = m_ranges->size();
    for (i = 0; i < siz; ++i)
    {
        m_ranges.get()->Intersect(r);
    }
}

/*static*/ Domain *Domain::Whole()
{
    Domain *d = new Domain;
    Ranges *r = Ranges::Whole();
    d->m_ranges = shared_ptr<Ranges>(r);
    return d;
}

/*static*/ Domain *Domain::Intersect(const Domain *d1, const Domain *d2)
{
    Domain *d = new Domain(*d1);
    d->RestrictTo(d2->m_dom_type);
    if (d->m_dom_type == Domain::EMPTY)
    {
        delete d;
        return NULL;
    }
    d->m_ranges->Intersect(*d2->m_ranges.get());
    if (d->m_ranges->empty())
    {
        delete d;
        return NULL;
    }
    d->m_afnContains.insert(d->m_afnContains.end(),
        d2->m_afnContains.begin(), d2->m_afnContains.end());
    return d;
}

DRR1D_VALUE *Domain::GetLBound(bool& has_min) const
{
    has_min = false;
    if (empty())
        return NULL;

    return m_ranges->GetLBound(has_min);
}

DRR1D_VALUE *Domain::GetUBound(bool& has_max) const
{
    has_max = false;
    if (empty())
        return NULL;

    return m_ranges->GetUBound(has_max);
}

bool Domain::GetValues(std::vector<DRR1D_VALUE>& values) const
{
    values.clear();
    if (empty())
        return true;

    bool has_min, has_max;
    DRR1D_VALUE *pnLBound = GetLBound(has_min);
    DRR1D_VALUE *pnUBound = GetUBound(has_max);
    if (pnLBound == NULL || pnUBound == NULL)
    {
        return false;
    }

    if (m_dom_type == Domain::REAL)
    {
        if (*pnLBound == *pnUBound && has_min && has_max)
        {
            values.push_back(*pnLBound);
            return true;
        }
        return false;
    }
    if (m_dom_type == Domain::EVEN)
    {
        DRR1D_VALUE m0, m1;

        #ifdef DRR1D_USES_PMPNUMBER
            if ((*pnLBound % 2).is_zero())
                m0.assign(pnLBound->to_i());
            else
                m0.assign(GauseModCeil(*pnLBound, 2));
            if ((*pnUBound % 2).is_zero())
                m1.assign(pnUBound->to_i());
            else
                m1 = DRR1D_VALUE(pnUBound->to_i()) - 1;
        #else
            if (std::fmod(*pnLBound, 2) == 0)
                m0 = (int)*pnLBound;
            else
                m0 = (int)*pnLBound + 1;
            if (std::fmod(*pnUBound, 2) == 0)
                m1 = (int)*pnUBound;
            else
                m1 = (int)*pnUBound - 1;
        #endif

        if (*pnLBound > *pnUBound)
            return true;
        if (*pnLBound == *pnUBound)
        {
            if (Contains(*pnLBound))
                values.push_back(*pnLBound);
            return true;
        }
        for (DRR1D_VALUE i = m0; i <= m1; ++i)
        {
            if (Contains(i))
                values.push_back(i);
        }
        return true;
    }
    if (m_dom_type == Domain::ODD)
    {
        DRR1D_VALUE m0, m1;

        #ifdef DRR1D_USES_PMPNUMBER
            if ((*pnLBound % 2).is_zero())
                m0 = DRR1D_VALUE(pnLBound->to_i()) + 1;
            else
                m0.assign(pnLBound->to_i());
            if ((*pnUBound % 2).is_zero())
                m1 = DRR1D_VALUE(pnUBound->to_i()) - 1;
            else
                m1.assign(pnUBound->to_i());
        #else
            if (std::fmod(*pnLBound, 2) == 0)
                m0 = (int)*pnLBound + 1;
            else
                m0 = (int)*pnLBound;
            if (std::fmod(*pnUBound, 2) == 0)
                m1 = (int)*pnUBound - 1;
            else
                m1 = (int)*pnUBound;
        #endif
        if (*pnLBound > *pnUBound)
            return true;
        if (*pnLBound == *pnUBound)
        {
            if (Contains(*pnLBound))
                values.push_back(*pnLBound);
            return true;
        }
        for (DRR1D_VALUE i = m0; i <= m1; i += 2)
        {
            if (Contains(i))
                values.push_back(i);
        }
        return true;
    }
    if (m_dom_type == Domain::PRIME)
    {
        DRR1D_VALUE m0, m1;
        #ifdef DRR1D_USES_PMPNUMBER
            m0.assign(pnLBound->to_i());
            m1 = DRR1D_VALUE(pnUBound->to_i()) + 1;
        #else
            m0 = (int)*pnLBound;
            m1 = (int)*pnUBound + 1;
        #endif
        if (*pnLBound > *pnUBound)
            return true;
        if (*pnLBound == *pnUBound)
        {
            if (Contains(*pnLBound))
                values.push_back(*pnLBound);
            return true;
        }
        for (DRR1D_VALUE i = m0; i <= m1; ++i)
        {
            if (Contains(i))
                values.push_back(i);
        }
        return true;
    }
    if (m_dom_type == Domain::REGULAR)
    {
        DRR1D_VALUE m0, m1;
        #ifdef DRR1D_USES_PMPNUMBER
            m0.assign(pnLBound->to_i());
            m1 = DRR1D_VALUE(pnUBound->to_i()) + 1;
        #else
            m0 = (int)*pnLBound;
            m1 = (int)*pnUBound + 1;
        #endif
        if (*pnLBound > *pnUBound)
            return true;
        if (*pnLBound == *pnUBound)
        {
            if (Contains(*pnLBound))
                values.push_back(*pnLBound);
            return true;
        }
        for (DRR1D_VALUE i = m0; i <= m1; ++i)
        {
            if (Contains(i))
                values.push_back(i);
        }
        return true;
    }
    return false;
}

void Domain::FixNarrower()
{
    switch (m_dom_type)
    {
    case Domain::EMPTY:
    case Domain::REAL:
        return;

    default:
        break;
    }

    //std::cout << "begin Domain::FixNarrower: " << *this << std::endl;
    std::size_t i, siz = m_ranges->size();
    for (i = 0; i < siz; ++i)
    {
        Range& r = *(*m_ranges.get())[i].get();
        switch (m_dom_type)
        {
        case Domain::REGULAR:
            if (r.m_pnLBound)
            {
                DRR1D_VALUE v1 = *r.m_pnLBound;
                #ifdef DRR1D_USES_PMPNUMBER
                    DRR1D_VALUE v2 = pmp::ceil(v1);
                #else
                    DRR1D_VALUE v2 = std::ceil(v1);
                #endif
                // v1 == 2, v2 == 2, has_min == true
                // result: 2, true
                // v1 == 2, v2 == 2, has_min == false
                // result: 3, true
                // v1 == 1.5, v2 == 2, has_min == true
                // result: 2, false
                // v1 == 1.5, v2 == 2, has_min == false
                // result: 2, false
                *r.m_pnLBound = v2;
                if (v1 == v2)
                {
                    if (!r.m_has_min)
                    {
                        (*r.m_pnLBound) += 1;
                        r.m_has_min = true;
                    }
                }
                else
                {
                    r.m_has_min = false;
                }
            }
            if (r.m_pnUBound)
            {
                DRR1D_VALUE v1 = *r.m_pnUBound;
                #ifdef DRR1D_USES_PMPNUMBER
                    DRR1D_VALUE v2 = pmp::floor(v1);
                #else
                    DRR1D_VALUE v2 = std::floor(v1);
                #endif
                *r.m_pnUBound = v2;
                if (v1 == v2)
                {
                    if (!r.m_has_max)
                    {
                        (*r.m_pnUBound) -= 1;
                        r.m_has_max = true;
                    }
                }
                else
                {
                    r.m_has_max = false;
                }
            }
            break;

        case Domain::EVEN:
            if (r.m_pnLBound)
            {
                DRR1D_VALUE v1 = *r.m_pnLBound;
                DRR1D_VALUE v2 = GauseModCeil(v1, 2);
                *r.m_pnLBound = v2;
                if (v1 == v2)
                {
                    if (!r.m_has_min)
                    {
                        (*r.m_pnLBound) += 2;
                        r.m_has_min = true;
                    }
                }
                else
                {
                    r.m_has_min = false;
                }
            }
            if (r.m_pnUBound)
            {
                DRR1D_VALUE v1 = *r.m_pnUBound;
                DRR1D_VALUE v2 = GauseModFloor(v1, 2);
                *r.m_pnUBound = v2;
                if (v1 == v2)
                {
                    if (!r.m_has_max)
                    {
                        (*r.m_pnUBound) -= 2;
                        r.m_has_max = true;
                    }
                }
                else
                {
                    r.m_has_max = false;
                }
            }
            break;

        case Domain::ODD:
            if (r.m_pnLBound)
            {
                DRR1D_VALUE v1 = *r.m_pnLBound;
                DRR1D_VALUE v2 = GauseModCeil(v1 + 1, 2) - 1;
                *r.m_pnLBound = v2;
                if (v1 == v2)
                {
                    if (!r.m_has_min)
                    {
                        (*r.m_pnLBound) += 2;
                        r.m_has_min = true;
                    }
                }
                else
                {
                    r.m_has_min = false;
                }
            }
            if (r.m_pnUBound)
            {
                DRR1D_VALUE v1 = *r.m_pnUBound;
                DRR1D_VALUE v2 = GauseModFloor(v1 - 1, 2) + 1;
                *r.m_pnUBound = v2;
                if (v1 == v2)
                {
                    if (!r.m_has_max)
                    {
                        (*r.m_pnUBound) -= 2;
                        r.m_has_max = true;
                    }
                }
                else
                {
                    r.m_has_max = false;
                }
            }
            break;

        case Domain::PRIME:
            if (r.m_pnLBound)
            {
                DRR1D_VALUE v1 = *r.m_pnLBound;
                #ifdef DRR1D_USES_PMPNUMBER
                    DRR1D_VALUE v2 = pmp::ceil(v1);
                #else
                    DRR1D_VALUE v2 = std::ceil(v1);
                #endif
                if (v2 < 2)
                {
                    break;
                }
                while (!IsPrimeNumber(v2))
                {
                    v2 += 1;
                }
                *r.m_pnLBound = v2;
                if (v1 == v2)
                {
                    if (!r.m_has_min)
                    {
                        do 
                        {
                            v2 += 1;
                        } while (!IsPrimeNumber(v2));
                        *r.m_pnLBound = v2;
                    }
                }
                else
                {
                    r.m_has_min = false;
                }
            }
            if (r.m_pnUBound)
            {
                DRR1D_VALUE v1 = *r.m_pnUBound;
                #ifdef DRR1D_USES_PMPNUMBER
                    DRR1D_VALUE v2 = pmp::floor(v1);
                #else
                    DRR1D_VALUE v2 = std::floor(v1);
                #endif
                while (v2 >= 2 && !IsPrimeNumber(v2))
                {
                    v2 -= 1;
                }
                if (v2 < 2)
                {
                    r.empty();
                    break;
                }
                *r.m_pnUBound = v2;
                if (v1 == v2)
                {
                    if (!r.m_has_max)
                    {
                        do
                        {
                            v2 -= 1;
                        } while (v2 >= 2 && !IsPrimeNumber(v2));
                        *r.m_pnUBound = v2;
                        r.m_has_max = true;
                    }
                }
                else
                {
                    r.m_has_max = false;
                }
            }
            break;

        default:
            assert(0);
        }
    }
    //std::cout << "end Domain::FixNarrower: " << *this << std::endl;
}

void Domain::FixWider()
{
    switch (m_dom_type)
    {
    case Domain::EMPTY:
    case Domain::REAL:
        return;

    default:
        break;
    }

    //std::cout << "begin Domain::FixWider: " << *this << std::endl;
    std::size_t i, siz = m_ranges->size();
    for (i = 0; i < siz; ++i)
    {
        Range& r = *(*m_ranges.get())[i].get();
        switch (m_dom_type)
        {
        case Domain::REGULAR:
            if (r.m_pnLBound)
            {
                DRR1D_VALUE v1 = *r.m_pnLBound;
                #ifdef DRR1D_USES_PMPNUMBER
                    DRR1D_VALUE v2 = pmp::floor(v1);
                #else
                    DRR1D_VALUE v2 = std::floor(v1);
                #endif
                *r.m_pnLBound = v2;
                if (v1 == v2 && r.m_has_min)
                {
                    r.m_has_min = false;
                    (*r.m_pnLBound) -= 1;
                }
                else
                {
                    r.m_has_min = false;
                }
            }
            if (r.m_pnUBound)
            {
                DRR1D_VALUE v1 = *r.m_pnUBound;
                #ifdef DRR1D_USES_PMPNUMBER
                    DRR1D_VALUE v2 = pmp::ceil(v1);
                #else
                    DRR1D_VALUE v2 = std::ceil(v1);
                #endif
                *r.m_pnUBound = v2;
                if (v1 == v2 && r.m_has_max)
                {
                    r.m_has_max = false;
                    (*r.m_pnUBound) += 1;
                }
                else
                {
                    r.m_has_max = false;
                }
            }
            break;

        case Domain::EVEN:
            if (r.m_pnLBound)
            {
                DRR1D_VALUE v1 = *r.m_pnLBound;
                DRR1D_VALUE v2 = GauseModFloor(v1, 2);
                *r.m_pnLBound = v2;
                if (v1 == v2 && r.m_has_min)
                {
                    r.m_has_min = false;
                    (*r.m_pnLBound) -= 2;
                }
                else
                {
                    r.m_has_min = false;
                }
            }
            if (r.m_pnUBound)
            {
                DRR1D_VALUE v1 = *r.m_pnUBound;
                DRR1D_VALUE v2 = GauseModCeil(v1, 2);
                *r.m_pnUBound = v2;
                if (v1 == v2 && r.m_has_max)
                {
                    r.m_has_max = false;
                    (*r.m_pnUBound) += 2;
                }
                else
                {
                    r.m_has_max = false;
                }
            }
            break;

        case Domain::ODD:
            if (r.m_pnLBound)
            {
                DRR1D_VALUE v1 = *r.m_pnLBound;
                DRR1D_VALUE v2 = GauseModFloor(v1 + 1, 2) - 1;
                *r.m_pnLBound = v2;
                if (v1 == v2 && r.m_has_min)
                {
                    r.m_has_min = false;
                    (*r.m_pnLBound) -= 2;
                }
                else
                {
                    r.m_has_min = false;
                }
            }
            if (r.m_pnUBound)
            {
                DRR1D_VALUE v1 = *r.m_pnUBound;
                DRR1D_VALUE v2 = GauseModCeil(v1 - 1, 2) + 1;
                *r.m_pnUBound = v2;
                if (v1 == v2 && r.m_has_max)
                {
                    r.m_has_max = false;
                    (*r.m_pnUBound) += 2;
                }
                else
                {
                    r.m_has_max = false;
                }
            }
            break;

        case Domain::PRIME:
            if (r.m_pnLBound)
            {
                DRR1D_VALUE v1 = *r.m_pnLBound;
                #ifdef DRR1D_USES_PMPNUMBER
                    DRR1D_VALUE v2 = pmp::floor(v1);
                #else
                    DRR1D_VALUE v2 = std::floor(v1);
                #endif
                while (v2 > 2 && !IsPrimeNumber(v2))
                {
                    v2 -= 1;
                }
                if (v2 < 2)
                    break;
                *r.m_pnLBound = v2;
                if (v1 == v2 && r.m_has_min)
                {
                    do
                    {
                        v2 -= 1;
                    }
                    while (v2 > 2 && !IsPrimeNumber(v2));
                    r.m_has_min = false;
                    (*r.m_pnLBound) = v2;
                }
                else
                {
                    r.m_has_min = false;
                }
            }
            if (r.m_pnUBound)
            {
                DRR1D_VALUE v1 = *r.m_pnUBound;
                #ifdef DRR1D_USES_PMPNUMBER
                    DRR1D_VALUE v2 = pmp::ceil(v1);
                #else
                    DRR1D_VALUE v2 = std::ceil(v1);
                #endif
                while (!IsPrimeNumber(v2))
                {
                    v2 += 1;
                }
                *r.m_pnUBound = v2;
                if (v1 == v2 && r.m_has_max)
                {
                    r.m_has_max = false;
                    do
                    {
                        v2 += 1;
                    } while (!IsPrimeNumber(v2));
                    (*r.m_pnUBound) = v2;
                }
                else
                {
                    r.m_has_max = false;
                }
            }
            break;

        default:
            assert(0);
        }
    }
    //std::cout << "end Domain::FixWider: " << *this << std::endl;
}

void Domain::Optimize()
{
    //std::cout << "begin Domain::Optimize: " << *this << std::endl;
    m_ranges.get()->Optimize();
    //std::cout << "end Domain::Optimize: " << *this << std::endl;
}

////////////////////////////////////////////////////////////////////////////
// Domains

bool Domains::empty() const
{
    if (size() == 0)
        return true;

    std::size_t i, siz = size();
    for (i = 0; i < siz; ++i)
    {
        if (!(*this)[0]->empty())
            return false;
    }
    return true;
}

bool Domains::Includes(const Domain& domain) const
{
    if (domain.empty())
        return true;

    std::size_t i, siz = size();
    for (i = 0; i < siz; ++i)
    {
        if ((*this)[i]->Includes(domain))
            return true;
        if (domain.m_dom_type == Domain::REGULAR)
        {
            Domain d1(domain), d2(domain);
            d1.RestrictTo(Domain::EVEN);
            d2.RestrictTo(Domain::ODD);
            if (!d1.empty() && !d2.empty() &&
                Includes(d1) && Includes(d2))
                return true;
        }
    }
    return false;
}

bool Domains::Includes(const Domains& domains) const
{
    std::size_t j, siz2 = domains.size();
    for (j = 0; j < siz2; ++j)
    {
        bool flag = false;
        if (Includes(*domains[j].get()))
        {
            flag = true;
            break;
        }
        if (!flag)
            return false;
    }
    return true;
}

bool Domains::Contains(DRR1D_VALUE value) const
{
    std::size_t i, siz = size();
    for (i = 0; i < siz; ++i)
    {
        if ((*this)[i]->Contains(value))
            return true;
    }
    return false;
}

void Domains::Intersect(const Range& range)
{
    std::size_t i, siz = size();
    for (i = 0; i < siz; ++i)
    {
        (*this)[i].get()->Intersect(range);
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
    std::size_t i, siz = size();
    for (i = 0; i < siz; ++i)
    {
        if ((*this)[i]->Includes(domain))
            return;
    }
    for (i = 0; i < siz; ++i)
    {
        if ((*this)[i]->m_dom_type == domain.m_dom_type)
        {
            (*this)[i].get()->m_ranges->Union(*domain.m_ranges.get());
            FixWider();
            return;
        }
    }
    push_back(shared_ptr<Domain>(new Domain(domain)));
    FixWider();
}

void Domains::Union(const Domains& domains)
{
    std::size_t i, siz = domains.size();
    for (i = 0; i < siz; ++i)
    {
        Union(*domains[i].get());
    }
    FixWider();
}

/*static*/ Domains *Domains::Intersect(const Domains *domains, const Domain *domain)
{
    Domains *pDomains = new Domains();
    std::size_t i, siz = domains->size();
    for (i = 0; i < siz; ++i)
    {
        Domain *d = Domain::Intersect((*domains)[i].get(), domain);
        if (d)
        {
            pDomains->Union(*d);
            delete d;
        }
    }
    return pDomains;
}

/*static*/ Domains *Domains::Intersect(const Domains *d1, const Domains *d2)
{
    Domains *pDomains = new Domains();
    std::size_t i, siz1 = d1->size();
    for (i = 0; i < siz1; ++i)
    {
        Domains *d = Domains::Intersect(d2, ((*d1)[i]).get());
        if (d->size())
            pDomains->insert(pDomains->end(), d->begin(), d->end());
        delete d;
    }
    return pDomains;
}

/*static*/ Domains *Domains::Whole()
{
    Domains *domains = new Domains;
    Domain *domain = Domain::Whole();
    domains->push_back(shared_ptr<Domain>(domain));
    return domains;
}

bool Domains::GetValues(std::vector<DRR1D_VALUE>& values) const
{
    values.clear();

    std::size_t i, siz = size();
    for (i = 0; i < siz; ++i)
    {
        std::vector<DRR1D_VALUE> vec;
        if (((*this)[i])->GetValues(vec))
        {
            values.insert(values.end(), vec.begin(), vec.end());
        }
        else
        {
            return false;
        }
    }
    std::sort(values.begin(), values.end());
    std::unique(values.begin(), values.end());
    return true;
}

void Domains::FixNarrower()
{
    std::size_t i, j, siz;

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

void Domains::FixWider()
{
    std::size_t i, siz = size();
    for (i = 0; i < siz; ++i)
    {
        ((*this)[i]).get()->FixWider();
        ((*this)[i]).get()->Optimize();
    }

    bool flag = false;
    for (i = 0; i < siz; ++i)
    {
        Domain& domain1 = *((*this)[i]).get();
        if (domain1.m_dom_type == Domain::REGULAR)
        {
            for (std::size_t j = 0; j < siz; ++j)
            {
                if (j == i)
                    continue;
                Domain& domain2 = *((*this)[j]).get();
                if (domain2.m_dom_type == Domain::ODD ||
                    domain2.m_dom_type == Domain::EVEN ||
                    domain2.m_dom_type == Domain::PRIME)
                {
                    domain2.m_ranges.get()->Union(*domain1.m_ranges.get());
                    flag = true;
                }
            }
        }
        else if (domain1.m_dom_type == Domain::REAL)
        {
            for (std::size_t j = 0; j < siz; ++j)
            {
                if (j == i)
                    continue;
                Domain& domain2 = *((*this)[j]).get();
                if (domain2.m_dom_type == Domain::ODD ||
                    domain2.m_dom_type == Domain::EVEN ||
                    domain2.m_dom_type == Domain::PRIME ||
                    domain2.m_dom_type == Domain::REGULAR)
                {
                    domain2.m_ranges.get()->Union(*domain1.m_ranges.get());
                    flag = true;
                }
            }
        }
    }

    if (flag)
    {
        for (i = 0; i < siz; ++i)
        {
            ((*this)[i]).get()->FixWider();
            ((*this)[i]).get()->Optimize();
        }
    }
}

////////////////////////////////////////////////////////////////////////////

} // namespace Ndrr1D

////////////////////////////////////////////////////////////////////////////
