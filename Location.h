// Location.h

#ifndef LOCATION_H_
#define LOCATION_H_

//
// CH_Location
//
struct CH_Location
{
    std::string m_file;
    int m_line;

    CH_Location() : m_line(1) { }

    CH_Location(const CH_Location& loc)
    : m_file(loc.m_file), m_line(loc.m_line)
    {
    }

    CH_Location(const char *file, int line) : m_file(file), m_line(line) { }

    CH_Location(const std::string& file, int line) : m_file(file), m_line(line)
    {
    }

    void set(const char *file, int line)
    {
        m_file = file;
        m_line = line;
    }

    void operator=(const CH_Location& loc)
    {
        m_file = loc.m_file;
        m_line = loc.m_line;
    }

    CH_Location& operator++()
    {
        m_line++;
        return *this;
    }

    CH_Location operator++(int)
    {
        CH_Location loc(*this);
        m_line++;
        return loc;
    }

    std::string to_string() const
    {
        std::string str = m_file;
        char buf[32];
        std::sprintf(buf, " (%d)", m_line);
        str += buf;
        return str;
    }
};

#endif  // def LOCATION_H_
