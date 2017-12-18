
/// File : bd_common.h
/// Date : 2015/03


#ifndef __BD_COMMON_H__
#define __BD_COMMON_H__

#include "bd_type.h"
#include "bd_math.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct bdPOINT_S
{
	BD_S32 s32X;
	BD_S32 s32Y;
} POINT_S;

typedef struct bdSIZE_S
{
	BD_U32 u32Width;
	BD_U32 u32Height;
} SIZE_S;

typedef struct bdRECT_S
{
	BD_S32 s32X;
	BD_S32 s32Y;
	BD_U32 u32Width;
	BD_U32 u32Height;
} RECT_S;

typedef enum bdROTATE_E
{
	ROTATE_NONE = 0,
	ROTATE_90   = 1,
	ROTATE_180  = 2,
	ROTATE_270  = 3,
	ROTATE_BUTT
} ROTATE_E;

typedef BD_S32 AI_CHN;
typedef BD_S32 AO_CHN;
typedef BD_S32 AENC_CHN;
typedef BD_S32 ADEC_CHN;
typedef BD_S32 VENC_CHN;
typedef BD_S32 VENC_GRP;
typedef BD_S32 VDEC_CHN;
typedef BD_U32 VI_DEV;
typedef BD_U32 VI_CHN;
typedef BD_U32 VI_WAY;
typedef BD_U32 VO_DEV;
typedef BD_U32 AUDIO_DEV;
typedef BD_U32 IVE_HANDLE;
	
#define BD_INVALID_CHN (-1)
#define BD_INVALID_WAY (-1)
#define BD_INVALID_LAYER (-1)
#define BD_INVALID_DEV (-1)
#define BD_INVALID_HANDLE (-1)

#define BD_INVALID_VALUE (-1)
#define BD_INVALID_TYPE (-1)

typedef enum bdMOD_ID_E
{
	BD_ID_CMPI    = 0,
	BD_ID_VB      = 1,
	BD_ID_SYS     = 2,
	BD_ID_VALG    = 3,

	BD_ID_VDEC    = 4,
	BD_ID_CHNL    = 5,
	BD_ID_GROUP   = 6,
	BD_ID_VENC    = 7,
	BD_ID_VPSS    = 8,
	BD_ID_VDA     = 9,

	BD_ID_H264E   = 10,
	BD_ID_JPEGE   = 11,
	BD_ID_MPEG4E  = 12,
	BD_ID_H264D   = 13,
	BD_ID_JPEGD   = 14,
	BD_ID_VOU     = 15,

	BD_ID_VIU     = 16,
	BD_ID_DSU     = 17,
	BD_ID_RGN	  = 18,
	BD_ID_RC      = 19,

	BD_ID_SIO     = 20,
	BD_ID_AI      = 21,
	BD_ID_AO      = 22,
	BD_ID_AENC    = 23,
	BD_ID_ADEC    = 24,

	BD_ID_AVENC   = 25,

	BD_ID_PCIV    = 26,
	BD_ID_PCIVFMW = 27,

	BD_ID_ISP	  = 28,

	BD_ID_IVE	  = 29,
	/* there is a hole */    

	BD_ID_DCCM    = 31,
	BD_ID_DCCS    = 32,

	BD_ID_PROC    = 33,
	BD_ID_LOG     = 34,
	BD_ID_MST_LOG = 35,
	BD_ID_VD      = 36,

	BD_ID_VCMP    = 38,
	BD_ID_FB      = 39,


	BD_ID_HDMI    = 40,
	BD_ID_VOIE    = 41,
	BD_ID_TDE     = 42,

	BD_ID_USR     = 43,			/* for capture dump */

	BD_ID_VEDU    = 44,
	BD_ID_VENC_GRP = 45,		/// 0x2d
	BD_ID_VDEC_GRP = 46,
	
	BD_ID_M2MU = 47,

	BD_ID_MIU	= 48,

	BD_ID_BUTT,
} MOD_ID_E;

typedef struct bdMPP_CHN_S
{
	MOD_ID_E    enModId;
	BD_S32      s32DevId;
	BD_S32      s32ChnId;
} MPP_CHN_S;

/* We just coyp this value of payload type from RTP/RTSP definition */
typedef enum
{
	PT_PCMU			= 0,
	PT_1016			= 1,
	PT_G721			= 2,
	PT_GSM			= 3,
	PT_G723			= 4,
	PT_DVI4_8K		= 5,
	PT_DVI4_16K		= 6,
	PT_LPC			= 7,
	PT_PCMA			= 8,
	PT_G722			= 9,
	PT_S16BE_STEREO	= 10,
	PT_S16BE_MONO	= 11,
	PT_QCELP		= 12,
	PT_CN			= 13,
	PT_MPEGAUDIO	= 14,
	PT_G728			= 15,
	PT_DVI4_3		= 16,
	PT_DVI4_4		= 17,
	PT_G729			= 18,
	PT_G711A		= 19,
	PT_G711U		= 20,
	PT_G726			= 21,
	PT_G729A		= 22,
	PT_LPCM			= 23,
	PT_CelB			= 25,
	PT_JPEG			= 26,
	PT_CUSM			= 27,
	PT_NV			= 28,
	PT_PICW			= 29,
	PT_CPV			= 30,
	PT_H261			= 31,
	PT_MPEGVIDEO	= 32,
	PT_MPEG2TS		= 33,
	PT_H263			= 34,
	PT_SPEG			= 35,
	PT_MPEG2VIDEO	= 36,
	PT_AAC			= 37,
	PT_WMA9STD		= 38,
	PT_HEAAC		= 39,
	PT_PCM_VOICE	= 40,
	PT_PCM_AUDIO	= 41,
	PT_AACLC		= 42,
	PT_MP3			= 43,
	PT_ADPCMA		= 49,
	PT_AEC			= 50,
	PT_X_LD			= 95,
	PT_H264			= 96,
	PT_D_GSM_HR		= 200,
	PT_D_GSM_EFR	= 201,
	PT_D_L8			= 202,
	PT_D_RED		= 203,
	PT_D_VDVI		= 204,
	PT_D_BT656		= 220,
	PT_D_H263_1998	= 221,
	PT_D_MP1S		= 222,
	PT_D_MP2P		= 223,
	PT_D_BMPEG		= 224,
	PT_MP4VIDEO		= 230,
	PT_MP4AUDIO		= 237,
	PT_VC1			= 238,
	PT_JVC_ASF		= 255,
	PT_D_AVI		= 256,
	PT_DIVX3		= 257,
	PT_AVS			= 258,
	PT_REAL8		= 259,
	PT_REAL9		= 260,
	PT_VP6			= 261,
	PT_VP6F			= 262,
	PT_VP6A			= 263,
	PT_SORENSON		= 264,
	PT_MAX			= 265,
	/* add by hisilicon */
	PT_AMR			= 1001, 
	PT_MJPEG		= 1002,
	PT_AMRWB		= 1003,
	PT_BUTT
} PAYLOAD_TYPE_E;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /* _BD_COMMON_H_ */

