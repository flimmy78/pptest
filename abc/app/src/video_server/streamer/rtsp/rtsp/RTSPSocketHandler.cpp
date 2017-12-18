
#include <string.h>
#include <vector>
#include <map>
#include <algorithm>

#include <nomad_log.h>
#include "RTSPSocketHandler.h"
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
//#include "RTSPServerManager.h"
#include "SystemUtil.h"

extern map < string, RTSPSocketHandler * > gRTSPSocketHandlerTable;
static const char* TAG = "RTSP";

/** @brief ������ RTSPSocketHandler���� ������ ������ �����ϸ�, ������ ó���Ѵ�.
	@return �ǹ� ����.
	@param ptr RTSPSocketHandler
*/
void *gRTSPSocketHandlerThreadProc(void *ptr)
{
	RTSPSocketHandler *rtspclient = (RTSPSocketHandler*)ptr;

	int status;
	char strIn[BUFSIZE];
	while(!rtspclient->isStop)
	{
		memset(strIn, 0x00, BUFSIZE);
		rtspclient->client->setRecvTimeout(DEFAULT_RTSP_TIMEOUT, 0);
		status = rtspclient->client->recv((char*)strIn);
		if (status < 0)
		{
			LOGE(TAG, "%s(%d) %s: Socket recv error. size(%d)", __CALL_INFO__, status);
			break;
		}

		if(status == 0) // timeout
		{
			LOGE(TAG, "%s(%d) %s: Socket recv timeout.", __CALL_INFO__);
			//continue;
		}
		LOGD(TAG, "%s(%d) %s: Socket recv msg(\n%s)", __CALL_INFO__, strIn);
		rtspclient->handleMessage(std::string(strIn));
	}

	rtspclient->removeSession();

	delete rtspclient;
	return (void*)NULL;
}

/** @brief RTSPSocketHandler ������. ������ socket���� �����͸� �о� �Ľ��ϰ� ��û�� ���ؼ� �����Ѵ�.
	@return �ǹ� ����.
	@param client ������ rtsp socket
	@param sessionManager session�� ������ manager
	@param rtspServerManager slave rtsp server�� ������ manager
	@param systemUtil ���� system�� ���¸� �������� ����.(cpu, memory ��)
*/
RTSPSocketHandler::RTSPSocketHandler(RTSPSocket *client, SessionManager* sessionManager, SystemUtil* systemUtil)
{
	key = "";
	key.append(client->remoteIP());
	key.append(":");
	key.append(intToString(client->remotePort()));

	request = new RTSPRequest();
	response = new RTSPResponse();
	sdpMaker = new SDPMaker();
	sdpMaker->setPayloadType(96);
	this->sessionManager = sessionManager;
	this->systemUtil = systemUtil;

	sessionID = "";
	playFilename = "";
	token = "";

	isStop = false;
	this->client = client;


	pthread_attr_t attr;
	pthread_attr_init(&attr);

	size_t stacksize = 1024*1024;
	pthread_attr_setstacksize (&attr, stacksize);

	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	if(pthread_create( &handlerThread, &attr, gRTSPSocketHandlerThreadProc, (void*) this))
	{
		LOGE(TAG, "%s(%d) %s: pthread create fail.", __CALL_INFO__);
	}
	pthread_attr_destroy(&attr);
}

/** @brief RTSPSocketHandler �Ҹ���. ������ socket���� �����͸� �о� �Ľ��ϰ� ��û�� ���ؼ� �����Ѵ�.
	@return �ǹ� ����.
	@param ����.
*/
RTSPSocketHandler::~RTSPSocketHandler()
{
	isStop = true;
	closeClient();

	delete request;
	delete response;
	delete sdpMaker;
}


/** @brief socket�� delete �ϰ� �������� �׸񿡼� �����Ѵ�.
	@return �ǹ� ����.
	@param ����.
*/
void RTSPSocketHandler::closeClient()
{
	if(client)
	{
		//sessionManager->deleteSession(sessionID.c_str());
		delete client;
		client = NULL;
		gRTSPSocketHandlerTable.erase(key);
		LOGD(TAG, "%s(%d) %s: close client(%s)", __CALL_INFO__, key.c_str());
	}
}

void RTSPSocketHandler::removeSession()
{
	RTSPSession* pSession;
	pSession = sessionManager->getSession(sessionID.c_str());
	if(pSession != (RTSPSession*)NULL)
	{
		pSession->setState(eRS_INIT);
		delete pSession;
	}
}

/** @brief ������ rtsp socket �� �����Ѵ�.
	@return �ǹ� ����.
	@param ����.
*/
void RTSPSocketHandler::stop()
{
	isStop = true;
	if(client) client->close();
}

/** @brief socket ���� ���� data�� �Ľ��ϰ� ó�� �Ѵ�.(socket�� ���ؼ� ������ ������.)
	@return �ǹ� ����.
	@param msg socket���� ���� data
*/
void RTSPSocketHandler::handleMessage(string msg)
{
	bool bRet;
	HRESULT hr;
	int nMethod;

	bRet = makeRequest(msg);

	if(!bRet)
	{
		isStop = true;
		return;
	}

	hr = request->getMethod(&nMethod);

	switch(nMethod)
	{
		case eRM_OPTIONS:
			LOGD_CALL(TAG, "OPTIONS");
			hr = doOptions(request, response);
			break;
		case eRM_DESCRIBE:
			LOGD_CALL(TAG, "DESCRIBE");
			hr = doDescribe(request, response);
			break;
		case eRM_SETUP:
			LOGD_CALL(TAG, "SETUP");
			hr = doSetup(request, response);
			break;
		case eRM_PLAY:
			LOGD_CALL(TAG, "PLAY");
			hr = doPlay(request, response);
			break;
		case eRM_PAUSE:
			LOGD_CALL(TAG, "PAUSE");
			hr = doPause(request, response);
			break;
		case eRM_TEARDOWN:
			LOGD_CALL(TAG, "TEARDOWN");
			hr = doTeardown(request, response);
			break;
		case eRM_GET_PARAMETER:
			LOGD_CALL(TAG, "GET_PARAMETER");
			hr = doGetParameter(request, response);
			break;
		case eRM_SET_PARAMETER:
			LOGD_CALL(TAG, "SET_PARAMETER");
			hr = doSetParameter(request, response);
			break;
		default:
			// 501: Not Implemented
			hr = E_NOTIMPL;
			break;
	}

	if(hr == E_NOTIMPL)
	{
		Header* pHeader;

		// CSeq
		hr = request->getHeader(eRH_CSEQ, &pHeader);
		if(SUCCEEDED(hr))
		{
			response->addHeader(pHeader);
		}

		// Content-Length
		hr = request->getHeader(eRH_CONTENT_LENGTH, &pHeader);
		if(FAILED(hr))
		{
			pHeader = new Header(eRH_CONTENT_LENGTH);
		}
		hr = pHeader->setIntValue(0);
		hr = response->addHeader(pHeader);

		response->setStatusCode(501);
	}

	bRet = sendResponse();
	response->clear();

	if(!bRet || nMethod == eRM_TEARDOWN)
	{
		isStop = true;
		return;
	}
}

/** @brief rtsp ��û�� �´� ���� ������ ������.
	@return �ǹ� ����.
	@param msg socket���� ���� data
*/
HRESULT RTSPSocketHandler::sendResponse()
{
	bool bRet;
	HRESULT hr;

	int nLength;
	int nDataSize;
	int nBufSize = 2048;
	char pBuf[nBufSize];

	char* szStatusLine = pBuf;
	char* szHeaders;
	char* pContent;

	// STATUS LINE
	hr = response->getStatusLine(szStatusLine, nBufSize);
	nLength = strlen(szStatusLine);

	// HEADERS
	szHeaders = pBuf + nLength;
	nBufSize -= nLength;
	nDataSize = nLength;
	hr = response->getHeaders(szHeaders, nBufSize);
	nLength = strlen(szHeaders);
	nDataSize += nLength;

	// CONTENT
	hr = response->getContent(&pContent, &nLength);
	if(SUCCEEDED(hr) && nLength > 0)
	{
		memcpy(pBuf + nDataSize, pContent, nLength);
		nDataSize += nLength;
		pBuf[nDataSize] = '\0';
	}

	LOGD(TAG, "%s(%d) %s\n%s", __CALL_INFO__, pBuf);

	bRet = client->send(pBuf, nDataSize);

	return bRet;
}

/** @brief rtp udp socket�� ������.
	@return bind ������ SOCKET
	@param bindOk bind ��������
	@param bindPort bind �� ��Ʈ
*/
SOCKET RTSPSocketHandler::createRtpSocket(int& bindOk, int& bindPort)
{
	SOCKET sockfd = -1;

	struct sockaddr_in sockAddr;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		return sockfd;
	}

	for(int i=0; i<100; i++)
	{
		bzero(&sockAddr, sizeof(sockAddr));
		sockAddr.sin_family = AF_INET;
		sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		bindPort = getRtpPort();
		sockAddr.sin_port = htons(bindPort);

		bindOk = bind(sockfd, (struct sockaddr *)&sockAddr, sizeof(sockAddr));
		if(bindOk != -1) return sockfd;
	}

	::close(sockfd);
	sockfd = -1;
	return sockfd;
}



/** @brief socket���� ���� string message�� ������ RTSPRequest���·� �������ش�.
	@return true or false
	@param msg socket���� ���� string message
*/
bool RTSPSocketHandler::makeRequest(string msg)
{
	bool bRet = true;
	std::vector<string> lineVec;
	std::string separator = "\r\n";
	Header* pHeader;

	HRESULT hr;

	split(msg, separator, lineVec);

	for(uint i=0; i<lineVec.size(); i++)
	{
		if(i == 0)
		{
			request->clear();
			parseRequestLine((char*)lineVec[i].c_str(), request);
		}
		else
		{
			parseHeader((char*)lineVec[i].c_str(), request);
		}
	}

	// Extracting a content...
	int nContentLength = 0;
	hr = request->getHeader(eRH_CONTENT_LENGTH, &pHeader);
	if(SUCCEEDED(hr))
	{
		nContentLength = pHeader->getIntValue();
	}

	if(nContentLength > 0)
	{
		bRet = recvContent(nContentLength);
	}
	return bRet;
}

/** @brief rtsp header�� contentsLength��ŭ�� body data�� socket���� �о� �´�.
	@return true or false
	@param nContentLength �о��� body data�� ũ��
*/
bool RTSPSocketHandler::recvContent(int nContentLength)
{
	int status;
	char buf[nContentLength];

	status = client->syncRecv(buf, nContentLength);

	if(status == nContentLength)
	{
		request->setContent(buf, nContentLength);
		LOGD(TAG, "%s(%d) %s: rtsp_body(\n%s)", __CALL_INFO__, buf);
		return true;
	}
	return false;
}

/** @brief socket���� �о��� �����͸� line ������ �о� Header���·� ������ RTSPMessage�� �־��ش�.
	@return true or false
	@param szLine line ������ data
	@param pMessage line ������ data�� �Ľ��� Header�� ������ Header�� ��� RTSPMessage
*/
bool RTSPSocketHandler::parseHeader(char* szLine, RTSPMessage* pMessage)
{
	int i;
	//HRESULT hr;
	int nHeader;
	Header* pHeader;

	char* szHeaderName;
	char* szHeaderValue;

	int nLength = strlen(szLine);

	for(i=0; i<nLength; i++)
	{
		if(szLine[i] == ':')
		{
			szLine[i] = '\0';
			break;
		}
	}

	if(i == nLength)
	{
		return E_FAIL;
	}

	szHeaderName = szLine;

	// Unsupported Header
	nHeader = rtspGetHeader(szHeaderName);
	if(nHeader < 0)
	{
		return E_FAIL;
	}

	pHeader = Header::create(nHeader);

	pHeader->setName(nHeader);

	i++;
	while(szLine[i] == ' ' || szLine[i] == '\t')
	{
		i++;
	}

	szHeaderValue = &szLine[i];

	if( nHeader == eRH_CSEQ				||
		nHeader == eRH_CONTENT_LENGTH	||
		nHeader == eRH_BLOCKSIZE		||
		nHeader == eRH_BANDWIDTH			)
	{

		if(szHeaderValue == NULL)
		{
			LOGE_LINE(TAG);
			return E_FAIL;
		}
		int nValue = atoi(szHeaderValue);
		pHeader->setIntValue(nValue);
	}
	else
	{
		pHeader->setStringValue(szHeaderValue);
	}

	pMessage->addHeader(pHeader);

	return S_OK;
}

/** @brief master�� ���� token ���� ����, vid ���� ������ ���� db���� duration�� �˾ƿ��� slave�� ���� index������ ���ؼ� duration�� �˾ƿ´�.
	@return true or false
	@param filename duration�� �˾ƿ� filename
	@param pRequest duration�� �˾ƾ��� RTSPRequest message
*/
int RTSPSocketHandler::getChannel(const std::string filename, RTSPRequest* pRequest)
{
	int channel = 0;
	if(filename.size() != 6) return channel; // /live1, /live2, /live3

	channel = atoi(filename.substr(5, 1).c_str());

	if(channel < 1 || channel > 3) return channel; // /live1, /live2, /live3

	LOGD(TAG, "%s(%d) %s: file_name_(%s), channel(%d)", __CALL_INFO__, filename.c_str(), channel);
	return channel;
}

/** @brief socket���� ���� msg�� ���� rtsp version host ip, port ���� �Ľ��Ѵ�.
	@return true or false
	@param szLine �Ľ��� string line
	@param pRequest �Ľ��� data�� ���� RTSPRequest
*/
bool RTSPSocketHandler::parseRequestLine(char* szLine, RTSPRequest* pRequest)
{
	const int METHOD	= 0;
	const int URL		= 1;
	const int VERSION	= 2;

	const char* szSeparator = " ";
	char szProtocol[8];
	char szUserID[64];
	char szHost[64];
	int nPort;
	char szAbsolutePath[128];
	char* szNext;

	memset(szUserID, 0x00, 20);

	int i;
	HRESULT hr;

	int nMethod;
	int nMajorVersion;
	int nMinorVersion;
	char* szToken;
	std::string strAbsolutePath;

	//int nMethodCount = rtspGetMethodCount();
	rtspGetMethodCount();

	szToken = strtok_r(szLine, szSeparator, &szNext);

	for(i=0; szToken != NULL; i++)
	{
		switch(i)
		{
		case METHOD:
			nMethod = rtspGetMethod(szToken);
			if(nMethod < 0)
			{
				ASSERT(FALSE);
				return E_FAIL;
			}

			pRequest->setMethod(nMethod);
			break;

		case URL:
			hr = ::parseURL(szToken, szProtocol, szUserID, szHost, &nPort, szAbsolutePath);
			hr = pRequest->setHost(szHost);
			hr = pRequest->setPort(nPort);
			strAbsolutePath = parseAbsolutePath(std::string(szAbsolutePath));
			hr = pRequest->setAbsolutePath((char*)strAbsolutePath.c_str());
			break;

		case VERSION:
			hr = ::rtspParseVersion(szToken, &nMajorVersion, &nMinorVersion);
			ASSERT(SUCCEEDED(hr));
			hr = pRequest->setVersion(nMajorVersion, nMinorVersion);
			break;
		}

		szToken = strtok_r(NULL, szSeparator, &szNext);
	}

	return S_OK;
}

/** @brief url path�� �Ľ��Ͽ� token, id, passwd, alticast protocol�� �����Ѵ�.
	@return string
	@param absolutePath �Ľ��� url path
*/
std::string RTSPSocketHandler::parseAbsolutePath(std::string absolutePath)
{
	std::string retStr = "";
	std::vector<std::string> tokenVec;
	std::vector<std::string> apiVec;

	split(absolutePath, '?', tokenVec);

	if(tokenVec.size() > 2)
	{
		retStr = absolutePath;
		return retStr;
	}
	else if(tokenVec.size() == 2)
	{
		retStr = tokenVec[0];
		std::vector<std::string> tmpVec;
		split(tokenVec[1], '=', tmpVec);
		if(tmpVec.size() == 2 && tmpVec[0] == "token")
		{
			if(endsWith(tmpVec[1], "/track1"))
			{
				this->token = tmpVec[1].substr(0, tmpVec[1].size() - 7);
				retStr.append("/track1");
			}
			else if(endsWith(tmpVec[1], "/"))
			{
				this->token = tmpVec[1].substr(0, tmpVec[1].size() - 1);
				//retStr.append("/");
			}
			else
			{
				this->token = tmpVec[1];
			}
		}
	}
	else
	{
		if(endsWith(absolutePath, "/track1"))
		{
			retStr = absolutePath.substr(0, absolutePath.size() - 7);
			retStr.append("/track1");
		}
		else if(endsWith(absolutePath, "/"))
		{
			retStr = absolutePath.substr(0, absolutePath.size() - 1);
		}
		else
		{
			retStr = absolutePath;
		}
	}

	LOGD(TAG, "%s(%d) %s: token(%s)", __CALL_INFO__, token.c_str());

	return retStr;
}


/** @brief option method�� ó���Ѵ�.
	@return E_FAIL or S_OK
	@param pRequest ó���� RTSPRequest
	@param pResponse �����ؾ��� RTSPResponse
*/
HRESULT RTSPSocketHandler::doOptions(RTSPRequest* pRequest, RTSPResponse* pResponse)
{
	HRESULT hr;
	Header* pHeader;

	// CSeq
	hr = pRequest->getHeader(eRH_CSEQ, &pHeader);
	pResponse->addHeader(pHeader);

	// Content-Length
	hr = pRequest->getHeader(eRH_CONTENT_LENGTH, &pHeader);
	if(FAILED(hr))
	{
		pHeader = new Header(eRH_CONTENT_LENGTH);
	}
	hr = pHeader->setIntValue(0);
	hr = pResponse->addHeader(pHeader);

	// Public
	pHeader = new Header(eRH_PUBLIC);
	hr = pHeader->setStringValue(
		"DESCRIBE,SETUP,TEARDOWN,PLAY,PAUSE,GET_PARAMETER");
	hr = pResponse->addHeader(pHeader);

	pResponse->setStatusCode(200);

	return S_OK;
}


/**
 * Handle DESCRIBE RTSP method.
 *
 * @param pRequest
 * @param pResponse
 * @return
 */
/** @brief describe method�� ó���Ѵ�.
	@return E_FAIL or S_OK
	@param pRequest ó���� RTSPRequest
	@param pResponse �����ؾ��� RTSPResponse
*/
HRESULT RTSPSocketHandler::doDescribe(RTSPRequest* pRequest, RTSPResponse* pResponse)
{
	LOGV_CALL(TAG, "[START]");

	HRESULT hr;

	Header* pHeader;

	sdpMaker->setHost(std::string(pRequest->getHost()));
	playFilename = pRequest->getAbsolutePath();
	const int channel = getChannel(playFilename, pRequest);
	if(channel < 1 || channel > 3)
	{
		pResponse->setStatusCode(404);
		return S_OK;
	}
	sdpMaker->setVideoInfo(sessionManager->getVideoInfo(channel-1));
	sdpMaker->run();
	const char* pContent = sdpMaker->toString();
	int nContentLength = sdpMaker->getLength();

	// Content-Base
	char pBase[1024];
	if(pRequest->getPort() > 0)
	{
		sprintf(pBase, "rtsp://%s:%d%s", pRequest->getHost(), pRequest->getPort(), pRequest->getAbsolutePath());
	}
	else
	{
		sprintf(pBase, "rtsp://%s%s", pRequest->getHost(), pRequest->getAbsolutePath());
	}
	pHeader = new Header(eRH_CONTENT_BASE);
	hr = pHeader->setStringValue(pBase);
	hr = pResponse->addHeader(pHeader);

	// Content-Type
	pHeader = new Header(eRH_CONTENT_TYPE);
	hr = pHeader->setStringValue("application/sdp");
	hr = pResponse->addHeader(pHeader);

	// Content-Length
	pHeader = new Header(eRH_CONTENT_LENGTH);
	hr = pHeader->setIntValue(nContentLength);
	hr = pResponse->addHeader(pHeader);

	// Cache-Control
	pHeader = new Header(eRH_CACHE_CONTROL);
	hr = pHeader->setStringValue("no-cache");
	hr = pResponse->addHeader(pHeader);

	// CSeq
	hr = pRequest->getHeader(eRH_CSEQ, &pHeader);
	LOGV(TAG, "%s:%d hr(%X)", __FILENAME__, __LINE__, hr);
	if(SUCCEEDED(hr))
	{
		pResponse->addHeader(pHeader);
	}
	else
	{
		LOGE("ERROR", "%s:%d CSeq doesn't exist", __FILENAME__, __LINE__);
	}

	hr = pResponse->setStatusCode(200);
	hr = pResponse->setContent(pContent, nContentLength);

	LOGV_CALL(TAG, "[END]");

	return hr;
}

#ifdef SLAVE
/** @brief slave rtsp server ���� setup method�� ó���Ѵ�.
	@return E_FAIL or S_OK
	@param pRequest ó���� RTSPRequest
	@param pResponse �����ؾ��� RTSPResponse
*/
HRESULT RTSPSocketHandler::doSetupSlave(RTSPRequest* pRequest, RTSPResponse* pResponse)
{
	HRESULT hr;
	RTSPSession* pSession = NULL;
	//int nStreamType;
	int nRTPPort;
	//int nRTCPPort;
	//int nSrcPort;
	//UINT nSSRC;
	int nTimeout;
	std::string strDestination;
	int nTTL;

	Header* pHeader;
	TransportHeader* pTransportHeader;

	char szBuf[64];
	//char userID[64];
	char getFilename[128];
	memset(getFilename, 0x00, sizeof(getFilename));

	hr = pRequest->getHeader(eRH_SESSION, &pHeader);

	// Inspect a stream type with request url.
	char* szAbsolutePath = pRequest->getAbsolutePath();
	char* findStr = strstr(szAbsolutePath, "/track");
	if(findStr)
	{
		//const int filenameLength = (int)(findStr - szAbsolutePath);
		//strncpy(getFilename, szAbsolutePath, filenameLength);
		//getFilename[filenameLength] = '\0';
	}
	else
	{
		strncpy(getFilename, szAbsolutePath, strlen(szAbsolutePath));
		playFilename = std::string(getFilename);
	}

	const int channel = getChannel(playFilename, pRequest);
	if(channel < 1 || channel > 3)
	{
		pResponse->setStatusCode(404);
		return S_OK;
	}

	/*hr = */pRequest->getHeader(eRH_TRANSPORT, (Header**) &pTransportHeader);
	const int nLowerTransport = pTransportHeader->getLowerTransport();

	if(nLowerTransport == eLT_UDP)
	{
		pResponse->setStatusCode(461);
		return S_OK;
	}

	if(FAILED(hr))
	{
		pSession = new RTSPSession(playFilename);
		sessionID = sessionManager->createSessionID();
	}
	else
	{
		sessionID = std::string(pHeader->getStringValue());
		pSession = sessionManager->getSession(sessionID.c_str());
	}

	if(pSession == NULL) return E_FAIL;

	nRTPPort  = pTransportHeader->getPort(CLIENT_PORT_RTP);
	//nRTCPPort = pTransportHeader->getPort(CLIENT_PORT_RTCP);

	strDestination = pTransportHeader->getDestination();
	nTTL = pTransportHeader->getTTL();


	if(pTransportHeader->getPort(PORT_RTP) != 0)
	{
		nRTPPort = pTransportHeader->getPort(PORT_RTP);
		//nRTCPPort = pTransportHeader->getPort(PORT_RTCP);
	}

	int bindOk = -1;
	int bindPort = 0;
	SOCKET rtpSoc;
	//if(strstr(szAbsolutePath, "/track1"))
	if(1)
	{
		RTSPSession::SParam* param = new RTSPSession::SParam;
		param->strSessionID = sessionID;
		param->token = this->token;
		if(strDestination != "")
		{
			param->strIP = strDestination;
		}
		else
		{
			param->strIP = client->remoteIP();
		}

		param->transportType = nLowerTransport;
		if(nLowerTransport == eLT_TCP)
		{
			param->rtspFD = client->socketFD;
			param->interleavedData = pTransportHeader->getChannel(eIC_DATA);
			param->interleavedControl = pTransportHeader->getChannel(eIC_CONTROL);
		}
		else
		{
			param->rtspFD = 0;
		}
		param->dstRtpPort = nRTPPort;
		param->dstRtcpPort = nRTPPort;
		param->SSRC = 0x10000001u;
		//param->payloadType = 33; // ts
		param->payloadType = 96; // h264
		//param->streamType = eST_TS; // ts
		param->streamType = eST_VIDEO;	// h264

		param->sampleRate = 90000;
		param->channel = 0;

		bindOk = -1;
		bindPort = 0;
		rtpSoc = createRtpSocket(bindOk, bindPort);

		if(bindOk == -1)
		{
			delete pSession;
			return E_FAIL;
		}
		param->srcRtpPort = bindPort;
		param->srcRtcpPort = 0;
		param->rtpFD = rtpSoc;

		pSession->setParam(param);
		pSession->setFilename(playFilename);

		pSession->setScalerId(channel - 1);
	}
	else
	{
		delete pSession;
		return E_FAIL;
	}
	// CSeq
	hr = pRequest->getHeader(eRH_CSEQ, &pHeader);
	if(SUCCEEDED(hr))
	{
		pResponse->addHeader(pHeader);
	}
	else
	{
		LOGE(TAG, "%s(%d) %s: CSeq doesn't exist", __CALL_INFO__);
		delete pSession;
		return E_FAIL;
	}

	// Session
	LOGV_CALL(TAG, "Session");
	hr = pRequest->getHeader(eRH_SESSION, &pHeader);
	if(FAILED(hr))
	{
		pHeader = new Header(eRH_SESSION);
	}

	nTimeout = pSession->getTimeout();
	sprintf(szBuf, "%s;timeout=%d", sessionID.c_str(), nTimeout);
	pHeader->setStringValue(szBuf);

	pResponse->addHeader(pHeader);


	// Transport
	LOGV_CALL(TAG, "Transport");
	pTransportHeader->setSSRC(0x10000001u);
	pTransportHeader->setMode(eRTHM_PLAY);
	if(nLowerTransport == eLT_UDP)
	{
		pTransportHeader->setPort(SERVER_PORT_RTP, bindPort);
		pTransportHeader->setPort(SERVER_PORT_RTCP, 0);
	}
	if(nTTL > 0) pTransportHeader->setTTL(nTTL);
	hr = pResponse->addHeader(pTransportHeader);

	// Content-Length
	LOGV_CALL(TAG, "Content-Length");
	hr = pRequest->getHeader(eRH_CONTENT_LENGTH, &pHeader);
	if(FAILED(hr))
	{
		pHeader = new Header(eRH_CONTENT_LENGTH);
	}
	hr = pHeader->setIntValue(0);
	hr = pResponse->addHeader(pHeader);

	// Cache-Control
	LOGV_CALL(TAG, "Cache-Control");
	pHeader = new Header(eRH_CACHE_CONTROL);
	hr = pHeader->setStringValue("no-cache");
	hr = pResponse->addHeader(pHeader);

	pHeader = new Header(eRH_BLOCKSIZE);
	hr = pHeader->setIntValue(1500);
	hr = pResponse->addHeader(pHeader);

	pResponse->setStatusCode(200);
	sessionManager->addSession(pSession);

	LOGV_CALL(TAG, "[END]");
	return S_OK;
}
#endif


/** @brief setup method�� ó���Ѵ�.
	@return E_FAIL or S_OK
	@param pRequest ó���� RTSPRequest
	@param pResponse �����ؾ��� RTSPResponse
*/
HRESULT RTSPSocketHandler::doSetup(RTSPRequest* pRequest, RTSPResponse* pResponse)
{
	return doSetupSlave(pRequest, pResponse);
	return S_OK;
}

/** @brief play method�� ó���Ѵ�.
	@return E_FAIL or S_OK
	@param pRequest ó���� RTSPRequest
	@param pResponse �����ؾ��� RTSPResponse
*/
HRESULT RTSPSocketHandler::doPlay(RTSPRequest* pRequest, RTSPResponse* pResponse)
{
	HRESULT hr;
	Header* pHeader;
	RangeHeader* pRangeHeader = NULL;
	RTSPSession* pSession;
	double llStartTime = 0.0;
	double llEndTime = 0.0;

	BOOL bSeek = FALSE;
	//int anSeq[2];
	//UINT anRtpTime[2];
	char szBuf[256];

	// Session
	hr = pRequest->getHeader(eRH_SESSION, &pHeader);
	if(FAILED(hr))
	{
		LOGE_CALL("ERROR", "SESSION NOT FOUND.");
		return E_FAIL;
	}

	// Range header
	hr = pRequest->getHeader(eRH_RANGE, (Header**) &pRangeHeader);
	if(SUCCEEDED(hr))
	{
		llStartTime = pRangeHeader->getStartTime();
		llEndTime = pRangeHeader->getEndTime();

		bSeek = TRUE;
	}
	LOGD(TAG, "%s(%d) hr(%X) seek(%d) Range: %.3f - %.3f", __FILENAME__, __LINE__, hr, bSeek, llStartTime, llEndTime);

	// Session
	pSession = sessionManager->getSession(pHeader->getStringValue());
	if(pSession == NULL)
	{
		LOGE_CALL("ERROR", "SESSION IS NULL.");
		return E_FAIL;
	}
	pResponse->addHeader(pHeader);

	pSession->setState(eRS_READY);

	// Content-Length
	pHeader = new Header(eRH_CONTENT_LENGTH);
	hr = pHeader->setIntValue(0);
	hr = pResponse->addHeader(pHeader);

	// Cache-Control
	pHeader = new Header(eRH_CACHE_CONTROL);
	hr = pHeader->setStringValue("no-cache");
	hr = pResponse->addHeader(pHeader);

	// CSeq
	hr = pRequest->getHeader(eRH_CSEQ, &pHeader);
	if(SUCCEEDED(hr))
	{
		pResponse->addHeader(pHeader);
	}
	else
	{
		LOGE("ERROR", "%s(%d) CSeq doesn't exist", __FILENAME__, __LINE__);
	}

	pResponse->setStatusCode(200);

	if(pRangeHeader == NULL)
	{
		pRangeHeader = new RangeHeader();
	}

	llEndTime = sdpMaker->getDuration();

	if(bSeek && llStartTime > 0.1)
	{
		pSession->setSeekPosition(static_cast<float>(llStartTime));
		llStartTime = static_cast<double>(pSession->getSeekPosition());
	}
	else
	{
		llStartTime = double(pSession->getNptTime()/90000.0);
	}

	pRangeHeader->setStartTime(llStartTime);
	pRangeHeader->setEndTime(llEndTime);
	pResponse->addHeader(pRangeHeader);

	const char* szHost = pRequest->getHost();
	const char* szAbsolutePath = pRequest->getAbsolutePath();
	const int nPort = pRequest->getPort();

	LOGD(TAG, "%s(%d) %s: path(%s)", __CALL_INFO__, szAbsolutePath);

	// Scale
	hr = pRequest->getHeader(eRH_SCALE, &pHeader);
	if(FAILED(hr))
	{
		LOGD_CALL(TAG, "SCALE NOT FOUND.");
		pSession->setScaleValue(10);
	}
	else
	{
		char tmpChar[15];
		memset(tmpChar, 0x00, sizeof(tmpChar));

		const char* scaleValue = pHeader->getStringValue();
		double scale = ::atof(scaleValue);
		if(scale < 2.0)
		{
			sprintf(tmpChar, "%.1lf", scale);
		}
		else
		{
			sprintf(tmpChar, "%.0lf", scale);
		}
		scale = ::atof(tmpChar);

		pHeader->setStringValue(tmpChar);
		pSession->setScaleValue(static_cast<int>(scale*10));
		pResponse->addHeader(pHeader);
	}

	// RTP-Info
	if(nPort > 0)
	{
		sprintf(szBuf, "url=rtsp://%s:%d%s;seq=%d;rtptime=%u", szHost, nPort, szAbsolutePath, pSession->getSequence(), pSession->getRtpTime());
	}
	else
	{
		sprintf(szBuf, "url=rtsp://%s:%d%s;seq=%d;rtptime=%u", szHost, nPort, szAbsolutePath, pSession->getSequence(), pSession->getRtpTime());
	}

	// RTP-Info
	pHeader = new Header(eRH_RTP_INFO);
	hr = pHeader->setStringValue(szBuf);
	hr = pResponse->addHeader(pHeader);

	if(pSession)
	{
		pSession->setState(eRS_PLAYING);
	}

	sessionManager->addSession(pSession);
	return S_OK;
}

/**
 * This method is related to seeking.
 * Seeking: PLAY -> PAUSE -> PLAY
 *
 * @param pRequest
 * @param pResponse
 * @return
 */
/** @brief pause method�� ó���Ѵ�.
	@return E_FAIL or S_OK
	@param pRequest ó���� RTSPRequest
	@param pResponse �����ؾ��� RTSPResponse
*/
HRESULT RTSPSocketHandler::doPause(RTSPRequest* pRequest, RTSPResponse* pResponse)
{
	HRESULT hr;
	Header* pHeader;
	RTSPSession* pSession;

	// Session
	hr = pRequest->getHeader(eRH_SESSION, &pHeader);
	if(FAILED(hr))
	{
		return E_FAIL;
	}

	// Session
	pSession = sessionManager->getSession(pHeader->getStringValue());
	if(pSession == NULL)
	{
		LOGE_CALL("ERROR", "SESSION IS NULL.");
		return E_FAIL;
	}
	pResponse->addHeader(pHeader);

	// Content-Length
	hr = pRequest->getHeader(eRH_CONTENT_LENGTH, &pHeader);
	if(FAILED(hr))
	{
		pHeader = new Header(eRH_CONTENT_LENGTH);
	}
	hr = pHeader->setIntValue(0);
	hr = pResponse->addHeader(pHeader);

	// Cache-Control
	pHeader = new Header(eRH_CACHE_CONTROL);
	hr = pHeader->setStringValue("no-cache");
	hr = pResponse->addHeader(pHeader);

	// CSeq
	hr = pRequest->getHeader(eRH_CSEQ, &pHeader);
	if(SUCCEEDED(hr))
	{
		pResponse->addHeader(pHeader);
	}
	else
	{
		LOGE("ERROR", "%s(%d) %s: CSeq doesn't exist", __FILENAME__, __LINE__, __FUNCTION__);
	}

	pResponse->setStatusCode(200);

	if(pSession)
	{
		pSession->setState(eRS_READY);
	}
	sessionManager->addSession(pSession);

	return S_OK;
}

/** @brief teardown method�� ó���Ѵ�.
	@return E_FAIL or S_OK
	@param pRequest ó���� RTSPRequest
	@param pResponse �����ؾ��� RTSPResponse
*/
HRESULT RTSPSocketHandler::doTeardown(RTSPRequest* pRequest, RTSPResponse* pResponse)
{
	LOGV_CALL(TAG, "[START]");

	HRESULT hr;
	Header* pHeader;
	RTSPSession* pSession;

	// Session
	hr = pRequest->getHeader(eRH_SESSION, &pHeader);
	if(FAILED(hr))
	{
		LOGE_CALL(TAG, "SESSION NOT FOUND");
		return E_FAIL;
	}

	pSession = sessionManager->getSession(pHeader->getStringValue());
	if(pSession == (RTSPSession*)NULL)
	{
		LOGE_CALL(TAG, "SESSION IS NULL");
		return E_FAIL;
	}

	pResponse->addHeader(pHeader);

	// CSeq
	hr = pRequest->getHeader(eRH_CSEQ, &pHeader);
	pResponse->addHeader(pHeader);

	// Content-Length
	hr = pRequest->getHeader(eRH_CONTENT_LENGTH, &pHeader);
	if(FAILED(hr))
	{
		pHeader = new Header(eRH_CONTENT_LENGTH);
	}
	hr = pHeader->setIntValue(0);
	hr = pResponse->addHeader(pHeader);

	pResponse->setStatusCode(200);

	pSession->setState(eRS_INIT);

	delete pSession;

	LOGV_CALL(TAG, "[END]");

	return S_OK;
}

/** @brief getparameter method�� ó���Ѵ�.
	@return E_FAIL or S_OK
	@param pRequest ó���� RTSPRequest
	@param pResponse �����ؾ��� RTSPResponse
*/
HRESULT RTSPSocketHandler::doGetParameter(RTSPRequest* pRequest, RTSPResponse* pResponse)
{
	LOGV_CALL(TAG, "[START]");
	HRESULT hr;
	Header* pHeader;
	RTSPSession* pSession = NULL;
	std::map<std::string, std::string> bodyMap;

	// Session
	hr = pRequest->getHeader(eRH_SESSION, &pHeader);
	if(FAILED(hr))
	{
		LOGE_CALL(TAG, "SESSION NOT FOUND");
		//return E_FAIL;
	}
	else
	{
		pSession = sessionManager->getSession(pHeader->getStringValue());
		if(pSession == (RTSPSession*)NULL)
		{
			LOGE_CALL(TAG, "SESSION IS NULL");
			return E_FAIL;
		}
	}

	// CSeq
	hr = pRequest->getHeader(eRH_CSEQ, &pHeader);
	if(SUCCEEDED(hr))
	{
		pResponse->addHeader(pHeader);
	}

	pResponse->setStatusCode(200);
	parseContents(pRequest, pResponse, bodyMap);

	std::string apiVersion;
	std::string command;
	std::string assetId;
	std::string subCommand;
	std::string value;

	const char* pContent = makeBody(bodyMap, pSession);
	int nContentLength = strlen(pContent);

	// Content-Length
	hr = pRequest->getHeader(eRH_CONTENT_LENGTH, &pHeader);
	if(FAILED(hr))
	{
		pHeader = new Header(eRH_CONTENT_LENGTH);
	}
	hr = pHeader->setIntValue(nContentLength);
	hr = pResponse->addHeader(pHeader);

	if(nContentLength > 0)
	{
		hr = pResponse->setContent(pContent, nContentLength);
	}

	if(pSession)
	{
		sessionManager->addSession(pSession);
	}

	LOGV_CALL(TAG, "[END]");
	return S_OK;
}

//#ifdef SLAVE
/** @brief setparameter method�� ó���Ѵ�.
	@return E_FAIL or S_OK
	@param pRequest ó���� RTSPRequest
	@param pResponse �����ؾ��� RTSPResponse
*/
HRESULT RTSPSocketHandler::doSetParameter(RTSPRequest* pRequest, RTSPResponse* pResponse)
{
	LOGV_CALL(TAG, "[START]");
	HRESULT hr;
	Header* pHeader;

	// CSeq
	hr = pRequest->getHeader(eRH_CSEQ, &pHeader);
	if(SUCCEEDED(hr))
	{
		pResponse->addHeader(pHeader);
	}

	// Content-Length
	hr = pRequest->getHeader(eRH_CONTENT_LENGTH, &pHeader);
	if(FAILED(hr))
	{
		pHeader = new Header(eRH_CONTENT_LENGTH);

		hr = pHeader->setIntValue(0);
		hr = pResponse->addHeader(pHeader);

		pResponse->setStatusCode(400);

		LOGV_CALL(TAG, "[END]");
		return S_OK;
	}

	if(pRequest->getContentLength() == 0)
	{
		hr = pHeader->setIntValue(0);
		hr = pResponse->addHeader(pHeader);

		pResponse->setStatusCode(400);

		LOGV_CALL(TAG, "[END]");
		return S_OK;
	}

	hr = parseContents(pRequest, pResponse);

	if(FAILED(hr))
	{
		hr = pHeader->setIntValue(0);
		hr = pResponse->addHeader(pHeader);

		pResponse->setStatusCode(503);
	}
	else
	{
		hr = pHeader->setIntValue(0);
		hr = pResponse->addHeader(pHeader);

		pResponse->setStatusCode(200);
	}

	LOGV_CALL(TAG, "[END]");
	return S_OK;
}

/** @brief setparameter���� ���� body message�� �Ľ��Ѵ�. slave�� token�� �����ϰ�, master�� ���������� db�� �����Ѵ�.
	@return E_FAIL or S_OK
	@param pRequest ó���� RTSPRequest
	@param pResponse �����ؾ��� RTSPResponse
*/
HRESULT RTSPSocketHandler::parseContents(RTSPRequest* pRequest, RTSPResponse* pResponse)
{
	//HRESULT hr;
	int j = 0;
	int nLength;
	char* pContent;
	char* szLine;
	char* szHeaderName;
	char* szHeaderValue;
	std::vector<string> lineVec;
	std::string separator = "\n";

	std::string token = "";
	std::map <std::string, std::string> contentsMap;

	pRequest->getContent(&pContent, &nLength);
	split(std::string(pContent), separator, lineVec);

	for(int i=0; i<(int)(lineVec.size()); i++)
	{
		szLine = (char*)lineVec[i].c_str();

		int nLength = strlen(szLine);

		for(j=0; j<nLength; j++)
		{
			if(szLine[j] == ':')
			{
				szLine[j] = '\0';
				break;
			}
		}

		if(i == nLength)
		{
			continue;
		}

		szHeaderName = szLine;
		j++;
		while(szLine[j] == ' ' || szLine[j] == '\t' || szLine[j] == '\r')
		{
			j++;
		}
		szHeaderValue = &szLine[j];

		if(std::string(szHeaderValue) != "")
		{
			contentsMap.insert(map<std::string, std::string>::value_type(std::string(szHeaderName), std::string(szHeaderValue)));
		}
	}

	return S_OK;
}

/** @brief getparameter���� ���� body message�� �Ľ��Ѵ�. body�� ���� key, value�� ������ �����Ѵ�.
	@return E_FAIL or S_OK
	@param pRequest ó���� RTSPRequest
	@param pResponse �����ؾ��� RTSPResponse
	@param bodyMap key, value�� ������ std::map
*/
HRESULT RTSPSocketHandler::parseContents(RTSPRequest* pRequest, RTSPResponse* pResponse, std::map<std::string, std::string> &bodyMap)
{
	int j = 0;
	int nLength = 0;
	char* pContent;
	char* szLine;
	char* szHeaderName;
	char* szHeaderValue;
	std::vector<string> lineVec;
	std::string separator = "\n";

	pRequest->getContent(&pContent, &nLength);
	if(nLength < 1) return E_FAIL;
	split(std::string(pContent), separator, lineVec);

	for(int i=0; i<(int)(lineVec.size()); i++)
	{
		szLine = (char*)lineVec[i].c_str();

		int nLength = strlen(szLine);

		for(j=0; j<nLength; j++)
		{
			if(szLine[j] == ':')
			{
				szLine[j] = '\0';
				break;
			}
		}

		szHeaderName = szLine;

		if(i == nLength)
		{
			if(strlen(szHeaderName) > 0)
			{
				bodyMap.insert(map<std::string, std::string>::value_type(std::string(szHeaderName), ""));
			}
			continue;
		}

		j++;
		while(szLine[j] == ' ' || szLine[j] == '\t' || szLine[j] == '\r')
		{
			j++;
		}
		szHeaderValue = &szLine[j];
		if(strlen(szHeaderName) > 0)
		{
			bodyMap.insert(map<std::string, std::string>::value_type(std::string(szHeaderName), std::string(szHeaderValue)));
		}
	}
	return S_OK;
}

/** @brief getparameter���� ���� ��û�� ���� body message�� �������ش�.
	@return body string
	@param bodyMap ��û�� body key ��
	@param session ��û�� RTSPSession
*/
const char* RTSPSocketHandler::makeBody(std::map<std::string, std::string> bodyMap, RTSPSession* session)
{
	std::string bodyStr = "";
	int mapSize = bodyMap.size();
	char tmpValue[128];


	if (mapSize > 0)
	{
		map<std::string, std::string>::iterator itr;
		itr = bodyMap.begin();
		while(itr != bodyMap.end())
		{
#ifdef SLAVE
			if((std::string)itr->first == ALTICAST_API_BODY_TABLE[eAAB_STREAM_STATE])
			{
				if(session)
				{
					sprintf(tmpValue, "%s: %s\r\n", ((std::string)itr->first).c_str(), RTSP_PLAY_STATE_TABLE[session->getPlayState()]);
					bodyStr.append(tmpValue);
				}
			}
			else if((std::string)itr->first == ALTICAST_API_BODY_TABLE[eAAB_POSITION])
			{
				if(session)
				{
					sprintf(tmpValue, "%s: %.3f\r\n", ((std::string)itr->first).c_str(), double(session->getNptTime()/90000.0));
					bodyStr.append(tmpValue);
				}
			}
			else if((std::string)itr->first == ALTICAST_API_BODY_TABLE[eAAB_CPU])
			{
				sprintf(tmpValue, "%s: %u\r\n", ((std::string)itr->first).c_str(), systemUtil->getSystemInfo(eUI_CPU));
				bodyStr.append(tmpValue);
			}
			else if((std::string)itr->first == ALTICAST_API_BODY_TABLE[eAAB_DISKUSAGE])
			{
				sprintf(tmpValue, "%s: %u\r\n", ((std::string)itr->first).c_str(), systemUtil->getSystemInfo(eUI_DISK));
				bodyStr.append(tmpValue);
			}
			else if((std::string)itr->first == ALTICAST_API_BODY_TABLE[eAAB_SESSIONS])
			{
				sprintf(tmpValue, "%s: %d\r\n", ((std::string)itr->first).c_str(), sessionManager->getSessionCount());
				bodyStr.append(tmpValue);
			}
			else if((std::string)itr->first == ALTICAST_API_BODY_TABLE[eAAB_TRAFFIC])
			{
				sprintf(tmpValue, "%s: %u\r\n", ((std::string)itr->first).c_str(), systemUtil->getSystemInfo(eUI_TRAFFIC));
				bodyStr.append(tmpValue);
			}
			else if((std::string)itr->first == ALTICAST_API_BODY_TABLE[eAAB_MAX_SESSIONS])
			{
				sprintf(tmpValue, "%s: %d\r\n", ((std::string)itr->first).c_str(), getMaxSessions());
				bodyStr.append(tmpValue);
			}
			else if((std::string)itr->first == ALTICAST_API_BODY_TABLE[eAAB_MAX_TRAFFIC])
			{
				sprintf(tmpValue, "%s: %u\r\n", ((std::string)itr->first).c_str(), getMaxTraffic());
				bodyStr.append(tmpValue);
			}
#endif

			itr++;
		}
	}
	return bodyStr.c_str();
}
