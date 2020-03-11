#ifndef _REMOTE_DATA_POINT_JSON_H
#define _REMOTE_DATA_POINT_JSON_H

#ifdef RAPID_JSON

#include "rapidjson/include/rapidjson/writer.h"
#include <sstream>

/// @brief Shared data structure to send between remote systems. 
/// User defined remote delegate data arguments must have:
/// 1) A default constructor
/// 2) Define operator<< and operator>> functions for the type passed
struct RemoteDataPointJson
{
public:
    RemoteDataPointJson(int x, int y) : m_x(x), m_y(y) {}
    RemoteDataPointJson() : m_x(0), m_y(0) {}
    int GetX() const { return m_x; }
    int GetY() const { return m_y; }

private:
    int m_x;
    int m_y;

    // Support send/recv of RemoteDataPointJson& args using remote delegates
    friend std::ostream& operator<< (std::ostream &out, const RemoteDataPointJson& data);
    friend std::istream& operator>> (std::istream &in, RemoteDataPointJson& data);

    template <typename Writer>
    void Serialize(Writer& writer) const
    {
        writer.StartObject();

        writer.String("m_x");
        writer.Int(m_x);
        writer.String("m_y");
        writer.Int(m_y);

        writer.EndObject();
    }
};

#endif // RAPID_JSON

#endif