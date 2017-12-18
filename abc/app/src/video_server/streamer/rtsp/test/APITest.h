#ifndef __APITEST_H__
#define __APITEST_H__

#include <string>
#include "nomad.h"

class RTSPSocket;

class APITest
{	
public :
	RTSPSocket *client;
	
	APITest(std::string serverIp, int serverPort);
	virtual ~APITest();

	void start();
	void stop();

	int sendMessage(std::string msg);
	bool parseResponseCode(const char* pLine, int& nResponseCode);
	bool recvContent(int nContentLength);
	
	int rtspTimeout;

private :
	std::string serverIp;
	int serverPort;

	std::string userId;
	std::string sessionId;

	int seqNumber;
	int rtspMethod;
};

#endif
