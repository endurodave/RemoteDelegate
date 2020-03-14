#include "stdafx.h"
#include "PipeDelegateRecv.h"
#include "DelegateLib.h"
#include "UserMsgs.h"
#include <sstream>

using namespace DelegateLib;
using namespace std;

#define BUF_SIZE 1024

static LPCTSTR lpszPipename = TEXT("\\\\.\\pipe\\pipedelegate");

PipeDelegateRecv& PipeDelegateRecv::GetInstance()
{
    static PipeDelegateRecv instance("PipeDelegateRecvThread");
    return instance;
}

PipeDelegateRecv::~PipeDelegateRecv()
{
    ExitThread();
}

void PipeDelegateRecv::Initialize()
{
    // Create named pipe
    m_hPipe = CreateNamedPipe(
        lpszPipename,             // pipe name 
        PIPE_ACCESS_DUPLEX,       // read/write access 
        PIPE_TYPE_MESSAGE |       // message type pipe 
        PIPE_READMODE_MESSAGE |   // message-read mode 
        PIPE_NOWAIT,              // non-blocking mode 
        PIPE_UNLIMITED_INSTANCES, // max. instances  
        BUF_SIZE,                 // output buffer size 
        BUF_SIZE,                 // input buffer size 
        0,                        // client time-out 
        NULL);                    // default security attribute 

    ASSERT_TRUE(m_hPipe != INVALID_HANDLE_VALUE);

    CreateThread();
}

unsigned long PipeDelegateRecv::Process(void* parameter)
{  
    MSG msg;
    BOOL connected = FALSE;
    char recvBuf[BUF_SIZE];

    for (;;)
    {
        // Check for thread exit message
        if (PeekMessage(&msg, NULL, WM_USER_BEGIN, WM_USER_END, PM_REMOVE) != 0)
        {
            switch (msg.message)
            {
            case WM_EXIT_THREAD:
                CloseHandle(m_hPipe);
                return 0;
            }
        }

        if (!connected)
        {
            // Check if client connected
            connected = ConnectNamedPipe(m_hPipe, NULL) ?
                TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
        }
        else
        {
            DWORD recvMsgSize = 0;
            BOOL success = ReadFile(m_hPipe, recvBuf, BUF_SIZE, &recvMsgSize, NULL);

            if (success && recvMsgSize > 0)
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
    }

    return 0;
}
