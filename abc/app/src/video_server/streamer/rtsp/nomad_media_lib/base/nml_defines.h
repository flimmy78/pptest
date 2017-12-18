
#ifndef _NML_DEFINES_H_
#define _NML_DEFINES_H_

#define NML_ENABLE_FILE_READER 
#define NML_ENABLE_FILE_MAP_READER  

//build config

// Compiler detection.
#ifdef __GNUC__
#define COMPILER_GCC 1
#elif (_MSC_VER >= 1500 && _MSC_VER < 1700)
//visual studio 2008 이상
#define COMPILER_MSVC 1
#elif (_MSC_VER >= 1700 && _MSC_VER < 1800)
//visual studio 2012 이상
#define COMPILER_MSVC 2
#else
#error not exist cmmpiler
#endif

#ifndef COMPILER_MSVC
// stdint.h is part of C99 but MSVC doesn't have it.
#include <stdint.h>         // For intptr_t.
#else
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN //reduce build time http://sweeper.egloos.com/1940473
#endif

#if defined(COMPILER_MSVC)
#define OVERRIDE override
#elif defined(__clang__)
#define OVERRIDE override
#else
#define OVERRIDE
#endif


// A set of macros to use for platform detection.
#if defined(__APPLE__)
#define OS_MACOSX 1
#elif defined(__linux__)
#define OS_LINUX 1
#elif defined(_WIN32)
#define OS_WIN 1
#else
#error Please add support for your platform 
#endif

// For access to standard POSIX features, use OS_POSIX instead of a more
// specific macro.
#if defined(OS_MACOSX) || defined(OS_LINUX)
#define OS_POSIX 1
#endif

#ifdef _DEBUG
#define NOMAD_DEBUG 
#endif // _DEBUG

#ifdef COMPILER_MSVC
#define NM_LONGLONG(x) x##I64
#define NM_ULONGLONG(x) x##UI64
#else
#define NM_LONGLONG(x) x##LL
#define NM_ULONGLONG(x) x##ULL
#endif

//////////////////////////////////////////////////////////////////////////
// constants
#define MAX_FILE_PATH 1024
#define TS_PACKET_SIZE 188
#define TS_PACKETS_PER_NETWORK_PACKET 7


//////////////////////////////////////////////////////////////////////////
// disable copy constructor and assign
// effective c++ item 5 , item 6
// http://goo.gl/oxm7ux, http://goo.gl/8AC4up
#define NML_DECLARE_NON_COPYABLE(className) \
    className (const className&);\
    className& operator= (const className&)

//////////////////////////////////////////////////////////////////////////
// getter, setter
#define NM_PROPERTY(var_type, var_name, func_name)\
protected: var_type var_name;\
public: virtual var_type get##func_name(void);\
public: virtual void set##func_name(var_type var);

#define NM_PROPERTY_PASS_BY_REF(var_type, var_name, func_name)\
protected: var_type var_name;\
public: virtual const var_type& get##func_name(void);\
public: virtual void set##func_name(const var_type& var);

#define NM_PROPERTY_READONLY(var_type, var_name, func_name)\
protected: var_type var_name;\
public: virtual var_type get##func_name(void);

#define NM_PROPERTY_READONLY_PASS_BY_REF(var_type, var_name, func_name)\
protected: var_type var_name;\
public: virtual const var_type& get##func_name(void);

#define NM_SYNTHESIZE(var_type, var_name, func_name)\
protected: var_type var_name;\
public: virtual var_type get##func_name(void) const { return var_name; }\
public: virtual void set##func_name(var_type var){ var_name = var; }

#define NM_SYNTHESIZE_READONLY(var_type, var_name, func_name)\
protected: var_type var_name;\
public: virtual var_type get##func_name(void) const { return var_name; }

#define NM_SYNTHESIZE_READ(var_type, var_name, func_name)\
protected: var_type var_name;\
public: virtual Status set##func_name(var_type var); \
public: virtual var_type get##func_name(void) const { return var_name; }

#define NM_SYNTHESIZE_PASS_BY_REF(var_type, var_name, func_name)\
protected: var_type var_name;\
public: virtual const var_type& get##func_name(void) const { return var_name; }\
public: virtual void set##func_name(const var_type& var){ var_name = var; }

#define NM_SYNTHESIZE_READONLY_PASS_BY_REF(var_type, var_name, func_name)\
protected: var_type var_name;\
public: virtual const var_type& get##func_name(void) const { return var_name; }


#define MAX_LEN 256
#define NML_COUNT_OF( obj )  (sizeof(obj)/sizeof(obj[0]))
//#define NML_MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )
//#define NML_MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
#define NML_ABS( a ) ( ((a) < 0) ? (-(a)) : (a) )
//////////////////////////////////////////////////////////////////////////
// exception check
#define NM_BREAK_IF(cond)            if(cond) break
#define TS_CHECK_NULL(var_name)      if(!var_name) return NML_ERROR_NULL_PTR;

#define NM_SAFE_DELETE(p)            do { if(p) { delete (p); (p) = nullptr; } } while(0)
#define NM_SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = nullptr; } } while(0)

// new callbacks based on C++11
#include <functional>
#define CALLBACK_0(__selector__,__target__, ...) std::bind(&__selector__,__target__, ##__VA_ARGS__)
#define CALLBACK_1(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, ##__VA_ARGS__)
#define CALLBACK_2(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, ##__VA_ARGS__)
#define CALLBACK_3(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ##__VA_ARGS__)

//benchmark require #include <chrono>
#define BANCHMARK_START()     auto start = std::chrono::high_resolution_clock::now();
#define BANCHMARK_END()\
	auto end = std::chrono::high_resolution_clock::now();\
    auto sec = std::chrono::duration_cast< std::chrono::duration< double > >(end - start);\
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
#define BANCHMARK_RESULT()\
    std::cout << green << "Time: " << sec.count() << reset << " sec "  \
    		  << green << ns.count() << reset << " nsec" << std::endl;
#endif //_NML_DEFINES_H_



