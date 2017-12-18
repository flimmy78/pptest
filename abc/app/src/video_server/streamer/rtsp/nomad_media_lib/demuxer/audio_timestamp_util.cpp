
#include "demuxer/audio_timestamp_util.h"

namespace nml_demuxer
{


AudioTimestampUtil::AudioTimestampUtil(int samples_per_second) :
    frameCount(0)
{
    baseTimestamp = base::DeltaTime::noTimeStamp();
    double fps = samples_per_second;
    microsecondsPerFrame = base::Time::kMicrosecondsPerSecond / fps;
}

void AudioTimestampUtil::setBaseTimestamp(base::DeltaTime base_timestamp)
{
    baseTimestamp = base_timestamp;
    frameCount = 0;
}

void AudioTimestampUtil::addFrames(int frame_count)
{
    frameCount += frame_count;
}

base::DeltaTime AudioTimestampUtil::getTimestamp() const
{
    return computeTimestamp(frameCount);
}

base::DeltaTime AudioTimestampUtil::getFrameDuration(int frame_count) const
{
    base::DeltaTime end_timestamp = computeTimestamp(frameCount + frame_count);
    return end_timestamp - getTimestamp();
}

base::DeltaTime AudioTimestampUtil::computeTimestamp(int64 frame_count) const 
{
    double frames_us = static_cast<double>(microsecondsPerFrame * frame_count);
    return baseTimestamp + base::DeltaTime::fromMicroseconds((int64)frames_us);
}

} //namespace nml_demuxer
