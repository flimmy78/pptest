#include <nomad.h>
#include "config.h"


const int DEFAULT_RTSP_TIMEOUT = 60;
const int DEFAULT_AUTH_TIMEOUT = 30;
const int DEFAULT_RTSP_CHECK_INTERVAL = 300;

const std::string RTSP_MEDIA_DIR = "/opt/demo/media"; // media data들이 들어갈 절대 경로
const std::string RTSP_CONF_DIR = "/root/video_server/conf"; // config 파일들이 들어갈 절대 경로
const std::string RTSP_INDEX_DIR = "/opt/demo/index"; // media data들의 index 파일이 들어갈 절대 경로
const std::string RTSP_LOG_DIR = "/opt/demo/log"; // 로그 파일들이 들어갈 절대 경로
const std::string RTSP_DB_DIR = "/opt/demo/db/asset.db"; // db 파일이 들어갈 절대 경로

const int SCALER_UDP_PORT_TABLE[] = {19001, 19002, 19003,};

const bool USE_AUDIO = true;
const int AUDIO_TYPE = eAT_PCMU;

int maxSessions = 0;

/** @brief 설정된 rtsp max session 수를 가져온다.
	@return 설정된 rtsp max session 수
	@param 없음.
*/
int getMaxSessions()
{
	return maxSessions;
}

/** @brief rtsp max session 수를 설정한다.
	@return 없음.
	@param sessions 설정할 rtsp max session 수
*/
void setMaxSessions(int sessions)
{
	maxSessions = sessions;
}

int maxTraffic = 0;

/** @brief 설정된 rtsp max traffic을 가져온다.
	@return 설정된 rtsp max traffic
	@param 없음.
*/
int getMaxTraffic()
{
	return maxTraffic;
}

/** @brief rtsp max traffic을 설정한다.
	@return 없음.
	@param traffic 설정할 rtsp max traffic
*/
void setMaxTraffic(int traffic)
{
	maxTraffic = traffic;
}

const int rtpPortMin = 30001; // rtp를 위해서 사용할 min port
const int rtpPortMax = 50000; // rtp를 위해서 사용할 max port
int rtpPort = 30000;

/** @brief 사용할 rtp 포트를 가져온다.
	@return 사용할 rtp 포트
	@param 없음.
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

/** @brief token을 사용할지 여부를 가져온다.
	@return 1 : token 사용, 0 : token 미사용
	@param 없음.
*/
const int getTokenUse()
{
	return tokenUse;
}

/** @brief token을 사용할지 여부를 설정한다.
	@return 없음.
	@param use 1 : token 사용, 0 : token 미사용
*/
void setTokenUse(const int use)
{
	tokenUse = use;
}
