#ifndef _REMOTE_NOTIFICATION_JSON_H
#define _REMOTE_NOTIFICATION_JSON_H

#ifdef RAPID_JSON

#include "RemoteDataPointJson.h"
#include <sstream>
#include <string>
#include <list>

/// @brief Shared data structure to send between remote systems. 
/// User defined remote delegate data arguments must have:
/// 1) A default constructor
/// 2) Define operator<< and operator>> functions for the type passed
struct RemoteNotificationJson
{
public:
    RemoteNotificationJson(std::string msg) : m_msg(msg) {}
    RemoteNotificationJson() : m_msg("") {}
    std::string GetMsg() const { return m_msg; }
    std::list<RemoteDataPointJson>& GetPoints() { return m_points; }

    template <typename Writer>
    void Serialize(Writer& writer) const
    {
        writer.StartObject();

        writer.String("m_msg");
        writer.String(m_msg.c_str());

        writer.String("m_points");
        writer.StartArray();

        for (auto it = m_points.begin(); it != m_points.end(); ++it)
            (*it).Serialize(writer);

        writer.EndArray();

        writer.EndObject();
    }

private:
    std::string m_msg;
    std::list<RemoteDataPointJson> m_points;

    // Support send/recv of RemoteNotificationJson& args using remote delegates
    friend std::ostream& operator<< (std::ostream &out, const RemoteNotificationJson& data);
    friend std::istream& operator>> (std::istream &in, RemoteNotificationJson& data);
};

#endif // RAPID_JSON

#endif