#ifndef _UDP_DELEGATE_RECV_H
#define _UDP_DELEGATE_RECV_H

#include "WorkerThreadStd.h"
#include <atomic>

/// @brief A worker thread used to receive incoming UDP messages.
class UdpDelegateRecv //: public ThreadWin
{
public:
    static UdpDelegateRecv& GetInstance();

    /// Call one time during startup
    void Initialize();

private:
    virtual ~UdpDelegateRecv();

    /// The worker thread entry function
    static void* Process(void* parameter);
};

#endif
