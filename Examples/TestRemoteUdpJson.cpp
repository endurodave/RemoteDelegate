#ifdef RAPID_JSON

#include "stdafx.h"
#include "TestRemoteUdpJson.h"
#include "RemoteId.h"
#include "RemoteDataPointJson.h"
#include "RemoteNotificationJson.h"
#include "DelegateLib.h"
#include "UdpDelegateRecv.h"
#include "UdpDelegateSend.h"
#include <iostream>

// This is a simple example of a remote send and receive delegate
// calling back functions using JSON. 
//
// The sender delegate sends a message over UDP. The receiver 
// delgate receives the message and calls back the function.  
// For sample code simplicity, the socket send/receive is on the 
// same CPU. Normally the send delegate is on CPU #1 and the 
// receive delegate on CPU #2.

using namespace DelegateLib;
using namespace std;

static void RecvDataPointJsonCb(RemoteDataPointJson& data)
{
    cout << "RecvDataPointJsonCb: " << data.GetX() << " " << data.GetY() << endl;
}

static void RecvNotificationJsonCb(int count, RemoteNotificationJson& data)
{
    cout << "RecvNotificationJsonCb: " << count << " " << data.GetMsg() << endl;
}

void TestRemoteUdpJson()
{
    // Binary stream of send data bytes
    stringstream ss(ios::in | ios::out | ios::binary);

    // Send delegates
    auto sendDataPointDelegate =
        MakeDelegate<RemoteDataPointJson&>(UdpDelegateSend::GetInstance(), ss, REMOTE_DATA_POINT_JSON_ID);
    auto sendNotificationDelegate =
        MakeDelegate<int, RemoteNotificationJson&>(UdpDelegateSend::GetInstance(), ss, REMOTE_NOTIFICATION_JSON_ID);

    // Receive delegates
    auto recvDataPointDelegate = MakeDelegate(&RecvDataPointJsonCb, REMOTE_DATA_POINT_JSON_ID);
    auto recvNotificationDelegate = MakeDelegate(&RecvNotificationJsonCb, REMOTE_NOTIFICATION_JSON_ID);

    int x = 100;
    int y = 9990;
    int count = 1;

    for (int i = 0; i < 10; i++)
    {
        // Send data point to remote system
        RemoteDataPointJson dataPoint(x++, y++);
        sendDataPointDelegate(dataPoint);

        // Send notification to remote system
        RemoteNotificationJson notification("Hello World!");
        notification.GetPoints().push_back(dataPoint);
        sendNotificationDelegate(count++, notification);
    }

    Sleep(1000);
}

#endif // RAPID_JSON
