
#include <string>
#include <map>
#include <pthread.h>
#include <stdio.h>

#include "StringUtil.h"
#include "RTSPSocket.h"
#include "RTSPServer.h"
#include "RTSPSocketHandler.h"
#include "ThreadSafe.h"
#include "SessionManager.h"
//#include "RTSPServerManager.h"
#include "SystemUtil.h"
#include "NetworkUtil.h"
#include "config.h"
#include <nomad_log.h>
#include "UDPReceiver.h"




using namespace std;

map < string, RTSPSocketHandler* > gRTSPSocketHandlerTable;

static const char* TAG = "RTSP";


/** @brief client socket�� ������ �޾Ƶ��̰� �޾Ƶ��� socket�� �̿��� RTSPSocketHandler�� ������.
	@return �ǹ� ����.
	@param ptr RTSPServer
*/
void *gRTSPServerThreadProc(void *ptr)
{
	RTSPServer *rtspServer = (RTSPServer*) ptr;
	while(rtspServer->getState() == _eSTART)
	{
		RTSPSocket *client = rtspServer->rtspServerSocket.acceptWait(3);
		if(client == (RTSPSocket*)NULL || !client->isValid())
		{
			continue;
		}
		RTSPSocketHandler *rsh = new RTSPSocketHandler(client, rtspServer->getSessionManager(), rtspServer->getSystemUtil());

		string clientKey;
		clientKey.append(client->remoteIP());
		clientKey.append(":");
		clientKey.append(intToString(client->remotePort()));

		LOGD(TAG, "%s(%d) %s: RTSPServer accept(%s)", __CALL_INFO__, clientKey.c_str());

		gRTSPSocketHandlerTable.insert(map < string, RTSPSocketHandler * >::value_type(clientKey, rsh));
	}

	return (void*)NULL;
}

/** @brief RTSPServer ������.
	@return �ǹ� ����.
	@param port server socket�� ��Ʈ
*/
RTSPServer::RTSPServer(const int port)
{
	setPort(port);
	setState(_eINIT);

	//threadSafe = new ThreadSafe();
	sessionManager = new SessionManager();
	systemUtil = new SystemUtil(RTSP_CONF_DIR + "/slave.conf");
	udpReceiverFHD = new UDPReceiver(SI_FHD, sessionManager);
	udpReceiverHD = new UDPReceiver(SI_HD, sessionManager);
	udpReceiverSD = new UDPReceiver(SI_SD, sessionManager);
}

/** @brief RTSPServer �Ҹ���.
	@return �ǹ� ����.
	@param ����.
*/
RTSPServer::~RTSPServer()
{
	close();
	//delete threadSafe;
	delete udpReceiverFHD;
	delete udpReceiverHD;
	//delete udpReceiverSD;
	delete sessionManager;
	delete systemUtil;
}

/** @brief server socket������ bind, listen�� �����Ѵ�.
	@return S_OK
	@param ����.
*/
int RTSPServer::open()
{
	if(getState() != _eINIT)
	{
		return E_FAIL;
	}

	LOGD(TAG, "%s(%d) %s: START.", __CALL_INFO__);

	rtspServerSocket.create();
	rtspServerSocket.bind (getPort());
	rtspServerSocket.listen();
	rtspServerSocket.setNonBlocking(true);

	setState(_eOPEN);
	LOGD(TAG, "%s(%d) %s: END.", __CALL_INFO__);
	return S_OK;
}

/** @brief server socket���� ������ �޾Ƶ��� thread�� �����ϰ� ������ ���� �Ѵ�.
	@return S_OK
	@param ����.
*/
int RTSPServer::start()
{
	if(getState() != _eOPEN)
	{
		return E_FAIL;
	}

	LOGD(TAG, "%s(%d) %s: START.", __CALL_INFO__);

	pthread_attr_t attr;
	pthread_attr_init(&attr);

	size_t stacksize = 1024*1024;
	pthread_attr_setstacksize (&attr, stacksize);

	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	if(pthread_create( &thread, &attr, gRTSPServerThreadProc, (void*) this))
	{
		LOGE(TAG, "%s(%d) %s: pthread create fail.", __CALL_INFO__);
		pthread_attr_destroy(&attr);
		return E_FAIL;
	}
	pthread_attr_destroy(&attr);
	setState(_eSTART);

	LOGD(TAG, "%s(%d) %s: END.", __CALL_INFO__);
	return S_OK;
}

/** @brief ������ �޾Ƶ��̰� �ִ� thread�� �����Ѵ�.
	@return S_OK
	@param ����.
*/
int RTSPServer::stop()
{
	if(getState() != _eSTART)
	{
		return E_FAIL;
	}

	LOGD(TAG, "%s(%d) %s: START.", __CALL_INFO__);

	setState(_eOPEN);

	pthread_join(thread, (void**)NULL);

	LOGD(TAG, "%s(%d) %s: END.", __CALL_INFO__);
	return S_OK;
}

/** @brief server socket�� �ݰ�, ���ӵǾ� �ִ� ���� socket�� ������ �����Ѵ�.
	@return S_OK
	@param ����.
*/
int RTSPServer::close()
{
	if(getState() != _eOPEN)
	{
		return E_FAIL;
	}

	LOGD(TAG, "%s(%d) %s: START.", __CALL_INFO__);

	setState(_eINIT);
	rtspServerSocket.close();

	shutDown();

	LOGD(TAG, "%s(%d) %s: END.", __CALL_INFO__);
	return S_OK;
}

/** @brief rtsp server socket�� ��Ʈ�� �����Ѵ�.
	@return ����.
	@param port ������ ��Ʈ
*/
void RTSPServer::setPort(const int port)
{
	this->port = port;
}

/** @brief rtsp server socket�� ��Ʈ�� �����´�.
	@return rtsp server socket�� ��Ʈ
	@param ����.
*/
int RTSPServer::getPort()
{
	return port;
}

/** @brief rtsp server�� ���¸� �����Ѵ�.(init, open, start...)
	@return ����.
	@param state ������ ���°�
*/
void RTSPServer::setState(const int state)
{
	this->state = state;
}

/** @brief rtsp server�� ���¸� �����´�.(init, open, start...)
	@return rtsp server�� ���°�
	@param ����.
*/
int RTSPServer::getState()
{
	return state;
}

//// ��������.
//void RTSPServer::lock()
//{
//	threadSafe->lock();
//}
//
//// ��������.
//void RTSPServer::unlock()
//{
//	threadSafe->unlock();
//}

/** @brief sessionmanager�� �����Ѵ�.(rtsp session ����)
	@return sessionmanager
	@param ����.
*/
SessionManager* RTSPServer::getSessionManager()
{
	return sessionManager;
}

/** @brief servermanager�� �����Ѵ�.(slave rtsp server ����)
	@return servermanager
	@param ����.
*/
RTSPServerManager* RTSPServer::getServerManager()
{
	return rtspServerManager;
}

/** @brief systemutil�� �����Ѵ�.(cpu, memory, disk�� ����)
	@return servermanager
	@param ����.
*/
SystemUtil* RTSPServer::getSystemUtil()
{
	return systemUtil;
}

/** @brief rtsp server ������ ���ӵǾ� �ִ� client�� ���� ���� �Ѵ�.
	@return ����.
	@param ����.
*/
void RTSPServer::shutDown()
{
	if (gRTSPSocketHandlerTable.size() > 0)
	{
		map< string, RTSPSocketHandler * >::iterator itr;
		itr = gRTSPSocketHandlerTable.begin();
		while(itr != gRTSPSocketHandlerTable.end())
		{
			((RTSPSocketHandler*)itr->second)->stop();
			itr++;
		}

		gRTSPSocketHandlerTable.clear();
	}
}
