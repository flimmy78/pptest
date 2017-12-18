#include <string.h>
#include <vector>
#include <map>
#include <algorithm>
#include <unistd.h>

#include <nomad_log.h>
#include "RTSPClient.h"
#include "StringUtil.h"
#include "rtsp.h"
#include "config.h"

#include "RTSPRequest.h"
#include "RTSPResponse.h"
#include "Header.h"
#include "TransportHeader.h"
#include "RangeHeader.h"
#include "SDPMaker.h"
#include "RTSPSession.h"
#include "SessionManager.h"
#include "RTSPSocket.h"

static const char* TAG = "RTSP_CLIENT";

void *gRTSPClientThreadProc(void *ptr)
{
	RTSPClient *rtspclient = (RTSPClient*)ptr;
	rtspclient->start();

	int status = 0;
	char strIn[BUFSIZE];	
	while(!rtspclient->isStop) 
	{
//		memset(strIn, 0x00, BUFSIZE);
//		rtspclient->client->setRecvTimeout(DEFAULT_RTSP_TIMEOUT-10, 0);
//		status = rtspclient->client->recv((char*)strIn);
//		if (status < 0)
//		{
//			printf("%s : %s(%d) %s: Socket recv error. size(%d)\n", TAG, __CALL_INFO__, status);
//			break;
//		}
//
//		if(status == 0) // timeout
//		{
//			printf("%s : %s(%d) %s: Socket recv timeout.\n", TAG, __CALL_INFO__);
//			rtspclient->sendMessage(eRM_GET_PARAMETER);
//		}
//		printf("%s : %s(%d) %s: Socket recv msg(%s)\n", TAG, __CALL_INFO__, strIn);
		usleep(1000*1000);
		status += 1;
		if(status > rtspclient->rtspTimeout-10)
		{
			rtspclient->sendMessage(eRM_GET_PARAMETER);
			status = 0;
		}
	}
	rtspclient->sendMessage(eRM_TEARDOWN);

	return (void*)NULL;
}

RTSPClient::RTSPClient(std::string serverIp, int serverPort, int rtpPort, std::string filename)
{
	client = new RTSPSocket();
	isStop = false;

	this->serverIp = serverIp;
	this->serverPort = serverPort;
	this->rtpPort = rtpPort;
	this->playFilename = filename;
	this->userId = "";
	this->sessionId = "";

	seqNumber = 0;
	rtspMethod = -1;
	rtspTimeout = 60;

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    
	if(pthread_create( &clientThread, &attr, gRTSPClientThreadProc, (void*) this))
	{
		printf("%s : %s(%d) %s: pthread create fail.\n", TAG, __CALL_INFO__);
	}
	pthread_attr_destroy(&attr);
}


RTSPClient::~RTSPClient()
{
	isStop = true;
	pthread_join(clientThread, (void**)NULL);
	
	if(client)
	{
		delete client;
		client = NULL;
	}
}

void RTSPClient::start()
{
	isStop = false;

	if(!client->create())
	{
		isStop = true;
		return;
	}
	
	if(!client->connect(serverIp, serverPort))
	{
		isStop = true;
		return;
	}

	if(!client->isValid())
	{
		isStop = true;
		return;
	}

	if(200 != sendMessage(eRM_OPTIONS))
	{
		isStop = true;
		return;
	}

	if(200 != sendMessage(eRM_DESCRIBE))
	{
		isStop = true;
		return;
	}

	if(301 == sendMessage(eRM_SETUP))
	{
		if(200 != sendMessage(eRM_SETUP))
		{
			isStop = true;
			return;
		}
	}
	else if(200 != sendMessage(eRM_SETUP))
	{
		isStop = true;
		return;
	}

	if(200 != sendMessage(eRM_PLAY))
	{
		isStop = true;
		return;
	}
	
//	// multicast test
//	if(200 != sendMessageMulti())
//	{
//		isStop = true;
//		return;
//	}
//
//	if(200 != sendMessagePlay())
//	{
//		isStop = true;
//		return;
//	}
}

void RTSPClient::stop()
{
	isStop = true;
}

int RTSPClient::sendMessageMulti()
{
	seqNumber++;
	std::string requestStr = "";
	
	bool ret = false;
	char url[256];
	char transport[256];
	char range[256];
	char tmpBuffer[256];;
	char recvBuffer[BUFSIZE];
	int responseCode = -1;

	rtspMethod = eRM_SETUP;

	if(!client->isValid())
	{
		return -1;
	}
	
	sprintf(url, "%s:%d/%s", serverIp.c_str(), serverPort, playFilename.c_str());
	
	sprintf(tmpBuffer, "SETUP rtsp://%s RTSP/1.0\r\nCSeq: %d\r\n", url, seqNumber);
	requestStr.append(tmpBuffer);
	
	int _port = 7000;
	sprintf(transport, "Transport: RTP/AVP;unicast;destination=192.168.11.80;port=%d-%d;ttl=127\r\n", _port, _port+1);
	requestStr.append(transport);

	if(sessionId != "")
	{
		requestStr.append("Session: ");
		requestStr.append(sessionId.c_str());
		requestStr.append("\r\n");

	}
	requestStr.append("\r\n");

	ret = client->send((char*)requestStr.c_str(), requestStr.size());

	if(!ret) return ret;

	int status = client->recv((char*)recvBuffer);
	if (status < 0)
	{
		printf("%s : %s(%d) %s: Socket recv error. size(%d)\n", TAG, __CALL_INFO__, status);
		return -1;
	}

	if(status == 0)
	{
		printf("%s : %s(%d) %s: Socket recv timeout.\n", TAG, __CALL_INFO__);
		return -1;
	}

	parseResponseCode(recvBuffer, responseCode);
	printf("%s : %s(%d) %s: responseCode(%d)\n", TAG, __CALL_INFO__, responseCode);

	return responseCode;
}

int RTSPClient::sendMessagePlay()
{
	seqNumber++;
	std::string requestStr = "";
	
	bool ret = false;
	char url[256];
	char transport[256];
	char range[256];
	char tmpBuffer[256];;
	char recvBuffer[BUFSIZE];
	int responseCode = -1;

	rtspMethod = eRM_PLAY;

	if(!client->isValid())
	{
		return -1;
	}
	
	sprintf(url, "%s:%d/%s", serverIp.c_str(), serverPort, playFilename.c_str());
		
	sprintf(tmpBuffer, "%s rtsp://%s RTSP/1.0\r\nCSeq: %d\r\n", 
		rtspGetMethodName(eRM_PLAY), url, seqNumber);
	requestStr.append(tmpBuffer);

	sprintf(range, "Range: npt=0.000-)\r\n");
	requestStr.append(range);

	if(sessionId != "")
	{
		requestStr.append("Session: ");
		requestStr.append(sessionId.c_str());
		requestStr.append("\r\n");

	}
	requestStr.append("\r\n");

	ret = client->send((char*)requestStr.c_str(), requestStr.size());

	if(!ret) return ret;

	int status = client->recv((char*)recvBuffer);
	if (status < 0)
	{
		printf("%s : %s(%d) %s: Socket recv error. size(%d)\n", TAG, __CALL_INFO__, status);
		return -1;
	}

	if(status == 0)
	{
		printf("%s : %s(%d) %s: Socket recv timeout.\n", TAG, __CALL_INFO__);
		return -1;
	}

	parseResponseCode(recvBuffer, responseCode);
	printf("%s : %s(%d) %s: responseCode(%d)\n", TAG, __CALL_INFO__, responseCode);

	return responseCode;
}

int RTSPClient::sendMessage(int method)
{
	seqNumber++;
	std::string requestStr = "";
	
	bool ret = false;
	char url[256];
	char transport[256];
	char range[256];
	char tmpBuffer[256];;
	char recvBuffer[BUFSIZE];
	int responseCode = -1;

	rtspMethod = method;

	if(!client->isValid())
	{
		return -1;
	}
	
	if(method == eRM_OPTIONS || method == eRM_DESCRIBE)
	{
		sprintf(url, "%s:%d/%s", serverIp.c_str(), serverPort, playFilename.c_str());
	}
	else
	{
		if(userId != "")
		{
			//sprintf(url, "%s@%s:%d/%s/track1", userId.c_str(), serverIp.c_str(), serverPort, playFilename.c_str());
			sprintf(url, "%s@%s:%d/%s", userId.c_str(), serverIp.c_str(), serverPort, playFilename.c_str());
		}
		else
		{
			//sprintf(url, "%s:%d/%s/track1", serverIp.c_str(), serverPort, playFilename.c_str());
			sprintf(url, "%s:%d/%s", serverIp.c_str(), serverPort, playFilename.c_str());
		}
	}
	
	sprintf(tmpBuffer, "%s rtsp://%s RTSP/1.0\r\nCSeq: %d\r\n", 
		rtspGetMethodName(method), url, seqNumber);
	requestStr.append(tmpBuffer);

	if(method == eRM_SETUP)
	{
		sprintf(transport, "Transport: RTP/AVP;unicast;client_port=%d-%d\r\n", rtpPort, rtpPort+1);
		requestStr.append(transport);
	}
	else if(method == eRM_PLAY)
	{
		sprintf(range, "Range: npt=0.000-)\r\n");
		requestStr.append(range);
	}

	if(sessionId != "")
	{
		requestStr.append("Session: ");
		requestStr.append(sessionId.c_str());
		requestStr.append("\r\n");

	}
	requestStr.append("\r\n");

	ret = client->send((char*)requestStr.c_str(), requestStr.size());

	if(!ret) return ret;

	int status = client->recv((char*)recvBuffer);
	if (status < 0)
	{
		printf("%s : %s(%d) %s: Socket recv error. size(%d)\n", TAG, __CALL_INFO__, status);
		return -1;
	}

	if(status == 0)
	{
		printf("%s : %s(%d) %s: Socket recv timeout.\n", TAG, __CALL_INFO__);
		return -1;
	}

	parseResponseCode(recvBuffer, responseCode);
	printf("%s : %s(%d) %s: responseCode(%d)\n", TAG, __CALL_INFO__, responseCode);

	return responseCode;
}

bool RTSPClient::parseResponseCode(const char* pLine, int& nResponseCode)
{
	int contentLength = 0;
	char ip[50];
	char port[50];
	char id[50];
	char filename[50];
	char session[50];
	int timeout;
	std::vector<string> lineVec;
	std::string separator = "\r\n";

	split(std::string(pLine), separator, lineVec);
	
	for(uint i=0; i<lineVec.size(); i++)
	{
		sscanf(lineVec[i].c_str(), "Content-Length:%u", &contentLength);
		if(contentLength > 0)
		{
			recvContent(contentLength);
			break;
		}
	}

	if(rtspMethod == eRM_SETUP)
	{
		//Session: 0004f3e6445b92d7;timeout=60
		for(uint i=0; i<lineVec.size(); i++)
		{
			if(sscanf(lineVec[i].c_str(), "Session: %[^';'];timeout=%u", session, &timeout) == 2)
			{
				sessionId = std::string(session);
				rtspTimeout = timeout;
				break;
			}
		}
	}

	if(sscanf(pLine, "RTSP/%*s%u", &nResponseCode) != 1)
	{
		return false;
	}

	if(nResponseCode == 301)
	{
		for(uint i=0; i<lineVec.size(); i++)
		{
			if(sscanf(lineVec[i].c_str(), "Location: rtsp://%[^'@']@%[^':']:%[^'/']/%s", id, ip, &port, filename) == 4)
			{
				userId = std::string(id);
				serverIp = std::string(ip);
				serverPort = atoi(port);
				playFilename = std::string(filename);

				client->close();

				if(!client->create())
				{
					isStop = true;
					return false;
				}

				if(!client->connect(serverIp, serverPort))
				{
					isStop = true;
					return false;
				}

				if(!client->isValid())
				{
					isStop = true;
					return false;
				}
				break;
			}
		}
	}

	return true;
}



bool RTSPClient::recvContent(int nContentLength)
{
	int status;
	char buf[nContentLength];

	status = client->syncRecv(buf, nContentLength);	

	if(status == nContentLength)
	{
		return true;
	}
	return false;
}

int main(int argc, char** argv)
{	
	char input;
	if(argc != 5 || atoi(argv[1]) > 299)
	{
		printf("./client clientcount serverip serverport filename\n");
		printf("clientcount < 300\n");
		printf("example : ./client 200 192.168.11.5 18000 AOA.ts\n");
		return 0;
	}
	
	int clientCount = atoi(argv[1]);
	int testPort = 10100;

	RTSPClient* test[clientCount];
	for(int i=0; i<clientCount; i++)
	{
		test[i] = new RTSPClient(argv[2], atoi(argv[3]), testPort, argv[4]);
		testPort += 2;
		usleep(200*1000);
		printf("client count : %d\n", i);
	}
	
	while((input = getchar()) != 's')
	{
		printf("stop : s\n");
	}

	for(int i=0; i<clientCount; i++)
	{
		test[i]->stop();
	}

	for(int i=0; i<clientCount; i++)
	{
		delete test[i];
	}

	return 0;
}