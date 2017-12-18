//=======================================================================
//
//  [12/10/2013 kwanghyun won]
//
//=======================================================================

#ifndef _MPEG_ES_H
#define _MPEG_ES_H

#include "base/basic_types.h"
#include "base/media_time.h"
#include "base/nml_structure.h"

#include "demuxer/ts/emit_frame_interface.h"

namespace nml_demuxer
{
namespace ts
{

class ESParser
{
public:
    ESParser() {}
    virtual ~ESParser() {}

    //////////////////////////////////////////////////////////////////////////
    // pure virtual function

    // ES parsing.
    virtual Status parse(const uint8* buffer, int size, 
                        base::DeltaTime pts, 
                        base::DeltaTime dts) = 0;
    virtual void flush() = 0;
    virtual void reset() = 0;
protected:
    EmitFrameInterface *cb;
};

} //namespace ts
} //namespace nml_demuxer
#endif //_MPEG_ES_H
