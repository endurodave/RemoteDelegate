#include "RemoteDataPoint.h"

// The insert/exract functions below write the object data to a stream.
// Here the data is output as characters. Read/write the object data however  
// you wish (JSON, XML, binary, ...)

std::ostream& operator<< (std::ostream &out, const RemoteDataPoint& data)
{
    out << data.m_x << std::endl;
    out << data.m_y << std::endl;
    return out;
}
std::ostream& operator<< (std::ostream &out, const RemoteDataPoint* data)
{
    out << data->m_x << std::endl;
    out << data->m_y << std::endl;
    return out;
}
std::ostream& operator<< (std::ostream &out, RemoteDataPoint** data)
{
    out << (*data)->m_x << std::endl;
    out << (*data)->m_y << std::endl;
    return out;
}
std::istream& operator>> (std::istream &in, RemoteDataPoint& data)
{
    in >> data.m_x;
    in >> data.m_y;
    return in;
}
std::istream& operator>> (std::istream &in, RemoteDataPoint* data)
{
    in >> data->m_x;
    in >> data->m_y;
    return in;
}

std::istream& operator>> (std::istream &in, RemoteDataPoint** data)
{
    in >> (*data)->m_x;
    in >> (*data)->m_y;
    return in;
}

