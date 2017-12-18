#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include <nomad_log.h>

#include "SlaveActivator.h"
#include "RTSPServer.h"
#include "ConfigManager.h"
#include "Logger.h"
#include "config.h"

//Google breakpad
//#include "client/linux/handler/exception_handler.h"

//Google breakpad
int send_issue_report( const char* msg ){

    char command[500];
    snprintf(command, 500, "send_report '%s' ", msg);
    printf("%s", command);
    return system(command);
}

//static bool dumpCallback(
//        const google_breakpad::MinidumpDescriptor& descriptor,
//        void* context,
//        bool succeeded){
    
//  printf("Dump path: %s\n", descriptor.path());
//  send_issue_report("Crashed in RTSP server process.");
//  return succeeded;
//}


namespace
{
	const char* TAG = "SLAVE_ACTIVATOR";
}

static int _run = 0;


void sigHandler(int signo)
{
	_run = 0;
	LOGD(TAG, "%s(%d) %s: Receive signal. signo(%d)", __CALL_INFO__, signo);
}

/** @brief SlaveActivator 생성자. slave config 파일 로드 config 설정. rtsp 서버 구동.
	@return 의미 없음.
	@param 없음.
*/
SlaveActivator::SlaveActivator()
{
	_run = 1;

	slaveConfig = new SlaveConfig;
	configManager = new ConfigManager();
	loadConfig();
	initLog();
	
	rtspServer = new RTSPServer(slaveConfig->serverPort);
}

/** @brief SlaveActivator 소멸자.
	@return 의미 없음.
	@param 없음.
*/
SlaveActivator::~SlaveActivator()
{	
	delete slaveConfig;
	delete configManager;
	delete rtspServer;
	
	shutdownLog();
}

/** @brief slave.conf 파일에서 각종 설정을 읽어와 저장한다.
	@return 의미 없음.
	@param 없음.
*/
void SlaveActivator::loadConfig()
{
	std::string confPath = RTSP_CONF_DIR + "/slave.conf";
	HRESULT hr = configManager->load(confPath.c_str());

	if(SUCCEEDED(hr))
	{
		const char* szValue;

		szValue = configManager->getValue("LOG_LEVEL");
		if(szValue)
		{
			slaveConfig->logLevel = atoi(szValue);
		}

		szValue = configManager->getValue("LOG_OUTPUT");
		if(szValue)
		{
			slaveConfig->logOutput = atoi(szValue);
		}

		szValue = configManager->getValue("LOG_FILE");
		if(szValue)
		{
			slaveConfig->logPathName = szValue;
		}

		szValue = configManager->getValue("CMS_LOG_FILE");
		if(szValue)
		{
			slaveConfig->cmsLogPathName = szValue;
		}

		szValue = configManager->getValue("CMS_LOG_INTERVAL");
		if(szValue)
		{
			slaveConfig->cmsLogInterval = atoi(szValue);
		}

		szValue = configManager->getValue("SERVER_PORT");
		if(szValue)
		{
			slaveConfig->serverPort = atoi(szValue);
		}

		szValue = configManager->getValue("ALTICAST_IP");
		if(szValue)
		{
			slaveConfig->alticastIp = szValue;
		}

		szValue = configManager->getValue("ALTICAST_PORT");
		if(szValue)
		{
			slaveConfig->alticastPort = atoi(szValue);
		}

		szValue = configManager->getValue("MAX_TRAFFIC");
		if(szValue)
		{
			slaveConfig->maxTraffic = atoi(szValue);
			setMaxTraffic(slaveConfig->maxTraffic);
		}

		szValue = configManager->getValue("MAX_SESSIONS");
		if(szValue)
		{
			slaveConfig->maxSessions = atoi(szValue);
			setMaxSessions(slaveConfig->maxSessions);
		}
	}
}

/** @brief slave.conf 파일에서 읽어온 로그 관련 설정을 적용하고 로그를 초기화 한다.
	@return 의미 없음.
	@param 없음.
*/
void SlaveActivator::initLog()
{
	nomadInitializeLog(slaveConfig->logPathName.c_str(), slaveConfig->logOutput);
	nomadSetLogLevel(slaveConfig->logLevel);
	
	cmsInitializeLog(slaveConfig->cmsLogPathName.c_str());

	LOGD(TAG, "%s(%d) %s: log: level(%d) output(%d) file(%s) cmsfile(%s) cmsinterval(%d)", 
		__CALL_INFO__, slaveConfig->logLevel, slaveConfig->logOutput, slaveConfig->logPathName.c_str(), slaveConfig->cmsLogPathName.c_str(), slaveConfig->cmsLogInterval);
}

/** @brief 로그를 중단 한다.
	@return 의미 없음.
	@param 없음.
*/
void SlaveActivator::shutdownLog()
{
	nomadFinalizeLog();
	cmsFinalizeLog();
}

/** @brief slave rtsp 서버를 시작한다.
	@return 의미 없음.
	@param 없음.
*/
void SlaveActivator::start()
{
	rtspServer->open();
	rtspServer->start();
}

/** @brief slave rtsp 서버를 종료한다.
	@return 의미 없음.
	@param 없음.
*/
void SlaveActivator::stop()
{
	rtspServer->stop();
	rtspServer->close();	
}

/** @brief 프로그램의 시작점.
	@return 의미 없음.
	@param 의미 없음.
*/
int main(int argc, const char** argv)
{
	//Google breakpad
	//google_breakpad::MinidumpDescriptor descriptor("/tmp");
	//google_breakpad::ExceptionHandler eh(descriptor,
	//                               NULL,
	//                               dumpCallback,
	//                               NULL,
	//                               true,
	//                               -1);

	signal(SIGINT, sigHandler);

	SlaveActivator activator;

	activator.start();

	while(_run)
	{
		sleep(1);
	}
	
	activator.stop();
	return 0;
}


