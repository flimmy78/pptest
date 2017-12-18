
/// File : bd_api_sys.h
/// Date : 2015/04

#include "bd_type.h"
#include "bd_common.h"
#include "bd_comm_sys.h"

#ifndef __BD_API_SYS_H__
#define __BD_API_SYS_H__

/******************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */
/******************************************/

BD_S32 BD_MPI_SYS_Init();
BD_S32 BD_MPI_SYS_Exit();
BD_S32 BD_MPI_SYS_SetConf(const MPP_SYS_CONF_S *pstSysConf);
BD_S32 BD_MPI_SYS_GetConf(MPP_SYS_CONF_S *pstSysConf);
BD_S32  BD_MPI_SYS_Bind(MPP_CHN_S *pstSrcChn, MPP_CHN_S *pstDestChn);
BD_S32  BD_MPI_SYS_UnBind(MPP_CHN_S *pstSrcChn, MPP_CHN_S *pstDestChn);
BD_S32  BD_MPI_SYS_GetBindbyDest(MPP_CHN_S *pstDestChn, MPP_CHN_S *pstSrcChn);
BD_S32 BD_MPI_SYS_MmzAlloc(BD_U32 *pu32PhyAddr, BD_VOID **ppVirtAddr, 
        const BD_CHAR *strMmb, const BD_CHAR *strZone, BD_U32 u32Len);
BD_S32 BD_MPI_SYS_MmzAlloc_Cached(BD_U32 *pu32PhyAddr, BD_VOID **ppVitAddr, 
        const BD_CHAR *pstrMmb, const BD_CHAR *pstrZone, BD_U32 u32Len);
BD_S32 BD_MPI_SYS_MmzFree(BD_U32 u32PhyAddr, BD_VOID *pVirtAddr);
BD_S32 BD_MPI_SYS_MmzFlushCache(BD_U32 u32PhyAddr, BD_VOID *pVitAddr, BD_U32 u32Offset, BD_U32 u32Size);
BD_S32 BD_MPI_SYS_MmzWbackInvCache(BD_U32 u32PhyAddr, BD_VOID *pVitAddr, BD_U32 u32Offset, BD_U32 u32Size);
BD_S32 BD_MPI_SYS_MmzWbackCache(BD_U32 u32PhyAddr, BD_VOID *pVitAddr, BD_U32 u32Offset, BD_U32 u32Size);
BD_S32 BD_MPI_SYS_MmzInvalidateCache(BD_U32 u32PhyAddr, BD_VOID *pVitAddr, BD_U32 u32Offset, BD_U32 u32Size);
BD_VOID * BD_MPI_SYS_Mmap(BD_U32 u32PhyAddr, BD_U32 u32Size);
BD_S32 BD_MPI_SYS_Munmap(BD_VOID* pVirAddr, BD_U32 u32Size);
BD_S32 BD_MPI_SYS_SetMemConf(MPP_CHN_S *pstMppChn,const BD_CHAR *pcMmzName);
BD_S32 BD_MPI_SYS_GetMemConf(MPP_CHN_S *pstMppChn,BD_CHAR *pcMmzName);
BD_S32 BD_MPI_SYS_MmzDMADesc(BD_U32 u32PhyAddr, BD_S32* s32BufFd);
BD_S32 BD_MPI_SYS_OpenI2CDevice(BD_U32 u32Chn);
BD_S32 BD_MPI_SYS_CloseI2CDevice(BD_U32 u32Chn);
BD_S32 BD_MPI_SYS_SetI2CSlave(BD_U32 u32Chn, BD_U8 u8ChipAddr);
BD_S32 BD_MPI_SYS_GetI2CSlave(BD_U32 u32Chn, BD_U8* u8ChipAddr);
BD_S32 BD_MPI_SYS_SetI2CWrite(BD_U32 u32Chn, BD_U8 u8RegAddr, BD_U8 u8RegVal);
BD_S32 BD_MPI_SYS_SetI2CRead(BD_U32 u32Chn, BD_U8 u8RegAddr, BD_U8* u8RegVal);


/******************************************/
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
/******************************************/
#endif /*__MPI_SYS_H__ */

