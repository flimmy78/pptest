
//=======================================================================
// 
//  [12/18/2013 kwanghyun won]
//                  
// abstract Program Specific Information parser class
// pat,cat,pmt,nit 는 이클래스를 상속하고 구현.
//======================================================================= 

#ifndef _TS_SECTION_PSI_H_
#define _TS_SECTION_PSI_H_

#include <map>

#include "base/nml_defines.h"
#include "base/byte_queue.h"
#include "demuxer/ts/ts_section.h"
#include "demuxer/ts/pid_state.h"
#include "reader/bit_reader.h"

namespace nml_demuxer
{
namespace ts
{

class TsSectionPSI : public TsSection
{
public:
    TsSectionPSI();
    virtual ~TsSectionPSI(){}


    //////////////////////////////////////////////////////////////////////////
    // Ts_Section implementation
    virtual Status parse( bool payload_unit_start_indicator,
                          const uint8* buffer, 
                          size_t size) OVERRIDE;

    virtual void flush() OVERRIDE;
    virtual void reset() OVERRIDE;

    //////////////////////////////////////////////////////////////////////////
    // pure virtual function
    virtual Status parsePsiSection(nml_reader::BitReader* bit_reader) = 0;
    virtual void resetPsiSection() = 0;

private:
    void resetPsiState();
    Status isCrcValid(const uint8* buffer, int size);

    base::ByteQueue byte_queue_;
    
    // payload unit start indicator 를 얻기전에는 parsing하지 않는다. 
    bool is_wait_for_pusi_;

    int leading_bytes_to_discard_;
};

} //namespace ts
} //namespace nml_demuxer

#endif //_TS_SECTION_PSI_H_


