

#include "demuxer/ts/es_avc_parser.h"



namespace nml_demuxer
{
namespace ts
{

EsAvcParser::EsAvcParser(EmitFrameInterface* cb) :
    esPosition(0),
    currentAccessUnitPostition(-1),
    currentNalPosition(-1),
    isKeyFrame(false)
{
    emitFrameCB = cb;
}

EsAvcParser::~EsAvcParser()
{
}

Status EsAvcParser::parse(const uint8* buffer, int size, 
                          base::DeltaTime pts,
                          base::DeltaTime dts)
{
    if (dts == base::DeltaTime::noTimeStamp() &&
        pts == base::DeltaTime::noTimeStamp()) 
    {
        TS_LOG(LOG_LEVEL_ERROR)
            << "no time stamp!";
        return NML_ERROR_INVALID_STREAM;
    }

    TimeStamp timestamp;
    timestamp.pts = pts;
    timestamp.dts = (dts != base::DeltaTime::noTimeStamp()) ? dts : pts;


    int raw_es_size;
    const uint8* raw_es;
    esByteQueue.peek(&raw_es, &raw_es_size);
    timeStampList.push_back(
        std::pair<int, TimeStamp>(raw_es_size, timestamp));

    // Add bytes to the ES queue.
    esByteQueue.push(buffer, size);

    // Add NALs 
    if (!parseInternal())
        return NML_ERROR_INVALID_STREAM;

    // Discard emitted frames
    int skip_count =
        (currentAccessUnitPostition >= 0) ? currentAccessUnitPostition : esPosition;

    discardEs(skip_count);

    return NML_OK;
}

void EsAvcParser::flush()
{
    if (currentAccessUnitPostition < 0)
        return;

    // Force emitting the last access unit.
    int next_audio_position;
    const uint8* raw_es;
    esByteQueue.peek(&raw_es, &next_audio_position);
    flushFrame(next_audio_position);
    currentNalPosition = -1;
    
    startFrame(-1);

    // Discard the emitted frame.
    discardEs(next_audio_position);
}


void EsAvcParser::reset()
{
    TS_LOG(LOG_LEVEL_INFO) << "EsAvcH264::reset";

    esByteQueue.reset();
    timeStampList.clear();
    esPosition = 0;
    currentNalPosition = -1;
    
    startFrame(-1);
    //last_video_decoder_config_ = VideoDecoderConfig();
}


Status EsAvcParser::parseInternal() 
{
    int raw_es_size;
    const uint8* raw_es;
    esByteQueue.peek(&raw_es, &raw_es_size);

    //NM_ASSERT(esPosition > 0);
    //NM_ASSERT(esPosition < raw_es_size);

    for ( ; esPosition < raw_es_size - 4; esPosition++) 
    {
        // syncword  00 00 00 01 or 00 00 01
        if (raw_es[esPosition + 0] != 0 || raw_es[esPosition + 1] != 0)
            continue;

        int syncword_length = 0;
        if (raw_es[esPosition + 2] == 0 && raw_es[esPosition + 3] == 1)
            syncword_length = 4;
        else if (raw_es[esPosition + 2] == 1)
            syncword_length = 3;
        else
            continue;

        // Parse the current NAL.
        if (currentNalPosition>= 0) 
        {
            int nal_size = esPosition - currentNalPosition;
            NM_ASSERT(nal_size > 0);
            TS_CHECK( nalParser(&raw_es[currentNalPosition], nal_size) );
        }
        currentNalPosition = esPosition + syncword_length;

        // Retrieve the NAL type.
        int nal_header = raw_es[currentNalPosition];
        int forbidden_zero_bit = (nal_header >> 7) & 0x1;
        TS_CHECK(forbidden_zero_bit == 0);
        NalUnitType nal_unit_type = static_cast<NalUnitType>(nal_header & 0x1f);


        TS_LOG(LOG_LEVEL_ES) 
            << "nal: offset=" << esPosition
            << " type=" << nal_unit_type;

        // Emit a frame if needed.
        if (nal_unit_type == kNalUnitTypeAUD)
            flushFrame(esPosition);

        // Skip the syncword.
        esPosition+= syncword_length;
    }

    return NML_OK;
}

// ISO/IEC 14496 - Part 10(40page): 7.3.1 NAL unit syntax
bool EsAvcParser::nalParser(const uint8* buffer, int size) 
{
    // Get the NAL header.
    if (size < 1) 
    {
        TS_LOG(LOG_LEVEL_ERROR) << "NalParser: incomplete NAL";
        return false;
    }
    int nal_header = buffer[0];
    buffer += 1;
    size -= 1;

    int forbidden_zero_bit = (nal_header >> 7) & 0x1;
    if (forbidden_zero_bit != 0)
        return false;

    int nal_ref_idc = (nal_header >> 5) & 0x3;
    int nal_unit_type = nal_header & 0x1f;

    // Process the NAL content.
    switch (nal_unit_type) 
    {
    case kNalUnitTypeSPS:
        TS_LOG(LOG_LEVEL_ES) << "NAL: SPS";
        // |nal_ref_idc| should not be 0 for a SPS.
        if (nal_ref_idc == 0)
            return false;

        if ( NML_OK != processSPS(buffer, size) )
            return false;
        else
            return true;

    case kNalUnitTypeIDRSlice:
        TS_LOG(LOG_LEVEL_ES) << "NAL: IDR slice";
        return true;

    case kNalUnitTypeNonIDRSlice:
        TS_LOG(LOG_LEVEL_ES) << "NAL: Non IDR slice";
        isKeyFrame = false;
        return true;

    case kNalUnitTypePPS:
        TS_LOG(LOG_LEVEL_ES) << "NAL: PPS";
        return true;

    case  kNalUnitTypeAUD:
        TS_LOG(LOG_LEVEL_ES) << "NAL: AUD";
        return true;

    case  kNalUnitTypeSEI:
        TS_LOG(LOG_LEVEL_ES) << "NAL: SEI";
        return true;

    default:
        TS_LOG(LOG_LEVEL_ES) << "NAL: " << nal_unit_type;
        return true;
    }

    //NOTREACHED();
    return false;
}

void EsAvcParser::flushFrame(int next_audio_position)
{
    // There is no current frame: start a new frame.
    if (currentAccessUnitPostition < 0) 
    {
        startFrame(next_audio_position);
        return;
    }

    // Get the access unit timing info.
    TimeStamp current_timestamp;
    while (!timeStampList.empty() &&
        timeStampList.front().first <= currentAccessUnitPostition)
    {
        current_timestamp = timeStampList.front().second;
        timeStampList.pop_front();
    }

    // Emit a frame.
    int raw_es_size;
    const uint8* raw_es;
    esByteQueue.peek(&raw_es, &raw_es_size);
    int access_unit_size = next_audio_position - currentAccessUnitPostition;

    StreamParserBuffer* stream_parser_buffer = 
            new StreamParserBuffer(&raw_es[currentAccessUnitPostition],
                                    access_unit_size,
                                    isKeyFrame);
    stream_parser_buffer->setDecodeTimestamp(current_timestamp.dts);
    stream_parser_buffer->setTimestamp(current_timestamp.pts);
    emitFrameCB->onEmitVideoFrame(stream_parser_buffer);

    // to the next AUD position.
    startFrame(next_audio_position);
}

void EsAvcParser::startFrame(int audio_position) 
{
    currentAccessUnitPostition = audio_position;
    isKeyFrame = (audio_position >= 0);
}

void EsAvcParser::discardEs(int bytes) 
{
    NM_ASSERT(bytes > 0);

    if (bytes == 0)
        return;

    // 현재 NAL ,AU position을 Update
    esPosition -= bytes;
    if (esPosition < 0)
        esPosition = 0;

    if (currentNalPosition >= 0) 
    {
        NM_ASSERT(currentNalPosition > bytes);
        currentNalPosition -= bytes;
    }

    if (currentAccessUnitPostition >= 0) 
    {
        NM_ASSERT(currentAccessUnitPostition > bytes);
        currentAccessUnitPostition -= bytes;
    }

    // Update the timing information 
    for (auto time_it : timeStampList)
    {
        time_it.first -= bytes;
    }

    // Discard bytes of ES.
    esByteQueue.pop(bytes);
}


// ISO/IEC 14496 - Part 10(41page): 7.3.2.1.1 Sequence parameter set daya syntax
Status EsAvcParser::processSPS(const uint8* buffer, int size)
{
    if (size <= 0)
        return NML_ERROR_INVALID_STREAM;

    //uint8* buffer_rbsp = new uint8[size];
    std::unique_ptr<uint8[]> buffer_rbsp( new uint8[size] );
    int rbsp_size = convertToRbsp(buffer, size, buffer_rbsp.get());

    BitReaderAVC bit_reader( buffer_rbsp.get(), rbsp_size);

    int profile_idc;
    int constraint_setX_flag;
    int level_idc;
    uint32 seq_parameter_set_id;
    uint32 log2_max_frame_num_minus4;
    uint32 pic_order_cnt_type;

    TS_CHECK(bit_reader.readBits(8, &profile_idc));
    TS_CHECK(bit_reader.readBits(8, &constraint_setX_flag));
    TS_CHECK(bit_reader.readBits(8, &level_idc));
    TS_CHECK(bit_reader.readBitsExpGolomb(&seq_parameter_set_id));

    if (profile_idc == 100 || profile_idc == 110 ||
        profile_idc == 122 || profile_idc == 244 ||
        profile_idc ==  44 || profile_idc ==  83 ||
        profile_idc ==  86 || profile_idc == 118 ||
        profile_idc == 128) 
    {
        uint32 chroma_format_idc;
        TS_CHECK(bit_reader.readBitsExpGolomb(&chroma_format_idc));
        if (chroma_format_idc == 3) 
        {
            int separate_colour_plane_flag;
            TS_CHECK(bit_reader.readBits(1, &separate_colour_plane_flag));
        }

        uint32 bit_depth_luma_minus8;
        uint32 bit_depth_chroma_minus8;
        int qpprime_y_zero_transform_bypass_flag;
        int seq_scaling_matrix_present_flag;

        TS_CHECK(bit_reader.readBitsExpGolomb(&bit_depth_luma_minus8));
        TS_CHECK(bit_reader.readBitsExpGolomb(&bit_depth_chroma_minus8));
        TS_CHECK(bit_reader.readBits(1, &qpprime_y_zero_transform_bypass_flag));
        TS_CHECK(bit_reader.readBits(1, &seq_scaling_matrix_present_flag));

        if (seq_scaling_matrix_present_flag) 
        {
            int skip_count = (chroma_format_idc != 3) ? 8 : 12;
            TS_CHECK(bit_reader.skipBits(skip_count));
        }
    }

    TS_CHECK(bit_reader.readBitsExpGolomb(&log2_max_frame_num_minus4));
    TS_CHECK(bit_reader.readBitsExpGolomb(&pic_order_cnt_type));

    // |pic_order_cnt_type| shall be in the range of 0 to 2.
    TS_CHECK(pic_order_cnt_type <= 2);
    if (pic_order_cnt_type == 0) 
    {
        uint32 log2_max_pic_order_cnt_lsb_minus4;
        TS_CHECK(bit_reader.readBitsExpGolomb(&log2_max_pic_order_cnt_lsb_minus4));
    }
    else if (pic_order_cnt_type == 1) 
    {
        int delta_pic_order_always_zero_flag;
        uint32 offset_for_non_ref_pic;
        uint32 offset_for_top_to_bottom_field;
        uint32 num_ref_frames_in_pic_order_cnt_cycle;
        TS_CHECK(bit_reader.readBits(1, &delta_pic_order_always_zero_flag));
        TS_CHECK(bit_reader.readBitsExpGolomb(&offset_for_non_ref_pic));
        TS_CHECK(bit_reader.readBitsExpGolomb(&offset_for_top_to_bottom_field));
        TS_CHECK(
            bit_reader.readBitsExpGolomb(&num_ref_frames_in_pic_order_cnt_cycle));

        for (uint32 i = 0; i < num_ref_frames_in_pic_order_cnt_cycle; i++) 
        {
            uint32 offset_for_ref_frame_codenum;
            TS_CHECK(bit_reader.readBitsExpGolomb(&offset_for_ref_frame_codenum));
        }
    }

    //get width, height
    uint32 num_ref_frames;
    int gaps_in_frame_num_value_allowed_flag;
    uint32 pic_width_in_mbs_minus1;
    uint32 pic_height_in_map_units_minus1;
    TS_CHECK(bit_reader.readBitsExpGolomb(&num_ref_frames));
    TS_CHECK(bit_reader.readBits(1, &gaps_in_frame_num_value_allowed_flag));
    TS_CHECK(bit_reader.readBitsExpGolomb(&pic_width_in_mbs_minus1));
    TS_CHECK(bit_reader.readBitsExpGolomb(&pic_height_in_map_units_minus1));

    int frame_mbs_only_flag;
    TS_CHECK(bit_reader.readBits(1, &frame_mbs_only_flag));
    if (!frame_mbs_only_flag)
    {
        int mb_adaptive_frame_field_flag;
        TS_CHECK(bit_reader.readBits(1, &mb_adaptive_frame_field_flag));
    }

    int direct_8x8_inference_flag;
    TS_CHECK(bit_reader.readBits(1, &direct_8x8_inference_flag));

    int frame_cropping_flag;
    uint32 frame_crop_left_offset = 0;
    uint32 frame_crop_right_offset = 0;
    uint32 frame_crop_top_offset = 0;
    uint32 frame_crop_bottom_offset = 0;
    TS_CHECK(bit_reader.readBits(1, &frame_cropping_flag));
    if (frame_cropping_flag)
    {
        TS_CHECK(bit_reader.readBitsExpGolomb(&frame_crop_left_offset));
        TS_CHECK(bit_reader.readBitsExpGolomb(&frame_crop_right_offset));
        TS_CHECK(bit_reader.readBitsExpGolomb(&frame_crop_top_offset));
        TS_CHECK(bit_reader.readBitsExpGolomb(&frame_crop_bottom_offset));
    }

    //VUI(Video Usablility Information)는 비디오의 확장된 정보를 알려주는 syntax
    int vui_parameters_present_flag;
    TS_CHECK(bit_reader.readBits(1, &vui_parameters_present_flag));
    int sar_width = 1;
    int sar_height = 1;
    if (vui_parameters_present_flag) 
    {
        int aspect_ratio_info_present_flag;
        TS_CHECK(bit_reader.readBits(1, &aspect_ratio_info_present_flag));
        if (aspect_ratio_info_present_flag) 
        {
            int aspect_ratio_idc;
            TS_CHECK(bit_reader.readBits(8, &aspect_ratio_idc));
            if (aspect_ratio_idc == kExtendedSar) 
            {
                TS_CHECK(bit_reader.readBits(16, &sar_width));
                TS_CHECK(bit_reader.readBits(16, &sar_height));
            }
            else if (aspect_ratio_idc < kSarTableSize) 
            {
                sar_width = kTableSarWidth[aspect_ratio_idc];
                sar_height = kTableSarHeight[aspect_ratio_idc];
            }
        }
    }

    if (sar_width == 0 || sar_height == 0) 
    {
        TS_LOG(1) << "Unspecified SAR not supported";
        return NML_ERROR_INVALID_STREAM;
    }

    return NML_OK;
}

int EsAvcParser::convertToRbsp(const uint8* buffer, int size, uint8* buffer_rbsp)
{
    int rbsp_size = 0;
    int zero_count = 0;
    for (int k = 0; k < size; k++) 
    {
        if (buffer[k] == 0x3 && zero_count >= 2) 
        {
            zero_count = 0;
            continue;
        }

        if (buffer[k] == 0)
            zero_count++;
        else
            zero_count = 0;
        buffer_rbsp[rbsp_size++] = buffer[k];
    }
    return rbsp_size;
}


} //namespace ts
} //namespace nml_demuxer


