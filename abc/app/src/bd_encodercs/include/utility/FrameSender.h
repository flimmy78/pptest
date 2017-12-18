#ifndef _FRAME_SENDER
#define _FRAME_SENDER

#include <stdint.h>
#if defined(__WIN32__) || defined(_WIN32)
#include <Winsock2.h>
#else
#include <sys/time.h>
#endif

#define FRAME_TYPE_I	0
#define FRAME_TYPE_P	1
#define FRAME_TYPE_SPS	2
#define FRAME_TYPE_PPS	3

#define CODEC_TYPE_H264	0
#define CODEC_TYPE_MJPEG	1

void *InitFrameSender(unsigned id, int size, const char *name);
void DeleteFrameSender(void *handle);
void SendFrame(
		void *handle,
		uint8_t *pBuf,
		struct timeval curtime,
		int size,
		uint8_t is_last,
		uint8_t slice_cnt,
		uint8_t frmtype,
		uint8_t codec_type,
		uint16_t *mv);

#endif
