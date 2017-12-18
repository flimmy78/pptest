#ifndef __SLAVE_ACTIVATOR_H__
#define __SLAVE_ACTIVATOR_H__

#include <string>

class ConfigManager;
class RTSPServer;

void sigHandler(int signo);

typedef struct SSlaveConfig
{
	int logLevel;
	int logOutput;
	std::string logPathName;
	std::string cmsLogPathName;
	int cmsLogInterval;
	int serverPort;
	std::string alticastIp;
	int alticastPort;

	int maxSessions;
	int maxTraffic;
}SlaveConfig;

class SlaveActivator {
	
public :	
	SlaveActivator();	
	virtual ~SlaveActivator();

	void start();
	void stop();
	
private :
	ConfigManager* configManager;
	RTSPServer* rtspServer;

	SlaveConfig* slaveConfig;

private :
	void loadConfig();
	void initLog();
	void shutdownLog();
};

#endif
