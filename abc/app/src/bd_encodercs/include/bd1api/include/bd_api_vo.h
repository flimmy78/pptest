#ifndef __BD_API_VO_H__
#define __BD_API_VO_H__

#include "bd_common.h"
#include "bd_comm_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef enum bdSTREAM_TYPE_E{
	STREAM_TYPE_LIVE_CAM0 = 0,
	STREAM_TYPE_LIVE_CAM1,
	STREAM_TYPE_LIVE_CAM2,
	STREAM_TYPE_DEC_CAM0,
	STREAM_TYPE_DEC_CAM1,
	STREAM_TYPE_UVC_CAM0,
	STREAM_TYPE_UVC_CAM1,
	STREAM_TYPE_BUTT
}STREAM_TYPE_E;

typedef enum {
    DU_FORMAT_RGB565          = 0x0,
    DU_FORMAT_RGBA4444,
    DU_FORMAT_RGBA5551,
    DU_FORMAT_RGB888,
    DU_FORMAT_RGBA8888,
    DU_FORMAT_Gray,
    DU_FORMAT_Index,
    DU_FORMAT_YUV420I,
    DU_FORMAT_YUV422I,
    DU_FORMAT_YUV444I,
    DU_FORMAT_YUV420P,
    DU_FORMAT_YUV422P,
    DU_FORMAT_YUV444P,
    DU_FORMAT_YUV422V,
    DU_FORMAT_YUV422IR,
} eDU_FORMAT;

typedef struct bdSTREAM_INFO_S{
	BD_U32	u32SrcWidth;
	BD_U32	u32SrcHeight;
	BD_U32	u32SrcStride;
	BD_U32	u32SrcColorStride;
	BD_U32	u32DstX;
	BD_U32	u32DstY;
	BD_U32	u32SrcCropWidth;	// used only in the sclr image
	BD_U32	u32SrcCropHeight;	// used only in the sclr image
	BD_U32	u32SrcCropOffsetX;	// used only in the sclr image
	BD_U32	u32SrcCropOffsetY;	// used only in the sclr image
	BD_U32	u32SrcEnCrop;		// used only in the sclr image
	BD_U32	u32DstWidth;
	BD_U32	u32DstHeight;
	BD_U32	u32DstStride;
	eDU_FORMAT	u32ColorFmt;
	STREAM_TYPE_E	enStreamType;
	BD_BOOL	bValid;
}STREAM_INFO_S;

typedef struct bdGUI_INFO_S{
	//GFXA
	RECT_S	stGUIRect;
	PIXEL_FORMAT_E enPixelFormat;
	BD_BOOL bValid;
	//GFXBCD
#if 1 
	BD_BOOL bValidGfxB;
	RECT_S	stGUIRectGfxB;
	BD_U32	u32LutAddrGfxB;
	BD_U32	u32RleAddrGfxB;
	BD_U32	u32AttributeGfxB;
	BD_BOOL bValidGfxC;
	RECT_S	stGUIRectGfxC;
	BD_U32	u32LutAddrGfxC;
	BD_U32	u32RleAddrGfxC;
	BD_U32	u32AttributeGfxC;
	BD_BOOL bValidGfxD;
	RECT_S	stGUIRectGfxD;
	BD_U32	u32LutAddrGfxD;
	BD_U32	u32RleAddrGfxD;
	BD_U32	u32AttributeGfxD;
#endif
}GUI_INFO_S;

typedef struct bdRMF_INFO_S{
	ROTATE_E	en8Rotate;
	BD_BOOL bEnMirror;
	BD_BOOL	bEnFlip;
}RMF_INFO_S;

typedef enum bdVO_DEVICE_TYPE_E{
	VO_DEVICE_TYPE_LCD = 0,
	VO_DEVICE_TYPE_BTO,
	VO_DEVICE_TYPE_TV,
	VO_DEVICE_TYPE_MEM,
	VO_DEVICE_TYPE_BTO_LCD,
	VO_DEVICE_TYPE_TV_LCD,
	VO_DEVICE_TYPE_BUTT
}VO_DEVICE_TYPE_E;

typedef struct bdVODEV_INFO_S{
	VO_DEVICE_TYPE_E enType;
	BD_U32 u32Reserved;
}VODEV_INFO_S;

typedef enum bdLCD_TYPE_E{
	LCD_TYPE_CPU_8BIT = 0,
//	LCD_TYPE_CPU_16BIT,
	LCD_TYPE_RGB,
	LCD_TYPE_BT656,
	LCD_TYPE_BT1120,
	LCD_TYPE_BUTT
}LCD_TYPE_E;

typedef enum bdBTO_TYPE_E{
	BT_TYPE_601 = 0,
	BT_TYPE_656,
	BT_TYPE_1120,
	BT_TYPE_YC16,
	BT_TYPE_BUTT
}BTO_TYPE_E;

typedef struct bdVO_LCD_CONFIG_S{
	BD_U32	u32Width;
	BD_U32	u32Height;
	BD_U32	u32VBlank;
	BD_U32	u32HBlank;
	BD_U32	u32VBackPorch;
	BD_U32	u32HBackPorch;
	BD_U32	u32VFrontPorch;
	BD_U32	u32HFrontPorch;
	LCD_TYPE_E enLCDType;
	BD_BOOL bUseRMF;
	BD_BOOL bValid;
}VO_LCD_CONFIG_S;

typedef struct bdVO_BTO_CONFIG_S{
	BD_U32	u32Width;
	BD_U32	u32Height;
	BD_U32	u32VBackPorch;
	BD_U32	u32HBackPorch;
	BD_U32	u32VFrontPorch;
	BD_U32	u32HFrontPorch;
	BTO_TYPE_E enBTOType;	
	BD_BOOL	bInterlaced;	// 0:Progressive, 0:Interlaced
	BD_BOOL bUseRMF;
	BD_BOOL bValid;
}VO_BTO_CONFIG_S;

typedef struct bdVO_TV_CONFIG_S{
	BD_BOOL	bTV_PAL;		// 0:NTSC, 1:PAL
	BD_BOOL bUseRMF;
	BD_BOOL bValid;
}VO_TV_CONFIG_S;

typedef struct bdVO_MEM_CONFIG_S{
	BD_U32	u32Width;
	BD_U32	u32Height;
	eDU_FORMAT	u32ColorFmt;
	BD_BOOL bUseRMF;
	BD_BOOL bValid;
}VO_MEM_CONFIG_S;

typedef struct bdVO_DEVICE_CONFIG_S{
	VO_LCD_CONFIG_S stLCDConfig;
	VO_BTO_CONFIG_S stBTOConfig;
	VO_TV_CONFIG_S stTVConfig;
	VO_MEM_CONFIG_S stMEMConfig;
	BD_BOOL bDualOut;
	BD_BOOL bUsePIP;
	BD_U32 u32MemUsed;
}VO_DEVICE_CONFIG_S;

typedef struct bdVO_CONFIG_S{
	STREAM_INFO_S stMainStream;
	STREAM_INFO_S stPipStream;
	STREAM_INFO_S stPip2Stream;
	GUI_INFO_S	stGUIInfo;
	RMF_INFO_S stRMFInfo;
	RMF_INFO_S stDualRMF;
	VODEV_INFO_S stVODEVInfo;
	BD_U32 u32ViChnSync;
	BD_U32 u32PbJPGMode;
}VO_CONFIG_S;


BD_S32 BD_MPI_VO_SetDeviceConfig(VO_DEVICE_CONFIG_S *stVoDeviceConfig);
BD_S32 BD_MPI_VO_GetDeviceConfig(VO_DEVICE_CONFIG_S *stVoDeviceConfig);
	
BD_S32 BD_MPI_VO_SetConfig(VO_CONFIG_S *stVoConfig);
BD_S32 BD_MPI_VO_GetConfig(VO_CONFIG_S *stVoConfig);

BD_S32 BD_MPI_VO_Create(void);
BD_S32 BD_MPI_VO_Destroy(void);
BD_S32 BD_MPI_VO_Enable(void);
BD_S32 BD_MPI_VO_Disable(void);
BD_S32 BD_MPI_VO_Pause(void);
BD_S32 BD_MPI_VO_Resume(void);
BD_S32 BD_MPI_VO_Refresh(void);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif


