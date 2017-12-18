#ifndef __MASTER_ACTIVATOR_H__
#define __MASTER_ACTIVATOR_H__

#include <string>

class ConfigManager;
class RTSPServer;

void sigHandler(int signo);

typedef struct SMasterConfig
{
	int logLevel;
	int logOutput;
	std::string logPathName;
	int serverPort;
	int tokenUse;
	std::string alticastIp;
	int alticastPort;
}MasterConfig;

class MasterActivator {
	
public :	
	MasterActivator();	
	virtual ~MasterActivator();

	void start();
	void stop();
	
private :
	ConfigManager* configManager;
	RTSPServer* rtspServer;

	MasterConfig* masterConfig;

private :
	void loadConfig();
	void initLog();
	void shutdownLog();
};

#endif
