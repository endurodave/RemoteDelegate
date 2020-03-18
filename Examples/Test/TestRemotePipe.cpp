#ifdef WIN32
#include "stdafx.h"
#endif
#include "TestRemotePipe.h"
#include "RemoteId.h"
#include "RemoteDataPoint.h"
#include "RemoteNotification.h"
#include "DelegateLib.h"
#include "PipeDelegateSend.h"
#include <sstream>
#include <iostream>
#include <chrono>
#include <thread>

// This is a simple example of a remote send and receive delegate
// calling back functions using a named pipe. 

using namespace DelegateLib;
using namespace std;

static void RecvDataPointPipeCb(RemoteDataPoint& data)
{
    cout << "RecvDataPointPipeCb: " << data.GetX() << " " << data.GetY() << endl;
}

static void RecvNotificationPipeCb(int count, RemoteNotification& data)
{
    cout << "RecvNotificationPipeCb: " << count << " " << data.GetMsg() << endl;
}

void TestRemotePipe()
{
    // Binary stream of send data bytes
    stringstream ss(ios::in | ios::out | ios::binary);

    // Send delegates (using MakeDelegate)
    auto sendDataPointDelegate =
        MakeDelegate<RemoteDataPoint&>(PipeDelegateSend::GetInstance(), ss, REMOTE_DATA_POINT_ID);
    auto sendNotificationDelegate =
        MakeDelegate<int, RemoteNotification&>(PipeDelegateSend::GetInstance(), ss, REMOTE_NOTIFICATION_ID);

    // Receive delegates (using MakeDelegate)
    auto recvDataPointDelegate = MakeDelegate(&RecvDataPointPipeCb, REMOTE_DATA_POINT_ID);
    auto recvNotificationDelegate = MakeDelegate(&RecvNotificationPipeCb, REMOTE_NOTIFICATION_ID);

    int x = 100;
    int y = 9990;
    int count = 1;

    for (int i = 0; i < 5; i++)
    {
        // Send data point to remote system
        RemoteDataPoint dataPoint(x++, y++);
        sendDataPointDelegate(dataPoint);

        // Send notification to remote system
        RemoteNotification notification("RPC using named pipe!");
        sendNotificationDelegate(count++, notification);
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
}
