#ifndef __SAMPLE_COMM_H__
#define __SAMPLE_COMM_H__

#include "bd_common.h"
#include "bd_comm_sys.h"
#include "bd_comm_vb.h"
#include "bd_comm_isp.h"
#include "bd_comm_vi.h"
#include "bd_comm_mi.h"
#include "bd_comm_vo.h"
#include "bd_comm_venc.h"
#include "bd_comm_vdec.h"
//#include "bd_comm_vda.h"
#include "bd_comm_region.h"
#include "bd_comm_adec.h"
#include "bd_comm_aenc.h"
//#include "bd_comm_ai.h"
//#include "bd_comm_ao.h"
#include "bd_comm_aio.h"
#include "bd_comm_isp.h"
#include "bd_defines.h"
#include "bd_comm_ive.h"
#include "bd_comm_m2m.h"


#include "bd_api_sys.h"
#include "bd_api_vb.h"
#include "bd_api_vi.h"
#include "bd_api_mi.h"
#include "bd_api_vo.h"
#include "bd_api_venc.h"
#include "bd_api_vdec.h"
//#include "bd_api_vda.h"
#include "bd_api_region.h"
#include "bd_api_adec.h"
#include "bd_api_aenc.h"
#include "bd_api_ai.h"
#include "bd_api_ao.h"
#include "bd_api_isp.h"
#include "bd_api_sns.h"
#include "bd_api_ive.h"
#include "bd_api_m2m.h"

#include "bd_sns_ctrl.h"
#include "pthread.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

/*******************************************************
    macro define 
*******************************************************/
#define ALIGN_BACK(x, a)              ((a) * (((x) / (a))))
#define SAMPLE_SYS_ALIGN_WIDTH      64
#define SAMPLE_PIXEL_FORMAT         PIXEL_FORMAT_YUV_SEMIPLANAR_420

#define TW2865_FILE "/dev/tw2865dev"
#define TW2960_FILE "/dev/tw2960dev"
#define TLV320_FILE "/dev/tlv320aic31"


#if BDCBDP == BD3518_V100 //FIXME
#define SAMPLE_VO_DEV_DSD1 0
#define SAMPLE_VO_DEV_DSD0 SAMPLE_VO_DEV_DSD1
#else
#error BDCBDP define may be error
#endif

/*** for init global parameter ***/
#define SAMPLE_ENABLE 		    1
#define SAMPLE_DISABLE 		    0
#define SAMPLE_NOUSE 		    -1

#define SAMPLE_AUDIO_AI_DEV 0
#define SAMPLE_AUDIO_AO_DEV 0


#define SAMPLE_AUDIO_HDMI_AO_DEV 3


#define SAMPLE_PRT(fmt...)   \
    do {\
        printf("[%s]-%d: ", __FUNCTION__, __LINE__);\
        printf(fmt);\
       }while(0)

/*******************************************************
    enum define 
*******************************************************/

typedef enum sample_ispcfg_opt_e
{
    CFG_OPT_NONE    = 0,
    CFG_OPT_SAVE    = 1,
    CFG_OPT_LOAD    = 2,
    CFG_OPT_BUTT
} SAMPLE_CFG_OPT_E;

typedef enum sample_vi_mode_e
{
	/* LVDS port */
    LVDS_PS1210K_LVDS_1080P_30FPS = 0,
    LVDS_PS1210K_LVDS_720P_30FPS,
    LVDS_PS3210M_LVDS_1080P_30FPS,
    LVDS_PS3210M_LVDS_720P_30FPS,
    LVDS_PS3210K_LVDS_1080P_30FPS,
    LVDS_PS3210K_LVDS_720P_30FPS,
    LVDS_PS4100K_LVDS_720P_30FPS,
    LVDS_PS7100K_LVDS_720P_30FPS,
    
	/* CAM0 port */
	CAM0_PS1210K_BAYER_1080P_30FPS,
    CAM0_PS1210K_BAYER_720P_30FPS,
    CAM0_PS1210M_BAYER_1080P_30FPS,
    CAM0_PS1210M_BAYER_720P_30FPS,
    CAM0_PS3210M_BAYER_1080P_30FPS,
    CAM0_PS3210M_BAYER_720P_30FPS,
    CAM0_PS3210P_BAYER_1080P_30FPS,
    CAM0_PS3210P_BAYER_720P_30FPS,
    CAM0_PS3210K_BAYER_1080P_30FPS,
    CAM0_PS3210K_BAYER_720P_30FPS,
    CAM0_PS4100K_BAYER_720P_30FPS,
    CAM0_PS7100K_BAYER_720P_30FPS,
    CAM0_PO3100K_BT1120_1080P_30FPS,
    CAM0_PO3100K_BT1120_720P_30FPS,
	CAM0_PC7030K_BT656_480I_30FPS,
    CAM0_PC7030K_BT656_576I_30FPS,
    
	/* CAM1 port */
	CAM1_PC7030K_BT656_480I_30FPS,
    CAM1_PC7030K_BT656_576I_30FPS,
    CAM1_PC3089N_BT601_480I_30FPS,
    CAM1_PC3089N_BT601_480P_30FPS,
	
	/* PVI port */
	PVI_ANALOG_SD_480I_30FPS,
	PVI_ANALOG_SD_576I_30FPS,
	PVI_ANALOG_PVI_720P_30FPS,
	PVI_ANALOG_PVI_1080P_30FPS,
	PVI_ANALOG_CVI_720P_30FPS,
	PVI_ANALOG_CVI_1080P_30FPS,
	PVI_ANALOG_HDA_720P_30FPS,
	PVI_ANALOG_HDA_1080P_30FPS,
	PVI_ANALOG_HDT_720P_30FPS,
	PVI_ANALOG_HDT_1080P_30FPS,

	VI_MODE_BUTT,
} SAMPLE_VI_MODE_E;

typedef enum 
{
    VI_DEV_MUX_LVDS = 0,
    VI_DEV_MUX_BAYER,
    VI_DEV_MUX_CAM0,
    VI_DEV_MUX_CAM1,
    VI_DEV_MUX_TVIN,
    VI_DEV_MUX_BUTT
} SAMPLE_VI_DEV_TYPE_E;

typedef enum sample_vi_chn_set_e
{
    VI_CHN_SET_NORMAL = 0,      /* mirror, flip close */
    VI_CHN_SET_MIRROR,          /* open MIRROR */
//  VI_CHN_SET_FLIP,            /* open filp */
//  VI_CHN_SET_FLIP_MIRROR      /* mirror, flip */
} SAMPLE_VI_CHN_SET_E;

typedef enum sample_vo_mode_e
{
    VO_MODE_1MUX = 0,
    VO_MODE_BUTT
} SAMPLE_VO_MODE_E;

typedef enum sample_rc_e
{
    SAMPLE_RC_CBR = 0,
    SAMPLE_RC_VBR,
    SAMPLE_RC_FIXQP
} SAMPLE_RC_E;

typedef enum sample_rgn_change_type_e
{
    RGN_CHANGE_TYPE_FGALPHA = 0,
    RGN_CHANGE_TYPE_BGALPHA,
    RGN_CHANGE_TYPE_LAYER
} SAMPLE_RGN_CHANGE_TYPE_EN;


/*******************************************************
    structure define 
*******************************************************/
typedef struct sample_vi_param_s
{
    BD_S32 s32ViDevCnt;         // VI Dev Total Count
    BD_S32 s32ViDevInterval;    // Vi Dev Interval
    BD_S32 s32ViChnCnt;         // Vi Chn Total Count
    BD_S32 s32ViChnInterval;    // VI Chn Interval
} SAMPLE_VI_PARAM_S;

typedef struct sample_video_loss_s
{
    BD_BOOL bStart;
    pthread_t Pid;
    SAMPLE_VI_MODE_E enViMode;
} SAMPLE_VIDEO_LOSS_S;

typedef struct sample_venc_getstream_s
{
     BD_BOOL bThreadStart;
     BD_S32  s32Cnt;
} SAMPLE_VENC_GETSTREAM_PARA_S;

typedef struct sample_vi_config_s
{
    SAMPLE_VI_MODE_E enViMode;
    VIDEO_NORM_E enNorm;           /*DC: VIDEO_ENCODING_MODE_AUTO */
    
    ROTATE_E enRotate;
    SAMPLE_VI_CHN_SET_E enViChnSet;    
	PIC_SIZE_E enPicSize;;
	FILE* dumpfp;
	BD_S32 s32FrameRate;	// Initial Frame Rate
	BD_U32 u32FrameDepth;	// Initial Buffer Depth
} SAMPLE_VI_CONFIG_S;

typedef BD_S32 (*CALLBACK_RECORD)(PAYLOAD_TYPE_E, BD_U16, BD_U16, VENC_STREAM_S *, BD_S32 s32Chn);
typedef struct sample_ispaddon_config_s
{
	ISP_MANR_ATTR_S stManrAttr;
	ISP_MANR_FILTER_S stManrFilter;
	
	ISP_DEFOCUS_ATTR_S stDefocusAttr;
	ISP_DEFOCUS_STATE_S stDefocusState;

	ISP_DEHAZE_ATTR_S stDehazeAttr;
	ISP_DEHAZE_DETECT_STATE_S stDehazeState;

	ISP_MOTION_DETECT_ATTR_S stMotionDetectAttr;
	ISP_MOTION_DETECT_MASK_S stMotionDetectMask;
	ISP_MOTION_DETECT_DATA_S stMotionDetectData;

	ISP_BLIND_DETECT_ATTR_S stBlindDetectAttr;
	ISP_BLIND_DETECT_STATE_S stBlindDetectState;
	ISP_BLIND_DETECT_MASK_S stBlindDetectMask;

	ISP_NIGHT_DETECT_ATTR_S stNightDetectAttr;
	ISP_NIGHT_DETECT_STATE_S stNightDetectState;

	ISP_DAY_DETECT_ATTR_S stDayDetectAttr;
	ISP_DAY_DETECT_STATE_S stDayDetectState;

} SAMPLE_ISPADDON_CONFIG_S;

/*******************************************************
    function announce  
*******************************************************/
BD_S32 SAMPLE_COMM_SYS_GetPicSize(VIDEO_NORM_E enNorm, PIC_SIZE_E enPicSize, SIZE_S *pstSize);
BD_S32 SAMPLE_COMM_SYS_GetEncSize(VIDEO_NORM_E enNorm, PIC_SIZE_E enPicSize, SIZE_S *pstSize);
BD_U32 SAMPLE_COMM_SYS_CalcPicVbBlkSize(VIDEO_NORM_E enNorm, PIC_SIZE_E enPicSize, PIXEL_FORMAT_E enPixFmt, BD_U32 u32AlignWidth);
BD_S32 SAMPLE_COMM_SYS_MemConfig(BD_VOID);
BD_VOID SAMPLE_COMM_SYS_Exit(void);
BD_S32 SAMPLE_COMM_SYS_Init(VB_CONF_S *pstVbConf);
BD_S32 SAMPLE_COMM_SYS_Payload2FilePostfix(PAYLOAD_TYPE_E enPayload, BD_CHAR* szFilePostfix);

BD_S32 SAMPLE_COMM_ISP_Init(SAMPLE_VI_MODE_E enViMode, SAMPLE_ISPADDON_CONFIG_S* pstISPAddonConfig);
BD_S32 SAMPLE_COMM_ISP_Stop(BD_VOID);
BD_S32 SAMPLE_COMM_ISP_Run(SAMPLE_VI_MODE_E enViMode, SAMPLE_ISPADDON_CONFIG_S* pstISPAddonConfig);

BD_S32 SAMPLE_COMM_ISP_RequestI2CAccess(BD_U32 u32Timeout);
BD_S32 SAMPLE_COMM_ISP_ReleaseI2CAccess(BD_VOID);
BD_S32 SAMPLE_COMM_ISP_ReadI2C(BD_U32 u32Chn, BD_U8 u8ChipAddr, BD_U8 u8RegAddr, BD_U8* u8Data, BD_U32 u32DataSize);
BD_S32 SAMPLE_COMM_ISP_WriteI2C(BD_U32 u32Chn, BD_U8 u8ChipAddr, BD_U8 u8RegAddr, BD_U8* u8Data, BD_U32 u32DataSize);

BD_U32 SAMPLE_COMM_ISP_WaitEvent(BD_U32 u32Timeout);


BD_S32 SAMPLE_COMM_ISP_SensorInit(SAMPLE_VI_MODE_E enViMode, VIDEO_NORM_E enNorm);
BD_S32 SAMPLE_COMM_ISP_SensorExit(SAMPLE_VI_MODE_E enViMode);
BD_S32 SAMPLE_COMM_ISP_SaveRegFile(SAMPLE_CFG_OPT_E enCfgOpt, char *pcIspCfgFile);
BD_S32 SAMPLE_COMM_ISP_FillConfig(SAMPLE_ISPADDON_CONFIG_S *pstISPAddonConfig, SAMPLE_VI_MODE_E enViMode, BD_BOOL bEnableManr, BD_BOOL bEnableDefocus, BD_BOOL bEnableDehaze, BD_BOOL bEnableMotion, BD_BOOL bEnableBlind, BD_BOOL bEnableNight, BD_BOOL bEnableDay);


BD_VOID* SAMPLE_COMM_ISP_ThreadFunc(BD_VOID* p);
BD_S32 SAMPLE_COMM_ISP_CreateThread(BD_VOID* ctx);
BD_S32 SAMPLE_COMM_ISP_ExitThread(BD_VOID);
#if 0
BD_S32 SAMPLE_COMM_ISP_StartManr(SAMPLE_ISPADDON_CONFIG_S* pstISPAddonConfig);
BD_S32 SAMPLE_COMM_ISP_StartDehaze(SAMPLE_ISPADDON_CONFIG_S* pstISPAddonConfig);
#endif
BD_S32 SAMPLE_COMM_ISP_StartDefocus(SAMPLE_ISPADDON_CONFIG_S* pstISPAddonConfig);
BD_S32 SAMPLE_COMM_ISP_StartMotion(SAMPLE_ISPADDON_CONFIG_S* pstISPAddonConfig, BD_U32* pDetectMask);
BD_S32 SAMPLE_COMM_ISP_StartBlind(SAMPLE_ISPADDON_CONFIG_S* pstISPAddonConfig, BD_U32* pDetectMask);
BD_S32 SAMPLE_COMM_ISP_StartNight(SAMPLE_ISPADDON_CONFIG_S* pstISPAddonConfig);
BD_S32 SAMPLE_COMM_ISP_StartDay(SAMPLE_ISPADDON_CONFIG_S* pstISPAddonConfig);
BD_S32 SAMPLE_COMM_ISP_GetSensorID(BD_U8 Addr);
BD_U16 SAMPLE_COMM_ISP_GetISPID(BD_U8 Addr);


VI_DEV_INPUT_E SAMPLE_COMM_VI_GetDevType(SAMPLE_VI_MODE_E enViMode);
BD_S32 SAMPLE_COMM_VI_Init(SAMPLE_VI_MODE_E enViMode0, SAMPLE_VI_MODE_E enViMode1, SAMPLE_VI_MODE_E enViMode2);
BD_VOID SAMPLE_COMM_VI_Exit(BD_VOID);
BD_S32 SAMPLE_COMM_VI_Mode2Param(SAMPLE_VI_MODE_E enViMode, SAMPLE_VI_PARAM_S *pstViParam);
VI_DEV SAMPLE_COMM_VI_GetDev(VI_CHN ViChn);
BD_S32 SAMPLE_COMM_VI_StartDev(VI_DEV ViDev, SAMPLE_VI_MODE_E enViMode);
BD_S32 SAMPLE_COMM_VI_StartChn(VI_CHN ViChn, SIZE_S *pstTarSize, SAMPLE_VI_CONFIG_S* pstViConfig);
BD_S32 SAMPLE_COMM_VI_StartADC(VI_CHN ViChn, SAMPLE_VI_CONFIG_S* pstViConfig);
BD_S32 SAMPLE_COMM_VI_StartNoIsp(VI_CHN ViChn, SAMPLE_VI_CONFIG_S* pstViConfig);
BD_S32 SAMPLE_COMM_VI_StartIsp(VI_CHN ViChn, SAMPLE_VI_CONFIG_S* pstViConfig, SAMPLE_ISPADDON_CONFIG_S* pstISPAddonConfig);
BD_S32 SAMPLE_COMM_VI_StopADC(VI_CHN ViChn, SAMPLE_VI_CONFIG_S* pstViConfig);
BD_S32 SAMPLE_COMM_VI_StopNoIsp(VI_CHN ViChn, SAMPLE_VI_CONFIG_S* pstViConfig);
BD_S32 SAMPLE_COMM_VI_StopIsp(VI_CHN ViChn, SAMPLE_VI_CONFIG_S* pstViConfig);
BD_S32 SAMPLE_COMM_VI_BindVpss(SAMPLE_VI_MODE_E enViMode);
BD_S32 SAMPLE_COMM_VI_UnBindVpss(SAMPLE_VI_MODE_E enViMode);
BD_S32 SAMPLE_COMM_VI_BindVenc(SAMPLE_VI_MODE_E enViMode);
BD_S32 SAMPLE_COMM_VI_UnBindVenc(VI_CHN ViChn, VENC_GRP VencGrp);
BD_S32 SAMPLE_COMM_VI_MemConfig(SAMPLE_VI_MODE_E enViMode);
BD_S32 SAMPLE_COMM_VI_GetVFrameFromYUV(VI_CHN ViChn, SAMPLE_VI_CONFIG_S* pstViConfig, VIDEO_FRAME_INFO_S *pstVFrameInfo);
BD_CHAR* SAMPLE_COMM_VI_GetModeString(SAMPLE_VI_MODE_E enViMode);
BD_S32 SAMPLE_COMM_VI_ChangeCapSize(VI_CHN ViChn, BD_U32 u32CapWidth, BD_U32 u32CapHeight,BD_U32 u32Width, BD_U32 u32Height);
BD_S32 SAMPLE_COMM_VI_StartVi(VI_CHN ViChn, SAMPLE_VI_CONFIG_S* pstViConfig, SAMPLE_ISPADDON_CONFIG_S* pstISPAddonConfig);
BD_S32 SAMPLE_COMM_VI_StopVi(VI_CHN ViChn, SAMPLE_VI_CONFIG_S* pstViConfig);
BD_S32 SAMPLE_COMM_VI_InitDump(const BD_CHAR* FuncName, VI_CHN ViChn, SAMPLE_VI_CONFIG_S* pstViConfig, BD_U32 args);
BD_S32 SAMPLE_COMM_VI_ExitDump(SAMPLE_VI_CONFIG_S* pstViConfig);
BD_S32 SAMPLE_COMM_VI_WriteDump(FILE* fp, const BD_VOID* ptr, BD_U32 u32Size);
BD_S32 SAMPLE_COMM_VI_SetFrameRate(VI_CHN ViChn, BD_S32 s32SrcFrameRate, BD_S32 s32FrameRate);
BD_S32 SAMPLE_COMM_VI_FillConfig(SAMPLE_VI_CONFIG_S *pstViConfig, SAMPLE_VI_MODE_E enViMode, PIC_SIZE_E enPicSize, VIDEO_NORM_E enNorm, BD_S32 s32FrameRate, BD_U32 u32FrameDepth);
BD_BOOL SAMPLE_COMM_VI_IsSensorInput(SAMPLE_VI_MODE_E enViMode);
BD_BOOL SAMPLE_COMM_VI_IsISPInput(SAMPLE_VI_MODE_E enViMode);

BD_S32 SAMPLE_COMM_VENC_MemConfig(BD_VOID);
BD_S32 SAMPLE_COMM_VENC_Start(VENC_GRP VencGrp, VENC_CHN VencChn, PAYLOAD_TYPE_E enType, VIDEO_NORM_E enNorm, PIC_SIZE_E enSize, SAMPLE_RC_E enRcMode);
BD_S32 SAMPLE_COMM_VENC_Stop(VENC_CHN VencChn);
BD_S32 SAMPLE_COMM_VENC_StartGetStream(BD_S32 s32Cnt);
BD_S32 SAMPLE_COMM_VENC_StartGetStream_shr_mem(BD_S32 s32Cnt, void *arg);
BD_S32 SAMPLE_COMM_VENC_StopGetStream();
BD_S32 SAMPLE_COMM_VENC_BindVi(VENC_GRP GrpChn,VI_DEV ViDev,VI_CHN ViChn);
BD_S32 SAMPLE_COMM_VENC_UnBindVi(VENC_GRP GrpChn,VI_DEV ViDev,VI_CHN ViChn);
BD_S32 SAMPLE_COMM_VENC_BindVo(VENC_GRP GrpChn);
BD_S32 SAMPLE_COMM_VENC_UnBindVo(VENC_GRP GrpChn);
BD_VOID SAMPLE_COMM_VENC_SetCallbackREC(CALLBACK_RECORD func);

//BD_S32 SAMPLE_COMM_VDA_MdStart(VDA_CHN VdaChn, VI_CHN ViChn, SIZE_S *pstSize);
//BD_S32 SAMPLE_COMM_VDA_MdStart(VDA_CHN VdaChn, VPSS_GRP VpssGrp, VPSS_CHN VpssChn, SIZE_S *pstSize);
//BD_S32 SAMPLE_COMM_VDA_OdStart(VDA_CHN VdaChn, VI_CHN ViChn, SIZE_S *pstSize);
//BD_VOID SAMPLE_COMM_VDA_MdStop(VDA_CHN VdaChn, VI_CHN ViChn);
//BD_VOID SAMPLE_COMM_VDA_OdStop(VDA_CHN VdaChn, VI_CHN ViChn);

BD_S32 SAMPLE_COMM_AUDIO_CreatTrdAiAo(AUDIO_DEV AiDev, AI_CHN AiChn, AUDIO_DEV AoDev, AO_CHN AoChn);
BD_S32 SAMPLE_COMM_AUDIO_CreatTrdAiRec(AUDIO_DEV AiDev, AI_CHN AiChn);
BD_S32 SAMPLE_COMM_AUDIO_CreatTrdAiAenc(AUDIO_DEV AiDev, AI_CHN AiChn, AENC_CHN AeChn, BD_BOOL bSendAo);
BD_S32 SAMPLE_COMM_AUDIO_CreatTrdAencAdec(AENC_CHN AeChn, ADEC_CHN AdChn, FILE *pAecFd);
BD_S32 SAMPLE_COMM_AUDIO_CreatTrdAencSave(AENC_CHN AeChn, FILE *pAecFd);
BD_S32 SAMPLE_COMM_AUDIO_DestoryTrdAencSave(AENC_CHN AeChn);
//BD_S32 SAMPLE_COMM_AUDIO_CreatTrdFileAdec(ADEC_CHN AdChn, FILE *pAdcFd);
BD_S32 SAMPLE_COMM_AUDIO_DestoryTrdAi(AUDIO_DEV AiDev, AI_CHN AiChn);
BD_S32 SAMPLE_COMM_AUDIO_DestoryTrdAencAdec(AENC_CHN AeChn);
BD_S32 SAMPLE_COMM_AUDIO_CreatTrdRecAo(AUDIO_DEV AoDev, AO_CHN AoChn, FILE *pfdin, AUDIO_FRAME_S *pstFrame);
BD_S32 SAMPLE_COMM_AUDIO_DestoryTrdRecAo(AUDIO_DEV AoDev, AO_CHN AoChn);

//BD_S32 SAMPLE_COMM_AUDIO_DestoryTrdFileAdec(ADEC_CHN AdChn);
BD_S32 SAMPLE_COMM_AUDIO_AoBindAdec(AUDIO_DEV AoDev, AO_CHN AoChn, ADEC_CHN AdChn);
BD_S32 SAMPLE_COMM_AUDIO_AoUnbindAdec(AUDIO_DEV AoDev, AO_CHN AoChn, ADEC_CHN AdChn);
BD_S32 SAMPLE_COMM_AUDIO_AoBindAi(AUDIO_DEV AiDev, AI_CHN AiChn, AUDIO_DEV AoDev, AO_CHN AoChn);
BD_S32 SAMPLE_COMM_AUDIO_AoUnbindAi(AUDIO_DEV AiDev, AI_CHN AiChn, AUDIO_DEV AoDev, AO_CHN AoChn);
BD_S32 SAMPLE_COMM_AUDIO_AencBindAi(AUDIO_DEV AiDev, AI_CHN AiChn, AENC_CHN AeChn);
BD_S32 SAMPLE_COMM_AUDIO_AencUnbindAi(AUDIO_DEV AiDev, AI_CHN AiChn, AENC_CHN AeChn);
//BD_S32 SAMPLE_COMM_AUDIO_CfgAcodec(AIO_ATTR_S *pstAioAttr, BD_BOOL bMacIn);
//BD_S32 SAMPLE_COMM_AUDIO_DisableAcodec();
BD_S32 SAMPLE_COMM_AUDIO_StartAi(AUDIO_DEV AiDevId, BD_S32 s32AiChnCnt, AIO_ATTR_S *pstAioAttr);
BD_S32 SAMPLE_COMM_AUDIO_StopAi(AUDIO_DEV AiDevId, BD_S32 s32AiChnCnt);
BD_S32 SAMPLE_COMM_AUDIO_StartAo(AUDIO_DEV AoDevId, BD_S32 s32AoChnCnt, AIO_ATTR_S *pstAioAttr);
BD_S32 SAMPLE_COMM_AUDIO_StopAo(AUDIO_DEV AoDevId, BD_S32 s32AoChnCnt);
BD_S32 SAMPLE_COMM_AUDIO_StartAenc(BD_S32 s32AencChnCnt, AENC_CHN_ATTR_S *pstAencAttr);
BD_S32 SAMPLE_COMM_AUDIO_StopAenc(BD_S32 s32AencChnCnt);
BD_S32 SAMPLE_COMM_AUDIO_CreatTrdAencFile(AENC_CHN AeChn, FILE *pFdIn, FILE *pFdOut, AUDIO_FRAME_S *pstFrm);
BD_S32 SAMPLE_COMM_AUDIO_DestoryTrdAencFile(AENC_CHN AeChn);

BD_S32 SAMPLE_COMM_AUDIO_StartAdec(ADEC_CHN AdChn, ADEC_CHN_ATTR_S *pstAdecAttr);
BD_S32 SAMPLE_COMM_AUDIO_StopAdec(ADEC_CHN AdChn);
BD_S32 SAMPLE_COMM_AUDIO_CreatTrdAdecFile(ADEC_CHN AdChn, FILE *pfdin, FILE *pfdout, AUDIO_STREAM_S *pstStream);
BD_S32 SAMPLE_COMM_AUDIO_DestoryTrdAdecFile(ADEC_CHN AdChn);



BD_S32 SAMPLE_COMM_VO_StartVO(BD_S32 s32Scene, VO_DEVICE_TYPE_E enODev, 
	STREAM_TYPE_E enPMSStreamType, PIC_SIZE_E enPMSSize, 
	STREAM_TYPE_E enPPSStreamType, PIC_SIZE_E enPPSSize);
BD_S32 SAMPLE_COMM_VO_StopVO(BD_VOID);
BD_S32 SAMPLE_COMM_FB_StartFb(BD_VOID);
BD_VOID SAMPLE_COMM_FB_StopFb(BD_VOID);

/** rgn definition */
BD_S32 SAMPLE_COMM_RGN_CoverCreate(BD_S32 Handle, BD_S32 s32SrcX, BD_S32 s32SrcY, BD_U32 u32SrcWidth, BD_U32 u32SrcHeight);
BD_S32 SAMPLE_COMM_RGN_CoverAttach(VI_CHN ViChn, BD_S32 Handle, BD_S32 s32DstX, BD_S32 s32DstY);
BD_S32 SAMPLE_COMM_RGN_CoverDetach(VI_CHN ViChn, BD_S32 Handle);
BD_S32 SAMPLE_COMM_RGN_CoverExit(BD_S32 Handle);
BD_VOID SAMPLE_COMM_RGN_OverlayCreate(BD_S32 Handle, RGN_TYPE_E enType,	BD_S32 s32SrcX, BD_S32 s32SrcY, BD_U32 u32SrcWidth, BD_U32 u32SrcHeight);
BD_VOID SAMPLE_COMM_RGN_OverlayAttach(VI_CHN ViChn, BD_S32 Handle, BD_S32 s32DstX, BD_S32 s32DstY);
BD_VOID SAMPLE_COMM_RGN_OverlayDetach(VI_CHN ViChn, BD_S32 Handle);
BD_VOID SAMPLE_COMM_RGN_OverlayExit(BD_S32 Handle);
BD_U8* SAMPLE_COMM_RGN_OverlayGetBuffer(BD_S32 Handle);
BD_VOID SAMPLE_COMM_RGN_OverlayUpdate(BD_S32 Handle);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif /* End of #ifndef __SAMPLE_COMMON_H__ */
