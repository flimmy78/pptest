
/// File: bd_api_venc.h
/// Date: 2015/03

#ifndef __BD_API_VENC_H__
#define __BD_API_VENC_H__

#include "bd_common.h"
#include "bd_comm_video.h"
#include "bd_comm_venc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

BD_S32 BD_MPI_VENC_Init(BD_VOID);
BD_S32 BD_MPI_VENC_Exit(BD_VOID);

BD_S32 BD_MPI_VENC_CreateGroup(VENC_GRP VeGroup);
BD_S32 BD_MPI_VENC_DestroyGroup(VENC_GRP VeGroup);

BD_S32 BD_MPI_VENC_CreateChn(VENC_CHN VeChn, const VENC_CHN_ATTR_S *pstAttr);
BD_S32 BD_MPI_VENC_DestroyChn(VENC_CHN VeChn);

BD_S32 BD_MPI_VENC_RegisterChn(VENC_GRP VeGroup, VENC_CHN VeChn );
BD_S32 BD_MPI_VENC_UnRegisterChn(VENC_CHN VeChn);

BD_S32 BD_MPI_VENC_StartRecvPic(VENC_CHN VeChn);
BD_S32 BD_MPI_VENC_StopRecvPic(VENC_CHN VeChn);

BD_S32 BD_MPI_VENC_Query(VENC_CHN VeChn, VENC_CHN_STAT_S *pstStat);

BD_S32 BD_MPI_VENC_SetChnAttr( VENC_CHN VeChn, const VENC_CHN_ATTR_S *pstAttr);
BD_S32 BD_MPI_VENC_GetChnAttr( VENC_CHN VeChn, VENC_CHN_ATTR_S *pstAttr);

BD_S32 BD_MPI_VENC_GetStream(VENC_CHN VeChn, VENC_STREAM_S *pstStream, BD_BOOL bBlockFlag);
BD_S32 BD_MPI_VENC_ReleaseStream(VENC_CHN VeChn, VENC_STREAM_S *pstStream);

BD_S32 BD_MPI_VENC_SetMaxStreamCnt(VENC_CHN VeChn, BD_U32 u32MaxStrmCnt);
BD_S32 BD_MPI_VENC_GetMaxStreamCnt(VENC_CHN VeChn, BD_U32 *pu32MaxStrmCnt);

BD_S32 BD_MPI_VENC_GetFd(VENC_CHN VeChn);

BD_S32 BD_MPI_VENC_JpegSnap(VENC_CHN VeChn, BD_U32 u32Count);

BD_S32 BD_MPI_VENC_RequestIDR(VENC_CHN VeChn);

BD_S32 BD_MPI_VENC_SetRoiCfg(VENC_CHN VeChn, VENC_ROI_CFG_S *pstVencRoiCfg);
BD_S32 BD_MPI_VENC_GetRoiCfg(VENC_CHN VeChn, BD_U32 u32Index, VENC_ROI_CFG_S *pstVencRoiCfg);

BD_S32 BD_MPI_VENC_SetPause(VENC_CHN VeChn, BD_BOOL bPause);
BD_S32 BD_MPI_VENC_GetPause(VENC_CHN VeChn, BD_BOOL *pbPause);

BD_S32 BD_MPI_VENC_GetSkipCnt(VENC_CHN VeChn, BD_U32 *pu32SkipCnt);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __BD_API_VENC_H__ */

