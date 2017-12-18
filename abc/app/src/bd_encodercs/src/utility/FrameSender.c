#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include <assert.h>
#include "SharedMemQue.h"
#include "FrameSender.h"

#define MV_SUM_INTV	4	/// should be a power of 2
#define MV_SX		16
#define MV_SY		9

#define FRAME_DURATIONS		40000

#define TIME_DELAY			0
#define SHOW_STATS			1
#define NOT_IDR				0

#define SEM_SHMQ			"sem_shmq"


typedef struct{
	unsigned int size;
	unsigned int tv_sec;
	unsigned int tv_usec;
	uint8_t is_last;
	uint8_t frmtype;		/// 0=IDR, 1=I, 2=P, 3=B
	uint8_t codec_type;		// 0: H264, 1: MJPEG
	uint8_t rsv[1];
	uint16_t mv[9];
	uint16_t rsv2;
}_DataHeader;

typedef struct{
	unsigned id;
	SHMQ shmq;
	sem_t *sem_shmq;
	uint8_t *m_pSPS;
	uint8_t *m_pPPS;
	unsigned int m_SPSSize;
	unsigned int m_PPSSize;
	unsigned int m_HdrFound;
}_FrameSender;

void *InitFrameSender(unsigned id, int size, const char *name)
{
	_FrameSender *fs;
	char sem_name[16];

	//m_pShmQue = new CSharedMemQue(1, shm_size, shm_name);
	fs = (_FrameSender *)malloc(sizeof(_FrameSender));
	assert(fs);
	fs->shmq = InitSharedMemQue(1, size, name);
	assert(fs->shmq);

	fs->id = id;
	fs->m_pSPS = NULL;
	fs->m_pPPS = NULL;
	fs->m_SPSSize = 0;
	fs->m_PPSSize = 0;
	fs->m_HdrFound = 0;

	sprintf(sem_name, "%s_%d", SEM_SHMQ, id);
	fs->sem_shmq = sem_open(sem_name, O_CREAT, 0777, 0);
	assert(fs->sem_shmq != NULL);

	return fs;
}


void DeleteFrameSender(void *handle)
{
	char sem_name[16];
	_FrameSender *fs = (_FrameSender *)handle;
	assert(fs);

	sprintf(sem_name, "%s_%d", SEM_SHMQ, fs->id);
	sem_unlink(sem_name);

	if(fs->m_pSPS){
		free(fs->m_pSPS);
		fs->m_pSPS = NULL;
	}
	if(fs->m_pPPS){
		free(fs->m_pPPS);
		fs->m_pPPS = NULL;
	}

	//if(fs->m_pShmQue)delete fs->m_pShmQue;
	DeleteSharedMemQue(fs->shmq);
	free(fs);
}

int FindStreamSize(uint8_t *pBuf, int bufsize)
{
	int i;
	int HdrDescCnt = 0;
	int ret=0;

	for(i=0;i<bufsize-4;i++){
		if(pBuf[i+0] == 0 && pBuf[i+1] == 0 && pBuf[i+2] == 0 && pBuf[i+3] == 1){
			HdrDescCnt++;
		}

		if(HdrDescCnt == 2){
			break;
		}
	}

	if(HdrDescCnt == 1){
		ret = i+4;
	}else if(HdrDescCnt == 2){
		ret = i;
	}else
		ret = 0;

	return ret;
}

// return : start offset
int FindHeader(uint8_t *pBuf, int size, int Type, int *hdr_size)
{
	int i;
	int ret = 0;
	int hdrtype = 7;
	int pattern_cnt = 0;
	unsigned int pattern = 0x01000000;
	unsigned int pattern_start = 0;
	unsigned int pattern_end = 0;
	int hdr_found = 0;

	if(Type == FRAME_TYPE_SPS) hdrtype = 7;	// sps
	else if(Type == FRAME_TYPE_PPS)hdrtype = 8;

	pattern_start = 0;
	pattern_end = size;

	for(i=0;i<size;i++){
		if(i >= 4){
			if(memcmp(&pBuf[i-4], &pattern, 4) == 0){
				if(hdr_found){
					pattern_end = i-4;
					break;
				}

				if((pBuf[i] & 0x1f) == hdrtype){
					pattern_start = i-4;
					hdr_found = 1;
					printf("sps or pps hdr found\n");
				}

			}
		}
	}

	if(hdr_found){
		ret = pattern_start;
		*hdr_size = pattern_end - pattern_start;
	}else{
		ret = -1;
		*hdr_size = 0;
	}

	return ret;
}

void SendFrame(void *handle, uint8_t *pBuf, struct timeval curtime, int size, uint8_t is_last, uint8_t slice_cnt, uint8_t frmtype, uint8_t codec_type, uint16_t *mv)
{
	int i;
	_DataHeader Header;
	_FrameSender *fs = (_FrameSender *)handle;
	assert(fs);

#if NOT_IDR
	int hdr_size, offset;

	if (frmtype == 0) {					/// 0=IMG_INTRA_IDR
		if(fs->m_pSPS == NULL){
			// Find SPS Header & Save
			if((offset = FindHeader(pBuf, size, FRAME_TYPE_SPS, &hdr_size)) >= 0){
				fs->m_SPSSize = hdr_size;
				fs->m_pSPS = (uint8_t *)malloc(fs->m_SPSSize);
				memcpy(fs->m_pSPS, &pBuf[offset], fs->m_SPSSize);
			}
		}
		if(fs->m_pPPS == NULL){
			// Find PPS Header & Save
			if((offset = FindHeader(pBuf, size, FRAME_TYPE_PPS, &hdr_size)) >= 0){
				fs->m_PPSSize = hdr_size;
				fs->m_pPPS = (uint8_t *)malloc(fs->m_PPSSize);
				memcpy(fs->m_pPPS, &pBuf[offset], fs->m_PPSSize);
			}
		}
	}

	if(fs->m_pSPS == NULL || fs->m_pPPS == NULL){

		goto END_FT;
	}
#endif

///printf("tv: %d.%d\n", curtime.tv_sec, curtime.tv_usec);
	Header.tv_sec = curtime.tv_sec;
	Header.tv_usec = curtime.tv_usec;
	Header.is_last = is_last;
	Header.frmtype = frmtype;
	Header.codec_type = codec_type;
	Header.rsv[0] = 0;
#if NOT_IDR
	if(slice_cnt == 0 && frmtype <= 1)
		Header.size = size + (fs->m_SPSSize + fs->m_PPSSize);
	else
#endif
		Header.size = size;
	if (mv) for (i=0; i<MV_SY; i++) Header.mv[i] = mv[i];
	Header.rsv2 = 0;
///	printf("[FS:%d] size:%8d s:%d us:%d l:%d t:%d\n", fs->id, Header.size, Header.tv_sec, Header.tv_usec,
///		Header.is_last, Header.frmtype);

	ShmWriteData(fs->shmq, (uint8_t *)&Header, sizeof(Header), 0);
#if NOT_IDR
	if(slice_cnt == 0 && frmtype <= 1){
		ShmWriteData(fs->shmq, (uint8_t *)fs->m_pSPS, fs->m_SPSSize, 0);
		ShmWriteData(fs->shmq, (uint8_t *)fs->m_pPPS, fs->m_PPSSize, 0);
	}
#endif
	ShmWriteData(fs->shmq, (uint8_t *)pBuf, size, 1);

	sem_post(fs->sem_shmq);

END_FT:
	return;
}
