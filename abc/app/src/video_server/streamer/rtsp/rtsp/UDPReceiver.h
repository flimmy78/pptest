#ifndef __UDPRECEIVER_H__
#define __UDPRECEIVER_H__

#include <string>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <map>

#define DATA_HEADER_SIZE	13
struct data_header
{
	uint32_t data_size_byte;
	uint64_t timeStampHz;  // (90000hz) 1초=90000hz로 변환해서 으로 환산한 인코딩 시간
	uint8_t data_type; // 0 sps, 1 pps, 2 idr, 3 non idr
}__attribute__((packed));


void *gUDPReceiverThreadProc( void * );

class ThreadSafe;
class NALUnitTokenizer;
class SessionManager;
	
class UDPReceiver
{	
public:

	UDPReceiver(const int scalerNumber, SessionManager* sm);
	virtual ~UDPReceiver();

	int open();
	int start();
	int stop();
	int close();
	
	bool getStop(void);
	void setStop(bool stop);

	void recvData();
	int recvTimeout(char* buf, size_t len);
	void sendData(uint8_t* buf, size_t len);

private:
	int recv_sock;
	struct sockaddr_in recv_addr;
	int recv_addr_len;

	bool isStop;
	int port;
	
	bool isInitialized;
	bool concatenateSlice;
	int scalerId;
	uint64_t firstTime;
	uint64_t previousTime;
	pthread_t thread;

	NALUnitTokenizer* nalUnitTokenizer;
	SessionManager* sessionManager;
	
	void setPort(const int port);
	int getPort();
};

#endif // __UDPRECEIVER_H__
