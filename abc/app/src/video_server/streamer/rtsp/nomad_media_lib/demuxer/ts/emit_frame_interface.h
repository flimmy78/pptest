
//=======================================================================
// 
//  [1/6/2014 kwanghyun won]
//                                                     
//======================================================================= 

#ifndef _EMIT_FRAME_INTERFACE_H_
#define _EMIT_FRAME_INTERFACE_H_

#include "demuxer/ts/stream_parser_buffer.h"

namespace nml_demuxer
{
namespace ts
{

class EmitFrameInterface
{
public:
    virtual void onEmitVideoFrame(StreamParserBuffer* stream_parser_buffer) = 0;
    virtual void onEmitAudioFrame(StreamParserBuffer* stream_parser_buffer) = 0;
};

} //namespace ts
} //namespace nml_demuxer

#endif //_EMIT_FRAME_INTERFACE_H_


