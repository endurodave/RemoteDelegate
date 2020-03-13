#include "stdafx.h"
#include "Fault.h"
#include "TestRemoteUdp.h"
#include "TestSysData.h"
#include "TestRemoteUdpAsyncSend.h"
#include "TestRemoteUdpAsyncRecv.h"
#ifdef RAPID_JSON
#include "TestRemoteUdpJson.h"
#endif
#include "UdpDelegateRecv.h"
#include "UdpDelegateSend.h"
#include "WorkerThreadWin.h"
#include <iostream>

// main.cpp
// @see https://www.codeproject.com/Articles/1160934/Asynchronous-Multicast-Delegates-in-Cplusplus
// David Lafreniere, Mar 2020.

using namespace std;

WorkerThread workerThread1("workerThread1");

extern void DelegateUnitTests();

static void RecvDataPointCb(const RemoteDataPoint& data)
{
    cout << "WorkerThread1 RecvDataPointCb: " << data.GetX() << " " << data.GetY() << endl;
}

static void RecvNotificationCb(int count, const RemoteNotification& data)
{
    cout << "WorkerThread1 RecvNotificationCb: " << count << " " << data.GetMsg() << endl;
}

//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------
int main(void)
{
    BOOL result = AfxWinInit(GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0);
    ASSERT_TRUE(result == TRUE);

    result = AfxSocketInit(NULL);
    ASSERT_TRUE(result == TRUE);

    UpdDelegateSend::GetInstance().Initialize();
    UdpDelegateRecv::GetInstance().Initialize();

    workerThread1.CreateThread();
    WorkerThread::StartAllThreads();

    // Run a simple test
    TestRemoteUdp();

    // Run a test using SysData
    TestSysData();

    // See RapidJSON_Readme.txt
#ifdef RAPID_JSON
    // Run a simple JSON test
    TestRemoteUdpJson();
#endif

    TestRemoteUdpAsyncRecv::GetInstance();
    TestRemoteUdpAsyncSend::GetInstance();

    // Register for asynchronous callbacks on workerThread1
    TestRemoteUdpAsyncRecv::DataPointRcvd += MakeDelegate(&RecvDataPointCb, &workerThread1);
    TestRemoteUdpAsyncRecv::NotificationRcvd += MakeDelegate(&RecvNotificationCb, &workerThread1);

    // Async send a data point to remote
    RemoteDataPoint dataPoint(111, 222);
    TestRemoteUdpAsyncSend::GetInstance().SendDataPoint(dataPoint);

    // Async send a notification to remote
    RemoteNotification notification("Async Notification Message!");
    TestRemoteUdpAsyncSend::GetInstance().SendNotification(9, notification);

	// Run all unit tests (uncomment to run unit tests)
	//DelegateUnitTests();

    Sleep(1000);

    // Cleanup
    workerThread1.ExitThread();
    TestRemoteUdpAsyncRecv::DataPointRcvd -= MakeDelegate(&RecvDataPointCb, &workerThread1);
    TestRemoteUdpAsyncRecv::NotificationRcvd -= MakeDelegate(&RecvNotificationCb, &workerThread1);

	return 0;
}

