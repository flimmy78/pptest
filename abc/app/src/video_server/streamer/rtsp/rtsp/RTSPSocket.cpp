#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/epoll.h>

#include <nomad_log.h>
#include "RTSPSocket.h"
#include "Timer.h"

static const char* TAG = "RTSPSOCKET";

/** @brief RTSPSocket 생성자. socket을 간단하게 사용하기 위한 wrapper
	@return 의미 없음.
	@param 없음.
*/
RTSPSocket::RTSPSocket() :
bufferSize(BUFSIZE),
dataSize(0)
{
	buffer = new char [bufferSize];	
	socketFD = -1;
	
	send_sec = 3;
	send_usec = 0;
	recv_sec = 10;
	recv_usec = 0;

	memset(&localAddress, 0, sizeof(localAddress));
	epollFD = epoll_create(EPOLLSIZE);
	ep_events = (struct epoll_event*)malloc(sizeof(struct epoll_event)*EPOLLSIZE);
}

/** @brief RTSPSocket 생성자. socket을 간단하게 사용하기 위한 wrapper
	@return 의미 없음.
	@param fd file descriptor
	@param addr local sockaddr_in
*/
RTSPSocket::RTSPSocket(int fd, sockaddr_in addr) :
bufferSize(BUFSIZE),
dataSize(0)
{
	buffer = new char [bufferSize];
	socketFD = fd;
	
	send_sec = 3;
	send_usec = 0;
	recv_sec = 10;
	recv_usec = 0;

	localAddress = addr;
	epollFD = epoll_create(EPOLLSIZE);	
	ep_events = (struct epoll_event*)malloc(sizeof(struct epoll_event)*EPOLLSIZE);
}

/** @brief RTSPSocket 소멸자. socket을 간단하게 사용하기 위한 wrapper
	@return 의미 없음.
	@param 없음.
*/
RTSPSocket::~RTSPSocket()
{
	close();
	delete[] buffer;
}

/** @brief tcp socket을 생성한다.
	@return true or false
	@param 없음.
*/
bool RTSPSocket::create()
{
	socketFD = socket(AF_INET, SOCK_STREAM, 0);

	if(!isValid())
		return false;

	int on = 1;
	if(setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on)) == -1)
	{
		return false;
	}
	else
	{
		return true;
	}
}

/** @brief socket을 종료한다.
	@return true or false
	@param 없음.
*/
void RTSPSocket::close()
{
	if(isValid())
	{
		::shutdown(socketFD, 1);
		::close(socketFD);
		socketFD = -1;
	}
	::shutdown(epollFD, 1);
	::close(epollFD);
	epollFD = -1;
	
	//free(ep_events);
}

/** @brief socket의 nonblocking 모드를 설정한다.
	@return 없음.
	@param b true : nonblocking false : blocking
*/
void RTSPSocket::setNonBlocking(bool b)
{
	int opts;

	opts = fcntl(socketFD, F_GETFL);

	if(opts < 0)
	{
		LOGE(TAG, "%s(%d) %s: F_GETFL error.", __CALL_INFO__);
		return;
	}

	if(b) opts = (opts | O_NONBLOCK);
	else opts = (opts & ~O_NONBLOCK);

	fcntl(socketFD, F_SETFL,opts);
}

/** @brief socket port와 bind한다.
	@return true or false
	@param port bind 할 port
*/
bool RTSPSocket::bind(int port)
{
	if(!isValid()) return false;

	localAddress.sin_family = AF_INET;
	localAddress.sin_addr.s_addr = INADDR_ANY;
	localAddress.sin_port = htons ( port );

	int bind_return = ::bind(socketFD, (struct sockaddr*)&localAddress, sizeof(localAddress));

	if(bind_return == -1)
	{
		return false;
	}
	
	return true;
}

/** @brief server socket을 listen 한다.
	@return true or false
	@param 없음.
*/
bool RTSPSocket::listen() 
{
	if(!isValid())
		return false;

	int listen_return = ::listen(socketFD, MAXCONNECTIONS);

	if(listen_return == -1)
	{
		return false;
	}
	event.data.fd = socketFD;
	//event.events = EPOLLIN;
	event.events = EPOLLIN | EPOLLOUT | EPOLLERR;
	epoll_ctl(epollFD, EPOLL_CTL_ADD, socketFD, &event);

	return true;
}

/** @brief server socket에 connect 한다.(client에서 사용)
	@return true or false
	@param host server ip
	@param port server port
*/
bool RTSPSocket::connect(std::string host, int port)
{
	if(!isValid()) return false;

	localAddress.sin_family = AF_INET;
	localAddress.sin_port = htons(port);

	int status = inet_pton(AF_INET, host.c_str(), &localAddress.sin_addr);

	if(status < 0) return false;
	if(errno == EAFNOSUPPORT) return false;

	return connectWait(socketFD, (sockaddr*)&localAddress, sizeof(localAddress), 3);
}

/** @brief server socket에 connect 한다.(time out 적용)
	@return true or false
	@param sockfd file descriptor
	@param sockaddr sockaddr_in
	@param addrsize sockaddr_in size
	@param sec connect time 시간 초
*/
bool RTSPSocket::connectWait(int sockfd, struct sockaddr *saddr, int addrsize, int sec)
{
	int res;

	int error = 0;
	int esize;

	setNonBlocking(true);

	if((res = ::connect(sockfd, saddr, addrsize)) < 0)
	{
		if(errno != EINPROGRESS && errno != EWOULDBLOCK)
		{
			LOGE(TAG, "%s(%d) %s: ::connect error.", __CALL_INFO__);
			return false;
		}
	}

	if(res == 0)
	{
		setNonBlocking(false);
		return true;
	}
	
	int event_count;
	event.data.fd = sockfd;
	event.events = EPOLLIN | EPOLLOUT | EPOLLERR;
	epoll_ctl(epollFD, EPOLL_CTL_ADD, sockfd, &event);

	event_count = epoll_wait(epollFD, ep_events, EPOLLSIZE, sec*1000);
	
	if(event_count > 0)
	{
		esize = sizeof(int);
		if(getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&esize) < 0)
		{
			epoll_ctl(epollFD, EPOLL_CTL_DEL, sockfd, &event);
			return false;
		}
	
		setNonBlocking(false);
		if(error)
		{
			epoll_ctl(epollFD, EPOLL_CTL_DEL, sockfd, &event);
			return false;
		}
		epoll_ctl(epollFD, EPOLL_CTL_DEL, sockfd, &event);
		return true;
	}
	
	epoll_ctl(epollFD, EPOLL_CTL_DEL, sockfd, &event);
	return false;
}

/** @brief server socket에서 client socket을 받아 들인다.
	@return RTSPSocket
	@param 없음.
*/
RTSPSocket *RTSPSocket::accept()
{
	int addr_length = sizeof(clientAddress);
	clientSocket = ::accept(socketFD, (sockaddr*)&clientAddress, (socklen_t*)&addr_length);

	if(clientSocket == -1)
		return (RTSPSocket*)NULL;

	RTSPSocket *rtnClientSocket = new RTSPSocket(clientSocket, clientAddress);
	return rtnClientSocket;
}

/** @brief server socket에서 client socket을 받아 들인다. timeout 적용
	@return RTSPSocket
	@param sec socket 을 받아들일 timeout 초
*/
RTSPSocket* RTSPSocket::acceptWait(int sec)
{
	RTSPSocket* retSoc = (RTSPSocket*)NULL;

	int event_count = epoll_wait(epollFD, ep_events, EPOLLSIZE, sec*1000);

	if(event_count < 1)
	{
		return retSoc;
	}

	int addr_length = sizeof(clientAddress);
	clientSocket = ::accept(socketFD, (sockaddr*)&clientAddress, (socklen_t*)&addr_length);

	if(clientSocket < 0)
	{
		return retSoc;
	}
	else
	{
		retSoc = new RTSPSocket(clientSocket, clientAddress);
	}
	return retSoc;
}

/** @brief 연결된 socket에 data를 보낸다.
	@return true or false
	@param data socket을 통해 보낼 data
	@param len socket을 통해 보낼 data 크기
*/
bool RTSPSocket::send(const void* data, size_t len)
{
	event.data.fd = socketFD;
	event.events = EPOLLOUT | EPOLLERR;
	//event.events = EPOLLIN | EPOLLOUT | EPOLLERR;
	epoll_ctl(epollFD, EPOLL_CTL_ADD, socketFD, &event);

	int event_count = epoll_wait(epollFD, ep_events, EPOLLSIZE, send_sec*1000);
	
	if(event_count > 0)
	{
		int status = ::send(socketFD, data, len, MSG_NOSIGNAL);

		if(status == -1)
		{
			LOGE(TAG, "%s(%d) %s: send error. status(-1)", __CALL_INFO__);
			epoll_ctl(epollFD, EPOLL_CTL_DEL, socketFD, &event);
			return false;
		}
		else
		{
			epoll_ctl(epollFD, EPOLL_CTL_DEL, socketFD, &event);
			return true;
		}
	}

	epoll_ctl(epollFD, EPOLL_CTL_DEL, socketFD, &event);
	return false;
}

/** @brief 연결된 socket에 data를 읽는다.
	@return 읽은 크기
	@param data 읽은 데이터를 저장할 버퍼
*/
int RTSPSocket::recv(char *data) 
{
	int i = 0;
    int nBytesRead;
    char* szHeaderEnd;

    int nSum = dataSize;
    int nBytesToRead = bufferSize;
	int nReadSize = 0;

    while(1)
    {
		if(nSum - i < 4)
		{

			nBytesRead = recv(&buffer[nSum], nBytesToRead - nSum);
			if(nBytesRead < 1)
			{
				return nBytesRead;
			}
		}
		else
		{
			// Some data still remains.
			nBytesRead = 0;
		}

        if(nBytesRead > 0)
        {
            nSum += nBytesRead;
		}
		
		if(removeInterleavedControl(nSum))
		{
			nSum = dataSize;
			continue;
		}

		szHeaderEnd = strstr(&buffer[i], "\r\n\r\n");

		// If this packet is not HTTPProtocol,
		// this code may fall into infinite loop.
		if(szHeaderEnd)
		{
			szHeaderEnd[0] = '\0';
			szHeaderEnd[1] = '\0';

			//nReadSize = (int)(szHeaderEnd - buffer) + 4;
			nReadSize = strlen(buffer) + 4;
			break;
		}
		else
		{
			i = max(0, nSum - 3);
		}
    }

	// Received data size until now.
	// Some data will remains in the socket.    
	LOGD(TAG, "%s(%d) %s: nSum(%d), nReadSize(%d)", __CALL_INFO__, nSum, nReadSize);
	if(nReadSize > nSum) return -1;
	memcpy(data, buffer, nReadSize);
	memmove(buffer, buffer + nReadSize, nSum - nReadSize);
	dataSize = nSum - nReadSize;
	return nReadSize;
}

/** @brief 연결된 socket에 data를 읽는다.
	@return 읽은 크기
	@param data socket을 통해 받을 버퍼
	@param len socket을 통해 받을 버퍼 크기
*/
int RTSPSocket::recv(void *data, size_t len)
{
	event.data.fd = socketFD;
	event.events = EPOLLIN | EPOLLERR;
	//event.events = EPOLLIN | EPOLLOUT | EPOLLERR;
	epoll_ctl(epollFD, EPOLL_CTL_ADD, socketFD, &event);

	int event_count = epoll_wait(epollFD, ep_events, EPOLLSIZE, recv_sec*1000);

	if(event_count > 0)
	{
		int status = ::recv(socketFD, data, len, 0);
		//LOGD(TAG, "%s(%d) %s: status(%d)", __CALL_INFO__, status);
		if(status > 0)
		{
			epoll_ctl(epollFD, EPOLL_CTL_DEL, socketFD, &event);
			return status;
		}
		epoll_ctl(epollFD, EPOLL_CTL_DEL, socketFD, &event);
		return -1;
	}
	else if(event_count = 0)
	{
		epoll_ctl(epollFD, EPOLL_CTL_DEL, socketFD, &event);
		return 0;
	}
	else
	{		
		epoll_ctl(epollFD, EPOLL_CTL_DEL, socketFD, &event);
		return -1;
	}
}

/** @brief 연결된 socket에 특정 크기 만큰 data를 읽는다.
	@return 읽은 크기
	@param data socket을 통해 받을 버퍼
	@param len socket을 통해 읽을 크기
*/
int RTSPSocket::syncRecv(void *data, size_t len)
{
	//int i = 0;
    int nBytesRead;
    //char* szHeaderEnd;

    size_t nSum = dataSize;
    int nBytesToRead = bufferSize;
	//int nReadSize = 0;

    while(1)
    {
		if(nSum < len)
		{
			nBytesRead = recv(&buffer[nSum], nBytesToRead - nSum);

			if(nBytesRead < 1)
			{
				return nBytesRead;
			}
		}
		else
		{
			break;
		}

        if(nBytesRead > 0)
        {
            nSum += nBytesRead;
		}
    }

	// Received data size until now.
	// Some data will remains in the socket.    
	memcpy(data, buffer, len);
	memmove(buffer, buffer + len, nSum - len);
	dataSize = nSum - len;
	return len;
}

/** @brief rtp over tcp의 경우 control channel을 통해 온 데이터를 읽어서 제거 한다.
	@return true or false
	@param totalLength socket을 실제 읽은 data 크기
*/
bool RTSPSocket::removeInterleavedControl(int &totalLength)
{
	const int INTERLEAVED_HEADER_SIZE = 4;
	size_t syncSize = 0;
	InterleavedHeader* header;
	
	//if(totalLength > INTERLEAVED_HEADER_SIZE-1 && buffer[0] == 0x24 && buffer[1] == 0x01)
	if(totalLength > INTERLEAVED_HEADER_SIZE-1 && buffer[0] == 0x24)
	{
		header = (InterleavedHeader*)buffer;
		syncSize = ntohs(header->length);

		memmove(buffer, buffer + INTERLEAVED_HEADER_SIZE, totalLength - INTERLEAVED_HEADER_SIZE);
		totalLength -= INTERLEAVED_HEADER_SIZE;
		dataSize = totalLength;
		if(syncSize > 0)
		{
			return recvInterleavedData(syncSize);
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}

/** @brief rtp over tcp의 경우 control channel을 통해 온 데이터를 실제 읽는 부분
	@return true or false
	@param dataLength 실제 읽어야 하는 data 크기
*/
bool RTSPSocket::recvInterleavedData(const int dataLength)
{
	int status;
	char buf[dataLength];

	status = syncRecv(buf, dataLength);

	if(status == dataLength)
	{
		return true;
	}
	return false;
}

/** @brief socket valid check
	@return true or false
	@param 없음.
*/
bool RTSPSocket::isValid()  
{
	return socketFD != -1;
}

/** @brief 현재 연결되어 있는 socket의 remote ip 를 알아온다.
	@return remote ip string
	@param 없음.
*/
string RTSPSocket::remoteIP()
{
	char *c_ip;
	c_ip = (char*)malloc(sizeof(char)*24);
	memset(c_ip, 0, sizeof(char)*24);
	sprintf(c_ip, "%s", inet_ntoa(((in_addr)(localAddress.sin_addr))));
	string remoteIP(c_ip);

	if(c_ip) free(c_ip);

	return remoteIP;
}

/** @brief 현재 연결되어 있는 socket의 remote port 를 알아온다.
	@return remote port
	@param 없음.
*/
int RTSPSocket::remotePort() 
{ 
	return static_cast<int>(ntohs(localAddress.sin_port)); 
}

/** @brief socekt send time out을 설정한다.
	@return 없음.
	@param sec 설정할 초
	@param usec 설정할 마이크로 초
*/
void RTSPSocket::setSendTimeout(int sec, int usec)
{
	send_sec = sec;
	send_usec = usec;
}

/** @brief socekt recv time out을 설정한다.
	@return 없음.
	@param sec 설정할 초
	@param usec 설정할 마이크로 초
*/
void RTSPSocket::setRecvTimeout(int sec, int usec)
{
	recv_sec = sec;
	recv_usec = usec;
}
