
#include "base/nml_defines.h"
#include "base/debug.h"
#include "demuxer/ts/pid_state.h"


namespace nml_demuxer
{
namespace ts
{

PidState::PidState()
{
    programId         = -1;
    programIdType     = kPidNullPacket;
    sectionParser     = nullptr;
    isEnable          = false;
    continuityCounter = -1;
	trickScale 	  = 10;
}

PidState::~PidState()
{

}

void PidState::init(int pid, PidType pid_type, TsSection* section_parser)
{
    programId         = pid;
    programIdType     = pid_type;
    sectionParser     = section_parser;
    isEnable          = false;
    continuityCounter = -1;
	duplicatePackets  = 0;
	trickScale 		  = 10;
}


void PidState::disable()
{
    if (!isEnable)
        return;

    reset_State();
    isEnable = false;
}

void PidState::reset_State()
{
    if(sectionParser)
	{
		sectionParser->reset();
		delete sectionParser;
	}
    continuityCounter = -1;
	duplicatePackets  = 0;
	trickScale		  = 10;
}

void PidState::flush()
{
    if(sectionParser) sectionParser->flush();
    reset_State();
}

Status PidState::pushTsPacket(const TsPacket& ts_packet)
{
    NM_ASSERT( ts_packet.getPid() == programId);

    if (!isEnable)
        return true;

    if (programIdType == kPidVideoPes)
	{
		int continuity_counter = ts_packet.getContinuityCounter();
		int expected_continuity_counter = (this->continuityCounter + 1) % 16;

		if ( continuity_counter != expected_continuity_counter )
		{
			if ( !ts_packet.getDiscontinuityIndicator() )
			{
				duplicatePackets++;
				if (duplicatePackets > kMaxDuplicatePacketsCount) 
				{
					TS_LOG(1)  
						<< " continuity_counter:" << continuity_counter
						<< " expected_continuity_counter:" << expected_continuity_counter;

					if (trickScale > 20) 
					{
						TS_LOG(1)  
							<< "TS discontinuity detected for pid:" << programId 
							<< " number: "<< ts_packet.getPacketNumber()
							<< " duplicatePacketsCount : " << duplicatePackets;  

						//ts_packet.fixDiscontinuityIndicator();
						ts_packet.fixContinuityCounter(expected_continuity_counter);
						continuity_counter = expected_continuity_counter;
					}
				}
			}
		} 
		this->continuityCounter = continuity_counter;
	}

    Status status = NML_OK;
    if (programIdType != kPidNullPacket)
    {
		if(sectionParser)
		{
			status = sectionParser->parse(
			ts_packet.getPayloadUnitStartIndicator(),
			ts_packet.getPayload(),
			ts_packet.getPayloadSize());
		}
    }

    if (NML_OK != status) 
    {
        TS_LOG(LOG_LEVEL_ERROR) 
            << "Parsing failed for pid = :" << programId;

        reset_State();
    }

    return status;
}


}
}


