#ifndef _UDP_DELEGATE_SEND_H
#define _UDP_DELEGATE_SEND_H

#include "DelegateTransport.h"
#include <ostream>

/// @brief Sends a delegate to a remote system using a UDP socket.  
class UdpDelegateSend : public DelegateLib::DelegateTransport
{
public:
    static UdpDelegateSend & GetInstance();

    /// Call one time during startup
    void Initialize();

    /// Called by the delegate library to invoke a remote delegate
    /// @param[in] s - the stream of bytes to send
    virtual void DispatchDelegate(std::iostream& s);

private:
    UdpDelegateSend();
    virtual ~UdpDelegateSend();

    CAsyncSocket m_sendSocket;
};

#endif