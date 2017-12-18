//=======================================================================
// 
//  [11/15/2013 kwanghyun won]
//                                                     
//======================================================================= 

#ifndef _FILE_READER_H_
#define _FILE_READER_H_

#include <stdio.h>
#include <string>

#include "base/nml_defines.h"
#include "base/basic_types.h"
#include "base/nml_structure.h"

#include "reader/data_reader.h"
#include "reader/vm_file_util.h"

#if defined (NML_ENABLE_FILE_READER)

namespace nml_reader
{

class FileReader : public DataReader
{
public:
	FileReader();
	virtual ~FileReader();

	virtual Status init(DataReaderParams *params);
    virtual Status close();
    virtual Status reset();

    virtual Status getData(void *data, size_t &size) OVERRIDE;
    virtual Status movePosition(int64 pos) OVERRIDE;
    virtual Status cacheData(void *data, size_t &size, int64 offset) OVERRIDE;
    virtual Status setPosition(float pos);
    virtual int64 getPosition();
    virtual int64 getSize();

    inline Status checkFileInit(Status result);
	
	std::string getFileName() { return fileName; }
protected:
	std::string fileName;
	VM_FILE_ID  fileId;
	int64 fileSize;
	int64 position;
};

inline Status FileReader::checkFileInit(Status result)
{
    if (NML_OK == result && !fileId)
        return NML_ERROR_NOT_INITIALIZED;

    return NML_OK;
}

}

#endif //NML_ENABLE_FILE_READER

#endif //_FILE_READER_H_


