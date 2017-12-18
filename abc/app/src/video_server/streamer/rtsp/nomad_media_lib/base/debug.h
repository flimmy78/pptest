
//=======================================================================
// 
//  [12/16/2013 kwanghyun won]
//                         
//  - [1/3/2014] add cpp iostream version
//======================================================================= 

#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "base/nml_defines.h"
#include "base/console_color.h"

#include <assert.h>
#include <string>
#include <sstream>
#if defined(OS_WIN)
#include <io.h> /* for write */
#else
#include <unistd.h>  /* for write */
#endif




// Windows warns on using write().  It prefers _write().
#if defined(OS_WIN)
#define write(fd, buf, count) _write(fd, buf, static_cast<unsigned int>(count))
// Windows doesn't define STDERR_FILENO.  Define it here.
#define STDERR_FILENO 2
#endif

namespace base
{

/// The max length of NMLog message.
static const int kMaxLogLen = 16*1024;

// Output Debug message.
void nm_log(const char * format, ...); 

// Pop out a message box
void message_Box(const char * msg, const char * title);


class LogMessage 
{
public:
    LogMessage(const char* file, int line) :
        flushed(false) 
    {
        stream() << file << ":" << line << ": ";
    }
    void flush() 
    {
        stream() << "\n";
        std::string s = message.str();
        int n = (int)s.size(); //msvc
        if(write(STDERR_FILENO, s.data(), n) < 0) {}  //gcc
        flushed = true;
    }
    ~LogMessage() 
    {
        if (!flushed) 
        {
            flush();
        }
    }
    std::ostream& stream() { return message; }

private:
    bool flushed;
    std::ostringstream message;
    NML_DECLARE_NON_COPYABLE(LogMessage);
};

class LogMessageFatal : public LogMessage 
{
public:
    LogMessageFatal(const char* file, int line)
        : LogMessage(file, line) { }
    ~LogMessageFatal() {
        flush();
        abort();
    }
private:
    NML_DECLARE_NON_COPYABLE(LogMessageFatal);
};

#if 0
//////////////////////////////////////////////////////////////////////////
// macro

#if defined(OS_WIN)
#define __NMLOG_WITH_FUNCTION(s, ...) \
    nm_log("%s : %s",__FUNCTION__ , __VA_ARGS__)
#elif defined(OS_POSIX)
#define __NMLOGWITHFUNCTION(s, ...) \
    nm_log("%s : %s", __PRETTY_FUNCTION__, __VA_ARGS__)
#endif


#if !defined(NOMAD_DEBUG) 
#define NMLOG(...)       do {} while (0)
#define NMLOGINFO(...)   do {} while (0)
#define NMLOGERROR(...)  do {} while (0)
#define NMLOGWARN(...)   do {} while (0)
#else
#define NMLOG(format, ...)      base::nm_log(format, ##__VA_ARGS__)
#define NMLOG_ERROR(format,...)  base::nm_log(format, ##__VA_ARGS__)
#define NMLOG_INFO(format,...)   base::nm_log(format, ##__VA_ARGS__)
#define NMLOG_WARN(...) __NMLOG_WITH_FUNCTION(__VA_ARGS__)
#endif // NOMAD_DEBUG
#endif
//////////////////////////////////////////////////////////////////////////
// iostream macro 
#define LOG_INFO base::LogMessage(__FILE__, __LINE__)
#define LOG_ERROR LOG_INFO
#define LOG_WARNING LOG_INFO
#define LOG_FATAL base::LogMessageFatal(__FILE__, __LINE__)
#define LOG_QFATAL LOG_FATAL
#define VLOG(x) if((x)>0){}else LOG_INFO.stream()
#define TS_LOG(verboseLevel) VLOG(verboseLevel)

#define CHECK(x)	if(x){}else base::LogMessageFatal(__FILE__, __LINE__).stream() << "Check failed: " #x
#define DCHECK(condition) assert(condition)

#ifdef NOMAD_DEBUG
#define DEBUG_MODE 0
#define LOG_DFATAL LOG_ERROR
#else
#define DEBUG_MODE 1
#define LOG_DFATAL LOG_FATAL
#endif

#define LOG_LEVEL_INFO 1
#define LOG_LEVEL_WARNING = -2
#define LOG_LEVEL_ERROR -3 
#define LOG_LEVEL_FATAL -4

#define LOG_LEVEL_TS  5
#define LOG_LEVEL_PES 6
#define LOG_LEVEL_ES  7
#define LOG_LEVEL_TS_TIME  7
#define LOG_LEVEL_TS_INDEX  -8

#define TS_CHECK(x) \
do { \
    if (!(x)) { \
        return NML_ERROR_INVALID_STREAM; \
    } \
} while (0)

//#define DUMP_TS 1 

//////////////////////////////////////////////////////////////////////////

//assert
#if !defined(NOMAD_DEBUG)
#define NM_ASSERT(condition) 
#else
#define NM_ASSERT(condition) assert(condition)
#endif
} //namespace base
#endif //_DEBUG_H_


