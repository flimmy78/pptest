
//=======================================================================
// 
//  [12/27/2013 kwanghyun won]
//  abstract base class for callback
//  ref : http://tedfelix.com/software/c++-callbacks.html
//======================================================================= 

#ifndef _REGISTER_INTERFACE_H_
#define _REGISTER_INTERFACE_H_

class RegisterInterface
{
public:

    virtual void registerPmtCB(int program_number, int pmt_pid) = 0;
    virtual void registerPesCB(int pes_pid, int stream_type) = 0;

};


#endif //_REGISTER_INTERFACE_H_


