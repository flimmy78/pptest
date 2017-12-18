
//=======================================================================
// 
//  [1/6/2014 kwanghyun won]
//                                                     
//======================================================================= 

#ifndef _STREAM_PARSER_BUFFER_H_
#define _STREAM_PARSER_BUFFER_H_

#include "base/media_time.h"

namespace nml_demuxer
{
namespace ts
{

class StreamParserBuffer
{
public:
    StreamParserBuffer(const uint8* data, int data_size, bool is_keyframe);
    ~StreamParserBuffer();

    int getDataSize() const { return size_; }
    base::DeltaTime getTimestamp() const { return timestamp; }
    void setTimestamp(base::DeltaTime val) { timestamp = val; }
    base::DeltaTime getDuration() const { return duration; }
    void setDuration(base::DeltaTime val) { duration = val; }
    base::DeltaTime getDecodeTimestamp() const { return decodeTimestamp; }
    void setDecodeTimestamp(base::DeltaTime val) { decodeTimestamp = val; }
    bool getIsKeyframe() const { return isKeyframe; }

private:
    //uint8* data_;
    int    size_;
    base::DeltaTime timestamp;
    base::DeltaTime decodeTimestamp;
    base::DeltaTime duration;

    bool isKeyframe;

    NML_DECLARE_NON_COPYABLE(StreamParserBuffer);
};

} //namespace ts
} //namespace nml_demuxer

#endif //_STREAM_PARSER_BUFFER_H_


