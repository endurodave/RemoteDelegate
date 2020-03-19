#include "RemoteDataPointBinary.h"

// The insert/exract functions below write the object data to a stream.
// Here the data is output as binary. Read/write the object data however  
// you wish (JSON, XML, binary, ...)

std::ostream& operator<< (std::ostream &out, const RemoteDataPointBinary& data)
{
    out.write(reinterpret_cast<const char*>(&data.m_x), sizeof(int));
    out.write(reinterpret_cast<const char*>(&data.m_y), sizeof(int));
    return out;
}
std::ostream& operator<< (std::ostream &out, const RemoteDataPointBinary* data)
{
    out.write(reinterpret_cast<const char*>(&data->m_x), sizeof(int));
    out.write(reinterpret_cast<const char*>(&data->m_y), sizeof(int));
    return out;
}
std::ostream& operator<< (std::ostream &out, RemoteDataPointBinary** data)
{
    out.write(reinterpret_cast<const char*>(&(*data)->m_x), sizeof(int));
    out.write(reinterpret_cast<const char*>(&(*data)->m_y), sizeof(int));
    return out;
}
std::istream& operator>> (std::istream &in, RemoteDataPointBinary& data)
{
    in.read(reinterpret_cast<char*>(&data.m_x), sizeof(int));
    in.read(reinterpret_cast<char*>(&data.m_y), sizeof(int));
    return in;
}
std::istream& operator>> (std::istream &in, RemoteDataPointBinary* data)
{
    in.read(reinterpret_cast<char*>(&data->m_x), sizeof(int));
    in.read(reinterpret_cast<char*>(&data->m_y), sizeof(int));
    return in;
}

std::istream& operator>> (std::istream &in, RemoteDataPointBinary** data)
{
    in.read(reinterpret_cast<char*>(&(*data)->m_x), sizeof(int));
    in.read(reinterpret_cast<char*>(&(*data)->m_y), sizeof(int));
    return in;
}

