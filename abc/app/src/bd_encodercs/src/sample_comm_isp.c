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
#include <pthread.h>

#include "sample_comm.h"

#define CAM0_I2C	SENSOR_CAM0_I2C
#define CAM0_SPWDN	SENSOR_CAM0_SPWDN

#if defined(SENSOR_CAM1_I2C)
#define CAM1_I2C	SENSOR_CAM1_I2C
#else
#define CAM1_I2C	SENSOR_CAM0_I2C
#endif

#if defined(SENSOR_CAM1_SPWDN)
#define CAM1_SPWDN	SENSOR_CAM1_SPWDN
#else
#define CAM1_SPWDN	SENSOR_CAM0_SPWDN
#endif

#if defined(SENSOR_CAM0_ADDR) 
#define CAM0_ADDR	SENSOR_CAM0_ADDR
#endif

#if defined(SENSOR_CAM1_ADDR)
#define CAM1_ADDR	SENSOR_CAM1_ADDR
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */


BD_S32 SAMPLE_COMM_ISP_GetSensorIndex(SAMPLE_VI_MODE_E enViMode)
{
	switch(enViMode) {
    case LVDS_PS1210K_LVDS_1080P_30FPS:
    case LVDS_PS1210K_LVDS_720P_30FPS:
	case CAM0_PS1210K_BAYER_1080P_30FPS:
    case CAM0_PS1210K_BAYER_720P_30FPS:
		return SNS_PS1210K;

    case CAM0_PS1210M_BAYER_1080P_30FPS:
    case CAM0_PS1210M_BAYER_720P_30FPS:
		return SNS_PS1210M;

    case LVDS_PS3210M_LVDS_1080P_30FPS:
    case LVDS_PS3210M_LVDS_720P_30FPS:
    case CAM0_PS3210M_BAYER_1080P_30FPS:
    case CAM0_PS3210M_BAYER_720P_30FPS:
		return SNS_PS3210M; 

    case CAM0_PS3210P_BAYER_1080P_30FPS:
    case CAM0_PS3210P_BAYER_720P_30FPS:
		return SNS_PS3210P;

    case CAM0_PO3100K_BT1120_1080P_30FPS:
    case CAM0_PO3100K_BT1120_720P_30FPS:
		return SNS_PO3100K;

	case CAM0_PC7030K_BT656_480I_30FPS:
    case CAM0_PC7030K_BT656_576I_30FPS:
	case CAM1_PC7030K_BT656_480I_30FPS:
    case CAM1_PC7030K_BT656_576I_30FPS:
		return SNS_PC7030K;

    case CAM1_PC3089N_BT601_480I_30FPS:
    case CAM1_PC3089N_BT601_480P_30FPS:
		return SNS_PC3089N;
    
	case LVDS_PS3210K_LVDS_1080P_30FPS:
    case LVDS_PS3210K_LVDS_720P_30FPS:
	case CAM0_PS3210K_BAYER_1080P_30FPS:
    case CAM0_PS3210K_BAYER_720P_30FPS:
		return SNS_PS3210K;
    
	case LVDS_PS4100K_LVDS_720P_30FPS:
    case CAM0_PS4100K_BAYER_720P_30FPS:
		return SNS_PS4100K;
	
	case LVDS_PS7100K_LVDS_720P_30FPS:
    case CAM0_PS7100K_BAYER_720P_30FPS:
		return SNS_PS7100K;
	
	/* PVI port */
	case PVI_ANALOG_SD_480I_30FPS:
	case PVI_ANALOG_SD_576I_30FPS:
	case PVI_ANALOG_PVI_720P_30FPS:
	case PVI_ANALOG_PVI_1080P_30FPS:
	case PVI_ANALOG_CVI_720P_30FPS:
	case PVI_ANALOG_CVI_1080P_30FPS:
	case PVI_ANALOG_HDA_720P_30FPS:
	case PVI_ANALOG_HDA_1080P_30FPS:
	case PVI_ANALOG_HDT_720P_30FPS:
	case PVI_ANALOG_HDT_1080P_30FPS:
	default:
		return -1;
	}
	return -1;
}

static BD_BOOL SAMPLE_COMM_ISP_FullHD(SAMPLE_VI_MODE_E enViMode)
{
	BD_BOOL bFullSize = BD_FALSE;

	switch (enViMode) {
        case LVDS_PS1210K_LVDS_1080P_30FPS:
        case LVDS_PS3210M_LVDS_1080P_30FPS:
        case LVDS_PS3210K_LVDS_1080P_30FPS:
		case CAM0_PS1210K_BAYER_1080P_30FPS:
		case CAM0_PS1210M_BAYER_1080P_30FPS:
		case CAM0_PS3210M_BAYER_1080P_30FPS:
		case CAM0_PS3210P_BAYER_1080P_30FPS:
		case CAM0_PS3210K_BAYER_1080P_30FPS:
			bFullSize = BD_TRUE;
			break;
		default: 
			break;	
	}

	return bFullSize;
}
/******************************************************************************
* funciton : Sensor init
* note : different sensor corresponding different lib. So,  you can change
		  SENSOR_CAM0_TYPE in Makefile.para, instead of modify program code.
******************************************************************************/
BD_S32 SAMPLE_COMM_ISP_SensorInit(SAMPLE_VI_MODE_E enViMode, VIDEO_NORM_E enNorm)
{
    BD_S32 s32Ret, s32SensorIndex = -1;
	BD_U32 u32Scan = 0, u32Slot = 2, u32Spwdn, u32Interface, u32I2cChn, u8ForcedAddr;

    switch (enViMode)
    {
		case LVDS_PS1210K_LVDS_1080P_30FPS:
		case LVDS_PS1210K_LVDS_720P_30FPS:
		case LVDS_PS3210M_LVDS_1080P_30FPS:
		case LVDS_PS3210M_LVDS_720P_30FPS:
		case LVDS_PS3210K_LVDS_1080P_30FPS:
		case LVDS_PS3210K_LVDS_720P_30FPS:
		case LVDS_PS4100K_LVDS_720P_30FPS:
		case LVDS_PS7100K_LVDS_720P_30FPS:
			u32Scan = SNS_SCAN_NTSC;
			u32Interface = SNS_INTF_LVDS;
			u32Slot = 0;
			break;

		case CAM0_PS1210K_BAYER_1080P_30FPS:
		case CAM0_PS1210K_BAYER_720P_30FPS:
		case CAM0_PS1210M_BAYER_1080P_30FPS:
		case CAM0_PS1210M_BAYER_720P_30FPS:
		case CAM0_PS3210M_BAYER_1080P_30FPS:
		case CAM0_PS3210M_BAYER_720P_30FPS:
		case CAM0_PS3210P_BAYER_1080P_30FPS:
		case CAM0_PS3210P_BAYER_720P_30FPS:
		case CAM0_PS3210K_BAYER_1080P_30FPS:
		case CAM0_PS3210K_BAYER_720P_30FPS:
		case CAM0_PS4100K_BAYER_720P_30FPS:
		case CAM0_PS7100K_BAYER_720P_30FPS:
			u32Scan = SNS_SCAN_NTSC;
			u32Interface = SNS_INTF_BAYER;
			u32Slot = 0;
			break;

		case CAM0_PO3100K_BT1120_1080P_30FPS:
		case CAM0_PO3100K_BT1120_720P_30FPS:
			u32Scan = SNS_SCAN_NTSC;
			u32Interface = SNS_INTF_BT1120;
			u32Slot = 0;
			break;

		case CAM0_PC7030K_BT656_480I_30FPS:
			if (enNorm == VIDEO_ENCODING_MODE_PAL) {
				printf("%s: invalid configuration: Sensor Scanmode (NTSC), User setting (PAL)!\n", 
					__FUNCTION__);
				return BD_FAILURE;
			}
			u32Scan = SNS_SCAN_NTSC;
			u32Interface = SNS_INTF_BT656;
			u32Slot = 0;
            break;

		case CAM0_PC7030K_BT656_576I_30FPS:
			if (enNorm == VIDEO_ENCODING_MODE_NTSC) {
				printf("%s: invalid configuration: Sensor Scanmode (PAL), User setting (NTSC)!\n", 
					__FUNCTION__);
				return BD_FAILURE;
			}
			u32Scan = SNS_SCAN_PAL;
			u32Interface = SNS_INTF_BT656;
			u32Slot = 0;
            break;

		case CAM1_PC7030K_BT656_480I_30FPS:
			if (enNorm == VIDEO_ENCODING_MODE_PAL) {
				printf("%s: invalid configuration: Sensor Scanmode (NTSC), User setting (PAL)!\n", 
					__FUNCTION__);
				return BD_FAILURE;
			}
			u32Scan = SNS_SCAN_NTSC;
			u32Interface = SNS_INTF_BT656;
			u32Slot = 1;
            break;

		case CAM1_PC7030K_BT656_576I_30FPS:
			if (enNorm == VIDEO_ENCODING_MODE_NTSC) {
				printf("%s: invalid configuration: Sensor Scanmode (PAL), User setting (NTSC)!\n", 
					__FUNCTION__);
				return BD_FAILURE;
			}
			u32Scan = SNS_SCAN_PAL;
			u32Interface = SNS_INTF_BT656;
			u32Slot = 1;
            break;

		case CAM1_PC3089N_BT601_480I_30FPS:
		case CAM1_PC3089N_BT601_480P_30FPS:
			if (enNorm == VIDEO_ENCODING_MODE_PAL) {
				printf("%s: invalid configuration: Sensor Scanmode (NTSC), User setting (PAL)!\n", 
					__FUNCTION__);
				return BD_FAILURE;
			}
			u32Scan = SNS_SCAN_NTSC;
			u32Interface = SNS_INTF_BT601;
			u32Slot = 1;
            break;

        default:
	        return BD_FAILURE;
    }

	s32SensorIndex = SAMPLE_COMM_ISP_GetSensorIndex(enViMode);	
	if (s32SensorIndex == -1) {
        printf("%s: ViMode (%d) : Not Supported Sensor \n", __FUNCTION__, enViMode);
		return BD_FAILURE;
	}

#if defined(USE_SENSOR_25FPS) 
	if (BD_MPI_Sensor_FrameRate() != 25) {
        printf("%s: sensor library not support 25fps ! \n", __FUNCTION__);
		return BD_FAILURE;
	}
#else	
	if (BD_MPI_Sensor_FrameRate() != 30) {
        printf("%s: sensor library not support 30fps ! \n", __FUNCTION__);
		return BD_FAILURE;
	}
#endif	
		
	if (u32Slot == 0) {
		/* can be configurable */
		u32Spwdn = CAM0_SPWDN;
		u32I2cChn = CAM0_I2C;
		u8ForcedAddr = CAM0_ADDR;
	}
	else if (u32Slot == 1) {
		/* can be configurable */
		u32Spwdn = CAM1_SPWDN;
		u32I2cChn = CAM1_I2C;
		u8ForcedAddr = CAM1_ADDR;
	}
	else {
		/* error */
        printf("%s: invalid slot! \n", __FUNCTION__);
		return BD_FAILURE;
	}

	/* 1. sensor init */
	s32Ret = BD_MPI_Sensor_Init(u32Slot, 
			u32Spwdn, 
			s32SensorIndex, 
			(unsigned int)u32Scan, 
			u32Interface, 
			u32I2cChn, 
			u8ForcedAddr);
	if (s32Ret != BD_SUCCESS) {
		printf("%s: sensor_%u failed with %#x!\n", \
				__FUNCTION__, u32Slot, s32Ret);
		return s32Ret;
	}
	
    return BD_SUCCESS;
}

/******************************************************************************
* funciton : ISP init
******************************************************************************/
BD_S32 SAMPLE_COMM_ISP_Init(SAMPLE_VI_MODE_E enViMode, SAMPLE_ISPADDON_CONFIG_S* pstISPAddonConfig)
{
    BD_S32 s32Ret;
	BD_BOOL bSetupManr = BD_TRUE;
    ISP_IMAGE_ATTR_S stImageAttr;
    ISP_INPUT_TIMING_S stInputTiming;
	ISP_MANR_ATTR_S stManrAttr;
	ISP_DEHAZE_ATTR_S stDehazeAttr;
    
	if (enViMode >= CAM0_PO3100K_BT1120_1080P_30FPS) {
        printf("%s: isp libaray not support !\n", __FUNCTION__);
		return BD_FAILURE;
	}

#if defined(USE_SENSOR_25FPS)	
	if (BD_MPI_ISP_FrameRate() != 25) {
        printf("%s: isp libaray not support 25fps !\n", __FUNCTION__);
		return BD_FAILURE;
	}
#else	
	if (BD_MPI_ISP_FrameRate() != 30) {
        printf("%s: isp libaray not support 30fps !\n", __FUNCTION__);
		return BD_FAILURE;
	}
#endif

    /* 1. isp init */
    s32Ret = BD_MPI_ISP_Init();
    if (s32Ret != BD_SUCCESS)
    {
        printf("%s: BD_MPI_ISP_Init failed!\n", __FUNCTION__);
        return s32Ret;
    }

    /* 2. isp set image attributes */
    /* note : different sensor, different ISP_IMAGE_ATTR_S define.
              if the sensor you used is different, you can change
              ISP_IMAGE_ATTR_S definition */

    switch (enViMode)
    {
        case LVDS_PS1210K_LVDS_1080P_30FPS:
		case LVDS_PS3210M_LVDS_1080P_30FPS:
		case LVDS_PS3210K_LVDS_1080P_30FPS:
            stImageAttr.enBayer      = ISP_BAYER_FORMAT_BUTT;
            stImageAttr.u16FrameRate = 30;
            stImageAttr.u16Width     = 1920;
            stImageAttr.u16Height    = 1080;
			bSetupManr = BD_TRUE;
            break;

    	case LVDS_PS1210K_LVDS_720P_30FPS:
    	case LVDS_PS3210M_LVDS_720P_30FPS:
    	case LVDS_PS3210K_LVDS_720P_30FPS:
    	case LVDS_PS4100K_LVDS_720P_30FPS:
		case LVDS_PS7100K_LVDS_720P_30FPS:
            stImageAttr.enBayer      = ISP_BAYER_FORMAT_BUTT;
            stImageAttr.u16FrameRate = 30;
            stImageAttr.u16Width     = 1280;
            stImageAttr.u16Height    = 720;
			bSetupManr = BD_TRUE;
            break;

		case CAM0_PS1210K_BAYER_1080P_30FPS:
		case CAM0_PS1210M_BAYER_1080P_30FPS:
		case CAM0_PS3210M_BAYER_1080P_30FPS:
		case CAM0_PS3210P_BAYER_1080P_30FPS:
		case CAM0_PS3210K_BAYER_1080P_30FPS:
            stImageAttr.enBayer      = ISP_BAYER_FORMAT_GRBG;
            stImageAttr.u16FrameRate = 30;
            stImageAttr.u16Width     = 1920;
            stImageAttr.u16Height    = 1080;
			bSetupManr = pstISPAddonConfig->stManrAttr.bEnable;
            break;

		case CAM0_PS1210K_BAYER_720P_30FPS:
		case CAM0_PS1210M_BAYER_720P_30FPS:
		case CAM0_PS3210M_BAYER_720P_30FPS:
		case CAM0_PS3210P_BAYER_720P_30FPS:
		case CAM0_PS3210K_BAYER_720P_30FPS:
		case CAM0_PS4100K_BAYER_720P_30FPS:
		case CAM0_PS7100K_BAYER_720P_30FPS:
            stImageAttr.enBayer      = ISP_BAYER_FORMAT_GRBG;
            stImageAttr.u16FrameRate = 30;
            stImageAttr.u16Width     = 1280;
            stImageAttr.u16Height    = 720;
			bSetupManr = pstISPAddonConfig->stManrAttr.bEnable;
            break;

        default:
			printf("%s: isp libaray not support type !\n", __FUNCTION__);
			return BD_FAILURE;
    }

    s32Ret = BD_MPI_ISP_SetImageAttr(&stImageAttr);
    if (s32Ret != BD_SUCCESS)
    {
        printf("%s: BD_MPI_ISP_SetImageAttr failed with %#x!\n", __FUNCTION__, s32Ret);
        return s32Ret;
    }

    /* 3. isp set timing */
	stInputTiming.enWndMode = ISP_WIND_ALL;
	stInputTiming.u16HorWndStart = 8;
	stInputTiming.u16VerWndStart = 8;

	if (SAMPLE_COMM_ISP_FullHD(enViMode) == BD_TRUE) {
		stInputTiming.u16HorWndLength = 1920;
		stInputTiming.u16VerWndLength = 1080;
	}
	else {
		stInputTiming.u16HorWndLength = 1280;
		stInputTiming.u16VerWndLength = 720;
	}

    s32Ret = BD_MPI_ISP_SetInputTiming(&stInputTiming);
    if (s32Ret != BD_SUCCESS)
    {
        printf("%s: BD_MPI_ISP_SetInputTiming failed with %#x!\n", __FUNCTION__, s32Ret);
        return s32Ret;
    }


	if (!pstISPAddonConfig) {
		printf("%s: Invalid ISPAddonConfig pointer, need ISPAddonConfig handle\n", 
				__FUNCTION__);
		return BD_FAILURE;
	}

	if (pstISPAddonConfig->stManrAttr.u32Width != stImageAttr.u16Width ||
		pstISPAddonConfig->stManrAttr.u32Height != stImageAttr.u16Height) {
		printf("%s: ISPAddonConfig setup %dx%d, but ISP %dx%d\n", 
			__FUNCTION__, 
			pstISPAddonConfig->stManrAttr.u32Width, 
			pstISPAddonConfig->stManrAttr.u32Height,
			stImageAttr.u16Width,
			stImageAttr.u16Height);
		return BD_FAILURE;
	}
	
	/* setup 3DNR config: can't config runtime */	
	s32Ret = BD_MPI_ISP_GetManrAttr(&stManrAttr);
	if (s32Ret != BD_SUCCESS) {
		printf("%s: BD_MPI_ISP_GetManrAttr failed with %#x!\n", __FUNCTION__, s32Ret);
		return s32Ret;
	}

	if (bSetupManr == BD_TRUE) {
		if (memcmp(&stManrAttr, &pstISPAddonConfig->stManrAttr, sizeof(ISP_MANR_ATTR_S))) {
			/* setup 3DNR config: can't config runtime */	
			s32Ret = BD_MPI_ISP_SetManrAttr(&pstISPAddonConfig->stManrAttr);
			if (s32Ret != BD_SUCCESS)
			{
				printf("%s: BD_MPI_ISP_SetManrAttr failed with %#x!\n", __FUNCTION__, s32Ret);
				return s32Ret;
			}
		}

		if (pstISPAddonConfig->stManrAttr.bEnable == BD_TRUE) {
			s32Ret = BD_MPI_ISP_SetManrFilter(&pstISPAddonConfig->stManrFilter);
			if (s32Ret != BD_SUCCESS)
			{
				printf("%s: BD_MPI_ISP_SetManrFilter failed with %#x!\n", __FUNCTION__, s32Ret);
				return s32Ret;
			}
		}
	}
	
	s32Ret = BD_MPI_ISP_GetDehazeAttr(&stDehazeAttr);
	if (s32Ret != BD_SUCCESS) {
		printf("%s: BD_MPI_ISP_GetDehazeAttr failed width %d!\n", __FUNCTION__, s32Ret);
		return s32Ret;
	}

	if (memcmp(&stDehazeAttr, &pstISPAddonConfig->stDehazeAttr, sizeof(ISP_DEHAZE_ATTR_S))) {
		/* setup Dehaze config: can't config runtime */
		s32Ret = BD_MPI_ISP_SetDehazeAttr(&pstISPAddonConfig->stDehazeAttr);
		if (s32Ret != BD_SUCCESS) {
			printf("%s: BD_MPI_ISP_SetDehazeAttr failed width %d!\n", __FUNCTION__, s32Ret);
			return s32Ret;
		}
	}

    return BD_SUCCESS;
}

/******************************************************************************
* funciton : ISP Run
******************************************************************************/
BD_S32 SAMPLE_COMM_ISP_Run(SAMPLE_VI_MODE_E enViMode, SAMPLE_ISPADDON_CONFIG_S* pstISPAddonConfig)
{
    BD_S32 s32Ret;

    s32Ret = SAMPLE_COMM_ISP_Init(enViMode, pstISPAddonConfig);
    if (BD_SUCCESS != s32Ret)
    {
        printf("%s: ISP init failed!\n", __FUNCTION__);
        return BD_FAILURE;
    }

    return BD_SUCCESS;
}

/******************************************************************************
* funciton : stop ISP, and stop isp thread
******************************************************************************/
BD_S32 SAMPLE_COMM_ISP_SensorExit(SAMPLE_VI_MODE_E enViMode)
{
	BD_S32 s32Ret = BD_SUCCESS;
	BD_U32 u32Slot = 0;

    switch (enViMode)
    {
		case LVDS_PS1210K_LVDS_1080P_30FPS:
		case LVDS_PS1210K_LVDS_720P_30FPS:
		case LVDS_PS3210M_LVDS_1080P_30FPS:
		case LVDS_PS3210M_LVDS_720P_30FPS:
		case LVDS_PS3210K_LVDS_1080P_30FPS:
		case LVDS_PS3210K_LVDS_720P_30FPS:
		case LVDS_PS4100K_LVDS_720P_30FPS:
		case LVDS_PS7100K_LVDS_720P_30FPS:
		case CAM0_PS1210K_BAYER_1080P_30FPS:
		case CAM0_PS1210K_BAYER_720P_30FPS:
		case CAM0_PS1210M_BAYER_1080P_30FPS:
		case CAM0_PS1210M_BAYER_720P_30FPS:
		case CAM0_PS3210M_BAYER_1080P_30FPS:
		case CAM0_PS3210M_BAYER_720P_30FPS:
		case CAM0_PS3210P_BAYER_1080P_30FPS:
		case CAM0_PS3210P_BAYER_720P_30FPS:
		case CAM0_PS3210K_BAYER_1080P_30FPS:
		case CAM0_PS3210K_BAYER_720P_30FPS:
		case CAM0_PS4100K_BAYER_720P_30FPS:
		case CAM0_PS7100K_BAYER_720P_30FPS:
		case CAM0_PO3100K_BT1120_1080P_30FPS:
		case CAM0_PO3100K_BT1120_720P_30FPS:
		case CAM0_PC7030K_BT656_480I_30FPS:
		case CAM0_PC7030K_BT656_576I_30FPS:
			u32Slot = 0;
            break;

		case CAM1_PC7030K_BT656_480I_30FPS:
		case CAM1_PC7030K_BT656_576I_30FPS:
		case CAM1_PC3089N_BT601_480I_30FPS:
		case CAM1_PC3089N_BT601_480P_30FPS:
			u32Slot = 1;
            break;

        default:
			bd1_sdk_err("invalid enViMode = %d\n", enViMode);
	        return BD_FAILURE;
    }

	/* sensor power off */
	s32Ret = BD_MPI_Sensor_Exit(u32Slot);
	if (s32Ret != BD_SUCCESS) {
		bd1_sdk_err("failed to stop sensor\n");
		return BD_FAILURE;
	}

    return BD_SUCCESS;
}

BD_S32 SAMPLE_COMM_ISP_Stop(BD_VOID)
{
	BD_S32 s32Ret = BD_SUCCESS;
 
    s32Ret = BD_MPI_ISP_Exit();
	if (s32Ret != BD_SUCCESS) {
		bd1_sdk_err("failed to stop ISP\n");
		return BD_FAILURE;
	}
    return BD_SUCCESS;
}


BD_S32 SAMPLE_COMM_ISP_RequestI2CAccess(BD_U32 u32Timeout)
{
	BD_S32 s32Ret = BD_SUCCESS;

	/* Get I2c access */
	s32Ret = BD_MPI_ISP_RequestI2CAccess(u32Timeout);
	if (s32Ret != BD_SUCCESS) {
		bd1_sdk_err("request failed\n");
		return BD_FAILURE;
	}
	
	return BD_SUCCESS;
}

BD_S32 SAMPLE_COMM_ISP_ReleaseI2CAccess(BD_VOID)
{
	BD_S32 s32Ret = BD_SUCCESS;

	/* Get I2c access */
	s32Ret = BD_MPI_ISP_ReleaseI2CAccess();
	if (s32Ret != BD_SUCCESS) {
		bd1_sdk_err("release failed\n");
		return BD_FAILURE;
	}

	return BD_SUCCESS;
}


BD_S32 SAMPLE_COMM_ISP_ReadI2C(BD_U32 u32Chn, BD_U8 u8ChipAddr, BD_U8 u8RegAddr, BD_U8* u8Data, BD_U32 u32DataSize)
{
	BD_S32 s32Ret = BD_SUCCESS, i;

	/* 
		this should be called after SAMPLE_COMM_ISP_RequestI2CAccess() function 
		Or i2c operation will be complicted
	*/
	s32Ret = BD_MPI_ISP_QueryI2CAccess();
	if (s32Ret != BD_SUCCESS) {
		bd1_sdk_err("ISP i2c blocked\n");
		goto I2C_Read_Fail;
	}

	/* Open I2c device */
	s32Ret = BD_MPI_SYS_OpenI2CDevice(u32Chn);
	if (s32Ret != BD_SUCCESS) {
		goto I2C_Read_Fail;
	}

	/* set target I2c device address */
	s32Ret = BD_MPI_SYS_SetI2CSlave(u32Chn, u8ChipAddr);
	if (s32Ret != BD_SUCCESS) {
		bd1_sdk_err("slave addr failed\n");
		goto I2C_Read_Fail;
	}

	/* tranfer i2c read or write */
	for (i = 0; i < u32DataSize; i++) {
		s32Ret = BD_MPI_SYS_SetI2CRead(u32Chn, u8RegAddr, &u8Data[i]);
		if (s32Ret != BD_SUCCESS) {
			bd1_sdk_err("read operation failed\n");
			goto I2C_Read_Fail;
		}
	}

	/* close i2c device */
	BD_MPI_SYS_CloseI2CDevice(u32Chn);
	return BD_SUCCESS;

I2C_Read_Fail:		

	BD_MPI_SYS_CloseI2CDevice(u32Chn);
	return BD_FAILURE;
}

BD_S32 SAMPLE_COMM_ISP_WriteI2C(BD_U32 u32Chn, BD_U8 u8ChipAddr, BD_U8 u8RegAddr, BD_U8* u8Data, BD_U32 u32DataSize)
{
	BD_S32 s32Ret = BD_SUCCESS, i;
	
	/* 
		this should be called after SAMPLE_COMM_ISP_RequestI2CAccess() function 
		Or i2c operation will be complicted
	*/
	s32Ret = BD_MPI_ISP_QueryI2CAccess();
	if (s32Ret != BD_SUCCESS) {
		bd1_sdk_err("ISP i2c blocked\n");
		goto I2C_Write_Fail;
	}

	/* Open I2c device */
	s32Ret = BD_MPI_SYS_OpenI2CDevice(u32Chn);
	if (s32Ret != BD_SUCCESS) {
		goto I2C_Write_Fail;
	}

	/* set target I2c device address */
	s32Ret = BD_MPI_SYS_SetI2CSlave(u32Chn, u8ChipAddr);
	if (s32Ret != BD_SUCCESS) {
		goto I2C_Write_Fail;
	}

	/* tranfer i2c read or write */
	for (i = 0; i < u32DataSize; i++) {
		s32Ret = BD_MPI_SYS_SetI2CWrite(u32Chn, u8RegAddr, u8Data[i]);
		if (s32Ret != BD_SUCCESS) {
			goto I2C_Write_Fail;
		}
	}

	/* close i2c device */
	BD_MPI_SYS_CloseI2CDevice(u32Chn);
	return BD_SUCCESS;

I2C_Write_Fail:		

	BD_MPI_SYS_CloseI2CDevice(u32Chn);
	return BD_FAILURE;
}

BD_U32 SAMPLE_COMM_ISP_WaitEvent(BD_U32 u32Timeout)
{
	BD_U32 u32Event = 0;
	BD_S32 s32Ret = BD_SUCCESS;

	s32Ret = BD_MPI_ISP_EventDetect(u32Timeout, &u32Event);
	if (s32Ret != BD_SUCCESS) {
        printf("%s: BD_MPI_ISP_EventDetect failed with %#x!\n", __FUNCTION__, s32Ret);
		return 0;
	}

	return u32Event;
}


static BD_VOID SAMPLE_COMM_ISP_GetManrProperty(SAMPLE_VI_MODE_E enViMode, BD_U32* u32WActive, BD_U32* u32HActive, BD_U32* u32WTotal, BD_U32* u32HTotal, BD_U32* u32WKick, BD_U32* u32HKick, BD_BOOL bEnableManr, BD_BOOL b25FPS)
{
	BD_U32 u32Width, u32Height, u32TotalWidth, u32TotalHeight, u32XKick, u32YKick;

	switch (enViMode)
	{
		case LVDS_PS1210K_LVDS_720P_30FPS:
		case LVDS_PS3210M_LVDS_720P_30FPS:
		case LVDS_PS3210K_LVDS_720P_30FPS:
		case LVDS_PS4100K_LVDS_720P_30FPS:
		case LVDS_PS7100K_LVDS_720P_30FPS:
		case CAM0_PS1210K_BAYER_720P_30FPS:
		case CAM0_PS1210M_BAYER_720P_30FPS:
		case CAM0_PS3210M_BAYER_720P_30FPS:
		case CAM0_PS3210P_BAYER_720P_30FPS:
		case CAM0_PS3210K_BAYER_720P_30FPS:
		case CAM0_PS4100K_BAYER_720P_30FPS:
		case CAM0_PS7100K_BAYER_720P_30FPS:
			u32Width = 1280;
			u32Height = 720;
			if (enViMode == CAM0_PS7100K_BAYER_720P_30FPS || enViMode == LVDS_PS7100K_LVDS_720P_30FPS) {
				/* currently ps7100k does not support std 1650x750 frame width */
				if (b25FPS == BD_TRUE) {
					u32TotalWidth = 2880;
					if (bEnableManr == BD_FALSE) {
						u32XKick = 1600;
						u32YKick = 2;
					}
					else {
						u32XKick = 1700;
						u32YKick = 2;
					}
				}
				else {
					u32TotalWidth = 2400;
					if (bEnableManr == BD_FALSE) {
						u32XKick = 1100;
						u32YKick = 2;
					}
					else {
						u32XKick = 1200;
						u32YKick = 2;
					}
				}
			}
			else {		
				if (b25FPS == BD_TRUE) {
					u32TotalWidth = 1980;		// 25 Frame total width
					if (bEnableManr == BD_FALSE) {
						u32XKick = 700;
						u32YKick = 2;
					}
					else {
						u32XKick = 800;
						u32YKick = 2;
					}
				}
				else {
					u32TotalWidth = 1650;
					if (bEnableManr == BD_FALSE) {
						u32XKick = 700;
						u32YKick = 2;
					}
					else {
						u32XKick = 800;
						u32YKick = 2;
					}
				}
			}
			u32TotalHeight = 750;
			break;
		case LVDS_PS1210K_LVDS_1080P_30FPS:
		case LVDS_PS3210M_LVDS_1080P_30FPS:
		case LVDS_PS3210K_LVDS_1080P_30FPS:
		case CAM0_PS1210K_BAYER_1080P_30FPS:
		case CAM0_PS1210M_BAYER_1080P_30FPS:
		case CAM0_PS3210M_BAYER_1080P_30FPS:
		case CAM0_PS3210P_BAYER_1080P_30FPS:
		case CAM0_PS3210K_BAYER_1080P_30FPS:
		default:
			u32Width = 1920;
			u32Height = 1080;
			if (b25FPS == BD_TRUE) {
				u32TotalWidth = 2640;		// 25 Frame total width
				if (bEnableManr == BD_FALSE) {
					u32XKick = 700;
					u32YKick = 2;
				}
				else {
					u32XKick = 800;
					u32YKick = 2;
				}
			}
			else {
				u32TotalWidth = 2200;
				if (bEnableManr == BD_FALSE) {
					u32XKick = 300;
					u32YKick = 2;
				}
				else {
					u32XKick = 300;
					u32YKick = 2;
				}
			}
			u32TotalHeight = 1125;
			break;
	}

	if (u32WActive) *u32WActive = u32Width;
	if (u32HActive) *u32HActive = u32Height;
	if (u32WTotal) *u32WTotal = u32TotalWidth;
	if (u32HTotal) *u32HTotal = u32TotalHeight;
	if (u32WKick) *u32WKick = u32XKick;
	if (u32HKick) *u32HKick = u32YKick;

	return;
}

BD_S32 SAMPLE_COMM_ISP_FillConfig(SAMPLE_ISPADDON_CONFIG_S *pstISPAddonConfig, SAMPLE_VI_MODE_E enViMode, BD_BOOL bEnableManr, BD_BOOL bEnableDefocus, BD_BOOL bEnableDehaze, BD_BOOL bEnableMotion, BD_BOOL bEnableBlind, BD_BOOL bEnableNight, BD_BOOL bEnableDay)
{
	BD_BOOL bDisableAttr = BD_FALSE;
	BD_U32 u32Width, u32Height, u32TotalWidth, u32TotalHeight, u32XKick, u32YKick;

	if (!pstISPAddonConfig) {
		SAMPLE_PRT("invalid IspConfig\n");
		return BD_FAILURE;
	}
	
	memset(pstISPAddonConfig, 0, sizeof(SAMPLE_ISPADDON_CONFIG_S));

	if (!SAMPLE_COMM_VI_IsISPInput(enViMode)) {
		SAMPLE_PRT("ERROR : enViMode (%d) invalid : only support bayer / lvds sensors (forced disable attribute)\n", enViMode);
		bDisableAttr = BD_TRUE;
	}

	SAMPLE_COMM_ISP_GetManrProperty(enViMode, 
			&u32Width, &u32Height, &u32TotalWidth, &u32TotalHeight, &u32XKick, &u32YKick, 
			(bDisableAttr) ? BD_FALSE : bEnableManr, 
#if defined(USE_SENSOR_25FPS)	
			BD_TRUE
#else
			BD_FALSE
#endif
			);

	/* fill manr config */
	pstISPAddonConfig->stManrAttr.bEnable = (bDisableAttr) ? BD_FALSE : bEnableManr;
	pstISPAddonConfig->stManrAttr.u32SFilter = 2;	//1;
	pstISPAddonConfig->stManrAttr.u32SGain0 = 0x40;
	pstISPAddonConfig->stManrAttr.u32SGain1 = 0x80;		// from0x40
	pstISPAddonConfig->stManrAttr.u32TFilter = 1;
	pstISPAddonConfig->stManrAttr.u32TGain = 0x40;
	pstISPAddonConfig->stManrAttr.u32KFactor = 0x2;
	pstISPAddonConfig->stManrAttr.u16NVAR_B = 0x30;
	pstISPAddonConfig->stManrAttr.u16NVAR_A = 0x9E;
	pstISPAddonConfig->stManrAttr.u32Threshold = ((0x0200<<16)|(0x00<<8)|(0xff));
	pstISPAddonConfig->stManrAttr.u32Width = u32Width;
	pstISPAddonConfig->stManrAttr.u32Height = u32Height;
#if defined(USE_SENSOR_25FPS)	
	pstISPAddonConfig->stManrAttr.bFrame25 = BD_TRUE;		// 25 fps
#else
	pstISPAddonConfig->stManrAttr.bFrame25 = BD_FALSE;		// 25 fps
#endif

	pstISPAddonConfig->stManrAttr.u32TotalWidth = u32TotalWidth;		
	pstISPAddonConfig->stManrAttr.u32TotalHeight = u32TotalHeight;		
	pstISPAddonConfig->stManrAttr.u32XKick = u32XKick;		
	pstISPAddonConfig->stManrAttr.u32YKick = u32YKick;		
	
	pstISPAddonConfig->stManrFilter.bTFilterEnable = bEnableManr;
	pstISPAddonConfig->stManrFilter.bSFilterEnable = bEnableManr;
		
	/* fill defocus config */	
	pstISPAddonConfig->stDefocusAttr.bEnable = (bDisableAttr) ? BD_FALSE : bEnableDefocus;
	pstISPAddonConfig->stDefocusAttr.u32Level = 0x20;
	pstISPAddonConfig->stDefocusAttr.u32Spartial = 0x02;
	pstISPAddonConfig->stDefocusAttr.u32Temporal = 0x10;
	/* defocus full window */
	pstISPAddonConfig->stDefocusAttr.stRect.s32X = (u32Width/10);
	pstISPAddonConfig->stDefocusAttr.stRect.s32Y = (u32Height/10);
	pstISPAddonConfig->stDefocusAttr.stRect.u32Width = 
		(u32Width-(pstISPAddonConfig->stDefocusAttr.stRect.s32X<<1));
	pstISPAddonConfig->stDefocusAttr.stRect.u32Height = 
		(u32Height-(pstISPAddonConfig->stDefocusAttr.stRect.s32Y<<1));
	pstISPAddonConfig->stDefocusState.bFocused = BD_FALSE;

	/* fill dehaze config */	

	pstISPAddonConfig->stDehazeAttr.bEnable = (bDisableAttr) ? BD_FALSE : bEnableDehaze;
	pstISPAddonConfig->stDehazeAttr.enInputResolution = 
		((u32Height==720)?ISP_DEHAZE_RESOLUTION_720P:ISP_DEHAZE_RESOLUTION_1080P);
	// 0/1 : 720p, 2:1080i, 3:1080p
	pstISPAddonConfig->stDehazeAttr.enFilterType = ISP_DEHAZE_FILTER_3X3WEIGHT;	// + Type, X Type, 3x3 Type, 3x3 Weighted Type
	pstISPAddonConfig->stDehazeAttr.u16StartX = 0;
	pstISPAddonConfig->stDehazeAttr.u16EndX = u32Width;
	pstISPAddonConfig->stDehazeAttr.u16StartY = 0;
	pstISPAddonConfig->stDehazeAttr.u16EndY = u32Height;
	pstISPAddonConfig->stDehazeAttr.u8HazeTemporalSensitivity = 0x7;
	pstISPAddonConfig->stDehazeAttr.bHazeDetectAllWindow = BD_FALSE;
	pstISPAddonConfig->stDehazeAttr.bGainDetectAllWindow = BD_FALSE;
	pstISPAddonConfig->stDehazeAttr.u8GainThreshold = 0x1C;
	pstISPAddonConfig->stDehazeAttr.u8GainMininum = 0x30;
	pstISPAddonConfig->stDehazeAttr.u8DeviationThreshold = 0x1C;
	pstISPAddonConfig->stDehazeAttr.u8DeviationMinimum = 0x30;
	pstISPAddonConfig->stDehazeAttr.u8GainTemporalSensitivity = 0x2;
	pstISPAddonConfig->stDehazeAttr.u8AutoGainWeight = 0x7C;
	pstISPAddonConfig->stDehazeAttr.bAutoGainControl = BD_TRUE;
	pstISPAddonConfig->stDehazeAttr.u16ManualGainLevel = 0xA0;
	pstISPAddonConfig->stDehazeAttr.bDehazeAllWindow = BD_FALSE;
	pstISPAddonConfig->stDehazeAttr.bBWEEnable = BD_TRUE;	
	pstISPAddonConfig->stDehazeAttr.bYPeak = BD_TRUE;
	pstISPAddonConfig->stDehazeAttr.enOutRange = ISP_DEHAZE_OUTRANGE_1_254;	
	pstISPAddonConfig->stDehazeAttr.bYCInvert = BD_FALSE;
	pstISPAddonConfig->stDehazeAttr.bUseBlank = BD_FALSE;
	pstISPAddonConfig->stDehazeState.u32GainValue = 0;
	pstISPAddonConfig->stDehazeState.u32DehazeAverage = 0;
	pstISPAddonConfig->stDehazeState.u32DeviationAverage = 0;
	pstISPAddonConfig->stDehazeState.u32DeviationCoef = 0;

	/* fill motion detect config */	
	pstISPAddonConfig->stMotionDetectAttr.bEnable = (bDisableAttr) ? BD_FALSE : bEnableMotion;
	pstISPAddonConfig->stMotionDetectAttr.u16TemporalSensitivity = 0x03;
	pstISPAddonConfig->stMotionDetectAttr.u16SpatialSensitivity = 0x00;
	pstISPAddonConfig->stMotionDetectAttr.u16LevelSensitivity = 0x100;
	pstISPAddonConfig->stMotionDetectAttr.u8RefUpdatePeriod = 0;		// frame unit.

	pstISPAddonConfig->stMotionDetectMask.bClear = BD_FALSE;				// 0: set, 1: clear
	memset(pstISPAddonConfig->stMotionDetectMask.u32MaskBit, 0, sizeof(BD_U32)*24);
	memset(pstISPAddonConfig->stMotionDetectData.u32DetectBit, 0, 
		sizeof(pstISPAddonConfig->stMotionDetectData.u32DetectBit));	// [i][j]:  i: vertical (total 48line), j[0]:msb, j[1]:lsb (total 64bit)

	/* fill blind detect config */	
	pstISPAddonConfig->stBlindDetectAttr.bEnable = (bDisableAttr) ? BD_FALSE : bEnableBlind;
	pstISPAddonConfig->stBlindDetectAttr.u16TemporalSensitivity = 0x03;
	pstISPAddonConfig->stBlindDetectAttr.u16SpatialSensitivity = 0x00;
	pstISPAddonConfig->stBlindDetectAttr.u16LevelSensitivity = 0x100;
	pstISPAddonConfig->stBlindDetectAttr.u8RefUpdatePeriod = pstISPAddonConfig->stMotionDetectAttr.u8RefUpdatePeriod;
	pstISPAddonConfig->stBlindDetectState.bBlinded = BD_FALSE;
	
	pstISPAddonConfig->stBlindDetectMask.bClear = BD_FALSE;				// 0: set, 1: clear
	memset(pstISPAddonConfig->stBlindDetectMask.u32MaskBit, 0, sizeof(BD_U32)*24);

	/* fill night detect config */	
	pstISPAddonConfig->stNightDetectAttr.bEnable = (bDisableAttr) ? BD_FALSE : bEnableNight;
	pstISPAddonConfig->stNightDetectAttr.u16TemporalSensitivity = 0x03;
	pstISPAddonConfig->stNightDetectAttr.u16LevelSensitivity = 0x300;
	pstISPAddonConfig->stNightDetectAttr.u8RefUpdatePeriod = pstISPAddonConfig->stMotionDetectAttr.u8RefUpdatePeriod;	
	pstISPAddonConfig->stNightDetectState.bNightScene = BD_FALSE;

	/* fill day detect config */	
	pstISPAddonConfig->stDayDetectAttr.bEnable = (bDisableAttr) ? BD_FALSE : bEnableDay;
	pstISPAddonConfig->stDayDetectAttr.u16TemporalSensitivity = 0x03;
	pstISPAddonConfig->stDayDetectAttr.u16LevelSensitivity = 0x900;
	pstISPAddonConfig->stDayDetectAttr.u8RefUpdatePeriod = pstISPAddonConfig->stMotionDetectAttr.u8RefUpdatePeriod;
	pstISPAddonConfig->stDayDetectState.bDayScene = BD_FALSE;

	return BD_SUCCESS;
}


//#define DIRECT_DRAW			1

static pthread_t g_IspDetectThrdID;
static BD_BOOL bThrdExitCondition = BD_FALSE;
static BD_BOOL bDetectThrdState = BD_FALSE;
#ifndef DIRECT_DRAW			
static pthread_t g_IspDrawThrdID;
static BD_BOOL bDrawThrdState = BD_FALSE;
static BD_BOOL bMotionDraw = BD_FALSE;
ISP_EVENT_DETECT_MASK u32EventMask = ISP_EVENT_DETECT_NONE;
#endif

BD_VOID* SAMPLE_COMM_ISP_DetectThreadFunc(BD_VOID* p)
{
	SAMPLE_ISPADDON_CONFIG_S *pstISPAddonConfig = (SAMPLE_ISPADDON_CONFIG_S*)p;
	BD_S32 s32Ret = BD_SUCCESS;
	//	BD_U32 u32LoopCount = 0, u32MaxLoopCount = 100000;
#ifdef DIRECT_DRAW			
	BD_S32 i, j;
	BD_U32 u32MSB, u32LSB;
	ISP_EVENT_DETECT_MASK u32EventMask = ISP_EVENT_DETECT_NONE;
#endif

	bDetectThrdState = BD_TRUE;

	SAMPLE_PRT("@@@@@@@@@@@@@ Addon Detect Thread Created @@@@@@@@@@@@@@@\n");

	while (!bThrdExitCondition) {

		s32Ret = BD_MPI_ISP_EventDetect(1000, &u32EventMask);
		if (s32Ret == BD_SUCCESS) {

			if (u32EventMask & ISP_EVENT_DETECT_MOTION) {
				/* motion detected */
				s32Ret = BD_MPI_ISP_GetMotionDetectInfo(&pstISPAddonConfig->stMotionDetectData);
				if (s32Ret != BD_SUCCESS) {
					SAMPLE_PRT("failed to get motion region %d!\n", s32Ret);
				}
#ifdef DIRECT_DRAW			
				SAMPLE_PRT("Motion Deteced --------------------------\n");

				for (j = 0; j < 66; j++) {
					fprintf(stderr, "_");
				}
				fprintf(stderr, "\n");
				for (i = 47; i >= 0; i--) {
					u32MSB = (pstISPAddonConfig->stMotionDetectData.u32DetectBit[0][i]);
					u32LSB = (pstISPAddonConfig->stMotionDetectData.u32DetectBit[1][i]);
					fprintf(stderr, "|");
					for (j = 0; j < 32; j++) {
						if (u32MSB & 0x80000000) {
							fprintf(stderr, "x");
						}
						else {
							fprintf(stderr, " ");
						}
						u32MSB = (u32MSB<<1);
					}
					for (j = 0; j < 32; j++) {
						if (u32LSB & 0x80000000) {
							fprintf(stderr, "x");
						}
						else {
							fprintf(stderr, " ");
						}
						u32LSB = (u32LSB<<1);
					}
					fprintf(stderr, "|\n");
				}
				for (j = 0; j < 66; j++) {
					fprintf(stderr, "_");
				}
				fprintf(stderr, "\n");
#else				
				bMotionDraw = BD_TRUE;
#endif				
			}
			else if (u32EventMask & ISP_EVENT_DETECT_BLIND) {
				/* blind detected */
				s32Ret = BD_MPI_ISP_GetBlindDetectInfo(&pstISPAddonConfig->stBlindDetectState);
				if (s32Ret != BD_SUCCESS) {
					SAMPLE_PRT("failed to get blind state %d!\n", s32Ret);
				}
#ifdef DIRECT_DRAW			
				if (pstISPAddonConfig->stBlindDetectState.bBlinded == BD_TRUE) {
					SAMPLE_PRT("EVENT: Blinded State\n");
				}				
#endif			
			}
			else if (u32EventMask & ISP_EVENT_DETECT_NIGHT) {
				/* night detected */
				s32Ret = BD_MPI_ISP_GetNightDetectInfo(&pstISPAddonConfig->stNightDetectState);
				if (s32Ret != BD_SUCCESS) {
					SAMPLE_PRT("failed to get night state %d!\n", s32Ret);
				}
#ifdef DIRECT_DRAW			
				if (pstISPAddonConfig->stNightDetectState.bNightScene == BD_TRUE) {
					SAMPLE_PRT("EVENT: Night State\n");
				}
#endif
			}
			else if (u32EventMask & ISP_EVENT_DETECT_DAY) {
				/* day detected */
				s32Ret = BD_MPI_ISP_GetDayDetectInfo(&pstISPAddonConfig->stDayDetectState);
				if (s32Ret != BD_SUCCESS) {
					SAMPLE_PRT("failed to get day state %d!\n", s32Ret);
				}
#ifdef DIRECT_DRAW			
				if (pstISPAddonConfig->stDayDetectState.bDayScene == BD_TRUE) {
					SAMPLE_PRT("EVENT: Day State\n");
				}
#endif	
			}
			else if (u32EventMask & ISP_EVENT_DETECT_DEFOCUS) {
				/* defocus detected */
				s32Ret = BD_MPI_ISP_GetDefocusDetectInfo(&pstISPAddonConfig->stDefocusState);
				if (s32Ret != BD_SUCCESS) {
					SAMPLE_PRT("failed to get defocus state %d!\n", s32Ret);
				}
#ifdef DIRECT_DRAW			
				if (pstISPAddonConfig->stDefocusState.bFocused == BD_TRUE) {
					SAMPLE_PRT("EVENT: Focused\n");
				}
				else {
					SAMPLE_PRT("EVENT: Un-Focused\n");
				}
#endif
			}
		}
		usleep(1000);
	}

	bDetectThrdState = BD_FALSE;

	return NULL;
}

#ifndef DIRECT_DRAW			
BD_VOID* SAMPLE_COMM_ISP_DrawThreadFunc(BD_VOID* p)
{
	SAMPLE_ISPADDON_CONFIG_S *pstISPAddonConfig = (SAMPLE_ISPADDON_CONFIG_S*)p;
	BD_S32 i, j;
	BD_U32 u32MSB, u32LSB;

	bDrawThrdState = BD_TRUE;

	SAMPLE_PRT("@@@@@@@@@@@@@ Addon Draw Thread Created @@@@@@@@@@@@@@@\n");

	while (!bThrdExitCondition) {

		if (bMotionDraw == BD_TRUE) {
			SAMPLE_PRT("Motion Deteced --------------------------\n");

			for (j = 0; j < 66; j++) {
				printf("_");
			}
			printf("\n");
			for (i = 47; i >= 0; i--) {
				u32MSB = (pstISPAddonConfig->stMotionDetectData.u32DetectBit[0][i]);
				u32LSB = (pstISPAddonConfig->stMotionDetectData.u32DetectBit[1][i]);
				printf("|");
				for (j = 0; j < 32; j++) {
					if (u32MSB & 0x80000000) {
						printf("x");
					}
					else {
						printf(" ");
					}
					u32MSB = (u32MSB<<1);
				}
				for (j = 0; j < 32; j++) {
					if (u32LSB & 0x80000000) {
						printf("x");
					}
					else {
						printf(" ");
					}
					u32LSB = (u32LSB<<1);
				}
				printf("|\n");
			}
			for (j = 0; j < 66; j++) {
				printf("_");
			}
			printf("\n");

			bMotionDraw = BD_FALSE;
		}
		else if (u32EventMask & ISP_EVENT_DETECT_BLIND) {
			/* blind detected */
			if (pstISPAddonConfig->stBlindDetectState.bBlinded == BD_TRUE) {
				SAMPLE_PRT("EVENT: Blinded State\n");
			}
		}
		else if (u32EventMask & ISP_EVENT_DETECT_NIGHT) {
			/* night detected */
			if (pstISPAddonConfig->stNightDetectState.bNightScene == BD_TRUE) {
				SAMPLE_PRT("EVENT: Night State\n");
			}
		}
		else if (u32EventMask & ISP_EVENT_DETECT_DAY) {
			/* day detected */
			if (pstISPAddonConfig->stDayDetectState.bDayScene == BD_TRUE) {
				SAMPLE_PRT("EVENT: Day State\n");
			}
		}
		else if (u32EventMask & ISP_EVENT_DETECT_DEFOCUS) {
			/* defocus detected */
			if (pstISPAddonConfig->stDefocusState.bFocused == BD_TRUE) {
				SAMPLE_PRT("EVENT: Focused\n");
			}
			else {
				SAMPLE_PRT("EVENT: Un-Focused\n");
			}
		}
		usleep(1000);
	}

	bDrawThrdState = BD_FALSE;

	return NULL;
}
#endif

BD_S32 SAMPLE_COMM_ISP_CreateThread(BD_VOID* ctx)
{
	SAMPLE_ISPADDON_CONFIG_S *pstISPAddonConfig = (SAMPLE_ISPADDON_CONFIG_S*)ctx;

	if (pstISPAddonConfig->stDefocusAttr.bEnable ||
			pstISPAddonConfig->stMotionDetectAttr.bEnable ||
			pstISPAddonConfig->stBlindDetectAttr.bEnable ||
			pstISPAddonConfig->stNightDetectAttr.bEnable ||
			pstISPAddonConfig->stDayDetectAttr.bEnable) {
		bThrdExitCondition = BD_FALSE;
		if (pthread_create(&g_IspDetectThrdID, NULL, &SAMPLE_COMM_ISP_DetectThreadFunc, (void*)ctx)) {
			return BD_FAILURE;
		}
#ifndef DIRECT_DRAW			
		if (pthread_create(&g_IspDrawThrdID, NULL, &SAMPLE_COMM_ISP_DrawThreadFunc, (void*)ctx)) {
			return BD_FAILURE;
		}
#endif		
	}
	return BD_SUCCESS;
}

BD_S32 SAMPLE_COMM_ISP_ExitThread(BD_VOID)
{
	BD_S32 status;
	if (g_IspDetectThrdID) {
		bThrdExitCondition = BD_TRUE;
		while (bDetectThrdState);
		pthread_join(g_IspDetectThrdID, (void**)&status);
		g_IspDetectThrdID = 0;
#ifndef DIRECT_DRAW			
		while (bDrawThrdState);
		pthread_join(g_IspDrawThrdID, (void**)&status);
		g_IspDrawThrdID = 0;
#endif		
	}
	return BD_SUCCESS;
}

#if 0
BD_S32 SAMPLE_COMM_ISP_StartManr(SAMPLE_ISPADDON_CONFIG_S* pstISPAddonConfig)
{
	/* should not config runtime, this should be config initial time */
	return BD_SUCCESS;
}
#endif

#if 0
BD_S32 SAMPLE_COMM_ISP_StartDehaze(SAMPLE_ISPADDON_CONFIG_S* pstISPAddonConfig)
{
	/* should not config runtime, this should be config initial time */
	return BD_SUCCESS;
}
#endif

BD_S32 SAMPLE_COMM_ISP_StartDefocus(SAMPLE_ISPADDON_CONFIG_S* pstISPAddonConfig)
{
	BD_S32 s32Ret;

	if (!pstISPAddonConfig)
		return BD_FAILURE;

	s32Ret = BD_MPI_ISP_SetDefocusAttr(&pstISPAddonConfig->stDefocusAttr);
	if (s32Ret != BD_SUCCESS) {
		SAMPLE_PRT("failed to set defocus  attribute %d!\n", s32Ret);
		return BD_FAILURE;
	}
	return BD_SUCCESS;
}

BD_S32 SAMPLE_COMM_ISP_StartMotion(SAMPLE_ISPADDON_CONFIG_S* pstISPAddonConfig, BD_U32* pDetectMask)
{
	BD_S32 s32Ret;

	if (!pstISPAddonConfig)
		return BD_FAILURE;

	s32Ret = BD_MPI_ISP_SetMotionDetectAttr(&pstISPAddonConfig->stMotionDetectAttr);
	if (s32Ret != BD_SUCCESS) {
		SAMPLE_PRT("failed to set motiondetect  attribute %d!\n", s32Ret);
		return BD_FAILURE;
	}

	/* motion masking clear : all window detection */	
	pstISPAddonConfig->stMotionDetectMask.bClear = BD_TRUE;
	s32Ret = BD_MPI_ISP_SetMotionMask(&pstISPAddonConfig->stMotionDetectMask);
	if (s32Ret != BD_SUCCESS) {
		SAMPLE_PRT("failed to clear motiondetect mask %d!\n", s32Ret);
		return BD_FAILURE;
	}

	if (pDetectMask) {
		/* add masking */
		pstISPAddonConfig->stMotionDetectMask.bClear = BD_FALSE;
		memcpy(pstISPAddonConfig->stMotionDetectMask.u32MaskBit, (BD_VOID*)pDetectMask, sizeof(BD_U32)*24);
		s32Ret = BD_MPI_ISP_SetMotionMask(&pstISPAddonConfig->stMotionDetectMask);
		if (s32Ret != BD_SUCCESS) {
			SAMPLE_PRT("failed to update motiondetect mask %d!\n", s32Ret);
			return BD_FAILURE;
		}
	}

	return BD_SUCCESS;
}

BD_S32 SAMPLE_COMM_ISP_StartBlind(SAMPLE_ISPADDON_CONFIG_S* pstISPAddonConfig, BD_U32* pDetectMask)
{
	BD_S32 s32Ret;

	if (!pstISPAddonConfig)
		return BD_FAILURE;

	s32Ret = BD_MPI_ISP_SetBlindDetectAttr(&pstISPAddonConfig->stBlindDetectAttr);
	if (s32Ret != BD_SUCCESS) {
		SAMPLE_PRT("failed to set blinddetect  attribute %d!\n", s32Ret);
		return BD_FAILURE;
	}

	pstISPAddonConfig->stBlindDetectMask.bClear = BD_TRUE;
	s32Ret = BD_MPI_ISP_SetBlindMask(&pstISPAddonConfig->stBlindDetectMask);
	if (s32Ret != BD_SUCCESS) {
		SAMPLE_PRT("failed to clear blinddetect mask %d!\n", s32Ret);
		return BD_FAILURE;
	}

	if (pDetectMask) {
		pstISPAddonConfig->stBlindDetectMask.bClear = BD_FALSE;
		memcpy(pstISPAddonConfig->stBlindDetectMask.u32MaskBit, (BD_VOID*)pDetectMask, sizeof(BD_U32)*24);
		s32Ret = BD_MPI_ISP_SetBlindMask(&pstISPAddonConfig->stBlindDetectMask);
		if (s32Ret != BD_SUCCESS) {
			SAMPLE_PRT("failed to update blinddetect mask %d!\n", s32Ret);
			return BD_FAILURE;
		}
	}
	return BD_SUCCESS;
}

BD_S32 SAMPLE_COMM_ISP_StartNight(SAMPLE_ISPADDON_CONFIG_S* pstISPAddonConfig)
{
	BD_S32 s32Ret;

	if (!pstISPAddonConfig)
		return BD_FAILURE;

	s32Ret = BD_MPI_ISP_SetNightDetectAttr(&pstISPAddonConfig->stNightDetectAttr);
	if (s32Ret != BD_SUCCESS) {
		SAMPLE_PRT("failed to set nightdetect  attribute %d!\n", s32Ret);
		return BD_FAILURE;
	}
	return BD_SUCCESS;
}

BD_S32 SAMPLE_COMM_ISP_StartDay(SAMPLE_ISPADDON_CONFIG_S* pstISPAddonConfig)
{
	BD_S32 s32Ret;

	if (!pstISPAddonConfig)
		return BD_FAILURE;

	s32Ret = BD_MPI_ISP_SetDayDetectAttr(&pstISPAddonConfig->stDayDetectAttr);
	if (s32Ret != BD_SUCCESS) {
		SAMPLE_PRT("failed to set daydetect  attribute %d!\n", s32Ret);
		return BD_FAILURE;
	}
	return BD_SUCCESS;
}

BD_S32 SAMPLE_COMM_ISP_GetSensorID(BD_U8 Addr)
{
	BD_S32 nRet = BD_SUCCESS;
	BD_U8 buf;
	BD_U16 dev_id, rev_id;

	nRet = SAMPLE_COMM_ISP_RequestI2CAccess(5000);
	if (nRet != BD_SUCCESS) {
		goto SensorID_Fail;
	}
	
	buf = 0x00;
	nRet = SAMPLE_COMM_ISP_WriteI2C(0, Addr, 0x03, &buf, 1);
	if (nRet != BD_SUCCESS) {
		goto SensorID_Fail;
	}
	
	nRet = SAMPLE_COMM_ISP_ReadI2C(0, Addr, 0x00, &buf, 1);
	if (nRet != BD_SUCCESS) {
		goto SensorID_Fail;
	}
	dev_id = buf << 8;
	
	nRet = SAMPLE_COMM_ISP_ReadI2C(0, Addr, 0x01, &buf, 1);
	if (nRet != BD_SUCCESS) {
		goto SensorID_Fail;
	}
	dev_id |= buf;

	nRet = SAMPLE_COMM_ISP_ReadI2C(0, Addr, 0x02, &buf, 1);
	if (nRet != BD_SUCCESS) {
		goto SensorID_Fail;
	}
	rev_id = buf;
	
	printf("*** : SNS: %04x(rev. %04x)\n", 
			dev_id, rev_id);

	nRet = SAMPLE_COMM_ISP_ReleaseI2CAccess();
	if (nRet != BD_SUCCESS) {
		goto SensorID_Fail;
	}
	return BD_SUCCESS;

SensorID_Fail:
	return BD_FAILURE;
}

BD_U16 SAMPLE_COMM_ISP_GetISPID(BD_U8 Addr)
{
	BD_S32 nRet = BD_SUCCESS;
	BD_U8 buf;
	BD_U16 dev_id, rev_id;

	nRet = SAMPLE_COMM_ISP_RequestI2CAccess(5000);
	if (nRet != BD_SUCCESS) {
		goto ISPID_Fail;
	}

	buf = 0x00;
	nRet = SAMPLE_COMM_ISP_WriteI2C(0, Addr, 0x03, &buf, 1);
	if (nRet != BD_SUCCESS) {
		goto ISPID_Fail;
	}

	nRet = SAMPLE_COMM_ISP_ReadI2C(0, Addr, 0x00, &buf, 1);
	if (nRet != BD_SUCCESS) {
		goto ISPID_Fail;
	}
	dev_id = buf << 8;
	
	nRet = SAMPLE_COMM_ISP_ReadI2C(0, Addr, 0x01, &buf, 1);
	if (nRet != BD_SUCCESS) {
		goto ISPID_Fail;
	}
	dev_id |= buf;

	nRet = SAMPLE_COMM_ISP_ReadI2C(0, Addr, 0x02, &buf, 1);
	if (nRet != BD_SUCCESS) {
		goto ISPID_Fail;
	}
	rev_id = buf;

	printf("*** : ISP: %04x(rev. %04x)\n", 
			dev_id, rev_id);

	nRet = SAMPLE_COMM_ISP_ReleaseI2CAccess();
	if (nRet != BD_SUCCESS) {
		goto ISPID_Fail;
	}
	return BD_SUCCESS;

ISPID_Fail:
	return BD_FAILURE;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

