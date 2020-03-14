#include "stdafx.h"
#include "TestSysData.h"
#include "RemoteId.h"
#include "SysData.h"
#include "DelegateLib.h"
#include "WorkerThreadWin.h"
#include "UdpDelegateRecv.h"
#include "UdpDelegateSend.h"
#include <sstream>
#include <iostream>

// This is an publish/subscribe example showing different types of 
// callbacks registered with SysData.

using namespace DelegateLib;
using namespace std;

// Worker thread used to receive asynchronous callbacks
static WorkerThread sysDataWorkerThread("sysDataWorkerThread");

// Callback function to receive notifications
static void SystemModeChangedCb(const SystemModeChanged& data)
{
    cout << "SystemModeChangedCb: " << data.CurrentSystemMode << " " << data.PreviousSystemMode << endl;
}

void TestSysData()
{
    sysDataWorkerThread.CreateThread();
    WorkerThread::StartAllThreads();

    // Binary stream of send data bytes
    stringstream ss(ios::in | ios::out | ios::binary);

    // Register to send a remote callback
    SysData::SystemModeChangedDelegate +=
        MakeDelegate<const SystemModeChanged&>(UdpDelegateSend::GetInstance(), ss, REMOTE_SYSTEM_MODE_CHANGED_ID);

    // Create a receive delegate to receive remote callback
    auto recvDataPointDelegate = MakeDelegate(&SystemModeChangedCb, REMOTE_SYSTEM_MODE_CHANGED_ID);

    // Register for synchronous callback
    SysData::SystemModeChangedDelegate += MakeDelegate(&SystemModeChangedCb);

    // Register for asynchronous callback on a worker thread
    SysData::SystemModeChangedDelegate += MakeDelegate(&SystemModeChangedCb, &sysDataWorkerThread);

    // Register for asynchronous blocking callback on a worker thread
    SysData::SystemModeChangedDelegate += MakeDelegate(&SystemModeChangedCb, &sysDataWorkerThread, 5000);

    // Change system mode. All registered subscribers are notified. 
    SysData::GetInstance().SetSystemMode(SystemMode::STARTING);
    SysData::GetInstance().SetSystemMode(SystemMode::NORMAL);

    Sleep(1000);

    // Unregister all callbacks
    SysData::SystemModeChangedDelegate -= MakeDelegate(&SystemModeChangedCb);
    SysData::SystemModeChangedDelegate -= MakeDelegate(&SystemModeChangedCb, &sysDataWorkerThread);
    SysData::SystemModeChangedDelegate -= MakeDelegate(&SystemModeChangedCb, &sysDataWorkerThread, 5000);
    SysData::SystemModeChangedDelegate -=
        MakeDelegate<const SystemModeChanged&>(UdpDelegateSend::GetInstance(), ss, REMOTE_SYSTEM_MODE_CHANGED_ID);

    sysDataWorkerThread.ExitThread();
}
