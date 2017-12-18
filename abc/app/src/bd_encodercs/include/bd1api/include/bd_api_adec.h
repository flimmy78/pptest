#ifndef __BD_API_ADEC_H__
#define __BD_API_ADEC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */
BD_VOID BD_MPI_ADEC_INIT(BD_VOID);
BD_VOID BD_MPI_ADEC_EXIT(BD_VOID);
BD_S32 BD_MPI_ADEC_CreateChn(ADEC_CHN AdChn, ADEC_CHN_ATTR_S *pstAttr);
BD_S32 BD_MPI_ADEC_DestroyChn(ADEC_CHN AdChn);

BD_S32 BD_MPI_ADEC_SendStream(ADEC_CHN AdChn, const AUDIO_STREAM_S *pstStream, BD_BOOL bBlock);

BD_S32 BD_MPI_ADEC_ClearChnBuf(ADEC_CHN AdChn);

BD_S32 BD_MPI_ADEC_GetFrame(ADEC_CHN AdChn, AUDIO_FRAME_S *pstFrm);

BD_S32 BD_MPI_ADEC_ReleaseFrame(ADEC_CHN AdChn, const AUDIO_FRAME_S *pstFrm);

//BD_S32 BD_MPI_ADEC_RegeisterDecoder(HI_S32 *ps32Handle, ADEC_DECODER_S *pstDecoder);
//BD_S32 BD_MPI_ADEC_UnRegisterDecoder(HI_S32 s32Handle);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif
