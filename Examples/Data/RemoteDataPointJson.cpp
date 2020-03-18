#ifdef RAPID_JSON

#include "RemoteDataPointJson.h"
#include "prettywriter.h"
#include "document.h"
#include "stringbuffer.h"

using namespace rapidjson;

std::ostream& operator<< (std::ostream &out, const RemoteDataPointJson& data)
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
std::istream& operator>> (std::istream &in, RemoteDataPointJson& data)
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
    data.m_x = d["m_x"].GetInt();
    data.m_y = d["m_y"].GetInt();

    free(buf);
    return in;
}

#endif // RAPID_JSON
