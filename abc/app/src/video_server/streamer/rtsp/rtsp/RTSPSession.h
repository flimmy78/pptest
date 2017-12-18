#ifndef __RTSP_SESSION_H__
#define __RTSP_SESSION_H__

#include <string>
#include <list>
#include <pthread.h>

#include "nomad_windef.h"

void *gRTSPSessionThreadProc( void * );


class RTPSender;
class ThreadSafe;

typedef struct _NALUNIT
{
	int32_t length;
	uint8_t* data;
	uint64_t recvTime;
} NALUNIT;

class RTSPSession
{
public:
	struct SParam
	{
		std::string token;
		std::string strSessionID;
		std::string strIP;

		int transportType;
		SOCKET rtspFD;
		SOCKET rtpFD;
		int interleavedData;
		int interleavedControl;
		int dstRtpPort;
		int dstRtcpPort;
		int srcRtpPort;
		int srcRtcpPort;
		UINT SSRC;
		int payloadType;
		int streamType;

		int sampleRate;
		int channel;
	};
	RTSPSession(std::string playFilename);
	~RTSPSession(void);

	void setParam(SParam* param);
	int getSequence();

	int getTimeout() const;
	void setTimeout(int nTimeout);

	long getLastRequestTime();
	void setLastRequestTime(long requestTime);

	const char* getID() const;
	const char* getToken() const;
	const int getTransportType() const;

	/**
	 * These method is associated with RTSP_STATE enumeration.
	 */
	UINT getState();
	void setState(UINT nState);

	UINT getPlayState();
	void setPlayState(UINT nState);

	void clear();

	const std::string getFilename();
	void setFilename(const std::string filename);

	void setSeekPosition(const float seekPos);
	const float getSeekPosition(void);

	void setScalerId(const int id);
	const int getScalerId(void);

	void setRtpInit(const bool init);
	const bool getRtpInit(void);

	const UINT getNptTime();
	const UINT getRtpTime();
	void setRtpTime(const UINT time);
	void setSync(const UINT syncTimeStamp);
	void sendData();

	void setScaleValue(int value);
	void setData(const uint8_t* ts_buffer, size_t read_size, uint64_t recvTime);
	void sendFragmentUnit(uint8_t* pBuffer, int nSize, int nMarkerBit, UINT nTimeStamp);

	uint getNalCount();
private:

	std::string playFilename;
	pthread_t thread;
	UINT state;
	UINT playState;

	RTPSender* rtpSender;

	long lastRequestTime;

	/**
	 * Unit: second
	 */
	int timeout;
	SParam* param;

	float seekPosition;
	UINT rtpTime;

	int scalerId;
	bool rtpInit;
	int scaleValue;
	uint64_t rtp_time;	
	uint64_t firstTime;
	ThreadSafe* threadSafe;
	std::list<NALUNIT> nalList;
};

#endif // __RTSP_SESSION_H__
