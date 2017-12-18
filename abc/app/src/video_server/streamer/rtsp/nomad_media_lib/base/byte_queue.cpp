
//=======================================================================
// 
//  [12/18/2013 kwanghyun won]
//                                                     
//======================================================================= 

#include "base/byte_queue.h"

#include <cstring>
#include "base/debug.h"

namespace base
{


ByteQueue::ByteQueue() :
    buffer_size_(kDefaultQueueSize),
    offset_(0),
    used_(0) 
{
    //buffer_ = new uint8[kDefaultQueueSize];
    buffer_= std::unique_ptr<uint8[]>(new uint8[kDefaultQueueSize]);
}

ByteQueue::~ByteQueue()
{
    //NM_SAFE_DELETE_ARRAY( buffer_);
}


void ByteQueue::reset()
{
    offset_ = 0;
    used_ = 0;
}

void ByteQueue::push(const uint8* data, int size)
{
    NM_ASSERT( data );
    //NM_ASSERT( size > 0);

    int need_size = used_ + size;
	buffer_size_ = need_size;
#if 0	
    if ( need_size > size )
    {
        int new_size = 2 * size;
        while ( need_size > new_size && new_size > size )
        {
            new_size *= 2;
        }
       
        //uint8* new_buffer =  new uint8[new_size];
        std::unique_ptr<uint8[]> new_buffer( new uint8[new_size] );
        if ( used_ > 0 )
        {
            std::memcpy(new_buffer.get(), front(), used_);
        }

        buffer_ = std::move(new_buffer) ;
        buffer_size_ = new_size;
        offset_ = 0;
    }
    else if ( (int)(offset_ + used_ + size) > size)
    {
        std::memmove(buffer_.get() ,front(), used_);
        offset_ = 0;
    }
#endif
    std::memcpy( front() + used_, data, size);
    used_ += size;
}

void ByteQueue::peek(const uint8** data, int* size) const
{
    NM_ASSERT( data );
    NM_ASSERT( size > 0);

    *data = front();
    *size = used_;
}

void ByteQueue::pop(int count)
{
    offset_ += count;
    used_ -= count;

    //buffer 끝에 다다르면 offset을 0으로
    if ( offset_ == buffer_size_ )
    {
        offset_ = 0;
    }
}

void ByteQueue::erase(int count)
{
    used_ -= count;
    std::memmove(front() ,front() + count, used_ );
    buffer_size_ -= count;

    //buffer 끝에 다다르면 offset을 0으로
    if ( offset_ == buffer_size_ )
    {
        offset_ = 0;
    }
}

uint8* ByteQueue::front() const
{
    return buffer_.get() + offset_;
}

} //namespace base
