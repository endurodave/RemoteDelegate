#ifndef _TEST_REMOTE_UDP_ASYNC_RECV_H
#define _TEST_REMOTE_UDP_ASYNC_RECV_H

#include "DelegateLib.h"
#include "RemoteDataPoint.h"
#include "RemoteNotification.h"
#include <iostream>

/// @brief A class to receive incoming remote delegates from a remote 
/// system using UDP.
class TestRemoteUdpAsyncRecv
{
public:
    // Register for remote receive notification callbacks with these two delegates.
    // Client can register for synhronous or asynchronous notification.
    static DelegateLib::MulticastDelegateSafe1<const RemoteDataPoint&> DataPointRcvd;
    static DelegateLib::MulticastDelegateSafe2<int, const RemoteNotification&> NotificationRcvd;

    static TestRemoteUdpAsyncRecv& GetInstance();

private:
    TestRemoteUdpAsyncRecv();
    ~TestRemoteUdpAsyncRecv();

    // Receive data point static callback
    static void RecvDataPointCb(RemoteDataPoint& data);

    // Receive notification instance callback
    void RecvNotificationCb(int count, RemoteNotification& data);

    // Remote receive delegates 
    DelegateLib::DelegateFreeRemoteRecv1<RemoteDataPoint&> recvDataPointDelegate;
    DelegateLib::DelegateMemberRemoteRecv2<TestRemoteUdpAsyncRecv, int, RemoteNotification&> recvNotificationDelegate;
};

#endif