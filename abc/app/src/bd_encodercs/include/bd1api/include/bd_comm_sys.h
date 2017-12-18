
/// File : bd_comm_sys.h
/// Date : 2015/04

#ifndef __BD_COMM_SYS_H__
#define __BD_COMM_SYS_H__

#include "bd_type.h"
#include "bd_errno.h"
#include "bd_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define BD_TRACE_SYS(level, fmt...) BD_TRACE(level, BD_ID_SYS,##fmt)
typedef struct bdMPP_SYS_CONF_S
{
    /* stride of picture buffer must be aligned with this value.
     * you can choose a value from 1 to 1024, and it must be multiple of 16.
     */
    BD_U32 u32AlignWidth;  

}MPP_SYS_CONF_S;


#define BD_ERR_SYS_NULL_PTR  BD_DEF_ERR(BD_ID_SYS, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
#define BD_ERR_SYS_NOTREADY  BD_DEF_ERR(BD_ID_SYS, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)
#define BD_ERR_SYS_NOT_PERM  BD_DEF_ERR(BD_ID_SYS, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)
#define BD_ERR_SYS_NOMEM     BD_DEF_ERR(BD_ID_SYS, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
#define BD_ERR_SYS_ILLEGAL_PARAM BD_DEF_ERR(BD_ID_SYS, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
#define BD_ERR_SYS_BUSY      BD_DEF_ERR(BD_ID_SYS, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /* __BD_COMM_SYS_H__ */

