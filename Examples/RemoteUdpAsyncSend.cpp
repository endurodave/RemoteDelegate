#include "stdafx.h"
#include "RemoteUdpAsyncSend.h"
#include "RemoteId.h"
#include "DelegateLib.h"
#include "UdpDelegateRecv.h"
#include "UdpDelegateSend.h"

using namespace DelegateLib;
using namespace std;

static WorkerThread asyncWorkerThread("asyncWorkerThread");

RemoteUdpAsyncSend& RemoteUdpAsyncSend::GetInstance()
{
    static RemoteUdpAsyncSend instance;
    return instance;
}

RemoteUdpAsyncSend::RemoteUdpAsyncSend() :
    ss(ios::in | ios::out | ios::binary),
    sendDataPointDelegate(UdpDelegateSend::GetInstance(), ss, REMOTE_DATA_POINT_ID),
    sendNotificationDelegate(UdpDelegateSend::GetInstance(), ss, REMOTE_NOTIFICATION_ID)
{
    asyncWorkerThread.CreateThread();
}

RemoteUdpAsyncSend::~RemoteUdpAsyncSend()
{
    asyncWorkerThread.ExitThread();
}

void RemoteUdpAsyncSend::SendDataPoint(const RemoteDataPoint& data)
{
    // If caller is not executing on asyncWorkerThread then re-invoke 
    // the function call on the correct thread using a delegate.
    // Delete these two lines if prefer synchronous sending. 
    if (ThreadWin::GetCurrentThreadId() != asyncWorkerThread.GetThreadId())
        return MakeDelegate(this, &RemoteUdpAsyncSend::SendDataPoint, &asyncWorkerThread)(data);

    // Sending data to remote is always executed on asyncWorkerThread
    sendDataPointDelegate(data);
}

void RemoteUdpAsyncSend::SendNotification(int count, const RemoteNotification& data)
{
    // If caller is not executing on asyncWorkerThread then re-invoke 
    // the function call on the correct thread using a delegate.
    // Delete these two lines if prefer synchronous sending. 
    if (ThreadWin::GetCurrentThreadId() != asyncWorkerThread.GetThreadId())
        return MakeDelegate(this, &RemoteUdpAsyncSend::SendNotification, &asyncWorkerThread)(count, data);

    // Sending data to remote is always executed on asyncWorkerThread
    sendNotificationDelegate(count, data);
}

