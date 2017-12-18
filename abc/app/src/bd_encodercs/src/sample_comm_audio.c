#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>


#include <sys/ipc.h>
#include <sys/shm.h>

#include "sample_comm.h"

#define TIME_MEASURE	0
#define  KEY_NUM     	9527
#define  MEM_SIZE    	480


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct tagSAMPLE_AI_S
{
    BD_BOOL bStart;
    BD_S32  AiDev;
    BD_S32  AiChn;
    BD_S32  AencChn;
    BD_S32  AoDev;
    BD_S32  AoChn;
    BD_BOOL bSendAenc;
    BD_BOOL bSendAo;
    pthread_t stAiPid;
} SAMPLE_AI_S;

typedef struct tagSAMPLE_AENC_S
{
    BD_BOOL bStart;
    pthread_t stAencPid;
    BD_S32  AeChn;
	BD_S32	AdChn;
    FILE    *pfd_in;
	FILE	*pfd_out;
	BD_BOOL bSendAdChn;
} SAMPLE_AENC_S;

typedef struct tagSAMPLE_ADEC_S
{
    BD_BOOL bStart;
    BD_S32 AdChn; 
    FILE *pfd_in;
	FILE *pfd_out;
    pthread_t stAdPid;
	
} SAMPLE_ADEC_S;

typedef struct tagSAMPLE_AO_S
{
    BD_BOOL bStart;
	BD_S32 AoDev;
    BD_S32 AoChn; 
    FILE *pfd_in;
    pthread_t stAoPid;
	
} SAMPLE_AO_S;


static SAMPLE_AI_S   gs_stSampleAi[AI_DEV_MAX_NUM*AIO_MAX_CHN_NUM];

static SAMPLE_AENC_S gs_stSampleAenc[AENC_MAX_CHN_NUM];
static AUDIO_FRAME_S gs_stFrameAenc[AENC_MAX_CHN_NUM];

static SAMPLE_ADEC_S gs_stSampleAdec[ADEC_MAX_CHN_NUM];
static AUDIO_STREAM_S gs_stStreamAdec[ADEC_MAX_CHN_NUM];

static SAMPLE_AO_S gs_stSampleAo[AI_DEV_MAX_NUM*AIO_MAX_CHN_NUM];
static AUDIO_FRAME_S gs_stFrameAo[AI_DEV_MAX_NUM*AIO_MAX_CHN_NUM];


/******************************************************************************
* function : get frame from Ai, send it  to Aenc or Ao
******************************************************************************/
void *SAMPLE_COMM_AUDIO_AiProc(void *parg)
{
    BD_S32 s32Ret;
    BD_S32 AiFd;
    SAMPLE_AI_S *pstAiCtl = (SAMPLE_AI_S *)parg;
    AUDIO_FRAME_S stFrame; 
    fd_set read_fds;
    struct timeval TimeoutVal;
#if 0	
	FILE *fp;
	fp = fopen("Apcm_chn0_rec.pcm","wb");
#endif	
//    AI_CHN_PARAM_S stAiChnPara;
/*
    s32Ret = BD_MPI_AI_GetChnParam(pstAiCtl->AiDev, pstAiCtl->AiChn, &stAiChnPara);
    if (BD_SUCCESS != s32Ret)
    {
        printf("%s: Get ai chn param failed\n", __FUNCTION__);
        return NULL;
    }
    
    stAiChnPara.u32UsrFrmDepth = 30;
    
    s32Ret = BD_MPI_AI_SetChnParam(pstAiCtl->AiDev, pstAiCtl->AiChn, &stAiChnPara);
    if (BD_SUCCESS != s32Ret)
    {
        printf("%s: set ai chn param failed\n", __FUNCTION__);
        return NULL;
    }
  */  
    FD_ZERO(&read_fds);
    AiFd = BD_MPI_AI_GetFd(pstAiCtl->AiDev, pstAiCtl->AiChn);
    FD_SET(AiFd,&read_fds);

    while (pstAiCtl->bStart)
    {     
        TimeoutVal.tv_sec = 1;
        TimeoutVal.tv_usec = 0;
        
        FD_ZERO(&read_fds);
        FD_SET(AiFd,&read_fds);
        
        s32Ret = select(AiFd+1, &read_fds, NULL, NULL, &TimeoutVal);
        if (s32Ret < 0) 
        {
            break;
        }
        else if (0 == s32Ret) 
        {
            printf("%s: get ai frame select time out\n", __FUNCTION__);
            break;
        }        
        
        if (FD_ISSET(AiFd, &read_fds))
        {
            /* get frame from ai chn */
            s32Ret = BD_MPI_AI_GetFrame(pstAiCtl->AiDev, pstAiCtl->AiChn, &stFrame, 0);	// 0: non block
            if (BD_SUCCESS != s32Ret )
            {
            	#if 0
                printf("%s: BD_MPI_AI_GetFrame(%d, %d), failed with %#x!\n",\
                       __FUNCTION__, pstAiCtl->AiDev, pstAiCtl->AiChn, s32Ret);
                pstAiCtl->bStart = BD_FALSE;
                return NULL;
				#else
				continue;
				#endif
            }

#if 0	// save file

			fwrite(stFrame.pVirAddr[0], 1, stFrame.u32Len, fp);
#endif


            /* send frame to encoder */
            if (BD_TRUE == pstAiCtl->bSendAenc)
            {
                s32Ret = BD_MPI_AENC_SendFrame(pstAiCtl->AencChn, &stFrame);
                if (BD_SUCCESS != s32Ret )
                {
                    printf("%s: BD_MPI_AENC_SendFrame(%d), failed with %#x!\n",\
                           __FUNCTION__, pstAiCtl->AencChn, s32Ret);
                    pstAiCtl->bStart = BD_FALSE;
                    return NULL;
                }
            }
            
            /* send frame to ao */
            if (BD_TRUE == pstAiCtl->bSendAo)
            {
                s32Ret = BD_MPI_AO_SendFrame(pstAiCtl->AoDev, pstAiCtl->AoChn, &stFrame, 1000);
                if (BD_SUCCESS != s32Ret )
                {
                    printf("%s: BD_MPI_AO_SendFrame(%d, %d), failed with %#x!\n",\
                           __FUNCTION__, pstAiCtl->AoDev, pstAiCtl->AoChn, s32Ret);
                    pstAiCtl->bStart = BD_FALSE;
                    return NULL;
                }
                
            }

            /* finally you must release the stream */
            s32Ret = BD_MPI_AI_ReleaseFrame(pstAiCtl->AiDev, pstAiCtl->AiChn, &stFrame);
            if (BD_SUCCESS != s32Ret )
            {
                printf("%s: BD_MPI_AI_ReleaseFrame(%d, %d), failed with %#x!\n",\
                       __FUNCTION__, pstAiCtl->AiDev, pstAiCtl->AiChn, s32Ret);
                pstAiCtl->bStart = BD_FALSE;
                return NULL;
            }
            
        }
    }
#if 0
	fclose(fp);
#endif
    pstAiCtl->bStart = BD_FALSE;
    return NULL;
}

/******************************************************************************
* function : get frame from Ai, save it into file
******************************************************************************/
void *SAMPLE_COMM_AUDIO_AiRecProc(void *parg)
{
    BD_S32 s32Ret;
    BD_S32 AiFd;
    SAMPLE_AI_S *pstAiCtl = (SAMPLE_AI_S *)parg;
    AUDIO_FRAME_S stFrame; 
    fd_set read_fds;
    struct timeval TimeoutVal;
	BD_CHAR szFileName[256];
	FILE *fp;

	sprintf(szFileName, "Apcm_chn%d.pcm", pstAiCtl->AiChn);
	fp = fopen(szFileName,"wb");
	if(fp == BD_NULL){
		printf("open stream file: \"%s\" for saving failed\n", szFileName);
	}else{
		printf("open stream file: \"%s\" for saving ok\n", szFileName);
	}

    FD_ZERO(&read_fds);
    AiFd = BD_MPI_AI_GetFd(pstAiCtl->AiDev, pstAiCtl->AiChn);
    FD_SET(AiFd,&read_fds);

    while (pstAiCtl->bStart)
    {     
        TimeoutVal.tv_sec = 1;
        TimeoutVal.tv_usec = 0;
        
        FD_ZERO(&read_fds);
        FD_SET(AiFd,&read_fds);
        
        s32Ret = select(AiFd+1, &read_fds, NULL, NULL, &TimeoutVal);
        if (s32Ret < 0) 
        {
            break;
        }
        else if (0 == s32Ret) 
        {
            printf("%s: get ai frame select time out\n", __FUNCTION__);
            break;
        }        
        
        if (FD_ISSET(AiFd, &read_fds))
        {
            /* get frame from ai chn */
            s32Ret = BD_MPI_AI_GetFrame(pstAiCtl->AiDev, pstAiCtl->AiChn, &stFrame, 0);	// 0: non block
            if (BD_SUCCESS != s32Ret )
            {
            	#if 0
                printf("%s: BD_MPI_AI_GetFrame(%d, %d), failed with %#x!\n",\
                       __FUNCTION__, pstAiCtl->AiDev, pstAiCtl->AiChn, s32Ret);
                pstAiCtl->bStart = BD_FALSE;
                return NULL;
				#else
				continue;
				#endif
            }



			fwrite(stFrame.pVirAddr[0], 1, stFrame.u32Len, fp);

            /* finally you must release the stream */
            s32Ret = BD_MPI_AI_ReleaseFrame(pstAiCtl->AiDev, pstAiCtl->AiChn, &stFrame);
            if (BD_SUCCESS != s32Ret )
            {
                printf("%s: BD_MPI_AI_ReleaseFrame(%d, %d), failed with %#x!\n",\
                       __FUNCTION__, pstAiCtl->AiDev, pstAiCtl->AiChn, s32Ret);
                pstAiCtl->bStart = BD_FALSE;
                return NULL;
            }
            
        }
    }
	fclose(fp);
    pstAiCtl->bStart = BD_FALSE;

    return NULL;
}


/******************************************************************************
* function : get stream from Aenc, send it  to Adec & save it to file
******************************************************************************/
void *SAMPLE_COMM_AUDIO_AencProc(void *parg)
{
    BD_S32 s32Ret;
    SAMPLE_AENC_S *pstAencCtl = (SAMPLE_AENC_S *)parg;
    AUDIO_STREAM_S stStream;
   	BD_S32 shm_id;
	  BD_VOID *shm_addr;

	  if ( -1 == ( shm_id = shmget( (key_t)KEY_NUM, MEM_SIZE, IPC_CREAT|0666)))
	    printf( "share mem create fail...\n");
	
	  if ( ( void *)-1 == ( shm_addr = shmat( shm_id, ( void *)0, 0)))
	    printf( "share mem add fail...\n");

    
    while (pstAencCtl->bStart)
    {     

		s32Ret = BD_MPI_AENC_GetStream(pstAencCtl->AeChn, &stStream, -1);
		if (BD_SUCCESS != s32Ret )
		{
			printf("%s: BD_MPI_AENC_GetStream(%d), failed with %#x!\n",\
				   __FUNCTION__, pstAencCtl->AeChn, s32Ret);
			pstAencCtl->bStart = BD_FALSE;
			return NULL;
		}

		if (BD_TRUE == pstAencCtl->bSendAdChn)
		{
			s32Ret = BD_MPI_ADEC_SendStream(pstAencCtl->AdChn, &stStream, BD_TRUE);
			if (BD_SUCCESS != s32Ret )
			{
				printf("%s: BD_MPI_ADEC_SendStream(%d), failed with %#x!\n",\
					   __FUNCTION__, pstAencCtl->AdChn, s32Ret);
				pstAencCtl->bStart = BD_FALSE;
				return NULL;
			}
		}
		//printf("[%s] Get Stream. Size: %d\n", __FUNCTION__, stStream.u32Len);
		memcpy( (unsigned char *)shm_addr, stStream.pStream[0], MEM_SIZE);
		
		/* finally you must release the stream */
		s32Ret = BD_MPI_AENC_ReleaseStream(pstAencCtl->AeChn, &stStream);
		if (BD_SUCCESS != s32Ret )
		{
			printf("%s: BD_MPI_AENC_ReleaseStream(%d), failed with %#x!\n",\
				   __FUNCTION__, pstAencCtl->AeChn, s32Ret);
			pstAencCtl->bStart = BD_FALSE;
			return NULL;
		}

    }
    
    fclose(pstAencCtl->pfd_out);
    pstAencCtl->bStart = BD_FALSE;
	
    return NULL;
}


/******************************************************************************
* function : get frame from file, send it  to Aenc & save it to file
******************************************************************************/
void *SAMPLE_COMM_AUDIO_AencFileProc(void *parg)
{
    BD_S32 s32Ret;
    SAMPLE_AENC_S *pstAencCtl = (SAMPLE_AENC_S *)parg;
	AUDIO_STREAM_S stStream;
	struct timeval CurrTime;
#if TIME_MEASURE
	BD_U64 u64TimeB;
	BD_U64 u64TimeE;
	BD_U64 u64TimeSum = 0;
	BD_U32 u32ByteSum = 0;
#endif


	printf("start thread[%s]\n", __FUNCTION__);
    while (pstAencCtl->bStart)
    {     

		if(feof(pstAencCtl->pfd_in) == 1){
			printf("End of file. exit\n");
			break;
		}
		fread(gs_stFrameAenc[pstAencCtl->AeChn].pVirAddr[0], 1, gs_stFrameAenc[pstAencCtl->AeChn].u32Len, pstAencCtl->pfd_in);
		gettimeofday(&CurrTime, NULL);
		//gs_stFrameAenc[pstAencCtl->AeChn].u64TimeStamp = (BD_U64)CurrTime.tv_sec * 1000000ULL + (BD_U64)CurrTime.tv_usec;
		memcpy(&gs_stFrameAenc[pstAencCtl->AeChn].u64TimeStamp, &CurrTime, sizeof(BD_U64));
		gs_stFrameAenc[pstAencCtl->AeChn].u32Seq++;

#if TIME_MEASURE
		u64TimeB = (BD_U64)CurrTime.tv_sec * 1000000ULL + (BD_U64)CurrTime.tv_usec;
#endif
		s32Ret = BD_MPI_AENC_SendFrame(pstAencCtl->AeChn, &gs_stFrameAenc[pstAencCtl->AeChn]);
        if (BD_SUCCESS != s32Ret )
        {
            printf("%s: BD_MPI_AENC_SendFrame(%d), failed with %#x!\n",\
                   __FUNCTION__, pstAencCtl->AeChn, s32Ret);
            pstAencCtl->bStart = BD_FALSE;
            return NULL;
        }
#if TIME_MEASURE
		gettimeofday(&CurrTime, NULL);
		u64TimeE = (BD_U64)CurrTime.tv_sec * 1000000ULL + (BD_U64)CurrTime.tv_usec;
		u64TimeSum += (u64TimeE - u64TimeB);
		u32ByteSum += gs_stFrameAenc[pstAencCtl->AeChn].u32Len;
#endif

		/* get stream from aenc chn */
		s32Ret = BD_MPI_AENC_GetStream(pstAencCtl->AeChn, &stStream, -1);	// block mode
		if (BD_SUCCESS != s32Ret )
		{
			printf("%s: BD_MPI_AENC_GetStream(%d), failed with %#x!\n",\
				   __FUNCTION__, pstAencCtl->AeChn, s32Ret);
			pstAencCtl->bStart = BD_FALSE;
			return NULL;
		}


		/* save audio stream to file */
		fwrite(stStream.pStream[0],1,stStream.u32Len, pstAencCtl->pfd_out);

		s32Ret = BD_MPI_AENC_ReleaseStream(pstAencCtl->AeChn, &stStream);
		if (BD_SUCCESS != s32Ret )
		{
			printf("%s: BD_MPI_AENC_ReleaseStream(%d), failed with %#x!\n",\
				   __FUNCTION__, pstAencCtl->AeChn, s32Ret);
			pstAencCtl->bStart = BD_FALSE;
			return NULL;
		}
		//usleep(1);
    }

#if TIME_MEASURE
	printf("Enc time: %d.%06d, Total bytes: %d bytes\n", (BD_U32)(u64TimeSum/1000000LL), (BD_U32)(u64TimeSum%1000000LL), u32ByteSum);
#endif

	fclose(pstAencCtl->pfd_in);
	fclose(pstAencCtl->pfd_out);
	pstAencCtl->bStart = BD_FALSE;
	
	return NULL;
		
}

/******************************************************************************
* function : get stream from file, send it  to Adec & save it to file
******************************************************************************/
void *SAMPLE_COMM_AUDIO_AdecFileProc(void *parg)
{
    BD_S32 s32Ret;
    SAMPLE_ADEC_S *pstAdecCtl = (SAMPLE_ADEC_S *)parg;
	AUDIO_FRAME_S stFrame;
	struct timeval CurrTime;

    while (pstAdecCtl->bStart)
    {     

		if(feof(pstAdecCtl->pfd_in) == 1){
			printf("End of file. exit\n");
			break;
		}
		fread(gs_stStreamAdec[pstAdecCtl->AdChn].pStream[0], 1, gs_stStreamAdec[pstAdecCtl->AdChn].u32Len, pstAdecCtl->pfd_in);
		gettimeofday(&CurrTime, NULL);
		gs_stFrameAenc[pstAdecCtl->AdChn].u64TimeStamp = (BD_U64)CurrTime.tv_sec * 1000000ULL + (BD_U64)CurrTime.tv_usec;
		gs_stFrameAenc[pstAdecCtl->AdChn].u32Seq++;

		s32Ret = BD_MPI_ADEC_SendStream(pstAdecCtl->AdChn, &gs_stStreamAdec[pstAdecCtl->AdChn], BD_TRUE);
        if (BD_SUCCESS != s32Ret )
        {
            printf("%s: BD_MPI_ADEC_SendStream(%d), failed with %#x!\n",\
                   __FUNCTION__, pstAdecCtl->AdChn, s32Ret);
            pstAdecCtl->bStart = BD_FALSE;
            return NULL;
        }

		if(pstAdecCtl->pfd_out){
			/* get stream from aenc chn */
			s32Ret = BD_MPI_ADEC_GetFrame(pstAdecCtl->AdChn, &stFrame);
			if (BD_SUCCESS != s32Ret )
			{
				printf("%s: BD_MPI_ADEC_GetStream(%d), failed with %#x!\n",\
					   __FUNCTION__, pstAdecCtl->AdChn, s32Ret);
				pstAdecCtl->bStart = BD_FALSE;
				return NULL;
			}


			/* save audio stream to file */
			fwrite(stFrame.pVirAddr[0], 1, stFrame.u32Len, pstAdecCtl->pfd_out);

			s32Ret = BD_MPI_ADEC_ReleaseFrame(pstAdecCtl->AdChn, &stFrame);
			if (BD_SUCCESS != s32Ret )
			{
				printf("%s: BD_MPI_ADEC_ReleaseFrame(%d), failed with %#x!\n",\
					   __FUNCTION__, pstAdecCtl->AdChn, s32Ret);
				pstAdecCtl->bStart = BD_FALSE;
				return NULL;
			}
		}
		usleep(1);
    }

	if(pstAdecCtl->pfd_in)fclose(pstAdecCtl->pfd_in);
	if(pstAdecCtl->pfd_out)fclose(pstAdecCtl->pfd_out);
	pstAdecCtl->bStart = BD_FALSE;
	
	return NULL;

}

/******************************************************************************
* function : get pcm audio from file, send it to Ao
******************************************************************************/
void *SAMPLE_COMM_AUDIO_RecAoProc(void *parg)
{
    BD_S32 s32Ret;
    SAMPLE_AO_S *pstAoCtl = (SAMPLE_AO_S *)parg;
	AUDIO_FRAME_S *pstFrame;
	struct timeval CurrTime;

	pstFrame = (AUDIO_FRAME_S *)&gs_stFrameAo[pstAoCtl->AoDev*AIO_MAX_CHN_NUM + pstAoCtl->AoChn];

    while (pstAoCtl->bStart)
    {     

		if(feof(pstAoCtl->pfd_in) == 1){
			printf("End of file. exit\n");
			break;
		}
		fread(pstFrame->pVirAddr[0], 1, pstFrame->u32Len, pstAoCtl->pfd_in);
		gettimeofday(&CurrTime, NULL);
		pstFrame->u64TimeStamp = (BD_U64)CurrTime.tv_sec * 1000000ULL + (BD_U64)CurrTime.tv_usec;
		pstFrame->u32Seq++;

		
		s32Ret = BD_MPI_AO_SendFrame(pstAoCtl->AoDev, pstAoCtl->AoChn, pstFrame, -1);
		if (BD_SUCCESS != s32Ret )
		{
			printf("%s: BD_MPI_AO_SendFrame(%d, %d), failed with %#x!\n",\
				   __FUNCTION__, pstAoCtl->AoDev, pstAoCtl->AoChn, s32Ret);
			pstAoCtl->bStart = BD_FALSE;
			return NULL;
		}

		usleep(1);
    }

	if(pstAoCtl->pfd_in)fclose(pstAoCtl->pfd_in);
	pstAoCtl->bStart = BD_FALSE;
	return NULL;

}







/******************************************************************************
* function : Start Ai
******************************************************************************/
BD_S32 SAMPLE_COMM_AUDIO_StartAi(AUDIO_DEV AiDevId, BD_S32 s32AiChnCnt, AIO_ATTR_S *pstAioAttr)
{
    BD_S32 i;
    BD_S32 s32Ret;
	
    s32Ret = BD_MPI_AI_SetPubAttr(AiDevId, pstAioAttr);
    if (s32Ret)
    {
        printf("%s: BD_MPI_AI_SetPubAttr(%d) failed with %#x\n", __FUNCTION__, AiDevId, s32Ret);
        return s32Ret;
    }

    s32Ret = BD_MPI_AI_Enable(AiDevId);
	if (s32Ret)
    {
        printf("%s: BD_MPI_AI_Enable(%d) failed with %#x\n", __FUNCTION__, AiDevId, s32Ret);
        return s32Ret;
    }   

	
    for (i=0; i<s32AiChnCnt; i++)
    {
        s32Ret = BD_MPI_AI_EnableChn(AiDevId, i);
		if (s32Ret)
        {
            printf("%s: BD_MPI_AI_EnableChn(%d,%d) failed with %#x\n", __FUNCTION__, AiDevId, i, s32Ret);
            return s32Ret;
        }        

    }

	BD_MPI_AI_SetADCVolume(AiDevId, 0);
   
    return BD_SUCCESS;
}


/******************************************************************************
* function : Stop Ai
******************************************************************************/
BD_S32 SAMPLE_COMM_AUDIO_StopAi(AUDIO_DEV AiDevId, BD_S32 s32AiChnCnt)
{
    BD_S32 i; 
    BD_S32 s32Ret;

	
    for (i=0; i<s32AiChnCnt; i++)
    {
        s32Ret = BD_MPI_AI_DisableChn(AiDevId, i);
        if(BD_SUCCESS != s32Ret)
        {
            printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "failed");
            return s32Ret;
        }
    }  
	
    s32Ret = BD_MPI_AI_Disable(AiDevId);
    if(BD_SUCCESS != s32Ret)
    {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "failed");
        return s32Ret;
    }

	s32Ret = BD_MPI_AI_ClrPubAttr(AiDevId);
    if(BD_SUCCESS != s32Ret)
    {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "failed");
        return s32Ret;
    }
	
    
    return BD_SUCCESS;
}

/******************************************************************************
* function : Start Ao
******************************************************************************/
BD_S32 SAMPLE_COMM_AUDIO_StartAo(AUDIO_DEV AoDevId, BD_S32 s32AoChnCnt, AIO_ATTR_S *pstAioAttr)
{
	BD_S32 i;
    BD_S32 s32Ret;

    s32Ret = BD_MPI_AO_SetPubAttr(AoDevId, pstAioAttr);
    if(BD_SUCCESS != s32Ret)
    {
        printf("%s: BD_MPI_AO_SetPubAttr(%d) failed with %#x!\n", __FUNCTION__, AoDevId,s32Ret);
        return BD_FAILURE;
    }
    
    s32Ret = BD_MPI_AO_Enable(AoDevId);
    if(BD_SUCCESS != s32Ret)
    {
        printf("%s: BD_MPI_AO_Enable(%d) failed with %#x!\n", __FUNCTION__, AoDevId, s32Ret);
        return BD_FAILURE;
    }

	for (i=0; i<s32AoChnCnt; i++)
    {
	    s32Ret = BD_MPI_AO_EnableChn(AoDevId, i);
	    if(BD_SUCCESS != s32Ret)
	    {
	        printf("%s: BD_MPI_AO_EnableChn(%d) failed with %#x!\n", __FUNCTION__, i, s32Ret);
	        return BD_FAILURE;
	    }
	    
	}
	

    return BD_SUCCESS;
}

/******************************************************************************
* function : Stop Ao
******************************************************************************/
BD_S32 SAMPLE_COMM_AUDIO_StopAo(AUDIO_DEV AoDevId, BD_S32 s32AoChnCnt)
{
	BD_S32 i;
    BD_S32 s32Ret;


	for (i=0; i<s32AoChnCnt; i++)
    {
	    s32Ret = BD_MPI_AO_DisableChn(AoDevId, i);
	    if (BD_SUCCESS != s32Ret)
	    {
	        printf("%s: BD_MPI_AO_DisableChn failed with %#x!\n", __FUNCTION__, s32Ret);
	        return s32Ret;
	    }
	}
	

	
    s32Ret = BD_MPI_AO_Disable(AoDevId);
    if (BD_SUCCESS != s32Ret)
    {
        printf("%s: BD_MPI_AO_Disable failed with %#x!\n", __FUNCTION__, s32Ret);
        return s32Ret;
    }

	s32Ret = BD_MPI_AO_ClrPubAttr(AoDevId);
    if (BD_SUCCESS != s32Ret)
    {
        printf("%s: BD_MPI_AO_ClrPubAttr failed with %#x!\n", __FUNCTION__, s32Ret);
        return s32Ret;
    }
	
    return BD_SUCCESS;
}


/******************************************************************************
* function : Ao bind Adec
******************************************************************************/
BD_S32 SAMPLE_COMM_AUDIO_AoBindAdec(AUDIO_DEV AoDev, AO_CHN AoChn, ADEC_CHN AdChn)
{
    MPP_CHN_S stSrcChn,stDestChn;

    stSrcChn.enModId = BD_ID_ADEC;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = AdChn;
    stDestChn.enModId = BD_ID_AO;
    stDestChn.s32DevId = AoDev;
    stDestChn.s32ChnId = AoChn;

	printf("[%s]id: 0x%x, chn: %d\n", __FUNCTION__, stSrcChn.enModId, stSrcChn.s32ChnId);
    return BD_MPI_SYS_Bind(&stSrcChn, &stDestChn); 
}

/******************************************************************************
* function : Ao unbind Adec
******************************************************************************/
BD_S32 SAMPLE_COMM_AUDIO_AoUnbindAdec(AUDIO_DEV AoDev, AO_CHN AoChn, ADEC_CHN AdChn)
{
    MPP_CHN_S stSrcChn,stDestChn;

    stSrcChn.enModId = BD_ID_ADEC;
    stSrcChn.s32ChnId = AdChn;
    stSrcChn.s32DevId = 0;
    stDestChn.enModId = BD_ID_AO;
    stDestChn.s32DevId = AoDev;
    stDestChn.s32ChnId = AoChn;
    
    return BD_MPI_SYS_UnBind(&stSrcChn, &stDestChn); 
}


/******************************************************************************
* function : Aenc bind Ai
******************************************************************************/
BD_S32 SAMPLE_COMM_AUDIO_AencBindAi(AUDIO_DEV AiDev, AI_CHN AiChn, AENC_CHN AeChn)
{
    MPP_CHN_S stSrcChn,stDestChn;

    stSrcChn.enModId = BD_ID_AI;
    stSrcChn.s32DevId = AiDev;
    stSrcChn.s32ChnId = AiChn;
    stDestChn.enModId = BD_ID_AENC;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = AeChn;
    
    return BD_MPI_SYS_Bind(&stSrcChn, &stDestChn);
}

/******************************************************************************
* function : Aenc unbind Ai
******************************************************************************/
BD_S32 SAMPLE_COMM_AUDIO_AencUnbindAi(AUDIO_DEV AiDev, AI_CHN AiChn, AENC_CHN AeChn)
{
    MPP_CHN_S stSrcChn,stDestChn;

    stSrcChn.enModId = BD_ID_AI;
    stSrcChn.s32DevId = AiDev;
    stSrcChn.s32ChnId = AiChn;
    stDestChn.enModId = BD_ID_AENC;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = AeChn;
    
    return BD_MPI_SYS_UnBind(&stSrcChn, &stDestChn);      
}

/******************************************************************************
* function : Ao bind Ai
******************************************************************************/
BD_S32 SAMPLE_COMM_AUDIO_AoBindAi(AUDIO_DEV AiDev, AI_CHN AiChn, AUDIO_DEV AoDev, AO_CHN AoChn)
{
    MPP_CHN_S stSrcChn,stDestChn;

    stSrcChn.enModId = BD_ID_AI;
    stSrcChn.s32ChnId = AiChn;
    stSrcChn.s32DevId = AiDev;
    stDestChn.enModId = BD_ID_AO;
    stDestChn.s32DevId = AoDev;
    stDestChn.s32ChnId = AoChn;
    
    return BD_MPI_SYS_Bind(&stSrcChn, &stDestChn); 
}

/******************************************************************************
* function : Ao unbind Ai
******************************************************************************/
BD_S32 SAMPLE_COMM_AUDIO_AoUnbindAi(AUDIO_DEV AiDev, AI_CHN AiChn, AUDIO_DEV AoDev, AO_CHN AoChn)
{
    MPP_CHN_S stSrcChn,stDestChn;

    stSrcChn.enModId = BD_ID_AI;
    stSrcChn.s32ChnId = AiChn;
    stSrcChn.s32DevId = AiDev;
    stDestChn.enModId = BD_ID_AO;
    stDestChn.s32DevId = AoDev;
    stDestChn.s32ChnId = AoChn;
    
    return BD_MPI_SYS_UnBind(&stSrcChn, &stDestChn); 
}



/******************************************************************************
* function : Start Aenc
******************************************************************************/
//BD_S32 SAMPLE_COMM_AUDIO_StartAenc(BD_S32 s32AencChnCnt, BD_U32 u32AencPtNumPerFrm, PAYLOAD_TYPE_E enType)
BD_S32 SAMPLE_COMM_AUDIO_StartAenc(BD_S32 s32AencChnCnt, AENC_CHN_ATTR_S *pstAencAttr)

{
    AENC_CHN AeChn;
    BD_S32 s32Ret, i;

    for (i=0; i<s32AencChnCnt; i++)
    {            
        AeChn = i;
        
        /* create aenc chn*/
        s32Ret = BD_MPI_AENC_CreateChn(AeChn, pstAencAttr);
        if (BD_SUCCESS != s32Ret)
        {
            printf("%s: BD_MPI_AENC_CreateChn(%d) failed with %#x!\n", __FUNCTION__,
                   AeChn, s32Ret);
            return s32Ret;
        }        
    }
    
    return BD_SUCCESS;
}

/******************************************************************************
* function : Stop Aenc
******************************************************************************/
BD_S32 SAMPLE_COMM_AUDIO_StopAenc(BD_S32 s32AencChnCnt)
{
    BD_S32 i;
    BD_S32 s32Ret;
    
    for (i=0; i<s32AencChnCnt; i++)
    {
        s32Ret = BD_MPI_AENC_DestroyChn(i);
        if (BD_SUCCESS != s32Ret)
        {
            printf("%s: BD_MPI_AENC_DestroyChn(%d) failed with %#x!\n", __FUNCTION__,
                   i, s32Ret);
            return s32Ret;
        }
        
    }
    
    return BD_SUCCESS;
}

/******************************************************************************
* function : Start Adec
******************************************************************************/
//BD_S32 SAMPLE_COMM_AUDIO_StartAdec(ADEC_CHN AdChn, BD_U32 u32AdecPtNumPerFrm, PAYLOAD_TYPE_E enType)
BD_S32 SAMPLE_COMM_AUDIO_StartAdec(ADEC_CHN AdChn, ADEC_CHN_ATTR_S *pstAdecAttr)

{
    BD_S32 s32Ret;
    
    /* create adec chn*/
    s32Ret = BD_MPI_ADEC_CreateChn(AdChn, pstAdecAttr);
    if (BD_SUCCESS != s32Ret)
    {
        printf("%s: BD_MPI_ADEC_CreateChn(%d) failed with %#x!\n", __FUNCTION__,\
               AdChn,s32Ret);
        return s32Ret;
    }
    return BD_SUCCESS;

}


/******************************************************************************
* function : Stop Adec
******************************************************************************/
BD_S32 SAMPLE_COMM_AUDIO_StopAdec(ADEC_CHN AdChn)
{
    BD_S32 s32Ret;
    
    s32Ret = BD_MPI_ADEC_DestroyChn(AdChn);
    if (BD_SUCCESS != s32Ret)
    {
        printf("%s: BD_MPI_ADEC_DestroyChn(%d) failed with %#x!\n", __FUNCTION__,
               AdChn, s32Ret);
        return s32Ret;
    }
    
    return BD_SUCCESS;
}



/******************************************************************************
* function : Create the thread to get frame from ai and send to ao
******************************************************************************/
BD_S32 SAMPLE_COMM_AUDIO_CreatTrdAiAo(AUDIO_DEV AiDev, AI_CHN AiChn, AUDIO_DEV AoDev, AO_CHN AoChn)
{
    SAMPLE_AI_S *pstAi = NULL;
    
    pstAi = &gs_stSampleAi[AiDev*AIO_MAX_CHN_NUM + AiChn];
    pstAi->bSendAenc = BD_FALSE;
    pstAi->bSendAo = BD_TRUE;
    pstAi->bStart= BD_TRUE;
    pstAi->AiDev = AiDev;
    pstAi->AiChn = AiChn;
    pstAi->AoDev = AoDev;
    pstAi->AoChn = AoChn;

    pthread_create(&pstAi->stAiPid, 0, SAMPLE_COMM_AUDIO_AiProc, pstAi);
    
    return BD_SUCCESS;
}

/******************************************************************************
* function : Create the thread to get frame from ai and save it to file
******************************************************************************/
BD_S32 SAMPLE_COMM_AUDIO_CreatTrdAiRec(AUDIO_DEV AiDev, AI_CHN AiChn)
{
    SAMPLE_AI_S *pstAi = NULL;
    
    pstAi = &gs_stSampleAi[AiDev*AIO_MAX_CHN_NUM + AiChn];
    pstAi->bSendAenc = BD_FALSE;
    pstAi->bSendAo = BD_FALSE;
    pstAi->bStart= BD_TRUE;
    pstAi->AiDev = AiDev;
    pstAi->AiChn = AiChn;

    pthread_create(&pstAi->stAiPid, 0, SAMPLE_COMM_AUDIO_AiRecProc, pstAi);
    
    return BD_SUCCESS;
}


/******************************************************************************
* function : Create the thread to get frame from ai and send to aenc
******************************************************************************/
BD_S32 SAMPLE_COMM_AUDIO_CreatTrdAiAenc(AUDIO_DEV AiDev, AI_CHN AiChn, AENC_CHN AeChn, BD_BOOL bSendAo)
{
    SAMPLE_AI_S *pstAi = NULL;
    
    pstAi = &gs_stSampleAi[AiDev*AIO_MAX_CHN_NUM + AiChn];
    pstAi->bSendAenc = BD_TRUE;
    pstAi->bSendAo = bSendAo;
    pstAi->bStart= BD_TRUE;
    pstAi->AiDev = AiDev;
    pstAi->AiChn = AiChn;
    pstAi->AencChn = AeChn;
    pthread_create(&pstAi->stAiPid, 0, SAMPLE_COMM_AUDIO_AiProc, pstAi);
    
    return BD_SUCCESS;
}


/******************************************************************************
* function : Destory the thread to get frame from ai and send to ao or aenc
******************************************************************************/
BD_S32 SAMPLE_COMM_AUDIO_DestoryTrdAi(AUDIO_DEV AiDev, AI_CHN AiChn)
{
    SAMPLE_AI_S *pstAi = NULL;
    
    pstAi = &gs_stSampleAi[AiDev*AIO_MAX_CHN_NUM + AiChn];
    pstAi->bStart= BD_FALSE;
    pthread_cancel(pstAi->stAiPid);
    pthread_join(pstAi->stAiPid, 0);
    
    return BD_SUCCESS;
}



/******************************************************************************
* function : Create the thread to get stream from aenc and send to adec
******************************************************************************/
BD_S32 SAMPLE_COMM_AUDIO_CreatTrdAencAdec(AENC_CHN AeChn, ADEC_CHN AdChn, FILE *pAecFd)
{
    SAMPLE_AENC_S *pstAenc = NULL;

    if (NULL == pAecFd)
    {
        return BD_FAILURE;
    }
    
    pstAenc = &gs_stSampleAenc[AeChn];
    pstAenc->AeChn = AeChn;
    pstAenc->AdChn = AdChn;
    pstAenc->bSendAdChn = BD_TRUE;
    pstAenc->pfd_out = pAecFd;    
    pstAenc->bStart = BD_TRUE;    
    pthread_create(&pstAenc->stAencPid, 0, SAMPLE_COMM_AUDIO_AencProc, pstAenc);
    
    return BD_SUCCESS;
}


/******************************************************************************
* function : Destory the thread to get stream from aenc and send to adec
******************************************************************************/
BD_S32 SAMPLE_COMM_AUDIO_DestoryTrdAencAdec(AENC_CHN AeChn)
{
    SAMPLE_AENC_S *pstAenc = NULL;

    pstAenc = &gs_stSampleAenc[AeChn];
    pstAenc->bStart = BD_FALSE;
    pthread_cancel(pstAenc->stAencPid);
    pthread_join(pstAenc->stAencPid, 0);
    
    return BD_SUCCESS;
}

/******************************************************************************
* function : Create the thread to get stream from aenc and save it to file
******************************************************************************/
BD_S32 SAMPLE_COMM_AUDIO_CreatTrdAencSave(AENC_CHN AeChn, FILE *pAecFd)
{
    SAMPLE_AENC_S *pstAenc = NULL;

    if (NULL == pAecFd)
    {
        return BD_FAILURE;
    }
    
    pstAenc = &gs_stSampleAenc[AeChn];
    pstAenc->AeChn = AeChn;
    pstAenc->bSendAdChn = BD_FALSE;
    pstAenc->pfd_out = pAecFd;    
    pstAenc->bStart = BD_TRUE;    
    pthread_create(&pstAenc->stAencPid, 0, SAMPLE_COMM_AUDIO_AencProc, pstAenc);
    
    return BD_SUCCESS;
}

/******************************************************************************
* function : Destory the thread to get stream from aenc and save it to file
******************************************************************************/
BD_S32 SAMPLE_COMM_AUDIO_DestoryTrdAencSave(AENC_CHN AeChn)
{
    SAMPLE_AENC_S *pstAenc = NULL;

    pstAenc = &gs_stSampleAenc[AeChn];
    pstAenc->bStart = BD_FALSE;
    pthread_cancel(pstAenc->stAencPid);
    pthread_join(pstAenc->stAencPid, 0);
    
    return BD_SUCCESS;
}


/******************************************************************************
* function : Create the thread to get stream from file, send to aenc and save it to file
******************************************************************************/
BD_S32 SAMPLE_COMM_AUDIO_CreatTrdAencFile(AENC_CHN AeChn, FILE *pFdIn, FILE *pFdOut, AUDIO_FRAME_S *pstFrm)
{
    SAMPLE_AENC_S *pstAenc = NULL;

    if (NULL == pFdIn || NULL == pFdOut)
    {
        return BD_FAILURE;
    }
	memcpy(&gs_stFrameAenc[AeChn], pstFrm, sizeof(AUDIO_FRAME_S));
    pstAenc = &gs_stSampleAenc[AeChn];
    pstAenc->AeChn = AeChn;
    pstAenc->pfd_in = pFdIn;
	pstAenc->pfd_out = pFdOut;
    pstAenc->bStart = BD_TRUE;    
    pthread_create(&pstAenc->stAencPid, 0, SAMPLE_COMM_AUDIO_AencFileProc, pstAenc);

    return BD_SUCCESS;
}

/******************************************************************************
* function : Destory the thread to get stream from aenc and send to adec
******************************************************************************/
BD_S32 SAMPLE_COMM_AUDIO_DestoryTrdAencFile(AENC_CHN AeChn)
{
    SAMPLE_AENC_S *pstAenc = NULL;

    pstAenc = &gs_stSampleAenc[AeChn];
    pstAenc->bStart = BD_FALSE;
    pthread_join(pstAenc->stAencPid, 0);
    
    return BD_SUCCESS;
}



/******************************************************************************
* function : Create the thread to get frame from aenc and send to adec
******************************************************************************/
BD_S32 SAMPLE_COMM_AUDIO_CreatTrdAdecFile(ADEC_CHN AdChn, FILE *pfdin, FILE *pfdout, AUDIO_STREAM_S *pstStream)
{
	SAMPLE_ADEC_S *pstAdec = NULL;

	if (NULL == pfdin)
    {
        return BD_FAILURE;
    }

	memcpy(&gs_stStreamAdec[AdChn], pstStream, sizeof(AUDIO_STREAM_S));

    pstAdec = &gs_stSampleAdec[AdChn];
    pstAdec->AdChn = AdChn;
    pstAdec->pfd_in = pfdin;
	pstAdec->pfd_out = pfdout;
    pstAdec->bStart = BD_TRUE;

    pthread_create(&pstAdec->stAdPid, 0, SAMPLE_COMM_AUDIO_AdecFileProc, pstAdec);

	return BD_SUCCESS;
}

BD_S32 SAMPLE_COMM_AUDIO_DestoryTrdAdecFile(ADEC_CHN AdChn)
{
    SAMPLE_ADEC_S *pstAdec = NULL;

    pstAdec = &gs_stSampleAdec[AdChn];
    pstAdec->bStart = BD_FALSE;
    pthread_join(pstAdec->stAdPid, 0);
    
    return BD_SUCCESS;

}


BD_S32 SAMPLE_COMM_AUDIO_CreatTrdRecAo(AUDIO_DEV AoDev, AO_CHN AoChn, FILE *pfdin, AUDIO_FRAME_S *pstFrame)
{
	SAMPLE_AO_S *pstAo = NULL;

	if (NULL == pfdin)
    {
        return BD_FAILURE;
    }

	memcpy(&gs_stFrameAo[AoDev*AIO_MAX_CHN_NUM + AoChn], pstFrame, sizeof(AUDIO_FRAME_S));

    pstAo = &gs_stSampleAo[AoDev*AIO_MAX_CHN_NUM + AoChn];
	pstAo->AoDev = AoDev;
	pstAo->AoChn = AoChn;
    pstAo->pfd_in = pfdin;
    pstAo->bStart = BD_TRUE;

    pthread_create(&pstAo->stAoPid, 0, SAMPLE_COMM_AUDIO_RecAoProc, pstAo);

	return BD_SUCCESS;

}

BD_S32 SAMPLE_COMM_AUDIO_DestoryTrdRecAo(AUDIO_DEV AoDev, AO_CHN AoChn)
{
    SAMPLE_AO_S *pstAo = NULL;

    pstAo = &gs_stSampleAo[AoDev*AIO_MAX_CHN_NUM + AoChn];
    pstAo->bStart = BD_FALSE;
    pthread_join(pstAo->stAoPid, 0);
    
    return BD_SUCCESS;

}

#ifdef __cplusplus
#if __cplusplus
}
#endif 
#endif /* End of #ifdef __cplusplus */


