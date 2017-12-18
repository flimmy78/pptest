
//=======================================================================
// 
//  [1/6/2014 kwanghyun won]
//  Audio Data Transport Stream(ADTS) parser
// ref :http://wiki.multimedia.cx/index.php?title=ADTS
//======================================================================= 

#ifndef _ES_ADTS_PARSER_H_
#define _ES_ADTS_PARSER_H_

#include <list>

#include "base/nml_structure.h"
#include "base/byte_queue.h"

#include "demuxer/audio_timestamp_util.h"
#include "demuxer/ts/es_parser.h"
#include "demuxer/ts/emit_frame_interface.h"

namespace nml_demuxer
{
namespace ts
{

class EsAdtsParser : public ESParser
{
public:
    explicit EsAdtsParser(EmitFrameInterface* cb);
    virtual ~EsAdtsParser();

    //////////////////////////////////////////////////////////////////////////
    // es parser implementation
    virtual Status parse( const uint8* buffer, int size, 
                          base::DeltaTime pts, 
                          base::DeltaTime dts ) OVERRIDE;
    virtual void flush() OVERRIDE;
    virtual void reset() OVERRIDE;

private:
    bool updateAudioConfiguration(const uint8* adts_header);
    void discardsEs(int bytes);

    typedef std::pair<int, base::DeltaTime> esPts;
    typedef std::list<esPts> esPtsList;

    EmitFrameInterface* emitFrameCB;

    base::ByteQueue esByteQueue;
    esPtsList ptsList;
    nml_demuxer::AudioTimestampUtil* audio_timestamp_util;
    NML_DECLARE_NON_COPYABLE(EsAdtsParser);
};

} //namespace ts
} //namespace nml_demuxer

#endif //_ES_ADTS_PARSER_H_


