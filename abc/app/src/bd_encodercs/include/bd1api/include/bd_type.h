/// File : BD_type.h
/// Date : 2015/03

#ifndef __BD_TYPE_H__
#define __BD_TYPE_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef unsigned char           BD_U8;
typedef unsigned short          BD_U16;
typedef unsigned int            BD_U32;

typedef signed char             BD_S8;
typedef short                   BD_S16;
typedef int                     BD_S32;

typedef unsigned long long		BD_U64;
typedef long long				BD_S64;

typedef char                    BD_CHAR;
#define BD_VOID                 void

/*----------------------------------------------*
 * const defination                             *
 *----------------------------------------------*/
typedef enum {
    BD_FALSE = 0,
    BD_TRUE  = 1,
} BD_BOOL;

#ifndef NULL
    #define NULL    0L
#endif

#define BD_NULL     0L
#define BD_SUCCESS  0
#define BD_FAILURE  (-1)

#define BD_INVALID	(-1)
#define BD_WAIT_TIMEOUT	1

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __BD_TYPE_H__ */

