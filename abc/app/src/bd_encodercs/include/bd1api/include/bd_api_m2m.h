
/// File: bd_api_m2m.h
/// Date: 2015/03

#ifndef __BD_API_M2M_H_
#define __BD_API_M2M_H_	1

#include "bd_common.h"
#include "bd_comm_m2m.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/* AHB M2M API */
BD_S32 BD_MPI_M2M_Init(BD_VOID);
BD_S32 BD_MPI_M2M_Exit(BD_VOID);
BD_S32 BD_MPI_M2M_SetCopy(BD_U32 u32Chn, BD_U32 u32SrcAddr, BD_U32 u32DstAddr, BD_U32 u32Size);
BD_S32 BD_MPI_M2M_WaitDone(BD_U32 u32Timeout);
BD_S32 BD_MPI_M2M_GetState(BD_U32 *u32State);


/* AXI M2M API */
BD_S32 BD_MPI_M2MAXI_Init(BD_U32 u32Chn);
BD_S32 BD_MPI_M2MAXI_Exit(BD_U32 u32Chn);
BD_S32 BD_MPI_M2MAXI_SetCopy(BD_U32 u32Chn, 
	BD_U32 u32SrcAddr, BD_U32 u32SrcWidth, BD_U32 u32SrcHeight, BD_U32 u32SrcStride, 
	BD_U32 u32DstAddr, BD_U32 u32DstStride);
BD_S32 BD_MPI_M2MAXI_WaitDone(BD_U32 u32Chn, BD_U32 u32Timeout);
BD_S32 BD_MPI_M2MAXI_GetState(BD_U32 u32Chn, BD_U32 *u32State);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
