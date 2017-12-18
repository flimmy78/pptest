#ifndef __TRANSPORT_HEADER_H__
#define __TRANSPORT_HEADER_H__

#include <string>

#include "Header.h"

enum RTSP_TRANSPORT_HEADER_MODE
{
	eRTHM_UNKNOWN			= -1,
	eRTHM_PLAY,
	eRTHM_RECORD,
};

enum PORT_TYPE
{
	PORT_RTP			= 0,
	PORT_RTCP			= 1,
	CLIENT_PORT_RTP		= 2,
	CLIENT_PORT_RTCP	= 3,
	SERVER_PORT_RTP		= 4,
	SERVER_PORT_RTCP	= 5,
};

// It is used to check whether some fields exist or not.
enum TRANSPORT_HEADER_FIELD_MASK
{
	eTHFM_INIT						= 0x00000000,
	eTHFM_CAST_TYPE					= 0x00000001,
	eTHFM_RTP_PORT					= 0x00000002,
	eTHFM_RTCP_PORT					= 0x00000004,
	eTHFM_CLIENT_RTP_PORT			= 0x00000008,
	eTHFM_CLIENT_RTCP_PORT			= 0x00000010,
	eTHFM_SERVER_RTP_PORT			= 0x00000020,
	eTHFM_SERVER_RTCP_PORT			= 0x00000040,
	eTHFM_SSRC						= 0x00000080,
	eTHFM_MODE						= 0x00000100,
	eTHFM_SOURCE					= 0x00000200,
	eTHFM_LOWER_TRANSPORT			= 0x00000400,
	eTHFM_INTERLEAVED_DATA			= 0x00000800,
	eTHFM_INTERLEAVED_CONTROL		= 0x00001000,
	eTHFM_TTL						= 0x00002000,
	eTHFM_DESTINATION				= 0x00004000,
	
};

enum LOWER_TRANSPORT
{
	eLT_TCP,
	eLT_UDP,
};

enum INTERLEAVED_CHANNEL
{
	eIC_DATA		= 0,
	eIC_CONTROL		= 1,
};

class Flags;

class TransportHeader : public Header
{
protected:
	Flags* fieldMask;

	UINT ssrc;
	int anPort[6];
	int mode;
	// rtp over tcp interleaved : data and control channel
	int interleaved[2];

	/**
	 * 0: TCP, 1: UDP
	 */
	int lowerTransport;

	/** 
	 * 0: multicast, 1: unicast
	 */
	int castType;

	std::string destination;
	int ttl;

protected:
	int getParameterIndex(char* szKey);
	virtual HRESULT parse(char* szValue);

	HRESULT parseTransportSpec(char* szToken);

public:
	TransportHeader(void);
	virtual ~TransportHeader(void);

	UINT getSSRC() const;
	void setSSRC(UINT nSSRC);

	int getPort(int nPortType);
	void setPort(int nPortType, int nPort);

	int getMode() const;
	void setMode(int nMode);

	virtual Header* duplicate() const;
	virtual HRESULT toString(char* szBuf, int nBufSize);

	int getLowerTransport() const;
	void setLowerTransport(int nLowerTransport);
	const char* getLowerTransportName() const;

	int getCastType() const;
	void setCastType(int nType);
	const char* getCastTypeName() const;

	const int getChannel(const int channelType);
	void setChannel(const int channelType, const int channel);

	const std::string getDestination();
	void setDestination(std::string dest);

	const int getTTL();
	void setTTL(const int ttl);
};

#endif // __TRANSPORT_HEADER_H__
