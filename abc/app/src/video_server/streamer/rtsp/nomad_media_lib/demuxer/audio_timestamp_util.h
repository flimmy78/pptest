
//=======================================================================
// 
//  [1/6/2014 kwanghyun won]
//                                                     
//======================================================================= 

#ifndef _AUDIO_TIMESTAMP_UTIL_H_
#define _AUDIO_TIMESTAMP_UTIL_H_

#include "base/media_time.h"

namespace nml_demuxer
{


class AudioTimestampUtil
{
public:
    AudioTimestampUtil(int samples_per_second);


    void setBaseTimestamp(base::DeltaTime base_timestamp);

    base::DeltaTime getBaseTimestamp() const { return baseTimestamp; }
    base::DeltaTime base_timestamp() const;
    void addFrames(int frame_count);

    base::DeltaTime getTimestamp() const;

    base::DeltaTime getFrameDuration(int frame_count) const;


private:
    base::DeltaTime computeTimestamp(int64 frame_count) const;

    double microsecondsPerFrame;
    base::DeltaTime baseTimestamp;

    int64 frameCount;
    NML_DECLARE_NON_COPYABLE(AudioTimestampUtil);
};


} //namespace nml_demuxer
#endif //_AUDIO_TIMESTAMP_UTIL_H_


