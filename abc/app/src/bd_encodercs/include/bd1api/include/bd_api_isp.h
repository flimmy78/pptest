
/// File: bd_api_isp.h
/// Date: 2015/03

#ifndef __BD_API_ISP_H_
#define __BD_API_ISP_H_	1

#include "bd_common.h"
#include "bd_comm_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

BD_S32 BD_MPI_ISP_Init(BD_VOID);
BD_S32 BD_MPI_ISP_Exit(BD_VOID);
BD_S32 BD_MPI_ISP_SetImageAttr(ISP_IMAGE_ATTR_S *pstImageAttr);
BD_S32 BD_MPI_ISP_SetInputTiming(ISP_INPUT_TIMING_S *pstInputTiming);
BD_S32 BD_MPI_ISP_LoadFileConfig(const BD_CHAR *pszCcfFile, const BD_CHAR *pszTxtFile);
BD_S32 BD_MPI_ISP_LoadManrConfig(BD_BOOL bFilterEnabled);

BD_S32 BD_MPI_ISP_QueryI2CAccess(BD_VOID);
BD_S32 BD_MPI_ISP_RequestI2CAccess(BD_U32 u32MilliSec);
BD_S32 BD_MPI_ISP_ReleaseI2CAccess(BD_VOID);
BD_S32 BD_MPI_ISP_SetClockPolarity(ISP_INPUT_POLARITY_S* pstInputPolarity);
BD_S32 BD_MPI_ISP_GetClockPolarity(ISP_INPUT_POLARITY_S* pstInputPolarity);

BD_S32 BD_MPI_ISP_SetDehazeAttr(ISP_DEHAZE_ATTR_S *pstDehazeAttr);
BD_S32 BD_MPI_ISP_GetDehazeAttr(ISP_DEHAZE_ATTR_S *pstDehazeAttr);
BD_S32 BD_MPI_ISP_GetDehazeDetectInfo(ISP_DEHAZE_DETECT_STATE_S *pstDehazeDetectInfo);
BD_S32 BD_MPI_ISP_SetDehazeEnable(BD_BOOL bEnable);

BD_S32 BD_MPI_ISP_SetManrAttr(ISP_MANR_ATTR_S *pstManrAttr);
BD_S32 BD_MPI_ISP_SetManrFilter(ISP_MANR_FILTER_S *pstManrFilter);
BD_S32 BD_MPI_ISP_GetManrAttr(ISP_MANR_ATTR_S *pstManrAttr);
BD_S32 BD_MPI_ISP_GetManrFilter(ISP_MANR_FILTER_S *pstManrFilter);

BD_S32 BD_MPI_ISP_SetDefocusAttr(ISP_DEFOCUS_ATTR_S *pstDefocusAttr);
BD_S32 BD_MPI_ISP_GetDefocusAttr(ISP_DEFOCUS_ATTR_S *pstDefocusAttr);
BD_S32 BD_MPI_ISP_GetDefocusDetectInfo(ISP_DEFOCUS_STATE_S *pstDefocusState);		

BD_S32 BD_MPI_ISP_SetMotionDetectAttr(ISP_MOTION_DETECT_ATTR_S *pstMotionDetectAttr);
BD_S32 BD_MPI_ISP_GetMotionDetectAttr(ISP_MOTION_DETECT_ATTR_S *pstMotionDetectAttr);
BD_S32 BD_MPI_ISP_SetMotionMask(ISP_MOTION_DETECT_MASK_S *pstMotionDetectMask);
BD_S32 BD_MPI_ISP_GetMotionMask(ISP_MOTION_DETECT_MASK_S *pstMotionDetectMask);
BD_S32 BD_MPI_ISP_GetMotionDetectInfo(ISP_MOTION_DETECT_DATA_S *pstMotionDetectRegion);
BD_S32 BD_MPI_ISP_SetMotionEnable(BD_BOOL bEnable);

BD_S32 BD_MPI_ISP_SetBlindDetectAttr(ISP_BLIND_DETECT_ATTR_S *pstBlindDetectAttr);
BD_S32 BD_MPI_ISP_GetBlindDetectAttr(ISP_BLIND_DETECT_ATTR_S *pstBlindDetectAttr);
BD_S32 BD_MPI_ISP_SetBlindMask(ISP_BLIND_DETECT_MASK_S *pstBlindDetectMask);
BD_S32 BD_MPI_ISP_GetBlindMask(ISP_BLIND_DETECT_MASK_S *pstBlindDetectMask);
BD_S32 BD_MPI_ISP_GetBlindDetectInfo(ISP_BLIND_DETECT_STATE_S *pstBlindDetectInfo);
BD_S32 BD_MPI_ISP_SetBlindEnable(BD_BOOL bEnable);

BD_S32 BD_MPI_ISP_SetNightDetectAttr(ISP_NIGHT_DETECT_ATTR_S *pstNightDetectAttr);
BD_S32 BD_MPI_ISP_GetNightDetectAttr(ISP_NIGHT_DETECT_ATTR_S *pstNightDetectAttr);
BD_S32 BD_MPI_ISP_GetNightDetectInfo(ISP_NIGHT_DETECT_STATE_S *pstNightDetectInfo);
BD_S32 BD_MPI_ISP_SetNightEnable(BD_BOOL bEnable);

BD_S32 BD_MPI_ISP_SetDayDetectAttr(ISP_DAY_DETECT_ATTR_S *pstDayDetectAttr);
BD_S32 BD_MPI_ISP_GetDayDetectAttr(ISP_DAY_DETECT_ATTR_S *pstDayDetectAttr);
BD_S32 BD_MPI_ISP_GetDayDetectInfo(ISP_DAY_DETECT_STATE_S *pstDayDetectInfo);
BD_S32 BD_MPI_ISP_SetDayEnable(BD_BOOL bEnable);

BD_S32 BD_MPI_ISP_EventDetect(BD_U32 u32MilliSec, ISP_EVENT_DETECT_MASK* eventMask);

BD_S32 BD_MPI_ISP_FrameRate(BD_VOID);

BD_S32 BD_MPI_ISP_SetGammaTable(ISP_GAMMATABLE_TYPE_E enTableType, BD_U8* u8TableValues);
BD_S32 BD_MPI_ISP_GetGammaTable(ISP_GAMMATABLE_TYPE_E enTableType, BD_U8* u8TableValues);
BD_S32 BD_MPI_ISP_SetISPFunc(BD_U32 u32FuncHigh, BD_U32 u32FuncLow);
BD_S32 BD_MPI_ISP_GetISPFunc(BD_U32* u32FuncHigh, BD_U32* u32FuncLow);
BD_S32 BD_MPI_ISP_SetColorCorrectionMatrix(BD_U8* u8TableValues);
BD_S32 BD_MPI_ISP_GetColorCorrectionMatrix(BD_U8* u8TableValues);
BD_S32 BD_MPI_ISP_SetColorSaturationMatrix(BD_U8* u8TableValues);
BD_S32 BD_MPI_ISP_GetColorSaturationMatrix(BD_U8* u8TableValues);
BD_S32 BD_MPI_ISP_SetMaxGlobalGain(BD_U8 u8MaxGlobalGain);
BD_S32 BD_MPI_ISP_GetMaxGlobalGain(BD_U8* u8MaxGlobalGain);
BD_S32 BD_MPI_ISP_SetLensShadingCorrectGain(BD_U8 u8RGain, BD_U8 u8G1Gain, BD_U8 u8G2Gain, BD_U8 u8BGain);
BD_S32 BD_MPI_ISP_GetLensShadingCorrectGain(BD_U8* u8RGain, BD_U8* u8G1Gain, BD_U8* u8G2Gain, BD_U8* u8BGain);
BD_S32 BD_MPI_ISP_SetLensShadingCorrectScale(BD_U32 u32RScale, BD_U32 u32G1Scale, BD_U32 u32G2Scale, BD_U32 u32BScale);
BD_S32 BD_MPI_ISP_GetLensShadingCorrectScale(BD_U32* u32RScale, BD_U32* u32G1Scale, BD_U32* u32G2Scale, BD_U32* u32BScale);
BD_S32 BD_MPI_ISP_SetAutoControl(BD_U32 u32AutoMode);
BD_S32 BD_MPI_ISP_GetAutoControl(BD_U32* u32AutoMode);

BD_S32 BD_MPI_ISP_SetFlickerControl(ISP_FLICKER_TYPE_E enFlickerType);
BD_S32 BD_MPI_ISP_GetFlickerControl(ISP_FLICKER_TYPE_E* enFlickerType);
BD_S32 BD_MPI_ISP_SetIrisControl(ISP_IRIS_TYPE_E enIrisType);
BD_S32 BD_MPI_ISP_GetIrisControl(ISP_IRIS_TYPE_E* enIrisType);
BD_S32 BD_MPI_ISP_GetExposureControl(BD_U32* u32Time, BD_U32* u32Value);
BD_S32 BD_MPI_ISP_SetExposureBrightnessControl(BD_U32 u32Brightness);
BD_S32 BD_MPI_ISP_GetExposureBrightnessControl(BD_U32* u32Brightness);
BD_S32 BD_MPI_ISP_SetAGCControl(BD_U32 u32AgcValue);
BD_S32 BD_MPI_ISP_GetAGCControl(BD_U32* u32AgcValue);
BD_S32 BD_MPI_ISP_SetBLCControl(ISP_BLC_TYPE_E enBLCType);
BD_S32 BD_MPI_ISP_GetBLCControl(ISP_BLC_TYPE_E* enBLCType);
BD_S32 BD_MPI_ISP_SetShutterControl(ISP_SHUTTER_TYPE_E enShutterMode);
BD_S32 BD_MPI_ISP_GetShutterControl(ISP_SHUTTER_TYPE_E* enShutterMode);
BD_S32 BD_MPI_ISP_SetSensorUpControl(ISP_SENSORUP_TYPE_E enSensorUpType);
BD_S32 BD_MPI_ISP_GetSensorUpControl(ISP_SENSORUP_TYPE_E* enSensorUpType);
BD_S32 BD_MPI_ISP_SetAWBControl(ISP_AWB_MODE_E enAWBMode, BD_U32 u32RGain, BD_U32 u32BGain);
BD_S32 BD_MPI_ISP_GetAWBControl(ISP_AWB_MODE_E *enAWBMode, BD_U32* u32RGain, BD_U32* u32BGain);
BD_S32 BD_MPI_ISP_SetNoiseFilterControl(ISP_NOISEFILTER_MODE_E enNoiseFilterType);
BD_S32 BD_MPI_ISP_GetNoiseFilterControl(ISP_NOISEFILTER_MODE_E* enNoiseFilterType);
BD_S32 BD_MPI_ISP_SetFlipControl(ISP_FLIP_MODE_E enFlipMode);
BD_S32 BD_MPI_ISP_GetFlipControl(ISP_FLIP_MODE_E* enFlipMode);
BD_S32 BD_MPI_ISP_SetBayerSequenceControl(ISP_BAYER_FORMAT_E enBayerSequence);
BD_S32 BD_MPI_ISP_GetBayerSequenceControl(ISP_BAYER_FORMAT_E* enBayerSequence);
BD_S32 BD_MPI_ISP_SetSaturationControl(BD_U32 u32Value);
BD_S32 BD_MPI_ISP_GetSaturationControl(BD_U32* u32Value);
BD_S32 BD_MPI_ISP_SetBrightnessControl(BD_U32 u32Value);
BD_S32 BD_MPI_ISP_GetBrightnessControl(BD_U32* u32Value);
BD_S32 BD_MPI_ISP_SetContrastControl(BD_U32 u32Value);
BD_S32 BD_MPI_ISP_GetContrastControl(BD_U32* u32Value);
BD_S32 BD_MPI_ISP_SetSharpnessControl(BD_U32 u32Value);
BD_S32 BD_MPI_ISP_GetSharpnessControl(BD_U32* u32Value);
BD_S32 BD_MPI_ISP_GetTotalGainControl(BD_U32* u32Value);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif
