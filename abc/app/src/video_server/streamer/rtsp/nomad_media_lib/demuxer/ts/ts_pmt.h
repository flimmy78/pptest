
//=======================================================================
// 
//  [11/25/2013 kwanghyun won]
//                                                     
//======================================================================= 

#ifndef _TS_PMT_H_
#define _TS_PMT_H_

//#include <vector>

#include "base/basic_types.h"
#include "demuxer/ts/ts_section_psi.h"
#include "demuxer/ts/register_interface.h"

namespace nml_demuxer
{
namespace ts
{ 

class TSPmt : public TsSectionPSI
{
public:
	TSPmt(RegisterInterface* cb);
    virtual ~TSPmt() {}

    //////////////////////////////////////////////////////////////////////////
    // Ts_Section_PSI implementation
    virtual Status parsePsiSection(nml_reader::BitReader* bit_reader);
    virtual void resetPsiSection() {}

private:
    RegisterInterface* cb_;    
};


} //namespace ts
} //namespace demuxer

#endif //_TS_PMT_H_
