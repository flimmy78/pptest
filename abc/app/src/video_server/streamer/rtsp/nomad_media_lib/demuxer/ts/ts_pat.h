//=======================================================================
// 
//  [11/25/2013 kwanghyun won]
//                                                     
//======================================================================= 

#ifndef _TS_PAT_H_
#define _TS_PAT_H_

#include "base/basic_types.h"
#include "base/nml_structure.h"

#include "demuxer/ts/ts_section_psi.h"
#include "demuxer/ts/register_interface.h"

namespace nml_demuxer
{
namespace ts
{ 

class TS_Pat : public TsSectionPSI
{
public:
    explicit TS_Pat(RegisterInterface *cb);
    virtual ~TS_Pat() {}

    //////////////////////////////////////////////////////////////////////////
    // Ts_Section_PSI implementation

    virtual Status parsePsiSection(nml_reader::BitReader* bit_reader);
    virtual void resetPsiSection();

private:
    RegisterInterface *cb_;
	uint8  version_;
};



} //namespace ts
} //namespace demuxer

#endif //_TS_PAT_H_
