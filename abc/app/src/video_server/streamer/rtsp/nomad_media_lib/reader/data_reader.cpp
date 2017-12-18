//////////////////////////////////////////////////////////////////////////
//
// [11/13/2013 by kwanghyun won]
// 

#include "data_reader.h"

namespace nml_reader
{


Status DataReader::getData(void *data, uint32 *size)
{
	//size_t data_size = (size_t)(*size);

	Status result = NML_OK;
	do 
	{
		*size = getData(data, size);
	}
	while (result == NML_ERROR_NOT_ENOUGH_DATA && stop_ == false);

	return result;
}



size_t DataReader::showData(uint8 **data)
{
    *data = data_pointer_;
    return (size_t)(eod_pointer_ - data_pointer_);
}


} //namespace nml_reader



