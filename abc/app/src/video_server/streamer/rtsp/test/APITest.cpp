#include <string.h>
#include <vector>
#include <map>
#include <algorithm>
#include <unistd.h>

#include <nomad_log.h>
#include "APITest.h"
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

static const char* TAG = "API_TEST";

APITest::APITest(std::string serverIp, int serverPort)
{
	client = new RTSPSocket();

	this->serverIp = serverIp;
	this->serverPort = serverPort;

	this->userId = "";
	this->sessionId = "";

	seqNumber = 0;
	rtspMethod = -1;
	rtspTimeout = 60;
}


APITest::~APITest()
{	
	if(client)
	{
		delete client;
		client = NULL;
	}
}

void APITest::start()
{
	std::string sendStr = "";
	int ret =1;

	if(!client->create())
	{
		return;
	}
	
	if(!client->connect(serverIp, serverPort))
	{
		return;
	}

	if(!client->isValid())
	{
		return;
	}

	sendStr = "SET_PARAMETER rtsp://192.168.11.130:18000/api0/asset/ASSETID00022222222221 RTSP/1.0\r\n";
	sendStr.append("CSeq: 0\r\n");
	sendStr.append("Accept: text/parameters\r\n");
	sendStr.append("Content-Type: text/parameters\r\n");
	sendStr.append("Content-Length: 30\r\n");
	sendStr.append("User-Agent: Alticast\r\n\r\n");
	sendStr.append("assetTypeId: MOVIE\n");
	sendStr.append("useFlag: 1\n");
	sendMessage(sendStr);

	sendStr = "SET_PARAMETER rtsp://192.168.11.130:18000/api0/asset/ASSETID00022222222221/system/192.168.11.5.18001 RTSP/1.0\r\n";
	sendStr.append("CSeq: 0\r\n");
	sendStr.append("Accept: text/parameters\r\n");
	sendStr.append("Content-Type: text/parameters\r\n");
	sendStr.append("Content-Length: 85\r\n");
	sendStr.append("User-Agent: Alticast\r\n\r\n");
	sendStr.append("filePath: /\n");
	sendStr.append("fileName: tvN_1994_E06.ts\n");
	sendStr.append("fileSize: 33333\n");
	sendStr.append("protectionType: CAS\n");
	sendStr.append("useFlag: 1\n");
	sendMessage(sendStr);

	sendStr = "SET_PARAMETER rtsp://10.20.0.132:18001/api0/asset/ASSETID00022222222221/vid/test/1111/test.ts RTSP/1.0\r\n";
	sendStr.append("CSeq: 0\r\n");
	sendStr.append("Accept: text/parameters\r\n");
	sendStr.append("Content-Type: text/parameters\r\n");
	sendStr.append("Content-Length: 24\r\n");
	sendStr.append("User-Agent: Alticast\r\n\r\n");
	sendStr.append("useFlag: 1\n");
	sendStr.append("validTime: 1\n");
	sendMessage(sendStr);

	sendStr = "SET_PARAMETER rtsp://10.20.0.132:18001/api0/asset/ASSETID00022222222221/token/ABCDEFG RTSP/1.0\r\n";
	sendStr.append("CSeq: 0\r\n");
	sendStr.append("Accept: text/parameters\r\n");
	sendStr.append("Content-Type: text/parameters\r\n");
	sendStr.append("Content-Length: 46\r\n");
	sendStr.append("User-Agent: Alticast\r\n\r\n");
	sendStr.append("contentId: CONTENTID222222222222\n");
	sendStr.append("validTime: 0\n");
	sendMessage(sendStr);

	sendStr = "SET_PARAMETER rtsp://192.168.11.130:18000/api0/system/192.168.11.5.18001 RTSP/1.0\r\n";
	sendStr.append("CSeq: 0\r\n");
	sendStr.append("Accept: text/parameters\r\n");
	sendStr.append("Content-Type: text/parameters\r\n");
	sendStr.append("Content-Length: 25\r\n");
	sendStr.append("User-Agent: Alticast\r\n\r\n");
	sendStr.append("useFlag: 1\n");
	sendStr.append("activeFlag: 1\n");
	sendMessage(sendStr);

	sendStr = "SET_PARAMETER rtsp://192.168.11.130:18000/api0/system/192.168.11.5.19001 RTSP/1.0\r\n";
	sendStr.append("CSeq: 0\r\n");
	sendStr.append("Accept: text/parameters\r\n");
	sendStr.append("Content-Type: text/parameters\r\n");
	sendStr.append("Content-Length: 25\r\n");
	sendStr.append("User-Agent: Alticast\r\n\r\n");
	sendStr.append("useFlag: 1\n");
	sendStr.append("activeFlag: 1\n");
	sendMessage(sendStr);
}

void APITest::stop()
{
}

int APITest::sendMessage(std::string sendMessage)
{
	seqNumber++;
	std::string requestStr = sendMessage;
	
	bool ret = false;
//	char url[256];
//	char transport[256];
//	char range[256];
//	char tmpBuffer[256];
	char recvBuffer[BUFSIZE];
	int responseCode = -1;

	if(!client->isValid())
	{
		return -1;
	}
		
	ret = client->send((char*)requestStr.c_str(), sendMessage.size());
	printf("send : %s\n", sendMessage.c_str());

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

bool APITest::parseResponseCode(const char* pLine, int& nResponseCode)
{
	int contentLength = 0;
//	char ip[50];
//	char port[50];
//	char id[50];
//	char filename[50];
//	char session[50];
//	int timeout;
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

	if(sscanf(pLine, "RTSP/%*s%u", &nResponseCode) != 1)
	{
		return false;
	}

	return true;
}



bool APITest::recvContent(int nContentLength)
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
	if(argc != 3)
	{
		printf("./client serverip serverport\n");
		printf("example : ./client 192.168.11.5 18000");
		return 0;
	}
	
	APITest test(argv[1], atoi(argv[2]));
	//APITest test = new APITest(argv[1], atoi(argv[2]));

	test.start();

	return 0;
}