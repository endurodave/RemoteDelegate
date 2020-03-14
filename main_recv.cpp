#include "stdafx.h"
#include "Fault.h"
#include "RemoteId.h"
#include "RemoteDataPoint.h"
#ifdef RAPID_JSON
#include "RemoteNotificationJson.h"
#endif
#include "UdpDelegateRecv.h"
#include <iostream>
#include <conio.h>

// main.cpp
// @see https://www.codeproject.com/Articles/1160934/Asynchronous-Multicast-Delegates-in-Cplusplus
// David Lafreniere, Mar 2020.

using namespace DelegateLib;
using namespace std;

static void RecvDataPointCb(RemoteDataPoint& data)
{
    cout << "RemoteDataPoint: " << data.GetX() << " " << data.GetY() << endl;
}

#ifdef RAPID_JSON
static void RecvNotificationJsonCb(int count, RemoteNotificationJson& data)
{
    cout << "RemoteNotificationJson: " << count << " " << data.GetMsg() << endl;
}
#endif

//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------
int main(void)
{
    BOOL result = AfxWinInit(GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0);
    ASSERT_TRUE(result == TRUE);

    result = AfxSocketInit(NULL);
    ASSERT_TRUE(result == TRUE);

    UdpDelegateRecv::GetInstance().Initialize();

    // Create a receive remote delegate
    auto recvDataPointDelegate = MakeDelegate(&RecvDataPointCb, REMOTE_DATA_POINT_ID);

    // See RapidJSON_Readme.txt
#ifdef RAPID_JSON
    auto recvNotificationDelegate = MakeDelegate(&RecvNotificationJsonCb, REMOTE_NOTIFICATION_JSON_ID);
#endif

    cout << "Press any key to exit program." << endl;

    while (!_kbhit())
    {
        Sleep(10);
    }

	return 0;
}

