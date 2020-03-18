#ifdef WIN32
#include "stdafx.h"
#endif
#include "RemoteUdpAsyncRecv.h"
#include "RemoteId.h"
#include "DelegateLib.h"
#include "UdpDelegateRecv.h"
#include "UdpDelegateSend.h"

using namespace DelegateLib;
using namespace std;

MulticastDelegateSafe1<const RemoteDataPoint&> RemoteUdpAsyncRecv::DataPointRcvd;
MulticastDelegateSafe2<int, const RemoteNotification&> RemoteUdpAsyncRecv::NotificationRcvd;

RemoteUdpAsyncRecv& RemoteUdpAsyncRecv::GetInstance()
{
    static RemoteUdpAsyncRecv instance;
    return instance;
}

RemoteUdpAsyncRecv::RemoteUdpAsyncRecv() :
    recvDataPointDelegate(&RemoteUdpAsyncRecv::RecvDataPointCb, REMOTE_DATA_POINT_ID),
    recvNotificationDelegate(this, &RemoteUdpAsyncRecv::RecvNotificationCb, REMOTE_NOTIFICATION_ID)
{
}

RemoteUdpAsyncRecv::~RemoteUdpAsyncRecv()
{
}

void RemoteUdpAsyncRecv::RecvDataPointCb(RemoteDataPoint& data)
{
    // Callback registered client(s), if any
    DataPointRcvd(data);
    cout << "RemoteUdpAsyncRecv::RecvDataPointCb: " << data.GetX() << " " << data.GetY() << endl;
}

void RemoteUdpAsyncRecv::RecvNotificationCb(int count, RemoteNotification& data)
{
    // Callback registered client(s), if any
    NotificationRcvd(count, data);
    cout << "RemoteUdpAsyncRecv::RecvNotificationCb: " << count << " " << data.GetMsg() << endl;
}
