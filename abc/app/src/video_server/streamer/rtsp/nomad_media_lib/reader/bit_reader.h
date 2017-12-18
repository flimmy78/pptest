
//=======================================================================
// 
//  [12/16/2013 kwanghyun won]
//                                                     
//======================================================================= 

#ifndef _BIT_READER_H_
#define _BIT_READER_H_

#include "base/basic_types.h"
#include "base/nml_structure.h"

namespace nml_reader
{


class BitReader
{
public:
    BitReader(const uint8* data, long size);
    ~BitReader(){}

    template<typename T>
    bool readBits(int number_of_bits, T* out) 
    {
        uint64 temp;
        bool result = readBitsInternal(number_of_bits, &temp);
        *out = static_cast<T>(temp);
        return result;
    }

    bool readBitsInternal(int number_of_bits, uint64* out);
    bool skipBits(int number_of_bits);
    uint32 bitsAvailable() const;
    void updateCurrentByte();

private:
    const uint8* data_;
    long         left_bytes_;
    uint8        current_byte_;
    int          number_of_remaining_bits_;
};

} //namespace nml_reader
#endif //_BIT_READER_H_



