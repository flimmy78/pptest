//=======================================================================
//
//  [11/15/2013 kwanghyun won]
//
//  [1/14/2014 kwanghyun won] fix for large file
//=======================================================================

#include "reader/file_reader.h"
#include "base/nml_defines.h"
#include "base/debug.h"

#if defined (NML_ENABLE_FILE_READER)

namespace nml_reader
{


FileReader::FileReader():
	fileId(nullptr),
	fileSize(0),
	position(0)
{}

FileReader::~FileReader()
{
	close();
}

Status FileReader::init(DataReaderParams *params)
{
	Status result = NML_OK;

    TS_CHECK_NULL( params );

	fileName = params->file_name_;

	fileId = fileOpen(params->file_name_.c_str() );
    checkFileInit(result);

	//if ( fileSeek64(fileId, 0, SEEK_END) )
	//	result = NML_ERROR_FAILED;

	if (NML_OK == result)
	{
#if 0
		fileSize = fileTell64(fileId);
        if ( -1 == static_cast<int64>(fileSize) )
            return NML_ERROR_FAILED;
#else
        struct stat st;
        if (stat(params->file_name_.c_str(), &st) == 0)
        {
            fileSize = st.st_size;
        }
        else
        {
            return NML_ERROR_NOT_INITIALIZED;
        }
#endif
	}

	if (NML_OK == result)
	{
		if ( fileSeek64(fileId, 0, SEEK_SET) )
		{
			result = NML_ERROR_FAILED;
		}
	}
	return result;
}

Status FileReader::close()
{
	if (fileId != nullptr)
		fileClose(fileId);

	fileSize = 0;
	position = 0;

	return NML_OK;
}

Status FileReader::reset()
{
	Status result = NML_OK;

    checkFileInit(result);

	if (NML_OK == result)
	{
		if (0 != fileSeek64(fileId, 0, SEEK_SET))
		{
			result = NML_ERROR_FAILED;
		}
		else
		{
			position = 0;
		}
	}
	return result;
}

Status FileReader::getData(void *data, size_t &size)
{
	Status  result = NML_OK;

    checkFileInit(result);
    TS_CHECK_NULL( data );


    if( size > (size_t)(fileSize - position) )
    {
        size  = static_cast<size_t>(fileSize - position);
        result = NML_ERROR_END_OF_STREAM;
    }

    if(!size)
        return NML_ERROR_END_OF_STREAM;

    size_t read_size = fread(data, 1, size, fileId);
    position += read_size;

    if( read_size < size )
    {
        size = read_size;
        if( !fileEnd(fileId))
            return NML_ERROR_FAILED;
        else
            result = NML_ERROR_END_OF_STREAM;
    }

	if(size == 0) result = NML_ERROR_END_OF_STREAM;
	else result = NML_OK;

	return result;
}

off_t getFileSize(const char *file_name)
{

    return -1;
}
Status FileReader::cacheData(void *data, size_t &size, int64 offset )
{
	Status result = NML_OK;

    checkFileInit(result);
    TS_CHECK_NULL(data);

    int64 initial_position = position + offset;
    if( initial_position < 0 )
    {
        initial_position = 0;
    }
    else if( initial_position > fileSize )
    {
        initial_position = fileSize;
    }

    if( size > (size_t)(fileSize - initial_position) )
        size = (size_t)(fileSize - initial_position);

    if( !size )
        return NML_OK;

    if(fileSeek64(fileId, initial_position, SEEK_SET))
        return NML_ERROR_FAILED;

    //size = fileRead(fileId, data, size);
	size = fread(data, 1, size, fileId);

    if(fileSeek64(fileId, initial_position, SEEK_SET))
        return NML_ERROR_FAILED;

	return NML_OK;
}

Status FileReader::movePosition(int64 pos)
{
	Status  result = NML_OK;

    //checkFileInit(result);

	//if (NML_OK == result && 0 != position)
	//{
		if (0 != fileSeek64(fileId, pos, SEEK_SET))
		{
			result = NML_ERROR_FAILED;
		}

		if (NML_OK == result)
		{
			position = fileTell64(fileId);
		}
	//}

	TS_LOG(1) << "movePosition() position = " << position << " result = " << result;

	return result;
}

int64 FileReader::getPosition()
{
	return position;
}

Status FileReader::setPosition(float pos)
{
	Status result = NML_OK;

    checkFileInit(result);

	if (NML_OK == result)
	{
		int64 offset = static_cast<int64>(fileSize * pos);
		if ((offset % 188) != 0 )
		{
			offset = 188 * (offset / 188);
		}
		if (0 != fileSeek64(fileId, offset, SEEK_SET))
		{
			result = NML_ERROR_FAILED;
		}
	}

	if (NML_OK == result)
	{
		position = fileTell64(fileId);
	}

	TS_LOG(-1) << "setPosition() position = " << position << " result = " << result;
	return result;
}

int64 FileReader::getSize()
{
	return fileSize;
}

} //namespace nml_reader
#endif // NML_ENABLE_FILE_READER

