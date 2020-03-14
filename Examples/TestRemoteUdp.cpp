#include "stdafx.h"
#include "TestRemoteUdp.h"
#include "RemoteId.h"
#include "RemoteDataPoint.h"
#include "RemoteNotification.h"
#include "DelegateLib.h"
#include "UdpDelegateRecv.h"
#include "UdpDelegateSend.h"
#include <sstream>
#include <iostream>

// This is a simple example of a remote send and receive delegate
// calling back functions. 
//
// The sender delegate sends a message over UDP. The receiver 
// delgate receives the message and calls back the function.  
// For sample code simplicity, the socket send/receive is on the 
// same CPU. Normally the send delegate is on CPU #1 and the 
// receive delegate on CPU #2.

using namespace DelegateLib;
using namespace std;

static void RecvDataPointCb(RemoteDataPoint* data)
{
    cout << "RecvDataPointCb: " << data->GetX() << " " << data->GetY() << endl;
}

static void RecvNotificationCb(int count, RemoteNotification& data)
{
    cout << "RecvNotificationCb: " << count << " " << data.GetMsg() << endl;
}

void TestRemoteUdp()
{
    // Binary stream of send data bytes
    stringstream ss(ios::in | ios::out | ios::binary);

#if 1
    // Send delegates (using MakeDelegate)
    auto sendDataPointDelegate =
        MakeDelegate<RemoteDataPoint*>(UpdDelegateSend::GetInstance(), ss, REMOTE_DATA_POINT_ID);
    auto sendNotificationDelegate =
        MakeDelegate<int, RemoteNotification&>(UpdDelegateSend::GetInstance(), ss, REMOTE_NOTIFICATION_ID);

    // Receive delegates (using MakeDelegate)
    auto recvDataPointDelegate = MakeDelegate(&RecvDataPointCb, REMOTE_DATA_POINT_ID);
    auto recvNotificationDelegate = MakeDelegate(&RecvNotificationCb, REMOTE_NOTIFICATION_ID);
#else
    // Send delegates (no MakeDelegate)
    DelegateRemoteSend1<RemoteDataPoint*> 
        sendDataPointDelegate(UpdDelegateSend::GetInstance(), oss, REMOTE_DATA_POINT_ID);
    DelegateRemoteSend2<int, RemoteNotification&>
        sendNotificationDelegate(UpdDelegateSend::GetInstance(), oss, REMOTE_NOTIFICATION_ID);

    // Receive delegates (no MakeDelegate)
    DelegateFreeRemoteRecv1<RemoteDataPoint*> recvDataPointDelegate(&RecvDataPointCb, REMOTE_DATA_POINT_ID);
    DelegateFreeRemoteRecv2<int, RemoteNotification&> recvNotificationDelegate(&RecvNotificationCb, REMOTE_NOTIFICATION_ID);

#endif

    int x = 100;
    int y = 9990;
    int count = 1;

    for (int i = 0; i < 10; i++)
    {
        // Send data point to remote system
        RemoteDataPoint dataPoint(x++, y++);
        sendDataPointDelegate(&dataPoint);

        // Send notification to remote system
        RemoteNotification notification("Hello World!");
        sendNotificationDelegate(count++, notification);
    }

    Sleep(1000);
}
