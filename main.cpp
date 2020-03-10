#include "stdafx.h"
#include "Fault.h"
#include "TestRemoteUdp.h"
#include "TestRemoteUdpAsync.h"
#include "UdpDelegateRecv.h"
#include "UdpDelegateSend.h"
#include "WorkerThreadWin.h"

// TODO 
// 1) Try RapidJSON
// 2) Try serializer
// 3) Try interprocess communication 
// 4) Unit tests

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

    // Register for asynchronous callbacks on workerThread1
    TestRemoteUdpAsync::DataPointRcvd += MakeDelegate(&RecvDataPointCb, &workerThread1);
    TestRemoteUdpAsync::NotificationRcvd += MakeDelegate(&RecvNotificationCb, &workerThread1);

    // Async send a data point to remote
    RemoteDataPoint dataPoint(111, 222);
    TestRemoteUdpAsync::GetInstance().SendDataPoint(dataPoint);

    // Async send a notification to remote
    RemoteNotification notification("Async Test!");
    TestRemoteUdpAsync::GetInstance().SendNotification(9, notification);

	// Run all unit tests (uncomment to run unit tests)
	//DelegateUnitTests();

    Sleep(1000);

    // Cleanup
    workerThread1.ExitThread();
    TestRemoteUdpAsync::DataPointRcvd -= MakeDelegate(&RecvDataPointCb, &workerThread1);
    TestRemoteUdpAsync::NotificationRcvd -= MakeDelegate(&RecvNotificationCb, &workerThread1);

	return 0;
}

