#include "RemoteNotification.h"

// The insert/exract functions below write the object data to a stream.
// Here the data is output as binary. Read/write the object data however  
// you wish (JSON, XML, binary, ...)

std::ostream& operator<< (std::ostream &out, const RemoteNotification& data)
{
    out << data.m_msg << std::endl;
    return out;
}
std::istream& operator>> (std::istream &in, RemoteNotification& data)
{
    std::getline(in, data.m_msg);
    return in;
}
