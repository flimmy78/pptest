
//=======================================================================
// 
//  [12/26/2013 kwanghyun won]
//                                                     
//======================================================================= 

#ifndef _PID_STATE_H_
#define _PID_STATE_H_

#include "demuxer/ts/ts_section.h"
#include "demuxer/ts/ts_packet_parser.h"

namespace nml_demuxer
{
namespace ts
{

class PidState
{
public:
    enum PidType 
    {
        kPidNullPacket,
        kPidPat,
        kPidPmt,
        kPidAudioPes,
        kPidVideoPes,
        kPidPrivatePes
    };

	static const int kMaxDuplicatePacketsCount = 1;
	
    PidState();
    virtual ~PidState();

    Status pushTsPacket(const TsPacket& ts_packet);
    void init(int pid, PidType pid_type, TsSection* section_parser);
    void flush();

    // Enable/disable the PID.
    // Disable된 PID packet은 계속 무시함.
    void enable() { isEnable = true; };
    void disable();

    bool getEnabled() const { return isEnable; }
    PidType getPidType() const { return programIdType; }
    int getPid() const { return programId; }
	void setTrickScale(int trick_scale ) { trickScale = trick_scale; }
private:
    void reset_State();

    int programId;;
    PidType programIdType;
    TsSection* sectionParser;
    bool isEnable;
    int continuityCounter;
	int duplicatePackets;
	int trickScale;
};


} //namespace ts
} //namespace nml_demuxer
#endif //_PID_STATE_H_


