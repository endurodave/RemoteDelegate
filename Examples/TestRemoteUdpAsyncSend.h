#ifndef _TEST_REMOTE_UDP_ASYNC_SEND_H
#define _TEST_REMOTE_UDP_ASYNC_SEND_H

#include "DelegateLib.h"
#include "RemoteDataPoint.h"
#include "RemoteNotification.h"
#include <iostream>

/// @brief A class to send ansynchonously send outgoing remote delegates to 
/// a remote system using UDP.  
class TestRemoteUdpAsyncSend
{
public:
    static TestRemoteUdpAsyncSend& GetInstance();

    // Asynchronously send a data point to remote system
    void SendDataPoint(const RemoteDataPoint& data);

    // Asynchronously send a notification to remote system
    void SendNotification(int count, const RemoteNotification& data);

private:
    TestRemoteUdpAsyncSend();
    ~TestRemoteUdpAsyncSend();

    // Binary stream of send data bytes
    std::ostringstream oss;

    // Remote send delegates
    DelegateLib::DelegateRemoteSend1<const RemoteDataPoint&> sendDataPointDelegate;
    DelegateLib::DelegateRemoteSend2<int, const RemoteNotification&> sendNotificationDelegate;
};

#endif