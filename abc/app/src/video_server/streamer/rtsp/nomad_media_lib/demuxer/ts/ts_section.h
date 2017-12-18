
//=======================================================================
// 
//  [12/18/2013 kwanghyun won]
//                                                     
//======================================================================= 

#ifndef _TS_SECTION_H_
#define _TS_SECTION_H_

#include <map>

#include "base/basic_types.h"
#include "base/nml_structure.h"

namespace nml_demuxer
{
namespace ts
{

class TsSection
{
public:
    // From ISO/IEC 13818-1 or ITU H.222 spec: Table 2-3 - PID table.
    enum SpecialPid {
        kPidPat = 0x0,
        kPidCat = 0x1,
        kPidTsdt = 0x2,
        kPidNullPacket = 0x1fff,
        kPidMax = 0x1fff,
    };

    
    virtual ~TsSection(){}

    //////////////////////////////////////////////////////////////////////////
    // pure virtual function
    virtual Status parse( bool payload_unit_start_indicator,
                          const uint8* buffer, 
                          size_t size) = 0;

    virtual void flush() = 0;
    virtual void reset() = 0;
};


} //namespace ts
} //namespace nml_demuxer
#endif //_TS_SECTION_H_



