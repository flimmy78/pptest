/**
 * @file bd_encoder.c
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
#include "bd_internal.h"
#include "FrameSender.h"

#include "bd_api_aenc.h"

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

static void PrnFrameInfo(BD_S32 id, VENC_STREAM_S *strms)
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



BD_S32 bd_init_hw(){
    BD_S32 s32Ret = BD_FAILURE;

	s32Ret = SAMPLE_COMM_SYS_Init(NULL);
	s32Ret += BD_MPI_AI_INIT();
	s32Ret += BD_MPI_AO_INIT();


	s32Ret += BD_MPI_VENC_Init();
	BD_MPI_AENC_INIT();
	BD_MPI_ADEC_INIT();

	printf("[bd_init_hw]... ret = %d\n", s32Ret);

	return s32Ret;
}



BD_S32 bd_init_shared_mem(
		   int channel_id,
           const char *shared_memory_name,
           int size_in_byte,
           BD_SHARED_MEM *sharedMem){

    sharedMem->shm_name
		= shared_memory_name;
    sharedMem->shm_size
		= size_in_byte;
	sharedMem->shared_mem_handle
		= InitFrameSender(
			channel_id,
			size_in_byte,
			shared_memory_name);

    return BD_SUCCESS;

}


BD_S32 bd_init_UDP_socket(
           const char *address,
           int port,
           BD_UDP_SOCKET *udp_socket){

	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0)
	{
        printf("socket creation error!!");
        return BD_FAILURE;
	}

	udp_socket->client_addr = NULL;

	struct sockaddr_in  *clientAddr
	= (struct sockaddr_in *)malloc( sizeof(struct sockaddr_in ) );
	memset(clientAddr, 0, sizeof(struct sockaddr_in ));
	clientAddr->sin_family = AF_INET;
	clientAddr->sin_addr.s_addr = inet_addr(address);
	clientAddr->sin_port = htons(port);

	udp_socket->udp_socket_port = sock;
	udp_socket->client_addr=clientAddr;

  return BD_SUCCESS;
}


#if 0
BD_S32 SAMPLE_VI_FillConfig(
    SAMPLE_VI_CONFIG_S *pstViConfig,
    SAMPLE_VI_MODE_E enViMode,
    PIC_SIZE_E enPicSize,
    VIDEO_NORM_E enNorm,
    BD_S32 s32FrameRate,
    BD_U32 u32FrameDepth){

    if (!pstViConfig) {
        return BD_FAILURE;
    }

    pstViConfig->enPicSize = enPicSize;
    pstViConfig->enNorm = enNorm;
    pstViConfig->enRotate = ROTATE_NONE;
    pstViConfig->enViChnSet = VI_CHN_SET_NORMAL;
    pstViConfig->enViMode = enViMode;
	pstViConfig->s32FrameRate = s32FrameRate;
	pstViConfig->u32FrameDepth = u32FrameDepth;



    return BD_SUCCESS;
}
#endif

BD_S32 bd_conf_video_input(
    int channel_index,
    PIC_SIZE_E enSize,
    BD_APP_CONTEXT* app_context){



    app_context->viChn = channel_index;
    app_context->vencGrp = channel_index;
    app_context->vencChn = channel_index;
		app_context->run_thread = THREAD_STOP;

		BD_S32 ret;
		ret = SAMPLE_COMM_VI_FillConfig(
	            &app_context->stViConfig,
	            SENSOR_TYPE,
	            enSize,
	            VIDEO_ENCODING_MODE_AUTO, 30, 4);


    return ret;
}


BD_S32 bd_open_video_input_device(
    BD_APP_CONTEXT *app_context1,
    BD_APP_CONTEXT *app_context2,
    BD_APP_CONTEXT *app_context3){

    if( (NULL!=app_context1) &&
        (NULL==app_context2) &&
        (NULL==app_context3) ){

            return SAMPLE_COMM_VI_Init(
                app_context1->stViConfig.enViMode,
                VI_MODE_BUTT,
                VI_MODE_BUTT);

    }else if(
        (NULL!=app_context1) &&
        (NULL!=app_context2) &&
        (NULL==app_context3) ){

            return SAMPLE_COMM_VI_Init(
                app_context1->stViConfig.enViMode,
                app_context2->stViConfig.enViMode,
                VI_MODE_BUTT);
    }else if(
        (NULL!=app_context1) &&
        (NULL!=app_context2) &&
        (NULL!=app_context3) ){

            return SAMPLE_COMM_VI_Init(
                app_context1->stViConfig.enViMode,
                app_context2->stViConfig.enViMode,
                app_context3->stViConfig.enViMode);
    }




    return BD_FAILURE;
}

BD_S32 bd_start_to_capture(
    BD_APP_CONTEXT *app_context){

	SAMPLE_ISPADDON_CONFIG_S stISPAddonConfig;

	SAMPLE_COMM_ISP_FillConfig(&stISPAddonConfig, SENSOR_TYPE, BD_TRUE, BD_FALSE, BD_FALSE, BD_FALSE, BD_FALSE, BD_FALSE, BD_FALSE);

    return SAMPLE_COMM_VI_StartVi(
        app_context->viChn,
        &app_context->stViConfig,
        &stISPAddonConfig
    );

}


BD_S32 bd_init_encoder(
    PAYLOAD_TYPE_E codec,
    BDBitrateControl bitrate_control,
		BD_SHARED_MEM *shared_mem,
		BD_UDP_SOCKET *udp_socket,
    BD_APP_CONTEXT *app_context){

	BD_S32 s32Ret = BD_SUCCESS;


    app_context->enPayLoad = codec;
    app_context->bitrateControl = bitrate_control;
		app_context->shared_mem = shared_mem;

		if(app_context->udp_socket == NULL && udp_socket != NULL)
			app_context->udp_socket = udp_socket;


	s32Ret = BD_MPI_VENC_CreateGroup(app_context->vencGrp);
	if (BD_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("BD_MPI_VENC_CreateGroup[%d] failed with %#x!\n", app_context->vencGrp, s32Ret);
		return BD_FAILURE;
	}



    BD_S32 ret
        = SAMPLE_COMM_VENC_Start(
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

BD_S32 send_video_to_udp(
		struct timeval current_time,
		BD_UDP_SOCKET* udp_socket,
		VENC_STREAM_S *stream){

	BD_S32 i;
	struct timeval mytime;
	struct video_data_header_for_udp header;

	char sendData[VIDEO_HEADER_SIZE + UDP_SEND_MAX_SIZE + UDP_SEND_MAX_SIZE/5];

  for (i = 0; i < stream->u32PackCount; i++){

			int size_of_stream
				= stream->pstPack[i].u32Len;

			//Set header.
			header.data_size_byte
				= size_of_stream;
			header.timeStampHz
				= current_time.tv_sec * 90000 +
					current_time.tv_usec * 9 / 100;
			header.reserved = 1;

			int total_stream_size_byte_to_send = 0;

			//Send video stream via UDP. UDP cannot send stream under 60000 bytes.
			//So we need to check the size of video stream and
			while( total_stream_size_byte_to_send < size_of_stream ){

				int stream_size_byte_to_send = 0;

				if( (size_of_stream - total_stream_size_byte_to_send)
					> UDP_SEND_MAX_SIZE )
					stream_size_byte_to_send
						= UDP_SEND_MAX_SIZE;
				else
					stream_size_byte_to_send
						= size_of_stream
							- total_stream_size_byte_to_send;

				memcpy(sendData,
						(const char*)&header,
						VIDEO_HEADER_SIZE);
				memcpy(sendData + VIDEO_HEADER_SIZE,
						(const char*)(stream->pstPack[i].pu8Addr+total_stream_size_byte_to_send),
						stream_size_byte_to_send);

				int sent_stream_size
					= sendto(
						udp_socket->udp_socket_port,
						sendData,
						VIDEO_HEADER_SIZE + stream_size_byte_to_send,
						0,
						(struct sockaddr *)udp_socket->client_addr,
						sizeof(struct sockaddr)  );

				//printf("[send_video_to_udp => %d]\n", sent_stream_size);


				if( sent_stream_size < 0)
					break;
				else
					total_stream_size_byte_to_send
						+= stream_size_byte_to_send;
			}
    }

	return BD_SUCCESS;
}


#define MV_SY 9

BD_S32 send_video_to_shared_mem(
		struct timeval current_time,
		BD_SHARED_MEM* shared_mem,
		VENC_STREAM_S *stream){

	BD_S32 i;
	uint16_t mv[MV_SY] ={ 0, 0, 0, 0,
						  0, 0, 0, 0,
						  0 };

	for (i = 0; i < stream->u32PackCount; i++){

		SendFrame(
			(void*)shared_mem->channel_id,
			stream->pstPack[i].pu8Addr,
			current_time,
			stream->pstPack[i].u32Len,
			1,
			stream->stH264Info.u32IpcmMbNum,
			stream->stH264Info.enRefType,
			CODEC_TYPE_H264,
			mv
		);
	}

	return BD_FAILURE;
}


BD_FR32 bd_get_framerate(
		BD_APP_CONTEXT  *app_context
){

	if(!app_context)
		return -1;

	VENC_CHN_ATTR_S stVencChnAttr;
	BD_S32 s32Ret;
	s32Ret
		= BD_MPI_VENC_GetChnAttr(
				app_context->viChn,
				&stVencChnAttr
			);
	if(s32Ret != BD_SUCCESS){
		SAMPLE_PRT("BD_MPI_VENC_GetChnAttr chn[%d] failed with %#x!\n", \
		app_context->viChn, s32Ret);
		return -1;
	}

	//Get framerate from different stucture for each different video codec.
	VENC_RC_MODE_E mode = stVencChnAttr.stRcAttr.enRcMode;

	BD_FR32 framerate = 0.0;

	switch(mode){
		case VENC_RC_MODE_H264CBR:
			framerate = stVencChnAttr.stRcAttr.stAttrH264Cbr.fr32TargetFrmRate;
		break;

		case VENC_RC_MODE_H264VBR:
			framerate = stVencChnAttr.stRcAttr.stAttrH264Vbr.fr32TargetFrmRate;
		break;

		case VENC_RC_MODE_H264FIXQP:
			framerate = stVencChnAttr.stRcAttr.stAttrH264FixQp.fr32TargetFrmRate;
		break;

		case VENC_RC_MODE_MJPEGFIXQP:
			framerate = stVencChnAttr.stRcAttr.stAttrMjpegeFixQp.fr32TargetFrmRate;
		break;

		default:
		break;
	}

	return framerate;
}



BD_S32 bd_open_video_input_devices(
		BD_APP_CONTEXT *app_contexts,
		int num_of_contexts
	){
	BD_S32 ret = BD_FAILURE;
	switch( num_of_contexts ){
	  case 1:
	    ret
	      = bd_open_video_input_device(
	          &app_contexts[0],
	          NULL,
	          NULL
	        );
	  break;

	  case 2:
	    ret
	      = bd_open_video_input_device(
	          &app_contexts[0],
	          &app_contexts[1],
	          NULL
	        );

	  break;

	  case 3:
	    ret
	      = bd_open_video_input_device(
	          &app_contexts[0],
	          &app_contexts[1],
	          &app_contexts[2]
	    );
	  break;

	  default:
	    break;
	}

	if( BD_SUCCESS != ret ){
	    printf("Error: Fail to configure video input.");
			return BD_FAILURE;
	}

	return ret;
}



BD_S32 bd_set_framerate(
		BD_FR32 framerate,
		int channel,
		PIC_SIZE_E video_size[],
		BD_APP_CONTEXT  *app_contexts,
		int num_of_contexts
	){

	if(!app_contexts)
		return BD_FAILURE;

	BD_S32 ret;
	int i;

	//Get Channel attribute.
	if(channel > num_of_contexts)
		return BD_FAILURE;
	BD_APP_CONTEXT *app_context = &app_contexts[channel];
	VENC_CHN_ATTR_S stVencChnAttr;
	ret
		= BD_MPI_VENC_GetChnAttr(
				app_context->viChn,
				&stVencChnAttr
			);
	if(ret != BD_SUCCESS){
		SAMPLE_PRT("BD_MPI_VENC_GetChnAttr chn[%d] failed with %#x!\n", \
		app_context->viChn, ret);
		return -1;
	}

	//Set framebuffer value.
	VENC_RC_MODE_E mode = stVencChnAttr.stRcAttr.enRcMode;
	switch(mode){
		case VENC_RC_MODE_H264CBR:
			stVencChnAttr.stRcAttr.stAttrH264Cbr.fr32TargetFrmRate = framerate;
			stVencChnAttr.stRcAttr.stAttrH264Cbr.u32BitRate = framerate;
		break;

		case VENC_RC_MODE_H264VBR:
			stVencChnAttr.stRcAttr.stAttrH264Vbr.fr32TargetFrmRate = framerate;
			stVencChnAttr.stRcAttr.stAttrH264Vbr.u32ViFrmRate = framerate;
		break;

		case VENC_RC_MODE_H264FIXQP:
			stVencChnAttr.stRcAttr.stAttrH264FixQp.fr32TargetFrmRate = framerate;
			stVencChnAttr.stRcAttr.stAttrH264FixQp.u32ViFrmRate = framerate;
		break;

		case VENC_RC_MODE_MJPEGFIXQP:
			stVencChnAttr.stRcAttr.stAttrMjpegeFixQp.fr32TargetFrmRate = framerate;
			stVencChnAttr.stRcAttr.stAttrMjpegeFixQp.u32ViFrmRate = framerate;
		break;

		default:
		break;
	}

	return _set_channel_attribute(
			stVencChnAttr,
			channel,
			video_size,
			app_contexts,
			num_of_contexts);
}

BD_S32 bd_run_encoder(
	BD_APP_CONTEXT *app_context){

	app_context->run_thread = THREAD_RUN;

	return pthread_create(
		&app_context->mv_thread_id,
		0,
		encoding_thread_func,
		(BD_VOID*)app_context
	);





}

BD_S32 bd_wait_encoding_finish(
	 BD_APP_CONTEXT *app_context){

 	if(!app_context)
 		return BD_FAILURE;

	if(app_context->run_thread == THREAD_RUN){
		app_context->run_thread = THREAD_STOP;
		pthread_join(app_context->mv_thread_id, NULL);
		return BD_SUCCESS;

	}

	return BD_FAILURE;
}


BD_S32 bd_stop_video_input_and_encoding(
    BD_APP_CONTEXT  *app_context){

	BD_S32 s32Ret = BD_FAILURE;

	app_context->viDev
		= SAMPLE_COMM_VI_GetDev(app_context->vencChn);

	//Wait until thread is finished.
	s32Ret
		= bd_wait_encoding_finish(app_context);
	if( BD_SUCCESS != s32Ret){
		SAMPLE_PRT("failed with %#x!\n", s32Ret);
		return BD_FAILURE;
	}

	s32Ret = SAMPLE_COMM_VENC_UnBindVi(
	  app_context->vencGrp,
		app_context->viDev,
		app_context->viChn);

	if( BD_SUCCESS != s32Ret){
		SAMPLE_PRT("failed with %#x!\n", s32Ret);
		return BD_FAILURE;
	}

#if 1 /* 20160411 JACOB */
s32Ret = SAMPLE_COMM_VENC_Stop(app_context->viChn);
#else
	s32Ret = SAMPLE_COMM_VENC_Stop(
		  app_context->vencGrp,
			app_context->viChn);
#endif


	if( BD_SUCCESS != s32Ret){
		SAMPLE_PRT("failed with %#x!\n", s32Ret);
		return BD_FAILURE;
	}

	//Stop video encoding.
	s32Ret = SAMPLE_COMM_VI_StopVi(
				app_context->viChn,
				&(app_context->stViConfig));

	if( BD_SUCCESS!= s32Ret)
		return BD_FAILURE;

	return s32Ret;
}


int bd_get_bitrate(
		BD_APP_CONTEXT  *app_context){

	VENC_CHN_ATTR_S channel_attr;
	BD_S32 ret;

	ret = BD_MPI_VENC_GetChnAttr(
		app_context->viChn,
		&channel_attr
		);
	if (ret != BD_SUCCESS)
		return -1;

	int bitrate = 0;

	switch( channel_attr.stRcAttr.enRcMode ){

		case VENC_RC_MODE_H264CBR:
			bitrate
				= channel_attr.stRcAttr.stAttrH264Cbr.u32BitRate;
			break;

		case VENC_RC_MODE_H264VBR:
			bitrate
			 = channel_attr.stRcAttr.stAttrH264Vbr.u32MaxBitRate;
			 break;

		default:
			bitrate = -1;
			break;
	}

	return bitrate;
}

int bd_set_bitrate(
	int bitrate_value,
	BD_APP_CONTEXT  *app_context){


	VENC_CHN_ATTR_S channel_attr;
	BD_S32 ret;

	ret = BD_MPI_VENC_GetChnAttr(
		app_context->viChn,
		&channel_attr
		);

	if (ret != BD_SUCCESS)
		return -1;

		switch( channel_attr.stRcAttr.enRcMode ){

			case VENC_RC_MODE_H264CBR:
				channel_attr.stRcAttr.stAttrH264Cbr.u32BitRate
					= bitrate_value;
					break;

			case VENC_RC_MODE_H264VBR:
				channel_attr.stRcAttr.stAttrH264Vbr.u32MaxBitRate
					= bitrate_value;
					break;

			default:
				break;
		}


		ret = BD_MPI_VENC_SetChnAttr(
			app_context->viChn,
			&channel_attr
			);

		if (ret != BD_SUCCESS)
			return -1;


		return bd_get_bitrate(app_context);
}

int bd_get_GOP(
		BD_APP_CONTEXT  *app_context){

	VENC_CHN_ATTR_S channel_attr;
	BD_S32 ret;

	ret = BD_MPI_VENC_GetChnAttr(
		app_context->viChn,
		&channel_attr
		);
	if (ret != BD_SUCCESS)
		return -1;

	int GOP_val = 0;

	switch( channel_attr.stRcAttr.enRcMode ){

		case VENC_RC_MODE_H264CBR:
			GOP_val
				= channel_attr.stRcAttr.stAttrH264Cbr.u32Gop;
			break;

		case VENC_RC_MODE_H264VBR:
			GOP_val
			 = channel_attr.stRcAttr.stAttrH264Vbr.u32Gop;
			 break;

		default:
			GOP_val = -1;
			break;
	}

	return GOP_val;
}

BD_S32 bd_set_GOP(
  int GOPval,
  int channel,
  PIC_SIZE_E video_size[],
  BD_APP_CONTEXT  *app_contexts,
  int num_of_contexts){

	if(!app_contexts)
		return BD_FAILURE;

	BD_S32 ret;
	int i;

	//Get Channel attribute.
	if(channel > num_of_contexts)
		return BD_FAILURE;
	BD_APP_CONTEXT *app_context = &app_contexts[channel];
	VENC_CHN_ATTR_S stVencChnAttr;
	ret
		= BD_MPI_VENC_GetChnAttr(
				app_context->viChn,
				&stVencChnAttr
			);
	if(ret != BD_SUCCESS){
		SAMPLE_PRT("BD_MPI_VENC_GetChnAttr chn[%d] failed with %#x!\n", \
		app_context->viChn, ret);
		return -1;
	}

	//Set framebuffer value.
	VENC_RC_MODE_E mode = stVencChnAttr.stRcAttr.enRcMode;
	switch(mode){
		case VENC_RC_MODE_H264CBR:
			stVencChnAttr.stRcAttr.stAttrH264Cbr.u32Gop = GOPval;
		break;

		case VENC_RC_MODE_H264VBR:
			stVencChnAttr.stRcAttr.stAttrH264Vbr.u32Gop = GOPval;
		break;

		default:
		break;
	}

	return _set_channel_attribute(
			stVencChnAttr,
			channel,
			video_size,
			app_contexts,
			num_of_contexts);

}





PIC_SIZE_E get_video_size_enum(int width, int height){

	PIC_SIZE_E size_enum;

	switch(width){

		#if 0
		case 176:
			size_enum = PIC_QCIF;
			break;

		case 352:
			size_enum = PIC_CIF;
			break;
		#endif

		case 720:
			size_enum = PIC_D1;
			break;

		case 960:
			size_enum = PIC_960H;
			break;

		#if 0
		case 360:
			size_enum = PIC_2CIF;
			break;
		#endif
		case 320:
			size_enum = PIC_QVGA;
			break;

		case 640:
			size_enum = PIC_VGA;
			break;

		case 1024:
			size_enum = PIC_XGA;
			break;

		#if 0
		case 1400:
			size_enum = PIC_SXGA;
			break;

		case 1500:
			size_enum = PIC_UXGA;
			break;

		case 2048:
			size_enum = PIC_QXGA;
			break;

		case 854:
				size_enum = PIC_WVGA;
				break;

		case 1680:
				size_enum = PIC_WSXGA;
				break;

		case 2560:
				size_enum = PIC_WQXGA;
				break;
		#endif

		case 1280:
				size_enum = PIC_HD720;
				break;

		case 1920:
			if (1080==height)
				size_enum = PIC_HD1080;
			else if (1088==height)
				size_enum = PIC_HD1088;
			//else if (1200==height)
			//		size_enum = PIC_WUXGA;

			break;


		default:
			break;
	}

	return size_enum;
}



BD_S32 bd_set_resolution(
		int index,
		int width,
		int height,
		BD_APP_CONTEXT  *app_context){
/*
	BD_S32 ret;

	//To-Do:Get right value from given resolution value.
	PIC_SIZE_E enSize
		= get_video_size_enum(
				width,
				height
			);

	//Stop encoding thread.
	if( BD_SUCCESS
			!= bd_stop_encoding(
					app_context
				)
			)
		return BD_FAILURE;

	//Re-start encoding
	ret = BD_FAILURE;

	ret
		= bd_conf_video_input(
				index,
				enSize,
				app_context
			);


	ret = bd_start_to_capture(
			app_context
	);

	ret
		= bd_init_encoder(
				PT_H264,
				BD_VBR,
				NULL,
				app_context->udp_socket,
				app_context
			);

	ret
		= bd_run_encoder(
				app_context
			);
		*/
		return BD_SUCCESS;
}


void bd_terminate_hw(){

	BD_MPI_ADEC_EXIT();

	BD_MPI_AENC_EXIT();
	BD_MPI_VENC_Exit();

	BD_MPI_AO_EXIT();
	BD_MPI_AI_EXIT();

	SAMPLE_COMM_SYS_Exit();
}





/*======================================================================================================*/
/* AUDIO Processing */
/*======================================================================================================*/



#define SAMPLE_AUDIO_PTNUMPERFRM	480
static PAYLOAD_TYPE_E gs_enPayloadType = PT_G711U;//PT_G711U;//PT_G726;
static G726_BPS_E gs_enG726BPS = G726_16K; //G726_16K//G726_32K
#define SAMPLE_DBG(s32Ret)\
do{\
	printf("s32Ret=%#x,fuc:%s,line:%d\n", s32Ret, __FUNCTION__, __LINE__);\
}while(0)



/******************************************************************************
* function : PT Number to String
******************************************************************************/
static char* SAMPLE_AUDIO_Pt2Str(PAYLOAD_TYPE_E enType){
	if (PT_G711A == enType)	{
		return "g711a";
	}else if (PT_G711U == enType){
		return "g711u";
	}else if (PT_G726 == enType){
		return "g726";
	}else{
		return "data";
	}
}

/******************************************************************************
* function : Open Aenc file
******************************************************************************/
static FILE *SAMPLE_AUDIO_OpenAencOutFile(AENC_CHN AeChn, PAYLOAD_TYPE_E enType, G726_BPS_E enBps)
{
    FILE *pfd;
	BD_CHAR aszFileName[128];

	/* create file for save stream*/
	if(PT_G726 == enType)
		sprintf(aszFileName, "/opt/Aenc_chn%d_%s.%s", AeChn, G726_16K == enBps ? "16k" : "32k", SAMPLE_AUDIO_Pt2Str(enType));
	else
		sprintf(aszFileName, "/opt/Aenc_chn%d.%s", AeChn, SAMPLE_AUDIO_Pt2Str(enType));

	pfd = fopen(aszFileName, "wb");
	if (NULL == pfd)
	{
		printf("%s: open file %s failed\n", __FUNCTION__, aszFileName);
		return NULL;
	}
	printf("open stream file:\"%s\" for saving ok\n", aszFileName);
	return pfd;

}



BD_S32 SAMPLE_AUDIO_AiAenc(BD_VOID)
{
	BD_S32 s32Ret = BD_SUCCESS;
	AUDIO_DEV	AiDev = SAMPLE_AUDIO_AI_DEV;
	AI_CHN		AiChn;
	AUDIO_DEV	AoDev = SAMPLE_AUDIO_AO_DEV;
	AO_CHN		AoChn = 0;
	BD_S32		s32AiChnCnt;
	BD_S32		s32AencChnCnt;
	BD_S32		s32AoChnCnt;
	AENC_CHN	AeChn;
	ADEC_CHN	AdChn = 0;
	FILE		*pfd = NULL;
	AIO_ATTR_S stAioAttr;
	AENC_CHN_ATTR_S stAencAttr;
	AENC_ATTR_G726_S stAencG726;
	ADEC_CHN_ATTR_S stAdecAttr;
	ADEC_ATTR_G726_S stAdecG726;
	BD_BOOL bSendAdec = BD_TRUE;
	BD_BOOL bUserGetMode = BD_FALSE;
	BD_S32 i;

	stAioAttr.enSamplerate	 = AUDIO_SAMPLE_RATE_8000;
	stAioAttr.enBitwidth	 = AUDIO_BIT_WIDTH_16;
	stAioAttr.enWorkmode	 = AIO_MODE_I2S_SLAVE;//AIO_MODE_I2S_MASTER;
	stAioAttr.enSoundmode	 = AUDIO_SOUND_MODE_MONO;//AUDIO_SOUND_MODE_MONO;	 //AUDIO_SOUND_MODE_STEREO
	stAioAttr.u32FrmNum 	 = 30;
	stAioAttr.u32PtNumPerFrm = SAMPLE_AUDIO_PTNUMPERFRM;
	if(AUDIO_SOUND_MODE_MONO == stAioAttr.enSoundmode){
		stAioAttr.u32ChnCnt 	 = AIO_MAX_CHN_NUM;
	}else{
		stAioAttr.u32ChnCnt 	 = (AIO_MAX_CHN_NUM >> 1);
	}


	s32AiChnCnt = 1;//stAioAttr.u32ChnCnt;
	s32AencChnCnt = 1;
	s32AoChnCnt = 1;//stAioAttr.u32ChnCnt;


	stAencAttr.enType = gs_enPayloadType;
	stAencAttr.u32BufSize = 30;
	stAencAttr.u32PtNumPerFrm = SAMPLE_AUDIO_PTNUMPERFRM;

	stAdecAttr.enType = gs_enPayloadType;
	stAdecAttr.u32BufSize = 30;
	stAdecAttr.u32PtNumPerFrm = SAMPLE_AUDIO_PTNUMPERFRM;

	if(PT_G711A == gs_enPayloadType || PT_G711U == gs_enPayloadType){
		stAencAttr.pValue = NULL;

		stAdecAttr.pValue = NULL;
	}else if(PT_G726 == gs_enPayloadType){
		stAencAttr.pValue = &stAencG726;
		stAencG726.enG726bps = gs_enG726BPS;

		stAdecAttr.pValue = &stAdecG726;
		stAdecG726.enG726bps = gs_enG726BPS;
	}else{
		printf("%s: invalid aenc payload type:%d\n", __FUNCTION__, gs_enPayloadType);
		return BD_FAILURE;
	}

	s32Ret = SAMPLE_COMM_AUDIO_StartAi(AiDev, s32AiChnCnt, &stAioAttr);
	if (s32Ret != BD_SUCCESS){
		SAMPLE_DBG(s32Ret);
		return BD_FAILURE;
	}

	s32Ret = SAMPLE_COMM_AUDIO_StartAenc(s32AencChnCnt, &stAencAttr);
	if (s32Ret != BD_SUCCESS){
		SAMPLE_DBG(s32Ret);
		return BD_FAILURE;
	}


	for(i=0;i<s32AencChnCnt;i++){
		AeChn = i;
		AiChn = i;
		if(BD_TRUE == bUserGetMode){
			s32Ret = SAMPLE_COMM_AUDIO_CreatTrdAiAenc(AiDev, AiChn, AeChn, BD_FALSE);
			if (s32Ret != BD_SUCCESS){
				SAMPLE_DBG(s32Ret);
				return BD_FAILURE;
			}
		}else{
			s32Ret = SAMPLE_COMM_AUDIO_AencBindAi(AiDev, AiChn, AeChn);
			if (s32Ret != BD_SUCCESS){
				SAMPLE_DBG(s32Ret);
				return s32Ret;
			}
		}
		printf("Ai(%d,%d) bind to AencChn:%d ok!\n",AiDev , AiChn, AeChn);
	}

	if(BD_TRUE == bSendAdec){
		// Start Adec
		s32Ret = SAMPLE_COMM_AUDIO_StartAdec(AdChn, &stAdecAttr);
		if(BD_SUCCESS != s32Ret){
			SAMPLE_DBG(s32Ret);
			return BD_FAILURE;

		}

		s32Ret = SAMPLE_COMM_AUDIO_StartAo(AoDev, s32AoChnCnt, &stAioAttr);
		if (BD_SUCCESS != s32Ret)
		{
			SAMPLE_DBG(s32Ret);
			return BD_FAILURE;
		}
		pfd = SAMPLE_AUDIO_OpenAencOutFile(AeChn, gs_enPayloadType, gs_enG726BPS);
		if(BD_NULL == pfd){
			SAMPLE_DBG(BD_FAILURE);
			return BD_FAILURE;
		}

		// Get encoded stream and save in file.
		// Send encoded stream to Adec
		s32Ret = SAMPLE_COMM_AUDIO_CreatTrdAencAdec(AeChn, AdChn, pfd);
		if (s32Ret != BD_SUCCESS)
		{
			SAMPLE_DBG(s32Ret);
			return BD_FAILURE;
		}

		// Bind Adec Ao
		s32Ret = SAMPLE_COMM_AUDIO_AoBindAdec(AoDev, AoChn, AdChn);
		if (s32Ret != BD_SUCCESS){
			SAMPLE_DBG(s32Ret);
			return BD_FAILURE;
		}

	}



	return BD_SUCCESS;


	printf("\nplease press twice ENTER to exit this sample\n");
	getchar();
	getchar();

	/********************************************
			step 6: exit the process
		********************************************/

	if(BD_TRUE == bSendAdec){
		s32Ret = SAMPLE_COMM_AUDIO_DestoryTrdAencAdec(AeChn);
		if (s32Ret != BD_SUCCESS){
			SAMPLE_DBG(s32Ret);
			return BD_FAILURE;
		}

		s32Ret = SAMPLE_COMM_AUDIO_StopAo(AoDev, s32AoChnCnt);
		if (s32Ret != BD_SUCCESS){
			SAMPLE_DBG(s32Ret);
			return BD_FAILURE;
		}
		s32Ret = SAMPLE_COMM_AUDIO_StopAdec(AdChn);
		if (s32Ret != BD_SUCCESS){
			SAMPLE_DBG(s32Ret);
			return BD_FAILURE;
		}

		s32Ret = SAMPLE_COMM_AUDIO_AoUnbindAdec(AoDev, AoChn, AdChn);
		if (s32Ret != BD_SUCCESS){
			SAMPLE_DBG(s32Ret);
			return BD_FAILURE;
		}

	}

	for(i=0;i<s32AencChnCnt;i++){
		AeChn = i;
		AiChn = i;
		if(BD_TRUE == bUserGetMode){
			s32Ret = SAMPLE_COMM_AUDIO_DestoryTrdAi(AiDev, AiChn);
			if (s32Ret != BD_SUCCESS){
				SAMPLE_DBG(s32Ret);
				return BD_FAILURE;
			}
		}else{
			s32Ret = SAMPLE_COMM_AUDIO_AencUnbindAi(AiDev, AiChn, AeChn);
			if (s32Ret != BD_SUCCESS){
				SAMPLE_DBG(s32Ret);
				return BD_FAILURE;
			}
		}
	}
	s32Ret = SAMPLE_COMM_AUDIO_StopAenc(s32AencChnCnt);
	if (s32Ret != BD_SUCCESS){
		SAMPLE_DBG(s32Ret);
		return BD_FAILURE;
	}

	s32Ret = SAMPLE_COMM_AUDIO_StopAi(AiDev, s32AiChnCnt);
	if (s32Ret != BD_SUCCESS){
		SAMPLE_DBG(s32Ret);
		return BD_FAILURE;
	}

	return BD_SUCCESS;
}
