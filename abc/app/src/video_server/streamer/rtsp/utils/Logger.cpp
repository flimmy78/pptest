#include "Logger.h"
#include "nomad_log.h"
#include <time.h>
#include "ThreadSafe.h"
#include <sys/stat.h>
#include "Flags.h"
#include <stdarg.h>
#include <gw.h>

#if !defined(WIN32)

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/time.h>

#endif

#if defined(ANDROID)

#include <android/log.h>
#define LOGD_LINE2(FILTER) \
	__android_log_print(ANDROID_LOG_DEBUG, FILTER, "%s(%d) %s", __FILE__, __LINE__, __FUNCTION__);

#endif

// -----------------------------------------------------------------------------
EXTERN_C_BEGIN

/** @brief logger를 가져온다.
	@return Logger
	@param 없음.
*/
Logger& getLogger()
{
	static Logger g_logger;
	return g_logger;
}

/** @brief logger를 초기화한다.
	@return 의미없음.
	@param szPathName 로그 파일을 만들 위치
	@param nFlags 하위 1bit : console , 다음 bit : file, 다음 bit 파일을 하루 단위로 나눌지 여부(예:7, console, file에 쓰고 파일을 하루 단위로 나눈다.)
*/
void nomadInitializeLog(const char* szPathName, const int nFlags)
{
	Logger& logger = getLogger();
	logger.setFlags(nFlags);
	logger.open(szPathName);
}

void nomadFinalizeLog()
{
	Logger& logger = getLogger();
	logger.close();
}

int nomadGetLogLevel()
{
	return getLogger().getMinLogLevel();
}

void nomadSetLogLevel(const int nLevel)
{
	return getLogger().setMinLogLevel(nLevel);
}

EXTERN_C_END
// --------------------------------------------------------------------------------

static const char* SZ_LOG_LEVEL[] =
{
	"",
	"",
	"VERBOSE",
	"DEBUG",
	"INFO",
	"WARN",
	"ERROR"
};

Logger::Logger(void) :
fp(NULL),
minLogLevel(eLL_VERBOSE),
bufSize(0),
message(NULL),
log(NULL),
flags( new Flags(eLO_CONSOLE | eLO_FILE) ),
threadSafe( new ThreadSafe() )
{
}

Logger::~Logger(void)
{
	close();
}

void Logger::lock()
{
	threadSafe->lock();
}

void Logger::unlock()
{
	threadSafe->unlock();
}

HRESULT Logger::open(const char* szPathName, int nBufSize)
{
	close();

	AutoLock lock(threadSafe.get());

	message = new char [nBufSize];
	log = new char [nBufSize * 2];
	bufSize = nBufSize;
	memset(&fileLogDate, 0, sizeof(fileLogDate));

	if( flags->getFlags(eLO_FILE) )
	{
		pathName = (szPathName == NULL) ? "" : szPathName;

		const char* szMode[] = { "a+", "a+" };
		const int nMode = lockExists() ? 1 : 0;

		int nRet = createLogFile(szMode[nMode]);

		if(nRet == 0)
		{
			lockPathName = pathName + std::string(".lock");
			createLockFile();
		}
	}

	return S_OK;
}

HRESULT Logger::close()
{
	AutoLock lock(threadSafe.get());

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

	deleteLockFile();

	bufSize = 0;
	pathName.clear();
	lockPathName.clear();

	return S_OK;
}

HRESULT Logger::print(int nLevel, const char* szTAG, const char* szFormat, ...)
{
	AutoLock lock(threadSafe.get());

	if(log == NULL)
	{
		return E_FAIL;
	}

	if( minLogLevel > nLevel)
	{
		return S_FALSE;
	}

	va_list argptr;
	va_start(argptr, szFormat);
	vsnprintf(message, bufSize, szFormat, argptr);
	va_end(argptr);

	fillLog(nLevel, szTAG, message);
	printToConsole(nLevel, szTAG);
	printToFile();

	return S_OK;
}

void Logger::printToConsole(const int nLevel, const char* szTAG)
{
	if( !flags->getFlags(eLO_CONSOLE) )
	{
		return;
	}

#if defined(ANDROID)

	__android_log_write(nLevel, szTAG, message);

#else

	printf("%s\n", log);

#endif
}

void Logger::printToFile()
{
	if( !flags->getFlags(eLO_FILE) )
	{
		return;
	}

	if( flags->getFlags(eLO_DATE_DIVISION) && isItTimeToDivideLogFile() )
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

unsigned int Logger::getCurrentThreadID() const
{
	unsigned int nThreadID;

#if defined(WIN32)

	nThreadID = GetCurrentThreadId();

#elif defined(OSX) || defined(LINUX)

	nThreadID = syscall(SYS_gettid);

#else
	// ANDROID
	nThreadID = gettid();

#endif

	return nThreadID;
}

void Logger::fillLog(const int nLevel, const char* szTAG, const char* szMessage)
{
	char szDate[128];
	unsigned int nThreadID;

	nThreadID = getCurrentThreadID();

	fillCurrentDateTimeString(szDate);

	sprintf(log, "%s: %s/%s(%u): %s",
		szDate, SZ_LOG_LEVEL[nLevel], szTAG, nThreadID, szMessage);
}

void Logger::fillCurrentDateTimeString(char* szBuf)
{
	struct tm* pDate;
	const char* szDateTimeFormat = "%02d-%02d %02d:%02d:%02d.%03d";

#ifdef WIN32

	struct tm date = { 0 };
	SYSTEMTIME lt;
	::GetLocalTime(&lt);

	sprintf(szBuf, szDateTimeFormat,
		lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds);

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
		pDate->tm_mon+1, pDate->tm_mday, pDate->tm_hour, pDate->tm_min, pDate->tm_sec, nMillisecond);

#endif

	const unsigned int nMask = eLO_FILE | eLO_DATE_DIVISION;
	if(flags->getFlags(nMask) == nMask)
	{
		memcpy(&fileLogDate, pDate, sizeof(struct tm));
	}
}

void Logger::setFlags(unsigned int nFlags)
{
	flags->setFlags(nFlags);
}

bool Logger::lockExists() const
{
	if(pathName.empty())
	{
		return false;
	}

	std::string strLockPathName = pathName + std::string(".lock");

	FILE* fp = fopen(strLockPathName.c_str(), "r");

	bool bExist = false;

	if(fp)
	{
		bExist = true;
		fclose(fp);
	}

	return bExist;
}

int Logger::createLogFile(const char* szMode)
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

void Logger::closeLogFile()
{
	if(fp)
	{
		fclose(fp);
		fp = NULL;
	}
}

void Logger::backupLogFile()
{
	char szPathName[512];
	::gw_snprintf(szPathName, sizeof(szPathName), "%s.%04d-%02d-%02d",
		pathName.c_str(),
		fileCreationDate.tm_year+1900, fileCreationDate.tm_mon+1, fileCreationDate.tm_mday);

	::gw_unlink(szPathName);
	rename(pathName.c_str(), szPathName);
}

void Logger::createLockFile()
{
	FILE* fp = fopen(lockPathName.c_str(), "w");

	if(fp)
	{
		fclose(fp);
	}
}

void Logger::deleteLockFile()
{
	if( lockPathName.empty() )
	{
		return;
	}

	::gw_unlink(lockPathName.c_str());
}

bool Logger::isItTimeToDivideLogFile()
{
	return (fileLogDate.tm_mday != 0 && fileCreationDate.tm_mday != fileLogDate.tm_mday);
}
