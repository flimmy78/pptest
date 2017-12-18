
/// File : bd_comm_video.h
/// Date : 2015/03

#ifndef __BD_COMM_VIDEO_H__
#define __BD_COMM_VIDEO_H__

#include <sys/time.h>
#include "bd_type.h"
#include "bd_common.h"
#include "bd_defines.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


typedef enum bdPIC_SIZE_E
{
	PIC_D1,
	PIC_960H,

	PIC_QVGA,    /* 320 * 240 */
	PIC_VGA,     /* 640 * 480 */    
	PIC_XGA,     /* 1024 * 768 */   

	PIC_HD720,   /* 1280 * 720 */
	PIC_HD1080,  /* 1920 * 1080 */
	PIC_HHD1080,  /* 960 * 540 */
	PIC_HD1088,  /* 1920 * 1088 */

	PIC_BUTT
}PIC_SIZE_E;

typedef enum bdVIDEO_NORM_E
{
	VIDEO_ENCODING_MODE_PAL=0,
	VIDEO_ENCODING_MODE_NTSC,
	VIDEO_ENCODING_MODE_AUTO,
	VIDEO_ENCODING_MODE_BUTT
} VIDEO_NORM_E;


/* we ONLY define picture format used, all unused will be deleted!*/
typedef enum bdPIXEL_FORMAT_E
{   
	PIXEL_FORMAT_RGB_1BPP = 0,
	PIXEL_FORMAT_RGB_2BPP,
	PIXEL_FORMAT_RGB_4BPP,
	PIXEL_FORMAT_RGB_8BPP,
	PIXEL_FORMAT_RGB_444,
	PIXEL_FORMAT_RGB_4444,
	PIXEL_FORMAT_RGB_555,
	PIXEL_FORMAT_RGB_565,
	PIXEL_FORMAT_RGB_1555,

	/*  9 reserved */
	PIXEL_FORMAT_RGB_888,
	PIXEL_FORMAT_RGB_8888,
	PIXEL_FORMAT_RGB_PLANAR_888,
	PIXEL_FORMAT_RGB_BAYER,

	PIXEL_FORMAT_YUV_A422,
	PIXEL_FORMAT_YUV_A444,

	PIXEL_FORMAT_YUV_PLANAR_422,
	PIXEL_FORMAT_YUV_PLANAR_420,
	PIXEL_FORMAT_YUV_PLANAR_444,

	PIXEL_FORMAT_YUV_SEMIPLANAR_422,
	PIXEL_FORMAT_YUV_SEMIPLANAR_420,
	PIXEL_FORMAT_YUV_SEMIPLANAR_444,

	PIXEL_FORMAT_UYVY_PACKAGE_422,
	PIXEL_FORMAT_YUYV_PACKAGE_422,
	PIXEL_FORMAT_VYUY_PACKAGE_422,
	PIXEL_FORMAT_YCbCr_PLANAR,

	PIXEL_FORMAT_BUTT   
} PIXEL_FORMAT_E;


typedef struct bdVIDEO_FRAME_INFO_S
{    
	BD_U32          u32Width;
	BD_U32          u32Height;    
	PIXEL_FORMAT_E  enPixelFormat;
	BD_U32          u32PhyAddr[2];
	BD_VOID         *pVirAddr[2];
	BD_U32          u32Stride[2];
	struct timeval  stTimeVal;
	BD_U32          u32TimeRef;			// Frame sequence number of a picture. according to dest frame rate
    BD_U32          u32PrivateData;	// ???
} VIDEO_FRAME_INFO_S;

typedef struct bdBITMAP_S
{
    PIXEL_FORMAT_E enPixelFormat;  /* Bitmap's pixel format */
    BD_U32 u32Width;               /* Bitmap's width */
    BD_U32 u32Height;              /* Bitmap's height */
    BD_VOID *pData;                /* Address of Bitmap's data */
} BITMAP_S;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* _BD_COMM_VIDEO_H_ */ 
