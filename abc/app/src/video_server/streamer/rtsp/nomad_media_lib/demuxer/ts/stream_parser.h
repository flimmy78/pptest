
//=======================================================================
// 
//  [11/15/2013 kwanghyun won]
//                                                     
//======================================================================= 

#ifndef _STREAM_PARSER_H
#define _STREAM_PARSER_H

#include "base/basic_types.h"
#include "base/nml_structure.h"

#include <functional> //std::function
namespace nml_demuxer
{
namespace ts
{

typedef std::function<void(const uint8*, size_t)> SendEvent;

class StreamParser
{
public:
    StreamParser(){};
    virtual ~StreamParser() {}

    //////////////////////////////////////////////////////////////////////////
    // pure virtual function

    virtual Status parse(const uint8* buffer, int size, SendEvent e = nullptr) = 0;
    virtual void flush() = 0;

};


} //namespace ts
}//namespace nomad_conn_media_framework
#endif //_NCMF_STREAM_PARSER_H


