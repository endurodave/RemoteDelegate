#include "RemoteNotification.h"

// The insert/exract functions below write the object data to a stream.
// Here the data is output as binary. Read/write the object data however  
// you wish (JSON, XML, binary, ...)

std::ostream& operator<< (std::ostream &out, const RemoteNotification& data)
{
    size_t size = data.m_msg.size();
    out.write(reinterpret_cast<char*>(&size), sizeof(size_t));
    out.write(data.m_msg.c_str(), size);
    return out;
}
std::istream& operator>> (std::istream &in, RemoteNotification& data)
{
    size_t size = 0;
    in.read(reinterpret_cast<char*>(&size), sizeof(size_t));
    data.m_msg.resize(size);
    in.read(const_cast<char*>(data.m_msg.c_str()), size);
    return in;
}
