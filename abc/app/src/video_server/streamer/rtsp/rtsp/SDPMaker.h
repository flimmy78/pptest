#ifndef __SDP_MAKER_H__
#define __SDP_MAKER_H__

#include <string>


typedef struct _VideoInfo
{
	int spsSize;
	uint8_t* spsData;

	int ppsSize;
	uint8_t* ppsData;
} VideoInfo;

class SDPMaker
{
protected:
	std::string strDesc;
	std::string strHost;

	//int m_nDuration;
	double duration;

	int payloadType;

	VideoInfo* videoInfo;

public:
	SDPMaker();
	~SDPMaker();

	//int GetDuration() const;
	double getDuration() const;
	void setDuration(double fDuration);

	int getPayloadType() const;
	void setPayloadType(int payloadType);

	std::string getHost() const;
	void setHost(std::string strHost);

	HRESULT run();
	void clear();

	const char* toString() const;
	int getLength();

	void setVideoInfo(VideoInfo videoInfo);
};

#endif //__SDP_MAKER_H__
