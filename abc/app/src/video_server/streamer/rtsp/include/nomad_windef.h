#ifndef __WINDEF_H__
#define __WINDEF_H__

#if !defined(WIN32)

#define MAX_PATH		260

typedef unsigned short      USHORT;
typedef int                HRESULT;
typedef unsigned char       BYTE;
typedef unsigned char       UCHAR;
typedef unsigned int        UINT;
typedef unsigned int 		UINT_PTR;
typedef unsigned long       DWORD;
typedef unsigned long       ULONG;
#ifndef OSX
typedef int                 BOOL;
#endif
typedef char                TCHAR;
typedef const char*			LPCSTR;
typedef const TCHAR*        LPCTSTR;
typedef long long           LONGLONG;
typedef LONGLONG            REFERENCE_TIME;
typedef unsigned long long  ULONGLONG;
typedef int					SOCKET;
typedef long				HANDLE;		// pthread_t
typedef unsigned long		ULONG_PTR;
typedef void*				PVOID;
typedef void*				LPVOID;
typedef unsigned short 		WORD;

#if !defined(INVALID_SOCKET)
#define INVALID_SOCKET  (SOCKET)(~0)
#endif

#if !defined(TRUE)
#define TRUE 1
#endif

#if !defined(FALSE)
#define FALSE 0
#endif

#if !defined(INFINITE)
#define INFINITE 0xFFFFFFFF
#endif

#endif

#endif