#include "nomad.h"
#include "rtsp.h"
#include "nomad_log.h"
#include <stdlib.h>
#include <string.h>

static const char* TAG = "RTSP";

typedef struct _SIndexAndTextItem
{
	int nIndex;
	const char* szText;

} SIndexAndTextItem;

const char* RTSP_PLAY_STATE_TABLE[] = {
	"ff",
	"rw",
	"playing",
	"paused",
	"bof",
	"eof",
	"stopped",
};

const char* RTSP_METHOD_TABLE[] = {
	"OPTIONS",
	"DESCRIBE",
	"ANNOUNCE",
	"SETUP",
	"PLAY",
	"PAUSE",
	"TEARDOWN",
	"GET_PARAMETER",
	"SET_PARAMETER",
	"REDIRECT",
	"RECORD",
};

const char* RTSP_HEADER_TABLE[] = {
	"Accept",
	"Accept-Encoding",
	"Accept-Language",
	"Allow",
	"Authorization",
	"Bandwidth",
	"Blocksize",
	"Cache-Control",
	"Conference",
	"Connection",
	"Content-Base",
	"Content-Encoding",
	"Content-Language",
	"Content-Length",
	"Content-Location",
	"Content-Type",
	"CSeq",
	"Date",
	"Expires",
	"From",
	"If-Modified-Since",
	"Last-Modified",
	"Location",
	"Proxy-Authenticate",
	"Proxy-Require",
	"Public",
	"Range",
	"Referer",
	"Require",
	"Retry-After",
	"RTP-Info",
	"Scale",
	"Session",
	"Server",
	"Speed",
	"Transport",
	"Unsupported",
	"User-Agent",
	"Via",
	"WWW-Authenticate",

	// alticast header
	"X-Token",
};

const SIndexAndTextItem STATUS_CODE_TABLE[] =
{
	{ 100, "Continue" },

	{ 200, "OK" },
	{ 201, "Created" },
	{ 250, "Low on Storage Space" },

	{ 300, "Multiple Choices" },
	{ 301, "Moved Permanently" },
	{ 302, "Moved Temporarily" },
	{ 303, "See Other" },
	{ 304, "Not Modified" },
	{ 305, "Use Proxy" },

	{ 400, "Bad Request" },
	{ 401, "Unauthorized" },
	{ 402, "Payment Required" },
	{ 403, "Forbidden" },
	{ 404, "Not Found" },
	{ 405, "Method Not Allowed" },
	{ 406, "Not Acceptable" },
	{ 407, "Proxy Authentication Required" },
	{ 408, "Request Time-out" },
	{ 410, "Gone" },
	{ 411, "Length Required" },
	{ 412, "Precondition Failed" },
	{ 413, "Request Entity Too Large" },
	{ 414, "Request-URI Too Large" },
	{ 415, "Unsupported Media Type" },
	{ 451, "Parameter Not Understood" },
	{ 452, "Conference Not Found" },
	{ 453, "Not Enough Bandwidth" },
	{ 454, "Session Not Found" },
	{ 455, "Method Not Valid in This State" },
	{ 456, "Header Field Not Valid for Resource" },
	{ 457, "Invalid Range" },
	{ 458, "Parameter Is Read-Only" },
	{ 459, "Aggregate operation not allowed" },
	{ 460, "Only Aggregate operation allowed" },
	{ 461, "Unsupported transport" },
	{ 462, "Destination unreachable" },
	
	{ 500, "Internal Server Error" },
	{ 501, "Not Implemented" },
	{ 502, "Bad Gateway" },
	{ 503, "Service Unavailable" },
	{ 504, "Gateway Time-out" },
	{ 505, "RTSP Version Not Supported" },
	{ 551, "Option not supported" },
};

// alticast api info
const char* ALTICAST_API_COMMAND_TABLE[] =
{
	"asset",
	"system",
	"token",
	"state",
	"file",
	"vid",
};

const char* ALTICAST_API_SUB_COMMAND_TABLE[] = 
{
	"system",
	"vid",
	"token",
	"resource",
};

const char* ALTICAST_API_BODY_TABLE[] = 
{
	"assetTypeId",
	"validTime",
	"useFlag",
	"activeFlag",
	"filePath",
	"fileName",
	"fileSize",
	"protectionType",
	"contentId",
	"token",
	"cpu",
	"diskUsage",
	"sessions",
	"traffic",
	"Stream-state",
	"position",
	"maxSessions",
	"maxTraffic",
};

#ifdef __cplusplus
extern "C" {
#endif

int rtspGetHeaderCount()
{
	return sizeof(RTSP_HEADER_TABLE) / sizeof(RTSP_HEADER_TABLE[0]);
}

int rtspGetMethodCount()
{
	return sizeof(RTSP_METHOD_TABLE) / sizeof(RTSP_METHOD_TABLE[0]);
}

int rtspGetMethod(char* method)
{
	int nCount = rtspGetMethodCount();

	for(int i=0; i<nCount; i++)
	{
		if(strcmp(RTSP_METHOD_TABLE[i], method) == 0)
		{
			return i;
		}
	}

	return -1;
}

const char* rtspGetMethodName(int method)
{
	return RTSP_METHOD_TABLE[method];
}

const char* rtspGetHeaderName(int header)
{
	return RTSP_HEADER_TABLE[header];
}

int rtspGetHeader(char* header)
{
	int count = rtspGetHeaderCount();

	for(int i=0; i<count; i++)
	{
		if(strcasecmp(RTSP_HEADER_TABLE[i], header) == 0)
		{
			return i;
		}
	}

	return -1;
}

HRESULT rtspParseVersion(char* version, int* majorVersion, int* minorVersion)
{
	char* tmpMajorVersion;
	char* tmpMinorVersion;
	int length = strlen(version);

	if(length < 8)
	{
		return E_FAIL;
	}

	if(version != strstr(version, "RTSP/"))
	{
		return E_FAIL;
	}

	version += 5;

	char* dot = strchr(version, '.');
	if(dot == NULL)
	{
		return E_FAIL;
	}

	dot[0] = '\0';
	tmpMajorVersion = version;
	tmpMinorVersion = dot + 1;

	if(tmpMajorVersion == NULL)
	{
		LOGE_CALL(TAG, "RTSP");
		return E_FAIL;
	}
	*majorVersion = atoi(tmpMajorVersion);

	if(tmpMinorVersion == NULL)
	{
		LOGE_CALL(TAG, "RTSP");
		return E_FAIL;
	}
	*minorVersion = atoi(tmpMinorVersion);

	return S_OK;
}

const char* rtspGetStatusReason(int statusCode)
{
	int count = sizeof(STATUS_CODE_TABLE) / sizeof(STATUS_CODE_TABLE[0]);

	for(int i=0; i<count; i++)
	{
		if(statusCode == STATUS_CODE_TABLE[i].nIndex)
		{
			return STATUS_CODE_TABLE[i].szText;
		}
	}

	return NULL;
}

HRESULT readLine(char* buffer, int dataSize, char** line, char** next)
{
    int i = 0;
	HRESULT hr;
    char* start;

    // Skip newlines and whitespaces
    for(i=0; i<dataSize; i++)
    {
        if( buffer[i] == '\r' ||
            buffer[i] == '\n' ||
            buffer[i] == ' '  ||
            buffer[i] == '\t' )
        {  
            i++;
        }
        else
        {
            break;
        }
    }

	if(i == dataSize)
	{
		return E_FAIL;
	}

    start = &buffer[i];

    while( buffer[i] != '\0' )
    {
        if( buffer[i] == '\r'  )
        {
            buffer[i] = '\0';
        }
		else if(buffer[i] == '\n')
		{
            buffer[i] = '\0';
            break;
		}

        i++;
    }

	if(start[0] == '\0')
	{
		hr = E_FAIL;
	}
	else
	{
		hr = S_OK;
	}

	if(SUCCEEDED(hr))
	{
		*next = &buffer[i+1];
	}

    *line = start;

    return hr;
}

HRESULT copyStringWithAllocation(char* source, char** dest)
{
	if(source == NULL)
	{
		return E_FAIL;
	}

	char* tmpDest;
	int length = strlen(source) + 1;
	tmpDest = new char [length];

	strcpy(tmpDest, source);

	*dest = tmpDest;

	return S_OK;
}

HRESULT releaseString(char** msgString)
{
	char* tmpString = *msgString;

	if(tmpString == NULL)
	{
		return E_FAIL;
	}

	delete[] tmpString;
	*msgString = NULL;

	return S_OK;
}

/**
 * It assumes that each string buffer's size is enough to save each information.
 * 
 * @param url
 * @param protocol
 * @param host
 * @param port
 * @param absolutePath
 */
HRESULT parseURL(
	const char* url,
	char* protocol, char* id, char* host, int* port, char* absolutePath)
{
    char* start = (char*) url;
	char* end;
	char tmpId[64] = {'\0', };
    int nPort = 80;
	int length;

	// 1.2.3.4
	if(url == NULL)
	{
		return E_FAIL;
	}

	const int nURLLength = strlen(url);
	if(nURLLength < 7)
	{
		return E_FAIL;
	}

    // If there is no protocal, ignore this address.
	end = strstr(start, "://");
    if(end != NULL)
    {  
		// PROTOCOL
		length = (int) (end - start);

		if(protocol)
		{
			strncpy(protocol, start, length);
			protocol[length] = '\0';
		}

		start = end + 3;
	}
	else
	{
		// Default protocol
		if(protocol)
		{
			strcpy(protocol, "rtsp");
		}
	}
   
	// I didn't consider decoding url-escape characters.
	end = strchr(start, '/');
    if(end == NULL)
    {  
        // There is no urlpath, thus we will regard urlpath as /.
		if(absolutePath)
		{
			strcpy(absolutePath, "/");
		}
		end = start + nURLLength;
    }
    else
    {
        // Absolute Path
		if(absolutePath)
		{
			strcpy(absolutePath, end);
		
//			char* startId;
//			int pathLength = strlen(absolutePath);
//			int idLength;
//			startId = strchr(absolutePath, '?');
//			if(startId)
//			{
//				startId[0] = '\0';
//				idLength = strlen(startId + 1);
//				printf("pathLength(%d), idLength(%d)\n", pathLength, idLength);
//				if(pathLength > idLength + 3)
//				{
//					strncpy(id, startId + 4, pathLength - (idLength + 3));
//				}
//			}
//			else
//			{
//				id[0] = '\0';
//			}
		}
    }

	// ID
    length = (int)(end - start);
	
	// Host
	if(length > 0)
	{
		strncpy(tmpId, start, length);
		tmpId[length] = '\0';
	}

	start = strchr(tmpId, '@');
	if(start)
	{
		start[0] = '\0';
		strcpy(host, start + 1);
	}
	else
	{
		strncpy(host, tmpId, length);
		host[length] = '\0';
		tmpId[0] = '\0';
	}

    // Extract a port number from hostname.
    start = strchr(host, ':');
    if(start != NULL)
    {
        start[0] = '\0';

        nPort = atoi(start + 1);
		ASSERT(nPort > 0);
    }

	if(port)
	{
		*port = nPort;
	}

	if(tmpId[0] != '\0')
	{
		strcpy(id, tmpId);
	}

	printf("protocol(%s), host(%s), port(%d), absolutePath(%s), id(%s)\n", protocol, host, *port, absolutePath, id);

    return S_OK;
}

/**
 * @param szText: ex) 1234-1235
 */
HRESULT parsePortNumbers(char* text, int* port0, int* port1)
{
	char* tmpToken;

	if(text == NULL)
	{
		return E_FAIL;
	}

	tmpToken = strchr(text, '-');
	if(tmpToken)
	{
		tmpToken[0] = '\0';
		//LOGD("WORM", "%s(%d) %s port1(%s)", __CALL_INFO__, tmpToken);
		*port1 = ::atoi(tmpToken + 1);
	}

	*port0 = ::atoi(text);

	return S_OK;
}

/**
 * pts: 100ns unit
 * nSampleRate: Hz in a second.
 */
UINT convertTimeToHz(LONGLONG pts, int sampleRate)
{
	UINT nTimeStamp = (UINT) ((pts * sampleRate) / 10000000LL);
	return nTimeStamp;
}

/**
 * pts: 100ns unit
 * sampleRate: Hz in a second.
 */
LONGLONG ConvertHzToTime(LONGLONG sampleRate, LONGLONG hz)
{
	return hz * 10000000LL / sampleRate;
}

HRESULT convertTimeToNPTFormat(double time, char* nptTime)
{
#if 1
	sprintf(nptTime, "%.3f", time);
	return S_OK;
#else
	if(time < 0)
	{
		return E_FAIL;
	}

	double dTime = ((double) time) / 10000000.0;
	sprintf(nptTime, "%.3f", dTime);

	return S_OK;
#endif
}

HRESULT parseNormalPlayTime(char* time, double* playTime)
{
#if 1
	if(strlen(time) == 0)
	{
		LOGD_CALL("RTSP", "");
		*playTime = -1.0;
		return S_FALSE;
	}
	*playTime = ::atof(time);
	
	return S_OK;
#else
	LOGD("RTSP", "%s:%d time: %s", __FILENAME__, __LINE__, time);
	if(strlen(time) == 0)
	{
		LOGD_CALL("RTSP", "");
		*playTime = -1LL;
		return S_FALSE;
	}

	int i;
	char* second = time;
	char* fraction = NULL;

	LOGD_CALL("RTSP", "");
	for(i=0; time[i] != '\0'; i++)
	{
		if(time[i] == '.')
		{
			time[i] = '\0';
			fraction = &time[i+1];
			break;
		}
	}

	int sec;
	int length = 0;
	LONGLONG llFraction = 0;

	if(second == NULL)
	{
		LOGE_LINE("ERROR");
		return E_FAIL;
	}
	sec = atoi(second);

	if(fraction)
	{
		length = strlen(fraction);
		if(length > 3)
		{
			fraction[3] = '\0';
			length = 3;
		}

		if(fraction == NULL)
		{
			LOGE_LINE("ERROR");
			return E_FAIL;
		}
		llFraction = atoi(fraction);
	}

	if(llFraction > 0)
	{
		LONGLONG llDivider = 1;

		for(i=0; i<length; i++)
		{
			llDivider *= 10;
		}
		
		// Unit: 100ns
		llFraction = (LONGLONG) llFraction * 10000000LL / llDivider;
	}
	else
	{
		llFraction = 0;
	}

	*playTime = sec * 10000000LL + llFraction;

	return S_OK;
#endif
}

#ifdef __cplusplus
}
#endif
