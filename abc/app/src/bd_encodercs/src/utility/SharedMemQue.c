
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#if defined(__WIN32__) || defined(_WIN32)
#include <Windows.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#endif

#include "SharedMemQue.h"


void *InitSharedMemQue(int is_server, int size, const char *name)
{
	SHMQ sq;
	uint8_t *pShmAddr;
	void *retval = NULL;

	sq = (SHMQ)malloc(sizeof(*sq));
	assert(sq);

	sq->m_pHeader = NULL;
	sq->m_pData = NULL;

#if defined(__WIN32__) || defined(_WIN32)
	if((sq->m_hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE ,NULL, PAGE_READWRITE,0, shm_size, shm_name)) == NULL){
		//Error
		printf("Shm Create Fail\n");
		goto END_FT;
	}

	if((pShmAddr = (unsigned char *)MapViewOfFile(sq->m_hFileMap,FILE_MAP_ALL_ACCESS,0,0,shm_size)) == NULL){
		// Error
		printf("Shm MapView Fail\n");
		goto END_FT;
	}

#else
	key_t key;
	key = ftok(name, 0);
///	printf("name:%s key:0x%08x\n", name, key);
	if((sq->m_shmId = shmget(key, size, 0666|IPC_CREAT)) == -1){
		printf("Shm Create Fail\n");
		goto END_FT;
	}

	pShmAddr = (uint8_t *)shmat(sq->m_shmId, (void *)0, 0);
	if(pShmAddr == (uint8_t *)-1){
		//Error
		printf("Shmat Fail\n");
		goto END_FT;
	}
#endif

	sq->m_pHeader = (_ShmHeader *)pShmAddr;
	sq->m_pData = pShmAddr + sizeof(_ShmHeader);
	sq->m_MaxPos = size - sizeof(_ShmHeader);
	sq->m_WritePos = 0;
	if(is_server){
		memset(pShmAddr, 0, size);
		sq->m_pHeader->que_size = sq->m_MaxPos;
		sq->m_pHeader->write_pos = 0;
		sq->m_pHeader->read_pos = 0;
		sq->m_IsShmServer = 1;
	}else{
		sq->m_ReadPos = sq->m_pHeader->write_pos;
		sq->m_pHeader->read_pos = sq->m_ReadPos;
		sq->m_IsShmServer = 0;

		printf("shm init readpos: %d\n", sq->m_ReadPos);
	}

	retval = (void *)sq;

END_FT:
	return retval;
}

//CSharedMemQue::~CSharedMemQue(void)
void DeleteSharedMemQue(SHMQ handle)
{
	SHMQ sq = handle;
	assert(sq);

#if defined(__WIN32__) || defined(_WIN32)
	if(sq->m_pHeader)UnmapViewOfFile(sq->m_pHeader);
	if(sq->m_hFileMap)CloseHandle(sq->m_hFileMap);

#else
	if(sq->m_pHeader)shmdt(sq->m_pHeader);
	
	if(sq->m_IsShmServer && sq->m_shmId != -1){
		printf("Remove shared memory\n");
		shmctl( sq->m_shmId, IPC_RMID, 0);
		sq->m_shmId = -1;
	}
#endif

	free(sq);
}

//int CSharedMemQue::WriteData(unsigned char *data, int size)
int ShmWriteData(SHMQ handle, unsigned char *data, int size, int update_p)
{
	SHMQ sq = handle;
	int ret = -1;
	int writep;

	assert(sq);

	if(!sq->m_IsShmServer)goto END_FT;
	if(sq->m_pHeader == NULL || sq->m_pData == NULL)goto END_FT;

	writep = sq->m_WritePos;
	if(writep + size > sq->m_MaxPos){
		memcpy(&sq->m_pData[writep], data, sq->m_MaxPos - writep);
		memcpy(sq->m_pData, &data[sq->m_MaxPos - writep], writep + size - sq->m_MaxPos);
	}else{	
		memcpy(&sq->m_pData[writep], data, size);
	}

	writep += size;
	if(writep >= sq->m_MaxPos)writep -= sq->m_MaxPos;
	sq->m_WritePos = writep;

	if (update_p) sq->m_pHeader->write_pos = writep;
///	printf("[0x%08x] wp:%d\n", (unsigned)sq, sq->m_pHeader->write_pos);

	ret = 1;

END_FT:
	return ret;
}


//int CSharedMemQue::ReadData(unsigned char *data, int size)
int ShmReadData(SHMQ handle, unsigned char *data, int size)
{
	SHMQ sq = handle;
	int ret = -1;

	assert(sq);

	if(sq->m_pHeader == NULL || sq->m_pData == NULL)goto END_FT;

	if( (sq->m_MaxPos + sq->m_pHeader->write_pos - sq->m_ReadPos)%sq->m_MaxPos >= size){
		if(sq->m_ReadPos + size > sq->m_MaxPos){
			memcpy(data, &sq->m_pData[sq->m_ReadPos], sq->m_MaxPos - sq->m_ReadPos);
			memcpy(&data[sq->m_MaxPos - sq->m_ReadPos], sq->m_pData, sq->m_ReadPos + size - sq->m_MaxPos);

		}else{
			memcpy(data, &sq->m_pData[sq->m_ReadPos], size);
		}
		sq->m_ReadPos += size;
		if(sq->m_ReadPos >= sq->m_MaxPos){
			sq->m_ReadPos -= sq->m_MaxPos;
		}
		ret = 1;

//		printf("writepos: %d, readpos: %d\n", sq->m_pHeader->write_pos, sq->m_ReadPos);
	}else{	
		ret = 0;
		goto END_FT;
	}


END_FT:
	sq->m_pHeader->read_pos = sq->m_ReadPos;

	return ret;
}

//int CSharedMemQue::ReadQueDataSize(void)
int ShmReadQueDataSize(SHMQ handle)
{
	SHMQ sq = handle;
	assert(sq);

///	printf("w:%d r:%d m:%d\n", sq->m_pHeader->write_pos, sq->m_pHeader->read_pos, sq->m_MaxPos);
//	return (m_MaxPos + m_pHeader->write_pos - m_ReadPos)%m_MaxPos;
	return (sq->m_MaxPos + sq->m_pHeader->write_pos - sq->m_pHeader->read_pos)%sq->m_MaxPos;

}

//void CSharedMemQue::ReadReset()
void ShmReadReset(SHMQ handle)
{
	SHMQ sq = handle;
	assert(sq);

	sq->m_ReadPos = sq->m_pHeader->write_pos;
	sq->m_pHeader->read_pos = sq->m_ReadPos;
	printf("Read reset. writepos: %d, readpos: %d\n", sq->m_pHeader->write_pos, sq->m_ReadPos);
}

