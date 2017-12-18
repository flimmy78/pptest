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

/** @brief MasterActivator ������. master config ���� �ε� config ����. rtsp ���� ����.
	@return �ǹ� ����.
	@param ����.
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

/** @brief MasterActivator �Ҹ���.
	@return �ǹ� ����.
	@param ����.
*/
MasterActivator::~MasterActivator()
{
	shutdownLog();
	
	delete masterConfig;
	delete configManager;
	delete rtspServer;
}

/** @brief master.conf ���Ͽ��� ���� ������ �о�� �����Ѵ�.
	@return �ǹ� ����.
	@param ����.
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

/** @brief master.conf ���Ͽ��� �о�� �α� ���� ������ �����ϰ� �α׸� �ʱ�ȭ �Ѵ�.
	@return �ǹ� ����.
	@param ����.
*/
void MasterActivator::initLog()
{
	nomadInitializeLog(masterConfig->logPathName.c_str(), masterConfig->logOutput);
	nomadSetLogLevel(masterConfig->logLevel);

	LOGD(TAG, "%s(%d) %s: log: level(%d) output(%d) file(%s)", __CALL_INFO__, masterConfig->logLevel, masterConfig->logOutput, masterConfig->logPathName.c_str());
}

/** @brief �α׸� �ߴ� �Ѵ�.
	@return �ǹ� ����.
	@param ����.
*/
void MasterActivator::shutdownLog()
{
	nomadFinalizeLog();
}

/** @brief master rtsp ������ �����Ѵ�.
	@return �ǹ� ����.
	@param ����.
*/
void MasterActivator::start()
{
	rtspServer->open();
	rtspServer->start();
}

/** @brief master rtsp ������ �����Ѵ�.
	@return �ǹ� ����.
	@param ����.
*/
void MasterActivator::stop()
{
	rtspServer->stop();
	rtspServer->close();	
}

/** @brief ���α׷��� ������.
	@return �ǹ� ����.
	@param �ǹ̾���.
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


