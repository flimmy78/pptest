/**
 * @file bd_internal.c
 * @brief BD(Blue Dragon) SDK implementaiton Copyright (C) PixelPlus - All Rights Reserved
 * Written by Jinho Yoo, Jan 2015
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>

//#include <cJSON.h>
//#include <curl/curl.h>

#include "bd_encoder.h"
#include "sample_comm.h"
#include "request_http.h"
#include "FrameSender.h"

#pragma pack(1)
struct video_data_header_for_udp{
	uint32_t data_size_byte;
	uint64_t timeStampHz;  //  Convert time stamp second to herz. 1sec=90000hz
	uint8_t reserved;  // 1 as default.
};
#pragma pack(0)

#define DEBUG_INFO //printf("%s:%d \n", __FILE__, __LINE__);

#define VIDEO_HEADER_SIZE sizeof(uint32_t)+sizeof(uint64_t)+sizeof(uint8_t)
#define UDP_SEND_MAX_SIZE	60000



//Set parameter after restart encoder H/W.
BD_S32 _set_channel_attribute(
		VENC_CHN_ATTR_S stVencChnAttr,
		int channel,
		PIC_SIZE_E video_size[],
		BD_APP_CONTEXT  *app_contexts,
	  int num_of_contexts){

	if(!app_contexts)
		return BD_FAILURE;

	if(channel > num_of_contexts)
		return BD_FAILURE;

	BD_APP_CONTEXT *app_context
		= &app_contexts[channel];

	BD_S32 ret;
	int i;

	//Stop video input.
	for(i=0; i<num_of_contexts; ++i){
		ret
			= bd_stop_video_input_and_encoding(
				&app_contexts[i]
			);
		if( BD_SUCCESS!= ret)
			return BD_FAILURE;
	}

	//Terminate H/W.
	bd_terminate_hw();


	//Initialize  encoder H/W.
	ret = bd_init_hw();
	if( ret != BD_SUCCESS){
			printf( "Init H/W Error! Error code = %d", ret);
			return BD_FAILURE;
	}

	//Re-configure video input.
	for(i=0; i<num_of_contexts; ++i){
	ret
		=	bd_conf_video_input(
			i,
			video_size[i], //{PIC_HD1088, PIC_HD720, PIC_VGA}
			&app_contexts[i]
		);
	if( BD_SUCCESS!= ret)
		return BD_FAILURE;
	}


	//Re-open video input.
	ret
		= bd_open_video_input_devices(
				app_contexts,
				num_of_contexts
			);
	if( BD_SUCCESS != ret)
		return BD_FAILURE;

  //Start to capture
	for(i=0; i<num_of_contexts; ++i){
	ret =	bd_start_to_capture(
			&app_contexts[i]
		);
	if( BD_SUCCESS!= ret)
		return BD_FAILURE;
	}

	//Re-init encoding
	for(i=0; i<num_of_contexts; ++i){

		printf( " [_init_encoder_with_param]= %d...%d", i, channel);


		if( i == channel){
			ret = _init_encoder_with_param(
				stVencChnAttr,
				channel,
				video_size,
				&app_contexts[i]
			);
		}else{
			ret
        = bd_init_encoder(
            PT_H264,
            BD_VBR,
            NULL,
            NULL,
            &app_contexts[i]
          );
		}

	if( BD_SUCCESS!= ret)
		return BD_FAILURE;
	}

	//Restart engoding.
	for(i=0; i<num_of_contexts; ++i){
	ret
		= bd_run_encoder(
 				&app_contexts[i]
			);
	if(BD_SUCCESS!= ret)
		return BD_FAILURE;
	}

}


BD_S32 _init_encoder_with_param(
    VENC_CHN_ATTR_S stVencChnAttr,
    int channel,
    PIC_SIZE_E video_size[],
    BD_APP_CONTEXT  *app_context
  ){

		BD_S32 ret
        = _VENC_Start_with_param(
						stVencChnAttr,
            app_context->vencGrp,
            app_context->vencChn,
            app_context->enPayLoad,
            VIDEO_ENCODING_MODE_NTSC,
            app_context->stViConfig.enPicSize,
            app_context->bitrateControl
        );

    if( BD_SUCCESS != ret )
        return BD_FAILURE;

    app_context->viDev
        = SAMPLE_COMM_VI_GetDev(
            app_context->viChn
        );

    ret
        = SAMPLE_COMM_VENC_BindVi(
        app_context->vencGrp,
        app_context->viDev,
        app_context->viChn);

    if( BD_SUCCESS != ret )
        return BD_FAILURE;

    return BD_SUCCESS;

}


BD_S32 _VENC_Start_with_param(
		VENC_CHN_ATTR_S vencChnAttr,
		VENC_GRP VencGrp,
		VENC_CHN VencChn,
		PAYLOAD_TYPE_E enType,
		VIDEO_NORM_E enNorm,
		PIC_SIZE_E enSize,
		SAMPLE_RC_E enRcMode)
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
	step 1: Greate Venc Group
	******************************************/
	SAMPLE_PRT("BD_MPI_VENC_CreateGroup =%d\n",VencGrp);

	s32Ret = BD_MPI_VENC_CreateGroup(VencGrp);


	if (BD_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("BD_MPI_VENC_CreateGroup[%d] failed with %#x!\n",\
				VencGrp, s32Ret);
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
			stH264Attr.u32BufSize  = stCapSize.u32Width * stCapSize.u32Height * 2;/*stream buffer size*/
			stH264Attr.u32Profile  = 0;/*0: baseline; 1:MP; 2:HP   ? */
			memcpy(&stVencChnAttr.stVeAttr.stAttrH264e, &stH264Attr, sizeof(VENC_ATTR_H264_S));

			if(SAMPLE_RC_CBR == enRcMode)
			{
				stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
				stH264Cbr.u32Gop            = vencChnAttr.stRcAttr.stAttrH264Cbr.u32Gop;
				stH264Cbr.u32ViFrmRate      = vencChnAttr.stRcAttr.stAttrH264Cbr.u32ViFrmRate;/* input (vi) frame rate */
				stH264Cbr.fr32TargetFrmRate = vencChnAttr.stRcAttr.stAttrH264Cbr.fr32TargetFrmRate;/* target frame rate */
				switch (enSize)
				{
					//case PIC_QCIF:
					//	stH264Cbr.u32BitRate = 256; /* average bit rate */
					//	break;
					case PIC_QVGA:    /* 320 * 240 */
					//case PIC_CIF:
						stH264Cbr.u32BitRate = 512;
						break;

					case PIC_D1:
					case PIC_VGA:	   /* 640 * 480 */
						stH264Cbr.u32BitRate = 768;
						break;
					case PIC_HD720:   /* 1280 * 720 */
						stH264Cbr.u32BitRate = 1024;
						break;
					case PIC_HD1080:  /* 1920 * 1080 */
						stH264Cbr.u32BitRate = 1024*2;
						break;
					case PIC_HD1088:  /* 1920 * 1080 */
						stH264Cbr.u32BitRate = 1024*4;
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
				stH264FixQp.u32Gop = vencChnAttr.stRcAttr.stAttrH264FixQp.u32Gop;
				stH264FixQp.u32ViFrmRate = vencChnAttr.stRcAttr.stAttrH264FixQp.u32ViFrmRate;
				stH264FixQp.fr32TargetFrmRate = vencChnAttr.stRcAttr.stAttrH264FixQp.fr32TargetFrmRate;
				stH264FixQp.u32IQp = 20;
				stH264FixQp.u32PQp = 23;
				memcpy(&stVencChnAttr.stRcAttr.stAttrH264FixQp, &stH264FixQp,sizeof(VENC_ATTR_H264_FIXQP_S));
			}
			else if (SAMPLE_RC_VBR == enRcMode)
			{
				stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264VBR;
				stH264Vbr.u32Gop = vencChnAttr.stRcAttr.stAttrH264FixQp.u32Gop;
				stH264Vbr.u32ViFrmRate = vencChnAttr.stRcAttr.stAttrH264FixQp.u32ViFrmRate;
				stH264Vbr.fr32TargetFrmRate = vencChnAttr.stRcAttr.stAttrH264FixQp.fr32TargetFrmRate;
				stH264Vbr.u32MinQp = 24;
				stH264Vbr.u32MaxQp = 32;
				switch (enSize)
				{
					//case PIC_QCIF:
					//	stH264Vbr.u32MaxBitRate= 256*3; /* average bit rate */
					//	break;
					case PIC_QVGA:    /* 320 * 240 */
					//case PIC_CIF:
						stH264Vbr.u32MaxBitRate = 512*3;
						break;
					case PIC_D1:
					case PIC_VGA:	   /* 640 * 480 */
						stH264Vbr.u32MaxBitRate = 1024*2*3;
						break;
					case PIC_HD720:   /* 1280 * 720 */
						stH264Vbr.u32MaxBitRate = 1024*3*3;
						break;
					case PIC_HD1080:  /* 1920 * 1080 */
					case PIC_HD1088:  /* 1920 * 1080 */
						stH264Vbr.u32MaxBitRate = 1024*6*3;
						break;
					default :
						stH264Vbr.u32MaxBitRate = 1024*4*3;
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
				stMjpegeFixQp.u32ViFrmRate      = vencChnAttr.stRcAttr.stAttrMjpegeFixQp.u32ViFrmRate;
				stMjpegeFixQp.fr32TargetFrmRate = vencChnAttr.stRcAttr.stAttrMjpegeFixQp.fr32TargetFrmRate;
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
			memcpy(&stVencChnAttr.stVeAttr.stAttrMjpeg, &stJpegAttr, sizeof(VENC_ATTR_MJPEG_S));
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


BD_VOID* encoding_thread_func(BD_VOID *p){


	VENC_CHN_ATTR_S stVencChnAttr;
	BD_S32 maxfd = 0;
	struct timeval TimeoutVal;
	fd_set read_fds;
	BD_S32 VencFd;

	char szFilePostfix[10];
	VENC_CHN_STAT_S stStat;
	VENC_STREAM_S stStream;
	BD_S32 s32Ret;
	VENC_CHN VencChn;
	PAYLOAD_TYPE_E enPayLoadType;

	BD_APP_CONTEXT *app_context = (BD_APP_CONTEXT*)p;
	app_context->run_thread = THREAD_RUN;

	/******************************************
	step 1:  check & prepare save-file & venc-fd
	******************************************/

	/* decide the stream file name, and open file to save stream */
	VencChn
		= app_context->viChn;
	s32Ret
		= BD_MPI_VENC_GetChnAttr(VencChn, &stVencChnAttr);
	if(s32Ret != BD_SUCCESS)
	{
		SAMPLE_PRT("BD_MPI_VENC_GetChnAttr chn[%d] failed with %#x!\n", \
		VencChn, s32Ret);
		return NULL;
	}
	enPayLoadType
		= stVencChnAttr.stVeAttr.enType;

	s32Ret
		= SAMPLE_COMM_VENC_GetFilePostfix(
			enPayLoadType,
			szFilePostfix
		);

	if(s32Ret != BD_SUCCESS)
	{
		SAMPLE_PRT(
			"SAMPLE_COMM_VENC_GetFilePostfix [%d] failed with %#x!\n",
			stVencChnAttr.stVeAttr.enType, s32Ret);
		return NULL;
	}

	/* Set Venc Fd. */
	VencFd
		= BD_MPI_VENC_GetFd(VencChn);
	if (VencFd < 0)
	{
		SAMPLE_PRT(
			"BD_MPI_VENC_GetFd failed with %#x!\n",
			VencFd);
		return NULL;
	}
	if (maxfd <= VencFd)
	{
		maxfd = VencFd;
	}


	/******************************************
	 step 2:  Start to get streams of each channel.
	******************************************/
	SAMPLE_PRT("Begin encoding thread. \n");
	while (THREAD_RUN == app_context->run_thread)
	{
		FD_ZERO(&read_fds);
		FD_SET(VencFd, &read_fds);

		TimeoutVal.tv_sec  = 2;
		TimeoutVal.tv_usec = 0;
		s32Ret
			= select(
				maxfd + 1,
				&read_fds,
				NULL,
				NULL,
				&TimeoutVal);
		if (s32Ret < 0)
		{
			SAMPLE_PRT("select failed!\n");
			break;
		}
		else if (s32Ret == 0)
		{
			SAMPLE_PRT("get venc stream time out, exit thread\n");
			continue;
		}
		else
		{
			if (FD_ISSET(VencFd, &read_fds))
			{
				/*******************************************************
				 step 2.1 : query how many packs in one-frame stream.
				*******************************************************/
				memset(&stStream, 0, sizeof(stStream));
				s32Ret
					= BD_MPI_VENC_Query(
						VencChn,
						&stStat);
				if (BD_SUCCESS != s32Ret)
				{
					SAMPLE_PRT(
						"BD_MPI_VENC_Query chn[%d] failed with %#x!\n",
						VencChn, s32Ret);
					break;
				}

				/*******************************************************
				 step 2.2 : malloc corresponding number of pack nodes.
				*******************************************************/
				stStream.pstPack
					= (VENC_PACK_S*)malloc(
						sizeof(VENC_PACK_S) * stStat.u32CurPacks
					);
				if (NULL == stStream.pstPack)
				{
					SAMPLE_PRT("malloc stream pack failed!\n");
					break;
				}

				/*******************************************************
				 step 2.3 : call mpi to get one-frame stream
				*******************************************************/
				stStream.u32PackCount = stStat.u32CurPacks;
				s32Ret
					= BD_MPI_VENC_GetStream(
						VencChn,
						&stStream,
						BD_TRUE
					);
				if (BD_SUCCESS != s32Ret)
				{
					free(stStream.pstPack);
					stStream.pstPack = NULL;
					SAMPLE_PRT(
							"BD_MPI_VENC_GetStream failed with %#x!\n",
							s32Ret);
					break;
				}

				//printf("[encoder => %d ... len = %d]\n", stStream.u32Seq, stStream.pstPack->u32Len);
			

				//Get current time stamp.
				struct timeval current_time;
				gettimeofday (&current_time, NULL);

				/*******************************************************
				 step 2.4 : send encoded frame to UDP.
				*******************************************************/
				if( NULL != app_context->udp_socket ){

					s32Ret
						= send_video_to_udp(
							current_time,
							app_context->udp_socket,
						 	&stStream);

					if (BD_SUCCESS != s32Ret){

						free(stStream.pstPack);
						stStream.pstPack = NULL;
						SAMPLE_PRT("save stream failed!\n");
						break;
					}
				}

				/*******************************************************
				 step 2.4 : send encoded frame to Shared memory.
				*******************************************************/
				if( NULL != app_context->shared_mem ){

					s32Ret
						= send_video_to_shared_mem(
							current_time,
							app_context->shared_mem,
							&stStream);
					if (BD_SUCCESS != s32Ret){

						free(stStream.pstPack);
						stStream.pstPack = NULL;
						SAMPLE_PRT("save stream failed!\n");
						break;
					}

				}

				/*******************************************************
				step 2.5 : release stream
				*******************************************************/
				s32Ret
					= BD_MPI_VENC_ReleaseStream(
						VencChn,
						&stStream
					);
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

	SAMPLE_PRT("Terminated encoding thread. \n");
}



