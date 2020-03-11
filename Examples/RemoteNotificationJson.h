#ifndef _REMOTE_NOTIFICATION_JSON_H
#define _REMOTE_NOTIFICATION_JSON_H

#ifdef RAPID_JSON

#include "rapidjson/include/rapidjson/writer.h"
#include <sstream>
#include <string>

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

private:
    std::string m_msg;

    // Support send/recv of RemoteNotificationJson& args using remote delegates
    friend std::ostream& operator<< (std::ostream &out, const RemoteNotificationJson& data);
    friend std::istream& operator>> (std::istream &in, RemoteNotificationJson& data);

    template <typename Writer>
    void Serialize(Writer& writer) const
    {
        writer.StartObject();

        writer.String("m_msg");
        writer.String(m_msg.c_str());

        writer.EndObject();
    }
};

#endif // RAPID_JSON

#endif