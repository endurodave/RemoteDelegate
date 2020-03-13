#include "stdafx.h"
#include "TestRemoteUdpAsyncRecv.h"
#include "RemoteId.h"
#include "DelegateLib.h"
#include "UdpDelegateRecv.h"
#include "UdpDelegateSend.h"

using namespace DelegateLib;
using namespace std;

MulticastDelegateSafe1<const RemoteDataPoint&> TestRemoteUdpAsyncRecv::DataPointRcvd;
MulticastDelegateSafe2<int, const RemoteNotification&> TestRemoteUdpAsyncRecv::NotificationRcvd;

TestRemoteUdpAsyncRecv& TestRemoteUdpAsyncRecv::GetInstance()
{
    static TestRemoteUdpAsyncRecv instance;
    return instance;
}

TestRemoteUdpAsyncRecv::TestRemoteUdpAsyncRecv() :
    recvDataPointDelegate(&TestRemoteUdpAsyncRecv::RecvDataPointCb, REMOTE_DATA_POINT_ID),
    recvNotificationDelegate(this, &TestRemoteUdpAsyncRecv::RecvNotificationCb, REMOTE_NOTIFICATION_ID)
{
}

TestRemoteUdpAsyncRecv::~TestRemoteUdpAsyncRecv()
{
}

void TestRemoteUdpAsyncRecv::RecvDataPointCb(RemoteDataPoint& data)
{
    // Callback registered client(s), if any
    DataPointRcvd(data);
    cout << "TestRemoteUdpAsyncRecv::RecvDataPointCb: " << data.GetX() << " " << data.GetY() << endl;
}

void TestRemoteUdpAsyncRecv::RecvNotificationCb(int count, RemoteNotification& data)
{
    // Callback registered client(s), if any
    NotificationRcvd(count, data);
    cout << "TestRemoteUdpAsyncRecv::RecvNotificationCb: " << count << " " << data.GetMsg() << endl;
}
