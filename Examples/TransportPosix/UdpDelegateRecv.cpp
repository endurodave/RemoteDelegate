#include "UdpDelegateRecv.h"
#include "DelegateLib.h"
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sstream>

using namespace DelegateLib;
using namespace std;

#define PORT    514

static int sockfd;
static sockaddr_in servaddr, cliaddr;
static std::atomic<bool> m_exit;
static pthread_t workerThread;

UdpDelegateRecv& UdpDelegateRecv::GetInstance()
{
    static UdpDelegateRecv instance;
    return instance;
}

UdpDelegateRecv::~UdpDelegateRecv()
{
	m_exit = true;
}

void UdpDelegateRecv::Initialize()
{
	pthread_create(&workerThread, NULL, &UdpDelegateRecv::Process, NULL);
}

void* UdpDelegateRecv::Process(void* parameter)
{  
    const int BUF_SIZE = 1024;
    char recvBuf[BUF_SIZE];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    ASSERT_TRUE(sockfd >= 0);

    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    int rval = bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));
	ASSERT_TRUE(rval >= 0);

    for (;;)
    {
        // Check for thread exit
    	if (m_exit)
    		break;

        // Check for socket receive message
    	int len;
		len = sizeof(cliaddr);
		int recvMsgSize = recvfrom(sockfd, (char *)recvBuf, BUF_SIZE,
				MSG_DONTWAIT, ( struct sockaddr *) &cliaddr, &len);
		if (recvMsgSize > 0)
        {
            // Copy receive data bytes into a stringstream
            stringstream ss(ios::in | ios::out | ios::binary);
            ss.write(recvBuf, recvMsgSize);

           // Invoke the remote delegate callback function
           DelegateRemoteInvoker::Invoke(ss);
        }
        else
        {
            timespec ts;
            ts.tv_sec = 0;
            ts.tv_nsec = 10000;
            nanosleep(&ts, &ts);
        }
    }

    return 0;
}
