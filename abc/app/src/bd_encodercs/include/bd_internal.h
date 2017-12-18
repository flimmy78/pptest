
#ifndef __BD_INTERNAL_H__
#define __BD_INTERNAL_H__

#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include "sample_comm.h"
#include "bd_encoder.h"
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */


BD_VOID* encoding_thread_func(BD_VOID *p);

BD_S32 _set_channel_attribute(
		VENC_CHN_ATTR_S stVencChnAttr,
		int channel,
		PIC_SIZE_E video_size[],
		BD_APP_CONTEXT  *app_contexts,
	  int num_of_contexts
  );


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef __BD_INTERNAL_H__ */
