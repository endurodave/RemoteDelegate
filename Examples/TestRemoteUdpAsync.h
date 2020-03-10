#ifndef _TEST_REMOTE_UDP_ASYNC_H
#define _TEST_REMOTE_UDP_ASYNC_H

#include "DelegateLib.h"
#include "RemoteDataPoint.h"
#include "RemoteNotification.h"
#include <iostream>

/// @brief A class to show ansynchonous incoming and outgoing interfaces
/// between remote systems. Remote send/recv delegates and asynchronous 
/// delegates are used. This class is both a sender and receiver to 
/// simplify running the sample code. Normally one CPU is a sender and 
/// another is receiver. 
class TestRemoteUdpAsync
{
public:
    // Register for remote receive notification callbacks with these two delegates.
    // Client can register for synhronous or asynchronous notification.
    static DelegateLib::MulticastDelegateSafe1<const RemoteDataPoint&> DataPointRcvd;
    static DelegateLib::MulticastDelegateSafe2<int, const RemoteNotification&> NotificationRcvd;

    static TestRemoteUdpAsync& GetInstance();

    // Asynchronously send a data point to remote system
    void SendDataPoint(const RemoteDataPoint& data);

    // Asynchronously send a notification to remote system
    void SendNotification(int count, const RemoteNotification& data);

private:
    TestRemoteUdpAsync();

    // Receive data point static callback
    static void RecvDataPointCb(RemoteDataPoint& data);

    // Receive notification instance callback
    void RecvNotificationCb(int count, RemoteNotification& data);

    // Binary stream of send data bytes
    std::ostringstream oss;

    // Remote send delegates
    DelegateLib::DelegateRemoteSend1<const RemoteDataPoint&> sendDataPointDelegate;
    DelegateLib::DelegateRemoteSend2<int, const RemoteNotification&> sendNotificationDelegate;

    // Remote receive delegates 
    DelegateLib::DelegateFreeRemoteRecv1<RemoteDataPoint&> recvDataPointDelegate;
    DelegateLib::DelegateMemberRemoteRecv2<TestRemoteUdpAsync, int, RemoteNotification&> recvNotificationDelegate;
};

#endif