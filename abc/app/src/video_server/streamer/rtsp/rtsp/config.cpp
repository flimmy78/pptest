#include <nomad.h>
#include "config.h"


const int DEFAULT_RTSP_TIMEOUT = 60;
const int DEFAULT_AUTH_TIMEOUT = 30;
const int DEFAULT_RTSP_CHECK_INTERVAL = 300;

const std::string RTSP_MEDIA_DIR = "/opt/demo/media"; // media data���� �� ���� ���
const std::string RTSP_CONF_DIR = "/root/video_server/conf"; // config ���ϵ��� �� ���� ���
const std::string RTSP_INDEX_DIR = "/opt/demo/index"; // media data���� index ������ �� ���� ���
const std::string RTSP_LOG_DIR = "/opt/demo/log"; // �α� ���ϵ��� �� ���� ���
const std::string RTSP_DB_DIR = "/opt/demo/db/asset.db"; // db ������ �� ���� ���

const int SCALER_UDP_PORT_TABLE[] = {19001, 19002, 19003,};

const bool USE_AUDIO = true;
const int AUDIO_TYPE = eAT_PCMU;

int maxSessions = 0;

/** @brief ������ rtsp max session ���� �����´�.
	@return ������ rtsp max session ��
	@param ����.
*/
int getMaxSessions()
{
	return maxSessions;
}

/** @brief rtsp max session ���� �����Ѵ�.
	@return ����.
	@param sessions ������ rtsp max session ��
*/
void setMaxSessions(int sessions)
{
	maxSessions = sessions;
}

int maxTraffic = 0;

/** @brief ������ rtsp max traffic�� �����´�.
	@return ������ rtsp max traffic
	@param ����.
*/
int getMaxTraffic()
{
	return maxTraffic;
}

/** @brief rtsp max traffic�� �����Ѵ�.
	@return ����.
	@param traffic ������ rtsp max traffic
*/
void setMaxTraffic(int traffic)
{
	maxTraffic = traffic;
}

const int rtpPortMin = 30001; // rtp�� ���ؼ� ����� min port
const int rtpPortMax = 50000; // rtp�� ���ؼ� ����� max port
int rtpPort = 30000;

/** @brief ����� rtp ��Ʈ�� �����´�.
	@return ����� rtp ��Ʈ
	@param ����.
*/
int getRtpPort()
{	
	if(rtpPort < rtpPortMax)
	{
		rtpPort ++;
		return rtpPort;
	}
	else
	{
		rtpPort = rtpPortMin;
		return rtpPort;
	}
}

int tokenUse = 1;

/** @brief token�� ������� ���θ� �����´�.
	@return 1 : token ���, 0 : token �̻��
	@param ����.
*/
const int getTokenUse()
{
	return tokenUse;
}

/** @brief token�� ������� ���θ� �����Ѵ�.
	@return ����.
	@param use 1 : token ���, 0 : token �̻��
*/
void setTokenUse(const int use)
{
	tokenUse = use;
}
