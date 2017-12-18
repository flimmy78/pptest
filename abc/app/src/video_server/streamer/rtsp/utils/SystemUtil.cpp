#include <unistd.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/statvfs.h>

#include "SystemUtil.h"
//#include "nomad_log.h"
#include <time.h>
#include "ThreadSafe.h"
#include <sys/stat.h>
#include <stdarg.h>
#include <gw.h>
#include <nomad_log.h>

static const char* TAG = "SYSTEMUTIL";

/** @brief cup 사용량과 network 사용량을 체크 하기 위한 thread 함수
	@return 의미 없음.
	@param ptr
*/
void *gCpuCheckerThreadProc(void *ptr)
{
	int checkCount = 0;
	SystemUtil *systemUtil = (SystemUtil*)ptr;
	
	while(!systemUtil->isStop) 
	{
		checkCount++;
		if(checkCount > CHECKINTERVAL)
		{
			// check cpu usage
			systemUtil->parseProcStat();
			systemUtil->parseNetworkStat();
			checkCount = 0;
		}
		usleep(1000*1000);
	}

	return (void*)NULL;
}

/** @brief SystemUtil 생성자.
	@return 의미 없음.
	@param fileName disk 사용량을 체크 하기 위한 filename
*/
SystemUtil::SystemUtil(std::string fileName) :
cpuIdle(0),
memoryUsage(0),
diskUsage(0),
trafficOld(0),
trafficNew(0),
isStop(false)
{
	statOld = new ProcessStat;
	statNew = new ProcessStat;
	memset(statOld, 0x00, sizeof(ProcessStat));
	memset(statNew, 0x00, sizeof(ProcessStat));

	defaultFile = fileName;

	pthread_attr_t attr;
	pthread_attr_init(&attr);

	size_t stacksize = 1024*1024;
	pthread_attr_setstacksize (&attr, stacksize);

	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	if(pthread_create(&cpuCheckerThread, &attr, gCpuCheckerThreadProc, (void*) this))
	{
		LOGE(TAG, "%s(%d) %s: pthread create fail.", __CALL_INFO__);
	}
	pthread_attr_destroy(&attr);
}


/** @brief SystemUtil 소멸자.
	@return 의미 없음.
	@param 없음.
*/
SystemUtil::~SystemUtil(void)
{
	isStop = true;
	pthread_join(cpuCheckerThread, (void**)NULL);

	delete statOld;
	delete statNew;
}

/** @brief cpu 사용량을 알아온다.
	@return 없음.
	@param 없음.
*/
void SystemUtil::parseProcStat(void)
{
	FILE *fp;
	int ret;

	ProcessStat* statTmp = new ProcessStat;
	memset(statTmp, 0x00, sizeof(ProcessStat));
	

	fp = fopen("/proc/stat", "r");

	if(fp == NULL)
	{
		LOGE(TAG, "%s(%d) %s: read /proc/stat error(%s).", __CALL_INFO__, strerror(errno));
		return;
	}
	
	//cpu  user    system nice    idle       wait    hi    si    zero1
	//cpu  659848  73318  118599  1733042473 233153  930   3994  0

	ret = fscanf(fp, "%s %llu %llu %llu %llu %llu %llu %llu %llu",
		statTmp->cpu,
		&statTmp->user,
		&statTmp->system,
		&statTmp->nice,
		&statTmp->idle,
		&statTmp->wait,
		&statTmp->hi,
		&statTmp->si,
		&statTmp->zero);

	if(9 == ret)
	{
		memcpy(statNew, statTmp, sizeof(ProcessStat));
		getCpuUsage();		
		memcpy(statOld, statNew, sizeof(ProcessStat));
	}

	delete(statTmp);
	fclose(fp);
}

/** @brief cpu 이전사용량과 현재를 비교 하여 사용량 %와 idle %를 구한다.
	@return 없음.
	@param 없음.
*/
void SystemUtil::getCpuUsage()
{
	unsigned long long cpuTotal;

	ProcessStat* statTmp = new ProcessStat;
	memset(statTmp, 0x00, sizeof(ProcessStat));

	statTmp->user = statNew->user - statOld->user;
	statTmp->system = statNew->system - statOld->system;
	statTmp->nice = statNew->nice - statOld->nice;
	statTmp->idle = statNew->idle - statOld->idle;
	statTmp->wait = statNew->wait - statOld->wait;
	statTmp->hi = statNew->hi - statOld->hi;
	statTmp->si = statNew->si - statOld->si;
	statTmp->zero = statNew->zero - statOld->zero;

	cpuTotal = statTmp->user + statTmp->system + statTmp->nice + statTmp->idle + statTmp->wait + statTmp->hi + statTmp->si;
	
	if(cpuTotal != 0)
	{
		cpuTotal = statTmp->user + statTmp->system + statTmp->nice + statTmp->idle + statTmp->wait + statTmp->hi + statTmp->si;
		cpuIdle = ((statTmp->idle*10000)/cpuTotal)/100;
	}
	LOGV(TAG, "%s(%d) %s: cpu idle(%d).", __CALL_INFO__, cpuIdle);

	delete(statTmp);
}

void SystemUtil::getMemoryInfo()
{
	FILE *fp;
	int ret;
	unsigned long memTotal = 0, memFree = 0, memTmp = 0;
	char parseName[20];
	memset(parseName, 0x00, sizeof(parseName));

	fp = fopen("/proc/meminfo", "r");	

	if(fp == NULL)
	{
		LOGE(TAG, "%s(%d) %s: read /proc/meminfo error(%s).", __CALL_INFO__, strerror(errno));
		return;
	}

	while((ret = fscanf(fp, "%s %lu", parseName, &memTmp)) > 0)
	{
		if(ret == 2)
		{
			if(strcmp(parseName, "MemTotal:") == 0) memTotal = memTmp;
			else if(strcmp(parseName, "MemFree:") == 0) memFree = memTmp;

			if(memTotal > 0 && memFree > 0) break;
		}
	}
	memoryUsage = (memFree*100)/memTotal;
	LOGV(TAG, "%s(%d) %s: memory free(%d).", __CALL_INFO__, memoryUsage);
	
	fclose(fp);
}

/** @brief memory 사용량을 알아온다.
	@return 없음.
	@param 없음.
*/
void SystemUtil::parseNetworkStat()
{
	FILE *fp;
	int ret;
	unsigned long long trafficTotal = 0, recvTmp = 0, sendTmp = 0;	
	char parseName[30];
	memset(parseName, 0x00, sizeof(parseName));

	fp = fopen("/proc/net/dev", "r");	

	if(fp == NULL)
	{
		LOGE(TAG, "%s(%d) %s: read /proc/net/dev error(%s).", __CALL_INFO__, strerror(errno));
		return;
	}

	while((ret = fscanf(fp, "%30[^:]: %llu %*u %*u %*u %*u %*u %*u %*u %llu %*100[^\n]\n", parseName, &recvTmp, &sendTmp)) > 0)
	{
		if(ret == 3)
		{
			if(strncmp(parseName, "eth", 3) == 0)
			{
				trafficTotal = trafficTotal + recvTmp + sendTmp;
			}
		}
	}

	trafficNew = trafficTotal;
	if(trafficOld == 0) trafficOld = trafficTotal;
	getNetworkUsage();
	trafficOld = trafficTotal;
	
	fclose(fp);
}

/** @brief network 사용량을 알아온다.
	@return 없음.
	@param 없음.
*/
void SystemUtil::getNetworkUsage()
{
	unsigned long long trafficDiff = 0;

	// 5초간 traffic
	trafficDiff = trafficNew - trafficOld;
	// Kbits / second로 변환
	traffic = (trafficDiff/CHECKINTERVAL)/128;
	
	LOGV(TAG, "%s(%d) %s: traffic(%u).", __CALL_INFO__, traffic);
}

/** @brief disk 사용량을 알아온다.
	@return 없음.
	@param 없음.
*/
void SystemUtil::getDiskUsage()
{
	struct statvfs buf;
	if(!statvfs(defaultFile.c_str(), &buf))
	{
		unsigned long diskSize, freeSize;
		diskSize = buf.f_blocks;
		freeSize = buf.f_bfree;
		diskUsage = ((diskSize - freeSize)*100)/diskSize;
		LOGD(TAG, "%s(%d) %s: diskUsage(%u)", __CALL_INFO__, diskUsage);
	}
	else
	{
		LOGE(TAG, "%s(%d) %s: Couldn't get file system statistics", __CALL_INFO__);
	}
}

/** @brief 현재 system에서 사용중인 cpu, memory 등의 사용량을 return 한다.
	@return 사용량
	@param index 알아오고자 하는 타입의 index(eUI_CPU : cpu, eUI_MEMORY : memory, eUI_DISK : disk, eUI_TRAFFIC : network)
*/
unsigned int SystemUtil::getSystemInfo(int index)
{
	unsigned int retVal = 0;

	switch(index)
	{
		case eUI_CPU:
			retVal = this->cpuIdle;
			break;
		case eUI_MEMORY:
			getMemoryInfo();
			retVal = this->memoryUsage;
			break;
		case eUI_DISK:
			getDiskUsage();
			retVal = this->diskUsage;
			break;
		case eUI_TRAFFIC:
			retVal = this->traffic;
			break;
		default :
			break;
	}

	return retVal;
}
