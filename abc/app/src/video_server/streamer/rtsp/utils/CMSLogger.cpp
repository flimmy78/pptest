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

/** @brief cmslogger�� �����´�.
	@return CMSLogger
	@param ����.
*/
CMSLogger& getCMSLogger()
{
	static CMSLogger cmsLogger;
	return cmsLogger;
}

/** @brief cmslogger�� �ʱ�ȭ�Ѵ�.
	@return �ǹ� ����.
	@param szPathName cms �α� ������ ���� ��ġ
	@param szPathName cms �α� ������ ���� �ð� ����
*/
void cmsInitializeLog(const char* szPathName, int interval)
{
	CMSLogger& logger = getCMSLogger();
	logger.open(szPathName, interval);
}

/** @brief cmslogger�� �����Ѵ�.
	@return �ǹ� ����.
	@param ����.
*/
void cmsFinalizeLog()
{
	CMSLogger& logger = getCMSLogger();
	logger.close();
}

EXTERN_C_END
// --------------------------------------------------------------------------------

/** @brief CMSLogger ������.
	@return �ǹ� ����.
	@param ����.
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

/** @brief CMSLogger �Ҹ���.
	@return �ǹ� ����.
	@param ����.
*/
CMSLogger::~CMSLogger(void)
{
	close();

	if(threadSafe) delete threadSafe;
}

/** @brief pthread mutex lock �������� �����忡�� ���ÿ� ����� �αװ� �̻��ϰ� �ۼ��ɼ� �����Ƿ� �����..
	@return �ǹ� ����.
	@param ����.
*/
void CMSLogger::lock()
{
	threadSafe->lock();
}

/** @brief pthread mutex unlock �������� �����忡�� ���ÿ� ����� �αװ� �̻��ϰ� �ۼ��ɼ� �����Ƿ� �����.
	@return �ǹ� ����.
	@param ����.
*/
void CMSLogger::unlock()
{
	threadSafe->unlock();
}

/** @brief CMSLogger �ʱ�ȭ
	@return �ǹ� ����.
	@param szPathName �α� ������ ���� ��ġ
	@param interval �α� ������ ���� ���� �ð�
	@param nBufSize �α׸� �� ���� ũ��
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

/** @brief CMSLogger ����
	@return �ǹ� ����.
	@param ����.
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

/** @brief ���� �α׸� ���Ͽ� ����.
	@return ���� �Ϸ� : S_OK, ���� ���� : E_FAIL
	@param szFormat �α׸� �� format
	@param ... format�� �´� ���� ����
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

/** @brief �α� ������ ���� ���� �ð��� üũ�Ͽ� �����ð��� �������� �� �α� ������ �����.
	@return �ǹ̾���.
	@param ����.
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

/** @brief ������ ������ ���� �����, buffer�� �ִ� �α� ������ ���Ͽ� ����Ѵ�.
	@return �ǹ̾���.
	@param ����.
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

/** @brief �α� �޽����� ���� �ð������� �߰� �Ͽ� ���Ͽ� ���� ������ �����.
	@return �ǹ̾���.
	@param szMessage �α� ����
*/
void CMSLogger::fillLog(const char* szMessage)
{
	char szDate[128];

	fillCurrentDateTimeString(szDate);

	sprintf(log, "%s:%s", szDate, szMessage);
}

/** @brief ���� �ð� ������ �����´�.
	@return �ǹ̾���.
	@param szBuf ������ �ð������� ä�� ����
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

/** @brief �α׸� �ۼ��� ������ �����.
	@return ���� : 0 ���� : 1
	@param szMode �α� ������ open�� ���
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

/** @brief �α� ������ close �Ѵ�.
	@return �ǹ� ����.
	@param ����.
*/
void CMSLogger::closeLogFile()
{
	if(fp)
	{
		fclose(fp);
		fp = NULL;
	}
}

/** @brief ���� �ۼ��ϰ� �ִ� �α������� ���� ��¥�� ���ο� �̸����� �����ϰ� ���ο� �α� ������ �����.
	@return �ǹ̾���.
	@param ����.
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

/** @brief �ð��� Ȯ���Ͽ� �α� ������ ���� �ð����� ������� �ϴ��� Ȯ���Ѵ�.
	@return true : �α� ������ ������. false : ���� ������ �ʴ´�.
	@param ����.
*/
bool CMSLogger::isItTimeToDivideLogFile()
{
	return (fileLogDate.tm_mday != 0 && fileCreationDate.tm_mday != fileLogDate.tm_mday);
}
