#ifndef __CRTSPSERVERMANAGER_H__
#define __CRTSPSERVERMANAGER_H__

#include <string>
#include <pthread.h>
#include <map>

#include "RTSPSocket.h"
#include "asset_db.h"

typedef struct SRTSPServerState
{
	int cpu; // cpu usage %
	int diskUsage; // disk usage %
	int sessions; // session count
	int traffic; // kbits / second
	int maxSessions;
	int maxTraffic;

	int activeFlag;

}RTSPServerState;

typedef struct SRTSPFileInfo
{
	std::string assetId;
	std::string rtspIp;
	int rtspPort;
	std::string filePath;
	std::string fileName;
	std::string fileDuration;
}RTSPFileInfo;

void *gRTSPServerStateCheckThreadProc( void * );
void *gRTSPServerFileInfoThreadProc( void * );

class RTSPServerManager
{
protected:
	std::map<std::string, RTSPServerState> rtspServerInfoMap;
	std::vector<RTSPFileInfo> rtspFileInfoMap;

private:
	pthread_t stateCheckThread;
	pthread_t fileInfoThread;

public:
	RTSPServerManager(void);
	~RTSPServerManager(void);
	void initRtspServerInfoMap();
	void initGetFileDuration();
	
	bool updateSystemInfo(std::string rtspIP, int useFlag, int activeFlag);
	void getInfo(std::string rtspIP, RTSPServerState& info);

	void addRtspServer(std::string assetid, std::string rtspIp, std::string filePath, std::string fileName);
	void delRtspServer(std::string assetid, std::string rtspIp, std::string filePath, std::string fileName);
	void getRTSPFileInfo();
	
	void getRTSPServerInfo();
	bool getRTSPServerState(std::string ip, int port, RTSPServerState& value);
	//void parseResponseCode(RTSPSocket client, const char* pLine, int& nResponseCode, RTSPServerState& value);
	void parseResponseCode(RTSPSocket* client, const char* pLine, int& nResponseCode, std::string& responseBody);
	std::string recvContent(RTSPSocket* client, int contentLength);

	bool getFileDuration(SRTSPFileInfo& value);
	int findSlaveServer(std::vector<AssetInfo> slaveList);

public:
	bool isStop;
};

#endif

