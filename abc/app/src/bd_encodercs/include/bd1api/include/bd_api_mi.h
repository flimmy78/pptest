
/// File: bd_api_mi.h
/// Date: 2017/07

#ifndef __BD_API_MI_H_
#define __BD_API_MI_H_

#include "bd_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

BD_VOID BD_MPI_MI_GetMIInfo(BD_U32* u32Count, BD_U32* u32Width, BD_U32* u32Height, BD_U32* u32Len);
BD_S32 BD_MPI_MI_GetFd(BD_U32 u32BufIndex);
BD_VOID BD_MPI_MI_Init(BD_VOID);
BD_S32 BD_MPI_MI_CreateChn(BD_U32 u32Width, BD_U32 u32Height, BD_U32 u32Payload, BD_U8 u8Depth);
BD_S32 BD_MPI_MI_DestroyChn(BD_VOID);
BD_VOID* BD_MPI_MI_GetBuffer(BD_U32 u32BufIndex);
BD_S32 BD_MPI_MI_SetBuffer(BD_U32 u32BufIndex);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
