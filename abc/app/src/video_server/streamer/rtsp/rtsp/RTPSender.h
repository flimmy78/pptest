#ifndef __RTP_SENDER_H__
#define __RTP_SENDER_H__

#include <netinet/in.h>
#include <map>
#include <vector>

class CAACParser;

typedef struct _SRTPHeader
{
	uint16_t csrcCount:4;
	uint16_t extension:1;
	uint16_t padding:1;
	uint16_t version:2;
	uint16_t payloadType:7;
	uint16_t marker:1;
	uint16_t sequenceNumber;
	uint32_t timestamp;
	uint32_t ssrc;
}SRTPHeader;

typedef struct _SRTPOverTcpHeader
{
	uint16_t magic:8;
	uint16_t channel:8;
	uint16_t length;
}SRTPOverTcpHeader;

class RTPSender
{
protected:	
	SOCKET udpSocketFD;
	SOCKET tcpSocketFD;
	SRTPHeader* rtpHeader;
	SRTPOverTcpHeader* rtpOverTcpHeader;
	sockaddr_in clientAddr;

	
	int bufferSize;
	BYTE* buffer;

//	int streamType;
	int sampleRate;
//	int channels;

	int interleavedData;
	int interleavedControl;

    /**
	 * The time when the latest packet was sent.
	 */
	UINT latestTimeStamp;
	LONGLONG syncTime;

	uint16_t sequenceNumber;
	uint16_t sequenceNumberAudio;

	FILE* audioFp;
	CAACParser* aacParser;
	uint64_t sendAudioFrameCount;
	uint64_t firstSampleTime;
	uint64_t sampleTime;


	// JACOB_20160422
	#if 0
	FILE *audioSavefp;
	#endif


	

protected:
	HRESULT wait(UINT timeStamp);

public:
	RTPSender(void);
	virtual ~RTPSender(void);

	UINT getSSRC() const;
	void setSSRC(UINT ssrc);

	HRESULT initialize();
	HRESULT setConfiguration(int streamType, int payloadType, int sampleRate, int channels);
	HRESULT open(int payloadType, char *host, int srcPort, int dstPort, BOOL blockingMode);
	HRESULT reset();
	HRESULT setSync(UINT syncTimeStamp);
	HRESULT close();
	HRESULT finalize();

	int send(const BYTE* sendBuffer, int length, int markerBit, UINT sendTime, bool video=true);

	int getSequence();
	UINT getRTPTime();

	void setSocket(SOCKET rtspSocket, SOCKET rtpSocket);
	void setInterleavedChannel(int dataChannel, int controlChannel);

	void sendAudio(uint64_t sendTime);
};

#endif
