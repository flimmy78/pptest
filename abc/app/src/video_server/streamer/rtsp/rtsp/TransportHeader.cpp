#include "nomad.h"
#include "rtsp.h"
#include "TransportHeader.h"
#include "Flags.h"
#include "nomad_log.h"
#include "StringUtil.h"

enum TRANSPORT_PARAMETERS
{
	eUNICAST,
	eMULTICAST,
	eDESTINATION,
	eINTERLEAVED,
	eAPPEND,
	eTTL,
	eLAYERS,
	ePORT,
	eCLIENT_PORT,
	eSERVER_PORT,
	eSSRC,
	eMODE,
};

static const char* SZ_TRANSPORT_PARAMETERS[] =
{
	"unicast",
	"multicast",
	"destination",
	"interleaved",
	"append",
	"ttl",
	"layers",
	"port",
	"client_port",
	"server_port",
	"ssrc",
	"mode",
};

static const char* TRANSPORT_HEADER_MODE_TABLE[] =
{
	"PLAY",
	"RECORD",
};

static const char* LOWER_TRANSPORT_TABLE[] =
{
	"TCP",
	"UDP",
};

enum CAST_TYPE
{
	eCT_MULTICAST,
	eCT_UNICAST,
};

static const char* CAST_TYPE_TABLE[] =
{
	"multicast",
	"unicast",
};

static const char* GetModeName(int nMode)
{
	return TRANSPORT_HEADER_MODE_TABLE[nMode];
}

static int GetMode(const char* szMode)
{
	int nSize = sizeof(TRANSPORT_HEADER_MODE_TABLE) / sizeof(TRANSPORT_HEADER_MODE_TABLE[0]);

	for(int i=0; i<nSize; i++)
	{
#if _WIN32
        if(_stricmp(szMode, TRANSPORT_HEADER_MODE_TABLE[i]) == 0)
#else
		if(strcasecmp(szMode, TRANSPORT_HEADER_MODE_TABLE[i]) == 0)
#endif
		{
			return i;
		}
	}

	return eRTHM_UNKNOWN;
}

/** @brief TransportHeader(rtsp header) ������.
	@return �ǹ� ����.
	@param ����.
*/
TransportHeader::TransportHeader(void) :
Header(eRH_TRANSPORT),
ssrc(0),
mode(eRTHM_UNKNOWN)
{
	memset(&anPort, 0, sizeof(anPort));
	anPort[SERVER_PORT_RTP] = 5000;
	anPort[SERVER_PORT_RTCP] = 5001;

	memset(&interleaved, 0, sizeof(interleaved));

	fieldMask = new Flags(eTHFM_CAST_TYPE);

	setLowerTransport(eLT_UDP);
	setCastType(eCT_UNICAST);

	destination = "";
	ttl = 0;
}

/** @brief TransportHeader(rtsp header) �Ҹ���.
	@return �ǹ� ����.
	@param ����.
*/
TransportHeader::~TransportHeader(void)
{
	delete fieldMask;
}

/** @brief TransportHeader(rtsp header)�� mode�� �����´�.(unicast, multicast, broadcast)
	@return mode�� index
	@param ����.
*/
int TransportHeader::getMode() const
{
	return mode;
}

/** @brief TransportHeader(rtsp header)�� mode�� �����Ѵ�.(unicast, multicast, broadcast)
	@return ����.
	@param nMode mode �� index
*/
void TransportHeader::setMode(int nMode)
{
	mode = nMode;
	fieldMask->setFlags(eTHFM_MODE, TRUE);
}

/** @brief TransportHeader(rtsp header)�� ssrc�� �����´�.
	@return ssrc ��
	@param ����.
*/
UINT TransportHeader::getSSRC() const
{
		return ssrc;
}

/** @brief TransportHeader(rtsp header)�� ssrc�� �����Ѵ�.
	@return ����.
	@param nSSRC ������ ssrc
*/
void TransportHeader::setSSRC(UINT nSSRC)
{
	ssrc = nSSRC;
	fieldMask->setFlags(eTHFM_SSRC, TRUE);
}

/** @brief TransportHeader(rtsp header)�� parameter�� index�� �����´�.
	@return ã�ƿ� index
	@param szKey ������ parameter�� key("unicast", "multicast",	"destination",	"interleaved",	"append",	"ttl",	"layers",	"port",	"client_port",	"server_port",	"ssrc",	"mode")
*/
int TransportHeader::getParameterIndex(char* szKey)
{
	int nCount = sizeof(SZ_TRANSPORT_PARAMETERS) / sizeof(SZ_TRANSPORT_PARAMETERS[0]);

	for(int i=0; i<nCount; i++)
	{
		if(strcmp(szKey, SZ_TRANSPORT_PARAMETERS[i]) == 0)
		{
			return i;
		}
	}

	return -1;
}

/** @brief TransportHeader(rtsp header)�� port�� �˾ƿ´�.
	@return �˾ƿ� port
	@param nType ������ port�� index(port 2, client port 2, server port 2)
*/
int TransportHeader::getPort(int nType)
{
	return anPort[nType];
}

/** @brief TransportHeader(rtsp header)�� port�� �����Ѵ�.
	@return ����.
	@param nPortType ���� �� port�� index(port 2, client port 2, server port 2)
	@param nPort ���� �� port number
*/
void TransportHeader::setPort(int nPortType, int nPort)
{
	anPort[nPortType] = nPort;

	UINT uMask = eTHFM_RTP_PORT << nPortType;
	fieldMask->setFlags(uMask, TRUE);
}

/** @brief TransportHeader(rtsp header)�� lowertransport �� �����´�.
	@return 0 : tcp , 1 : udp
	@param ����.
*/
int TransportHeader::getLowerTransport() const
{
	return lowerTransport;
}

/** @brief TransportHeader(rtsp header)�� lowertransport �� �����Ѵ�.
	@return ����.
	@param nLowerTransport 0 : tcp , 1 : udp
*/
void TransportHeader::setLowerTransport(const int nLowerTransport)
{
	lowerTransport = nLowerTransport;
	fieldMask->setFlags(eTHFM_LOWER_TRANSPORT, TRUE);
}

/** @brief TransportHeader(rtsp header)�� lowertransport�� �̸��� �����´�.
	@return "tcp" or "udp"
	@param ����.
*/
const char* TransportHeader::getLowerTransportName() const
{
	int nIndex = lowerTransport ? eLT_UDP : eLT_TCP;
	return LOWER_TRANSPORT_TABLE[nIndex];
}

/** @brief TransportHeader(rtsp header)�� cast���� �����´�.
	@return 0 : multicast, 1 : unicast
	@param ����.
*/
int TransportHeader::getCastType() const
{
	return castType;
}

/** @brief TransportHeader(rtsp header)�� cast���� �����Ѵ�.
	@return ����.
	@param nType 0 : multicast, 1 : unicast
*/
void TransportHeader::setCastType(int nType)
{
	castType = nType;
	fieldMask->setFlags(eTHFM_CAST_TYPE, TRUE);
}

/** @brief TransportHeader(rtsp header)�� cast�� �̸��� �����´�.
	@return "multicast" or "unicast"
	@param ����.
*/
const char* TransportHeader::getCastTypeName() const
{
	int nIndex = castType ? eCT_UNICAST : eCT_MULTICAST;
	return CAST_TYPE_TABLE[nIndex];
}

/** @brief TransportHeader(rtsp header)�� rtp over tcp interleaved channel ���� �����´�.
	@return channel number
	@param channelType 0 : data channel , 1 : control channel
*/
const int TransportHeader::getChannel(const int channelType)
{
	return interleaved[channelType];
}

/** @brief TransportHeader(rtsp header)�� rtp over tcp interleaved channel ���� �����Ѵ�.
	@return ����.
	@param channelType 0 : data channel , 1 : control channel
	@param channel ������ channel ��
*/
void TransportHeader::setChannel(const int channelType, const int channel)
{
	interleaved[channelType] = channel;
}

/** @brief TransportHeader(rtsp header)�� destination ���� �����´�.
	@return destination
	@param ����.
*/
const std::string TransportHeader::getDestination()
{
	return destination;
}

/** @brief TransportHeader(rtsp header)�� destination ���� �����Ѵ�.
	@return ����.
	@param dest ������ destination ��
*/
void TransportHeader::setDestination(const std::string dest)
{
	destination = dest;
}

/** @brief TransportHeader(rtsp header)�� ttl ���� �����´�.
	@return ttl
	@param ����.
*/
const int TransportHeader::getTTL()
{
	return ttl;
}

/** @brief TransportHeader(rtsp header)�� ttl ���� �����Ѵ�.
	@return ����.
	@param ttl ������ ttl ��
*/
void TransportHeader::setTTL(const int ttl)
{
	this->ttl = ttl;
}

/** @brief TransportHeader(rtsp header)�� transport-spec ���� �Ľ��Ѵ�.
	@return E_FAIL or S_OK
	@param szToken �Ľ���  string
*/
HRESULT TransportHeader::parseTransportSpec(char* szToken)
{
	if(szToken == NULL)
	{
		LOGE_LINE("ERROR");
		return E_FAIL;
	}

	if(strstr(szToken, "RTP/AVP") != szToken)
	{
		LOGE("ERROR", "%s(%d) %s: Wrong Transport-spec: %s", __CALL_INFO__, szToken);
		return E_FAIL;
	}

	if(szToken[7] == '\0')
	{
		// Default lower-transport is udp;
		setLowerTransport(eLT_UDP);
	}
	else if(szToken[7] == '/')
	{
		const char* szLowerTransport = szToken + 8;

		int i;
		int nLowerTransport = eLT_TCP;
		for(i=0; i<2; i++, nLowerTransport++)
		{
			if( strstr(szLowerTransport, LOWER_TRANSPORT_TABLE[nLowerTransport]) )
			{
				setLowerTransport(nLowerTransport);
				break;
			}
		}

		if(i == 2)
		{
			LOGE("ERROR", "%s(%d) %s: Wrong Transport-spec: %s", __CALL_INFO__, szToken);
			return E_FAIL;
		}
	}
	else
	{
		LOGE("ERROR", "%s(%d) %s: Wrong Transport-spec: %s", __CALL_INFO__, szToken);
		return E_FAIL;
	}

	return S_OK;
}

/**
 * Note that szValue contains only Transport Header's value part.
 */
/** @brief TransportHeader(rtsp header)�� �Ľ��Ѵ�.
	@return E_FAIL or S_OK
	@param szHeaderValue �Ľ���  string
*/
HRESULT TransportHeader::parse(char* szHeaderValue)
{
	HRESULT hr;

	int i;
	char* szNext;
	char* szToken;
	char* szKey;
	char* szValue;
	szToken = strtok_r(szHeaderValue, ";", &szNext);

	hr = parseTransportSpec(szToken);
	if(FAILED(hr))
	{
		LOGE_LINE("ERROR");
		return E_FAIL;
	}

	// Default cast type
	setCastType(eCT_UNICAST);

	szToken = ::strtok_r(NULL, ";", &szNext);
	while(szToken != NULL)
	{
		szKey = szToken;

		szValue = strchr(szToken, '=');
		if(szValue)
		{
			// Make szValue start from the next character of '='.
			szValue[0] = '\0';
			szValue++;
		}

		i = getParameterIndex(szKey);

		switch(i)
		{
		case eINTERLEAVED:
			hr = parsePortNumbers(szValue, &interleaved[eIC_DATA], &interleaved[eIC_CONTROL]);
			if(FAILED(hr))
			{
				LOGE_LINE("ERROR");
				return E_FAIL;
			}
			fieldMask->setFlags(eTHFM_INTERLEAVED_DATA | eTHFM_INTERLEAVED_CONTROL, TRUE);
			break;
		case eUNICAST:
			setCastType(eCT_UNICAST);
			break;
		case eMULTICAST:
			setCastType(eCT_MULTICAST);
			break;
		case eDESTINATION:
			setDestination(std::string(szValue));
			fieldMask->setFlags(eTHFM_DESTINATION, TRUE);
			break;
		case eTTL:
			setTTL(atoi(szValue));
			fieldMask->setFlags(eTHFM_TTL, TRUE);
			break;
		case ePORT:
			hr = parsePortNumbers(szValue, &anPort[PORT_RTP], &anPort[PORT_RTCP]);		
			if(FAILED(hr))
			{
				LOGE_LINE("ERROR");
				return E_FAIL;
			}
			fieldMask->setFlags(eTHFM_RTP_PORT | eTHFM_RTCP_PORT, TRUE);
			break;
		case eCLIENT_PORT:
			hr = parsePortNumbers(szValue, &anPort[CLIENT_PORT_RTP], &anPort[CLIENT_PORT_RTCP]);
			if(FAILED(hr))
			{
				LOGE_LINE("ERROR");
				return E_FAIL;
			}
			fieldMask->setFlags(eTHFM_CLIENT_RTP_PORT | eTHFM_CLIENT_RTCP_PORT, TRUE);
			break;
		case eSERVER_PORT:
			hr = parsePortNumbers(szValue, &anPort[SERVER_PORT_RTP], &anPort[SERVER_PORT_RTCP]);
			if(FAILED(hr))
			{
				LOGE_LINE("ERROR");
				return E_FAIL;
			}
			fieldMask->setFlags(eTHFM_SERVER_RTP_PORT | eTHFM_SERVER_RTCP_PORT, TRUE);
			break;
		case eSSRC:
			stringToHex(szValue, 8, (unsigned char*) &ssrc, sizeof(ssrc));
			fieldMask->setFlags(eTHFM_SSRC, TRUE);
			break;
		case eMODE:
			mode = ::GetMode(szValue);
			if(mode != eRTHM_UNKNOWN)
			{
				fieldMask->setFlags(eTHFM_MODE, TRUE);
			}
			break;
		}

		szToken = ::strtok_r(NULL, ";", &szNext);
	}

	return S_OK;
}

/** @brief TransportHeader(rtsp header)�� �����Ѵ�.
	@return ����� TransportHeader
	@param ����.
*/
Header* TransportHeader::duplicate() const
{
	TransportHeader* pHeader = new TransportHeader();

	memcpy(pHeader->anPort, anPort, sizeof(anPort));

	pHeader->setName(name);
	pHeader->setIntValue(integerValue);
	pHeader->setStringValue(stringValue);
	pHeader->ssrc = ssrc;

	return pHeader;
}

/**
 * Write header information into a buffer.
 */
/** @brief TransportHeader string ���� �����.
	@return E_FAIL or S_OK
	@param szBuf string�� ������ ����
	@param nBufSize string�� ������ ������ ũ��
*/
HRESULT TransportHeader::toString(char* szBuf, int nBufSize)
{
//	if(getDestination() != "")
//	{
//		const char* szName = rtspGetHeaderName(name);
//		if(szName == NULL)
//		{
//			return E_FAIL;
//		}
//
//		char szText[128];
//		const char* szParameter;
//		int nType;
//
//		const char* szLowerTransport = getLowerTransportName();
//		sprintf(szBuf, "%s: %s/%s/%s", szName, "RTP", "AVP", szLowerTransport);
//
//		// Cast type
//		if(fieldMask->getFlags(eTHFM_CAST_TYPE))
//		{
//			szParameter = getCastTypeName();
//
//			sprintf(szText, ";%s", szParameter);
//			strcat(szBuf, szText);
//		}
//
//		// destination
//		if(fieldMask->getFlags(eTHFM_TTL))
//		{
//			nType = PORT_RTP;
//
//			//sprintf(szText, ";port=%d-%d", anPort[nType], anPort[nType+1]);
//			sprintf(szText, ";destination=%s", getDestination().c_str());
//			strcat(szBuf, szText);
//		}
//
//		// Port
//		if(fieldMask->getFlags(eTHFM_RTP_PORT | eTHFM_RTCP_PORT))
//		{
//			nType = PORT_RTP;
//
//			//sprintf(szText, ";port=%d-%d", anPort[nType], anPort[nType+1]);
//			sprintf(szText, ";port=%d", anPort[nType]);
//			strcat(szBuf, szText);
//		}
//
//		// ttl
//		if(fieldMask->getFlags(eTHFM_TTL))
//		{
//			nType = PORT_RTP;
//
//			//sprintf(szText, ";port=%d-%d", anPort[nType], anPort[nType+1]);
//			sprintf(szText, ";ttl=%d", getTTL());
//			strcat(szBuf, szText);
//		}
//		strcat(szBuf, "\r\n");
//
//		return S_OK;
//	}
//	else
//	{
		const char* szName = rtspGetHeaderName(name);
		if(szName == NULL)
		{
			return E_FAIL;
		}

	#if 0
		sprintf(szBuf,
			"%s: %s/%s/%s;unicast;"
			"client_port=%d-%d;"
			"server_port=%d-%d;"
			"ssrc=%s;mode=%s\r\n",
			GetHeaderName(m_nName),
			"RTP", "AVP", "UDP",
			m_anPort[CLIENT_PORT_RTP], m_anPort[CLIENT_PORT_RTCP],
			m_anPort[SERVER_PORT_RTP], m_anPort[SERVER_PORT_RTCP],
			szSSRC, "play");
	#else
		char szText[128];
		const char* szParameter;
		int nType;

	#if 1
		// Transport-protocol / profile / lower-transport
		const char* szLowerTransport = getLowerTransportName();
		sprintf(szBuf, "%s: %s/%s/%s", szName, "RTP", "AVP", szLowerTransport);
	#elif 1
		sprintf(szBuf, "%s: %s/%s/%s", szName, "RTP", "AVP", "UDP");
	#else
		sprintf(szBuf, "%s: %s/%s", szName, "RTP", "AVP");
	#endif

		// Cast type
		if(fieldMask->getFlags(eTHFM_CAST_TYPE))
		{
			szParameter = getCastTypeName();

			sprintf(szText, ";%s", szParameter);
			strcat(szBuf, szText);
		}

		// destination
		if(fieldMask->getFlags(eTHFM_DESTINATION))
		{
			sprintf(szText, ";destination=%s", getDestination().c_str());
			strcat(szBuf, szText);
		}

		// Port
		if(fieldMask->getFlags(eTHFM_RTP_PORT | eTHFM_RTCP_PORT))
		{
			nType = PORT_RTP;
			sprintf(szText, ";port=%d", anPort[nType]);
			strcat(szBuf, szText);
		}

		// Client Port
		if(fieldMask->getFlags(eTHFM_CLIENT_RTP_PORT | eTHFM_CLIENT_RTCP_PORT))
		{
			szParameter = "client_port";
			nType = CLIENT_PORT_RTP;

			//sprintf(szText, ";%s=%d-%d", szParameter, anPort[nType], anPort[nType+1]);
			sprintf(szText, ";%s=%d", szParameter, anPort[nType]);
			strcat(szBuf, szText);
		}

		// interleaved
		if(fieldMask->getFlags(eTHFM_INTERLEAVED_DATA | eTHFM_INTERLEAVED_CONTROL))
		{
			szParameter = "interleaved";
			nType = eIC_DATA;

			sprintf(szText, ";%s=%d-%d", szParameter, interleaved[eIC_DATA], interleaved[eIC_DATA+1]);
			strcat(szBuf, szText);
		}

		// ttl
		if(fieldMask->getFlags(eTHFM_TTL))
		{
			sprintf(szText, ";ttl=%d", getTTL());
			strcat(szBuf, szText);
		}

	#if 0
		// Source
		//if(m_pFieldMask->GetFlags(eTHFM_SOURCE))
		if(TRUE)
		{
			szParameter = "source";
			sprintf(szText, ";%s=127.0.0.1", szParameter);
			strcat(szBuf, szText);
		}
	#endif

		// Server Port
		if(fieldMask->getFlags(eTHFM_SERVER_RTP_PORT | eTHFM_SERVER_RTCP_PORT))
		{
			szParameter = "server_port";
			nType = SERVER_PORT_RTP;

			//sprintf(szText, ";%s=%d-%d", szParameter, anPort[nType], anPort[nType+1]);
			sprintf(szText, ";%s=%d", szParameter, anPort[nType]);
			strcat(szBuf, szText);
		}

	#if 1
		if(fieldMask->getFlags(eTHFM_SSRC))
		{
			szParameter = "ssrc";

			UINT nSSRC =
				((ssrc & 0xFF000000) >> 24) |
				((ssrc & 0x00FF0000) >> 8 ) |
				((ssrc & 0x0000FF00) << 8 ) |
				((ssrc & 0x000000FF) << 24);

			char szSSRC[9];
			hexToString((const BYTE*) &nSSRC, sizeof(ssrc), szSSRC, 8);
			szSSRC[8] = '\0';

			LOGV("HEADER", "%s:%d SSRC=%s", __FILENAME__, __LINE__, szSSRC);

			sprintf(szText, ";%s=%s", szParameter, szSSRC);
			strcat(szBuf, szText);
		}
	#endif

	#if 1
		if(fieldMask->getFlags(eTHFM_MODE))
		{
			szParameter = "mode";

			sprintf(szText, ";%s=%s", szParameter, GetModeName(mode));
			strcat(szBuf, szText);
		}
	#endif

	#endif

		strcat(szBuf, "\r\n");

		return S_OK;
	//}
}
