
#include "base/debug.h"
#include "base/nml_defines.h"

#include "demuxer/ts/es_adts_parser.h"

namespace nml_demuxer
{
namespace ts
{

// Adts header is at least 7 bytes (can be 9 bytes).
static const int kAdtsHeaderMinSize = 7;
// Number of samples per frame.
static const int kNumberSamplesPerAACFrame = 1024;

static const int adts_frequency_table[16] = {
    96000,
    88200,
    64000,
    48000,
    44100,
    32000,
    24000,
    22050,
    16000,
    12000,
    11025,
    8000,
    7350,
    0,
    0,
    0,
};
static const int kMaxSupportedFrequencyIndex = 12;

static int extractAdtsFrequencyIndex(const uint8* adts_header) 
{
    return ((adts_header[2] >> 2) & 0xf);
}

static int extractAdtsChannelConfig(const uint8* adts_header) 
{
    return (((adts_header[3] >> 6) & 0x3) |
        ((adts_header[2] & 0x1) << 2));
}

static int extractAdtsFrameSize(const uint8* adts_header) 
{
    return ((static_cast<int>(adts_header[5]) >> 5) |
        (static_cast<int>(adts_header[4]) << 3) |
        ((static_cast<int>(adts_header[3]) & 0x3) << 11));
}

static bool isAdtsSyncWord(const uint8* buf) 
{
    return (buf[0] == 0xff) && ((buf[1] & 0xf6) == 0xf0);
}

static bool syncWord(const uint8* raw_es, int raw_es_size,
    int pos,
    int* new_pos, int* frame_sz) 
{
    NM_ASSERT(pos > 0);
    NM_ASSERT(pos < raw_es_size);

    int max_offset = raw_es_size - kAdtsHeaderMinSize;
    if (pos >= max_offset) 
    {
        *new_pos = pos;
        return false;
    }

    for (int offset = pos; offset < max_offset; offset++) 
    {
        const uint8* current_buffer = &raw_es[offset];

        if (!isAdtsSyncWord(current_buffer))
            continue;

        int frame_size = extractAdtsFrameSize(current_buffer);
        if (frame_size < kAdtsHeaderMinSize) 
        {
            continue;
        }

        int remaining_size = raw_es_size - offset;
        if (remaining_size >= frame_size + 2 &&
            !isAdtsSyncWord(&current_buffer[frame_size])) 
        {
            continue;
        }

        *new_pos = offset;
        *frame_sz = frame_size;
        return true;
    }

    *new_pos = max_offset;
    return false;
}

EsAdtsParser::EsAdtsParser(EmitFrameInterface* cb) :
    emitFrameCB(cb)
{
}

EsAdtsParser::~EsAdtsParser()
{
}

Status EsAdtsParser::parse( const uint8* buffer, int size, 
                           base::DeltaTime pts,
                           base::DeltaTime dts )
{
    int raw_es_size;
    const uint8* raw_es;

    if (pts != base::DeltaTime::noTimeStamp())
    {
        esByteQueue.peek(&raw_es, &raw_es_size);
        ptsList.push_back(esPts(raw_es_size, pts));
    }

    esByteQueue.push(buffer, size);
    esByteQueue.peek(&raw_es, &raw_es_size);

    int es_position = 0;
    int frame_size;
    while (syncWord(raw_es, raw_es_size, es_position,
        &es_position, &frame_size)) 
    {
        TS_LOG(LOG_LEVEL_ES)
            << "ADTS syncword @ pos=" << es_position
            << " frame_size=" << frame_size;
 

        int remaining_size = raw_es_size - es_position;
        if (frame_size > remaining_size)
            break;

        if (!updateAudioConfiguration(&raw_es[es_position]))
            return NML_ERROR_INVALID_STREAM;


        // Get the PTS & the duration 
        while (!ptsList.empty() &&
            ptsList.front().first <= es_position) 
        {
            audio_timestamp_util->setBaseTimestamp(ptsList.front().second);
            ptsList.pop_front();
        }

        base::DeltaTime current_pts = audio_timestamp_util->getTimestamp();
        base::DeltaTime frame_duration =
            audio_timestamp_util->getFrameDuration(kNumberSamplesPerAACFrame);

        // Emit an audio frame.
        bool is_key_frame = true;

        StreamParserBuffer* stream_parser_buffer = 
            new StreamParserBuffer(&raw_es[es_position], frame_size, is_key_frame);
        stream_parser_buffer->setDecodeTimestamp(current_pts);
        stream_parser_buffer->setTimestamp(current_pts);
        stream_parser_buffer->setDuration(frame_duration);
        emitFrameCB->onEmitVideoFrame(stream_parser_buffer);

        // Update the PTS of next frame.
        audio_timestamp_util->addFrames(kNumberSamplesPerAACFrame);

        // Skip current frame.
        es_position += frame_size;
    }

    discardsEs(es_position);

    return NML_OK;
}

bool EsAdtsParser::updateAudioConfiguration(const uint8* adts_header) 
{
    int frequency_index = extractAdtsFrequencyIndex(adts_header);
    if (frequency_index > kMaxSupportedFrequencyIndex) 
    {
        return false;
    }

    int channel_configuration = extractAdtsChannelConfig(adts_header);
    if (channel_configuration == 0) 
    {
        return false;
    }

    int samples_per_second = adts_frequency_table[frequency_index];
    int adts_profile = (adts_header[2] >> 6) & 0x3;


    TS_LOG(LOG_LEVEL_ES) << "Sampling frequency: " << samples_per_second;
    //TS_LOG(1) << "Extended sampling frequency: " << extended_samples_per_second;
    TS_LOG(LOG_LEVEL_ES) << "Channel config: " << channel_configuration;
    TS_LOG(LOG_LEVEL_ES) << "Adts profile: " << adts_profile;

    // Reset the timestamp helper to use a new time scale.
    if (audio_timestamp_util) 
    {
        base::DeltaTime base_timestamp = audio_timestamp_util->getTimestamp();
        audio_timestamp_util = 
            new nml_demuxer::AudioTimestampUtil(samples_per_second);
        audio_timestamp_util->setBaseTimestamp(base_timestamp);
    } 
    else 
    {
        audio_timestamp_util = 
            new nml_demuxer::AudioTimestampUtil(samples_per_second);
    }

    return true;
}
void EsAdtsParser::flush()
{
    
}

void EsAdtsParser::reset()
{
    esByteQueue.reset();
    ptsList.clear();
}

void EsAdtsParser::discardsEs(int bytes)
{
    if (bytes <= 0)
        return;

    //각각의 pts 의 es position 조정
    for (auto it : ptsList)
    {
        it.first -= bytes;
    }

    esByteQueue.pop(bytes);
}

} //namespace ts
} //namespace nml_demuxer


