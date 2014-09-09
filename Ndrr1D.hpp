/////////////////////////////////////////////////////////////////////////////
// Ndrr1D.hpp --- The one-dimensional Numeric Domains/Ranges Resolver
// See file "ReadMe.txt" and "License.txt".
/////////////////////////////////////////////////////////////////////////////

#ifndef __NDRR1D__
#define __NDRR1D__

#ifndef __cplusplus
    #error You lose.
#endif

#include <iostream> // for std::cout, std::endl, size_t
#include <vector>   // for std::vector
#include <cassert>  // for assert

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

#ifndef DRR1D_USES_PMPNUMBER
    #include <cmath>
#endif

////////////////////////////////////////////////////////////////////////////

namespace Ndrr1D
{
    //
    // number_type
    //
    #ifdef DRR1D_USES_PMPNUMBER
        typedef pmp::Number         number_type;
        typedef pmp::integer_type   integer_type;
    #else
        typedef double              number_type;
        typedef int                 integer_type;
    #endif

    bool IsPositiveNumber(const number_type& value);
    bool IsNegativeNumber(const number_type& value);
    bool IsNaturalNumber(const number_type& value);
    bool IsRegularNumber(const number_type& value);
    bool IsEvenNumber(const number_type& value);
    bool IsOddNumber(const number_type& value);
    bool IsPrimeNumber(const number_type& value);
    bool IsCompositeNumber(const number_type& value);
    number_type ModFloor(number_type d, number_type mod);
    number_type ModCeil(number_type d, number_type mod);

    integer_type gcd(integer_type x, integer_type y);
    integer_type lcm(integer_type x, integer_type y);
    integer_type gcdx(integer_type& x, integer_type& y,
                      const integer_type& a, const integer_type& b);
    integer_type cnrem(
        const integer_type& a1, const integer_type& m1,
        const integer_type& a2, const integer_type& m2);
    bool katayama_qz(
        integer_type & y, integer_type & M, integer_type & N,
        const integer_type& a1, integer_type m1,
        const integer_type& a2, integer_type m2);

    //
    // Ndrr1D::Range
    //
    struct Range
    {
        bool            m_has_min;
        bool            m_has_max;
        number_type *    m_pnLBound;
        number_type *    m_pnUBound;

        Range();
        Range(bool has_min, bool has_max,
              number_type *pnLBound, number_type *pnUBound);
        Range(const number_type& value);
        Range(const Range& s);
        virtual ~Range();
        Range& operator=(const Range& s);

        number_type *GetLBound(bool& has_min) const
        {
            has_min = m_has_min;
            return m_pnLBound;
        }

        number_type *GetUBound(bool& has_max) const
        {
            has_max = m_has_max;
            return m_pnUBound;
        }

        bool empty() const;
        bool entire() const;
        void clear();

        bool Includes(const Range& r) const;
        bool Contains(const number_type& value) const;
        void Intersect(const Range& r);
        bool Equal(const Range& r) const;

        static Range *Intersect(const Range *r1, const Range *r2);
        static Range *Whole() { return new Range(); }
    };

    //
    // Ndrr1D::Ranges
    //
    struct Ranges : public std::vector<shared_ptr<Range> >
    {
        Ranges() { }
        Ranges(const Range& r);
        Ranges(bool has_min, bool has_max,
               number_type *pnLBound, number_type *pnUBound);
        Ranges(const number_type& value);
        Ranges(const std::vector<shared_ptr<Range> >& ranges);
        Ranges& operator=(const std::vector<shared_ptr<Range> >& ranges);

        bool empty() const;
        bool entire() const;

        void add(Range *r);

        bool Includes(const Range& r) const;
        bool Includes(const Ranges& r) const;
        bool Contains(const number_type& value) const;

        void Optimize();
        void Intersect(const Range& r);
        void Intersect(const Ranges& r);
        void Union(const Ranges& ranges);

        number_type *GetLBound(bool& has_min) const;
        number_type *GetUBound(bool& has_max) const;
        bool Equal(const Ranges& r) const;

        static Ranges *Union(const Ranges *r1, const Ranges *r2);
        static Ranges *Intersect(const Ranges *ranges, const Range *r);
        static Ranges *Intersect(const Ranges *r1, const Ranges *r2);
        static Ranges *Whole();
    };

    //
    // Ndrr1D::Aspect
    //
    struct Aspect
    {
        integer_type *  m_pnModulus;
        integer_type *  m_pnResidue;
        bool            m_must_be_prime;
        bool            m_must_be_composite;

        Aspect();
        Aspect(integer_type *pnModulus, integer_type *pnResidue = NULL,
               bool must_be_prime = false, bool must_be_composite = false);
        Aspect(const Aspect& a);
        Aspect& operator=(const Aspect& a);
        virtual ~Aspect();

        integer_type *GetModulus() const    { return m_pnModulus; }
        integer_type *GetResidue() const    { return m_pnResidue; }
        bool MustBePrime() const            { return m_must_be_prime; }
        bool MustBeComposite() const        { return m_must_be_composite; }
        bool MustBe2() const;

        bool empty() const;
        bool entire() const;
        void clear();

        bool Includes(const Aspect& a) const;
        bool Contains(const number_type& value) const;
        void Intersect(const Aspect& a);
        bool Equal(const Aspect& a) const;

        static Aspect *Intersect(const Aspect *a1, const Aspect *a2);
    };

    //
    // Ndrr1D::Domain
    //
    struct Domain
    {
        Aspect  m_aspect;
        Ranges  m_ranges;

        Domain() { }
        Domain(bool has_min, bool has_max,
               number_type *pnLBound, number_type *pnUBound);
        Domain(const Aspect& a, bool has_min, bool has_max,
               number_type *pnLBound, number_type *pnUBound);
        Domain(const number_type& value) : m_ranges(value) { }
        Domain(const Aspect& a, const number_type& value);
        Domain(const Aspect& a);
        Domain(const Aspect& a, const Ranges& r) : m_aspect(a), m_ranges(r) { }
        Domain(const Domain& d) : m_aspect(d.m_aspect), m_ranges(d.m_ranges) { }

        Domain& operator=(const Domain& d)
        {
            m_aspect = d.m_aspect;
            m_ranges = d.m_ranges;
            return *this;
        }

        void FixBeforeIncludes(Domain& d);

        bool Includes(const Range& r) const { return m_ranges.Includes(r); }
        bool Includes(const Ranges& r) const { return m_ranges.Includes(r); }

        // NOTE: You have to call this->FixBeforeIncludes(d) before
        //       Domain::Includes(const Domain& d) call.
        bool Includes(const Domain& d) const;

        bool Contains(const number_type& value) const
        {
            return m_aspect.Contains(value) && m_ranges.Contains(value);
        }

        bool GetValues(std::vector<number_type>& values) const;

        bool empty() const;
        bool entire() const;
        void clear();

        void Intersect(const Aspect& a);
        void Intersect(const Range& r);
        void Intersect(const Domain& d);
        void Optimize();
        void FixWider();
        void FixNarrower();

        number_type *GetLBound(bool& has_min) const
            { return m_ranges.GetLBound(has_min); }
        number_type *GetUBound(bool& has_max) const
            { return m_ranges.GetUBound(has_max); }
        integer_type *GetModulus() const { return m_aspect.GetModulus(); }
        integer_type *GetResidue() const { return m_aspect.GetResidue(); }
        bool MustBePrime() const         { return m_aspect.MustBePrime(); }
        bool MustBeComposite() const     { return m_aspect.MustBeComposite(); }
        bool MustBe2() const             { return m_aspect.MustBe2(); }
        bool Equal(const Domain& d) const;

        enum Type
        {
            EMPTY,      // none or empty region
            POSITIVE,   // number > 0
            NEGATIVE,   // number < 0
            NATURAL,    // MODULAR 1 RESIDUE 0 (number >= 1)
            REGULAR,    // MODULAR 1 RESIDUE 0
            EVEN,       // MODULAR 2 RESIDUE 0
            ODD,        // MODULAR 2 RESIDUE 1
            REAL,       // the entire region
            PRIME,      // MODULAR 1 RESIDUE 0 (number is a prime)
            COMPOSITE   // MODULAR 1 RESIDUE 0 (number is a composite)
        };
        static Domain *Whole(Domain::Type type = Domain::REAL);
        static Domain *Intersect(const Domain *d1, const Domain *d2);
    };

    //
    // Ndrr1D::Domains
    //
    struct Domains : public std::vector<shared_ptr<Domain> >
    {
        Domains() { }
        Domains(const number_type& value)         { add(new Domain(value));     }
        Domains(const Aspect& a, const number_type& value)
                                                  { add(new Domain(a, value));  }
        Domains(const Aspect& a, const Ranges& r) { add(new Domain(a, r));      }
        Domains(const Domain& dom)                { add(new Domain(dom));       }
        Domains(const Domains& domains) :
            std::vector<shared_ptr<Domain> >(domains) { }

        Domains& operator=(const Domains& domains)
        {
            this->assign(domains.begin(), domains.end());
            return *this;
        }

        bool empty() const;
        bool entire() const;

        void add(Domain *d);

        void FixBeforeIncludes(Domain& domain);
        void FixBeforeIncludes(Domains& domains);

        // NOTE: You have to call this->FixBeforeIncludes(domain) before
        //       Domains::Includes(const Domain& domain) call.
        bool Includes(const Domain& domain) const;
        // NOTE: You have to call this->FixBeforeIncludes(domains) before
        //       Domains::Includes(const Domains& domains) call.
        bool Includes(const Domains& domains) const;
        bool Contains(const number_type& value) const;
        void Intersect(const Aspect& a);
        void Intersect(const Range& r);
        void Intersect(const Domain& domain);
        void Intersect(const Domains& domains);
        void Union(const Domain& domain);
        void Union(const Domains& domains);
        bool GetValues(std::vector<number_type>& values) const;
        void Optimize();
        void FixWider();
        void FixNarrower();
        bool Equal(const Domains& d) const;

        number_type *GetLBound() const;
        number_type *GetUBound() const;
        number_type *GetLBound(bool& has_min) const;
        number_type *GetUBound(bool& has_max) const;

        static Domains *Intersect(const Domains *domains, const Domain *domain);
        static Domains *Intersect(const Domains *d1, const Domains *d2);
        static Domains *Union(const Domains *domains, const Domain *domain);
        static Domains *Union(const Domains *d1, const Domains *d2);
        static Domains *Whole(Domain::Type type = Domain::REAL);
    };
} // namespace Ndrr1D

////////////////////////////////////////////////////////////////////////////
// compare two aspects

bool operator==(const Ndrr1D::Aspect& a1, const Ndrr1D::Aspect& a2);
bool operator!=(const Ndrr1D::Aspect& a1, const Ndrr1D::Aspect& a2);

////////////////////////////////////////////////////////////////////////////
// output

template <class charT, class traits>
std::basic_ostream<charT,traits>&
operator<<(std::basic_ostream<charT,traits>& os, const Ndrr1D::Range& r)
{
    os << "[ ";
    if (r.entire())
    {
        os << "entire";
    }
    else
    {
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
    }
    return os << " ]";
}

template <class charT, class traits>
std::basic_ostream<charT,traits>&
operator<<(std::basic_ostream<charT,traits>& os, const Ndrr1D::Ranges& r)
{
    os << "[ ";
    if (r.empty())
    {
        os << "empty";
    }
    else if (r.entire())
    {
        os << "entire";
    }
    else
    {
        for (size_t i = 0; i < r.size(); ++i)
        {
            os << *(r[i].get());
        }
    }
    return os << " ]";
}

template <class charT, class traits>
std::basic_ostream<charT,traits>&
operator<<(std::basic_ostream<charT,traits>& os, const Ndrr1D::Aspect& a)
{
    if (a.entire())
    {
        return os << "entire";
    }
    os << "[ modulus: ";
    if (a.m_pnModulus)
        os << *a.m_pnModulus;
    else
        os << "null";
    os << ", residue: ";
    if (a.m_pnResidue)
        os << *a.m_pnResidue;
    else
        os << "null";
    os << ", must_be_prime: ";
    if (a.m_must_be_prime)
        os << "true";
    else
        os << "false";
    return os << " ]";
}

template <class charT, class traits>
std::basic_ostream<charT,traits>&
operator<<(std::basic_ostream<charT,traits>& os, const Ndrr1D::Domain& d)
{
    return os << "[ ranges: " << d.m_ranges <<
        ", aspect: " << d.m_aspect << " ]";
}

template <class charT, class traits>
std::basic_ostream<charT,traits>&
operator<<(std::basic_ostream<charT,traits>& os, const Ndrr1D::Domains& d)
{
    os << "[ domains: ";
    if (d.empty())
    {
        os << "empty";
    }
    else if (d.entire())
    {
        os << "entire";
    }
    else
    {
        for (size_t i = 0; i < d.size(); ++i)
        {
            os << *(d[i].get());
        }
    }
    return os << " ]";
}

////////////////////////////////////////////////////////////////////////////

#endif  // ndef __NDRR1D__
