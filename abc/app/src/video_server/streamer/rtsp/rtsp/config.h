#ifndef __CONFIG_H__
#define __CONFIG_H__
#include <string>
#include <nomad.h>

enum RTSP_STATE
{   
    eRS_INIT,
    eRS_READY,
    eRS_PLAYING,
    eRS_RECORDING,
};

enum STREAM_TYPE
{   
	eST_UNKNOWN		= -1,
    eST_AUDIO		= 0,
    eST_VIDEO		= 1,
	eST_TS			= 2,
};

enum SCALER_ID
{
	SI_FHD = 0,
	SI_HD,
	SI_SD,
};

enum AUDIO_TYPE
{
	eAT_AAC = 0,
	eAT_PCMU,
	eAT_PCMA,
};

//#define USE_ORTP 1
extern const int DEFAULT_RTSP_TIMEOUT;
extern const int DEFAULT_AUTH_TIMEOUT;
extern const int DEFAULT_RTSP_CHECK_INTERVAL;
extern const std::string RTSP_MEDIA_DIR;
extern const std::string RTSP_CONF_DIR;
extern const std::string RTSP_INDEX_DIR;
extern const std::string RTSP_LOG_DIR;
extern const std::string RTSP_DB_DIR;

extern const int SCALER_UDP_PORT_TABLE[];

extern int getMaxSessions();
extern void setMaxSessions(int sessions);
extern int getMaxTraffic();
extern void setMaxTraffic(int traffic);
extern int getRtpPort();
extern const int getTokenUse();
extern void setTokenUse(const int use);

extern const bool USE_AUDIO;
extern const int AUDIO_TYPE;

#endif // __CONFIG_H__
