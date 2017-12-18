#ifndef __BD_API_VDEC_H__
#define __BD_API_VDEC_H__

#include "bd_common.h"
#include "bd_comm_vdec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

BD_S32 BD_VDEC_INIT(VDEC_SYS_ATTR_S Attr);
BD_VOID BD_VDEC_EXIT(BD_VOID);

BD_S32 BD_MPI_VDEC_CreateChn(VDEC_CHN VdChn, VDEC_CHN_ATTR_S pstAttr);
BD_S32 BD_MPI_VDEC_DestroyChn(VDEC_CHN VdChn);
BD_S32 BD_MPI_VDEC_StartRecvStream(VDEC_CHN VdChn);
BD_S32 BD_MPI_VDEC_SendStream(VDEC_CHN VdChn, const VDEC_BITSTREAM_S *pstStream, BD_S32 s32MilliSec);
BD_S32 BD_MPI_VDEC_GetFd(VDEC_CHN VdChn);
//type, left frame N, decoded frame N
BD_S32 BD_MPI_VDEC_Query(VDEC_CHN VdChn, VDEC_CHN_STAT_S *pstStat);
//type, buffSize, width, height
BD_S32 BD_MPI_VDEC_GetChnAttr(VDEC_CHN VdChn, VDEC_CHN_ATTR_S *pstAttr);

BD_U8* BD_MPI_VDEC_GetBitStreamAddr(VDEC_CHN VdChn);

//BD_S32 BD_MPI_VDEC_ResetChn(VDEC_CHN VdChn);

//Queries the status of a VDEC ch

//Obtains decoded pictures form VDEC ch
//BD_S32 BD_MPI_VDEC_GetImage(VDEC_CHN VdChn, VIDEO_FRAME_INFO_S *pstFrameInfo, BD_S32 s32MilliSec);

//Obtains the device file descriptor(FD) corresponding to a VDEC ch

//Release the buffer for storing the decoded pictures of a VDEC ch
//BD_S32 BD_MPI_VDEC_ReleaseImage(VDEC_CHN VdChn, VIDEO_FRAME_INFO_S *pstFrameInfo);

//----->
//ch parameter??
//BD_S32 BD_MPI_VDEC_SetChnParam(VDEC_CHN VdChn, VDEC_CHN_PARAM_S* pstParam);
//BD_S32 BD_MPI_VDEC_GetChnParam(VDEC_CHN VdChn, VDEC_CHN_PARAM_S* pstParam);

//BD_S32 BD_MPI_VDEC_StopRecvStream(VDEC_CHN VdChn);
//BD_S32 BD_MPI_VDEC_SetProtocolParam(VDEC_CHN VdChn,VDEC_PRTCL_PARAM_S *pstParam);
//BD_S32 BD_MPI_VDEC_GetProtocolParam(VDEC_CHN VdChn,VDEC_PRTCL_PARAM_S *pstParam);

/* s32MilliSec: -1 is block£¬ 0 is no block£¬other positive number is timeout */

//BD_S32 BD_MPI_VDEC_GetUserData(VDEC_CHN VdChn, VDEC_USERDATA_S *pstUserData, BD_S32 s32MilliSec);
//BD_S32 BD_MPI_VDEC_ReleaseUserData(VDEC_CHN VdChn, VDEC_USERDATA_S *pstUserData);

//BD_S32 BD_MPI_VDEC_SetRotate(VDEC_CHN VdChn, ROTATE_E enRotate);
//BD_S32 BD_MPI_VDEC_GetRotate(VDEC_CHN VdChn, ROTATE_E *penRotate);

//BD_S32 BD_MPI_VDEC_GetChnLuma(VDEC_CHN VdChn, VDEC_CHN_LUM_S *pstLuma);

//BD_S32 BD_MPI_VDEC_SetUserPic(VDEC_CHN VdChn, VIDEO_FRAME_INFO_S *pstUsrPic);
//BD_S32 BD_MPI_VDEC_EnableUserPic(VDEC_CHN VdChn, BD_BOOL bInstant);
//BD_S32 BD_MPI_VDEC_DisableUserPic(VDEC_CHN VdChn);

//BD_S32 BD_MPI_VDEC_SetDisplayMode(VDEC_CHN VdChn, VIDEO_DISPLAY_MODE_E enDisplayMode);
//BD_S32 BD_MPI_VDEC_GetDisplayMode(VDEC_CHN VdChn, VIDEO_DISPLAY_MODE_E *penDisplayMode);

//BD_S32 BD_MPI_VDEC_SetChnVBCnt(VDEC_CHN VdChn, BD_U32 u32BlkCnt);
//BD_S32 BD_MPI_VDEC_GetChnVBCnt(VDEC_CHN VdChn, BD_U32 *pu32BlkCnt);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif

