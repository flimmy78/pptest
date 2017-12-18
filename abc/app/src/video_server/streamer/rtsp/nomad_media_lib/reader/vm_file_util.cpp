

#include "reader/vm_file_util.h"
#include  <stdlib.h>

namespace nml_reader
{

#if defined(OS_WIN)
VM_FILE_ID fileOpen(const char* file_name)
{
    _set_fmode(_O_BINARY);
    VM_FILE_ID file_handle = _open( file_name, _O_RDONLY );
    return file_handle;
}

size_t fileRead(VM_FILE_ID file_id, void* buffer, size_t size)
{
    int read_byte = _read(file_id, buffer, size);
    return static_cast<size_t>( read_byte );
}
#else

VM_FILE_ID fileOpen(const char* file_name)
{
    VM_FILE_ID file_handle = nullptr;
    file_handle = fopen( file_name, "rb" );
	return file_handle;
}

size_t fileRead(VM_FILE_ID file_id, void* buffer, size_t size)
{
    size_t read_byte = fread(buffer, 1, size, file_id);
    return read_byte;
}

#endif

} //namespace nml_reader
