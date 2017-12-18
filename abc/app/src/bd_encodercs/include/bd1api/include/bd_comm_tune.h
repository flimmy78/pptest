#ifndef __BD_COMM_TUNE_H_
#define __BD_COMM_TUNE_H_	1

#include "bd_type.h"
#include "bd_errno.h"
#include "bd_common.h"
#include "bd_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

typedef enum bdTUN_BRATE_E
{
    TUN_BRATE_9600 = 0,
	TUN_BRATE_19200,
	TUN_BRATE_38400,
	TUN_BRATE_57600,
    TUN_BRATE_115200,
	TUN_BRATE_230400,
} TUN_BRATE_E;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif

