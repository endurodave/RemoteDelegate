#ifndef _UDP_DELEGATE_RECV_H
#define _UDP_DELEGATE_RECV_H

#include "WorkerThreadWin.h"

/// @brief A worker thread used to receive incoming UDP messages.
class UdpDelegateRecv : public ThreadWin
{
public:
    static UdpDelegateRecv& GetInstance();

    /// Call one time during startup
    void Initialize();

private:
    UdpDelegateRecv(const CHAR* threadName) : ThreadWin(threadName, FALSE) {}
    virtual ~UdpDelegateRecv();

    /// The worker thread entry function
    virtual unsigned long Process(void* parameter);

    CAsyncSocket m_recvSocket;
};

#endif