#include "Fault.h"
#include "TestRemoteUdp.h"
#include "TestSysData.h"
#include "UdpDelegateRecv.h"
#include "UdpDelegateSend.h"
#include <iostream>

// main.cpp
// @see https://www.codeproject.com/Articles/5262271/Remote-Procedure-Calls-using-Cplusplus-Delegates
// David Lafreniere, Mar 2020.
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

    // Run a test using SysData
    TestSysData();

	// Run all unit tests (uncomment to run unit tests)
	//DelegateUnitTests();

	return 0;
}

