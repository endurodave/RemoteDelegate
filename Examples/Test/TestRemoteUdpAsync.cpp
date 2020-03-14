#include "stdafx.h"
#include "TestRemoteUdpAsync.h"
#include "RemoteId.h"
#include "RemoteUdpAsyncSend.h"
#include "RemoteUdpAsyncRecv.h"
#include "DelegateLib.h"
#include "UdpDelegateRecv.h"
#include "UdpDelegateSend.h"
#include <sstream>
#include <iostream>

// This is a simple example of a remote send and receive delegate
// combined with asynchronous delegates.

using namespace DelegateLib;
using namespace std;

static void RecvDataPointAsyncCb(const RemoteDataPoint& data)
{
    cout << "AsyncRecvDataPointCb: " << data.GetX() << " " << data.GetY() << endl;
}

static void RecvNotificationAsyncCb(int count, const RemoteNotification& data)
{
    cout << "AsyncRecvNotificationCb: " << count << " " << data.GetMsg() << endl;
}

void TestRemoteUdpAsync()
{
    WorkerThread workerThread1("workerThread1");
    workerThread1.CreateThread();
    WorkerThread::StartAllThreads();

    RemoteUdpAsyncRecv::GetInstance();
    RemoteUdpAsyncSend::GetInstance();

    // Register for asynchronous callbacks on workerThread1
    RemoteUdpAsyncRecv::DataPointRcvd += MakeDelegate(&RecvDataPointAsyncCb, &workerThread1);
    RemoteUdpAsyncRecv::NotificationRcvd += MakeDelegate(&RecvNotificationAsyncCb, &workerThread1);

    // Async send a data point to remote
    RemoteDataPoint dataPoint(111, 222);
    RemoteUdpAsyncSend::GetInstance().SendDataPoint(dataPoint);

    // Async send a notification to remote
    RemoteNotification notification("Async RPC using UDP!");
    RemoteUdpAsyncSend::GetInstance().SendNotification(9, notification);

    Sleep(1000);

    workerThread1.ExitThread();
    RemoteUdpAsyncRecv::DataPointRcvd -= MakeDelegate(&RecvDataPointAsyncCb, &workerThread1);
    RemoteUdpAsyncRecv::NotificationRcvd -= MakeDelegate(&RecvNotificationAsyncCb, &workerThread1);
}
