#ifndef __SYSTEMUTIL_H__
#define __SYSTEMUTIL_H__

#include <nomad.h>
#include <string>
#include <memory>
#include <time.h>
#include <pthread.h>

#define CHECKINTERVAL		5

typedef struct _ProcessStat
{
	char cpu[20];
	unsigned long long user;
	unsigned long long system;
	unsigned long long nice;
	unsigned long long idle;
	unsigned long long wait;
	unsigned long long hi;
	unsigned long long si;
	unsigned long long zero;
}ProcessStat;

enum UTIL_INDEX
{
	eUI_CPU		= 0,
	eUI_MEMORY,
	eUI_DISK,
	eUI_TRAFFIC,
};

void *gCpuCheckerThreadProc( void * ptr );

class SystemUtil
{
private:
	pthread_t cpuCheckerThread;
	unsigned int cpuIdle;
	unsigned int memoryUsage;
	unsigned int diskUsage;
	unsigned int traffic;
	std::string defaultFile;
	

	ProcessStat* statOld;
	ProcessStat* statNew;

	unsigned long long trafficOld;
	unsigned long long trafficNew;

	void getCpuUsage();
	void getNetworkUsage();
	void getMemoryInfo();
	void getDiskUsage();

public:
	SystemUtil(std::string fileName);
	~SystemUtil(void);

	bool isStop;

	void parseProcStat();
	void parseNetworkStat();

	unsigned int getSystemInfo(int index);
};
#endif //__SYSTEMUTIL_H__
