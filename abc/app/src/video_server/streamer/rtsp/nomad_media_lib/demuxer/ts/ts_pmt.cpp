//=======================================================================
// 
//  [11/25/2013 kwanghyun won]
//                                                     
//======================================================================= 

#include "demuxer/ts/ts_pmt.h"

#include <cstdlib>
#include <cstring>
#include <map>

#include "base/debug.h"
#include "demuxer/ts/pes_parser.h"
#include "demuxer/ts/es_parser.h"

namespace nml_demuxer
{
namespace ts
{ 

TSPmt::TSPmt(RegisterInterface* cb) :
    cb_(cb)
{

}

Status TSPmt::parsePsiSection(nml_reader::BitReader* bit_reader)
{
    // Read up to |last_section_number|.
    int table_id;
    int section_syntax_indicator;
    int dummy_zero;
    int reserved;
    int section_length;
    int program_number;
    int version_number;
    int current_next_indicator;
    int section_number;
    int last_section_number;
    TS_CHECK(bit_reader->readBits(8, &table_id));
    TS_CHECK(bit_reader->readBits(1, &section_syntax_indicator));
    TS_CHECK(bit_reader->readBits(1, &dummy_zero));
    TS_CHECK(bit_reader->readBits(2, &reserved));
    TS_CHECK(bit_reader->readBits(12, &section_length));
    int section_start_marker = bit_reader->bitsAvailable() / 8;

    TS_CHECK(bit_reader->readBits(16, &program_number));
    TS_CHECK(bit_reader->readBits(2, &reserved));
    TS_CHECK(bit_reader->readBits(5, &version_number));
    TS_CHECK(bit_reader->readBits(1, &current_next_indicator));
    TS_CHECK(bit_reader->readBits(8, &section_number));
    TS_CHECK(bit_reader->readBits(8, &last_section_number));

    TS_CHECK(table_id == 0x2);
    TS_CHECK(section_syntax_indicator);
    TS_CHECK(!dummy_zero);
    TS_CHECK(section_length <= 1021);
    TS_CHECK(section_number == 0);
    TS_CHECK(last_section_number == 0);

    int pcr_pid;
    int program_info_length;
    TS_CHECK(bit_reader->readBits(3, &reserved));
    TS_CHECK(bit_reader->readBits(13, &pcr_pid));
    TS_CHECK(bit_reader->readBits(4, &reserved));
    TS_CHECK(bit_reader->readBits(12, &program_info_length));
    TS_CHECK(program_info_length < 1024);

    // Read the program info descriptor.
    // section 2.6 of ISO-13818.
    TS_CHECK(bit_reader->skipBits(8 * program_info_length));

    // Read the ES description table.
    // (4 bytes = size of the CRC).
    uint32 pid_map_end_marker = (uint32)(section_start_marker - section_length + 4);
    std::map<int, int> pid_map;
    while (bit_reader->bitsAvailable() > 8 * pid_map_end_marker) 
    {
        int stream_type;
        int reserved;
        int pid_es;
        int es_info_length;
        TS_CHECK(bit_reader->readBits(8, &stream_type));
        TS_CHECK(bit_reader->readBits(3, &reserved));
        TS_CHECK(bit_reader->readBits(13, &pid_es));
        TS_CHECK(bit_reader->readBits(4, &reserved));
        TS_CHECK(bit_reader->readBits(12, &es_info_length));

        pid_map.insert(std::pair<int, int>(pid_es, stream_type));

        // Read the ES info descriptors.
        TS_CHECK(bit_reader->skipBits(8 * es_info_length));
    }

    // Read the CRC.
    int crc32;
    TS_CHECK(bit_reader->readBits(32, &crc32));

    for (auto it : pid_map)
    {
        cb_->registerPesCB(it.first, it.second);
    }
    return NML_OK;
}

} //namespace ts
} //namespace nml_demuxer

