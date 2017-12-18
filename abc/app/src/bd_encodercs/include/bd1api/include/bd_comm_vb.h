#ifndef __BD_COMM_VB_H__
#define __BD_COMM_VB_H__

#include <bd_type.h>
#include <bd_errno.h>
#include <bd_debug.h>
#include <bd_defines.h>

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

 
#define VB_MAX_POOLS 		256
#define VB_MAX_COMM_POOLS 	16
#define VB_MAX_BLOCKS		16


/* user ID for VB */
#define VB_MAX_USER   19

#define VB_UID_VIU     0 
#define VB_UID_VOU     1
#define VB_UID_DSU     2
#define VB_UID_VENC    3
#define VB_UID_VDEC    4
#define VB_UID_VDA      5
#define VB_UID_H264E   6
#define VB_UID_JPEGE   7
#define VB_UID_MPEG4E  8 
#define VB_UID_H264D   9 
#define VB_UID_JPEGD   10   
#define VB_UID_MPEG4D  11 
#define VB_UID_VPSS    12 
#define VB_UID_GRP     13
#define VB_UID_MPI     14 
#define VB_UID_PCIV    15
#define VB_UID_AI      16
#define VB_UID_AENC    17 
#define VB_UID_RC      18

#define VB_INVALID_POOLID (-1UL)
#define VB_INVALID_HANDLE (-1UL)

typedef BD_U32 VB_POOL;
typedef BD_U32 VB_BLK;

typedef struct bdVB_CONF_S
{
    BD_U32 u32MaxPoolCnt;     /* max count of pools, (0,VB_MAX_POOLS]  */    
    struct bdVB_CPOOL_S
    {
        BD_U32 u32BlkSize;
        BD_U32 u32BlkCnt;
        BD_CHAR acMmzName[MAX_MMZ_NAME_LEN];
    } astCommPool[VB_MAX_COMM_POOLS];
} VB_CONF_S;


#define BD_ERR_VB_NULL_PTR  BD_DEF_ERR(BD_ID_VB, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
#define BD_ERR_VB_NOMEM     BD_DEF_ERR(BD_ID_VB, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
#define BD_ERR_VB_NOBUF     BD_DEF_ERR(BD_ID_VB, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF)
#define BD_ERR_VB_UNEXIST   BD_DEF_ERR(BD_ID_VB, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST)
#define BD_ERR_VB_ILLEGAL_PARAM BD_DEF_ERR(BD_ID_VB, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
#define BD_ERR_VB_NOTREADY  BD_DEF_ERR(BD_ID_VB, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)
#define BD_ERR_VB_BUSY      BD_DEF_ERR(BD_ID_VB, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)
#define BD_ERR_VB_NOT_PERM  BD_DEF_ERR(BD_ID_VB, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)

#define BD_ERR_VB_2MPOOLS BD_DEF_ERR(BD_ID_VB, EN_ERR_LEVEL_ERROR, EN_ERR_BUTT + 1)

#define BD_TRACE_VB(level,fmt...) BD_TRACE(level, BD_ID_VB,##fmt)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /* __BD_COMM_VB_H_ */

