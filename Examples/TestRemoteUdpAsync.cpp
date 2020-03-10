#include "stdafx.h"
#include "TestRemoteUdpAsync.h"
#include "RemoteId.h"
#include "DelegateLib.h"
#include "UdpDelegateRecv.h"
#include "UdpDelegateSend.h"


// This is a simple example of a remote send and receive delegate
// encapulated into a class. Public callback delegates notify 
// subscribers. 
//
// The sender delegate sends a message over UDP. The receiver 
// delgate receives the message and calls back the function.  
// For sample code simplicity, the socket send/receive is on the 
// same CPU. Normally the send delegate is on CPU #1 and the 
// receive delegate on CPU #2.

using namespace DelegateLib;
using namespace std;

static WorkerThread asyncWorkerThread("asyncWorkerThread");

MulticastDelegateSafe1<const RemoteDataPoint&> TestRemoteUdpAsync::DataPointRcvd;
MulticastDelegateSafe2<int, const RemoteNotification&> TestRemoteUdpAsync::NotificationRcvd;

TestRemoteUdpAsync& TestRemoteUdpAsync::GetInstance()
{
    static TestRemoteUdpAsync instance;
    return instance;
}

TestRemoteUdpAsync::TestRemoteUdpAsync() :
    oss(ios::in | ios::out | ios::binary),
    sendDataPointDelegate(UpdDelegateSend::GetInstance(), oss, REMOTE_DATA_POINT_ID),
    sendNotificationDelegate(UpdDelegateSend::GetInstance(), oss, REMOTE_NOTIFICATION_ID),
    recvDataPointDelegate(&TestRemoteUdpAsync::RecvDataPointCb, REMOTE_DATA_POINT_ID),
    recvNotificationDelegate(this, &TestRemoteUdpAsync::RecvNotificationCb, REMOTE_NOTIFICATION_ID)
{
    asyncWorkerThread.CreateThread();
}

void TestRemoteUdpAsync::SendDataPoint(const RemoteDataPoint& data)
{
    // If caller is not executing on asyncWorkerThread then re-invoke 
    // on the correct thread using a delegate
    if (ThreadWin::GetCurrentThreadId() != asyncWorkerThread.GetThreadId())
        return MakeDelegate(this, &TestRemoteUdpAsync::SendDataPoint, &asyncWorkerThread)(data);

    // Sending data to remote is always executed on asyncWorkerThread
    sendDataPointDelegate(data);
}

void TestRemoteUdpAsync::SendNotification(int count, const RemoteNotification& data)
{
    // If caller is not executing on asyncWorkerThread then re-invoke 
    // on the correct thread using a delegate
    if (ThreadWin::GetCurrentThreadId() != asyncWorkerThread.GetThreadId())
        return MakeDelegate(this, &TestRemoteUdpAsync::SendNotification, &asyncWorkerThread)(count, data);

    // Sending data to remote is always executed on asyncWorkerThread
    sendNotificationDelegate(count, data);
}

void TestRemoteUdpAsync::RecvDataPointCb(RemoteDataPoint& data)
{
    // Callback registered client(s), if any
    DataPointRcvd(data);

    cout << "TestRemoteUdpAsync::RecvDataPointCb: " << data.GetX() << " " << data.GetY() << endl;
}

void TestRemoteUdpAsync::RecvNotificationCb(int count, RemoteNotification& data)
{
    // Callback registered client(s), if any
    NotificationRcvd(count, data);

    cout << "TestRemoteUdpAsync::RecvNotificationCb: " << count << " " << data.GetMsg() << endl;
}
