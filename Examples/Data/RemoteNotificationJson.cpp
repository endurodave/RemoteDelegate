#ifdef RAPID_JSON

#include "Fault.h"
#include "RemoteNotificationJson.h"
#include "prettywriter.h"
#include "document.h"
#include "stringbuffer.h"

using namespace rapidjson;

std::ostream& operator<< (std::ostream &out, const RemoteNotificationJson& data)
{
    StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);

    // Serialize object using JSON
    data.Serialize(writer);

    // Add JSON length 
    out << sb.GetLength() + 1;

    // Add JSON string
    out << sb.GetString();
    return out;
}
std::istream& operator>> (std::istream &in, RemoteNotificationJson& data)
{
    // Get JSON length
    size_t bufLen = 0;
    in >> bufLen;

    // Allocate storage buffer
    char* buf = (char*)malloc(bufLen);
    ASSERT_TRUE(buf != NULL);

    // Copy JSON into buffer 
    in.rdbuf()->sgetn(buf, bufLen);

    // Parse JSON
    Document d;
    d.Parse(buf);

    // Get JSON values into object variables
    data.m_msg = d["m_msg"].GetString();

    const Value& points = d["m_points"];
    for (auto& point : points.GetArray())
    {
        RemoteDataPointJson p(point["m_x"].GetInt(), point["m_y"].GetInt());
        data.m_points.push_back(p);
    }

    free(buf);
    return in;
}

#endif // RAPID_JSON
