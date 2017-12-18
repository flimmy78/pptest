
//=======================================================================
//
//  [11/18/2013 kwanghyun won]
//
//  MPEG Transport Stream Parser
//=======================================================================

#include "demuxer/ts/ts_packet_parser.h"

#include <cstring>
#include <iostream>

#include "base/debug.h"
#include "demuxer/ts/ts_pat.h"


namespace nml_demuxer
{
namespace ts
{


//////////////////////////////////////////////////////////////////////////
// static function
uint32 TsPacket::sync(const uint8* buffer, int size)
{
    int k = 0;
    for ( ; k < size; k++)
    {
        bool is_header = true;
        for (int i = 0; i< 4; i++)
        {
            int index = k + i * kTsPacketSize;
            if (index >= size)
                break;

            if (buffer[index] != kTsSyncByte)
            {

                TS_LOG(LOG_LEVEL_TS)
                    << "Invalid TS header"
                    << "ByteSync" << index << ": "
                    << std::hex << static_cast<int>(buffer[index]) << std::dec;

                is_header = false;
                break;
            }
        }

        if (is_header)
            break;
    }

    return k;
}


Status TsPacket::parse(const uint8* buffer, int size, int packet_number)
{
    if (size < kTsPacketSize)
    {
        TS_LOG(LOG_LEVEL_TS)
                << "buffer not full ts packet"
                << "buffer size" << size;
        return NML_ERROR_INVALID_STREAM;
    }

    if ( buffer[0] != kTsSyncByte)
    {
        TS_LOG(LOG_LEVEL_TS) << "Not on a TS syncword:"
            << " buf[0]="
            << std::hex << static_cast<int>(buffer[0]) << std::dec;
        return NML_ERROR_INVALID_STREAM;
    }

    this->packetNumber = packet_number;
    this->tsHeader = nullptr;
    Status status = parseHeader(buffer);

    if (status != NML_OK)
    {
        TS_LOG(LOG_LEVEL_TS)
            << "fail parse ts header";
        return NML_ERROR_INVALID_STREAM;
    }

    return NML_OK;
}


//////////////////////////////////////////////////////////////////////////
// private member function
#if 0
Status TsPacket::parseHeader(const uint8* buffer)
{
    nml_reader::BitReader bit_reader(buffer, kTsPacketSize);
    payload = buffer;
    payloadSize = kTsPacketSize;

	tsHeader = const_cast<uint8*>(buffer);
	adaptationField = nullptr;

    // Read TS header: 4 bytes.
    int sync_byte;
    int transport_error_indicator;
    int payload_unit_start_indicator;
    int transport_priority;
    int transport_scrambling_control;
    int adaptation_field_control;

    TS_CHECK(bit_reader.readBits(8, &sync_byte));
    TS_CHECK(bit_reader.readBits(1, &transport_error_indicator));
    TS_CHECK(bit_reader.readBits(1, &payload_unit_start_indicator));
    TS_CHECK(bit_reader.readBits(1, &transport_priority));
    TS_CHECK(bit_reader.readBits(13, &pid));
    TS_CHECK(bit_reader.readBits(2, &transport_scrambling_control));
    TS_CHECK(bit_reader.readBits(2, &adaptation_field_control));
    TS_CHECK(bit_reader.readBits(4, &continuityCounter));
    payloadUnitStartIndicator = (payload_unit_start_indicator != 0);
    payload += 4;
    payloadSize -= 4;

    //no adaptation field.
    discontinuityIndicator = false;
    randomAccessIndicator = false;

    // Done if no adaptation field.
    if ((adaptation_field_control & 0x2) == 0)
        return NML_OK;

    // Read the adaptation field
    int adaptation_field_length;
    TS_CHECK(bit_reader.readBits(8, &adaptation_field_length));
    TS_LOG(LOG_LEVEL_INFO)
        << "adaptation_field_length = "<< adaptation_field_length;
    payload += 1;
    payloadSize -= 1;
    if ((adaptation_field_control & 0x1) == 0 &&
        adaptation_field_length != 183)
    {
        TS_LOG(LOG_LEVEL_ERROR)
            << "adaptation_field_length = "<< adaptation_field_length;

        return NML_ERROR_INVALID_STREAM;
    }

    if ((adaptation_field_control & 0x1) == 1 &&
        adaptation_field_length > 182)
    {
        TS_LOG(LOG_LEVEL_ERROR)
            << "adaptation_field_length = "<< adaptation_field_length;

        return NML_ERROR_INVALID_STREAM;
    }

    if (adaptation_field_length == 0)
        return NML_OK;

	adaptationField =  const_cast<uint8*>(payload);
    Status status = parseAdaptationField(&bit_reader, adaptation_field_length);
    payload += adaptation_field_length;
    payloadSize -= adaptation_field_length;

    return status;
}
#else

Status TsPacket::parseHeader(const uint8* buffer)
{
    payload = buffer;
    payloadSize = kTsPacketSize;

    tsHeader = const_cast<uint8*>(buffer);
    adaptationField = nullptr;

    // Read TS header: 4 bytes.
    //int sync_byte = buffer[0];
    //int transport_error_indicator = (buffer[1] & 0x80) >> 7;
    int payload_unit_start_indicator = (buffer[1] & 0x40) >> 6;
    //int transport_priority = (buffer[1] & 0x20) >> 5;
    this->pid = ((buffer[1] & 31) << 8) | buffer[2];
    //int transport_scrambling_control = (buffer[3] & 0xC0);
    int adaptation_field_control = (buffer[3] & 0x30) >> 4;
    this->continuityCounter = (buffer[3] & 0xF);

    payloadUnitStartIndicator = (payload_unit_start_indicator != 0);
    payload += 4;
    payloadSize -= 4;

    //no adaptation field.
    discontinuityIndicator = false;
    randomAccessIndicator = false;

    // Done if no adaptation field.
    if ((adaptation_field_control & 0x2) == 0)
        return NML_OK;

    // Read the adaptation field
    int adaptation_field_length = buffer[4];

    TS_LOG(LOG_LEVEL_TS)
        << "adaptation_field_length = "<< adaptation_field_length;

    payload += 1;
    payloadSize -= 1;
    if ((adaptation_field_control & 0x1) == 0 &&
        adaptation_field_length != 183)
    {
        TS_LOG(LOG_LEVEL_ERROR)
            << "adaptation_field_length = "<< adaptation_field_length;

        return NML_ERROR_INVALID_STREAM;
    }

    if ((adaptation_field_control & 0x1) == 1 &&
        adaptation_field_length > 182)
    {
        TS_LOG(LOG_LEVEL_ERROR)
            << "adaptation_field_length = "<< adaptation_field_length;

        return NML_ERROR_INVALID_STREAM;
    }

    if (adaptation_field_length == 0)
        return NML_OK;

    adaptationField =  const_cast<uint8*>(payload);
    Status status = parseAdaptationField(adaptationField);

    payload += adaptation_field_length;
    payloadSize -= adaptation_field_length;

    return status;
}
#endif

#if 0
Status TsPacket::parseAdaptationField(nml_reader::BitReader* bit_reader,
                                         int adaptation_field_length)
{
    int adaptation_field_start_marker = bit_reader->bitsAvailable() / 8;

    int discontinuity_indicator;
    int random_access_indicator;
    int elementary_stream_priority_indicator;
    int pcr_flag;
    int opcr_flag;
    int splicing_point_flag;
    int transport_private_data_flag;
    int adaptation_field_extension_flag;

    TS_CHECK(bit_reader->readBits(1, &discontinuity_indicator));
    TS_CHECK(bit_reader->readBits(1, &random_access_indicator));
    TS_CHECK(bit_reader->readBits(1, &elementary_stream_priority_indicator));
    TS_CHECK(bit_reader->readBits(1, &pcr_flag));
    TS_CHECK(bit_reader->readBits(1, &opcr_flag));
    TS_CHECK(bit_reader->readBits(1, &splicing_point_flag));
    TS_CHECK(bit_reader->readBits(1, &transport_private_data_flag));
    TS_CHECK(bit_reader->readBits(1, &adaptation_field_extension_flag));
    discontinuityIndicator = (discontinuity_indicator != 0);
    randomAccessIndicator = (random_access_indicator != 0);


    if (pcr_flag)
    {
        //int64 pcr_base;
        int reserved;
        //int pcr_extension;

        TS_CHECK(bit_reader->readBits(33, &pcrBase));
        TS_CHECK(bit_reader->readBits(6, &reserved));
        TS_CHECK(bit_reader->readBits(9, &pcrExtension));

        double sys_time = ((double)pcrBase / 90000.0f) +
                     ((double)pcrExtension / 27000000.0f);

        pcrTime = sys_time;
        //NMLOG_INFO("sys_time:%0.3f, base:%lld, ext:%d",sys_time, pcrBase, pcrExtension);
        TS_LOG(LOG_LEVEL_TS_TIME) <<
            green << "sys_time : " << sys_time << reset;
    }

    if (opcr_flag)
    {
        int64 original_program_clock_reference_base;
        int reserved;
        int original_program_clock_reference_extension;

        TS_CHECK(bit_reader->readBits(33, &original_program_clock_reference_base));
        TS_CHECK(bit_reader->readBits(6, &reserved));
        TS_CHECK(bit_reader->readBits(9, &original_program_clock_reference_extension));
    }

    if (splicing_point_flag)
    {
        int splice_countdown;
        TS_CHECK(bit_reader->readBits(8, &splice_countdown));
    }

    if (transport_private_data_flag)
    {
        int transport_private_data_length;
        TS_CHECK(bit_reader->readBits(8, &transport_private_data_length));
        TS_CHECK(bit_reader->skipBits(8 * transport_private_data_length));
    }

    if (adaptation_field_extension_flag)
    {
        int adaptation_field_extension_length;
        TS_CHECK(bit_reader->readBits(8, &adaptation_field_extension_length));
        TS_CHECK(bit_reader->skipBits(8 * adaptation_field_extension_length));
    }

    // The rest of the adaptation field should be stuffing bytes.
    int adaptation_field_remaining_size = adaptation_field_length -
        (adaptation_field_start_marker - bit_reader->bitsAvailable() / 8);

    TS_CHECK(adaptation_field_remaining_size >= 0);

    for (int k = 0; k < adaptation_field_remaining_size; k++)
    {
        int stuffing_byte;
        TS_CHECK(bit_reader->readBits(8, &stuffing_byte));
        TS_CHECK(stuffing_byte == 0xff);
    }

    TS_LOG(LOG_LEVEL_TS)
        << "random_access_indicator =" << randomAccessIndicator;
    return NML_OK;
}
#else
Status TsPacket::parseAdaptationField(const uint8* adaptationField)
{

    int discontinuity_indicator = (adaptationField[0] & 0x80) >> 7;
    int random_access_indicator = (adaptationField[0] & 0x40) >> 6;
    int pcr_flag                = (adaptationField[0] & 0x10) >> 4;

    discontinuityIndicator = (discontinuity_indicator != 0);
    randomAccessIndicator = (random_access_indicator != 0);

    if (pcr_flag)
    {
        pcrBase =
            ((uint64)adaptationField[1] << 25) |
            ((uint64)adaptationField[2] << 17) |
            ((uint64)adaptationField[3] << 9) |
            ((uint64)adaptationField[4] << 1) |
            (((uint64)adaptationField[5] & 0x80) >> 7);

        pcrExtension =
            (((uint64)adaptationField[5] & 0x1) << 8) |
            (uint64)adaptationField[6];

        double sys_time = ((double)pcrBase / 90000.0f) +
            ((double)pcrExtension / 27000000.0f);

        pcrTime = sys_time;
        //NMLOG_INFO("sys_time:%0.3f, base:%lld, ext:%d",sys_time, pcrBase, pcrExtension);
        TS_LOG(LOG_LEVEL_TS_TIME)
            << "sys_time : " << sys_time ;
    }
    TS_LOG(LOG_LEVEL_TS)
        << "random_access_indicator =" << randomAccessIndicator;
    return NML_OK;
}
#endif

//change "discontinuity_indicator" value to true
void TsPacket::fixDiscontinuityIndicator(void) const
{
	if (adaptationField != nullptr)
	{
		uint8 data = adaptationField[2];
		if (data != 0x00)
			adaptationField[2] = data | 0x80;

		TS_LOG(1)
				<< " before:[0x"<< std::hex << static_cast<int>(data) << std::dec << "]"
				<< " after fix:[0x"<< std::hex << static_cast<int>(adaptationField[2]) << std::dec << "]";
	}
}

void TsPacket::fixContinuityCounter(int new_continuity) const
{
	if (tsHeader != nullptr)
	{
		uint8 data = tsHeader[3];
		tsHeader[3] = (tsHeader[3] & 0xF0) + (new_continuity & 0x0F);
		TS_LOG(1) << " before:[0x"<< std::hex << static_cast<int>(data) << std::dec << "]"
					<< " after:[0x"<< std::hex << static_cast<int>(tsHeader[3]) << std::dec << "]";
	}
}

} //namespace ts
} //namespace nml_demuxer


