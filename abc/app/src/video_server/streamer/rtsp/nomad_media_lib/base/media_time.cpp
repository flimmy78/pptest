
#include "base/media_time.h"
namespace base
{


int64 DeltaTime::inMilliseconds() const 
{
    return delta / Time::kMicrosecondsPerMillisecond;
}

int64 DeltaTime::inMillisecondsRoundedUp() const 
{
    return (delta + Time::kMicrosecondsPerMillisecond - 1) /
        Time::kMicrosecondsPerMillisecond;
}

int64 DeltaTime::inMicroseconds() const 
{
    return delta;
}

} //namespace base
