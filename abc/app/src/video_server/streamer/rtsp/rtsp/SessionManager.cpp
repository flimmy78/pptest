#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "nomad.h"
#include "SessionManager.h"
#include "ThreadSafe.h"
#include "RTSPSession.h"

#include "StringUtil.h"
#include "nomad_log.h"

#include "TransportHeader.h"

static const char* TAG = "SESSIONMANAGER";
const int sessionMaxSize = 15;
//const int sessionMaxSize = 1;

RTSPSession* sList[sessionMaxSize] = { NULL, };

/** @brief ���� ����Ǿ� �ִ� rtsp session�� timeout�� üũ�ϰ� �����Ѵ�.
	@return �ǹ� ����.
	@param ptr SessionManager
*/
void *gSessionManagerThreadProc(void* ptr)
{
	int sessionCheckCount = 30;
	SessionManager *sessionManager = (SessionManager*)ptr;
	
	while(!sessionManager->isStop)
	{
		sessionCheckCount++;

		if(sessionCheckCount > 30)
		{
			sessionManager->sessionCheckTimeout();
			sessionCheckCount = 0;
		}
		
		usleep(1000*1000);		
	}

	return (void*)NULL;
}

/** @brief SessionManager ������. ����Ǿ��� rtsp session�� �����Ѵ�.
	@return �ǹ� ����.
	@param ����.
*/
SessionManager::SessionManager(void) :
isStop(false)
{
	videoInfo = new VideoInfo[3];
	for(int i=0; i<3; i++)
	{
		if(videoInfo[i].spsData) free(videoInfo[i].spsData);
		if(videoInfo[i].ppsData) free(videoInfo[i].ppsData);
		videoInfo[i].spsData = NULL;
		videoInfo[i].ppsData = NULL;
		
		videoInfo[i].spsSize = 0;
		videoInfo[i].ppsSize = 0;
	}

	struct timeval tv;
	gettimeofday(&tv, NULL);
	sessionID = tv.tv_sec*1000000LL + tv.tv_usec;

	pthread_attr_t attr;
	pthread_attr_init(&attr);

	size_t stacksize = 1024*1024;
	pthread_attr_setstacksize (&attr, stacksize);

	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	if(pthread_create(&thread, &attr, gSessionManagerThreadProc, (void*) this))
	{
		LOGE(TAG, "%s(%d) %s: pthread create fail.", __CALL_INFO__);
	}
	pthread_attr_destroy(&attr);
}

/** @brief SessionManager �Ҹ���. ����Ǿ��� rtsp session�� �����Ѵ�.
	@return �ǹ� ����.
	@param ����.
*/
SessionManager::~SessionManager(void)
{
	isStop = true;
	pthread_join(thread, (void**)NULL);
	clearSession();
	
	for(int i=0; i<3; i++)
	{
		videoInfo[i].spsData = NULL;
		videoInfo[i].ppsData = NULL;
		
		videoInfo[i].spsSize = 0;
		videoInfo[i].ppsSize = 0;
	}
	delete[] videoInfo;
}

/** @brief session ��û�� ������� session id�� ����� return �Ѵ�.
	@return session id
	@param ����.
*/
std::string SessionManager::createSessionID(void) // for test
{
	char hexStr[17];
	memset(hexStr, 0x00, 17);

	sessionID += 1;

	sprintf(hexStr, "%016llx", sessionID);

	return std::string(hexStr);
}

/** @brief ���� ����Ǿ��� session�� �ִ��� Ȯ���Ѵ�.
	@return RTSPSession
	@param sID ����Ǿ��� session�� �ִ��� Ȯ���� session id
*/
RTSPSession* SessionManager::getSession(const char* sID)
{
	lock();
	for(int i=0; i<sessionMaxSize; i++) {
		RTSPSession* pSession = sList[i];
		if(pSession) {
			if(strcasecmp(pSession->getID(), sID) == 0) {
				sList[i] = NULL;
				unlock();
				return pSession;
			}
		}
	}
	unlock();
	return (RTSPSession*)NULL;
}

/** @brief RTSP Session�� �߰��Ѵ�.
	@return true
	@param pSession �߰��� rtsp session
*/
bool SessionManager::addSession(RTSPSession* pSession)
{
	struct timeval curTime;	
	gettimeofday(&curTime, NULL);
	pSession->setLastRequestTime(curTime.tv_sec);

	lock();
	for(int i=0; i<sessionMaxSize; i++) {
		if(sList[i] == NULL) {
			sList[i] = pSession;
			break;
		}
	}
	unlock();
	LOGD(TAG, "%s(%d) %s: Session Count(%d)", __CALL_INFO__, getSessionCount());
	return true;
}


/** @brief RTSP Session�� �����Ѵ�.
	@return true or false(session id�� �ְų�, �������)
	@param sID ������ session id
*/
bool SessionManager::deleteSession(const char* sID)
{
	lock();
	for(int i=0; i<sessionMaxSize; i++) {
		RTSPSession* pSession = sList[i];
		if(pSession) {
			if(strcasecmp(pSession->getID(), sID) == 0) {
				sList[i] = NULL;
				delete pSession;
				unlock();
				return true;
			}
		}
	}
	lock();
	return false;
}

/** @brief ���� ����Ǿ��� session���� return �Ѵ�.
	@return ���� ����Ǿ��� session ��
	@param ����.
*/
int SessionManager::getSessionCount(void)
{
	int count = 0;
	lock();
	for(int i=0; i<sessionMaxSize; i++) {
		if(sList[i] != NULL) {
			count++;
		}
	}
	unlock();
	return count;
}

/** @brief ��� ����Ǿ��� session�� close�Ѵ�.
	@return ����.
	@param ����.
*/
void SessionManager::clearSession()
{
	lock();
	for(int i=0; i<sessionMaxSize; i++) {
		RTSPSession* pSession = sList[i];
		if(pSession) {
			sList[i] = NULL;
			delete pSession;
			LOGD(TAG, "%s(%d) %s: remove session(%d)", __CALL_INFO__, getSessionCount());
		}
	}
	unlock();
	return;
}

void SessionManager::lock()
{

}

void SessionManager::unlock()
{
	
}

/** @brief ���� ����Ǿ��� ��� session�� ���� timeout�� �˻��ϰ� timeout �ð��� ���� session�� ���ؼ��� �����Ѵ�.
	@return ����.
	@param ����.
*/
void SessionManager::sessionCheckTimeout()
{
	long nRequestTime;
	struct timeval curTime;	
	gettimeofday(&curTime, NULL);

	lock();
	for(int i=0; i<sessionMaxSize; i++) {
		RTSPSession* pSession = sList[i];
		if(pSession) {
			nRequestTime = pSession->getLastRequestTime();

			if(curTime.tv_sec - nRequestTime > pSession->getTimeout() + 30)
			{
				sList[i] = NULL;				
				delete pSession;
				LOGD(TAG, "%s(%d) %s: remove session(%d)", __CALL_INFO__, getSessionCount());
			}
			else
			{
				break;
			}
		}
	}
	unlock();
	
	return;
}

void SessionManager::broadcastData(uint8_t* buf, size_t length, int scalerId, uint64_t recvTime, bool initFrame)
{
	lock();
	int count = 0;
	for(int i=0; i<sessionMaxSize; i++) {
		RTSPSession* pSession = sList[i];
		if(pSession) {
			
		//	printf("\n pSession exist.\n");

			if(pSession->getScalerId() == scalerId) {
				if(initFrame)
				{
					pSession->setRtpInit(initFrame);
					pSession->setData(buf, length, recvTime);
				}
				else if(pSession->getRtpInit())
				{
					pSession->setData(buf, length, recvTime);
				}
			}
			
			count++;
		}
	}
	unlock();
	return;
}

VideoInfo SessionManager::getVideoInfo(int id)
{
	if(id<0 || id>2) return videoInfo[0];

	return videoInfo[id];
}


void SessionManager::setVideoInfoSPS(const uint8_t* data, int size, int id)
{
	if(id<0 || id>2) return;
	
	if(videoInfo[id].spsData)
	{
		free(videoInfo[id].spsData);
		videoInfo[id].spsSize = 0;
	}
	
	videoInfo[id].spsData = (uint8_t*)malloc(size);
	videoInfo[id].spsSize = size;
	memcpy(videoInfo[id].spsData, data, size);
}

void SessionManager::setVideoInfoPPS(const uint8_t* data, int size, int id)
{
	if(id<0 || id>2) return;
	
	if(videoInfo[id].ppsData)
	{
		free(videoInfo[id].ppsData);
		videoInfo[id].ppsSize = 0;
	}
	
	videoInfo[id].ppsData = (uint8_t*)malloc(size);
	videoInfo[id].ppsSize = size;
	memcpy(videoInfo[id].ppsData, data, size);
}
