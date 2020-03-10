#include "stdafx.h"
#include "UdpDelegateRecv.h"
#include "DelegateLib.h"
#include "UserMsgs.h"
#include <sstream>

using namespace DelegateLib;
using namespace std;

UdpDelegateRecv& UdpDelegateRecv::GetInstance()
{
    static UdpDelegateRecv instance("UdpDelegateRecvThread");
    return instance;
}

UdpDelegateRecv::~UdpDelegateRecv()
{
    ExitThread();
}

void UdpDelegateRecv::Initialize()
{
    CreateThread();
}

unsigned long UdpDelegateRecv::Process(void* parameter)
{  
    MSG msg;

    const int BUF_SIZE = 1024;
    char recvBuf[BUF_SIZE];

    SOCKADDR_IN addr;
    int addrLen = sizeof(addr);

    BOOL success = AfxSocketInit(NULL);
    ASSERT_TRUE(success == TRUE);

    success = m_recvSocket.Create(514, SOCK_DGRAM, NULL);
    ASSERT_TRUE(success);

    for (;;)
    {
        // Check for thread exit message
        if (PeekMessage(&msg, NULL, WM_USER_BEGIN, WM_USER_END, PM_REMOVE) != 0)
        {
            switch (msg.message)
            {
            case WM_EXIT_THREAD:
                m_recvSocket.Close();
                return 0;
            }
        }

        // Check for socket receive message
        int recvMsgSize = m_recvSocket.Receive(recvBuf, BUF_SIZE, 0);
        if (recvMsgSize > 0)
        {
            // Copy receive data bytes into a stringstream
            stringstream ss(ios::in | ios::out | ios::binary);
            ss.write(recvBuf, recvMsgSize);

           // Invoke the remote delegate callback function
           DelegateRemoteInvoker::Invoke(ss);
        }
        else
        {
            Sleep(100);
        }
    }

    return 0;
}
