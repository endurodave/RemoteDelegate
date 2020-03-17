#include "UdpDelegateSend.h"
#include "DelegateLib.h"
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

using namespace DelegateLib;
using namespace std;

#define PORT    514

static int sockfd;
static sockaddr_in servaddr;

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
    close(sockfd);
}

void UdpDelegateSend::Initialize()
{
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    ASSERT_TRUE(sockfd >= 0);

    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;
}

void UdpDelegateSend::DispatchDelegate(std::iostream& s)
{
    size_t len = (size_t)s.tellp();
    char* sendBuf = (char*)malloc(len);

    // Copy char buffer into heap allocated memory
    s.rdbuf()->sgetn(sendBuf, len);

    // Send data to remote system using a socket
    int result = sendto(sockfd, (const char *)sendBuf, len,
        0, (const struct sockaddr *) &servaddr, sizeof(servaddr));
    ASSERT_TRUE((size_t)result == len);

    free(sendBuf);

    // Reset stream positions
    s.seekg(0);
    s.seekp(0);
}

