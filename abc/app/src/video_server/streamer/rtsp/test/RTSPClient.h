#ifndef __RTSPCLIENT_H__
#define __RTSPCLIENT_H__

#include <string>
#include "nomad.h"

class RTSPSocket;

void *gRTSPClientThreadProc( void * ptr );

class RTSPClient
{	
public :
	bool isStop;
	RTSPSocket *client;
	
	RTSPClient(std::string serverIp, int serverPort, int rtpPort, std::string filename);
	virtual ~RTSPClient();

	void start();
	void stop();

	int sendMessage(int method);
	int sendMessageMulti();
	int sendMessagePlay();
	bool parseResponseCode(const char* pLine, int& nResponseCode);
	bool recvContent(int nContentLength);
	
	int rtspTimeout;

private :
	pthread_t clientThread;

	std::string serverIp;
	int serverPort;
	int rtpPort;
	std::string playFilename;
	std::string userId;
	std::string sessionId;

	int seqNumber;
	int rtspMethod;
};

#endif
