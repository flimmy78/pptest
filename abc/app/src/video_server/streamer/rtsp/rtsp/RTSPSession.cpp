#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "nomad.h"
#include "RTSPSession.h"
#include "config.h"
#include "RTPSender.h"
#include "TimeUtil.h"
#include "ThreadSafe.h"
#include "NALUnitTokenizer.h"
#include <nomad_log.h>

#include "rtsp.h"

static const char* TAG = "RTSP";
#define RTP_PACKETS_SIZE 1400


/** @brief ���� ���°� playing �ϰ��� rtp data�� client socket�� ������.
	@return �ǹ� ����.
	@param ptr RTSPSession
*/
void *gRTSPSessionThreadProc(void *ptr)
{
	RTSPSession *rtspSession = (RTSPSession*) ptr;
	while(rtspSession->getState() != eRS_INIT)
	{
		if(rtspSession->getState() == eRS_PLAYING && rtspSession->getNalCount() > 0)
		{


			rtspSession->sendData();


		}
		else
		{
			GW::msleep(10);
		}
	}
	return (void*)NULL;
}

/** @brief RTSPSession ������. rtpsender�� ���� rtp data�� client socket�� ������.
	@return �ǹ� ����.
	@param filename client�� play�� filename
*/
RTSPSession::RTSPSession(std::string filename) :
playFilename(filename),
state(eRS_READY),
playState(eRPS_BOF),
rtpSender(new RTPSender()),
lastRequestTime(0),
timeout(DEFAULT_RTSP_TIMEOUT),
param(NULL),
seekPosition(0.0f),
rtpTime(0),
scaleValue(10),
rtp_time(0)
{
	firstTime = 0;
	rtpInit = false;

	rtpSender->initialize();

	threadSafe = new ThreadSafe();

	nalList.clear();

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	if(pthread_create(&thread, &attr, gRTSPSessionThreadProc, (void*) this))
	{
		LOGE(TAG, "%s(%d) %s: pthread create fail.", __CALL_INFO__);
	}
	pthread_attr_destroy(&attr);
}

/** @brief RTSPSession ������. rtpsender�� ���� rtp data�� client socket�� ������.
	@return �ǹ� ����.
	@param ����.
*/
RTSPSession::~RTSPSession(void)
{
	setState(eRS_INIT);
	pthread_join(thread, (void**)NULL);
	if(param) delete param;

	rtpSender->finalize();
	delete rtpSender;

	//delete[] readData;

	if(threadSafe) delete threadSafe;

	uint count = nalList.size();
	if (count > 0)
	{
		for(uint i=0; i<count; i++)
		{
			NALUNIT nal = (NALUNIT)nalList.front();
			if(nal.data) free(nal.data);
			nalList.pop_front();

		}
	}
}

/** @brief parameter�� �����Ѵ�. ���� �����͸� ���� rtpsender�� �Ķ����͸� ������ �ش�.
	@return �ǹ� ����.
	@param param rtspsession�� parameter
*/
void RTSPSession::setParam(SParam* param)
{
	if(this->param) delete this->param;
	this->param = param;

	rtpSender->setSSRC(param->SSRC);
	rtpSender->setConfiguration(param->streamType, param->payloadType, param->sampleRate, param->channel);

	rtpSender->setSocket(param->rtspFD, param->rtpFD);
	rtpSender->setInterleavedChannel(param->interleavedData, param->interleavedControl);

	const BOOL bBlockingMode = TRUE;
	rtpSender->open(param->payloadType, (char*)param->strIP.c_str(), param->srcRtpPort, param->dstRtpPort, bBlockingMode);
}

/** @brief rtp sequence number�� �����´�.
	@return rtp sequence number
	@param ����.
*/
int RTSPSession::getSequence()
{
	return rtpSender->getSequence();
}

/** @brief rtspsession�� ���� ���¸� �����Ѵ�.
	@return ����.
	@param nState ������ ���� (READY, PLAYING)
*/
void RTSPSession::setState(UINT nState)
{
	LOGD_CALL(TAG, "START");
	printf("nState : %d\n", nState);

	if(nState == eRS_READY)
	{
		setPlayState(eRPS_PAUSE);
	}
	else if(nState == eRS_PLAYING)
	{
		setPlayState(eRPS_PLAY);
		rtpSender->reset();
	}
	state = nState;

	LOGD_CALL(TAG, "END");
}

/** @brief rtspsession�� play ���¸� �����´�.
	@return play ���� (PLAY, PAUSE)
	@param ����.
*/
UINT RTSPSession::getPlayState()
{
	return playState;
}

/** @brief rtspsession�� play ���¸� �����Ѵ�..
	@return ����.
	@param nState play ���� (PLAY, PAUSE)
*/
void RTSPSession::setPlayState(UINT nState)
{
	playState = nState;
}

/** @brief rtspsession�� ���� ���¸� �����´�.
	@return ���� ���� (READY, PLAYING)
	@param ����.
*/
UINT RTSPSession::getState()
{
	return state;
}

/** @brief rtspsession�� ���¸� �ʱ�ȭ�Ѵ�.
	@return ����.
	@param ����.
*/
void RTSPSession::clear()
{
	setState(eRS_INIT);
}

/** @brief rtspsession�� time out �ð��� �����´�.
	@return time out �ð���
	@param ����.
*/
int RTSPSession::getTimeout() const
{
	return timeout;
}

/** @brief rtspsession�� time out �ð��� �����Ѵ�.
	@return ����.
	@param nTimeout ������ time out �ð���
*/
void RTSPSession::setTimeout(int nTimeout)
{
	timeout = nTimeout;
}

/** @brief rtspsession�� session id�� �����´�.
	@return session id
	@param ����.
*/
const char* RTSPSession::getID() const
{
	return param->strSessionID.c_str();
}

/** @brief rtspsession�� token�� �����´�.
	@return token
	@param ����.
*/
const char* RTSPSession::getToken() const
{
	return param->token.c_str();
}

/** @brief rtspsession�� transport type�� �����´�.
	@return transport type
	@param ����.
*/
const int RTSPSession::getTransportType() const
{
	return param->transportType;
}

/** @brief rtspsession�� last request time�� �����´�.
	@return last request time
	@param ����.
*/
long RTSPSession::getLastRequestTime()
{
	return lastRequestTime;
}

/** @brief rtspsession�� last request time�� �����Ѵ�.
	@return ����.
	@param requestTime ������ last request time
*/
void RTSPSession::setLastRequestTime(long requestTime)
{
	lastRequestTime = requestTime;
}

/** @brief rtspsession�� ���� play���� filename�� �����´�.
	@return play ���� filename
	@param ����.
*/
const std::string RTSPSession::getFilename()
{
	return playFilename;
}

/** @brief rtspsession�� play�� filename�� �����Ѵ�.
	@return ����.
	@param filename play �� filename
*/
void RTSPSession::setFilename(const std::string filename)
{
	playFilename = filename;
}

/** @brief ���� play���� ������ seek ��ġ�� �����Ѵ�.
	@return ����.
	@param seekPos play���� file�� seek ��ġ
*/
void RTSPSession::setSeekPosition(const float seekPos)
{
	threadSafe->lock();
	seekPosition = seekPos;

	//Status success = parser.seekStream(seekPos);
	//parser.seekStream(seekPos);

	LOGD(TAG, "%s(%d) %s: rtp_time(%llu)", __CALL_INFO__, rtp_time);
	threadSafe->unlock();
}

/** @brief ���� play���� ���Ͽ��� seek�� ��ġ�� �����Ѵ�.
	@return seek ��ġ
	@param ����.
*/
const float RTSPSession::getSeekPosition(void)
{
	return seekPosition;
}

/** @brief ���� play���� ������ npt time�� �����´�.
	@return npt time
	@param ����.
*/
const UINT RTSPSession::getNptTime()
{
	//return parser.getPosition();
	return 0;
}

void RTSPSession::setScalerId(const int id)
{
	scalerId = id;
}

const int RTSPSession::getScalerId(void)
{
	return scalerId;
}

void RTSPSession::setRtpInit(const bool init)
{
	rtpInit = init;
}

const bool RTSPSession::getRtpInit(void)
{
	return rtpInit;
}

/** @brief ���� play���� ������ rtp time�� �����´�.
	@return rtp time
	@param ����.
*/
const UINT RTSPSession::getRtpTime()
{
	return rtpTime;
}

/** @brief ���� play���� ������ rtp time�� ���� �����Ѵ�.
	@return ����.
	@param time ���� ������ rtp time
*/
void RTSPSession::setRtpTime(const UINT time)
{
	rtpTime = time;
}

/** @brief ���� play���� ���Ͽ��� ���� �ð����� rtp time�� sync�Ѵ�.
	@return ����.
	@param syncTimeStamp ���� sync�� time stamp
*/
void RTSPSession::setSync(const UINT syncTimeStamp)
{
	rtpSender->setSync(syncTimeStamp);
}



/** @brief ���� data�� ���� callback �Լ��� �����Ѵ�. callback�Լ� ȣ���� ������ ts parser�ʿ��� ȣ���Ѵ�.
	@return ����.
	@param ����.
*/
void RTSPSession::sendData()
{
	while(1)
	{
		if(nalList.size() == 0) return;

		threadSafe->lock();
		NALUNIT nal = nalList.front();
		nalList.pop_front();
		threadSafe->unlock();

		uint32_t nalSize = nal.length;
		uint8_t* nalPayload = nal.data;
		//LOGD(TAG, "%s(%d) %s: read_size(%u), rtp_time(%u)", __CALL_INFO__, nalSize, rtp_time);
#if 0
		struct timeval timeval;
		gettimeofday(&timeval, NULL);

		uint64_t currentTime = 0;
		uint64_t diffTime = 0;
		if(firstTime == 0) firstTime = currentTime = timeval.tv_sec * 1000 + timeval.tv_usec / 1000;
		currentTime = timeval.tv_sec * 1000 + timeval.tv_usec / 1000;
		diffTime = currentTime - firstTime;
		rtp_time = diffTime * 90;
#endif
		if(firstTime == 0) firstTime = nal.recvTime;
		rtp_time = nal.recvTime - firstTime;

		if(nalSize <= RTP_PACKETS_SIZE)
		{
			rtpSender->send(nalPayload+1, (int)(nalSize-1), 1, (UINT)rtp_time);
		}
		else
		{
			sendFragmentUnit(nalPayload+1, (int)(nalSize-1), 1, (UINT)rtp_time);
		}
		if(USE_AUDIO)
		{
			rtpSender->sendAudio(rtp_time);
		}

		if(nalPayload) free(nalPayload);
	}
}

/** @brief ���� data�� ������.
	@return ����.
	@param ts_buffer ���� data buffer
	@param read_size ���� data buffer size
*/
void RTSPSession::setData(const uint8_t* ts_buffer, size_t read_size, uint64_t recvTime)
{
#if 0
	NALUNIT nal;
	uint8_t* nalUnit = (uint8_t*)malloc(read_size + 1);
	memcpy(nalUnit, ts_buffer - 1, read_size + 1);
	nal.length = read_size + 1;
	nal.data = nalUnit;
	threadSafe->lock();
	nalList.push_back(nal);
	threadSafe->unlock();
#else
	NALUNIT nal;
	uint8_t* nalUnit = (uint8_t*)malloc(read_size + 1);
	memcpy(nalUnit, ts_buffer - 1, read_size + 1);
	nal.length = read_size + 1;
	nal.data = nalUnit;
	nal.recvTime = recvTime;
	threadSafe->lock();
	nalList.push_back(nal);
	threadSafe->unlock();
#endif
}

void RTSPSession::sendFragmentUnit(uint8_t* pBuffer, int nSize, int nMarkerBit, UINT nTimeStamp)
{
	const int FU_A = 28;

	int i;
	int nPayloadSize;
	int nNALRefIndex;
	int nNALType;
	int nMarkBit;
	bool bStart, bEnd;
	uint8_t* pPayload;

	const int nBlockSize = RTP_PACKETS_SIZE;

	nNALRefIndex = pBuffer[0] & 0x60;
	nNALType = pBuffer[0] & 0x1F;
	nSize--;	// Remove a nalunit type size.

	for(i=1; nSize>0; i+=nPayloadSize)
	{
		pPayload = pBuffer + i;

		// 12(rtp fixed header size) 2(fu header size)
		//nPayloadSize = min(nSize, nBlockSize - 12 - 2);
		nPayloadSize = min(nSize, nBlockSize - 2);
		nSize -= nPayloadSize;

		bStart = (i == 1);
		bEnd = (nSize == 0);
		nMarkBit = bEnd ? nMarkerBit : 0;

		// FU indicator
		*(pPayload - 2) = nNALRefIndex | FU_A;
		// FU header
		*(pPayload - 1) = (bStart << 7) | (bEnd << 6) | nNALType;

		// Don't confuse nMarkerBit and nMarkBit.
		//const BOOL bBlockingMode = CheckBlockingMode(nTimeStamp - m_nTimeStampBase);

		rtpSender->send(pPayload-2, nPayloadSize+2, nMarkBit, nTimeStamp);

		//LOGD(TAG, "%s(%d) %s: mark(%d) size(%d) ts(%d)", __CALL_INFO__, nMarkBit, nPayloadSize+2, nTimeStamp);
	}
}

/** @brief ������ �����Ѵ�.
	@return ����.
	@param value 10:1����, 20:2���� -:������
*/
void RTSPSession::setScaleValue(int value)
{
	threadSafe->lock();
	scaleValue = value;
	//parser.setScaleFactor(scaleValue);
	if(value == 10)
	{
		setPlayState(eRPS_PLAY);
	}
	else if(value > 10)
	{
		setPlayState(eRPS_FF);
	}
	else if(value < 0)
	{
		setPlayState(eRPS_RW);
	}

	threadSafe->unlock();
}

uint RTSPSession::getNalCount()
{
	return nalList.size();
}
