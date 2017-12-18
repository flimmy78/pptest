#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "nomad.h"
#include "RTSPServerManager.h"

#include "StringUtil.h"
#include "nomad_log.h"
#include "config.h"
#include "rtsp.h"

static const char* TAG = "RTSPSERVERMANAGER";




/** @brief ���� �ð� �������� slave ������ ���¸� �����Ѵ�.
	@return �ǹ� ����.
	@param ptr RTSPServerManager
*/
void *gRTSPServerStateCheckThreadProc(void* ptr)
{
	int rtspServerCheckCount = DEFAULT_RTSP_CHECK_INTERVAL;
	RTSPServerManager *rtspServerManager = (RTSPServerManager*)ptr;
	
	while(!rtspServerManager->isStop)
	{
		rtspServerCheckCount++;

		if(rtspServerCheckCount > DEFAULT_RTSP_CHECK_INTERVAL)
		{
			rtspServerManager->getRTSPServerInfo();
			rtspServerCheckCount = 0;
		}
		
		usleep(1000*1000);		
	}

	return (void*)NULL;
}

/** @brief ���� �ð� �������� slave ������ index ������ ���鵵�� �Ѵ�.
	@return �ǹ� ����.
	@param ptr RTSPServerManager
*/
void *gRTSPServerFileInfoThreadProc(void* ptr)
{
	int rtspServerInfoCount = 30;
	RTSPServerManager *rtspServerManager = (RTSPServerManager*)ptr;
	
	while(!rtspServerManager->isStop)
	{
		rtspServerInfoCount++;

		if(rtspServerInfoCount > 30)
		{
			rtspServerManager->getRTSPFileInfo();
			rtspServerInfoCount = 0;
		}
		
		usleep(1000*1000);		
	}

	return (void*)NULL;
}

/** @brief RTSPServerManager ������. rtsp server�� ���¸� üũ �ϰ�, index ������ ������ ���鵵�� describe �޽����� �۽��ϴ� thread�� �����Ѵ�.
	@return �ǹ� ����.
	@param ����.
*/
RTSPServerManager::RTSPServerManager(void) :
isStop(false)
{
	rtspServerInfoMap.clear();
	rtspFileInfoMap.clear();

	initRtspServerInfoMap();
	initGetFileDuration();

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	if(pthread_create(&stateCheckThread, &attr, gRTSPServerStateCheckThreadProc, (void*) this))
	{
		LOGE(TAG, "%s(%d) %s: pthread create fail.", __CALL_INFO__);
	}

	if(pthread_create(&fileInfoThread, &attr, gRTSPServerFileInfoThreadProc, (void*) this))
	{
		LOGE(TAG, "%s(%d) %s: pthread create fail.", __CALL_INFO__);
	}

	pthread_attr_destroy(&attr);
}

/** @brief RTSPServerManager �Ҹ���
	@return �ǹ� ����.
	@param ����.
*/
RTSPServerManager::~RTSPServerManager(void)
{
	isStop = true;
	pthread_join(stateCheckThread, (void**)NULL);
	pthread_join(fileInfoThread, (void**)NULL);
	rtspServerInfoMap.clear();
	rtspFileInfoMap.clear();
}

/** @brief db���� file duration�� ����� ���� ���� ����� �����´�. thread���� file duration�� 0�̸� slave�� describe�޽����� �۽��Ͽ� index ������ ����� duration�� db�� ������ �Ѵ�.
	@return �ǹ� ����.
	@param ����.
*/
void RTSPServerManager::initGetFileDuration()
{
	std::vector<AssetInfo> vecServer = getRtspInfoDB().getAllItemFromRtspWhereInitFindDuration();
	int vecSize = vecServer.size();

	for(int i=0; i<vecSize; i++)
	{
		std::vector<std::string> vecIp;
		split(std::string(vecServer[i].slave_ip), ".", vecIp);
		if(vecIp.size() == 5)
		{
			RTSPFileInfo tmpInfo;
			tmpInfo.assetId = std::string(vecServer[i].assetId);
			tmpInfo.rtspIp = vecIp[0] + "." + vecIp[1] + "." + vecIp[2] + "." + vecIp[3];
			tmpInfo.rtspPort = atoi(vecIp[4].c_str());
			tmpInfo.filePath = std::string(vecServer[i].filePath);
			tmpInfo.fileName = std::string(vecServer[i].fileName);
			tmpInfo.fileDuration = std::string(vecServer[i].duration);

			rtspFileInfoMap.push_back(tmpInfo);
		}
	}
}

/** @brief db���� ��� slave rtsp server�� ������ �����´�. thread���� rtsp server�� ���¸� �ľ��Ҷ� ����Ѵ�.
	@return �ǹ� ����.
	@param ����.
*/
void RTSPServerManager::initRtspServerInfoMap()
{
	std::map<std::string, int> mapServer = getRtspSystemDB().getAllItemFromSystem();
	std::map<std::string, int>::iterator itr = mapServer.begin();

	while(itr != mapServer.end())
	{
		RTSPServerState value;
		memset(&value, 0x00, sizeof(RTSPServerState));
		value.activeFlag = (*itr).second;
		rtspServerInfoMap.insert(std::map<std::string, RTSPServerState>::value_type((*itr).first, value));
		itr++;
	}
}

/** @brief file duration�� �˾ƿ;� �ϴ� rtsp server�߰� �Ѵ�.
	@return �ǹ� ����.
	@param assetid rtsp server�� asset id
	@param rtspIp rtsp server�� ������
	@param filePath rtsp server�� ���� ���
	@param fileName rtsp server�� ���� �̸�
*/
void RTSPServerManager::addRtspServer(std::string assetid, std::string rtspIp, std::string filePath, std::string fileName)
{
	LOGD(TAG, "%s(%d) %s: ip(%s), filePath(%s), fileName(%s)", __CALL_INFO__, rtspIp.c_str(), filePath.c_str(), fileName.c_str());

	std::vector<std::string> vecIp;
	split(rtspIp, ".", vecIp);
	if(vecIp.size() == 5)
	{
		RTSPFileInfo tmpInfo;
		tmpInfo.assetId = assetid;
		tmpInfo.rtspIp = vecIp[0] + "." + vecIp[1] + "." + vecIp[2] + "." + vecIp[3];
		tmpInfo.rtspPort = atoi(vecIp[4].c_str());
		tmpInfo.filePath = filePath;
		tmpInfo.fileName = fileName;
		tmpInfo.fileDuration = "";

		rtspFileInfoMap.push_back(tmpInfo);
	}
}

/** @brief file duration�� �˾ƿ;� �ϴ� rtsp server���� �Ѵ�.
	@return �ǹ� ����.
	@param assetid rtsp server�� asset id
	@param rtspIp rtsp server�� ������
	@param filePath rtsp server�� ���� ���
	@param fileName rtsp server�� ���� �̸�
*/
void RTSPServerManager::delRtspServer(std::string assetid, std::string rtspIp, std::string filePath, std::string fileName)
{
	LOGD(TAG, "%s(%d) %s: ip(%s), filePath(%s), fileName(%s)", __CALL_INFO__, rtspIp.c_str(), filePath.c_str(), fileName.c_str());
	
	std::vector<std::string> vecIp;
	split(rtspIp, ".", vecIp);
	RTSPFileInfo tmpInfo;
	if(vecIp.size() == 5)
	{
		tmpInfo.assetId = assetid;
		tmpInfo.rtspIp = vecIp[0] + "." + vecIp[1] + "." + vecIp[2] + "." + vecIp[3];
		tmpInfo.rtspPort = atoi(vecIp[4].c_str());
		tmpInfo.filePath = filePath;
		tmpInfo.fileName = fileName;
	}
	else return;

	std::vector<RTSPFileInfo>::iterator itr = rtspFileInfoMap.begin();
	while(itr != rtspFileInfoMap.end())
	{
		if(tmpInfo.assetId == (*itr).assetId &&
			tmpInfo.rtspIp == (*itr).rtspIp &&
			tmpInfo.rtspPort == (*itr).rtspPort &&
			tmpInfo.filePath == (*itr).filePath &&
			tmpInfo.fileName == (*itr).fileName)
		{
			itr = rtspFileInfoMap.erase(itr);
		}
		else ++itr;
	}	
}

/** @brief �������� slave rtsp server�� use, active���¸� �����ϰ� db�� ������Ʈ �Ѵ�.
	@return �ǹ� ����.
	@param rtspIp rtsp server�� ������
	@param useFlag use ���� ����
	@param activeFlag active ���� ����
*/
bool RTSPServerManager::updateSystemInfo(std::string rtspIP, int useFlag, int activeFlag)
{
	LOGD(TAG, "%s(%d) %s: rtsp serverip(%s), useFlag(%d), activeFlag(%d)", __CALL_INFO__, rtspIP.c_str(), useFlag, activeFlag);
	RTSPServerState value;
	std::map<std::string, RTSPServerState>::iterator itr = rtspServerInfoMap.find(rtspIP);
	bool bRet = true;

	if(useFlag != 0)
	{
		if(!getRtspSystemDB().insertOrUpdateRecord(rtspIP.c_str(), activeFlag)) return false;
		if(itr != rtspServerInfoMap.end())
		{
			(*itr).second.activeFlag = activeFlag;

		}
		else
		{
			memset(&value, 0x00, sizeof(RTSPServerState));
			value.activeFlag = activeFlag;
			rtspServerInfoMap.insert(std::map<std::string, RTSPServerState>::value_type(rtspIP, value));
		}
		
	}
	else
	{
		if(!getRtspSystemDB().querySystemInfo(rtspIP)) return false;
		bRet = getRtspSystemDB().deleteRecordWithKey(rtspIP);
		if(itr != rtspServerInfoMap.end())
		{
			rtspServerInfoMap.erase(itr);
		}
	}
	return bRet;
}

/** @brief �������� slave rtsp server���¸� �����´�.
	@return �ǹ� ����.
	@param rtspIp rtsp server�� ������
	@param info rtsp server�� ���� ����(cpu, memory��)
*/
void RTSPServerManager::getInfo(std::string rtspIP, RTSPServerState& info)
{
	LOGD(TAG, "%s(%d) %s: rtsp server ip(%s)", __CALL_INFO__, rtspIP.c_str());
	//RTSPServerState value;

	std::map<std::string, RTSPServerState>::iterator itr = rtspServerInfoMap.find(rtspIP);
	if(itr != rtspServerInfoMap.end())
	{		
		memcpy(&info, &((*itr).second), sizeof(RTSPServerState));
	}
	else
	{
		//memset(&value, 0x00, sizeof(RTSPServerState));
		//rtspServerInfoMap.insert(std::map<std::string, RTSPServerState>::value_type(rtspIP, value));
	}
}

/** @brief duration�� �˾ƿ��� ���� slave rtsp server�� ���� index ���� ������ duration�� �˾ƿ� db�� ������Ʈ �Ѵ�.
	@return �ǹ� ����.
	@param rtspIp rtsp server�� ������
	@param info rtsp server�� ���� ����(cpu, memory��)
*/
void RTSPServerManager::getRTSPFileInfo()
{
	std::vector<RTSPFileInfo>::iterator itr = rtspFileInfoMap.begin();

	while(itr != rtspFileInfoMap.end() && !isStop)
	{
		// rtsp ������ describe message �����Ͽ� index ������ ���鵵�� �Ѵ�.
		// NOT FOUND �̰ų� index ���� ������ �������� ����
		if(getFileDuration((*itr)))
		{
			if((*itr).fileDuration != "")
			{
				getRtspInfoDB().updateRecordDuration((*itr).rtspIp + "." +std::to_string((*itr).rtspPort), (*itr).fileDuration);
			}
			itr = rtspFileInfoMap.erase(itr);
		}
		else ++itr;
	}
}

/** @brief ���� ���� ���� slave rtsp server�� ���¸� ���� �ϰ� ���¸� ������Ʈ �Ѵ�.
	@return �ǹ� ����.
	@param ����.
*/
void RTSPServerManager::getRTSPServerInfo()
{
	RTSPServerState value;
	memset(&value, 0x00, sizeof(RTSPServerState));

	std::map<std::string, RTSPServerState>::iterator itr = rtspServerInfoMap.begin();
	while(itr != rtspServerInfoMap.end())
	{
		std::vector<std::string> vecIp;
		split((std::string)(*itr).first, ".", vecIp);
		if(vecIp.size() == 5)
		{
			getRTSPServerState(vecIp[0] + "." + vecIp[1] + "." + vecIp[2] + "." + vecIp[3], atoi(vecIp[4].c_str()), value);
			memcpy(&((*itr).second), &value, sizeof(RTSPServerState));
		}
		itr++;
	}
}

/** @brief slave rtsp server�� ���¸� ���� �ϰ� ���¸� ������Ʈ �Ѵ�.
	@return true or false ���� Ȯ�� ������ ���� 
	@param ip slave rtsp server ip
	@param port slave rtsp server port
	@param value ����Ȯ���� �������̾����� slave rtsp server�� ���°�
*/
bool RTSPServerManager::getRTSPServerState(std::string ip, int port, RTSPServerState& value)
{
	LOGD(TAG, "%s(%d) %s: rtsp ip(%s), port(%d)", __CALL_INFO__, ip.c_str(), port);

	std::string requestStr = "";
	
	std::string separator = "\n";
	bool ret = false;
	int bodysize = -1;
	char tmpBuffer[256];
	char bodyBuffer[256];
	char recvBuffer[BUFSIZE];
	int responseCode = -1;
	std::string responseBody = "";
	RTSPSocket* rtspClient = new RTSPSocket();

	if(!rtspClient->create())
	{
		delete rtspClient;
		return false;
	}
	
	if(!rtspClient->connect(ip, port))
	{
		delete rtspClient;
		return false;
	}

	if(!rtspClient->isValid())
	{
		//rtspClient->close();
		delete rtspClient;
		return false;
	}

	bodysize = snprintf(bodyBuffer, sizeof(bodyBuffer),
			"%s",
			"cpu: \r\ndiskUsage: \r\nsessions: \r\ntraffic: \r\nmaxSessions: \r\nmaxTraffic: \r\n");

	sprintf(tmpBuffer, "GET_PARAMETER rtsp://%s:%d RTSP/1.0\r\n", ip.c_str(), port);
	requestStr.append(tmpBuffer);
	requestStr.append("CSeq: 1\r\n");
	requestStr.append("Content-type: text/parameters\r\n");
	sprintf(tmpBuffer, "Content-length: %d\r\n\r\n", bodysize);
	requestStr.append(tmpBuffer);

	requestStr.append(bodyBuffer);

	ret = rtspClient->send((char*)requestStr.c_str(), requestStr.size());

	if(!ret) 
	{
		//rtspClient->close();
		delete rtspClient;
		return ret;
	}

	int status = rtspClient->recv((char*)recvBuffer);
	if (status < 0)
	{
		LOGE(TAG, "%s(%d) %s: Socket recv error. size(%d)", __CALL_INFO__, status);
		delete rtspClient;
		//rtspClient->close();
		return false;
	}

	if(status == 0)
	{
		LOGE(TAG, "%s(%d) %s: Socket recv timeout.", __CALL_INFO__);
		//rtspClient->close();
		delete rtspClient;
		return false;
	}	

	parseResponseCode(rtspClient, recvBuffer, responseCode, responseBody);
	LOGD(TAG, "%s(%d) %s: responseCode(%d)", __CALL_INFO__, responseCode);

	if(responseBody != "" && responseCode == 200)
	{
		std::vector<std::string> contentVec;
		std::vector<string> headerVec;
		split(responseBody, separator, contentVec);

		for(uint i=0; i<contentVec.size(); i++)
		{
			headerVec.clear();
			split(contentVec[i], ':', headerVec);
			if(headerVec.size() == 2)
			{
				if(headerVec[0] == ALTICAST_API_BODY_TABLE[eAAB_CPU]) value.cpu = atoi(headerVec[1].c_str());
				else if(headerVec[0] == ALTICAST_API_BODY_TABLE[eAAB_DISKUSAGE]) value.diskUsage = atoi(headerVec[1].c_str());
				else if(headerVec[0] == ALTICAST_API_BODY_TABLE[eAAB_SESSIONS]) value.sessions = atoi(headerVec[1].c_str());
				else if(headerVec[0] == ALTICAST_API_BODY_TABLE[eAAB_TRAFFIC]) value.traffic = atoi(headerVec[1].c_str());
				else if(headerVec[0] == ALTICAST_API_BODY_TABLE[eAAB_MAX_SESSIONS]) value.maxSessions = atoi(headerVec[1].c_str());
				else if(headerVec[0] == ALTICAST_API_BODY_TABLE[eAAB_MAX_TRAFFIC]) value.maxTraffic = atoi(headerVec[1].c_str());
			}
		}
		LOGD(TAG, "%s(%d) %s: cpu(%d), diskUsage(%d), sessions(%d), traffic(%d), maxSessions(%d), maxTraffic(%d)", 
			__CALL_INFO__, value.cpu, value.diskUsage, value.sessions, value.traffic, value.maxSessions, value.maxTraffic);
	}
	
	//rtspClient->close();
	delete rtspClient;
	if(responseCode == 200) return true;
	return false;
}

/** @brief slave rtsp server���� ���� �޽����� �Ľ��Ѵ�.
	@return ����.
	@param client slave rtsp server socket
	@param pLine return code�� �ִ� string line
	@param nResponseCode string line�� �Ľ������� response code (200, 404 ���)
	@param responseBody body message
*/
void RTSPServerManager::parseResponseCode(RTSPSocket* client, const char* pLine, int& nResponseCode, std::string& responseBody)
{
	std::vector<string> lineVec;
	std::vector<string> headerVec;
	std::string separator = "\n";
	int contentsLength = 0;

	split(std::string(pLine), separator, lineVec);
	
	for(uint i=0; i<lineVec.size(); i++)
	{
		if(i == 0)
		{
			sscanf((char*)lineVec[i].c_str(), "RTSP/%*s%u", &nResponseCode);
		}
		else
		{
			headerVec.clear();
			split(lineVec[i], ':', headerVec);
			if(headerVec.size() == 2 && headerVec[0] == RTSP_HEADER_TABLE[eRH_CONTENT_LENGTH])
			{
				contentsLength = atoi(headerVec[1].c_str());
				break;
			}
		}
	}
	
	if(contentsLength > 0)
	{
		responseBody = recvContent(client, contentsLength);	
	}
}

/** @brief slave rtsp server�� describe �޽����� ������ ���� index ������ �����ϰ� �ϰ�, duration�� �˾ƿ´�.
	@return true or false return code�� ����
	@param value index ������ ���������� �����ǰ� duration�� ���������� �˾ƿ����� ���� ä���ش�.
*/
bool RTSPServerManager::getFileDuration(RTSPFileInfo& value)
{
	LOGD(TAG, "%s(%d) %s: rtsp ip(%s), filePath(%s), fileName(%s)", __CALL_INFO__, value.rtspIp.c_str(), value.filePath.c_str(), value.fileName.c_str());

	std::string requestStr = "";
	
	bool ret = false;
	int bodysize = 0;
	char tmpBuffer[256];
	//char bodyBuffer[256];
	char recvBuffer[BUFSIZE];
	int responseCode = -1;
	std::string responseBody = "";
	std::string separator = "\n";
	RTSPSocket* rtspClient = new RTSPSocket();

	if(!rtspClient->create())
	{
		delete rtspClient;
		return ret;
	}
	
	if(!rtspClient->connect(value.rtspIp, value.rtspPort))
	{
		delete rtspClient;
		return ret;
	}

	if(!rtspClient->isValid())
	{
		delete rtspClient;
		//rtspClient->close();
		return ret;
	}

	sprintf(tmpBuffer, "DESCRIBE rtsp://%s:%d%s/%s RTSP/1.0\r\n", value.rtspIp.c_str(), value.rtspPort, value.filePath.c_str(), value.fileName.c_str());
	requestStr.append(tmpBuffer);
	requestStr.append("CSeq: 1\r\n");
	requestStr.append("Content-type: application/sdp\r\n");
	sprintf(tmpBuffer, "Content-length: %d\r\n\r\n", bodysize);
	requestStr.append(tmpBuffer);

	ret = rtspClient->send((char*)requestStr.c_str(), requestStr.size());

	if(!ret) 
	{
		delete rtspClient;
		//rtspClient->close();
		return ret;
	}

	int status = rtspClient->recv((char*)recvBuffer);
	if (status < 0)
	{
		LOGE(TAG, "%s(%d) %s: Socket recv error. size(%d)", __CALL_INFO__, status);
		
		delete rtspClient;
		//rtspClient->close();
		return ret;
	}

	if(status == 0)
	{
		LOGE(TAG, "%s(%d) %s: Socket recv timeout.", __CALL_INFO__);
		delete rtspClient;
		//rtspClient->close();
		return ret;
	}	
	
	parseResponseCode(rtspClient, recvBuffer, responseCode, responseBody);
	LOGD(TAG, "%s(%d) %s: responseCode(%d)", __CALL_INFO__, responseCode);

	if(responseBody != "" && responseCode == 200)
	{
		std::vector<std::string> contentVec;
		std::vector<string> headerVec;
		split(responseBody, separator, contentVec);

		for(uint i=0; i<contentVec.size(); i++)
		{
			headerVec.clear();
			split(contentVec[i], ':', headerVec);
			if(headerVec.size() == 2)
			{
				if(headerVec[0] == "a=range")
				{
					int start = 0, end = 0;
					if((end = headerVec[1].find('-', start)) != (int)(string::npos))
					{
						start = end + 1;
						value.fileDuration = headerVec[1].substr(start);
					}
				}
			}
		}
	}
	LOGD(TAG, "%s(%d) %s: duration(%s)", __CALL_INFO__, value.fileDuration.c_str());
	
	//rtspClient->close();
	delete rtspClient;
	if(responseCode == 404 || (responseCode == 200 && value.fileDuration != "")) ret = true;

	return ret;
}

/** @brief socket���� contentLength�� ������ body�� �о� �´�.
	@return �о�� body
	@param value contentLength socket���� �о�� �� bodyLength
*/
std::string RTSPServerManager::recvContent(RTSPSocket* client, int contentLength)
{
	int status;
	char buf[contentLength];

	status = client->syncRecv(buf, contentLength);	

	if(status == contentLength)
	{
		return std::string(buf);
	}
	return "";
}

/** @brief ���� network ��뷮�� ���� ���� slave rtsp server�� �˾ƿ´�.
	@return ���� network��뷮�� ���� ���� slave rtsp server�� ����ִ� map�� index
	@param slaveList slave rtsp server list
*/
int RTSPServerManager::findSlaveServer(std::vector<AssetInfo> slaveList)
{
	int nIndex = -1;
	int networkUsage = -1;
	
	for(uint i=0; i<slaveList.size(); i++)
	{
		std::map<std::string, RTSPServerState>::iterator itr = rtspServerInfoMap.find(std::string(slaveList[i].slave_ip));
		if(itr != rtspServerInfoMap.end())
		{
			if(networkUsage == -1) networkUsage = (itr->second).traffic;
			
			if(networkUsage >= (itr->second).traffic)
			{
				nIndex = i;
			}
		}
	}

	return nIndex;
}
