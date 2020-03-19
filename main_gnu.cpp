#include "Fault.h"
#include "TestRemoteUdp.h"
#include "TestRemoteUdpAsync.h"
#include "TestSysData.h"
#ifdef RAPID_JSON
#include "TestRemoteUdpJson.h"
#endif
#include "UdpDelegateRecv.h"
#include "UdpDelegateSend.h"
#include <iostream>

// main.cpp
// @see https://www.codeproject.com/Articles/5262271/Remote-Procedure-Calls-using-Cplusplus-Delegates
// David Lafreniere, Mar 2020.
//
// @see https://www.codeproject.com/Articles/1160934/Asynchronous-Multicast-Delegates-in-Cplusplus
// David Lafreniere, Dec 2016.
//
// Executes POSIX examples.

using namespace std;

extern void DelegateUnitTests();

//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------
int main(void)
{
    UdpDelegateRecv::GetInstance().Initialize();
    UdpDelegateSend::GetInstance().Initialize();

    // Run a simple test
    TestRemoteUdp();

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

