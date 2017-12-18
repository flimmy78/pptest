#ifndef __BD_COMM_REGION_H__
#define __BD_COMM_REGION_H__

#include "bd_common.h"
#include "bd_comm_video.h"
#include "bd_errno.h"
#include "bd_defines.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */


typedef BD_U32 RGN_HANDLE;

#define MAX_RGN_HANDLE		32

/* type of video regions */
typedef enum bdRGN_TYPE_E
{
    RGN_RGB_OVERLAY = 0,     
    RGN_YUV_OVERLAY,     
    RGN_YUV_COVER,
    RGN_BUTT
} RGN_TYPE_E;

#define GetRGNTypeString(x)	\
	((x == RGN_RGB_OVERLAY)?"rgb_overlay":\
	(x == RGN_YUV_OVERLAY)?"yuv_overlay":\
	(x == RGN_YUV_COVER)?"yuv_cover":"unknown")

typedef struct bdOVERLAY_CHN_ATTR_S
{
    POINT_S stPoint;
} OVERLAY_CHN_ATTR_S;


typedef struct bdCOVER_CHN_ATTR_S
{
    POINT_S stPoint;
    BD_U32 u32Color;
} COVER_CHN_ATTR_S;


typedef struct bdRGN_ATTR_U
{
    PIXEL_FORMAT_E enPixelFmt;
    POINT_S stPoint;
    SIZE_S stSize;
} RGN_ATTR_U;

typedef union bdRGN_CHN_ATTR_U
{
    OVERLAY_CHN_ATTR_S      stOverlayChn;      /* attribute of overlay region */
    COVER_CHN_ATTR_S        stCoverChn;        /* attribute of cover region */

} RGN_CHN_ATTR_U;

/* attribute of a region */
typedef struct bdRGN_ATTR_S
{
    RGN_TYPE_E enType;  /* region type */
    RGN_ATTR_U unAttr;  /* region attribute */

} RGN_ATTR_S;

/* attribute of a region */
typedef struct bdRGN_CHN_ATTR_S
{
    RGN_TYPE_E        enType;     /* region type */
    RGN_CHN_ATTR_U    unChnAttr;  /* region attribute */
} RGN_CHN_ATTR_S;


typedef struct bdRGN_CANVAS_INFO_S
{
    BD_U32         u32PhyAddr;
    BD_U32         u32VirtAddr;
    SIZE_S         stSize;              
    BD_U32         u32Stride;
    PIXEL_FORMAT_E enPixelFmt;  
} RGN_CANVAS_INFO_S;



/* invlalid device ID */
#define BD_ERR_RGN_INVALID_DEVID     BD_DEF_ERR(BD_ID_RGN, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_DEVID)
/* invlalid channel ID */
#define BD_ERR_RGN_INVALID_CHNID     BD_DEF_ERR(BD_ID_RGN, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID)
/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define BD_ERR_RGN_ILLEGAL_PARAM     BD_DEF_ERR(BD_ID_RGN, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
/* channel exists */
#define BD_ERR_RGN_EXIST             BD_DEF_ERR(BD_ID_RGN, EN_ERR_LEVEL_ERROR, EN_ERR_EXIST)
/*UN exist*/
#define BD_ERR_RGN_UNEXIST           BD_DEF_ERR(BD_ID_RGN, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST)
/* using a NULL point */
#define BD_ERR_RGN_NULL_PTR          BD_DEF_ERR(BD_ID_RGN, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
/* try to enable or initialize system,device or channel, before configing attribute */
#define BD_ERR_RGN_NOT_CONFIG        BD_DEF_ERR(BD_ID_RGN, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_CONFIG)
/* operation is not supported by NOW */
#define BD_ERR_RGN_NOT_SUPPORT       BD_DEF_ERR(BD_ID_RGN, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
/* operation is not permitted ,eg, try to change stati attribute */
#define BD_ERR_RGN_NOT_PERM          BD_DEF_ERR(BD_ID_RGN, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)
/* failure caused by malloc memory */
#define BD_ERR_RGN_NOMEM             BD_DEF_ERR(BD_ID_RGN, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
/* failure caused by malloc buffer */
#define BD_ERR_RGN_NOBUF             BD_DEF_ERR(BD_ID_RGN, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF)
/* no data in buffer */
#define BD_ERR_RGN_BUF_EMPTY         BD_DEF_ERR(BD_ID_RGN, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY)
/* no buffer for new data */
#define BD_ERR_RGN_BUF_FULL          BD_DEF_ERR(BD_ID_RGN, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL)
/* bad address, eg. used for copy_from_user & copy_to_user */
#define BD_ERR_RGN_BADADDR           BD_DEF_ERR(BD_ID_RGN, EN_ERR_LEVEL_ERROR, EN_ERR_BADADDR)
/* resource is busy, eg. destroy a venc chn without unregistering it */
#define BD_ERR_RGN_BUSY              BD_DEF_ERR(BD_ID_RGN, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)

/* System is not ready,maybe not initialed or loaded.
 * Returning the error code when opening a device file failed.
 */
#define BD_ERR_RGN_NOTREADY          BD_DEF_ERR(BD_ID_RGN, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __BD_COMM_REGION_H__ */



