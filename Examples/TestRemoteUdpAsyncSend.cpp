#include "stdafx.h"
#include "TestRemoteUdpAsyncSend.h"
#include "RemoteId.h"
#include "DelegateLib.h"
#include "UdpDelegateRecv.h"
#include "UdpDelegateSend.h"

using namespace DelegateLib;
using namespace std;

static WorkerThread asyncWorkerThread("asyncWorkerThread");

TestRemoteUdpAsyncSend& TestRemoteUdpAsyncSend::GetInstance()
{
    static TestRemoteUdpAsyncSend instance;
    return instance;
}

TestRemoteUdpAsyncSend::TestRemoteUdpAsyncSend() :
    oss(ios::in | ios::out | ios::binary),
    sendDataPointDelegate(UpdDelegateSend::GetInstance(), oss, REMOTE_DATA_POINT_ID),
    sendNotificationDelegate(UpdDelegateSend::GetInstance(), oss, REMOTE_NOTIFICATION_ID)
{
    asyncWorkerThread.CreateThread();
}

TestRemoteUdpAsyncSend::~TestRemoteUdpAsyncSend()
{
    asyncWorkerThread.ExitThread();
}

void TestRemoteUdpAsyncSend::SendDataPoint(const RemoteDataPoint& data)
{
    // If caller is not executing on asyncWorkerThread then re-invoke 
    // the function call on the correct thread using a delegate.
    // Delete these two lines if prefer synchronous sending. 
    if (ThreadWin::GetCurrentThreadId() != asyncWorkerThread.GetThreadId())
        return MakeDelegate(this, &TestRemoteUdpAsyncSend::SendDataPoint, &asyncWorkerThread)(data);

    // Sending data to remote is always executed on asyncWorkerThread
    sendDataPointDelegate(data);
}

void TestRemoteUdpAsyncSend::SendNotification(int count, const RemoteNotification& data)
{
    // If caller is not executing on asyncWorkerThread then re-invoke 
    // the function call on the correct thread using a delegate.
    // Delete these two lines if prefer synchronous sending. 
    if (ThreadWin::GetCurrentThreadId() != asyncWorkerThread.GetThreadId())
        return MakeDelegate(this, &TestRemoteUdpAsyncSend::SendNotification, &asyncWorkerThread)(count, data);

    // Sending data to remote is always executed on asyncWorkerThread
    sendNotificationDelegate(count, data);
}

