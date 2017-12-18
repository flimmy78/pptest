#ifndef __BD_COMM_VI_H__
#define __BD_COMM_VI_H__

#include "bd_common.h"
#include "bd_errno.h"
#include "bd_comm_video.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

#define VI_INVALID_FRMRATE  (-1UL)
#define VIU_MAX_USER_FRAME_DEPTH 8

typedef enum bdEN_VIU_ERR_CODE_E
{
    ERR_VI_FAILED_NOTENABLE = 64,   /* device or channel not enable*/
    ERR_VI_FAILED_NOTDISABLE,       /* device not disable*/
    ERR_VI_FAILED_CHNOTDISABLE,     /* channel not disable*/
    ERR_VI_CFG_TIMEOUT,             /* config timeout*/
    ERR_VI_NORM_UNMATCH,            /* video norm of ADC and VIU is unmatch*/
    ERR_VI_INVALID_WAYID,           /* invlalid way ID     */
    ERR_VI_INVALID_PHYCHNID,        /* invalid phychn id*/
    ERR_VI_FAILED_NOTBIND,          /* device or channel not bind */
    ERR_VI_FAILED_BINDED,           /* device or channel not unbind */
	ERR_VI_FAILED_TIMEOUT,			/* init or select fail */
	ERR_VI_FAILED_INTERNAL,			/* msg error */
} EN_VIU_ERR_CODE_E;

#define BD_ERR_VI_INVALID_PARA          BD_DEF_ERR(BD_ID_VIU, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
#define BD_ERR_VI_INVALID_DEVID         BD_DEF_ERR(BD_ID_VIU, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_DEVID)
#define BD_ERR_VI_INVALID_CHNID         BD_DEF_ERR(BD_ID_VIU, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID)
#define BD_ERR_VI_INVALID_NULL_PTR      BD_DEF_ERR(BD_ID_VIU, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
#define BD_ERR_VI_FAILED_NOTCONFIG      BD_DEF_ERR(BD_ID_VIU, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_CONFIG)
#define BD_ERR_VI_SYS_NOTREADY          BD_DEF_ERR(BD_ID_VIU, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)
#define BD_ERR_VI_BUF_EMPTY             BD_DEF_ERR(BD_ID_VIU, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY)
#define BD_ERR_VI_BUF_FULL              BD_DEF_ERR(BD_ID_VIU, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL)
#define BD_ERR_VI_NOMEM                 BD_DEF_ERR(BD_ID_VIU, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
#define BD_ERR_VI_NOT_SUPPORT           BD_DEF_ERR(BD_ID_VIU, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
#define BD_ERR_VI_BUSY                  BD_DEF_ERR(BD_ID_VIU, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)
#define BD_ERR_VI_NOT_PERM              BD_DEF_ERR(BD_ID_VIU, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)

#define BD_ERR_VI_FAILED_NOTENABLE      BD_DEF_ERR(BD_ID_VIU, EN_ERR_LEVEL_ERROR, ERR_VI_FAILED_NOTENABLE)
#define BD_ERR_VI_FAILED_NOTDISABLE     BD_DEF_ERR(BD_ID_VIU, EN_ERR_LEVEL_ERROR, ERR_VI_FAILED_NOTDISABLE)
#define BD_ERR_VI_FAILED_CHNOTDISABLE   BD_DEF_ERR(BD_ID_VIU, EN_ERR_LEVEL_ERROR, ERR_VI_FAILED_CHNOTDISABLE)
#define BD_ERR_VI_CFG_TIMEOUT           BD_DEF_ERR(BD_ID_VIU, EN_ERR_LEVEL_ERROR, ERR_VI_CFG_TIMEOUT)
#define BD_ERR_VI_NORM_UNMATCH          BD_DEF_ERR(BD_ID_VIU, EN_ERR_LEVEL_ERROR, ERR_VI_NORM_UNMATCH)
#define BD_ERR_VI_INVALID_WAYID         BD_DEF_ERR(BD_ID_VIU, EN_ERR_LEVEL_ERROR, ERR_VI_INVALID_WAYID)
#define BD_ERR_VI_INVALID_PHYCHNID      BD_DEF_ERR(BD_ID_VIU, EN_ERR_LEVEL_ERROR, ERR_VI_INVALID_PHYCHNID)
#define BD_ERR_VI_FAILED_NOTBIND        BD_DEF_ERR(BD_ID_VIU, EN_ERR_LEVEL_ERROR, ERR_VI_FAILED_NOTBIND)
#define BD_ERR_VI_FAILED_BINDED         BD_DEF_ERR(BD_ID_VIU, EN_ERR_LEVEL_ERROR, ERR_VI_FAILED_BINDED)
#define BD_ERR_VI_FAILED_TIMEOUT        BD_DEF_ERR(BD_ID_VIU, EN_ERR_LEVEL_ERROR, ERR_VI_FAILED_TIMEOUT)
#define BD_ERR_VI_FAILED_INTERNAL       BD_DEF_ERR(BD_ID_VIU, EN_ERR_LEVEL_ERROR, ERR_VI_FAILED_INTERNAL)


typedef enum bdVI_DEV_INPUT_E
{
	VI_DEV_INPUT_LVDS = 0,
	VI_DEV_INPUT_BAYER,
	VI_DEV_INPUT_CAM0,	
	VI_DEV_INPUT_CAM1,	
	VI_DEV_INPUT_PVI,	
    VI_DEV_INPUT_BUTT
} VI_DEV_INPUT_E;

typedef enum bdVI_CAM_INTF_E
{
	/* BD1 LVDS Interface */
	VI_INTF_LVDS_1080P = 0,
	VI_INTF_LVDS_720P,
	
	/* BD1 CAM0 Interface (Bayer/BT1358/BT656/BT1120 Support) */
	VI_INTF_CAM0_BAYER_1080P,		/* 1920X1080 progressive */
	VI_INTF_CAM0_BAYER_720P,		/* 1920X1080 progressive */

	VI_INTF_CAM0_BT1358_480P,		/* 720X480 progressive */
	VI_INTF_CAM0_BT1358_576P,		/* 720X576 progressive */

	VI_INTF_CAM0_BT656_480I,		/* 720X480 interlaced */
	VI_INTF_CAM0_BT656_576I,		/* 720X576 interlaced */

	VI_INTF_CAM0_BT1120_720P,		/* 1280X720 progressive */
	VI_INTF_CAM0_BT1120_720I,		/* 1280X720 interlaced */
	VI_INTF_CAM0_BT1120_1080P,		/* 1920X1080 progressive */
	VI_INTF_CAM0_BT1120_1080I,		/* 1920X1080 interlaced */

	/* BD1 CAM1 Interface (BT1358/BT656/BT601 Support) */
	VI_INTF_CAM1_BT1358_480P,		/* 720X480 progressive */
	VI_INTF_CAM1_BT1358_576P,		/* 720X576 progressive */

	VI_INTF_CAM1_BT656_480I,		/* 720X480 interlaced */
	VI_INTF_CAM1_BT656_576I,		/* 720X576 interlaced */

	VI_INTF_CAM1_BT601_480P,		/* 720X480 progressive */
	VI_INTF_CAM1_BT601_576P,		/* 720X576 progressive */

	VI_INTF_CAM1_BT601_480I,		/* 720X480 interlaced */
	VI_INTF_CAM1_BT601_576I,		/* 720X576 interlaced */	

	/* Analog NTSC / PAL format */
	VI_INTF_PVI_480I,				/* 720x480 interlaced */
	VI_INTF_PVI_576I,				/* 720x576 interlaced */

	/* PVI format */
	VI_INTF_PVI_PVI_720P,			/* 1280x720 progressive */
	VI_INTF_PVI_PVI_1080P,			/* 1920x1080 progressive */
	
	/* CVI format */
	VI_INTF_PVI_CVI_720P,			/* 1280x720 progressive */
	VI_INTF_PVI_CVI_1080P,			/* 1920x1080 progressive */
	
	/* HDA format */
	VI_INTF_PVI_HDA_720P,			/* 1280x720 progressive */
	VI_INTF_PVI_HDA_1080P,			/* 1920x1080 progressive */
	
	/* HDT format */
	VI_INTF_PVI_HDT_720P,			/* 1280x720 progressive */
	VI_INTF_PVI_HDT_1080P,			/* 1920x1080 progressive */
	
    VI_INTF_BUTT
} VI_CAM_INTF_E;


typedef enum bdVI_DATA_PATH_E
{
    VI_PATH_BYPASS    = 0,       /* ISP bypass */
    VI_PATH_ISP       = 1,        /* ISP enable */
    VI_PATH_BUTT
}VI_DATA_PATH_E;

typedef enum bdVI_DATA_SHIFT_E
{
	VI_DATASHIFT_BAYER_2TO13 = 0,
	VI_DATASHIFT_BAYER_4TO15,
	VI_DATASHIFT_BAYER_8TO19,
	VI_DATASHIFT_BT1120_LOWCHIY,
	VI_DATASHIFT_BT1120_HICLOWY,
	VI_DATASHIFT_BT656_2TO9,
	VI_DATASHIFT_BT656_12TO19,
	VI_DATASHIFT_BUTT
} VI_DATA_SHIFT_E;

/* for PVI input format */
typedef enum bdVI_PVI_FORMAT
{
	VI_PVI_FORMAT_PVI = 0,
	VI_PVI_FORMAT_CVI, 
	VI_PVI_FORMAT_HDA,
	VI_PVI_FORMAT_HDT,
	VI_PVI_FORMAT_SD,
	VI_PVI_FORMAT_BUTT,
} VI_PVI_FORMAT_E;

typedef enum bdVI_PVI_FREQUENCY
{
	VI_PVI_FREQUENCY_25HZ = 0,
	VI_PVI_FREQUENCY_30HZ,
	VI_PVI_FREQUENCY_50HZ,
	VI_PVI_FREQUENCY_60HZ,
	VI_PVI_FREQUENCY_BUTT,
} VI_PVI_FREQUENCY_E;

typedef struct bdVI_PVI_ATTR_S
{
	VI_PVI_FORMAT_E		enPviFormat;
    BD_U16 u16horizontalActiveLine;  /* horizontal active lines count */
    BD_U16 u16horizontalDelay;       /* horizontal line delay */
    BD_U16 u16verticalActiveLine;  /* vertical active lines count */
    BD_U16 u16verticalDelay;       /* vertical line delay */
	VI_PVI_FREQUENCY_E enFrequency;		
} VI_PVI_ATTR_S;


/* the attributes of a VI device */
typedef struct bdVI_DEV_ATTR_S
{
    VI_CAM_INTF_E      enIntfMode;         /* Interface mode */
    VI_DATA_PATH_E      enDataPath;         /* ISP enable or bypass */
	VI_DATA_SHIFT_E		enDataShift;		// camera data shift, valid for CAM0
	VI_PVI_ATTR_S		stPviAttr;			/* valid only enIntfMode == VI_INTF_PVI_XXXXXX */
} VI_DEV_ATTR_S;


typedef struct bdVI_CHN_BIND_ATTR_S
{
    VI_DEV ViDev;
    VI_WAY ViWay;
} VI_CHN_BIND_ATTR_S;



/* the attributes of a VI channel */
typedef struct bdVI_CHN_ATTR_S
{
    SIZE_S          stDestSize;         /* Target picture size.
                                                For primary channels, stDestSize must be equal to stCapRect's u32Width and u32Height,
                                                because primary channel doesn't have scale capability. Additionally, it is a static
                                                attribute, That is, the value of stDestSize can be changed only after primary and
                                                secondary channels are disabled.
                                                For secondary channels, stDestSize is a dynamic attribute */
    BD_S32          s32SrcFrameRate;    /* Source frame rate. The value -1 indicates that the frame rate is not controlled */
    BD_S32          s32FrameRate;       /* Target frame rate. The value -1 indicates that the frame rate is not controlled */
} VI_CHN_ATTR_S;


typedef struct bdVI_CHN_STAT_S
{
    BD_BOOL bEnable;                    /* Whether this channel is enabled */
    BD_U32 u32IntCnt;                   /* The video frame interrupt count */
    BD_U32 u32FrmRate;                  /* current frame rate */
    BD_U32 u32LostInt;                  /* The interrupt is received but nobody care */
    BD_U32 u32VbFail;                   /* Video buffer malloc failure */
    BD_U32 u32PicWidth;                 /* curren pic width */
    BD_U32 u32PicHeight;                /* current pic height */
} VI_CHN_STAT_S;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* End of #ifndef__BD_COMM_VI_H__ */


