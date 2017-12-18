
//=======================================================================
//
//  [11/18/2013 kwanghyun won]
//
//	MPEG Packetized Elementary Stream Parser
//=======================================================================

#ifndef _PES_PARSER_H_
#define _PES_PARSER_H_

#include "base/nml_defines.h"
//#include "base/byte_queue.h"

#include "demuxer/ts/ts_section.h"
#include "demuxer/ts/es_parser.h"

typedef std::function<void(const int64)> VideoPtsCB;

namespace nml_demuxer
{
namespace ts
{

class PESParser : public TsSection
{
public:
    PESParser(VideoPtsCB cb);
    //PESParser();
    virtual ~PESParser();

    static const int kPesStartCode = 0x000001;

    static int64 convertTimestampSectionToTimestamp(int64 timestamp_section);
    //////////////////////////////////////////////////////////////////////////
    // Ts_Section implementation
    virtual Status parse( bool payload_unit_start_indicator,
                          const uint8* buffer,
                          size_t size) OVERRIDE;

    virtual void flush() OVERRIDE;
    virtual void reset() OVERRIDE;

private:

    // Emit a reassembled PES packet.
    Status emit(bool emit_for_unknown_size);
    Status parseInternal(const uint8* raw_pes, int raw_pes_size);
    //Status parseInternal(const uint8* raw_pes, int raw_pes_size);
    void resetPesState();

    //base::ByteQueue pesByteQueue;

    // payload unit start indicator.
    bool waitForPusi;

#if 0
    // Used to unroll PTS and DTS.
    bool  previousPtsValid;
    int64 previousPts;
    bool  previousDtsValid;
    int64 previousDts;
#endif

    VideoPtsCB videoPtsCB;
};


} //namespace ts
} //namespace nml_demuxer

#endif //_PES_PARSER_H_



