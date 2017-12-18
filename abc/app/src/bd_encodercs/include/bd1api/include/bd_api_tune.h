
/// File: bd_api_tune.h
/// Date: 2016/06

#ifndef __BD_API_TUNE_H_
#define __BD_API_TUNE_H_	1

#include "bd_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

BD_S32 BD_MPI_TUNE_OpenSerial(BD_CHAR* pTTYName, TUN_BRATE_E eBrate);
BD_VOID BD_MPI_TUNE_CloseSerial(BD_VOID);
BD_BOOL	BD_MPI_TUNE_WaitEvent(BD_VOID);
BD_VOID BD_MPI_TUNE_Process(BD_VOID);

/* for debugging */
BD_S32 BD_MPI_TUNE_ExternalOpenSerial(BD_S32 s32Handle);
BD_VOID BD_MPI_TUNE_ExternalCloseSerial(BD_VOID);
BD_VOID BD_MPI_TUNE_ExternalProcess(BD_U8 RecvChar);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

