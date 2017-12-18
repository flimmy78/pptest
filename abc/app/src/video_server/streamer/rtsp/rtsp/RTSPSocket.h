#ifndef __RTSPSOCKET_H__
#define __RTSPSOCKET_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>
#include <sys/epoll.h>

#define MAXCONNECTIONS			100
#define BUFSIZE					32*1024
#define EPOLLSIZE				10

using namespace std;

typedef struct _InterleavedHeader
{
	unsigned char magic;
	unsigned char channel;
	unsigned short length;
}InterleavedHeader;

class RTSPSocket
{
protected:
	const int bufferSize;
	int dataSize;
	//int m_nHeaderLength;
	char* buffer;

public :
	// Local CRTSPSocket(Server or Client) filedescriptor
 	int socketFD;

	RTSPSocket();
	RTSPSocket(int sock, sockaddr_in addr);
	virtual ~RTSPSocket();

	///
	/// ServerSocket initialization
	///
	bool create();
	bool bind(int port);
	bool listen() ;
	RTSPSocket* accept();
	RTSPSocket* acceptWait(int sec);
	
	// Connection
	bool connect(string host, int port);
    bool connectWait(int sockfd, struct sockaddr *saddr, int addrsize, int sec);
	void close();

	// option
	void setNonBlocking(bool b);

	// Status, Environment Value
	bool isValid();
	string remoteIP();
	int remotePort();

	void setSendTimeout(int sec, int usec);
	void setRecvTimeout(int sec, int usec);
	
	bool send(const void* data, size_t len);
	int recv(char *data);
	int recv(void *data, size_t len);
	int syncRecv(void *data, size_t len);

private :
	int send_sec;
	int send_usec;
	int recv_sec;
	int recv_usec;
 	
 	// Remote Socket(Server or Client) filedescriptor
 	int clientSocket;
 	 	
	// Local Socket(Server or Client) sockaddr_in
 	sockaddr_in localAddress;
 	
 	// Remote Socket(Server or Client) sockaddr_in
 	sockaddr_in clientAddress;

	// for epoll
	int epollFD;
	struct epoll_event *ep_events;
	struct epoll_event event;

	bool removeInterleavedControl(int &totalLength);
	bool recvInterleavedData(const int dataLength);
};

#endif


