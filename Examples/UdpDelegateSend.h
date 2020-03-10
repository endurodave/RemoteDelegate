#ifndef _UDP_DELEGATE_SEND_H
#define _UDP_DELEGATE_SEND_H

#include "DelegateTransport.h"
#include <ostream>

/// @brief Sends a delegate to a remote system using a UDP socket.  
class UpdDelegateSend : public DelegateLib::DelegateTransport
{
public:
    static UpdDelegateSend & GetInstance();

    /// Call one time during startup
    void Initialize();

    /// Called by the delegate library to invoke a remote delegate
    /// @param[in] s - the stream of bytes to send
    virtual void DispatchDelegate(std::ostream& s);

private:
    UpdDelegateSend();
    virtual ~UpdDelegateSend();

    CAsyncSocket m_sendSocket;
};

#endif