
/// File : sample_comm_sys.c
/// Date : 2015/03

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>

#include "sample_comm.h"

/******************************************************************************
* function : get picture size(w*h), according Norm and enPicSize
******************************************************************************/
BD_S32 SAMPLE_COMM_SYS_GetPicSize(VIDEO_NORM_E enNorm, PIC_SIZE_E enPicSize, SIZE_S *pstSize)
{
///	fprintf(stderr, "enPicsize = %d, norm = %d\n", enPicSize, enNorm);
    switch (enPicSize)
    {
        case PIC_D1:
            pstSize->u32Width = 704;
            pstSize->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?576:480;
            break;
        case PIC_960H:
            pstSize->u32Width = 960;
            pstSize->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?576:480;
            break;			
        case PIC_QVGA:    /* 320 * 240 */
            pstSize->u32Width = 320;
            pstSize->u32Height = 240;
            break;
        case PIC_VGA:     /* 640 * 480 */
            pstSize->u32Width = 640;
            pstSize->u32Height = 480;
            break;
        case PIC_XGA:     /* 1024 * 768 */
            pstSize->u32Width = 1024;
            pstSize->u32Height = 768;
            break;
        case PIC_HD720:   /* 1280 * 720 */
            pstSize->u32Width = 1280;
            pstSize->u32Height = 720;
            break;
        case PIC_HD1080:  /* 1920 * 1080 */
            pstSize->u32Width = 1920;
            pstSize->u32Height = 1080;
            break;
        case PIC_HHD1080:  /* 960 * 540 */
            pstSize->u32Width = 960;
            pstSize->u32Height = 540;
            break;
        case PIC_HD1088:  /* 1920 * 1088 */
            pstSize->u32Width = 1920;
            pstSize->u32Height = 1088;
            break;
        default:
            return BD_FAILURE;
    }
    return BD_SUCCESS;
}

BD_S32 SAMPLE_COMM_SYS_GetEncSize(VIDEO_NORM_E enNorm, PIC_SIZE_E enPicSize, SIZE_S *pstSize)
{
    switch (enPicSize)
    {
        case PIC_D1:
            pstSize->u32Width = 704;
            pstSize->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?576:480;
            break;
        case PIC_960H:
            pstSize->u32Width = 960;
            pstSize->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?576:480;
            break;			
        case PIC_QVGA:    /* 320 * 240 */
            pstSize->u32Width = 320;
            pstSize->u32Height = 240;
            break;
        case PIC_VGA:     /* 640 * 480 */
            pstSize->u32Width = 640;
            pstSize->u32Height = 480;
            break;
        case PIC_XGA:     /* 1024 * 768 */
            pstSize->u32Width = 1024;
            pstSize->u32Height = 768;
            break;
        case PIC_HD720:   /* 1280 * 720 */
            pstSize->u32Width = 1280;
            pstSize->u32Height = 720;
            break;
        case PIC_HHD1080:  /* 960 * 540 */
            pstSize->u32Width = 960;
            pstSize->u32Height = 540;
            break;
        case PIC_HD1080:  /* 1920 * 1080 */
        case PIC_HD1088:  /* 1920 * 1088 */
            pstSize->u32Width = 1920;
            pstSize->u32Height = 1080;
            break;
        default:
            return BD_FAILURE;
    }
    return BD_SUCCESS;
}

#if 0
/******************************************************************************
* function : calculate VB Block size of Histogram.
******************************************************************************/
BD_U32 SAMPLE_COMM_SYS_CalcHistVbBlkSize(VIDEO_NORM_E enNorm, PIC_SIZE_E enPicSize, SIZE_S *pstHistBlkSize, BD_U32 u32AlignWidth)
{
    BD_S32 s32Ret;
    SIZE_S stPicSize;
    
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(enNorm, enPicSize, &stPicSize);
    if (BD_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("get picture size[%d] failed!\n", enPicSize);
            return BD_FAILURE;
    }

   SAMPLE_PRT("stPicSize.u32Width%d,pstHistBlkSize->u32Width%d\n,stPicSize.u32Height%d,pstHistBlkSize->u32Height%d\n",
   	stPicSize.u32Width,pstHistBlkSize->u32Width,
   	stPicSize.u32Height,pstHistBlkSize->u32Height );
    return (CEILING_2_POWER(44, u32AlignWidth)*CEILING_2_POWER(44, u32AlignWidth)*16*4);

    return BD_SUCCESS;
}


/******************************************************************************
* function : Set system memory location
******************************************************************************/
BD_S32 SAMPLE_COMM_SYS_MemConfig(BD_VOID)
{
    BD_S32 i = 0;
    BD_S32 s32Ret = BD_SUCCESS;

    BD_CHAR * pcMmzName;
    MPP_CHN_S stMppChnVI;
    MPP_CHN_S stMppChnVO;
    MPP_CHN_S stMppChnVPSS;
    MPP_CHN_S stMppChnGRP;
    MPP_CHN_S stMppChnVENC;
    MPP_CHN_S stMppChnRGN;
    MPP_CHN_S stMppChnVDEC;

    /*VI,VDEC最大通道数为32*/
    for(i=0;i<32;i++)
    {
        stMppChnVI.enModId = BD_ID_VIU;
        stMppChnVI.s32DevId = 0;
        stMppChnVI.s32ChnId = i;
        
        stMppChnVDEC.enModId = BD_ID_VDEC;
        stMppChnVDEC.s32DevId = 0;
        stMppChnVDEC.s32ChnId = i;
        
        if(0 == (i%2))
        {
            pcMmzName = NULL;  
        }
        else
        {
            pcMmzName = "ddr1";
        }

        /*vi*/
        s32Ret = BD_MPI_SYS_SetMemConf(&stMppChnVI,pcMmzName);
        if (s32Ret)
        {
            SAMPLE_PRT("BD_MPI_SYS_SetMemConf ERR !\n");
            return BD_FAILURE;
        }

        /*vdec*/
        s32Ret = BD_MPI_SYS_SetMemConf(&stMppChnVDEC,pcMmzName);
        if (s32Ret)
        {
            SAMPLE_PRT("BD_MPI_SYS_SetMemConf ERR !\n");
            return BD_FAILURE;
        }

    }  

    /*vpss,grp,venc最大通道为64*/
    for(i=0;i<64;i++)
    {
        stMppChnVPSS.enModId  = BD_ID_VPSS;
        stMppChnVPSS.s32DevId = i;
        stMppChnVPSS.s32ChnId = 0;

        stMppChnGRP.enModId  = BD_ID_GROUP;
        stMppChnGRP.s32DevId = i;
        stMppChnGRP.s32ChnId = 0;

        stMppChnVENC.enModId = BD_ID_VENC;
        stMppChnVENC.s32DevId = 0;
        stMppChnVENC.s32ChnId = i;

        if(0 == (i%2))
        {
            pcMmzName = NULL;  
        }
        else
        {
            pcMmzName = "ddr1";
        }

        /*vpss*/
        s32Ret = BD_MPI_SYS_SetMemConf(&stMppChnVPSS,pcMmzName);
        if (s32Ret)
        {
            SAMPLE_PRT("BD_MPI_SYS_SetMemConf ERR !\n");
            return BD_FAILURE;
        }

        /*grp*/
        s32Ret = BD_MPI_SYS_SetMemConf(&stMppChnGRP,pcMmzName);
        if (s32Ret)
        {
            SAMPLE_PRT("BD_MPI_SYS_SetMemConf ERR !\n");
            return BD_FAILURE;
        } 

        /*venc*/
        s32Ret = BD_MPI_SYS_SetMemConf(&stMppChnVENC,pcMmzName);
        if (s32Ret)
        {
            SAMPLE_PRT("BD_MPI_SYS_SetMemConf ERR !\n");
            return BD_FAILURE;
        }

    }
    
    stMppChnRGN.enModId  = BD_ID_RGN;
    stMppChnRGN.s32DevId = 0;
    stMppChnRGN.s32ChnId = 0;
        
    /*配置VO内存*/
    stMppChnVO.enModId  = BD_ID_VOU;
    stMppChnVO.s32DevId = 0;
    stMppChnVO.s32ChnId = 0;
    s32Ret = BD_MPI_SYS_SetMemConf(&stMppChnVO,"ddr1");
    if (s32Ret)
    {
        SAMPLE_PRT("BD_MPI_SYS_SetMemConf ERR !\n");
        return BD_FAILURE;
    } 
    
    return s32Ret;
}

#else

BD_U32 SAMPLE_COMM_SYS_CalcPicVbBlkSize(VIDEO_NORM_E enNorm, PIC_SIZE_E enPicSize, PIXEL_FORMAT_E enPixFmt, BD_U32 u32AlignWidth)
{
    BD_S32 s32Ret = BD_FAILURE;
    SIZE_S stSize;

    s32Ret = SAMPLE_COMM_SYS_GetPicSize(enNorm, enPicSize, &stSize);
    if (BD_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("get picture size[%d] failed!\n", enPicSize);
            return BD_FAILURE;
    }

    if (PIXEL_FORMAT_YUV_SEMIPLANAR_422 != enPixFmt && PIXEL_FORMAT_YUV_SEMIPLANAR_420 != enPixFmt)
    {
        SAMPLE_PRT("pixel format[%d] input failed!\n", enPixFmt);
            return BD_FAILURE;
    }

    if (16 != u32AlignWidth && 32 != u32AlignWidth && 64 != u32AlignWidth)
    {
        SAMPLE_PRT("system align width[%d] input failed!\n",\
               u32AlignWidth);
            return BD_FAILURE;
    }

    SAMPLE_PRT("w:%d, u32AlignWidth:%d\n", CEILING_2_POWER(stSize.u32Width,u32AlignWidth), u32AlignWidth);
    SAMPLE_PRT("h:%d, u32AlignWidth:%d\n", CEILING_2_POWER(stSize.u32Height,u32AlignWidth), u32AlignWidth);
    return (CEILING_2_POWER(stSize.u32Width, u32AlignWidth) * \
            CEILING_2_POWER(stSize.u32Height,u32AlignWidth) * \
           ((PIXEL_FORMAT_YUV_SEMIPLANAR_422 == enPixFmt)?2:1.5));
}

#endif

/******************************************************************************
* function : vb init & MPI system init
******************************************************************************/
BD_S32 SAMPLE_COMM_SYS_Init(VB_CONF_S *pstVbConf)
{
    MPP_SYS_CONF_S stSysConf = {0};
    BD_S32 s32Ret = BD_FAILURE;

    BD_MPI_SYS_Exit();
	BD_MPI_VB_Exit();

	if (pstVbConf) {
		s32Ret = BD_MPI_VB_SetConf(pstVbConf);
		if (s32Ret != BD_SUCCESS) {
			SAMPLE_PRT("BD_MPI_VB_SetConf failed\n");
			return BD_FAILURE;
		}

		s32Ret = BD_MPI_VB_Init();
		if (s32Ret != BD_SUCCESS) {
			SAMPLE_PRT("BD_MPI_VB_Init failed\n");
			return BD_FAILURE;
		}
	}

    stSysConf.u32AlignWidth = SAMPLE_SYS_ALIGN_WIDTH;
    s32Ret = BD_MPI_SYS_SetConf(&stSysConf);
    if (BD_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("BD_MPI_SYS_SetConf failed\n");
        return BD_FAILURE;
    }

    s32Ret = BD_MPI_SYS_Init();
    if (BD_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("BD_MPI_SYS_Init failed!\n");
        return BD_FAILURE;
    }

    return BD_SUCCESS;
}

/******************************************************************************
* function : vb exit & MPI system exit
******************************************************************************/
BD_VOID SAMPLE_COMM_SYS_Exit(void)
{
	BD_MPI_SYS_Exit();
	BD_MPI_VB_Exit();
    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
