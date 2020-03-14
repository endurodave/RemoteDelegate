#include "stdafx.h"
#include "Fault.h"
#include "RemoteId.h"
#include "RemoteDataPoint.h"
#ifdef RAPID_JSON
#include "RemoteNotificationJson.h"
#endif
#include "UdpDelegateSend.h"
#include "DelegateLib.h"
#include <iostream>
#include <conio.h>

// main.cpp
// @see https://www.codeproject.com/Articles/1160934/Asynchronous-Multicast-Delegates-in-Cplusplus
// David Lafreniere, Mar 2020.

using namespace DelegateLib;
using namespace std;

//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------
int main(void)
{
    BOOL result = AfxWinInit(GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0);
    ASSERT_TRUE(result == TRUE);

    result = AfxSocketInit(NULL);
    ASSERT_TRUE(result == TRUE);

    UdpDelegateSend::GetInstance().Initialize();

    // Create a stream to hold send data
    stringstream ss(ios::in | ios::out | ios::binary);

    // Create a send remote delegate
    auto sendDataPointDelegate =
        MakeDelegate<RemoteDataPoint&>(UdpDelegateSend::GetInstance(), ss, REMOTE_DATA_POINT_ID);

    // See RapidJSON_Readme.txt
#ifdef RAPID_JSON
    auto sendNotificationDelegate =
        MakeDelegate<int, RemoteNotificationJson&>(UdpDelegateSend::GetInstance(), ss, REMOTE_NOTIFICATION_JSON_ID);
#endif

    cout << "Press any key to exit program." << endl;

    int x = 1;
    int y = 1;
    int count = 1;

    while (!_kbhit())
    {
        // Send data point to remote system
        RemoteDataPoint dataPoint(x++, y++);
        sendDataPointDelegate(dataPoint);

#ifdef RAPID_JSON
        // Send notification to remote system
        RemoteNotificationJson notification("RPC using UDP and JSON!");
        notification.GetPoints().push_back(RemoteDataPointJson(x, y));
        sendNotificationDelegate(count++, notification);
#endif
        Sleep(5);
    }

	return 0;
}

