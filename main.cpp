#include "stdafx.h"
#include "Fault.h"
#include "TestRemoteUdp.h"
#include "TestSysData.h"
#include "TestRemoteUdpAsync.h"
#ifdef RAPID_JSON
#include "TestRemoteUdpJson.h"
#endif
#include "UdpDelegateRecv.h"
#include "UdpDelegateSend.h"
#include "WorkerThreadWin.h"
#include <iostream>

// TODO - need to clear stream?
// TODO - test over long periods at full speed for leaks, etc...

// main.cpp
// @see https://www.codeproject.com/Articles/1160934/Asynchronous-Multicast-Delegates-in-Cplusplus
// David Lafreniere, Mar 2020.

using namespace std;

extern void DelegateUnitTests();

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

    // Run a simple test
    //TestRemoteUdp();

    // Run a simple async test
    //TestRemoteUdpAsync();

    // Run a test using SysData
    //TestSysData();

    // See RapidJSON_Readme.txt
#ifdef RAPID_JSON
    // Run a simple JSON test
    TestRemoteUdpJson();
#endif

	// Run all unit tests (uncomment to run unit tests)
	//DelegateUnitTests();

    Sleep(1000);
	return 0;
}

