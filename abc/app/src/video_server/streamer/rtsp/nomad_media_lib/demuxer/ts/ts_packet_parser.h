
//=======================================================================
// 
//  [11/18/2013 kwanghyun won]
//
//  MPEG Transport Stream Parser  
//
//  * [12/18/2013] re implementaion
//======================================================================= 

#ifndef _TS_PARSER_H_
#define _TS_PARSER_H_

#include "base/basic_types.h"
#include "base/nml_structure.h"
#include "reader/bit_reader.h"

namespace nml_demuxer
{
namespace ts
{
#if 0
enum SpecialPid 
{
    kPidPat        = 0x00,
    kPidCat        = 0x01,
    kPidTsdt       = 0x02,
    kPidNullPacket = 0x1fff,
    kPidMax        = 0x1fff,
};
#endif

enum StreamType
{
    // ISO-13818.1 / ITU H.222 Table 2.34 "Stream type assignments"
    // http://msdn.microsoft.com/en-us/library/windows/desktop/dd695092(v=vs.85).aspx

    //video
    kStreamTypeMPEG2      = 0x02,
    kStreamTypeMPEG4Video = 0x10,
    kStreamTypeAVC        = 0x1b,

    //audio
    kStreamTypeMPEG1Audio = 0x03,
    kStreamTypeMPEG2Audio = 0x04,
    kStreamTypeAAC        = 0x0f,
    kStreamTypeAC3        = 0x81,
    kStreamTypePes        = 0x06, //AC3

    kStreamTypePrivate    = 0x05
};


class TsPacket
{
public:
    static const int kTsPacketSize = 188;
    static const uint8  kTsSyncByte = 0x47;

    // Return the number of bytes 
    static uint32 sync(const uint8* buffer, int size);

    // Parse a TS packet.
    //static TsPacket* parse(const uint8* buffer, int size, int packet_number);
    Status parse(const uint8* buffer, int size, int packet_number);
    
    TsPacket()
    {
        pcrTime = 0.0;
    }

    ~TsPacket(){}

    // TS header accessors.
    bool getPayloadUnitStartIndicator() const 
    {
        return payloadUnitStartIndicator;
    }

    int getPid() const { return pid; }
    int getContinuityCounter() const { return continuityCounter; }
    bool getDiscontinuityIndicator() const { return discontinuityIndicator; }
    bool getRandomAccessIndicator() const { return randomAccessIndicator; }

    // Return the offset and the size of the payload.
    const uint8* getPayload() const { return payload; }
    int getPayloadSize() const { return payloadSize; }
	
	
	//change "discontinuity_indicator" value to true
    void fixDiscontinuityIndicator(void) const;
	void fixContinuityCounter(int new_continuity) const;
	
    double getPcrTime() const {return pcrTime;}
    //base::property<double, get> pcrTime;
    int64  getPcrBase() const {return pcrBase;}
    int    getPcrExt() const  {return pcrExtension;}
	
	int getPacketNumber() const { return packetNumber; }
    void setPacketNumber(int val) { packetNumber = val; }

    void reset()
    {
        pcrBase = 0;
        pcrExtension = 0;
        pcrTime = 0;
        packetNumber = 0;
    }

    //Status print_Info();
private:
    

    // Parse an Mpeg2 TS header.
    Status parseHeader(const uint8* buffer);
    
    //Status parseAdaptationField(nml_reader::BitReader* bit_reader, int adaptation_field_length);
    Status parseAdaptationField(const uint8* adaptationField);

    const uint8* payload;
	uint8* adaptationField;
	uint8* tsHeader;
    uint32 payloadSize;

    //TS header
    bool payloadUnitStartIndicator;
    uint32 pid;
    uint32 continuityCounter;


    //adaptation field
    bool discontinuityIndicator;
    bool randomAccessIndicator;

    //timestamp
    int64 pcrBase;
    int pcrExtension;

    double pcrTime;
	int packetNumber;
};


} //namespace ts
} //namespace demuxer


#endif //_TS_PARSER_H_


