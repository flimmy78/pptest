#ifndef __CMSLOGGER_H__
#define __CMSLOGGER_H__

#include <nomad.h>
#include <string>
#include <memory>
#include <time.h>

class ThreadSafe;

class CMSLogger
{
private:
	FILE* fp;
	std::string pathName;
	int bufSize;
	int devideInterval;
	char* message;
	char* log;
	ThreadSafe* threadSafe;

	struct tm fileLogDate;
	struct tm fileCreationDate;

private:
	void lock();
	void unlock();
	void fillLog(const char* szMessage);
	void fillCurrentDateTimeString(char* szBuf);

	void printToFile();

	int createLogFile(const char* szMode);
	void closeLogFile();
	void backupLogFile();
	bool isItTimeToDivideLogFile();

public:
	CMSLogger(void);
	~CMSLogger(void);

	HRESULT open(const char* szPathName, int interval = 1, int nBufSize = 8192);
	HRESULT close();
	HRESULT print(const char* szFormat, ...);
	void checkDevide();
};

EXTERN_C_BEGIN

	CMSLogger& getCMSLogger();
	void cmsInitializeLog(const char* szPathName, int interval = 1);
	void cmsFinalizeLog();

EXTERN_C_END

#endif //__LOGGER_H__
