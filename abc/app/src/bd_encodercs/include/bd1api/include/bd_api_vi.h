
/// File: bd_api_vi.h
/// Date: 2015/03

#ifndef __BD_API_VI_H_
#define __BD_API_VI_H_

#include "bd_common.h"
#include "bd_comm_vi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

BD_S32 BD_MPI_VI_InitDev(VI_DEV_INPUT_E enPrimaryInputMode, VI_DEV_INPUT_E enSecondaryInputMode);
BD_VOID BD_MPI_VI_ExitDev(BD_VOID);
BD_S32 BD_MPI_VI_SetDevAttr(VI_DEV enViDev, const VI_DEV_ATTR_S *pstDevAttr);
BD_S32 BD_MPI_VI_GetDevAttr(VI_DEV enViDev, VI_DEV_ATTR_S *pstDevAttr);
BD_S32 BD_MPI_VI_EnableDev(VI_DEV enViDev);
BD_S32 BD_MPI_VI_DisableDev(VI_DEV enViDev, BD_U32 u32MilliSec);
BD_S32 BD_MPI_VI_SetChnAttr(VI_CHN ViChn, const VI_CHN_ATTR_S *pstAttr);
BD_S32 BD_MPI_VI_GetChnAttr(VI_CHN ViChn, VI_CHN_ATTR_S *pstAttr);
BD_S32 BD_MPI_VI_EnableChn(VI_CHN ViChn);
BD_S32 BD_MPI_VI_DisableChn(VI_CHN ViChn);
BD_S32 BD_MPI_VI_PauseChn(VI_CHN ViChn);
BD_S32 BD_MPI_VI_ResumeChn(VI_CHN ViChn);
BD_S32 BD_MPI_VI_GetFrame(VI_CHN ViChn, VIDEO_FRAME_INFO_S *pstFrameInfo);
BD_S32 BD_MPI_VI_GetFrameTimeOut(VI_CHN ViChn, VIDEO_FRAME_INFO_S *pstFrameInfo, BD_U32 u32MilliSec);
BD_S32 BD_MPI_VI_ReleaseFrame(VI_CHN ViChn, VIDEO_FRAME_INFO_S *pstFrameInfo);
BD_S32 BD_MPI_VI_SetFrameDepth(VI_CHN ViChn, BD_U32 u32Depth);
BD_S32 BD_MPI_VI_GetFrameDepth(VI_CHN ViChn, BD_U32 *pu32Depth);
BD_S32 BD_MPI_VI_SetFrameRate(VI_CHN ViChn, BD_S32 s32SrcFrameRate, BD_S32 s32FrameRate);
BD_S32 BD_MPI_VI_GetFrameRate(VI_CHN ViChn, BD_S32* ps32SrcFrameRate, BD_S32* ps32FrameRate);
BD_S32 BD_MPI_VI_Query(VI_CHN ViChn, VI_CHN_STAT_S *pstStat);
BD_S32 BD_MPI_VI_EnableChnInterrupt(VI_CHN ViChn);
BD_S32 BD_MPI_VI_DisableChnInterrupt(VI_CHN ViChn);
BD_S32 BD_MPI_VI_GetPviState(BD_U32* u32LockState, BD_U32* u32Format, BD_U32* u32Resol, BD_U32* u32Freq, BD_U32 u32Timeout);
BD_S32 BD_MPI_VI_SetPviFormat(VI_PVI_FORMAT_E enPviFormat);
BD_S32 BD_MPI_VI_LoadPviFileConfig(const BD_CHAR *pszTxtFile);
BD_S32 BD_MPI_VI_SetFrameSize(VI_CHN ViChn, BD_U32 u32Width, BD_U32 u32Height);

BD_S32 BD_MPI_VI_UsrBind(VI_CHN ViChn);
BD_S32 BD_MPI_VI_UsrUnBind(VI_CHN ViChn);
BD_S32 BD_MPI_VI_UsrGetFrame(VI_CHN ViChn, BD_U8** pVirBuf, BD_S32* s32BufIndex, BD_U32 u32MilliSec);
BD_S32 BD_MPI_VI_UsrReleaseFrame(VI_CHN ViChn, BD_U32 u32BufIndex);
BD_S32 BD_MPI_VI_GetBufferAddress(VI_CHN ViChn, BD_U32 u32BufIndex, BD_U32* u32PhyAddr, BD_U32* u32VirtAddr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
