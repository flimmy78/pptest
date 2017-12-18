#include <nomad.h>
#include "SDPMaker.h"

#include "base64.h"
#include "StringUtil.h"
#include "config.h"


/** @brief SDPMaker 생성자. sdp 정보를 만든다.
	@return 의미 없음.
	@param 없음.
*/
SDPMaker::SDPMaker() :
strDesc(""),
strHost(""),
duration(0),
payloadType(0)
{

}

/** @brief SDPMaker 소멸자. sdp 정보를 만든다.
	@return 의미 없음.
	@param 없음.
*/
SDPMaker::~SDPMaker()
{
}
/**
 * Unit: second
 *
 */
/** @brief duration 값을 return 한다.
	@return duration
	@param 없음.
*/
double SDPMaker::getDuration() const
{
	return duration;
}

/**
 * Unit: second
 *
 * @param nDuration
 */

/** @brief duration 값을 설정한다.
	@return 없음.
	@param fDuration 설정할 duration
*/
void SDPMaker::setDuration(double fDuration)
{
	duration = fDuration;
}

/** @brief payloadtype을 가져온다.
	@return payloadtype
	@param 없음.
*/
int SDPMaker::getPayloadType() const
{
	return payloadType;
}

/** @brief payloadtype을 설정한다.
	@return 없음.
	@param nPayloadType 설정할 payloadtype
*/
void SDPMaker::setPayloadType(int nPayloadType)
{
	payloadType = nPayloadType;
}

/** @brief host IP를 가져온다.
	@return host ip
	@param 없음.
*/
std::string SDPMaker::getHost() const
{
	return strHost;
}

/** @brief host IP를 설정한다.
	@return 없음.
	@param  strHost 설정할 host ip
*/
void SDPMaker::setHost(std::string strHost)
{
	strHost = strHost;
}

/** @brief sdp 정보를 string 형태로 return 한다.
	@return string sdp 정보
	@param  없음.
*/
const char* SDPMaker::toString() const
{
	return strDesc.c_str();
}

/** @brief sdp 정보의 길이를 return 한다.
	@return string sdp 정보의 길리
	@param  없음.
*/
int SDPMaker::getLength()
{
	return strDesc.length();
}

/**
 * @brief run to make a sdp description with given information. 
 *
 * @return success of fail
 */
/** @brief 설정된 sdp 정보를 가지고 string 형태로 만든다.
	@return 없음.
	@param  없음.
*/
HRESULT SDPMaker::run()
{
	char szBuf[256];

	// General
	strDesc.append("v=0\r\n");
	strDesc.append(
		"o=- 1386658563105026 1 IN IP4 127.0.0.1\r\n");
	strDesc.append("c=IN IP4 0.0.0.0\r\n");

#if 0
	sprintf(szBuf, "b=AS:%d\r\n", MAX_AUDIO_BANDWIDTH + MAX_VIDEO_BANDWIDTH);
	m_strDesc.append(szBuf);
#endif
	strDesc.append("t=0 0\r\n");
	strDesc.append("a=recvonly\r\n");
	strDesc.append("a=type:broadcast\r\n");
	strDesc.append("a=charset:UTF-8\r\n");
	if(duration > 0.0)
	{
		sprintf(szBuf, "a=range:npt=0-%.4f\r\n", duration);
		strDesc.append(szBuf);
	}
	else
	{
		strDesc.append("a=range:npt=0-\r\n");
	}

	strDesc.append("a=control:*\r\n");	

	sprintf(szBuf, "m=video 0 RTP/AVP %d\r\n", payloadType);
		
	strDesc.append(szBuf);
#if 1
	//sprintf(szBuf, "b=AS:%d\r\n", MAX_VIDEO_BANDWIDTH);
	sprintf(szBuf, "b=AS:%d\r\n", 5000);
	strDesc.append(szBuf);
#endif
	
	sprintf(szBuf, "a=rtpmap:%d H264/90000\r\n", payloadType);
	strDesc.append(szBuf);
	
	//int nSize;
	char szData[128];
	std::string strData;
	
	if(videoInfo->spsData && videoInfo->ppsData)
	{
		// profile-level-id
		hexToString(videoInfo->spsData+1, 3, szData, 6);
		szData[6] = '\0';
		sprintf(szBuf, "a=fmtp:%d packetization-mode=1;profile-level-id=%s", payloadType, szData);
		strDesc.append(szBuf);

		// sequence parameter set
		strData = base64_encode((unsigned char const*)videoInfo->spsData, (unsigned int)videoInfo->spsSize);
		sprintf(szBuf, ";sprop-parameter-sets=%s", strData.c_str());
		strDesc.append(szBuf);

		// picture parameter set
		strData = base64_encode((unsigned char const*)videoInfo->ppsData, (unsigned int)videoInfo->ppsSize);
		sprintf(szBuf, ",%s\r\n", strData.c_str());
		strDesc.append(szBuf);
	}

	strDesc.append("a=control:track0\r\n");
	if(USE_AUDIO)
	{
		if(AUDIO_TYPE == eAT_AAC)
		{
			strDesc.append("m=audio 0 RTP/AVP 97\r\n");
			//b=RR:0;
			strDesc.append("a=rtpmap:97 mpeg4-generic/44100/2\r\n");
			strDesc.append("a=fmtp:97 streamtype=5; profile-level-id=15; mode=AAC-hbr; config=1210; SizeLength=13; IndexLength=3; IndexDeltaLength=3; Profile=1;\r\n");
		}
		else if(AUDIO_TYPE == eAT_PCMU)
		{
			strDesc.append("m=audio 0 RTP/AVP 0\r\n");
		}
		else if(AUDIO_TYPE == eAT_PCMA)
		{
			strDesc.append("m=audio 0 RTP/AVP 8\r\n");
		}
		strDesc.append("a=control:track1\r\n");
	}


	return S_OK;
}

/** @brief 설정된 정보들을 clear 한다.
	@return 없음.
	@param  없음.
*/
void SDPMaker::clear()
{
	strDesc.clear();
	duration = 0;
	payloadType = 0;
	strHost = "";
}

void SDPMaker::setVideoInfo(VideoInfo videoInfo)
{
	this->videoInfo = &videoInfo;
}