#ifndef __RTSPSOCKETHANDLER_H__
#define __RTSPSOCKETHANDLER_H__

#include <string>
#include "nomad.h"
#include "RTSPSocket.h"
#include "RTSPSession.h"


class RTSPMessage;
class RTSPRequest;
class RTSPResponse;
class SDPMaker;
class SessionManager;
class SystemUtil;

void *gRTSPSocketHandlerThreadProc( void * ptr );

class RTSPSocketHandler
{	
public :	
	bool isStop;
	
	RTSPSocket *client;
	RTSPSocketHandler(RTSPSocket *client, SessionManager* sessionManager, SystemUtil* systemUtil);
	virtual ~RTSPSocketHandler();
	
	void handleMessage(string msg);
	void closeClient();
	void stop();
	void removeSession();

protected :
	int getChannel(const std::string filename, RTSPRequest* pRequest);
	//bool parseRequest(CRTSPRequest* pRequest);
	bool parseRequestLine(char* szLine, RTSPRequest* pRequest);
	std::string parseAbsolutePath(std::string absolutePath);
	bool parseHeader(char* szLine, RTSPMessage* pMessage);
	bool recvContent(int nContentLength);
	bool makeRequest(std::string msg);
	HRESULT sendResponse();
	SOCKET createRtpSocket(int& bindOk, int& bindPort);

#ifdef SLAVE
HRESULT doSetupSlave(RTSPRequest* pRequest, RTSPResponse* pResponse);
#endif
	
	
	HRESULT doOptions(RTSPRequest* pRequest, RTSPResponse* pResponse);
	HRESULT doDescribe(RTSPRequest* pRequest, RTSPResponse* pResponse);
	HRESULT doSetup(RTSPRequest* pRequest, RTSPResponse* pResponse);
	HRESULT doPlay(RTSPRequest* pRequest, RTSPResponse* pResponse);
	HRESULT doPause(RTSPRequest* pRequest, RTSPResponse* pResponse);
	HRESULT doTeardown(RTSPRequest* pRequest, RTSPResponse* pResponse);
	HRESULT doGetParameter(RTSPRequest* pRequest, RTSPResponse* pResponse);
//#ifdef SLAVE
	HRESULT doSetParameter(RTSPRequest* pRequest, RTSPResponse* pResponse);
	HRESULT parseContents(RTSPRequest* pRequest, RTSPResponse* pResponse);
//#endif
	HRESULT parseContents(RTSPRequest* pRequest, RTSPResponse* pResponse, std::map<std::string, std::string> &bodyMap);
	const char* makeBody(std::map<std::string, std::string> bodyMap, RTSPSession* session);
		
private :
	pthread_t handlerThread;
	std::string key;

	RTSPRequest* request;
	RTSPResponse* response;
	SDPMaker* sdpMaker;
	SessionManager* sessionManager;
	SystemUtil* systemUtil;

	string sessionID;
	string playFilename;
	string token;


};

#endif
