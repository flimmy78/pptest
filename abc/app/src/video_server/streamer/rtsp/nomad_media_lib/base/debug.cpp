
#include "base/debug.h"

#include <stdio.h>

#ifdef OS_WIN
#include <windows.h>
#else 
#include <stdarg.h>
#include <string.h>
#endif // OS_WIN

namespace base
{


void nm_log(const char * format, ...)
{
    char buffer[kMaxLogLen];
#if defined(OS_WIN)

    va_list ap;
    va_start(ap, format);
    vsnprintf_s(buffer, kMaxLogLen, kMaxLogLen, format, ap);
    va_end(ap);

    WCHAR wszBuf[kMaxLogLen] = {0};
    MultiByteToWideChar(CP_UTF8, 0, buffer, -1, wszBuf, sizeof(wszBuf));
    OutputDebugStringW(wszBuf);
    OutputDebugStringA("\n");

    WideCharToMultiByte(CP_ACP, 0, wszBuf, sizeof(wszBuf), buffer, sizeof(buffer), NULL, FALSE);
    printf("%s\n", buffer);
#elif defined(OS_POSIX)

    va_list ap;
    va_start(ap, format);
    vsnprintf(buffer, kMaxLogLen, format, ap);
    va_end(ap);

    // strip any trailing newlines from log message
    size_t length = strlen(buffer);
    while ( length && buffer[length-1] == '\n')
    {
        buffer[length-1] = '\0';
        length--;
    }


#endif
}


void message_Box(const char * msg, const char * title)
{
#ifdef OS_WIN
    MessageBoxA(NULL, msg, title, MB_OK);
#else
    nm_log("%s: %s", title, msg);
#endif // OS_WIN

}

} //namespace base
