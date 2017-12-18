
//=======================================================================
// 
//  [12/17/2013 kwanghyun won]
//                                                     
//======================================================================= 

#ifndef _FILE_SINKER_H_
#define _FILE_SINKER_H_

#include "base/nml_defines.h"
#include "sinker/base_sinker.h"

namespace nml_sinker
{

class FileSinker : public BaseSinker
{
public:
    FileSinker();
    ~FileSinker();

    Status init(BaseSinkerParams* params) OVERRIDE;
    Status close(void) OVERRIDE;
    Status reset(void) OVERRIDE;
    Status putData(const void *data, size_t &size) OVERRIDE;
    uint64 getPosition(void) OVERRIDE;
    Status movePosition(int64 offset) OVERRIDE;

    uint64 getFileSize();

private:
    BaseSinkerParams params_;
    FILE*              file_;
    uint64             file_size_;
};



} //namespace nml_sinker
#endif //_FILE_SINKER_H_


