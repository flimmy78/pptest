
//=======================================================================
//
//  [11/18/2013 kwanghyun won]
//
//  MPEG Packetized Elementary Stream Parser
//
//  [12/23/2013] - reimplementaion with Bit_Reader
//=======================================================================


#include "base/debug.h"
#include "base/media_time.h"

#include "demuxer/ts/pes_parser.h"
#include "reader/bit_reader.h"

namespace nml_demuxer
{
namespace ts
{

#if 0
// |time| + k * (2 ^ 33)
static int64 unrollTimestamp(int64 previous_unrolled_time, int64 time)
{
    const int nbits = 33;

    NM_ASSERT( (time >> nbits) == 0);

    int64 previous_unrolled_time_high =
        (previous_unrolled_time >> nbits);
    int64 time0 = ((previous_unrolled_time_high - 1) << nbits) | time;
    int64 time1 = ((previous_unrolled_time_high + 0) << nbits) | time;
    int64 time2 = ((previous_unrolled_time_high + 1) << nbits) | time;

    int64 diff0 = time0 - previous_unrolled_time;
    int64 diff1 = time1 - previous_unrolled_time;
    int64 diff2 = time2 - previous_unrolled_time;
    if (diff0 < 0)
        diff0 = -diff0;
    if (diff1 < 0)
        diff1 = -diff1;
    if (diff2 < 0)
        diff2 = -diff2;

    int64 unrolled_time;
    int64 min_diff;
    if (diff1 < diff0)
    {
        unrolled_time = time1;
        min_diff = diff1;
    }
    else
    {
        unrolled_time = time0;
        min_diff = diff0;
    }
    if (diff2 < min_diff)
        unrolled_time = time2;

    return unrolled_time;
}

static bool isTimestampSectionValid(int64 timestamp_section)
{
    // |pts_section| has 40 bits:
    // See ITU H.222 standard - PES section.
    return ((timestamp_section & 0x1) != 0) &&
           ((timestamp_section & 0x10000) != 0) &&
           ((timestamp_section & 0x100000000) != 0);
}
#endif
int64 PESParser::convertTimestampSectionToTimestamp(int64 timestamp_section)
{
    return (((timestamp_section >> 33) & 0x7) << 30) |
           (((timestamp_section >> 17) & 0x7fff) << 15) |
           (((timestamp_section >> 1) & 0x7fff) << 0);
}


PESParser::PESParser(VideoPtsCB cb) :
    waitForPusi(true),
    videoPtsCB( cb )
#if 0
    previousPtsValid(false),
    previousPts(0),
    previousDtsValid(false),
    previousDts(0),
#endif
{
}

PESParser::~PESParser()
{
    //NM_SAFE_DELETE( esParser );
}

//////////////////////////////////////////////////////////////////////////
// Ts_Section implementation

Status PESParser::parse(bool payload_unit_start_indicator,
                        const uint8* buffer,
                        size_t size)
{
#if 0
    // Ignore partial PES.
    if (waitForPusi && !payload_unit_start_indicator)
        return NML_OK;

    Status parse_result = NML_OK;
    if (payload_unit_start_indicator)
    {
        int raw_pes_size;
        const uint8* raw_pes;
        pesByteQueue.peek(&raw_pes, &raw_pes_size);
        if (raw_pes_size > 0)
            parse_result = emit(true);

        resetPesState();

        // Update the state.
        waitForPusi = false;
    }

    // Add the data
    if (size > 0)
        pesByteQueue.push(buffer, size);

    return (parse_result && emit(false));

#else
    if (waitForPusi && !payload_unit_start_indicator)
        return NML_OK;

    Status parse_result = NML_OK;
    if (payload_unit_start_indicator)
    {
        int raw_pes_size = static_cast<int>(size);
        const uint8* raw_pes = buffer;

        parse_result = parseInternal(raw_pes, raw_pes_size);
        resetPesState();
    }
    return parse_result;
#endif
    return NML_OK;
}

void PESParser::flush()
{
    //emit(true);
    //esParser->flush();
}

void PESParser::reset()
{
    resetPesState();
    //esParser->reset();

#if 0
    previousPtsValid = false;
    previousPts       = kNoTimestamp;
    previousDtsValid = false;
    previousDts       = kNoTimestamp;
#endif
}

//////////////////////////////////////////////////////////////////////////
// private function

Status PESParser::emit(bool emit_for_unknown_size)
{
    int raw_pes_size = 0;
    const uint8* raw_pes = nullptr;
    //pesByteQueue.peek(&raw_pes, &raw_pes_size);

    // PES 패킷은 최소 6 bytes.
    if (raw_pes_size < 6)
        return NML_OK;

    int pes_packet_length =
        (static_cast<int>(raw_pes[4]) << 8) |
        (static_cast<int>(raw_pes[5]));

    if ((pes_packet_length == 0 && !emit_for_unknown_size) )
        //|| (pes_packet_length != 0 && raw_pes_size < pes_packet_length + 6))
    {
        return NML_OK;
    }

    TS_LOG(LOG_LEVEL_PES) << "pes_packet_length =" << pes_packet_length;
    // Parse the packet.
    Status parse_result = parseInternal(raw_pes, raw_pes_size);

    resetPesState();
    return parse_result;
}

Status PESParser::parseInternal(const uint8* raw_pes, int raw_pes_size)
{
    // Read the pes_packet_length (6 bytes).

    int packet_start_code_prefix = (raw_pes[0] | raw_pes[1] | raw_pes[2]);
    int stream_id = raw_pes[3];
    //int pes_packet_length = (raw_pes[4] | raw_pes[5] );

    TS_CHECK(packet_start_code_prefix == kPesStartCode);
    TS_LOG(LOG_LEVEL_PES) << " stream_id= " << std::hex << stream_id << std::dec;

    // 모르는 stream id를 가진 PES는 무시해버림.
    // See ITU H.222 Table 2-22 "Stream_id assignments"
    bool is_audio_stream_id = ((stream_id & 0xe0) == 0xc0);
    bool is_video_stream_id = ((stream_id & 0xf0) == 0xe0);
    if (!is_audio_stream_id && !is_video_stream_id)
        return NML_OK;

    // Read "pes_header_data_length".
    int pts_flag = (raw_pes[7] & 0xC0) >> 7;;
    //int pes_header_data_length = raw_pes[8];
    int dts_flag = (raw_pes[7] & 0x40) >> 6;

    int64 pts = kNoTimestamp;
    int64 dts = kNoTimestamp;

    if (pts_flag == 1)
    {
        pts =
            ((int64)raw_pes[9]  << 32) |
            ((int64)raw_pes[10] << 24) |
            ((int64)raw_pes[11] << 16) |
            ((int64)raw_pes[12] << 8) |
            ((int64)raw_pes[13] );

        pts = convertTimestampSectionToTimestamp(pts);
    }

    if (dts_flag == 1)
    {
        dts =
            ((int64)raw_pes[14]  << 32) |
            ((int64)raw_pes[15] << 24) |
            ((int64)raw_pes[16] << 16) |
            ((int64)raw_pes[17] << 8) |
            ((int64)raw_pes[18] );

        dts = convertTimestampSectionToTimestamp(dts);
    }

    //FIXME: pts역전 현상을 제거하기 위해 dts와 비교하여 보정.
    int64 timestamp = kNoTimestamp;
    if (dts < pts && dts != kNoTimestamp && dts != 0)
    {
        timestamp = dts;
    }
    else
    {
        timestamp = pts;
    }

    //FIXME: video packet의 timestamp만 보낸다.
    if (is_video_stream_id == true)
        videoPtsCB(timestamp);

    return NML_OK;
}

#if 0
Status PESParser::parseInternal(const uint8* raw_pes, int raw_pes_size)
{
    nml_reader::BitReader bit_reader(raw_pes, raw_pes_size);

    // Read the pes_packet_length (6 bytes).
    int packet_start_code_prefix;
    int stream_id;
    int pes_packet_length;
    TS_CHECK( bit_reader.readBits(24, &packet_start_code_prefix) );
    TS_CHECK(bit_reader.readBits(8, &stream_id));
    TS_CHECK(bit_reader.readBits(16, &pes_packet_length));

    TS_CHECK(packet_start_code_prefix == kPesStartCode);
    TS_LOG(LOG_LEVEL_PES) << " stream_id= " << std::hex << stream_id << std::dec;
    //NMLOG_INFO("stream_id = :%d",stream_id);

    if (pes_packet_length == 0)
        pes_packet_length = bit_reader.bitsAvailable() / 8;

    // 모르는 stream id를 가진 PES는 무시해버림.
    // See ITU H.222 Table 2-22 "Stream_id assignments"
    bool is_audio_stream_id = ((stream_id & 0xe0) == 0xc0);
    bool is_video_stream_id = ((stream_id & 0xf0) == 0xe0);
    if (!is_audio_stream_id && !is_video_stream_id)
        return NML_OK;

    // Read "pes_header_data_length".
    int dummy_2;
    int PES_scrambling_control;
    int PES_priority;
    int data_alignment_indicator;
    int copyright;
    int original_or_copy;
    int pts_dts_flags;
    int escr_flag;
    int es_rate_flag;
    int dsm_trick_mode_flag;
    int additional_copy_info_flag;
    int pes_crc_flag;
    int pes_extension_flag;
    int pes_header_data_length;

    TS_CHECK(bit_reader.readBits(2, &dummy_2));
    TS_CHECK(dummy_2 == 0x2);
    TS_CHECK(bit_reader.readBits(2, &PES_scrambling_control));
    TS_CHECK(bit_reader.readBits(1, &PES_priority));
    TS_CHECK(bit_reader.readBits(1, &data_alignment_indicator));
    TS_CHECK(bit_reader.readBits(1, &copyright));
    TS_CHECK(bit_reader.readBits(1, &original_or_copy));
    TS_CHECK(bit_reader.readBits(2, &pts_dts_flags));
    TS_CHECK(bit_reader.readBits(1, &escr_flag));
    TS_CHECK(bit_reader.readBits(1, &es_rate_flag));
    TS_CHECK(bit_reader.readBits(1, &dsm_trick_mode_flag));
    TS_CHECK(bit_reader.readBits(1, &additional_copy_info_flag));
    TS_CHECK(bit_reader.readBits(1, &pes_crc_flag));
    TS_CHECK(bit_reader.readBits(1, &pes_extension_flag));
    TS_CHECK(bit_reader.readBits(8, &pes_header_data_length));
    int pes_header_start_size = bit_reader.bitsAvailable() / 8;

    // ES payload 의 사이즈와 offset계산.
    int es_size = pes_packet_length - 3 - pes_header_data_length;
    //int es_offset = 6 + 3 + pes_header_data_length;
    TS_CHECK(es_size >= 0);
    //TS_CHECK(es_offset + es_size <= raw_pes_size);

    // Read timestamp(pts,dts)관련
    bool is_pts_valid = false;
    bool is_dts_valid = false;
    int64 pts_section = 0;
    int64 dts_section = 0;
    if (pts_dts_flags == 0x2)
    {
        TS_CHECK(bit_reader.readBits(40, &pts_section));
        TS_CHECK((((pts_section >> 36) & 0xf) == 0x2) &&
                 isTimestampSectionValid(pts_section));
        is_pts_valid = true;
    }
    if (pts_dts_flags == 0x3)
    {
        TS_CHECK(bit_reader.readBits(40, &pts_section));
        TS_CHECK(bit_reader.readBits(40, &dts_section));
        TS_CHECK((((pts_section >> 36) & 0xf) == 0x3) &&
                 isTimestampSectionValid(pts_section));
        TS_CHECK((((dts_section >> 36) & 0xf) == 0x1) &&
                 isTimestampSectionValid(dts_section));
        is_pts_valid = true;
        is_dts_valid = true;
    }

    // Convert and unroll the timestamps.
    base::DeltaTime media_pts(base::DeltaTime::noTimeStamp());
    base::DeltaTime media_dts(base::DeltaTime::noTimeStamp());

    if (is_pts_valid)
    {
        int64 pts = convertTimestampSectionToTimestamp(pts_section);
        if (previousPtsValid)
            pts = unrollTimestamp(previousPts, pts);

        previousPts = pts;
        previousPtsValid = true;

        media_pts = base::DeltaTime::fromMicroseconds( (1000 * pts) / 90);
    }

    if (is_dts_valid)
    {
        int64 dts = convertTimestampSectionToTimestamp(dts_section);
        if (previousDtsValid)
            dts = unrollTimestamp(previousDts, dts);

        previousDts = dts;
        previousDtsValid = true;

        media_dts = base::DeltaTime::fromMicroseconds( (1000 * dts) / 90);
    }

    // Discard the rest of the PES packet header.
    NM_ASSERT(bit_reader.bitsAvailable() % 8 == 0);
    int pes_header_remaining_size = pes_header_data_length -
                                    (pes_header_start_size - bit_reader.bitsAvailable() / 8);
    TS_CHECK(pes_header_remaining_size >= 0);

    // Read the PES packet.
    TS_LOG(LOG_LEVEL_PES)
            << "Emit a reassembled PES:"
            << " size=" << es_size
            << " pts=" << media_pts.inMilliseconds()
            << " dts=" << media_dts.inMilliseconds();

#if 1
    videoPtsCB(media_pts.inMilliseconds());

    return NML_OK;
#else
    return esParser->parse(&raw_pes[es_offset], es_size, media_pts, media_dts);
#endif
}
#endif

void PESParser::resetPesState()
{
    //pesByteQueue.reset();
    waitForPusi = true;
}

} //namespace ts
} //namespace nml_demuxer
