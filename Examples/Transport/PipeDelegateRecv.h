#ifndef _PIPE_DELEGATE_RECV_H
#define _PIPE_DELEGATE_RECV_H

#include "WorkerThreadWin.h"
#include <atomic>

/// @brief A worker thread used to receive incoming UDP messages.
class PipeDelegateRecv : public ThreadWin
{
public:
    static PipeDelegateRecv& GetInstance();

    /// Call one time during startup
    void Initialize();

private:
    PipeDelegateRecv(const CHAR* threadName) : 
        ThreadWin(threadName, FALSE) {}
    virtual ~PipeDelegateRecv();

    /// The worker thread entry function
    virtual unsigned long Process(void* parameter);

    HANDLE m_hPipe;
};

#endif