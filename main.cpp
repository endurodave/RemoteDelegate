#include "stdafx.h"
#include "Fault.h"
#include "TestRemoteUdp.h"
#include "TestRemotePipe.h"
#include "TestSysData.h"
#include "TestRemoteUdpAsync.h"
#ifdef RAPID_JSON
#include "TestRemoteUdpJson.h"
#endif
#include "UdpDelegateRecv.h"
#include "UdpDelegateSend.h"
#include "PipeDelegateRecv.h"
#include "PipeDelegateSend.h"
#include <iostream>

// main.cpp
// @see https://www.codeproject.com/Articles/5262271/Remote-Procedure-Calls-using-Cplusplus-Delegates
// David Lafreniere, Mar 2020.
//
// @see https://www.codeproject.com/Articles/1160934/Asynchronous-Multicast-Delegates-in-Cplusplus
// David Lafreniere, Dec 2016.
//
// Executes Windows examples. 

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

    UdpDelegateRecv::GetInstance().Initialize();
    UdpDelegateSend::GetInstance().Initialize();
    PipeDelegateRecv::GetInstance().Initialize();
    PipeDelegateSend::GetInstance().Initialize();

    // Run a simple test
    TestRemoteUdp();

    // Run a named pipe test
    TestRemotePipe();

    // See RapidJSON_Readme.txt
#ifdef RAPID_JSON
    // Run a JSON test
    TestRemoteUdpJson();
#endif

    // Run an async test
    TestRemoteUdpAsync();

    // Run a test using SysData
    TestSysData();

	// Run all unit tests (uncomment to run unit tests)
	//DelegateUnitTests();

	return 0;
}

