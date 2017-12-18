#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>

#include "sample_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */


BD_S32 SAMPLE_COMM_RGN_CoverCreate(BD_S32 Handle, BD_S32 s32SrcX, BD_S32 s32SrcY, BD_U32 u32SrcWidth, BD_U32 u32SrcHeight)
{
	RGN_ATTR_S stRegionAttr;
	BD_S32 s32Ret = BD_SUCCESS;

	stRegionAttr.enType = RGN_YUV_COVER;
	stRegionAttr.unAttr.enPixelFmt = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
	stRegionAttr.unAttr.stPoint.s32X = s32SrcX;
	stRegionAttr.unAttr.stPoint.s32Y = s32SrcY;
	stRegionAttr.unAttr.stSize.u32Width = u32SrcWidth;
	stRegionAttr.unAttr.stSize.u32Height = u32SrcHeight;

	s32Ret = BD_MPI_RGN_Create(Handle, &stRegionAttr);
	if (s32Ret != BD_SUCCESS) {
		SAMPLE_PRT("failed to create region %d!\n", s32Ret);
		goto create_fail;
	}

	return BD_SUCCESS;

create_fail:
	return BD_FAILURE;
}


BD_S32 SAMPLE_COMM_RGN_CoverAttach(VI_CHN ViChn, BD_S32 Handle, BD_S32 s32DstX, BD_S32 s32DstY)
{
	MPP_CHN_S stChnBind;
	RGN_CHN_ATTR_S stChnAttr;
	BD_S32 s32Ret = BD_SUCCESS;

	stChnBind.s32ChnId = ViChn;
	//stChnAttr.enType = RGN_YUV_COVER;     /* region type */
	stChnAttr.unChnAttr.stCoverChn.stPoint.s32X = s32DstX;
	stChnAttr.unChnAttr.stCoverChn.stPoint.s32Y = s32DstY;
	stChnAttr.unChnAttr.stCoverChn.u32Color = 0x80808080;

	s32Ret = BD_MPI_RGN_AttachToChn(Handle, &stChnBind, &stChnAttr);
	if (s32Ret != BD_SUCCESS) {
		SAMPLE_PRT("failed to attach region to channel %d!\n", s32Ret);
		goto create_fail;
	}

	return BD_SUCCESS;

create_fail:
	return BD_FAILURE;
}

BD_S32 SAMPLE_COMM_RGN_CoverDetach(VI_CHN ViChn, BD_S32 Handle)
{
	MPP_CHN_S stChnBind;
	BD_S32 s32Ret = BD_SUCCESS;

	stChnBind.s32ChnId = ViChn;
	s32Ret = BD_MPI_RGN_DetachFromChn(Handle, &stChnBind);
	if (s32Ret != BD_SUCCESS) {
		SAMPLE_PRT("failed to detach region from channel %d!\n", s32Ret);
		return BD_FAILURE;
	}

	return BD_SUCCESS;
}


BD_S32 SAMPLE_COMM_RGN_CoverExit(BD_S32 Handle)
{
	BD_S32 s32Ret = BD_SUCCESS;

	s32Ret = BD_MPI_RGN_Destroy(Handle);
	if (s32Ret != BD_SUCCESS) {
		SAMPLE_PRT("region create failed with %d!\n", s32Ret);
		return BD_FAILURE;
	}
	return BD_SUCCESS;
}



BD_VOID SAMPLE_COMM_RGN_OverlayCreate(BD_S32 Handle, RGN_TYPE_E enType,
		BD_S32 s32SrcX, BD_S32 s32SrcY, BD_U32 u32SrcWidth, BD_U32 u32SrcHeight)
{
	RGN_ATTR_S stRegionAttr;
	RGN_ATTR_S stSetRegionAttr;
	RGN_ATTR_S stGetRegionAttr;
	BD_S32 s32Ret = BD_SUCCESS;

	if (enType != RGN_YUV_OVERLAY && enType != RGN_RGB_OVERLAY) {
		SAMPLE_PRT("invalid region type %d\n", enType);
		goto create_fail;
	}

	stRegionAttr.enType = enType;
	if (enType == RGN_YUV_OVERLAY) {
		stRegionAttr.unAttr.enPixelFmt = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
	}
	else {
		stRegionAttr.unAttr.enPixelFmt = PIXEL_FORMAT_RGB_888;
	}

	stRegionAttr.unAttr.stPoint.s32X = s32SrcX;
	stRegionAttr.unAttr.stPoint.s32Y = s32SrcY;
	stRegionAttr.unAttr.stSize.u32Width = u32SrcWidth;
	stRegionAttr.unAttr.stSize.u32Height = u32SrcHeight;

	s32Ret = BD_MPI_RGN_Create(Handle, &stRegionAttr);
	if (s32Ret != BD_SUCCESS) {
		SAMPLE_PRT("failed to create region %d!\n", s32Ret);
		goto create_fail;
	}

	stSetRegionAttr.enType = stRegionAttr.enType;
	stSetRegionAttr.unAttr.enPixelFmt = stRegionAttr.unAttr.enPixelFmt;
	stSetRegionAttr.unAttr.stPoint.s32X = s32SrcX;
	stSetRegionAttr.unAttr.stPoint.s32Y = s32SrcY;
	stSetRegionAttr.unAttr.stSize.u32Width = u32SrcWidth;
	stSetRegionAttr.unAttr.stSize.u32Height = u32SrcHeight;

	s32Ret = BD_MPI_RGN_SetAttr(Handle, &stSetRegionAttr);	
	if (s32Ret != BD_SUCCESS) {
		SAMPLE_PRT("failed to set region attribute %d!\n", s32Ret);
		goto create_fail;
	}

	s32Ret = BD_MPI_RGN_GetAttr(Handle, &stGetRegionAttr);	
	if (s32Ret != BD_SUCCESS) {
		SAMPLE_PRT("failed to get region attribute %d!\n", s32Ret);
		goto create_fail;
	}

	SAMPLE_PRT("get info : type %s, pix %d, x %d, y %d, w %d, h %d\n", 
			GetRGNTypeString(stGetRegionAttr.enType),
			stGetRegionAttr.unAttr.enPixelFmt,
			stGetRegionAttr.unAttr.stPoint.s32X,
			stGetRegionAttr.unAttr.stPoint.s32Y,
			stGetRegionAttr.unAttr.stSize.u32Width,
			stGetRegionAttr.unAttr.stSize.u32Height
			);	

	return;

create_fail:
	return;
}

BD_VOID SAMPLE_COMM_RGN_OverlayAttach(VI_CHN ViChn, BD_S32 Handle,
		BD_S32 s32DstX, BD_S32 s32DstY)
{
	MPP_CHN_S stChnBind;
	RGN_CHN_ATTR_S stChnAttr;
	BD_S32 s32Ret = BD_SUCCESS;

	stChnBind.s32ChnId = ViChn;
	//stChnAttr.enType = RGN_YUV_OVERLAY;     /* region type */
	stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = s32DstX;
	stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = s32DstY;
	s32Ret = BD_MPI_RGN_AttachToChn(Handle, &stChnBind, &stChnAttr);
	if (s32Ret != BD_SUCCESS) {
		SAMPLE_PRT("failed to attach region to channel %d!\n", s32Ret);
		return;
	}

	return;
}

BD_VOID SAMPLE_COMM_RGN_OverlayDetach(VI_CHN ViChn, BD_S32 Handle)
{
	MPP_CHN_S stChnBind;
	BD_S32 s32Ret = BD_SUCCESS;

	stChnBind.s32ChnId = ViChn;
	s32Ret = BD_MPI_RGN_DetachFromChn(Handle, &stChnBind);
	if (s32Ret != BD_SUCCESS) {
		SAMPLE_PRT("failed to detach region from channel %d!\n", s32Ret);
	}

	return;
}

BD_VOID SAMPLE_COMM_RGN_OverlayExit(BD_S32 Handle)
{
	BD_S32 s32Ret = BD_SUCCESS;

	s32Ret = BD_MPI_RGN_Destroy(Handle);
	if (s32Ret != BD_SUCCESS) {
		SAMPLE_PRT("region create failed with %d!\n", s32Ret);
	}
	return;
}

BD_U8* SAMPLE_COMM_RGN_OverlayGetBuffer(BD_S32 Handle)
{
	BD_S32 s32Ret = BD_SUCCESS;
	//	BITMAP_S stBitMap;
	RGN_CANVAS_INFO_S stCanvasInfo;

	///	_dbg("draw bitmap using BD_MPI_RGN_GetCanvasInfo / BD_MPI_RGN_UpdateCanvas\n");
	s32Ret = BD_MPI_RGN_GetCanvasInfo(Handle, &stCanvasInfo);
	if (s32Ret != BD_SUCCESS) {
		SAMPLE_PRT("failed to get cavas region region %d!\n", s32Ret);
		goto getbuf_fail;
	}

	if (stCanvasInfo.u32PhyAddr == 0 ||
			stCanvasInfo.u32VirtAddr == 0) {
		SAMPLE_PRT("invalid canvas Memory region \n");
		goto getbuf_fail;
	}

/*
	if (stCanvasInfo.enPixelFmt != PIXEL_FORMAT_YUV_SEMIPLANAR_420) {
		SAMPLE_PRT("invalid canvas Pixel format region \n");
		goto getbuf_fail;
	}
*/
	return (BD_U8*)stCanvasInfo.u32VirtAddr;

getbuf_fail:

	return NULL;
}

BD_VOID SAMPLE_COMM_RGN_OverlayUpdate(BD_S32 Handle)
{
	BD_S32 s32Ret = BD_SUCCESS;

	s32Ret = BD_MPI_RGN_UpdateCanvas(Handle);
	if (s32Ret != BD_SUCCESS) {
		SAMPLE_PRT("failed to update canvas region %d!\n", s32Ret);
		goto update_fail;
	}

update_fail:

	return;
}




#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
