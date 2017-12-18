#ifndef __MI_H_
#define __MI_H_	1

#include <bd_defines.h>
#include <linux/videodev2.h>
#include <linux/types.h>

#define MAX_MIMSGQ_SIZE	256

struct stMemBlkInfo {
	BD_BOOL				bValid;			// flag of info validation
	BD_U32 				u32PhyAddr;		// Physical Address
	BD_VOID*			VirtAddr;		// Virtual Address
	BD_S32				s32DmaFd;		// for DMA Fd
	BD_S32				s32RefCount;
};

struct stMIConfig {
	BD_U32 				u32Width;			// image width
	BD_U32				u32Height;			// image height
	BD_U32				u32Payload;			// total read (instead format)
	BD_U8				u8FrameDepth;		// total alloc count
	struct stMemBlkInfo astMemInfo[VIDEO_MAX_FRAME];	
};

void Miu_GetMIInfo(unsigned char* count, unsigned int* width, unsigned int* height, unsigned int* payload);
int Miu_GetFd(unsigned int index);

#endif
