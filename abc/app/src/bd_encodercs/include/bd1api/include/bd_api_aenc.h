#ifndef __BD_API_AENC_H__
#define __BD_API_AENC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */
BD_VOID BD_MPI_AENC_INIT(BD_VOID);
BD_VOID BD_MPI_AENC_EXIT(BD_VOID);
BD_S32 BD_MPI_AENC_CreateChn(AENC_CHN AeChn, const AENC_CHN_ATTR_S *pstAttr);
BD_S32 BD_MPI_AENC_DestroyChn(AENC_CHN AeChn);

BD_S32 BD_MPI_AENC_SendFrame(AENC_CHN AeChn, const AUDIO_FRAME_S *pstFrm);	//, const AEC_FRAME_S *pstAecFrm);

BD_S32 BD_MPI_AENC_GetStream(AENC_CHN AeChn, AUDIO_STREAM_S *pstStream, BD_S32 s32MilliSec); 
BD_S32 BD_MPI_AENC_ReleaseStream(AENC_CHN AeChn, const AUDIO_STREAM_S *pstStream);

BD_S32 BD_MPI_AENC_GetFd(AENC_CHN AeChn);

//BD_S32 BD_MPI_AENC_RegeisterEncoder(BD_S32 *ps32Handle, AENC_ENCODER_S *pstEncoder);
//BD_S32 BD_MPI_AENC_UnRegisterEncoder(BD_S32 s32Handle);
//BD_S32 BD_MPI_AENC_SaveFile(AENC_CHN AeChn, AUDIO_SAVE_FILE_INFO_S *pstSaveFileInfo);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif


