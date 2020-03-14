#include "stdafx.h"
#include "PipeDelegateSend.h"
#include "DelegateLib.h"
#include "WorkerThreadWin.h"

using namespace DelegateLib;
using namespace std;

static LPCTSTR lpszPipename = TEXT("\\\\.\\pipe\\pipedelegate");

PipeDelegateSend & PipeDelegateSend::GetInstance()
{
    static PipeDelegateSend instance;
    return instance;
}

PipeDelegateSend::PipeDelegateSend() : m_hPipe(INVALID_HANDLE_VALUE)
{
}

PipeDelegateSend::~PipeDelegateSend()
{
}

void PipeDelegateSend::Initialize()
{
    // Create named pipe
    m_hPipe = CreateFile(
        lpszPipename,   // pipe name 
        GENERIC_READ |  // read and write access 
        GENERIC_WRITE,
        0,              // no sharing 
        NULL,           // default security attributes
        OPEN_EXISTING,  // opens existing pipe 
        0,              // default attributes 
        NULL);          // no template file 

    ASSERT_TRUE(m_hPipe != INVALID_HANDLE_VALUE);
}

void PipeDelegateSend::DispatchDelegate(std::iostream& s)
{
    size_t len = (size_t)s.tellp();
    char* sendBuf = (char*)malloc(len);

    // Copy char buffer into heap allocated memory
    s.rdbuf()->sgetn(sendBuf, len);

    // Send message through named pipe
    DWORD sentLen = 0;
    BOOL success = WriteFile(m_hPipe, sendBuf, len, &sentLen, NULL);
    ASSERT_TRUE(success && sentLen == len);

    free(sendBuf);

    // Reset stream positions
    s.seekg(0);
    s.seekp(0);
}

