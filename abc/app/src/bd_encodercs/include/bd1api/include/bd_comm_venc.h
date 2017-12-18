
/// File : bd_comm_venc.h
/// Date : 2015/03

#ifndef __BD_COMM_VENC_H__
#define __BD_COMM_VENC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "bd_type.h"
#include "bd_common.h"
#include "bd_errno.h"
#include "bd_comm_video.h"
#include "bd_comm_rc.h"

/* invlalid device ID */
#define BD_ERR_VENC_INVALID_DEVID     BD_DEF_ERR(BD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_DEVID)
/* invlalid channel ID */
#define BD_ERR_VENC_INVALID_CHNID     BD_DEF_ERR(BD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID)
/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define BD_ERR_VENC_ILLEGAL_PARAM     BD_DEF_ERR(BD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
/* channel exists */
#define BD_ERR_VENC_EXIST             BD_DEF_ERR(BD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_EXIST)
/* channel exists */
#define BD_ERR_VENC_UNEXIST           BD_DEF_ERR(BD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST)
/* using a NULL point */
#define BD_ERR_VENC_NULL_PTR          BD_DEF_ERR(BD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
/* try to enable or initialize system,device or channel, before configing attribute */
#define BD_ERR_VENC_NOT_CONFIG        BD_DEF_ERR(BD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_CONFIG)
/* operation is not supported by NOW */
#define BD_ERR_VENC_NOT_SUPPORT       BD_DEF_ERR(BD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
/* operation is not permitted ,eg, try to change stati attribute */
#define BD_ERR_VENC_NOT_PERM          BD_DEF_ERR(BD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)
/* failure caused by malloc memory */
#define BD_ERR_VENC_NOMEM             BD_DEF_ERR(BD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
/* failure caused by malloc buffer */
#define BD_ERR_VENC_NOBUF             BD_DEF_ERR(BD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF)
/* no data in buffer */
#define BD_ERR_VENC_BUF_EMPTY         BD_DEF_ERR(BD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY)
/* no buffer for new data */
#define BD_ERR_VENC_BUF_FULL          BD_DEF_ERR(BD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL)
/* system is not ready,had not initialed or loaded*/
#define BD_ERR_VENC_SYS_NOTREADY      BD_DEF_ERR(BD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)
/* system is busy*/
#define BD_ERR_VENC_BUSY              BD_DEF_ERR(BD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)

typedef struct bdVENC_PACK_S
{
	BD_U32   u32PhyAddr;			/*the physics address of stream*/
	BD_U8   *pu8Addr;				/*the virtual address of stream*/
	BD_U32   u32Len;				/*the length of stream*/
	BD_U64   u64PTS;				/*PTS*/
} VENC_PACK_S;

typedef enum bdH264E_REF_TYPE_E
{
	IDRSLICE = 0,
	PSLICE,
	REF_TYPE_BUTT
} H264E_REF_TYPE_E;

typedef struct bdVENC_STREAM_INFO_H264_S
{    
	BD_U32 u32PicBytesNum;			/* the coded picture stream byte number */
	BD_U32 u32PSkipMbNum;			/* the skip macroblock num */
	BD_U32 u32IpcmMbNum;			/* the ipcm macroblock num */
	H264E_REF_TYPE_E enRefType;
	BD_U16 *pu16mv;					/* motion vector area */
	BD_U16 u16MVH;					/* h-size of motion vector */
	BD_U16 u16MVV;					/* v-size of motion vector */
} VENC_STREAM_INFO_H264_S;

typedef struct bdVENC_STREAM_INFO_JPEG_S
{
	BD_U32 u32PicBytesNum;         /* the coded picture stream byte number */
	BD_U32 u32UpdateAttrCnt;
} VENC_STREAM_INFO_JPEG_S;

typedef struct bdVENC_STREAM_S
{
	VENC_PACK_S *pstPack;       /*stream pack attribute*/
	BD_U32      u32PackCount;   /*the pack number of one frame stream*/
	BD_U32      u32Seq;         /*the list number of stream*/
	union
	{
		VENC_STREAM_INFO_H264_S stH264Info;   /*the stream info of h264*/
		VENC_STREAM_INFO_JPEG_S stJpegInfo;   /*the stream info of jpeg*/
	};
} VENC_STREAM_S;

typedef struct bdVENC_ATTR_H264_S
{
	BD_U32  u32EncWidth;		/*width  of a picture to be encoded, in pixel*/
	BD_U32  u32EncHeight;		/*height of a picture to be encoded, in pixel*/    
	BD_U32  u32BufWidth;		/*maximum width  of a picture to be captured, in pixel*/
	BD_U32  u32BufHeight;		/*maximum height of a picture to be captured, in pixel*/

	BD_U32  u32BufSize;			/*stream buffer size*/
	BD_U32  u32MaxFrmSize;		/*maximum stream buffer size per a frame*/
	BD_U32	u32SlotsToUse;		/*slot count to use default 2 [2,8]*/
	BD_U32	u32PicNodes;		/*picture nodes [2,4]*/
	BD_U32  u32Profile;			/*0: baseline; 1:MP; 2:HP [0,2]*/          
	BD_U32  u32UseMotion;		/*Using motion info or not*/
} VENC_ATTR_H264_S;

typedef struct bdVENC_ATTR_MJPEG_S
{
	BD_U32  u32EncWidth;		/*width  of a picture to be encoded, in pixel*/
	BD_U32  u32EncHeight;		/*height of a picture to be encoded, in pixel*/    
	BD_U32  u32BufWidth;		/*maximum width  of a picture to be captured, in pixel*/
	BD_U32  u32BufHeight;		/*maximum height of a picture to be captured, in pixel*/

	BD_U32  u32BufSize;			/*stream buffer size*/
} VENC_ATTR_MJPEG_S;

typedef struct bdVENC_ATTR_JPEG_S 
{
	BD_U32  u32EncWidth;		/*width  of a picture to be encoded, in pixel*/
	BD_U32  u32EncHeight;		/*height of a picture to be encoded, in pixel*/    
	BD_U32  u32BufWidth;		/*maximum width  of a picture to be captured, in pixel*/
	BD_U32  u32BufHeight;		/*maximum height of a picture to be captured, in pixel*/

	BD_U32  u32BufSize;			/*stream buffer size*/
	BD_U32  u32Qfactor;			/*Qfactor [1,99]*/
} VENC_ATTR_JPEG_S;

typedef struct bdVENC_ATTR_S
{
	PAYLOAD_TYPE_E  enType;      /*the type of payload*/
	union
	{
		VENC_ATTR_H264_S  stAttrH264e;     /*attributes of h264*/
		VENC_ATTR_MJPEG_S stAttrMjpeg;     /*attributes of mjpeg*/
		VENC_ATTR_JPEG_S  stAttrJpeg;      /*attributes of jpeg*/
	};    
} VENC_ATTR_S;

typedef struct bdVENC_CHN_ATTR_S
{
	VENC_ATTR_S stVeAttr;        /*the attribute of video encoder*/
	VENC_RC_ATTR_S stRcAttr;     /*the attribute of rate  ctrl*/
} VENC_CHN_ATTR_S;

typedef struct bdVENC_CHN_STAT_S
{
	BD_BOOL bRegistered;         /*registered ?*/
	BD_U32 u32LeftStreamBytes;   /*left stream bytes*/
	BD_U32 u32LeftStreamFrames;  /*left stream frames*/
	BD_U32 u32CurPacks;          /*pack number of current frame*/
} VENC_CHN_STAT_S;


typedef struct bdCROP_INFO_S
{
	BD_BOOL bCrop;
	BD_U16  u16OffsetTop;      /* top offset of show area */
	BD_U16  u16OffsetBottom;   /* bottom offset of show area */
	BD_U16  u16OffsetLeft;     /* left offset of show area */
	BD_U16  u16OffsetRight;    /* right offset of show area */
	BD_U32  u32AlignHeight;
	BD_U32  u32AlignWidth;
	BD_U32  u32PicHeight;
	BD_U32  u32PicWidth;
} CROP_INFO_S;

/* ROI struct */
typedef struct bdVENC_ROI_CFG_S
{
	BD_U32  u32Index;          /* Index of an ROI */
	BD_BOOL bEnable;           /* Whether to enable this ROI */
	BD_BOOL bAbsQp;            /* QP mode of an ROI. BD_FALSE: relative QP, BD_TURE: absolute QP.*/
	BD_S32  s32Qp;             /* QP value. */
	RECT_S  stRect;            /* Region of an ROI*/
} VENC_ROI_CFG_S;

typedef struct bdGROUP_CROP_CFG_S
{
	BD_BOOL bEnable;           /* Crop region enable */
	RECT_S  stRect;            /* Crop region, note: s32X must be multi of 16 */
} GROUP_CROP_CFG_S;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_COMM_VENC_H__ */
