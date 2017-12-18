#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#include "config.h"
#include <nomad_log.h>

#include "ThreadSafe.h"
#include "UDPReceiver.h"
#include "NALUnitTokenizer.h"
#include "SessionManager.h"


#define USE_FRAME_HEADER	1

using namespace std;

static const char* TAG = "UDPRECEIVER";


void *gUDPReceiverThreadProc(void *ptr)
{
	UDPReceiver *udpReceiver = (UDPReceiver*) ptr;
	while(!udpReceiver->getStop())
	{
		udpReceiver->recvData();
	}

	return (void*)NULL;
}

/** @brief RTSPServer ������. 
	@return �ǹ� ����.
	@param port udp receiver �� ��Ʈ
*/
UDPReceiver::UDPReceiver(const int id, SessionManager* sm)
{
	sessionManager = sm;
	isStop = false;
	nalUnitTokenizer = new NALUnitTokenizer(512 * 1024);
	scalerId = id;
	firstTime = 0;
	previousTime = 0;
	concatenateSlice = false;
	isInitialized = false;

	setPort(SCALER_UDP_PORT_TABLE[scalerId]);
	open();
	start();	
}

/** @brief RTSPServer �Ҹ���.
	@return �ǹ� ����.
	@param ����.
*/
UDPReceiver::~UDPReceiver()
{
	stop();
	close();
	if(nalUnitTokenizer) delete nalUnitTokenizer;
}

/** @brief udp socket����� bind�� �����Ѵ�.
	@return S_OK
	@param ����.
*/
int UDPReceiver::open()
{
	LOGD(TAG, "%s(%d) %s: START.", __CALL_INFO__);

	recv_sock = socket(AF_INET, SOCK_DGRAM, 0);	

	recv_addr.sin_family = AF_INET;
    recv_addr.sin_port = htons(port);
    recv_addr.sin_addr.s_addr= htonl(INADDR_ANY);
    bzero(&(recv_addr.sin_zero), 8);
    recv_addr_len = sizeof (recv_addr);

    if(bind(recv_sock, (struct sockaddr*)&recv_addr, sizeof(struct sockaddr)) == -1)
	{
		LOGD(TAG, "%s(%d) %s: bind error.", __CALL_INFO__);
    }
	LOGD(TAG, "%s(%d) %s: END.", __CALL_INFO__);
	return S_OK;
}

/** @brief thread�� �����ϰ� ������ ������ ����Ѵ�.
	@return S_OK
	@param ����.
*/
int UDPReceiver::start()
{
	LOGD(TAG, "%s(%d) %s: START.", __CALL_INFO__);

	pthread_attr_t attr;
	pthread_attr_init(&attr);

	size_t stacksize = 1024*1024;
	pthread_attr_setstacksize (&attr, stacksize);

	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	if(pthread_create( &thread, &attr, gUDPReceiverThreadProc, (void*) this))
	{
		LOGE(TAG, "%s(%d) %s: pthread create fail.", __CALL_INFO__);
		pthread_attr_destroy(&attr);
		return E_FAIL;
	}
	pthread_attr_destroy(&attr);

	LOGD(TAG, "%s(%d) %s: END.", __CALL_INFO__);
	return S_OK;
}

/** @brief udp ������ ���� thread�� �����Ѵ�.
	@return S_OK
	@param ����.
*/
int UDPReceiver::stop()
{
	isStop = true;
	pthread_join(thread, (void**)NULL);
	return S_OK;
}

/** @brief udp socket�� �ݴ´�.
	@return S_OK
	@param ����.
*/
int UDPReceiver::close()
{
	LOGD(TAG, "%s(%d) %s: START.", __CALL_INFO__);

	::close(recv_sock);

	LOGD(TAG, "%s(%d) %s: END.", __CALL_INFO__);
	return S_OK;
}

/** @brief udp socket�� ��Ʈ�� �����Ѵ�.
	@return ����.
	@param port ������ ��Ʈ
*/
void UDPReceiver::setPort(const int port)
{
	this->port = port;
}

/** @brief udp socket�� ��Ʈ�� �����´�.
	@return rtsp server socket�� ��Ʈ
	@param ����.
*/
int UDPReceiver::getPort()
{
	return port;
}

/** @brief udp receiver�� ���¸� �����Ѵ�.(stop = true or true)
	@return ����.
	@param state ������ ���°�
*/
void UDPReceiver::setStop(const bool stop)
{
	this->isStop = stop;
}

/** @brief udp receiver�� ���¸� �����´�.(stop = true or false)
	@return state ������ ���°�
	@param ����.
*/
bool UDPReceiver::getStop()
{
	return isStop;
}

void UDPReceiver::recvData()
{
	uint8_t recv_data[128 * 1024];
	int ret;


	//printf("[recvData => %d]\n", recv_data[0]);

	ret = recvTimeout((char*)recv_data, sizeof(recv_data));
	if(ret > 0)
	{
#if USE_FRAME_HEADER
		data_header* frame_header = (data_header*)recv_data;
		if(firstTime == 0) firstTime = frame_header->timeStampHz;
		uint64_t diffTime = frame_header->timeStampHz - firstTime;

		//nalUnitTokenizer->Input(recv_data + sizeof(data_header), ret - sizeof(data_header));
		nalUnitTokenizer->Input(recv_data + DATA_HEADER_SIZE, ret - DATA_HEADER_SIZE);
#else
		struct timeval timeval;
		gettimeofday(&timeval, NULL);

		uint64_t currentTime = 0;
		uint64_t diffTime = 0;
		if(firstTime == 0) firstTime = currentTime = (uint64_t)timeval.tv_sec * 1000 + (uint64_t)timeval.tv_usec / 1000;
		currentTime = (uint64_t)timeval.tv_sec * 1000 + (uint64_t)timeval.tv_usec / 1000;
		diffTime = currentTime - firstTime;
		diffTime = diffTime * 90;

		nalUnitTokenizer->Input(recv_data, ret);
#endif		

		nalUnitTokenizer->Parse();

		int nalCount = nalUnitTokenizer->GetCount();
		
		int nNALUnitType;
		int nPayloadSize;
		//uint8_t* pBuffer;
		uint8_t* pPayload;

		//printf("\n nalcount = %d\n ", nalCount);

		for(int i=0; i<nalCount; i++)
		{
			int hr = nalUnitTokenizer->NextToken(&pPayload, &nPayloadSize, &nNALUnitType, false);
			if(hr < 0)
			{
				//LOGV(TAG, "%s(%d) %s: i(%d) nalCount(%d)", __CALL_INFO__, i, nNALUnitCount);
				hr = nalUnitTokenizer->Reset();
				break;
			}
			


			//nNALUnitType = pPayload[0] & 0x1F;
			//LOGD(TAG, "%s(%d) %s: nal(%d) nalCount(%d) pictureCount(%d)", __FILENAME__, __LINE__, __FUNCTION__, nNALUnitType, nNALUnitCount, nPictureCount);
			
			//if(nNALUnitType != 1) printf("nalunit type : %d, payloadSize : %d\n", nNALUnitType, nPayloadSize);
			//if(scalerId == 0 && nNALUnitType != 1) printf("nalunit type : %d, payloadSize : %d, scalerId : %d, diffTime : %lld\n", nNALUnitType, nPayloadSize, scalerId, previousTime);
//#if USE_FRAME_HEADER
#if 1
			if(nNALUnitType == eNUT_SPS)
			{
				sessionManager->setVideoInfoSPS(pPayload, nPayloadSize, scalerId);
			}
			else if(nNALUnitType == eNUT_PPS)
			{
				sessionManager->setVideoInfoPPS(pPayload, nPayloadSize, scalerId);
			}
#else
			if(nNALUnitType == eNUT_SPS)
			{
				isInitialized = true;
				concatenateSlice = false;
				sessionManager->setVideoInfoSPS(pPayload, nPayloadSize, scalerId);
			}
			else if(nNALUnitType == eNUT_PPS)
			{
				isInitialized = true;
				concatenateSlice = false;
				sessionManager->setVideoInfoPPS(pPayload, nPayloadSize, scalerId);
			}
			else
			{
				if(isInitialized) concatenateSlice = !concatenateSlice;
			}
#endif

#if USE_FRAME_HEADER
			//printf("\n PAYLOADSIZE = %d ,,, SCALERID = %d\n", nPayloadSize, scalerId);

			//sessionManager->broadcastData(pPayload, nPayloadSize, scalerId, diffTime, nNALUnitType == eNUT_IDR_PICTURE);
			sessionManager->broadcastData(pPayload, nPayloadSize, scalerId, previousTime, nNALUnitType == eNUT_SPS || nNALUnitType == eNUT_PPS);
			previousTime = diffTime;
#else
			//if(concatenateSlice) sessionManager->broadcastData(pPayload, nPayloadSize, scalerId, previousTime);
			sessionManager->broadcastData(pPayload, nPayloadSize, scalerId, previousTime, nNALUnitType == eNUT_SPS || nNALUnitType == eNUT_PPS);

			//if(nNALUnitType != 7 && nNALUnitType != 8)
			//{
			//	if(isInitialized) concatenateSlice = !concatenateSlice;
			//}

			previousTime = diffTime;
#endif			
		}
	}
}

void UDPReceiver::sendData(uint8_t* buf, size_t len)
{
	
}

int UDPReceiver::recvTimeout(char* buf, size_t len)
{
    int ret = 0;
    fd_set readfds;
    struct timeval timeout;

    timeout.tv_sec = 3;
    timeout.tv_usec = 0;

    FD_ZERO(&readfds);
    FD_SET(recv_sock, &readfds);
    if(select(recv_sock+1, &readfds, NULL, NULL, &timeout) < 0)
    {
		printf("recvfrom select error\n");
		return -1;
    }

    if(FD_ISSET(recv_sock, &readfds))
    {
		
	    ret = recvfrom(recv_sock, buf, len, 0, (struct sockaddr *)&recv_addr, (socklen_t*)&(recv_addr_len));
	    //printf("receive from len = %d\n", len);
    }
    else
    {
		// the socket timed out
		printf("recvfrom timeout\n");
		return 0;
    }

    return ret;
}
