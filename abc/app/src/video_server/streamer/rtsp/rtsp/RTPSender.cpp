#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#include "TimeUtil.h"
#include "nomad.h"
#include "RTPSender.h"
#include "config.h"
#include "nomad_log.h"
#include "config.h"
#include "AACParser.h"

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>



static const char* TAG = "RTP";

// JACOB_20160422
#if 0
unsigned int gseq=0;
#endif


#define  KEY_NUM     9527
#define  MEM_SIZE    480


int   shm_id;
void *shm_addr;

int   bFirst = 1;





/** @brief RTPSender 생성자. 실제로 rtp data를 보내는 루틴 수행
	@return 의미 없음.
	@param 없음.
*/
RTPSender::RTPSender(void) :
udpSocketFD(0),
tcpSocketFD(0)
{
	rtpHeader = new SRTPHeader;
	rtpHeader->csrcCount = 0;
	rtpHeader->extension = 0;
	rtpHeader->padding = 0;
	rtpHeader->version = 2;
	//rtpHeader->sequenceNumber = 0;

	rtpOverTcpHeader = new SRTPOverTcpHeader;

	bufferSize = 1500;
	buffer = new BYTE [bufferSize];	

	sequenceNumber = 0;
	sequenceNumberAudio = 0;
	sendAudioFrameCount = 0;
	firstSampleTime = 0;
	sampleTime = 0;
	
	if(USE_AUDIO)
	{

		#if 0	// JACOB_20160520
		if(AUDIO_TYPE == eAT_AAC)
		{
			audioFp = fopen("/root/video_server/mbc.aac", "rb");
			aacParser = new CAACParser(256 * 1024);
		}
		else if(AUDIO_TYPE == eAT_PCMU)
		{
			audioFp = fopen("/root/video_server/sample_8k_mono-s.ulaw", "rb");
		}
		else if(AUDIO_TYPE == eAT_PCMA)
		{
			audioFp = fopen("/root/video_server/sample_8k_mono-s.alaw", "rb");
		}
		#endif

		#if 1	// JACOB_20160425


		if (bFirst )
		{
			bFirst = 0;
		
			if ( -1 == ( shm_id = shmget( (key_t)KEY_NUM, MEM_SIZE, IPC_CREAT|0666)))
			   printf( "share mem create fail...\n");


			printf("[RTP Sender id= %d]\n", shm_id);
	
			if ( ( void *)-1 == ( shm_addr = (void *)shmat( shm_id, ( void *)0, 0)))
			   printf( "share mem add fail...\n");

			printf("[RTP Sender addr= 0x%x]\n", shm_addr);

		}	
		#endif






		
	}
	else
	{
		audioFp = NULL;
		aacParser = NULL;
	}

}

/** @brief RTPSender 소멸자.
	@return 의미 없음.
	@param 없음.
*/
RTPSender::~RTPSender(void)
{
	finalize();
	if(rtpHeader) delete rtpHeader;
	if(rtpOverTcpHeader) delete rtpOverTcpHeader;

	if(buffer)
	{
		delete[] buffer;
	}


#if 0	// JACOB_20160520

	if(USE_AUDIO)
	{
		if(AUDIO_TYPE == eAT_AAC)
		{
			if(audioFp)
			{
				fclose(audioFp);
				audioFp = NULL;
			}

			delete aacParser;
		}
		else if(AUDIO_TYPE == eAT_PCMU)
		{
			if(audioFp)
			{
				fclose(audioFp);
				audioFp = NULL;
			}
		}
		else if(AUDIO_TYPE == eAT_PCMA)
		{
			if(audioFp)
			{
				fclose(audioFp);
				audioFp = NULL;
			}
		}		
	}
#endif

#if 0	// JACOB_20160520
	if(shm_addr)shmdt(shm_addr);
	shmctl( shm_id, IPC_RMID, 0);
	shm_id = -1;
#endif


}

/** @brief ssrc 값을 가져온다.
	@return ssrc
	@param 없음.
*/
UINT RTPSender::getSSRC() const
{
	return rtpHeader->ssrc;
}

/** @brief ssrc 값을 설정한다.
	@return 없음.
	@param ssrc 설정할 ssrc
*/
void RTPSender::setSSRC(UINT ssrc)
{
	rtpHeader->ssrc = ssrc;
}

/** @brief RTPSender를 초기화 한다. udp socket을 생성한다.
	@return E_FAIL or S_OK
	@param 없음.
*/
HRESULT RTPSender::initialize()
{

    LOGD_CALL(TAG, "START");


	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0)
	{
		ASSERT(FALSE);
		return E_FAIL;
	}

	udpSocketFD = sock;

    LOGD_CALL(TAG, "END");

	return S_OK;
}

/** @brief RTPSender를 마무리 한다. udp socket을 닫는다.
	@return E_FAIL or S_OK
	@param 없음.
*/
HRESULT RTPSender::finalize()
{
    LOGD_CALL(TAG, "START");

	if(udpSocketFD != 0)
	{
		::close(udpSocketFD);
		udpSocketFD = 0;
	}

    LOGD_CALL(TAG, "END");

	return S_OK;
}

/** @brief RTPSender에서 알아야 할 값들을 설정한다.
	@return E_FAIL or S_OK
	@param streamType
	@param payloadType rtp header의 payload type
	@param sampleRate stream의 samplerate
	@param channels
*/
HRESULT RTPSender::setConfiguration(int streamType, int payloadType, int sampleRate, int channels)
{
	LOGD(TAG, "%s(%d) %s: [START] type(%d) payloadtype(%d) samplerate(%d) channels(%d)",
		__CALL_INFO__, streamType, payloadType, sampleRate, channels);
	
	rtpHeader->payloadType = payloadType;
	this->sampleRate = sampleRate;	

	LOGD_CALL(TAG, "[END]");

	return S_OK;
}

/** @brief RTPSender의 시간 정보를 초기화 한다.
	@return S_OK
	@param 없음.
*/
HRESULT RTPSender::reset()
{
	LOGD_CALL(TAG, "[START]");

	syncTime = 0LL;
	latestTimeStamp = 0;

	LOGD_CALL(TAG, "[END]");

	return S_OK;
}

/** @brief RTPSender의 시간 정보를 현재 시간과 동기화 한다.
	@return S_OK
	@param syncTimeStamp 현재 시간과 동기화 해야 하는 마지막 timestamp
*/
HRESULT RTPSender::setSync(UINT syncTimeStamp)
{
	syncTime = GW::getTimeMicroSecond();
	latestTimeStamp = syncTimeStamp;
	return S_OK;
}

/**
 * Open RTPHandler.
 *
 * @param nPayloadType
 * @param pHost
 * @param nSrcPort source port of a socket
 * @param nDstPort destination port of a socket
 * @return
 */
// 사용안함.
HRESULT RTPSender::open(int payloadType, char *host, int srcPort, int dstPort, BOOL blockingMode)
{
	LOGD(TAG, "%s(%d) %s: [START] m_nSampleRate(%d) payloadtype(%d) host(%s) srcport(%d) dstport(%d)", __FILENAME__, __LINE__, __FUNCTION__, sampleRate, payloadType, host, srcPort, dstPort);

	if(udpSocketFD == 0)
	{
		ASSERT(FALSE);
		return E_FAIL;
	}

	memset(&clientAddr, 0, sizeof(sockaddr_in));
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_addr.s_addr = inet_addr(host);
	clientAddr.sin_port = htons(dstPort);

	rtpHeader->payloadType = payloadType;
	syncTime = 0;
	sequenceNumber = 0;
	sequenceNumberAudio = 0;
	latestTimeStamp = 0;

	LOGD_CALL(TAG, "[END]");

	return S_OK;
}

// 사용안함.
HRESULT RTPSender::close()
{
    LOGD_CALL(TAG, "START");

    LOGD_CALL(TAG, "END");

	return S_OK;
}

/** @brief 실제로 데이터를 socket을 통해서 보낸다.
	@return 보낸 data 의 크기
	@param sendBuffer 보낼 버퍼
	@param nLength 보낼 버퍼 크기
	@param nMarkerBit marker bit
	@param sendTime 보내야 할 시간.
*/
int RTPSender::send(const BYTE* sendBuffer, int nLength, int nMarkerBit, UINT sendTime, bool video)
{
	LOGV(TAG, "%s(%d): sendTime(%d hz) mark(%d) length(%d)", __FILENAME__, __LINE__, sendTime, nMarkerBit, nLength);
	int nBytesSent = 0;	
			
	const int FIXED_RTP_HEADER_SIZE = 12;
	const int FIXED_RTP_OVER_TCP_HEADER_SIZE = 4;
	nBytesSent = nLength + FIXED_RTP_HEADER_SIZE;
	rtpHeader->marker = nMarkerBit;

	//wait(sendTime);
	rtpHeader->timestamp = htonl(sendTime);
	if(video || !USE_AUDIO)
	{
		rtpHeader->sequenceNumber = htons(sequenceNumber);
	}
	else
	{
		rtpHeader->sequenceNumber = htons(sequenceNumberAudio);
	}
	
	if(tcpSocketFD > 0)
	{
		nBytesSent += FIXED_RTP_OVER_TCP_HEADER_SIZE;

		rtpOverTcpHeader->magic = 0x24;
		if(video && USE_AUDIO)
		{
			rtpOverTcpHeader->channel = interleavedData - 2;
			rtpHeader->payloadType = 96;
		}
		else if(video && !USE_AUDIO)
		{
			rtpOverTcpHeader->channel = interleavedData;
			rtpHeader->payloadType = 96;
		}
		else
		{
			rtpOverTcpHeader->channel = interleavedData;
			if(AUDIO_TYPE == eAT_AAC)
			{
				rtpHeader->payloadType = 97;
			}
			else if(AUDIO_TYPE == eAT_PCMU)
			{
				rtpHeader->payloadType = 0;
			}
			else if(AUDIO_TYPE == eAT_PCMA)
			{
				rtpHeader->payloadType = 8;
			}
		}

		rtpOverTcpHeader->length = htons(nLength + FIXED_RTP_HEADER_SIZE);
		
		memcpy(this->buffer, (const char*)rtpOverTcpHeader, FIXED_RTP_OVER_TCP_HEADER_SIZE);
		memcpy(this->buffer + FIXED_RTP_OVER_TCP_HEADER_SIZE, (const char*)rtpHeader, FIXED_RTP_HEADER_SIZE);
		memcpy(this->buffer + FIXED_RTP_OVER_TCP_HEADER_SIZE + FIXED_RTP_HEADER_SIZE, sendBuffer, nLength);

		nBytesSent = ::send(tcpSocketFD, (const char*) this->buffer, nBytesSent, MSG_NOSIGNAL);
	}
	else
	{
		memcpy(this->buffer, (const char*)rtpHeader, FIXED_RTP_HEADER_SIZE);
		memcpy(this->buffer + FIXED_RTP_HEADER_SIZE, sendBuffer, nLength);

		int clilen = sizeof(clientAddr);
		nBytesSent = sendto(udpSocketFD, (const char*) this->buffer, nBytesSent, 0, (struct sockaddr *)&clientAddr, clilen);
	}
	
	if(video || !USE_AUDIO)
	{
		sequenceNumber = (sequenceNumber + 1) & 0xFFFF;
	}
	else
	{
		sequenceNumberAudio = (sequenceNumberAudio + 1) & 0xFFFF;
	}

	return nBytesSent;
}

void RTPSender::sendAudio(uint64_t sendTime)
{
	//if(!USE_AUDIO || !audioFp) return;
	
	if(AUDIO_TYPE == eAT_AAC)
	{
		const int write_size = 256 * 1024 - aacParser->GetDataSize();
		BYTE buff[write_size];
		const int read_size = fread(buff, 1, write_size, audioFp);

		if(read_size == 0 && feof(audioFp))
		{
			fseek(audioFp, 0, SEEK_SET);
		}
		aacParser->Input(buff, read_size);
		
		//uint64_t sampleTime = sendAudioFrameCount*1024ull*900ull/441ull;

		struct timeval timeval;
		gettimeofday(&timeval, NULL);

		uint64_t currentTime = 0;
		uint64_t diffTime = 0;
		uint64_t sec = (uint64_t)(timeval.tv_sec);
		uint64_t usec = (uint64_t)(timeval.tv_usec);

		if(firstSampleTime == 0) firstSampleTime = currentTime = sec * 100 + usec / 10000;
		currentTime = sec * 100 + usec / 10000;
		diffTime = currentTime - firstSampleTime;

		sampleTime = diffTime * 441;

		while(sampleTime > sendAudioFrameCount*1024)
		{
			BYTE* buffer;
			int payloadSize;
			HRESULT hr = aacParser->Output(&buffer, &payloadSize);
			
			if(FAILED(hr))
			{
				fseek(audioFp, 0, SEEK_SET);
				break;
			}

			if(payloadSize > 1000)
			{
				printf("frame count : %lld, frame size : %d, write_size : %d\n", sendAudioFrameCount, payloadSize, write_size);
				break;
			}

			int nPayloadSize;
			// Unit: bytes
			int nAUHeaderSize = 2 + 1 * 2;
			BYTE bodyBuff[nAUHeaderSize + payloadSize];
			memcpy(bodyBuff + nAUHeaderSize, buffer, payloadSize);
			USHORT* psAUHeader = (USHORT*)bodyBuff;

			// Unit: bit
			//USHORT nLength = m_nAACFrameCount * 0x10;
			USHORT nLength = 1 * 0x10;
			//*psAUHeader = ConvertEndianUInt16(nLength);
			*psAUHeader = htons(nLength);
			
			USHORT sPayloadSize = (USHORT)payloadSize;
			for(int i=0; i<1/*i<m_nAACFrameCount*/; i++)
			{
				nLength = sPayloadSize << 3;
				//nLength = ConvertEndianUInt16(nLength);
				nLength = htons(nLength);

				psAUHeader[i+1] = nLength;
			}

			nPayloadSize = payloadSize + nAUHeaderSize;
			
			int ret = send(bodyBuff, nPayloadSize, 1, (UINT)(sendAudioFrameCount*1024), false);
			sendAudioFrameCount++;
			//sampleTime = sendAudioFrameCount*1024ull*900ull/441ull;
		}
	}
	else
	{

#if 1
#if 1 // JACOB_20160425
		const int write_size = 480;
		BYTE buff[write_size];


		struct timeval timeval;
		gettimeofday(&timeval, NULL);

		uint64_t currentTime = 0;
		uint64_t diffTime = 0;
		uint64_t sec = (uint64_t)(timeval.tv_sec);
		uint64_t usec = (uint64_t)(timeval.tv_usec);

		if(firstSampleTime == 0) firstSampleTime = currentTime = sec * 1000 + usec / 1000;
		currentTime = sec * 1000 + usec / 1000;
		diffTime = currentTime - firstSampleTime;

		sampleTime = diffTime * 8;

		while(sampleTime > sendAudioFrameCount*480)
		{
			const int read_size = 480;
			memcpy(buff, (unsigned char *)shm_addr, 480);
			//printf("[AUDIO] read_size = %d,,buff[] => 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n",	read_size, buff[0], buff[1], buff[2], buff[3], buff[4]);
			int ret = send(buff, read_size, 1, (UINT)(sendAudioFrameCount*480), false);
			
			sendAudioFrameCount++;
		}
#else
		const int write_size = 1120;
		BYTE buff[write_size];

		
		struct timeval timeval;
		gettimeofday(&timeval, NULL);

		uint64_t currentTime = 0;
		uint64_t diffTime = 0;
		uint64_t sec = (uint64_t)(timeval.tv_sec);
		uint64_t usec = (uint64_t)(timeval.tv_usec);

		if(firstSampleTime == 0) firstSampleTime = currentTime = sec * 1000 + usec / 1000;
		currentTime = sec * 1000 + usec / 1000;
		diffTime = currentTime - firstSampleTime;

		sampleTime = diffTime * 8;

		while(sampleTime > sendAudioFrameCount*1120)
		{


		
			const int read_size = fread(buff, 1, write_size, audioFp);
			if(read_size < 1120 )
			{
				fseek(audioFp, 0, SEEK_SET);
				continue;
			}



			#if 0
			// JACOB_20160422
			//printf("[AUDIO] read_size = %d,,buff[] => 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n",  read_size, buff[0], buff[1], buff[2], buff[3], buff[4]);


			if (gseq== 0)
			{
				audioSavefp = fopen("/opt/AStream001.g711u", "wb");
			}
			
			if (gseq < 1000 )
			{
				fwrite(buff, 1, read_size, audioSavefp);
				fflush(audioSavefp);
			}
			
			
			if(gseq == 1000)
				printf("[AStream END]\n");

			gseq++;
			#endif

			int ret = send(buff, read_size, 1, (UINT)(sendAudioFrameCount*1120), false);
			
			sendAudioFrameCount++;
		}
#endif		
#endif
		
	}
	
}

/** @brief sequence number를 가져온다.
	@return sequence number
	@param 없음.
*/
int RTPSender::getSequence()
{
	return (int)sequenceNumber;
}

// 사용안함.
UINT RTPSender::getRTPTime()
{
	return 0;// for test
}

/** @brief rtsp, rtp socket을 설정한다. rtp over tcp 일 경우 rtspsocket을 사용한다.
	@return 없음.
	@param rtspSocket rtsp socket
	@param rtpSocket rtp socket
*/
void  RTPSender::setSocket(SOCKET rtspSocket, SOCKET rtpSocket)
{
	tcpSocketFD = rtspSocket;
	udpSocketFD = rtpSocket;
}

/** @brief interleaved channel을 설정한다. rtp over tcp일 경우 사용
	@return 없음.
	@param dataChannel interleaved data channel
	@param controlChannel interleaved control channel
*/
void RTPSender::setInterleavedChannel(int dataChannel, int controlChannel)
{
	interleavedData = dataChannel;
	interleavedControl = controlChannel;
}

/** @brief timestamp를 받아와서 실제로 data를 보내야 할 시점까지 sleep 한다.
	@return S_OK
	@param timeStamp 현재 보내야 할 data의 timestamp
*/
HRESULT RTPSender::wait(UINT timeStamp)
{
	if(latestTimeStamp > 0)
	{
		long long diff = GW::convertTSToMicroSecond(timeStamp - latestTimeStamp, sampleRate);
		long long syncDiff = (long long) (GW::getTimeMicroSecond() - syncTime);
		//long long sleepTime = diff - syncDiff - 1000;
		long long sleepTime = diff - syncDiff;

		if(sleepTime > 0)
		{
			GW::nsleep(sleepTime*1000);
		}
	}
	else
	{
		syncTime = GW::getTimeMicroSecond();
		latestTimeStamp = timeStamp;
	}

	return S_OK;
}
