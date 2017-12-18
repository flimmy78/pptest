
/// File : sample_comm_venc.c
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

#if 0
const BD_U8 g_SOI[2] = {0xFF, 0xD8};
const BD_U8 g_EOI[2] = {0xFF, 0xD9};
#endif
static pthread_t gs_VencPid;
static SAMPLE_VENC_GETSTREAM_PARA_S gs_stPara;
static BD_S32 gs_s32SnapCnt = 0;

/******************************************************************************
* function : venc bind vi          
******************************************************************************/
BD_S32 SAMPLE_COMM_VENC_BindVi(VENC_GRP GrpChn, VI_DEV ViDev, VI_CHN ViChn)
{
    BD_S32 s32Ret = BD_SUCCESS;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

///printf("GrpChn:%d ViDev:%d ViChn:%d\n", GrpChn, ViDev, ViChn);
    stSrcChn.enModId = BD_ID_VIU;
    stSrcChn.s32DevId = ViDev;
    stSrcChn.s32ChnId = ViChn;

    stDestChn.enModId = BD_ID_VENC_GRP;
    stDestChn.s32DevId = GrpChn;
    stDestChn.s32ChnId = 0;

    s32Ret = BD_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (s32Ret != BD_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return BD_FAILURE;
    }

    return s32Ret;
}

/******************************************************************************
* function : venc unbind vi
******************************************************************************/
BD_S32 SAMPLE_COMM_VENC_UnBindVi(VENC_GRP GrpChn,VI_DEV ViDev,VI_CHN ViChn)
{
    BD_S32 s32Ret = BD_SUCCESS;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = BD_ID_VIU;
    stSrcChn.s32DevId = ViDev;
    stSrcChn.s32ChnId = ViChn;

    stDestChn.enModId = BD_ID_VENC_GRP;
    stDestChn.s32DevId = GrpChn;
    stDestChn.s32ChnId = 0;

    s32Ret = BD_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    if (s32Ret != BD_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return BD_FAILURE;
    }

    return s32Ret;
}
/******************************************************************************
* funciton : get file postfix according palyload_type.
******************************************************************************/
BD_S32 SAMPLE_COMM_VENC_GetFilePostfix(PAYLOAD_TYPE_E enPayload, char *szFilePostfix)
{
    if (PT_H264 == enPayload)
    {
        strcpy(szFilePostfix, ".h264");
    }
    else if (PT_JPEG == enPayload)
    {
        strcpy(szFilePostfix, ".jpg");
    }
    else if (PT_MJPEG == enPayload)
    {
        strcpy(szFilePostfix, ".mjp");
    }
    else
    {
        SAMPLE_PRT("payload type err! - 0x%x\n", enPayload);
        return BD_FAILURE;
    }
    return BD_SUCCESS;
}

/******************************************************************************
* funciton : save mjpeg stream. 
* WARNING: in Hi3531, user needn't write SOI & EOI.
******************************************************************************/
BD_S32 SAMPLE_COMM_VENC_SaveMJpeg(FILE* fpJpegFile, VENC_STREAM_S *pstStream)
{
    VENC_PACK_S*  pstData;
    BD_U32 i;

    //fwrite(g_SOI, 1, sizeof(g_SOI), fpJpegFile); //in Hi3531, user needn't write SOI!

    for (i = 0; i < pstStream->u32PackCount; i++)
    {
        pstData = &pstStream->pstPack[i];
        fwrite(pstData->pu8Addr, pstData->u32Len, 1, fpJpegFile);
    }

    //fwrite(g_EOI, 1, sizeof(g_EOI), fpJpegFile);//in Hi3531, user needn't write SOI!

    return BD_SUCCESS;
}

/******************************************************************************
* funciton : save H264 stream
******************************************************************************/
BD_S32 SAMPLE_COMM_VENC_SaveH264(FILE* fpH264File, VENC_STREAM_S *pstStream)
{
    BD_S32 i;

    
    for (i = 0; i < pstStream->u32PackCount; i++)
    {
        fwrite(pstStream->pstPack[i].pu8Addr,
               pstStream->pstPack[i].u32Len, 1, fpH264File);

        fflush(fpH264File);
    }
    

    return BD_SUCCESS;
}

/******************************************************************************
* funciton : save jpeg stream
******************************************************************************/
BD_S32 SAMPLE_COMM_VENC_SaveJPEG(FILE *fpJpegFile, VENC_STREAM_S *pstStream)
{
    VENC_PACK_S*  pstData;
    BD_U32 i;

    for (i = 0; i < pstStream->u32PackCount; i++)
    {
        pstData = &pstStream->pstPack[i];
        fwrite(pstData->pu8Addr, pstData->u32Len, 1, fpJpegFile);
    }

    return BD_SUCCESS;
}

/******************************************************************************
* funciton : save snap stream
******************************************************************************/
BD_S32 SAMPLE_COMM_VENC_SaveSnap(VENC_STREAM_S *pstStream)
{
    char acFile[128]  = {0};
    FILE *pFile;
    BD_S32 s32Ret;

    sprintf(acFile, "snap_%d.jpg", gs_s32SnapCnt);
    pFile = fopen(acFile, "wb");
    if (pFile == NULL)
    {
        SAMPLE_PRT("open file err\n");
        return BD_FAILURE;
    }
    s32Ret = SAMPLE_COMM_VENC_SaveJPEG(pFile, pstStream);
    if (BD_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("save snap picture failed!\n");
        return BD_FAILURE;
    }
    fclose(pFile);
    gs_s32SnapCnt++;
    return BD_SUCCESS;
}

/******************************************************************************
* funciton : save stream
******************************************************************************/
BD_S32 SAMPLE_COMM_VENC_SaveStream(PAYLOAD_TYPE_E enType,FILE *pFd, VENC_STREAM_S *pstStream)
{
    BD_S32 s32Ret;

    if (PT_H264 == enType)
    {
        s32Ret = SAMPLE_COMM_VENC_SaveH264(pFd, pstStream);
    }
    else if (PT_MJPEG == enType)
    {
        s32Ret = SAMPLE_COMM_VENC_SaveMJpeg(pFd, pstStream);
    }
    else
    {
        return BD_FAILURE;
    }
    return s32Ret;
}

/******************************************************************************
* funciton : Start venc stream mode (h264, mjpeg)
* note      : rate control parameter need adjust, according your case.
******************************************************************************/
BD_S32 SAMPLE_COMM_VENC_Start(VENC_GRP VencGrp, VENC_CHN VencChn, PAYLOAD_TYPE_E enType, VIDEO_NORM_E enNorm, PIC_SIZE_E enSize, SAMPLE_RC_E enRcMode)
{
	BD_S32 s32Ret;
	VENC_CHN_ATTR_S stVencChnAttr;
	VENC_ATTR_H264_S stH264Attr;
	VENC_ATTR_H264_CBR_S    stH264Cbr;
	VENC_ATTR_H264_VBR_S    stH264Vbr;
	VENC_ATTR_H264_FIXQP_S  stH264FixQp;
	VENC_ATTR_MJPEG_S stMjpegAttr;
	VENC_ATTR_MJPEG_FIXQP_S stMjpegeFixQp;
	VENC_ATTR_JPEG_S stJpegAttr;
	SIZE_S stCapSize, stEncSize;

	s32Ret = SAMPLE_COMM_SYS_GetPicSize(enNorm, enSize, &stCapSize);
	if (BD_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("Get picture size failed!\n");
		return BD_FAILURE;
	}
	s32Ret = SAMPLE_COMM_SYS_GetEncSize(enNorm, enSize, &stEncSize);
	if (BD_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("Get picture size failed!\n");
		return BD_FAILURE;
	}

	/******************************************
	step 2:  Create Venc Channel
	******************************************/
	stVencChnAttr.stVeAttr.enType = enType;
	switch(enType)
	{
		case PT_H264:
		{
			stH264Attr.u32BufWidth  = stCapSize.u32Width;
			stH264Attr.u32BufHeight = stCapSize.u32Height;
			stH264Attr.u32EncWidth  = stEncSize.u32Width;
			stH264Attr.u32EncHeight = stEncSize.u32Height;
			stH264Attr.u32BufSize   = stCapSize.u32Width * stCapSize.u32Height * 2;
			stH264Attr.u32MaxFrmSize = 512*1024;
			stH264Attr.u32SlotsToUse = 2;
			stH264Attr.u32PicNodes = 2;
			stH264Attr.u32Profile  = 2;/*0: baseline; 1:MP; 2:HP   ? */
			stH264Attr.u32UseMotion = 0;
			memcpy(&stVencChnAttr.stVeAttr.stAttrH264e, &stH264Attr, sizeof(VENC_ATTR_H264_S));

			if(SAMPLE_RC_CBR == enRcMode)
			{
				stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
				stH264Cbr.u32Gop            = (VIDEO_ENCODING_MODE_PAL== enNorm)?25:30;
				stH264Cbr.u32ViFrmRate      = (VIDEO_ENCODING_MODE_PAL== enNorm)?25:30;/* input (vi) frame rate */
				stH264Cbr.fr32TargetFrmRate = (VIDEO_ENCODING_MODE_PAL== enNorm)?25:30;/* target frame rate */
				stH264Cbr.u32MinQp = 16;		/// 0:No limit
				stH264Cbr.u32MaxQp = 51;		/// 0:No limit
				stH264Cbr.bDisableBitStuffing = BD_FALSE;
				switch (enSize)
				{
					case PIC_QVGA:    /* 320 * 240 */
						stH264Cbr.u32BitRate = 512;
						break;
					case PIC_D1:
					case PIC_VGA:	   /* 640 * 480 */
						stH264Cbr.u32BitRate = 1024;
						break;
					case PIC_HD720:   /* 1280 * 720 */
						stH264Cbr.u32BitRate = 1024*4;
						break;
					case PIC_HD1080:  /* 1920 * 1080 */
					case PIC_HD1088:  /* 1920 * 1080 */
						stH264Cbr.u32BitRate = 1024*8;
						break;
					default :
						stH264Cbr.u32BitRate = 1024;
						break;
				}

				memcpy(&stVencChnAttr.stRcAttr.stAttrH264Cbr, &stH264Cbr, sizeof(VENC_ATTR_H264_CBR_S));
			}
			else if (SAMPLE_RC_FIXQP == enRcMode) 
			{
				stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264FIXQP;
				stH264FixQp.u32Gop = (VIDEO_ENCODING_MODE_PAL== enNorm)?25:30;
				stH264FixQp.u32ViFrmRate = (VIDEO_ENCODING_MODE_PAL== enNorm)?25:30;
				stH264FixQp.fr32TargetFrmRate = (VIDEO_ENCODING_MODE_PAL== enNorm)?25:30;
				stH264FixQp.u32IQp = 20;
				stH264FixQp.u32PQp = 23;
				memcpy(&stVencChnAttr.stRcAttr.stAttrH264FixQp, &stH264FixQp,sizeof(VENC_ATTR_H264_FIXQP_S));
			}
			else if (SAMPLE_RC_VBR == enRcMode) 
			{
				stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264VBR;
				stH264Vbr.u32Gop = (VIDEO_ENCODING_MODE_PAL== enNorm)?25:30;
				stH264Vbr.u32ViFrmRate = (VIDEO_ENCODING_MODE_PAL== enNorm)?25:30;
				stH264Vbr.fr32TargetFrmRate = (VIDEO_ENCODING_MODE_PAL== enNorm)?25:30;
				stH264Vbr.u32MinQp = 24;
				stH264Vbr.u32MaxQp = 32;
				switch (enSize)
				{
					case PIC_QVGA:    /* 320 * 240 */
						stH264Vbr.u32MaxBitRate = 512*3;
						break;
					case PIC_D1:
					case PIC_VGA:	   /* 640 * 480 */
						stH264Vbr.u32MaxBitRate = 1024;
						break;
					case PIC_HD720:   /* 1280 * 720 */
						stH264Vbr.u32MaxBitRate = 1024*4;
						break;
					case PIC_HD1080:  /* 1920 * 1080 */
					case PIC_HD1088:  /* 1920 * 1080 */
						stH264Vbr.u32MaxBitRate = 1024*8;
						break;
					default :
						stH264Vbr.u32MaxBitRate = 1024;
						break;
				}
				memcpy(&stVencChnAttr.stRcAttr.stAttrH264Vbr, &stH264Vbr, sizeof(VENC_ATTR_H264_VBR_S));
			}
			else
			{
				return BD_FAILURE;
			}
		}
		break;

		case PT_MJPEG:
		{
			stMjpegAttr.u32BufWidth  = stCapSize.u32Width;
			stMjpegAttr.u32BufHeight = stCapSize.u32Height;
			stMjpegAttr.u32EncWidth  = stEncSize.u32Width;
			stMjpegAttr.u32EncHeight = stEncSize.u32Height;
			stMjpegAttr.u32BufSize   = stCapSize.u32Width * stCapSize.u32Height * 2;
			memcpy(&stVencChnAttr.stVeAttr.stAttrMjpeg, &stMjpegAttr, sizeof(VENC_ATTR_MJPEG_S));

			if(SAMPLE_RC_FIXQP == enRcMode)
			{
				stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_MJPEGFIXQP;
				stMjpegeFixQp.u32Qfactor        = 90;
				stMjpegeFixQp.u32ViFrmRate      = (VIDEO_ENCODING_MODE_PAL== enNorm)?25:30;
				stMjpegeFixQp.fr32TargetFrmRate = (VIDEO_ENCODING_MODE_PAL== enNorm)?25:30;
				memcpy(&stVencChnAttr.stRcAttr.stAttrMjpegeFixQp, &stMjpegeFixQp,
						sizeof(VENC_ATTR_MJPEG_FIXQP_S));
			}
			else 
			{
				SAMPLE_PRT("cann't support other mode in this version!\n");

				return BD_FAILURE;
			}
		}
		break;

		case PT_JPEG:
			stJpegAttr.u32BufWidth  = stCapSize.u32Width;
			stJpegAttr.u32BufHeight = stCapSize.u32Height;
			stJpegAttr.u32EncWidth  = stEncSize.u32Width;
			stJpegAttr.u32EncHeight = stEncSize.u32Height;
			stJpegAttr.u32BufSize   = stCapSize.u32Width * stCapSize.u32Height * 2;
			stJpegAttr.u32Qfactor   = 90;
			memcpy(&stVencChnAttr.stVeAttr.stAttrJpeg, &stJpegAttr, sizeof(VENC_ATTR_JPEG_S));
			break;
		default:
			return BD_ERR_VENC_NOT_SUPPORT;
	}

	s32Ret = BD_MPI_VENC_CreateChn(VencChn, &stVencChnAttr);
	if (BD_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("BD_MPI_VENC_CreateChn [%d] faild with %#x!\n",\
		VencChn, s32Ret);
		return s32Ret;
	}

	/******************************************
	step 3:  Regist Venc Channel to VencGrp
	******************************************/
	s32Ret = BD_MPI_VENC_RegisterChn(VencGrp, VencChn);
	if (BD_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("BD_MPI_VENC_RegisterChn faild with %#x!\n", s32Ret);
		return BD_FAILURE;
	}

	/******************************************
	step 4:  Start Recv Venc Pictures
	******************************************/
	s32Ret = BD_MPI_VENC_StartRecvPic(VencChn);
	if (BD_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("BD_MPI_VENC_StartRecvPic faild with%#x!\n", s32Ret);
		return BD_FAILURE;
	}

	return BD_SUCCESS;
}

/******************************************************************************
* funciton : Stop venc ( stream mode -- H264, MJPEG )
******************************************************************************/
BD_S32 SAMPLE_COMM_VENC_Stop(VENC_CHN VencChn)
{
    BD_S32 s32Ret;

    /******************************************
     step 1:  Stop Recv Pictures
    ******************************************/
    s32Ret = BD_MPI_VENC_StopRecvPic(VencChn);
    if (BD_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("BD_MPI_VENC_StopRecvPic vechn[%d] failed with %#x!\n",\
               VencChn, s32Ret);
        return BD_FAILURE;
    }

    /******************************************
     step 2:  UnRegist Venc Channel
    ******************************************/
    s32Ret = BD_MPI_VENC_UnRegisterChn(VencChn);
    if (BD_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("BD_MPI_VENC_UnRegisterChn vechn[%d] failed with %#x!\n",\
               VencChn, s32Ret);
        return BD_FAILURE;
    }

    /******************************************
     step 3:  Distroy Venc Channel
    ******************************************/
    s32Ret = BD_MPI_VENC_DestroyChn(VencChn);
    if (BD_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("BD_MPI_VENC_DestroyChn vechn[%d] failed with %#x!\n",\
               VencChn, s32Ret);
        return BD_FAILURE;
    }

    return BD_SUCCESS;
}


#include <time.h>
#include <assert.h>

#define PRN_EVERY_FRAME			0

typedef struct _stat {
	struct timeval pre_time;
	BD_U32 bytes_encoded_t;
	BD_U32 bytes_encoded_i;
	BD_U32 bytes_encoded_ni;
	BD_U32 frames;
} FRM_STAT;
FRM_STAT frm_stat[3];

static void diff(struct timeval t1, struct timeval *t0, struct timeval *df)
{
	if (t0->tv_usec > t1.tv_usec) {
		t1.tv_usec += 1000000;
		t1.tv_sec--;
	}
	df->tv_sec = t1.tv_sec - t0->tv_sec;
	df->tv_usec = t1.tv_usec - t0->tv_usec;
}

static void PrnFrameInfoH264(BD_S32 id, VENC_STREAM_S *strms)
{
	VENC_STREAM_INFO_H264_S *stinfo = &strms->stH264Info;
#if PRN_EVERY_FRAME
	struct timeval *tv = (struct timeval *)&strms->pstPack->u64PTS;
#endif
	struct timeval cur_time, dif_time;
	FRM_STAT *pfs = &frm_stat[id];
	int rtv, apb;

	if (stinfo->enRefType == IDRSLICE) {
		rtv = gettimeofday(&cur_time, NULL);
		assert(rtv == 0);
		diff(cur_time, &pfs->pre_time, &dif_time);
		apb = ((pfs->frames > 1) ? pfs->bytes_encoded_ni/(pfs->frames-1) : 0);
		printf("[%d] %d.%03ds %2d frms %8d bytes %6d kbits, i:%7d apb:%6d pb:%8d bytes pa:0x%08x\n", id,
			(int)dif_time.tv_sec, (int)dif_time.tv_usec / 1000, pfs->frames,
			pfs->bytes_encoded_t, pfs->bytes_encoded_t >> 7,
			pfs->bytes_encoded_i, apb, pfs->bytes_encoded_ni,
			strms->pstPack->u32PhyAddr);
		pfs->bytes_encoded_t = 0;
		pfs->bytes_encoded_i = 0;
		pfs->bytes_encoded_ni = 0;
		pfs->frames = 0;
		pfs->pre_time = cur_time;
	}

#if PRN_EVERY_FRAME
	printf("[%d]%u.%u PicBytes:%d PSkipMb:%d IpcmMb:%d, I/P:%d POC:%d\n",
		id, (BD_U32)tv->tv_sec, (BD_U32)tv->tv_usec,
		stinfo->u32PicBytesNum, stinfo->u32PSkipMbNum, stinfo->u32IpcmMbNum,
		stinfo->enRefType, strms->u32Seq);
#endif

	pfs->frames++;
	pfs->bytes_encoded_t += stinfo->u32PicBytesNum;
	if (stinfo->enRefType == IDRSLICE) {
		pfs->bytes_encoded_i += stinfo->u32PicBytesNum;
	} else {
		pfs->bytes_encoded_ni += stinfo->u32PicBytesNum;
	}
}
static void PrnFrameInfoJPEG(BD_S32 id, VENC_STREAM_S *strms)
{
	VENC_STREAM_INFO_JPEG_S *stinfo = &strms->stJpegInfo;
#if PRN_EVERY_FRAME
	struct timeval *tv = (struct timeval *)&strms->pstPack->u64PTS;
#endif
	struct timeval cur_time;
	FRM_STAT *pfs = &frm_stat[id];
	int rtv;

	rtv = gettimeofday(&cur_time, NULL);
	assert(rtv == 0);
	if (cur_time.tv_usec < pfs->pre_time.tv_usec) {
		printf("[%d] %2d frms %8d bytes %6d kbits, avg %8d bytes pa:0x%08x\n", id,
			pfs->frames, pfs->bytes_encoded_t, pfs->bytes_encoded_t >> 7,
			pfs->bytes_encoded_t/pfs->frames, strms->pstPack->u32PhyAddr);
		pfs->bytes_encoded_t = 0;
		pfs->bytes_encoded_i = 0;
		pfs->bytes_encoded_ni = 0;
		pfs->frames = 0;
	}

#if PRN_EVERY_FRAME
	printf("[%d]%u.%u PicBytes:%d PSkipMb:%d IpcmMb:%d, I/P:%d POC:%d\n",
		id, (BD_U32)tv->tv_sec, (BD_U32)tv->tv_usec,
		stinfo->u32PicBytesNum, stinfo->u32PSkipMbNum, stinfo->u32IpcmMbNum,
		stinfo->enRefType, strms->u32Seq);
#endif

	pfs->frames++;
	pfs->pre_time = cur_time;
	pfs->bytes_encoded_t += stinfo->u32PicBytesNum;
}
static void PrnFrameInfo(BD_S32 id, VENC_STREAM_S *strms, PAYLOAD_TYPE_E type)
{
	if (type == PT_H264) {
		PrnFrameInfoH264(id, strms);
	} else if (type == PT_MJPEG) {
		PrnFrameInfoJPEG(id, strms);
	} else if (type == PT_JPEG) {
		printf("JPEG captured 1 frame\n");
	}
}

/******************************************************************************
* funciton : get stream from each channels and save them
******************************************************************************/
BD_VOID* SAMPLE_COMM_VENC_GetVencStreamProc(BD_VOID *p)
{
	BD_S32 i;
	BD_S32 s32ChnTotal;
	VENC_CHN_ATTR_S stVencChnAttr;
	SAMPLE_VENC_GETSTREAM_PARA_S *pstPara;
	BD_S32 maxfd = 0;
	struct timeval TimeoutVal;
	fd_set read_fds;
	BD_S32 VencFd[VENC_MAX_CHN_NUM];
	BD_CHAR aszFileName[VENC_MAX_CHN_NUM][64];
	FILE *pFile[VENC_MAX_CHN_NUM];
	char szFilePostfix[10];
	VENC_CHN_STAT_S stStat;
	VENC_STREAM_S stStream;
	BD_S32 s32Ret;
	VENC_CHN VencChn;
	PAYLOAD_TYPE_E enPayLoadType[VENC_MAX_CHN_NUM];

	pstPara = (SAMPLE_VENC_GETSTREAM_PARA_S*)p;
	s32ChnTotal = pstPara->s32Cnt;

	/******************************************
	step 1:  check & prepare save-file & venc-fd
	******************************************/
	if (s32ChnTotal >= VENC_MAX_CHN_NUM)
	{
		SAMPLE_PRT("input count invaild\n");
		return NULL;
	}
	for (i = 0; i < s32ChnTotal; i++)
	{
		/* decide the stream file name, and open file to save stream */
		VencChn = i;
		s32Ret = BD_MPI_VENC_GetChnAttr(VencChn, &stVencChnAttr);
		if(s32Ret != BD_SUCCESS)
		{
			SAMPLE_PRT("BD_MPI_VENC_GetChnAttr chn[%d] failed with %#x!\n", \
			VencChn, s32Ret);
			return NULL;
		}
		enPayLoadType[i] = stVencChnAttr.stVeAttr.enType;

		if ((PT_H264 == enPayLoadType[i]) || (PT_MJPEG == enPayLoadType[i]))
		{
			s32Ret = SAMPLE_COMM_VENC_GetFilePostfix(enPayLoadType[i], szFilePostfix);
			if(s32Ret != BD_SUCCESS)
			{
				SAMPLE_PRT("SAMPLE_COMM_VENC_GetFilePostfix [%d] failed with %#x!\n", \
						stVencChnAttr.stVeAttr.enType, s32Ret);
				return NULL;
			}
			sprintf(aszFileName[i], "stream_chn%d%s", i, szFilePostfix);
			pFile[i] = fopen(aszFileName[i], "wb");
			if (!pFile[i])
			{
				SAMPLE_PRT("open file[%s] failed!\n", 
				aszFileName[i]);
				return NULL;
			}
		}
		else
		{
			pFile[i] = NULL;
		}

		/* Set Venc Fd. */
		VencFd[i] = BD_MPI_VENC_GetFd(i);
		if (VencFd[i] < 0)
		{
			SAMPLE_PRT("BD_MPI_VENC_GetFd failed with %#x!\n", 
			VencFd[i]);
			return NULL;
		}
		if (maxfd <= VencFd[i])
		{
			maxfd = VencFd[i];
		}
	}

	/******************************************
	 step 2:  Start to get streams of each channel.
	******************************************/
	while (BD_TRUE == pstPara->bThreadStart)
	{
		FD_ZERO(&read_fds);
		for (i = 0; i < s32ChnTotal; i++)
		{
			FD_SET(VencFd[i], &read_fds);
		}

		TimeoutVal.tv_sec  = 2;
		TimeoutVal.tv_usec = 0;
		s32Ret = select(maxfd + 1, &read_fds, NULL, NULL, &TimeoutVal);
		if (s32Ret < 0)
		{
			SAMPLE_PRT("select failed!\n");
			break;
		}
		else if (s32Ret == 0)
		{
			SAMPLE_PRT("get venc stream time out\n");
			continue;
		}
		else
		{
			for (i = 0; i < s32ChnTotal; i++)
			{
				if (FD_ISSET(VencFd[i], &read_fds))
				{
					/*******************************************************
					 step 2.1 : query how many packs in one-frame stream.
					*******************************************************/
					memset(&stStream, 0, sizeof(stStream));
					s32Ret = BD_MPI_VENC_Query(i, &stStat);
					if (BD_SUCCESS != s32Ret)
					{
						SAMPLE_PRT("BD_MPI_VENC_Query chn[%d] failed with %#x!\n", i, s32Ret);
						break;
					}

					/*******************************************************
					 step 2.2 : malloc corresponding number of pack nodes.
					*******************************************************/
					stStream.pstPack = (VENC_PACK_S*)malloc(sizeof(VENC_PACK_S) * stStat.u32CurPacks);
					if (NULL == stStream.pstPack)
					{
						SAMPLE_PRT("malloc stream pack failed!\n");
						break;
					}

					/*******************************************************
					 step 2.3 : call mpi to get one-frame stream
					*******************************************************/
					stStream.u32PackCount = stStat.u32CurPacks;
					s32Ret = BD_MPI_VENC_GetStream(i, &stStream, BD_TRUE);
					if (BD_SUCCESS != s32Ret)
					{
						free(stStream.pstPack);
						stStream.pstPack = NULL;
						SAMPLE_PRT("BD_MPI_VENC_GetStream failed with %#x!\n", \
								s32Ret);
						break;
					}

					PrnFrameInfo(i, &stStream, enPayLoadType[i]);

					/*******************************************************
					 step 2.4 : save frame to file
					*******************************************************/
					if (PT_JPEG == enPayLoadType[i]) {
						s32Ret = SAMPLE_COMM_VENC_SaveSnap(&stStream);
						if (BD_SUCCESS != s32Ret)
						{
							free(stStream.pstPack);
							stStream.pstPack = NULL;
							SAMPLE_PRT("save stream failed!\n");
							break;
						}
					} else {
#if !defined(VENC_SKIP_RECORDING)
						s32Ret = SAMPLE_COMM_VENC_SaveStream(enPayLoadType[i], pFile[i], &stStream);
						if (BD_SUCCESS != s32Ret)
						{
							free(stStream.pstPack);
							stStream.pstPack = NULL;
							SAMPLE_PRT("save stream failed!\n");
							break;
						}
#endif
					}
					/*******************************************************
					step 2.5 : release stream
					*******************************************************/
					s32Ret = BD_MPI_VENC_ReleaseStream(i, &stStream);
					if (BD_SUCCESS != s32Ret)
					{
						free(stStream.pstPack);
						stStream.pstPack = NULL;
						break;
					}
					/*******************************************************
					step 2.6 : free pack nodes
					*******************************************************/
					free(stStream.pstPack);
					stStream.pstPack = NULL;
				}
			}
		}
	}

	/*******************************************************
	* step 3 : close save-file
	*******************************************************/
	for (i = 0; i < s32ChnTotal; i++)
	{
		if (pFile[i]) fclose(pFile[i]);
	}

	return NULL;
}

/******************************************************************************
* funciton : start get venc stream process thread
******************************************************************************/
BD_S32 SAMPLE_COMM_VENC_StartGetStream(BD_S32 s32Cnt)
{
    gs_stPara.bThreadStart = BD_TRUE;
    gs_stPara.s32Cnt = s32Cnt;

    return pthread_create(&gs_VencPid, 0, SAMPLE_COMM_VENC_GetVencStreamProc, (BD_VOID*)&gs_stPara);
}

/******************************************************************************
* funciton : stop get venc stream process.
******************************************************************************/
BD_S32 SAMPLE_COMM_VENC_StopGetStream()
{
	if (BD_TRUE == gs_stPara.bThreadStart)
	{
		gs_stPara.bThreadStart = BD_FALSE;
		pthread_join(gs_VencPid, 0);
	}
    return BD_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

