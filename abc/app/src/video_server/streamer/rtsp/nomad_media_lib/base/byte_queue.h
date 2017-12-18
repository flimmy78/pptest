
//=======================================================================
// 
//  [12/18/2013 kwanghyun won]
//                                                     
//======================================================================= 


#ifndef _BYTE_QUEUE_H_
#define _BYTE_QUEUE_H_

#include <memory>

#include "base/basic_types.h"
#include "base/nml_defines.h"

namespace base
{

class ByteQueue
{
public:
    static const int kDefaultQueueSize = TS_PACKET_SIZE * TS_PACKETS_PER_NETWORK_PACKET;
    
    ByteQueue();
    ~ByteQueue();

    void reset();
    void push(const uint8* data, int size);

    // Get a pointer to the front of the queue and the queue size.
    void peek(const uint8** data, int* size) const;

    // Remove |count| bytes from the front of the queue.
    void pop(int count);
	void erase(int count);
	
	size_t getBuffersize() const { return buffer_size_; }
    uint8* getBuffer() const { return buffer_.get(); }
	
private:
    // Returns a pointer to the front of the queue.
    uint8* front() const;
    std::unique_ptr<uint8[]> buffer_;
    //uint8* buffer_;

    size_t buffer_size_;
    size_t offset_;
    int used_;
	
	NML_DECLARE_NON_COPYABLE(ByteQueue);
};

} //namespace base
#endif //_BYTE_QUEUE_H_



