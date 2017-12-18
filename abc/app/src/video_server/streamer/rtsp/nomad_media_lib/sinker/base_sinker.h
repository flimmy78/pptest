
//=======================================================================
// 
//  [12/17/2013 kwanghyun won]
//                                
//  Base abstract class of data writer.
//======================================================================= 

#ifndef _BASE_SINKER_H_
#define _BASE_SINKER_H_

#include "base/nml_structure.h"
#include <string>

namespace nml_sinker
{

struct BaseSinkerParams
{
    BaseSinkerParams()
    {
        file_name_ = "";
        buffer_size_ = 0;
        is_append_ = false;
    }

    std::string file_name_;
    size_t      buffer_size_;
    bool        is_append_;
};

class BaseSinker
{
public:
    BaseSinker(){}
    virtual ~BaseSinker(){}


    // Initialization abstract destination media
    virtual Status init(BaseSinkerParams* params) = 0;

    // Close destination media
    virtual Status close(void) = 0;

    // Reset all internal parameters to start writing from begin
    virtual Status reset(void) = 0;

    // Write data to output stream
    virtual Status putData(const void *data, size_t &size) = 0;

    // Get absolute position in the stream
    virtual uint64 getPosition(void) = 0;

    // Move position relatively
    virtual Status movePosition(int64 offset) = 0;

    // Set position absolutely
    virtual Status setPosition(uint64 position)
    {
        return movePosition(position - getPosition());
    }

};

} //namespace nml_sinker
#endif //_BASE_SINKER_H_


