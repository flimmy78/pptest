#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <nomad.h>
#include <string>
#include <memory>
#include <time.h>

enum LOG_LEVEL
{
	eLL_VERBOSE = 2,
	eLL_DEBUG,
	eLL_INFO,
	eLL_WARN,
	eLL_ERROR,
};

enum LOG_OUTPUT
{
	eLO_NONE			= 0x00000000,
    eLO_CONSOLE         = 0x00000001,
    eLO_FILE            = 0x00000002,
	eLO_DATE_DIVISION	= 0x00000004,
	eLO_ALL				= 0xFFFFFFFF,
};

class Flags;
class ThreadSafe;

class Logger
{
private:
	FILE* fp;
	int minLogLevel;
	std::string pathName;
	std::string lockPathName;
	int bufSize;
	char* message;
	char* log;
	std::auto_ptr<Flags> flags;
	std::auto_ptr<ThreadSafe> threadSafe;

	struct tm fileLogDate;
	struct tm fileCreationDate;

private:
	void lock();
	void unlock();
	void fillLog(const int nLevel, const char* szTAG, const char* szMessage);
	void fillCurrentDateTimeString(char* szBuf);
	unsigned int getCurrentThreadID() const;

	void printToConsole(const int nLevel, const char* szTAG);
	void printToFile();

	bool lockExists() const;
	void createLockFile();
	void deleteLockFile();

	int createLogFile(const char* szMode);
	void closeLogFile();
	void backupLogFile();
	bool isItTimeToDivideLogFile();

public:
	Logger(void);
	~Logger(void);

	int getMinLogLevel() const { return minLogLevel; };
	void setMinLogLevel(const int nLevel) { minLogLevel = nLevel; };

	HRESULT open(const char* szPathName, int nBufSize = 8192);
	HRESULT close();
	HRESULT print(int nLevel, const char* szTAG, const char* szFormat, ...);
	void setFlags(unsigned int nFlags);
};

EXTERN_C_BEGIN

	Logger& getLogger();
	void nomadInitializeLog(const char* szPathName, const int nFlags);
	void nomadFinalizeLog();
	int nomadGetLogLevel();
	void nomadSetLogLevel(const int nLevel);

EXTERN_C_END

#endif //__LOGGER_H__
