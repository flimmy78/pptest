
//=======================================================================
// 
//  [1/3/2014 kwanghyun won]
//                                                     
//======================================================================= 

#ifndef _TIME_H_
#define _TIME_H_

#include "base/basic_types.h"
namespace base
{

class Time 
{
public:
    static const int64 kMillisecondsPerSecond = 1000;
    static const int64 kMicrosecondsPerMillisecond = 1000;
    static const int64 kMicrosecondsPerSecond = kMicrosecondsPerMillisecond *
                                                kMillisecondsPerSecond;
    static const int64 kMicrosecondsPerMinute = kMicrosecondsPerSecond * 60;
    static const int64 kMicrosecondsPerHour = kMicrosecondsPerMinute * 60;
};

class DeltaTime
{
public:
    DeltaTime() : delta(0) {}

    static DeltaTime fromSeconds(int64 secs);
    static DeltaTime fromMilliseconds(int64 ms);
    static DeltaTime fromMicroseconds(int64 us);
    static DeltaTime noTimeStamp();

    int64 inMilliseconds() const;
    int64 inMillisecondsRoundedUp() const;
    int64 inMicroseconds() const;

    //////////////////////////////////////////////////////////////////////////
    // operator overloading
    DeltaTime operator+(DeltaTime other) const 
    {
        return DeltaTime(delta + other.delta);
    }
    DeltaTime operator-(DeltaTime other) const 
    {
        return DeltaTime(delta - other.delta);
    }

    bool operator==(DeltaTime other) const
    {
        return this->delta == other.delta;
    }
    bool operator!=(DeltaTime other) const 
    {
        return delta != other.delta;
    }

private:
    explicit DeltaTime(int64 delta_us) : delta(delta_us) { }

    // Delta in microseconds.
    int64 delta;

};

inline DeltaTime DeltaTime::noTimeStamp() 
{
    return DeltaTime::fromMicroseconds(kNoTimestamp);
}

inline DeltaTime DeltaTime::fromSeconds(int64 secs) 
{
    return DeltaTime(secs * Time::kMicrosecondsPerSecond);
}

inline DeltaTime DeltaTime::fromMilliseconds(int64 ms) 
{
    return DeltaTime(ms * Time::kMicrosecondsPerMillisecond);
}

inline DeltaTime DeltaTime::fromMicroseconds(int64 us) 
{
    return DeltaTime(us);
}


} //namespace base
#endif //_TIME_H_


