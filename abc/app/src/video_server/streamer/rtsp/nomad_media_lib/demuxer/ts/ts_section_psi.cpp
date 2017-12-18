
//=======================================================================
// 
//  [12/18/2013 kwanghyun won]
//                                                     
//======================================================================= 

#include "demuxer/ts/ts_section_psi.h"

#include <algorithm>

namespace nml_demuxer
{
namespace ts
{

TsSectionPSI::TsSectionPSI() :
    is_wait_for_pusi_(true),
    leading_bytes_to_discard_(0) {}

Status TsSectionPSI::parse(bool payload_unit_start_indicator, 
                             const uint8* buffer, 
                             size_t size)
{
    if (is_wait_for_pusi_ && !payload_unit_start_indicator)
        return NML_OK;

    if (payload_unit_start_indicator)
    {
        resetPsiState();

        is_wait_for_pusi_ = false;
        //int pointer_field = buffer[0];
        leading_bytes_to_discard_ = buffer[0];
        buffer++;
        size--;
    }

    if (leading_bytes_to_discard_ > 0 )
    {
        int bytes_to_discard = std::min( leading_bytes_to_discard_, 
                                        static_cast<int>(size) );
        buffer += bytes_to_discard;
        size -= bytes_to_discard;
        leading_bytes_to_discard_ -= bytes_to_discard;
    }
    if (size == 0)
        return NML_OK;

    //byte queue data 추가 
    byte_queue_.push(buffer, size);
    int raw_psi_size;
    const uint8* raw_psi;
    byte_queue_.peek(&raw_psi, &raw_psi_size);

    // parsing의 충분한 데이터인지 확인
    if (raw_psi_size < 3)
        return NML_OK;

    int section_length =
        ((static_cast<int>(raw_psi[1]) << 8) |
        (static_cast<int>(raw_psi[2]))) & 0xfff;

    if (section_length >= 1021)
        return NML_ERROR_INVALID_STREAM;

    int psi_length = section_length + 3;

    if (raw_psi_size < psi_length) 
    {
        return NML_OK;
    }

    Status status = isCrcValid(raw_psi, psi_length);
    if ( NML_OK !=  status)
        return status;

    // Parse the PSI section.
    nml_reader::BitReader bit_reader(raw_psi, raw_psi_size);
    status = parsePsiSection(&bit_reader);

    if ( status != NML_OK )
        resetPsiState();

    return status;
    
}

Status TsSectionPSI::isCrcValid(const uint8* buffer, int size)
{
    uint32 crc = 0xffffffffu;
    const uint32 kCrcPoly = 0x4c11db7;

    for (int k = 0; k < size; k++) 
    {
        int nbits = 8;
        uint32 data_msb_aligned = buffer[k];
        data_msb_aligned <<= (32 - nbits);

        while (nbits > 0) 
        {
            if ((data_msb_aligned ^ crc) & 0x80000000) 
            {
                crc <<= 1;
                crc ^= kCrcPoly;
            }
            else 
            {
                crc <<= 1;
            }

            data_msb_aligned <<= 1;
            nbits--;
        }
    }

    if (crc == 0)
        return NML_OK;

    return NML_ERROR_FAILED;
}

void TsSectionPSI::flush()
{

}

void TsSectionPSI::reset()
{
    resetPsiSection();
    resetPsiState();
}

void TsSectionPSI::resetPsiState()
{
    is_wait_for_pusi_ = true;
    byte_queue_.reset();
    leading_bytes_to_discard_ = 0;
}


} //namespace ts
} //namespace nml_demuxer



