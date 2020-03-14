#ifndef _PIPE_DELEGATE_SEND_H
#define _PIPE_DELEGATE_SEND_H

#include "DelegateTransport.h"
#include <ostream>

/// @brief Sends a delegate to a remote system using a named pipe.  
class PipeDelegateSend : public DelegateLib::DelegateTransport
{
public:
    static PipeDelegateSend & GetInstance();

    /// Call one time during startup
    void Initialize();

    /// Called by the delegate library to invoke a remote delegate
    /// @param[in] s - the stream of bytes to send
    virtual void DispatchDelegate(std::iostream& s);

private:
    PipeDelegateSend();
    virtual ~PipeDelegateSend();

    HANDLE m_hPipe;
};

#endif