
/// File : bd_defines.h
/// Date : 2015/03

#ifndef __BD_DEFINES_H__
#define __BD_DEFINES_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define MAX_MMZ_NAME_LEN 16

/* For VENC */
#define VENC_MAX_GRP_NUM	16
#define VENC_MAX_CHN_NUM	16
#define VENC_MAX_ROI_NUM	8
#define H264E_MAX_WIDTH    1920
#define H264E_MAX_HEIGHT   2048
#define H264E_MIN_WIDTH    160
#define H264E_MIN_HEIGHT   64
#define JPEGE_MAX_WIDTH    8192
#define JPEGE_MAX_HEIGHT   8192
#define JPEGE_MIN_WIDTH    64
#define JPEGE_MIN_HEIGHT   64


/* For Region */
#define RGN_HANDLE_MAX         1024
#define OVERLAY_MAX_NUM        8
#define COVEREX_MAX_NUM        16
#define COVER_MAX_NUM          4
#define OVERLAYEX_MAX_NUM      0 

/* number of channle and device on video input unit of chip
 * Note! VIU_MAX_CHN_NUM is NOT equal to VIU_MAX_DEV_NUM
 * multiplied by VIU_MAX_CHN_NUM, because all VI devices
 * can't work at mode of 4 channles at the same time.
 */
#define VIU_MAX_DEV_NUM              2
#define VIU_MAX_WAY_NUM_PER_DEV      1
#define VIU_MAX_CHN_NUM_PER_DEV      2
#define VIU_MAX_PHYCHN_NUM           3
#define VIU_MAX_CHN_NUM              VIU_MAX_PHYCHN_NUM


#define VIU_PRIMARY_CHN_DEV			 0
#define VIU_SECONDARY_CHN_DEV		 1

#define VIU_ENQ_CHN_DEV				 15
#define VIU_ENQ_CHN_NUM				 15

/* memory input dest id */
#define MIU_ENQ_CHN_DEV				 14
#define MIU_ENQ_CHN_NUM				 14

#define VIU_CHN2DEV(ch)				(ch/VIU_MAX_CHN_NUM_PER_DEV)

#define AI_DEV_MAX_NUM          1
#define AO_DEV_MAX_NUM          1
#define AIO_MAX_CHN_NUM         2
#define AENC_MAX_CHN_NUM        32
#define ADEC_MAX_CHN_NUM        32


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_DEFINES_H__ */

