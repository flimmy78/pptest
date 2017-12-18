//////////////////////////////////////////////////////////////////////////
//
// [11/13/2013 by kwanghyun won]
// 

#ifndef _DATA_READER_H_
#define _DATA_READER_H_

#include "base/basic_types.h"
#include "base/nml_structure.h"

#include <string>

namespace nml_reader
{

class DataReaderParams
{
public:
    DataReaderParams()
    {
        file_name_   = "";
        buffer_size_ = 0;
    }

    std::string file_name_;
    size_t      buffer_size_;
};

class DataReader
{

public:
	DataReader()
	{ 
		data_pointer_ = 0; 
		eod_pointer_ = 0;
		stop_ = 0; 
	}

	virtual ~DataReader(void){};

	/// Initialization abstract source data
	//virtual Status init(DataReaderParams *init_params) = 0;

	/// Close source data
	virtual Status close(void) = 0;

	/// Reset all internal parameters to start reading from begin
	virtual Status reset(void) = 0;

	/// Get data
	Status getData(void *data, uint32 *size);


	/**
    Read nsize bytes and copy to data (return number bytes which was copy).
    Cache data in case of small nsize
    */
    virtual Status getData(void *data, size_t &size) = 0;
    virtual Status movePosition(int64 position) = 0;

    //Status check_Data(void *data, uint32 *size, int32 how_far);
    size_t showData(uint8 **data);
    virtual Status cacheData(void *data, size_t &size, int64 offset = 0) = 0;

    virtual int64 getPosition(void) = 0;
	virtual int64 getSize(void) = 0;

    /// Set new position
    virtual Status setPosition (uint64 position)
    {
		return movePosition(position - getPosition());
    }


//////////////////////////////////////////////////////////////////////////
// member
public:
	uint8 *data_pointer_; // current data pointer
	uint8 *eod_pointer_;  // end of data pointer
private:
	bool stop_;
};

} //namespace nml_reader


#endif //_DATA_READER_H_
