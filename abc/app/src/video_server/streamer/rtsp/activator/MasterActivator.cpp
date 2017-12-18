#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include <nomad_log.h>

#include "MasterActivator.h"
#include "RTSPServer.h"
#include "ConfigManager.h"
#include "Logger.h"
#include "config.h"

namespace
{
	const char* TAG = "MASTER_ACTIVATOR";
}

static int _run = 0;

void sigHandler(int signo)
{
	_run = 0;
	LOGD(TAG, "%s(%d) %s: Receive signal. signo(%d)", __CALL_INFO__, signo);
}

/** @brief MasterActivator 생성자. master config 파일 로드 config 설정. rtsp 서버 구동.
	@return 의미 없음.
	@param 없음.
*/
MasterActivator::MasterActivator()
{
	_run = 1;

	masterConfig = new MasterConfig;
	configManager = new ConfigManager();
	loadConfig();
	initLog();
	
	rtspServer = new RTSPServer(masterConfig->serverPort, masterConfig->alticastIp, masterConfig->alticastPort);
}

/** @brief MasterActivator 소멸자.
	@return 의미 없음.
	@param 없음.
*/
MasterActivator::~MasterActivator()
{
	shutdownLog();
	
	delete masterConfig;
	delete configManager;
	delete rtspServer;
}

/** @brief master.conf 파일에서 각종 설정을 읽어와 저장한다.
	@return 의미 없음.
	@param 없음.
*/
void MasterActivator::loadConfig()
{
	std::string confPath = RTSP_CONF_DIR + "/master.conf";
	HRESULT hr = configManager->load(confPath.c_str());

	if(SUCCEEDED(hr))
	{
		const char* szValue;

		szValue = configManager->getValue("LOG_LEVEL");
		if(szValue)
		{
			masterConfig->logLevel = atoi(szValue);
		}

		szValue = configManager->getValue("LOG_OUTPUT");
		if(szValue)
		{
			masterConfig->logOutput = atoi(szValue);
		}

		szValue = configManager->getValue("LOG_FILE");
		if(szValue)
		{
			masterConfig->logPathName = szValue;
		}

		szValue = configManager->getValue("SERVER_PORT");
		if(szValue)
		{
			masterConfig->serverPort = atoi(szValue);
		}

		szValue = configManager->getValue("TOKEN_USE");
		if(szValue)
		{
			masterConfig->tokenUse = atoi(szValue);
			setTokenUse(masterConfig->tokenUse);
		}

		szValue = configManager->getValue("ALTICAST_IP");
		if(szValue)
		{
			masterConfig->alticastIp = szValue;
		}

		szValue = configManager->getValue("ALTICAST_PORT");
		if(szValue)
		{
			masterConfig->alticastPort = atoi(szValue);
		}
	}
}

/** @brief master.conf 파일에서 읽어온 로그 관련 설정을 적용하고 로그를 초기화 한다.
	@return 의미 없음.
	@param 없음.
*/
void MasterActivator::initLog()
{
	nomadInitializeLog(masterConfig->logPathName.c_str(), masterConfig->logOutput);
	nomadSetLogLevel(masterConfig->logLevel);

	LOGD(TAG, "%s(%d) %s: log: level(%d) output(%d) file(%s)", __CALL_INFO__, masterConfig->logLevel, masterConfig->logOutput, masterConfig->logPathName.c_str());
}

/** @brief 로그를 중단 한다.
	@return 의미 없음.
	@param 없음.
*/
void MasterActivator::shutdownLog()
{
	nomadFinalizeLog();
}

/** @brief master rtsp 서버를 시작한다.
	@return 의미 없음.
	@param 없음.
*/
void MasterActivator::start()
{
	rtspServer->open();
	rtspServer->start();
}

/** @brief master rtsp 서버를 종료한다.
	@return 의미 없음.
	@param 없음.
*/
void MasterActivator::stop()
{
	rtspServer->stop();
	rtspServer->close();	
}

/** @brief 프로그램의 시작점.
	@return 의미 없음.
	@param 의미없음.
*/
int main(int argc, const char** argv)
{
	signal(SIGINT, sigHandler);

	MasterActivator activator;

	activator.start();

	while(_run)
	{
		sleep(1);
	}
	
	activator.stop();
	return 0;
}


