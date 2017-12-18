
/// File: bd_api_region.h
/// Date: 2015/03

#ifndef __BD_API_REGION_H__
#define __BD_API_REGION_H__

#include "bd_common.h"
#include "bd_comm_region.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

BD_S32 BD_MPI_RGN_Create(RGN_HANDLE Handle, const RGN_ATTR_S *pstRegion);
BD_S32 BD_MPI_RGN_Destroy(RGN_HANDLE Handle);

BD_S32 BD_MPI_RGN_GetAttr(RGN_HANDLE Handle, RGN_ATTR_S *pstRegion);
BD_S32 BD_MPI_RGN_SetAttr(RGN_HANDLE Handle, const RGN_ATTR_S *pstRegion);

BD_S32 BD_MPI_RGN_SetBitMap(RGN_HANDLE Handle, const BITMAP_S *pstBitmap);

BD_S32 BD_MPI_RGN_AttachToChn(RGN_HANDLE Handle, const MPP_CHN_S *pstChn, const RGN_CHN_ATTR_S *pstChnAttr);
BD_S32 BD_MPI_RGN_DetachFromChn(RGN_HANDLE Handle, const MPP_CHN_S *pstChn);

BD_S32 BD_MPI_RGN_GetCanvasInfo(RGN_HANDLE Handle, RGN_CANVAS_INFO_S *pstCanvasInfo);
BD_S32 BD_MPI_RGN_UpdateCanvas(RGN_HANDLE Handle);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* End of #ifndef __BD_API_REGION_H__ */

