
//=======================================================================
// 
//  [11/25/2013 kwanghyun won]
//                                                     
//======================================================================= 

#include "demuxer/ts/ts_pat.h"

#include <vector>
#include <map>

#include "base/debug.h"
#include "base/nml_defines.h"
#include "demuxer/ts/pid_state.h"
#include "demuxer/ts/ts_pmt.h"

namespace nml_demuxer
{
namespace ts
{ 

TS_Pat::TS_Pat(RegisterInterface* cb) :
    cb_(cb),
    version_(-1)
{
    
}

 // See ITU H.222 standard 2.4.4.3 Program Association Table
Status TS_Pat::parsePsiSection(nml_reader::BitReader* bit_reader)
{
    // Read the fixed section length.
    int table_id;
    int section_syntax_indicator;
    int dummy_zero;
    int reserved;
    int section_length;
    int transport_stream_id;
    int version_number;
    int current_next_indicator;
    int section_number;
    int last_section_number;
    TS_CHECK(bit_reader->readBits(8, &table_id));
    TS_CHECK(bit_reader->readBits(1, &section_syntax_indicator));
    TS_CHECK(bit_reader->readBits(1, &dummy_zero));
    TS_CHECK(bit_reader->readBits(2, &reserved));
    TS_CHECK(bit_reader->readBits(12, &section_length));
    TS_CHECK(section_length >= 5);
    TS_CHECK(section_length <= 1021);
    TS_CHECK(bit_reader->readBits(16, &transport_stream_id));
    TS_CHECK(bit_reader->readBits(2, &reserved));
    TS_CHECK(bit_reader->readBits(5, &version_number));
    TS_CHECK(bit_reader->readBits(1, &current_next_indicator));
    TS_CHECK(bit_reader->readBits(8, &section_number));
    TS_CHECK(bit_reader->readBits(8, &last_section_number));
    section_length -= 5;

    //error check
    TS_CHECK(table_id == 0x0);
    TS_CHECK(section_syntax_indicator);
    TS_CHECK(!dummy_zero);

    TS_CHECK((section_length % 4) == 0);
    int pmt_pid_count = (section_length - 4) / 4;

    std::vector<int> program_number_array(pmt_pid_count);
    std::vector<int> pmt_pid_array(pmt_pid_count);
    for (int k = 0; k < pmt_pid_count; k++) 
    {
        int reserved;
        TS_CHECK(bit_reader->readBits(16, &program_number_array[k]));
        TS_CHECK(bit_reader->readBits(3, &reserved));
        TS_CHECK(bit_reader->readBits(13, &pmt_pid_array[k]));
    }
    int crc32;
    TS_CHECK(bit_reader->readBits(32, &crc32));

    if (!current_next_indicator) 
    {
        TS_LOG(LOG_LEVEL_INFO)
            << "Not supported: received a PAT yet";
        return NML_OK;
    }

    // Ignore the program table if it hasn't changed.
    if (version_number == version_)
        return NML_OK;

    if (pmt_pid_count > 1) 
    {
        TS_LOG(LOG_LEVEL_INFO)
            << "Multiple programs in the TS stream";
        return NML_ERROR_INVALID_STREAM;
    }

    //register the PMT.
#if !defined(NOMAD_DEBUG)
    int expected_version_number = version_number;
    if (version_ >= 0)
    {
        expected_version_number = (version_ + 1) % 32;
    }
#endif

    for (int k = 0; k < pmt_pid_count; k++)
    {
        if (program_number_array[k] != 0) 
        {
            cb_->registerPmtCB(program_number_array[k], pmt_pid_array[k]);
            break;
        }
    }
    version_ = version_number;

    return NML_OK;
}

void TS_Pat::resetPsiSection()
{
    version_ = -1;
}

} //namespace ts
} //namespace demuxer
