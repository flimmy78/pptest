#ifndef _SHARED_MEM_QUE
#define _SHARED_MEM_QUE

typedef struct{
	unsigned int user_num;
	unsigned int que_size;
	unsigned int write_pos;
	unsigned int read_pos;
	unsigned int rsv[2];
}_ShmHeader;

typedef struct{
#if defined(__WIN32__) || defined(_WIN32)
	void *m_hFileMap;
#else
	int m_shmId;
#endif
	_ShmHeader *m_pHeader;
	unsigned char *m_pData;
	unsigned int m_WritePos;
	unsigned int m_ReadPos;
	unsigned int m_MaxPos;
	unsigned char m_IsShmServer;

}_SharedMemQue;

typedef _SharedMemQue * SHMQ;

void *InitSharedMemQue(int is_server, int size, const char *name);
void DeleteSharedMemQue(SHMQ handle);

int ShmWriteData(SHMQ handle, unsigned char *data, int size, int update_p);
int ShmReadData(SHMQ handle, unsigned char *data, int size);
int ShmReadQueDataSize(SHMQ handle);
void ShmReadReset(SHMQ handle);

#endif

