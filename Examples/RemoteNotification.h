#ifndef _REMOTE_NOTIFICATION_H
#define _REMOTE_NOTIFICATION_H

#include <sstream>
#include <string>

/// @brief Shared data structure to send between remote systems. 
/// User defined remote delegate data arguments must have:
/// 1) A default constructor
/// 2) Define operator<< and operator>> functions for the type passed
struct RemoteNotification
{
public:
    RemoteNotification(std::string msg) : m_msg(msg) {}
    RemoteNotification() : m_msg("") {}
    std::string GetMsg() const { return m_msg; }

private:
    std::string m_msg;

    // Support send/recv of RemoteNotification& args using remote delegates
    friend std::ostream& operator<< (std::ostream &out, const RemoteNotification& data);
    friend std::istream& operator>> (std::istream &in, RemoteNotification& data);
};

#endif