#include "stdafx.h"
#include "UdpDelegateSend.h"
#include "DelegateLib.h"
#include "WorkerThreadWin.h"

using namespace DelegateLib;
using namespace std;

UdpDelegateSend & UdpDelegateSend::GetInstance()
{
    static UdpDelegateSend instance;
    return instance;
}

UdpDelegateSend::UdpDelegateSend()
{
}

UdpDelegateSend::~UdpDelegateSend()
{
    m_sendSocket.Close();
}

void UdpDelegateSend::Initialize()
{
    BOOL success = m_sendSocket.Create(0, SOCK_DGRAM, NULL);
    ASSERT_TRUE(success);

    success = m_sendSocket.Connect(L"localhost", 514);
    ASSERT_TRUE(success);
}

void UdpDelegateSend::DispatchDelegate(std::iostream& s)
{
    size_t len = (size_t)s.tellp();
    char* sendBuf = (char*)malloc(len);

    // Copy char buffer into heap allocated memory
    s.rdbuf()->sgetn(sendBuf, len);

    // Send data to remote system using a socket
    int result = m_sendSocket.Send((void*)sendBuf, len, 0);
    ASSERT_TRUE(result == len);

    free(sendBuf);

    // Reset stream positions
    s.seekg(0);
    s.seekp(0);
}

