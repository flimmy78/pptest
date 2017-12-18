
/// File: bd_api_vb.h
/// Date: 2016/01

#ifndef __BD_API_VB_H__
#define __BD_API_VB_H__

#include "bd_comm_vb.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */


VB_POOL BD_MPI_VB_CreatePool(BD_U32 u32BlkSize, BD_U32 u32BlkCnt, const BD_CHAR *pcMmzName);

BD_S32 BD_MPI_VB_DestroyPool(VB_POOL Pool);
VB_BLK BD_MPI_VB_GetBlock(VB_POOL Pool, BD_U32 u32BlkSize, const BD_CHAR *pcMmzName);
BD_S32 BD_MPI_VB_ReleaseBlock(VB_BLK Block);

BD_U32 BD_MPI_VB_Handle2PhyAddr(VB_BLK Block);
VB_POOL BD_MPI_VB_Handle2PoolId(VB_BLK Block);

BD_S32 BD_MPI_VB_Init (BD_VOID);
BD_S32 BD_MPI_VB_Exit (BD_VOID);
BD_S32 BD_MPI_VB_SetConf (const VB_CONF_S *pstVbConf);
BD_S32 BD_MPI_VB_GetConf (VB_CONF_S *pstVbConf);

BD_S32 BD_MPI_VB_MmapPool(VB_POOL Pool);
BD_S32 BD_MPI_VB_MunmapPool(VB_POOL Pool);
BD_S32 BD_MPI_VB_GetBlkVirAddr(VB_POOL Pool, BD_U32 u32PhyAddr, BD_VOID **ppVirAddr);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif 
