#ifdef RAPID_JSON

#include "RemoteNotificationJson.h"
#include "rapidjson/include/rapidjson/prettywriter.h"
#include "rapidjson/include/rapidjson/document.h"
#include "rapidjson/include/rapidjson/stringbuffer.h"

using namespace rapidjson;

std::ostream& operator<< (std::ostream &out, const RemoteNotificationJson& data)
{
    StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);

    // Serialize object using JSON
    data.Serialize(writer);

    // Add JSON length 
    // Note: Sender handle endianness here for binary bufLen if necessary
    size_t bufLen = sb.GetLength() + 1;
    out.write(reinterpret_cast<const char*>(&bufLen), sizeof(size_t));

    // Add JSON string
    out << sb.GetString();
    out << std::ends;
    return out;
}
std::istream& operator>> (std::istream &in, RemoteNotificationJson& data)
{
    // Get JSON length
    // Note: Receiver handle endianness here for binary bufLen if necessary
    size_t bufLen = 0;
    in.read(reinterpret_cast<char*>(&bufLen), sizeof(size_t));

    // Allocate storage buffer
    char* buf = (char*)malloc(bufLen);

    // Copy JSON into buffer 
    in.rdbuf()->sgetn(buf, bufLen);

    // Parse JSON
    Document d;
    d.Parse(buf);

    // Get JSON values into object variables
    data.m_msg = d["m_msg"].GetString();

    free(buf);
    return in;
}

#endif // RAPID_JSON
