#ifndef __RTSPSERVER_H__
#define __RTSPSERVER_H__

#include <string>
#include <pthread.h>

#include "RTSPSocket.h"

enum RTSP_SERVER_STATE
{
	_eINIT,
	_eOPEN,
	_eSTART
};

void *gRTSPServerThreadProc( void * );

class SessionManager;
//class ThreadSafe;
class RTSPServerManager;
class SystemUtil;
class UDPReceiver;
	
class RTSPServer
{	
public:
	RTSPSocket rtspServerSocket;

	RTSPServer(int port);
	virtual ~RTSPServer();

	int open();
	int start();
	int stop();
	int close();
	void shutDown();

	void setState(const int state);
	int getState();

	//void lock();
	//void unlock();

	SessionManager* getSessionManager();
	RTSPServerManager* getServerManager();
	SystemUtil* getSystemUtil();

private:
	int port;
	int state;

	pthread_t thread;

	//ThreadSafe* threadSafe;
	SessionManager* sessionManager;
	RTSPServerManager* rtspServerManager;
	SystemUtil* systemUtil;
	UDPReceiver* udpReceiverFHD;
	UDPReceiver* udpReceiverHD;
	UDPReceiver* udpReceiverSD;
	
	void setPort(const int port);
	int getPort();

#ifdef MASTER
	bool initDb();
	bool initAlticastServer();
	bool sendHttpInitMessage();
	bool parseResponseCode(const char* pLine, int& nResponseCode);
#endif
};

#endif
