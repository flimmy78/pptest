#ifndef __BD_API_AI_H__
#define __BD_API_AI_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */
BD_S32 BD_MPI_AI_INIT(BD_VOID);
BD_VOID BD_MPI_AI_EXIT(BD_VOID);
BD_S32 BD_MPI_AI_SetPubAttr(AUDIO_DEV AudioDevId, const AIO_ATTR_S *pstAttr);
BD_S32 BD_MPI_AI_GetPubAttr(AUDIO_DEV AudioDevId, AIO_ATTR_S *pstAttr);

BD_S32 BD_MPI_AI_Enable(AUDIO_DEV AudioDevId);
BD_S32 BD_MPI_AI_Disable(AUDIO_DEV AudioDevId);

BD_S32 BD_MPI_AI_EnableChn(AUDIO_DEV AudioDevId, AI_CHN AiChn);
BD_S32 BD_MPI_AI_DisableChn(AUDIO_DEV AudioDevId, AI_CHN AiChn);

BD_S32 BD_MPI_AI_GetFrame(AUDIO_DEV AudioDevId, AI_CHN AiChn, AUDIO_FRAME_S *pstFrm, BD_S32 s32MilliSec);	//AEC_FRAME_S *pstAecFrm, BD_BOOL bBlock);
BD_S32 BD_MPI_AI_ReleaseFrame(AUDIO_DEV AudioDevId, AI_CHN AiChn, AUDIO_FRAME_S *pstFrm);
BD_S32 BD_MPI_AI_SetChnParam(AUDIO_DEV AudioDevId, AI_CHN AiChn, AI_CHN_PARAM_S *pstChnParam);
BD_S32 BD_MPI_AI_GetChnParam(AUDIO_DEV AudioDevId, AI_CHN AiChn, AI_CHN_PARAM_S *pstChnParam);
BD_S32 BD_MPI_AI_SetADCVolume(AUDIO_DEV AudioDevId, BD_S32 s32VolumeDb);
BD_S32 BD_MPI_AI_GetADCVolume(AUDIO_DEV AudioDevId, BD_S32 *ps32VolumeDb);
BD_S32 BD_MPI_AI_SetMICVolume(AUDIO_DEV AudioDevId, BD_S32 s32VolumeDb);
BD_S32 BD_MPI_AI_GetMICVolume(AUDIO_DEV AudioDevId, BD_S32 *ps32VolumeDb);
BD_S32 BD_MPI_AI_SetMute(AUDIO_DEV AudioDevId, BD_BOOL bEnable);
BD_S32 BD_MPI_AI_GetMute(AUDIO_DEV AudioDevId, BD_BOOL *pbEnable);


//BD_S32 BD_MPI_AI_EnableAec(AUDIO_DEV AiDevId, AI_CHN AiChn, AUDIO_DEV AoDevId, AO_CHN AoChn);
//BD_S32 BD_MPI_AI_DisableAec(AUDIO_DEV AiDevId, AI_CHN AiChn);

//BD_S32 BD_MPI_AI_EnableReSmp(AUDIO_DEV AudioDevId, AI_CHN AiChn, AUDIO_RESAMPLE_ATTR_S *pstAttr);
//BD_S32 BD_MPI_AI_DisableReSmp(AUDIO_DEV AudioDevId, AI_CHN AiChn);

//BD_S32 BD_MPI_AI_EnableAnr(AUDIO_DEV AudioDevId, AI_CHN AiChn);
//BD_S32 BD_MPI_AI_DisableAnr(AUDIO_DEV AudioDevId, AI_CHN AiChn);
BD_S32 BD_MPI_AI_ClrPubAttr(AUDIO_DEV AudioDevId);

BD_S32 BD_MPI_AI_GetFd(AUDIO_DEV AudioDevId, AI_CHN AiChn);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif

