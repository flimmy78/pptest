
//=======================================================================
// 
//  [12/16/2013 kwanghyun won]
//                                                     
//======================================================================= 


#include "reader/bit_reader.h"

#include <algorithm>
#include "base/nml_defines.h"
#include "base/debug.h"

namespace nml_reader
{

BitReader::BitReader(const uint8* data, long size) :
    data_(data),
    left_bytes_(size),
    number_of_remaining_bits_(0)
{
    updateCurrentByte();
}

void BitReader::updateCurrentByte()
{
    if ( left_bytes_ == 0)
        return;

    //load a new byte
    current_byte_ = *data_;
    ++data_;
    --left_bytes_;

    number_of_remaining_bits_ = 8;
}

bool BitReader::readBitsInternal(int number_of_bits, uint64* out)
{
    *out = 0;

    while (number_of_remaining_bits_ != 0 && number_of_bits !=0)
    {
        int take_bit = std::min( number_of_remaining_bits_, number_of_bits);
        *out <<= take_bit;
        *out += current_byte_ >> ( number_of_remaining_bits_ - take_bit );
        number_of_bits -= take_bit;

        number_of_remaining_bits_ -= take_bit;
        current_byte_ &= (1 << number_of_remaining_bits_) - 1;

        if (number_of_remaining_bits_ == 0)
            updateCurrentByte();
    }

    return (number_of_bits == 0);
}

bool BitReader::skipBits(int number_of_bits)
{
    //NM_ASSERT( number_of_bits <= 128);
    while ( number_of_bits > 0 && number_of_bits > number_of_remaining_bits_ )
    {
        number_of_bits -= number_of_remaining_bits_;
        number_of_remaining_bits_ = 0;
        updateCurrentByte();

        if (number_of_remaining_bits_ == 0)
            return (number_of_bits == 0);
    }

    //8bit이하일때는 read_Bit_Internal을 이용
    uint64 temp;
    return readBitsInternal(number_of_bits, &temp);
}

uint32 BitReader::bitsAvailable() const
{
    return static_cast<uint32>(8 * left_bytes_ + number_of_remaining_bits_);
}

} //namespace nml_reader
