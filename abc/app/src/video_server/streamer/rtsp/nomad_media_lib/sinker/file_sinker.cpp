
//=======================================================================
// 
//  [12/17/2013 kwanghyun won]
//                                                     
//======================================================================= 

#include "sinker/file_sinker.h"

namespace nml_sinker
{

FileSinker::FileSinker() :
    file_(nullptr),
    file_size_(0) { }

FileSinker::~FileSinker()
{
    close();
}


Status FileSinker::init(BaseSinkerParams* params)
{
    TS_CHECK_NULL( params );
    params_ = *params;

    close();

    if ( params_.is_append_ )
        file_ = fopen( params_.file_name_.c_str(), "ab");
    else
        file_ = fopen( params_.file_name_.c_str(), "wb");

    TS_CHECK_NULL( file_ );

    if ( !params_.buffer_size_ )
        params_.buffer_size_ = 0xFFFF;

    if ( setvbuf( file_, nullptr, _IOFBF, params_.buffer_size_) )
        return NML_ERROR_FAILED;

    return NML_OK;
}

Status FileSinker::close()
{
    if ( file_ )
    {
        fclose(file_);
        file_ = nullptr;
    }

    file_size_ = 0;

    return NML_OK;
}

Status FileSinker::reset()
{
    if (!file_)
        return NML_ERROR_NOT_INITIALIZED;

    if ( fseek( file_, 0, SEEK_SET) != 0)
        return NML_ERROR_FAILED;

    return NML_OK;
}

Status FileSinker::putData(const void *data, size_t &size)
{
    TS_CHECK_NULL( data );
    TS_CHECK_NULL( file_ );

    if (!size)
        return NML_OK;


    size_t write_size = fwrite(data, 1, size, file_);
    uint64 position = getPosition();

    if ( position > file_size_ )
        position = file_size_;

    if ( write_size < size )
    {
        size = write_size;
        return NML_ERROR_FAILED;
    }

    size = write_size;
    return NML_OK;
}

Status FileSinker::movePosition(int64 offset)
{
    TS_CHECK_NULL(file_);

    if ( fseek( file_, offset, SEEK_CUR) )
        return NML_ERROR_FAILED;

    return NML_OK;
}

uint64 FileSinker::getPosition()
{
    uint64 position = static_cast<uint64>( ftell( file_ ));
    return position;
}

uint64 FileSinker::getFileSize()
{
    return file_size_;
}

} //namespace nml_sinker

