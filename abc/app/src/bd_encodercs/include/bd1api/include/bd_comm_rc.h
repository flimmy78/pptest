
/// File : bd_comm_rc.h
/// Date : 2015/03

#ifndef __BD_COMM_RC_H__
#define __BD_COMM_RC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef BD_U32 BD_FR32;

typedef enum bdVENC_RC_MODE_E
{
	VENC_RC_MODE_H264CBR = 1,    
	VENC_RC_MODE_H264VBR,        
	VENC_RC_MODE_H264FIXQP,      

	VENC_RC_MODE_MJPEGFIXQP,  

	VENC_RC_MODE_BUTT,
}VENC_RC_MODE_E;

typedef struct bdVENC_ATTR_H264_FIXQP_S
{
	BD_U32      u32Gop;                 /*the interval of ISLICE. */
	BD_U32      u32ViFrmRate;           /* the input frame rate of the venc chnnel */
	BD_FR32     fr32TargetFrmRate ;     /* the target frame rate of the venc chnnel */     
	BD_U32      u32IQp;                 /* qp of the i frame */
	BD_U32      u32PQp;                 /* qp of the p frame */
} VENC_ATTR_H264_FIXQP_S;

typedef struct bdVENC_ATTR_H264_CBR_S
{
	BD_U32      u32Gop;                 /*the interval of ISLICE. */
	BD_U32      u32ViFrmRate;           /* the input frame rate of the venc chnnel */
	BD_FR32     fr32TargetFrmRate ;     /* the target frame rate of the venc chnnel */ 
	BD_U32      u32BitRate;             /* average bitrate */
	BD_U32      u32MaxQp;               /* the max qp */
	BD_U32      u32MinQp;               /* the min qp */
	BD_BOOL		bDisableBitStuffing;	/* bit stuffing or not */
} VENC_ATTR_H264_CBR_S;                                         
	 
typedef struct bdVENC_ATTR_H264_VBR_S
{
	BD_U32      u32Gop;                 /*the interval of ISLICE. */
	BD_U32      u32ViFrmRate;           /* the input frame rate of the venc chnnel */
	BD_FR32     fr32TargetFrmRate ;     /* the target frame rate of the venc chnnel */     
	BD_U32      u32MaxBitRate;          /* the max bitrate */                      
	BD_U32      u32MaxQp;               /* the max qp */
	BD_U32      u32MinQp;               /* the min qp */
}VENC_ATTR_H264_VBR_S;

typedef struct bdVENC_ATTR_MJPEG_FIXQP_S
{
	BD_U32      u32ViFrmRate;           /* the input frame rate of the venc chnnel */
	BD_FR32     fr32TargetFrmRate;      /* the target frame rate of the venc chnnel */
	BD_U32      u32Qfactor;             /* image quality :[1,99]*/
}VENC_ATTR_MJPEG_FIXQP_S;

typedef struct bdVENC_RC_ATTR_S
{
	VENC_RC_MODE_E enRcMode;            /*the type of rc*/
	union
	{
		VENC_ATTR_H264_CBR_S    stAttrH264Cbr;
		VENC_ATTR_H264_VBR_S    stAttrH264Vbr;
		VENC_ATTR_H264_FIXQP_S  stAttrH264FixQp;
		VENC_ATTR_MJPEG_FIXQP_S stAttrMjpegeFixQp;
	};   
}VENC_RC_ATTR_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __BD_COMM_RC_H__ */
