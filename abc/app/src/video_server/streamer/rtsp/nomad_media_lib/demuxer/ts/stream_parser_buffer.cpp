
#include <memory>
#include "demuxer/ts/stream_parser_buffer.h"

namespace nml_demuxer
{
namespace ts
{

StreamParserBuffer::StreamParserBuffer(const uint8* data, int data_size, 
                                       bool is_keyframe) :
    size_(data_size)
{
    //data_ = (uint8*)malloc(data_size);
    //memset(data_, 0x00, data_size);
    //memcpy(data_, data, data_size);
}


StreamParserBuffer::~StreamParserBuffer()
{
    //free(data_);
}

} //namespace ts
} //namespace nml_demuxer


