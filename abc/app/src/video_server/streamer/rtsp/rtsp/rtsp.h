#ifndef __RTSP_H__
#define __RTSP_H__

enum RTSP_PLAY_STATE
{
	eRPS_FF = 0,
	eRPS_RW,
	eRPS_PLAY,
	eRPS_PAUSE,
	eRPS_BOF,
	eRPS_EOF,
	eRPS_STOPPED,
};

enum RTSP_METHOD
{
	eRM_UNKNOWN				= -1,
	eRM_OPTIONS,
	eRM_DESCRIBE,
	eRM_ANNOUNCE,
	eRM_SETUP,
	eRM_PLAY,
	eRM_PAUSE,
	eRM_TEARDOWN,
	eRM_GET_PARAMETER,
	eRM_SET_PARAMETER,
	eRM_REDIRECT,
	eRM_RECORD,
};

enum RTSP_HEADER
{
	eRH_ACCEPT				= 0,
	eRH_ACCEPT_ENCODING,
	eRH_ACCEPT_LANGUAGE,
	eRH_ALLOW,
	eRH_AUTHORIZATION,
	eRH_BANDWIDTH,
	eRH_BLOCKSIZE,
	eRH_CACHE_CONTROL,
	eRH_CONFERENCE,
	eRH_CONNECTION,
	eRH_CONTENT_BASE,
	eRH_CONTENT_ENCODING,
	eRH_CONTENT_LANGUAGE,
	eRH_CONTENT_LENGTH,
	eRH_CONTENT_LOCATION,
	eRH_CONTENT_TYPE,
	eRH_CSEQ,
	eRH_DATE,
	eRH_EXPIRES,
	eRH_FROM,
	eRH_IF_MODIFIED_SINCE,
	eRH_LAST_MODIFIED,
	eRH_LOCATION,
	eRH_PROXY_AUTHENTICATE,
	eRH_PROXY_REQUIRE,
	eRH_PUBLIC,
	eRH_RANGE,
	eRH_REFERER,
	eRH_REQUIRE,
	eRH_RETRY_AFTER,
	eRH_RTP_INFO,
	eRH_SCALE,
	eRH_SESSION,
	eRH_SERVER,
	eRH_SPEED,
	eRH_TRANSPORT,
	eRH_UNSUPPORTED,
	eRH_USER_AGENT,
	eRH_VIA,
	eRH_WWW_AUTHENTICATE,
	
	// alticast header
	eRH_XTOKEN,
};


// alticast api info
enum ALTICAST_API_COMMAND
{
	eAAC_ASSET = 0,
	eAAC_SYSTEM,
	eAAC_TOKEN,
	eAAC_STATE,
	eAAC_FILE,
	eAAC_VID,
};

enum ALTICAST_API_SUB_COMMAND
{
	eAASC_SYSTEM = 0,
	eAASC_VID,
	eAASC_TOKEN,
	eAASC_RESOURCE,
};

enum ALTICAST_API_BODY
{
	eAAB_ASSETTYPEID = 0,
	eAAB_VALIDTIME,
	eAAB_USEFLAG,
	eAAB_ACTIVEFLAG,
	eAAB_FILEPATH,
	eAAB_FILENAME,
	eAAB_FILESIZE,
	eAAB_PROTECTIONTYPE,
	eAAB_CONTENTID,
	eAAB_TOKEN,
	eAAB_CPU,
	eAAB_DISKUSAGE,
	eAAB_SESSIONS,
	eAAB_TRAFFIC,
	eAAB_STREAM_STATE,
	eAAB_POSITION,
	eAAB_MAX_SESSIONS,
	eAAB_MAX_TRAFFIC,
};

#ifdef __cplusplus
extern "C" {
#endif

extern const char* RTSP_HEADER_TABLE[];
extern const char* RTSP_PLAY_STATE_TABLE[];
// alticast api info
extern const char* ALTICAST_API_COMMAND_TABLE[];
extern const char* ALTICAST_API_SUB_COMMAND_TABLE[];
extern const char* ALTICAST_API_BODY_TABLE[];

int rtspGetMethodCount();
int rtspGetMethod(char* method);
const char* rtspGetMethodName(int method);

int rtspGetHeaderCount();
int rtspGetHeader(char* header);
const char* rtspGetHeaderName(int header);

const char* rtspGetStatusReason(int statusCode);
const char* rtspGetTransportHeaderMode(int mode);

HRESULT rtspParseVersion(char* version, int* majorVersion, int* minorVersion);

HRESULT readLine(char* buffer, int dataSize, char** line, char** next);
HRESULT copyStringWithAllocation(char* source, char** dest);
HRESULT releaseString(char** string);

HRESULT parseURL(
	const char* url,
	char* protocol, char* id, char* host, int* port, char* absolutePath);


HRESULT parsePortNumbers(char* text, int* port0, int* port1);

UINT convertTimeToHz(LONGLONG pts, int sampleRate);
LONGLONG convertHzToTime(LONGLONG sampleRate, LONGLONG hz);

HRESULT convertTimeToNPTFormat(double time, char* nptTime);
HRESULT parseNormalPlayTime(char* time, double* playTime);

#ifdef __cplusplus
}
#endif
#endif // __HELPER_FUNC_H__
