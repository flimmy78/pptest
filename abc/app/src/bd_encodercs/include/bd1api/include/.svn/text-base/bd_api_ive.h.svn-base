#ifndef __BD_API_IVE_H__
#define __BD_API_IVE_H__

#include "bd_comm_ive.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

BD_S32 BD_MPI_IVE_INIT(BD_VOID);
BD_VOID BD_MPI_IVE_EXIT(BD_VOID);
BD_S32 BD_MPI_IVE_FILTER(IVE_HANDLE *pIveHandle, IVE_SRC_INFO_S *pstSrc, 
    IVE_MEM_INFO_S *pstDst, IVE_FILTER_CTRL_S *pstFilterCtrl,BD_BOOL bInstant);
BD_S32 BD_MPI_IVE_HIST(IVE_HANDLE *pIveHandle, IVE_SRC_INFO_S *pstSrc, 
    BD_S32 *pResult, BD_BOOL bInstant);
BD_S32 BD_MPI_IVE_Query(IVE_HANDLE IveHandle, BD_BOOL *pbFinish, BD_BOOL bBlock);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif/*__MPI_IVE_H__*/

