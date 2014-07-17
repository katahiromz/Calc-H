/////////////////////////////////////////////////////////////////////////////
// Ndrr1D.hpp --- The one-dimensional Numeric Domains/Ranges Resolver
// See file "ReadMe.txt" and "License.txt".
/////////////////////////////////////////////////////////////////////////////

#ifndef __NDRR1D__
#define __NDRR1D__

#ifndef __cplusplus
    #error You lose.
#endif

#include <iostream> // for std::cout, std::endl, std::size_t
#include <vector>   // for std::vector

#ifndef shared_ptr
    #if (__cplusplus >= 201103L)
        #include <memory>
        using std::shared_ptr;
        using std::static_pointer_cast;
        using std::dynamic_pointer_cast;

        using std::make_shared;
    #else
        // boost::shared_ptr
        #include <Boost/shared_ptr.hpp>
        using boost::shared_ptr;
        using boost::static_pointer_cast;
        using boost::dynamic_pointer_cast;

        // boost::make_shared
        #include <boost/make_shared.hpp>
        using boost::make_shared;
    #endif
    #define shared_ptr shared_ptr
#endif  // ndef shared_ptr

////////////////////////////////////////////////////////////////////////////

namespace Ndrr1D
{
    //
    // DRR1D_VALUE
    //
    #ifndef DRR1D_VALUE
        #ifdef PMPNUMBER_HPP_
            #define DRR1D_VALUE pmp::Number
            #ifndef DRR1D_USES_PMPNUMBER
                #define DRR1D_USES_PMPNUMBER
            #endif
        #else
            #define DRR1D_VALUE double
        #endif
    #endif

    bool IsPositiveNumber(const DRR1D_VALUE& value);
    bool IsNegativeNumber(const DRR1D_VALUE& value);
    bool IsNaturalNumber(const DRR1D_VALUE& value);
    bool IsRegularNumber(const DRR1D_VALUE& value);
    bool IsEvenNumber(const DRR1D_VALUE& value);
    bool IsOddNumber(const DRR1D_VALUE& value);
    bool IsPrimeNumber(const DRR1D_VALUE& value);
    DRR1D_VALUE GauseModFloor(const DRR1D_VALUE& d, const DRR1D_VALUE& mod);
    DRR1D_VALUE GauseModCeil(const DRR1D_VALUE& d, const DRR1D_VALUE& mod);

    //
    // Ndrr1D::Range
    //
    struct Range
    {
        bool            m_has_min;
        bool            m_has_max;
        DRR1D_VALUE *   m_pnLBound;
        DRR1D_VALUE *   m_pnUBound;

        Range() :
            m_has_min(false), m_has_max(false),
            m_pnLBound(NULL), m_pnUBound(NULL)
        {
        }

        Range(bool has_min, bool has_max, DRR1D_VALUE *pnLBound, DRR1D_VALUE *pnUBound) :
            m_has_min(has_min), m_has_max(has_max),
            m_pnLBound(pnLBound ? new DRR1D_VALUE(*pnLBound) : NULL),
            m_pnUBound(pnUBound ? new DRR1D_VALUE(*pnUBound) : NULL)
        {
        }

        Range(const DRR1D_VALUE& value) :
            m_has_min(true), m_has_max(true),
            m_pnLBound(new DRR1D_VALUE(value)), m_pnUBound(new DRR1D_VALUE(value))
        {
        }

        ~Range()
        {
            delete m_pnLBound;
            delete m_pnUBound;
        }

        Range(const Range& r) :
            m_has_min(r.m_has_min),
            m_has_max(r.m_has_max),
            m_pnLBound(new DRR1D_VALUE(*r.m_pnLBound)),
            m_pnUBound(new DRR1D_VALUE(*r.m_pnUBound))
        {
        }

        Range& operator=(const Range& r)
        {
            m_has_min = r.m_has_min;
            delete m_pnLBound;
            m_pnLBound = new DRR1D_VALUE(*r.m_pnLBound);
            m_has_max = r.m_has_max;
            delete m_pnUBound;
            m_pnUBound = new DRR1D_VALUE(*r.m_pnUBound);
            return *this;
        }

        bool empty() const;
        void clear();
        bool Includes(const Range& range) const;
        bool Contains(DRR1D_VALUE value) const;
        void Intersect(const Range& r);

        DRR1D_VALUE *GetLBound(bool& has_min) const
        {
            has_min = m_has_min;
            return m_pnLBound;
        }

        DRR1D_VALUE *GetUBound(bool& has_max) const
        {
            has_max = m_has_max;
            return m_pnUBound;
        }

        static Range *Whole()
        {
            return new Range;
        }

    protected:
        static Range *Intersect(const Range *r1, const Range *r2);
        friend struct Ranges;
        friend struct Domain;
        friend struct Domains;
    };

    //
    // Ndrr1D::Ranges
    //
    struct Ranges : std::vector<shared_ptr<Range> >
    {
        Ranges() { }

        Ranges(const Ranges& ranges) : std::vector<shared_ptr<Range> >(ranges)
        { }

        Ranges& operator=(const Ranges& ranges)
        {
            this->assign(ranges.begin(), ranges.end());
            return *this;
        }

        bool empty() const;
        bool Includes(const Ranges& ranges) const;
        bool Contains(DRR1D_VALUE value) const;
        void Intersect(const Range& range);
        void Intersect(const Ranges& ranges);
        void Union(const Ranges& ranges);
        void Optimize();

        DRR1D_VALUE *GetLBound(bool& has_min) const;
        DRR1D_VALUE *GetUBound(bool& has_max) const;
        static Ranges *Whole();

    protected:
        static Ranges *Optimize(const Ranges *ranges);
        static Ranges *Intersect(const Ranges *ranges, const Range *range);
        static Ranges *Intersect(const Ranges *r1, const Ranges *r2);
        static Ranges *Union(const Ranges *r1, const Ranges *r2);
        friend struct Range;
        friend struct Domain;
        friend struct Domains;
    };

    //
    // Ndrr1D::FN_CONTAINS
    //
    typedef bool (*FN_CONTAINS)(const DRR1D_VALUE& value);

    //
    // Ndrr1D::Domain
    //
    struct Domain
    {
        enum Type { EMPTY, SEISUU, GUUSUU, KISUU, JISSUU, SOSUU };
        Type                        m_dom_type;
        shared_ptr<Ranges>          m_ranges;
        std::vector<FN_CONTAINS>    m_afnContains;

        Domain() : m_dom_type(JISSUU) { }

        Domain(const Domain& d) :
            m_dom_type(d.m_dom_type),
            m_ranges(d.m_ranges),
            m_afnContains(d.m_afnContains)
        {
        }

        Domain& operator=(const Domain& d)
        {
            m_dom_type = d.m_dom_type;
            m_ranges = d.m_ranges;
            m_afnContains = d.m_afnContains;
            return *this;
        }

        bool empty() const;
        void clear();
        bool Includes(Domain::Type type) const;
        bool Includes(const Domain& domain) const;
        bool Contains(const DRR1D_VALUE& value) const;
        void RestrictTo(Domain::Type type);
        void Intersect(const Range& r);
        void Intersect(const Domain& d);
        void Optimize();
        DRR1D_VALUE *GetLBound(bool& has_min) const;
        DRR1D_VALUE *GetUBound(bool& has_max) const;
        bool GetValues(std::vector<DRR1D_VALUE>& values) const;
        void FixWider();
        void FixNarrower();
        static Domain *Whole();

    protected:
        static Domain *Intersect(const Domain *d1, const Domain *d2);
        friend struct Range;
        friend struct Ranges;
        friend struct Domains;
    };

    //
    // Ndrr1D::Domains
    //
    struct Domains : std::vector<shared_ptr<Domain> >
    {
        Domains() { }

        Domains(const Domains& domains) : std::vector<shared_ptr<Domain> >(domains)
        { }

        Domains& operator=(const Domains& domains)
        {
            this->assign(domains.begin(), domains.end());
            return *this;
        }

        bool empty() const;

        // NOTE: You have to call domain.FixNarrower and this->FixWider before
        //       Domains::Includes(const Domain&) call.
        bool Includes(const Domain& domain) const;

        // NOTE: You have to call domains.FixNarrower and this->FixWider before
        //       Domains::Includes(const Domains&) call.
        bool Includes(const Domains& domains) const;

        bool Contains(DRR1D_VALUE value) const;
        void Intersect(const Range& range);
        void Intersect(const Domain& domain);
        void Intersect(const Domains& domains);
        void Union(const Domain& domain);
        void Union(const Domains& domains);
        bool GetValues(std::vector<DRR1D_VALUE>& values) const;
        static Domains *Whole();
        void FixWider();
        void FixNarrower();

    protected:
        static Domains *Intersect(const Domains *domains, const Domain *domain);
        static Domains *Intersect(const Domains *d1, const Domains *d2);
        static Domains *Union(const Domains *d1, const Domains *d2);
        friend struct Range;
        friend struct Ranges;
        friend struct Domain;
    };
} // namespace Ndrr1D

template <class charT, class traits>
std::basic_ostream<charT,traits>&
operator<<(std::basic_ostream<charT,traits>& os, const Ndrr1D::Range& r)
{
    os << "[";
    if (r.m_pnLBound)
    {
        if (r.m_has_min) os << "(Closed)";
        else os << "(Open)";
        os << "lower: " << *r.m_pnLBound << " ";
    }
    if (r.m_pnUBound)
    {
        if (r.m_has_max) os << "(Closed)";
        else os << "(Open)";
        os << "upper: " << *r.m_pnUBound;
    }
    os << "]";
    return os;
}

template <class charT, class traits>
std::basic_ostream<charT,traits>&
operator<<(std::basic_ostream<charT,traits>& os, const Ndrr1D::Ranges& r)
{
    os << "[";
    for (std::size_t i = 0; i < r.size(); ++i)
    {
        os << *(r[i].get());
    }
    os << "]";
    return os;
}

template <class charT, class traits>
std::basic_ostream<charT,traits>&
operator<<(std::basic_ostream<charT,traits>& os, const Ndrr1D::Domain& d)
{
    os << "[" << d.m_dom_type << ": " <<
        (*d.m_ranges.get()) << "]";
    return os;
}

template <class charT, class traits>
std::basic_ostream<charT,traits>&
operator<<(std::basic_ostream<charT,traits>& os, const Ndrr1D::Domains& d)
{
    os << "[domains: ";
    for (std::size_t i = 0; i < d.size(); ++i)
    {
        os << *(d[i].get());
    }
    os << "]";
    return os;
}

#endif  // ndef __NDRR1D__
