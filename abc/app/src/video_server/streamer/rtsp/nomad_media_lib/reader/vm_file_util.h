
//=======================================================================
// 
//  [1/14/2014 kwanghyun won]
// 
// for platform dependant
// 
//======================================================================= 

#ifndef _NML_VM_FILE_UTIL_H_
#define _NML_VM_FILE_UTIL_H_

#include "base/nml_defines.h"

#if defined(OS_WIN)
#include <io.h>
#include <fcntl.h>
#endif
#include <sys/stat.h>
#include <stdio.h>

namespace nml_reader
{

#if defined(OS_WIN)

#define VM_FILE_ID   int

#define fileSeek   _lseek
#define fileSeek64 _lseeki64
#define fileTell   _telli
#define fileTell64 _telli64
#define fileWrite  _write
#define fileClose  _close
#define fileEnd    _eof

#define stat       _stat
#define stat64     _stat64

VM_FILE_ID fileOpen(const char* file_name);
size_t fileRead(VM_FILE_ID file_id, void* buffer, size_t size);


#else

#define VM_FILE_ID   FILE*

#define fileRead   fread
#define fileSeek64 fseeko64
#define fileTell64 ftello64
#define fileWrite  fwrite
#define fileClose  fclose
#define fileEnd    feof
VM_FILE_ID fileOpen(const char* file_name);
size_t fileRead(VM_FILE_ID file_id, void* buffer, size_t size);

#endif

} //namespace nml_reader

#endif //_NML_VM_FILE_UTIL_H_


