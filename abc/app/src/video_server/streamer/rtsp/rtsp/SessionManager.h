#ifndef __CSESSIONMANAGER_H__
#define __CSESSIONMANAGER_H__

#include <string>
#include <list>
#include <pthread.h>
#include "SDPMaker.h"

void *gSessionManagerThreadProc( void * );

class RTSPSession;

class SessionManager
{
protected:
	std::list<RTSPSession*> sessionList;
	unsigned long long sessionID;
	
	// scale 0, 1, 2
	VideoInfo* videoInfo;

private:
	pthread_t thread;
	void clearSession(void);

public:
	SessionManager(void);
	~SessionManager(void);

	std::string createSessionID(void);
	RTSPSession* getSession(const char* sID);
	bool addSession(RTSPSession* pSession);
	bool deleteSession(const char* sID);
	int getSessionCount(void);

	void lock();
	void unlock();

	void sessionCheckTimeout();

	void broadcastData(uint8_t* buf, size_t length, int channel, uint64_t recvTime, bool initFrame);

	VideoInfo getVideoInfo(int id);
	void setVideoInfoSPS(const uint8_t* data, int size, int id);
	void setVideoInfoPPS(const uint8_t* data, int size, int id);

public:
	bool isStop;
};

#endif

