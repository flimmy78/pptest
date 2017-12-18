
/// File: bd_api_sns.h
/// Date: 2015/03

#include "bd_common.h"

#ifndef __BD_API_SNS_H_
#define __BD_API_SNS_H_	1

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

BD_S32 BD_MPI_Sensor_Init(BD_U32 u32Slot, BD_U32 u32Spwdn, BD_S32 s32SIndex, BD_U32 u32ScanType, BD_U32 u32Interface, BD_U32 u32I2cChn, BD_U8 u8ForcedAddr);
BD_S32 BD_MPI_Sensor_Exit(BD_U32 u32Slot);
BD_S32 BD_MPI_Sensor_LoadFileConfig(const BD_CHAR *pszFile, BD_U32 u32Slot, BD_U8 u8Addr);
BD_S32 BD_MPI_Sensor_FrameRate(BD_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
