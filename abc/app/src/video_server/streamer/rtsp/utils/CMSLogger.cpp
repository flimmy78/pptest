#include <unistd.h>
#include <sys/syscall.h>
#include <sys/time.h>

#include "CMSLogger.h"
//#include "nomad_log.h"
#include <time.h>
#include "ThreadSafe.h"
#include <sys/stat.h>
#include <stdarg.h>
#include <gw.h>

// -----------------------------------------------------------------------------
EXTERN_C_BEGIN

/** @brief cmslogger를 가져온다.
	@return CMSLogger
	@param 없음.
*/
CMSLogger& getCMSLogger()
{
	static CMSLogger cmsLogger;
	return cmsLogger;
}

/** @brief cmslogger를 초기화한다.
	@return 의미 없음.
	@param szPathName cms 로그 파일을 만들 위치
	@param szPathName cms 로그 파일을 나눌 시간 단위
*/
void cmsInitializeLog(const char* szPathName, int interval)
{
	CMSLogger& logger = getCMSLogger();
	logger.open(szPathName, interval);
}

/** @brief cmslogger를 종료한다.
	@return 의미 없음.
	@param 없음.
*/
void cmsFinalizeLog()
{
	CMSLogger& logger = getCMSLogger();
	logger.close();
}

EXTERN_C_END
// --------------------------------------------------------------------------------

/** @brief CMSLogger 생성자.
	@return 의미 없음.
	@param 없음.
*/
CMSLogger::CMSLogger(void) :
fp(NULL),
bufSize(0),
devideInterval(1),
message(NULL),
log(NULL),
threadSafe(new ThreadSafe())
{
}

/** @brief CMSLogger 소멸자.
	@return 의미 없음.
	@param 없음.
*/
CMSLogger::~CMSLogger(void)
{
	close();

	if(threadSafe) delete threadSafe;
}

/** @brief pthread mutex lock 여러개의 쓰레드에서 동시에 쓸경우 로그가 이상하게 작성될수 있으므로 사용함..
	@return 의미 없음.
	@param 없음.
*/
void CMSLogger::lock()
{
	threadSafe->lock();
}

/** @brief pthread mutex unlock 여러개의 쓰레드에서 동시에 쓸경우 로그가 이상하게 작성될수 있으므로 사용함.
	@return 의미 없음.
	@param 없음.
*/
void CMSLogger::unlock()
{
	threadSafe->unlock();
}

/** @brief CMSLogger 초기화
	@return 의미 없음.
	@param szPathName 로그 파일이 생길 위치
	@param interval 로그 파일을 나눌 단위 시간
	@param nBufSize 로그를 쓸 버퍼 크기
*/
HRESULT CMSLogger::open(const char* szPathName, int interval, int nBufSize)
{
	close();

	lock();

	message = new char [nBufSize];
	log = new char [nBufSize * 2];
	bufSize = nBufSize;
	memset(&fileLogDate, 0, sizeof(fileLogDate));

	if(60 % interval == 0)
	{
		devideInterval = interval;
	}
	else
	{
		devideInterval = 1; // default 1 min
	}

	//if( flags->getFlags(eLO_FILE) )
	{
		pathName = (szPathName == NULL) ? "" : szPathName;

		createLogFile("w");
	}
	
	unlock();

	return S_OK;
}

/** @brief CMSLogger 종료
	@return 의미 없음.
	@param 없음.
*/
HRESULT CMSLogger::close()
{
	lock();

	if(fp)
	{
		fclose(fp);
		fp = NULL;
	}

	if(message)
	{
		delete[] message;
		message = NULL;
	}

	if(log)
	{
		delete[] log;
		log = NULL;
	}

	bufSize = 0;
	pathName.clear();

	unlock();

	return S_OK;
}

/** @brief 실제 로그를 파일에 쓴다.
	@return 쓰기 완료 : S_OK, 쓰기 실패 : E_FAIL
	@param szFormat 로그를 쓸 format
	@param ... format에 맞는 실제 값들
*/
HRESULT CMSLogger::print(const char* szFormat, ...)
{
	lock();

	if(log == NULL)
	{
		unlock();
		return E_FAIL;
	}

	va_list argptr;
	va_start(argptr, szFormat);
	vsnprintf(message, bufSize, szFormat, argptr);
	va_end(argptr);

	fillLog(message);
	printToFile();
	
	unlock();
	return S_OK;
}

/** @brief 로그 파일을 나눌 단위 시간로 체크하여 단위시간이 지났으며 새 로그 파일을 만든다.
	@return 의미없음.
	@param 없음.
*/
void CMSLogger::checkDevide()
{
	lock();

	char szDate[128];
	fillCurrentDateTimeString(szDate);

	if(isItTimeToDivideLogFile())
	{
		closeLogFile();
		backupLogFile();
		createLogFile("w");
	}

	unlock();
}

/** @brief 파일이 없으면 새로 만들고, buffer에 있는 로그 내용을 파일에 기록한다.
	@return 의미없음.
	@param 없음.
*/
void CMSLogger::printToFile()
{
	if(isItTimeToDivideLogFile())
	{
		closeLogFile();
		backupLogFile();
		createLogFile("w");
	}

	if(fp)
	{
		fprintf(fp, "%s\n", log);
		fflush(fp);
	}
}

/** @brief 로그 메시지에 대해 시간정보를 추가 하여 파일에 적을 내용을 만든다.
	@return 의미없음.
	@param szMessage 로그 내용
*/
void CMSLogger::fillLog(const char* szMessage)
{
	char szDate[128];

	fillCurrentDateTimeString(szDate);

	sprintf(log, "%s:%s", szDate, szMessage);
}

/** @brief 현재 시간 정보를 가져온다.
	@return 의미없음.
	@param szBuf 가져온 시간정보를 채울 버퍼
*/
void CMSLogger::fillCurrentDateTimeString(char* szBuf)
{
	struct tm* pDate;
	const char* szDateTimeFormat = "%04d-%02d-%02d-%02d-%02d-%02d-%03d";

#ifdef WIN32

	struct tm date = { 0 };
	SYSTEMTIME lt;
	::GetLocalTime(&lt);

	sprintf(szBuf, szDateTimeFormat,
		lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds);

	date.tm_year = lt.wYear - 1900;
	date.tm_mon = lt.wMonth - 1;
	date.tm_mday = lt.wDay;
	date.tm_hour = lt.wHour;
	date.tm_min = lt.wMinute;
	date.tm_sec = lt.wSecond;
	pDate = &date;

#else

	time_t curTime;
	int nMillisecond;

	struct timeval tvTime;
	gettimeofday(&tvTime, NULL);
	curTime = tvTime.tv_sec;
	nMillisecond = tvTime.tv_usec / 1000;

	pDate = localtime(&curTime);
	sprintf(szBuf, szDateTimeFormat,
		pDate->tm_year+1900, pDate->tm_mon+1, pDate->tm_mday, pDate->tm_hour, pDate->tm_min, pDate->tm_sec, nMillisecond);

#endif

	memcpy(&fileLogDate, pDate, sizeof(struct tm));
}

/** @brief 로그를 작성할 파일을 만든다.
	@return 성공 : 0 실패 : 1
	@param szMode 로그 파일을 open할 모드
*/
int CMSLogger::createLogFile(const char* szMode)
{
	if(pathName.empty())
	{
		return -1;
	}

	FILE* fd = fopen(pathName.c_str(), szMode);

	if(fd == NULL)
	{
		return -1;
	}

	this->fp = fd;

	struct tm* pDate;
	time_t curTime = time(NULL);
	pDate = localtime(&curTime);
	memcpy(&fileCreationDate, pDate, sizeof(struct tm));

	return 0;
}

/** @brief 로그 파일을 close 한다.
	@return 의미 없음.
	@param 없음.
*/
void CMSLogger::closeLogFile()
{
	if(fp)
	{
		fclose(fp);
		fp = NULL;
	}
}

/** @brief 현재 작성하고 있는 로그파일을 현재 날짜의 새로운 이름으로 저장하고 새로운 로그 파일을 만든다.
	@return 의미없음.
	@param 없음.
*/
void CMSLogger::backupLogFile()
{
	char szPathName[512];
	::gw_snprintf(szPathName, sizeof(szPathName), "%s.%04d-%02d-%02d-%02d-%02d",
		pathName.c_str(),
		fileCreationDate.tm_year+1900, fileCreationDate.tm_mon+1, fileCreationDate.tm_mday, fileCreationDate.tm_hour, fileCreationDate.tm_min);

	::gw_unlink(szPathName);
	rename(pathName.c_str(), szPathName);
}

//bool CMSLogger::isItTimeToDivideLogFile()
//{
//	if(fileLogDate.tm_mday != 0)
//	{
//		if(fileCreationDate.tm_min != fileLogDate.tm_min && fileLogDate.tm_min % devideInterval == 0)
//		{
//			return true;
//		}
//		else
//		{
//			return false;
//		}
//	}
//	return false;
//}

/** @brief 시간을 확인하여 로그 파일을 단위 시간으로 나누어야 하는지 확인한다.
	@return true : 로그 파일을 나눈다. false : 아직 나누지 않는다.
	@param 없음.
*/
bool CMSLogger::isItTimeToDivideLogFile()
{
	return (fileLogDate.tm_mday != 0 && fileCreationDate.tm_mday != fileLogDate.tm_mday);
}
