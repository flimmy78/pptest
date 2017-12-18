/******************************************************************************
  Some simple Hisilicon BD3531 video input functions.

  Copyright (C), 2010-2011, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
    Modification:  2011-8 Created
******************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

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
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>

#include "sample_comm.h"

static VI_DEV_ATTR_S DEV_ATTR_INTERFACE[VI_MODE_BUTT] = {
	/* DEV_ATTR_PS1210K_LVDS_1080P */
	{
		VI_INTF_LVDS_1080P,		VI_PATH_ISP,	VI_DATASHIFT_BUTT,
		{	
			VI_PVI_FORMAT_BUTT,	0, 0, 0, 0,	VI_PVI_FREQUENCY_BUTT,	
		},
	},

	/*  DEV_ATTR_PS1210K_LVDS_720P */
	{
		VI_INTF_LVDS_720P,		VI_PATH_ISP,	VI_DATASHIFT_BUTT,
		{
			VI_PVI_FORMAT_BUTT,	0, 0, 0, 0,	VI_PVI_FREQUENCY_BUTT,
		},
	},

	/*  DEV_ATTR_PS3210M_LVDS_1080P */
	{
		VI_INTF_LVDS_1080P,		VI_PATH_ISP,	VI_DATASHIFT_BUTT,
		{
			VI_PVI_FORMAT_BUTT,	0, 0, 0, 0,	VI_PVI_FREQUENCY_BUTT,
		},
	},

	/*  DEV_ATTR_PS3210M_LVDS_720P */
	{
		VI_INTF_LVDS_720P,		VI_PATH_ISP,	VI_DATASHIFT_BUTT,
		{
			VI_PVI_FORMAT_BUTT,	0, 0, 0, 0,	VI_PVI_FREQUENCY_BUTT,
		},
	},

	/*  DEV_ATTR_PS3210K_LVDS_1080P */
	{
		VI_INTF_LVDS_1080P,		VI_PATH_ISP,	VI_DATASHIFT_BUTT,
		{
			VI_PVI_FORMAT_BUTT,	0, 0, 0, 0,	VI_PVI_FREQUENCY_BUTT,
		},
	},

	/*  DEV_ATTR_PS3210K_LVDS_720P */
	{
		VI_INTF_LVDS_720P,		VI_PATH_ISP,	VI_DATASHIFT_BUTT,
		{
			VI_PVI_FORMAT_BUTT,	0, 0, 0, 0,	VI_PVI_FREQUENCY_BUTT,
		},
	},

	/*  DEV_ATTR_PS4100K_LVDS_720P */
	{
		VI_INTF_LVDS_720P,		VI_PATH_ISP,	VI_DATASHIFT_BUTT,
		{
			VI_PVI_FORMAT_BUTT,	0, 0, 0, 0,	VI_PVI_FREQUENCY_BUTT,
		},
	},

	/*  DEV_ATTR_PS7100K_LVDS_720P */
	{
		VI_INTF_LVDS_720P,		VI_PATH_ISP,	VI_DATASHIFT_BUTT,
		{
			VI_PVI_FORMAT_BUTT,	0, 0, 0, 0,	VI_PVI_FREQUENCY_BUTT,
		},
	},

	/*  DEV_ATTR_PS1210K_BAYER_1080P */
	{
		VI_INTF_CAM0_BAYER_1080P,	VI_PATH_ISP,	VI_DATASHIFT_BAYER_2TO13,
		{
			VI_PVI_FORMAT_BUTT,	0, 0, 0, 0,	VI_PVI_FREQUENCY_BUTT,
		},
	},

	/*  DEV_ATTR_PS1210K_BAYER_720P */
	{
		VI_INTF_CAM0_BAYER_720P,	VI_PATH_ISP,	VI_DATASHIFT_BAYER_2TO13,
		{
			VI_PVI_FORMAT_BUTT,	0, 0, 0, 0,	VI_PVI_FREQUENCY_BUTT,
		},
	},

	/*  DEV_ATTR_PS1210M_BAYER_1080P : for FPGA */
	{
		VI_INTF_CAM0_BAYER_1080P,	VI_PATH_ISP,	/*VI_DATASHIFT_BAYER_2TO13*/VI_DATASHIFT_BAYER_8TO19,
		{
			VI_PVI_FORMAT_BUTT,	0, 0, 0, 0,	VI_PVI_FREQUENCY_BUTT,
		},
	},

	/*  DEV_ATTR_PS1210M_BAYER_720P */
	{
		VI_INTF_CAM0_BAYER_720P,	VI_PATH_ISP,	/*VI_DATASHIFT_BAYER_2TO13*/VI_DATASHIFT_BAYER_8TO19,
		{
			VI_PVI_FORMAT_BUTT,	0, 0, 0, 0,	VI_PVI_FREQUENCY_BUTT,
		},
	},

	/*  DEV_ATTR_PS3210M_BAYER_1080P */
	{
		VI_INTF_CAM0_BAYER_1080P,	VI_PATH_ISP,	VI_DATASHIFT_BAYER_2TO13,
		{
			VI_PVI_FORMAT_BUTT,	0, 0, 0, 0,	VI_PVI_FREQUENCY_BUTT,
		},
	},

	/*  DEV_ATTR_PS3210M_BAYER_720P */
	{
		VI_INTF_CAM0_BAYER_720P,	VI_PATH_ISP,	VI_DATASHIFT_BAYER_2TO13,
		{
			VI_PVI_FORMAT_BUTT,	0, 0, 0, 0,	VI_PVI_FREQUENCY_BUTT,
		},
	},

	/*  DEV_ATTR_PS3210P_BAYER_1080P */
	{
		VI_INTF_CAM0_BAYER_1080P,	VI_PATH_ISP,	VI_DATASHIFT_BAYER_2TO13,
		{
			VI_PVI_FORMAT_BUTT,	0, 0, 0, 0,	VI_PVI_FREQUENCY_BUTT,
		},
	},

	/*  DEV_ATTR_PS3210P_BAYER_720P */
	{
		VI_INTF_CAM0_BAYER_720P,	VI_PATH_ISP,	VI_DATASHIFT_BAYER_2TO13,
		{
			VI_PVI_FORMAT_BUTT,	0, 0, 0, 0,	VI_PVI_FREQUENCY_BUTT,
		},
	},

	/*  DEV_ATTR_PS3210K_BAYER_1080P */
	{
		VI_INTF_CAM0_BAYER_1080P,	VI_PATH_ISP,	VI_DATASHIFT_BAYER_2TO13,
		{
			VI_PVI_FORMAT_BUTT,	0, 0, 0, 0,	VI_PVI_FREQUENCY_BUTT,
		},
	},

	/*  DEV_ATTR_PS3210K_BAYER_720P */
	{
		VI_INTF_CAM0_BAYER_720P,	VI_PATH_ISP,	VI_DATASHIFT_BAYER_2TO13,
		{
			VI_PVI_FORMAT_BUTT,	0, 0, 0, 0,	VI_PVI_FREQUENCY_BUTT,
		},
	},

	/*  DEV_ATTR_PS4100K_BAYER_720P */
	{
		VI_INTF_CAM0_BAYER_720P,	VI_PATH_ISP,	VI_DATASHIFT_BAYER_2TO13,
		{
			VI_PVI_FORMAT_BUTT,	0, 0, 0, 0,	VI_PVI_FREQUENCY_BUTT,
		},
	},

	/*  DEV_ATTR_PS7100K_BAYER_720P */
	{
		VI_INTF_CAM0_BAYER_720P,	VI_PATH_ISP,	VI_DATASHIFT_BAYER_2TO13,
		{
			VI_PVI_FORMAT_BUTT,	0, 0, 0, 0,	VI_PVI_FREQUENCY_BUTT,
		},
	},

	/*  DEV_ATTR_PO3100K_1080P */
	{
		VI_INTF_CAM0_BT1120_1080P,	VI_PATH_BYPASS,	VI_DATASHIFT_BT1120_LOWCHIY,
		{
			VI_PVI_FORMAT_BUTT,	0, 0, 0, 0,	VI_PVI_FREQUENCY_BUTT,
		},
	},

	/*  DEV_ATTR_PO3100K_720P */
	{
		VI_INTF_CAM0_BT1120_720P,	VI_PATH_BYPASS,	VI_DATASHIFT_BT1120_LOWCHIY,
		{
			VI_PVI_FORMAT_BUTT,	0, 0, 0, 0,	VI_PVI_FREQUENCY_BUTT,
		},
	},

	/*  DEV_ATTR_PC7030K_480I_0 */
	{
		VI_INTF_CAM0_BT656_480I,	VI_PATH_BYPASS,	VI_DATASHIFT_BUTT,
		{
			VI_PVI_FORMAT_BUTT,	0, 0, 0, 0,	VI_PVI_FREQUENCY_BUTT,
		},
	},

	/*  DEV_ATTR_PC7030K_576I_0 */
	{
		VI_INTF_CAM0_BT656_576I,	VI_PATH_BYPASS,	VI_DATASHIFT_BUTT,
		{
			VI_PVI_FORMAT_BUTT,	0, 0, 0, 0,	VI_PVI_FREQUENCY_BUTT,
		},
	},

	/*  DEV_ATTR_PC7030K_480I_1 */
	{
		VI_INTF_CAM1_BT656_480I,	VI_PATH_BYPASS,	VI_DATASHIFT_BUTT,
		{
			VI_PVI_FORMAT_BUTT,	0, 0, 0, 0,	VI_PVI_FREQUENCY_BUTT,
		},
	},

	/*  DEV_ATTR_PC7030K_576I_1 */
	{
		VI_INTF_CAM1_BT656_576I,	VI_PATH_BYPASS,	VI_DATASHIFT_BUTT,
		{
			VI_PVI_FORMAT_BUTT,	0, 0, 0, 0,	VI_PVI_FREQUENCY_BUTT,
		},
	},

	/*  DEV_ATTR_PC3089N_480I */
	{
		VI_INTF_CAM1_BT601_480I,	VI_PATH_BYPASS,	VI_DATASHIFT_BUTT,
		{
			VI_PVI_FORMAT_BUTT,	0, 0, 0, 0,	VI_PVI_FREQUENCY_BUTT,
		},
	},

	/*  DEV_ATTR_PC3089N_480P */
	{
		VI_INTF_CAM1_BT601_480P,	VI_PATH_BYPASS,	VI_DATASHIFT_BUTT,
		{
			VI_PVI_FORMAT_BUTT,	0, 0, 0, 0,	VI_PVI_FREQUENCY_BUTT,
		},
	},


	/*  DEV_ATTR_PVI_480I */
	{
		VI_INTF_PVI_480I,	VI_PATH_BYPASS,		VI_DATASHIFT_BUTT,
		{
			VI_PVI_FORMAT_BUTT,	720, 0, 480, 0,	VI_PVI_FREQUENCY_BUTT,
		},
	},

	/*  DEV_ATTR_PVI_576I */
	{
		VI_INTF_PVI_576I,	VI_PATH_BYPASS,		VI_DATASHIFT_BUTT,
		{
			VI_PVI_FORMAT_BUTT,	720, 0, 576, 0,	VI_PVI_FREQUENCY_BUTT,
		},
	},

	/*  DEV_ATTR_PVI_PVI_720P */
	{
		VI_INTF_PVI_PVI_720P,	VI_PATH_BYPASS,	VI_DATASHIFT_BUTT,
		{
			VI_PVI_FORMAT_PVI,	1280, 0, 720, 0,
#if defined(USE_PVI_25FPS)
			VI_PVI_FREQUENCY_25HZ,		// changable 25/30/50/60
#else
			VI_PVI_FREQUENCY_30HZ,		// changable 25/30/50/60
#endif		
		},
	},

	/*  DEV_ATTR_PVI_PVI_1080P */
	{
		VI_INTF_PVI_PVI_1080P,	VI_PATH_BYPASS,	VI_DATASHIFT_BUTT,
		{
			VI_PVI_FORMAT_PVI,	1920, 0, 1080, 0,
#if defined(USE_PVI_25FPS)
			VI_PVI_FREQUENCY_25HZ,		// changable 25/30/50/60
#else
			VI_PVI_FREQUENCY_30HZ,		// changable 25/30/50/60
#endif		
		},
	},

	/*  DEV_ATTR_PVI_CVI_720P */
	{
		VI_INTF_PVI_CVI_720P,	VI_PATH_BYPASS,	VI_DATASHIFT_BUTT,
		{
			VI_PVI_FORMAT_CVI,	1280, 0, 720, 0,
#if defined(USE_PVI_25FPS)
			VI_PVI_FREQUENCY_25HZ,		// changable 25/30/50/60
#else
			VI_PVI_FREQUENCY_30HZ,		// changable 25/30/50/60
#endif
		},
	},

	/*  DEV_ATTR_PVI_CVI_1080P */
	{
		VI_INTF_PVI_CVI_1080P,	VI_PATH_BYPASS,	VI_DATASHIFT_BUTT,
		{
			VI_PVI_FORMAT_CVI,	1920, 0, 1080, 0,
#if defined(USE_PVI_25FPS)
			VI_PVI_FREQUENCY_25HZ,		// changable 25/30/50/60
#else
			VI_PVI_FREQUENCY_30HZ,		// changable 25/30/50/60
#endif		
		},
	},

	/*  DEV_ATTR_PVI_HDA_720P */
	{
		VI_INTF_PVI_HDA_720P,	VI_PATH_BYPASS,	VI_DATASHIFT_BUTT,
		{
			VI_PVI_FORMAT_HDA,	1280, 0, 720, 0,
#if defined(USE_PVI_25FPS)
			VI_PVI_FREQUENCY_25HZ,		// changable 25/30/50/60
#else
			VI_PVI_FREQUENCY_30HZ,		// changable 25/30/50/60
#endif
		},
	},

	/*  DEV_ATTR_PVI_HDA_1080P */
	{
		VI_INTF_PVI_HDA_1080P,	VI_PATH_BYPASS,	VI_DATASHIFT_BUTT,
		{
			VI_PVI_FORMAT_HDA,	1920, 0, 1080, 0,
#if defined(USE_PVI_25FPS)
			VI_PVI_FREQUENCY_25HZ,		// changable 25/30/50/60
#else
			VI_PVI_FREQUENCY_30HZ,		// changable 25/30/50/60
#endif
		},
	},

	/*  DEV_ATTR_PVI_HDT_720P */
	{
		VI_INTF_PVI_HDT_720P,	VI_PATH_BYPASS,	VI_DATASHIFT_BUTT,
		{
			VI_PVI_FORMAT_HDT,	1280, 0, 720, 0,
#if defined(USE_PVI_25FPS)
			VI_PVI_FREQUENCY_25HZ,		// changable 25/30/50/60
#else
			VI_PVI_FREQUENCY_30HZ,		// changable 25/30/50/60
#endif
		},
	},

	/*  DEV_ATTR_PVI_HDT_1080P */
	{
		VI_INTF_PVI_HDT_1080P,	VI_PATH_BYPASS,	VI_DATASHIFT_BUTT,
		{
			VI_PVI_FORMAT_HDT,
			1920, 0, 1080, 0,
#if defined(USE_PVI_25FPS)
			VI_PVI_FREQUENCY_25HZ,		// changable 25/30/50/60
#else
			VI_PVI_FREQUENCY_30HZ,		// changable 25/30/50/60
#endif
		},
	},

};

#if 0
VI_DEV_ATTR_S DEV_ATTR_PS1210K_LVDS_1080P =
{
	VI_INTF_LVDS_1080P,
	VI_PATH_ISP,
	VI_DATASHIFT_BUTT,
	{
		VI_PVI_FORMAT_BUTT,
		0, 0, 0, 0,
		VI_PVI_FREQUENCY_BUTT,
	},
};

VI_DEV_ATTR_S DEV_ATTR_PS1210K_LVDS_720P =
{
	VI_INTF_LVDS_720P,
	VI_PATH_ISP,
	VI_DATASHIFT_BUTT,
	{
		VI_PVI_FORMAT_BUTT,
		0, 0, 0, 0,
		VI_PVI_FREQUENCY_BUTT,
	},
};

VI_DEV_ATTR_S DEV_ATTR_PS3210M_LVDS_1080P =
{
	VI_INTF_LVDS_1080P,
	VI_PATH_ISP,
	VI_DATASHIFT_BUTT,
	{
		VI_PVI_FORMAT_BUTT,
		0, 0, 0, 0,
		VI_PVI_FREQUENCY_BUTT,
	},
};

VI_DEV_ATTR_S DEV_ATTR_PS3210M_LVDS_720P =
{
	VI_INTF_LVDS_720P,
	VI_PATH_ISP,
	VI_DATASHIFT_BUTT,
	{
		VI_PVI_FORMAT_BUTT,
		0, 0, 0, 0,
		VI_PVI_FREQUENCY_BUTT,
	},
};

VI_DEV_ATTR_S DEV_ATTR_PS3210K_LVDS_1080P =
{
	VI_INTF_LVDS_1080P,
	VI_PATH_ISP,
	VI_DATASHIFT_BUTT,
	{
		VI_PVI_FORMAT_BUTT,
		0, 0, 0, 0,
		VI_PVI_FREQUENCY_BUTT,
	},
};

VI_DEV_ATTR_S DEV_ATTR_PS3210K_LVDS_720P =
{
	VI_INTF_LVDS_720P,
	VI_PATH_ISP,
	VI_DATASHIFT_BUTT,
	{
		VI_PVI_FORMAT_BUTT,
		0, 0, 0, 0,
		VI_PVI_FREQUENCY_BUTT,
	},
};

VI_DEV_ATTR_S DEV_ATTR_PS4100K_LVDS_720P =
{
	VI_INTF_LVDS_720P,
	VI_PATH_ISP,
	VI_DATASHIFT_BUTT,
	{
		VI_PVI_FORMAT_BUTT,
		0, 0, 0, 0,
		VI_PVI_FREQUENCY_BUTT,
	},
};

VI_DEV_ATTR_S DEV_ATTR_PS7100K_LVDS_720P =
{
	VI_INTF_LVDS_720P,
	VI_PATH_ISP,
	VI_DATASHIFT_BUTT,
	{
		VI_PVI_FORMAT_BUTT,
		0, 0, 0, 0,
		VI_PVI_FREQUENCY_BUTT,
	},
};

VI_DEV_ATTR_S DEV_ATTR_PS1210K_BAYER_1080P =
{
	VI_INTF_CAM0_BAYER_1080P,
	VI_PATH_ISP,
	VI_DATASHIFT_BAYER_2TO13,
	{
		VI_PVI_FORMAT_BUTT,
		0, 0, 0, 0,
		VI_PVI_FREQUENCY_BUTT,
	},
};

VI_DEV_ATTR_S DEV_ATTR_PS1210K_BAYER_720P =
{
	VI_INTF_CAM0_BAYER_720P,
	VI_PATH_ISP,
	VI_DATASHIFT_BAYER_2TO13,
	{
		VI_PVI_FORMAT_BUTT,
		0, 0, 0, 0,
		VI_PVI_FREQUENCY_BUTT,
	},
};

VI_DEV_ATTR_S DEV_ATTR_PS1210M_BAYER_1080P =
{
	VI_INTF_CAM0_BAYER_1080P,
	VI_PATH_ISP,
#if 0		
	VI_DATASHIFT_BAYER_2TO13,
#else	
	/* for FPGA */
	VI_DATASHIFT_BAYER_8TO19,
#endif
	{
		VI_PVI_FORMAT_BUTT,
		0, 0, 0, 0,
		VI_PVI_FREQUENCY_BUTT,
	},
};

VI_DEV_ATTR_S DEV_ATTR_PS1210M_BAYER_720P =
{
	VI_INTF_CAM0_BAYER_720P,
	VI_PATH_ISP,
#if 0		
	VI_DATASHIFT_BAYER_2TO13,
#else	
	/* for FPGA */
	VI_DATASHIFT_BAYER_8TO19,
#endif
	{
		VI_PVI_FORMAT_BUTT,
		0, 0, 0, 0,
		VI_PVI_FREQUENCY_BUTT,
	},
};

VI_DEV_ATTR_S DEV_ATTR_PS3210M_BAYER_1080P =
{
	VI_INTF_CAM0_BAYER_1080P,
	VI_PATH_ISP,
	VI_DATASHIFT_BAYER_2TO13,
	{
		VI_PVI_FORMAT_BUTT,
		0, 0, 0, 0,
		VI_PVI_FREQUENCY_BUTT,
	},
};

VI_DEV_ATTR_S DEV_ATTR_PS3210M_BAYER_720P =
{
	VI_INTF_CAM0_BAYER_720P,
	VI_PATH_ISP,
	VI_DATASHIFT_BAYER_2TO13,
	{
		VI_PVI_FORMAT_BUTT,
		0, 0, 0, 0,
		VI_PVI_FREQUENCY_BUTT,
	},
};

VI_DEV_ATTR_S DEV_ATTR_PS3210P_BAYER_1080P =
{
	VI_INTF_CAM0_BAYER_1080P,
	VI_PATH_ISP,
	VI_DATASHIFT_BAYER_2TO13,
	{
		VI_PVI_FORMAT_BUTT,
		0, 0, 0, 0,
		VI_PVI_FREQUENCY_BUTT,
	},
};

VI_DEV_ATTR_S DEV_ATTR_PS3210P_BAYER_720P =
{
	VI_INTF_CAM0_BAYER_720P,
	VI_PATH_ISP,
	VI_DATASHIFT_BAYER_2TO13,
	{
		VI_PVI_FORMAT_BUTT,
		0, 0, 0, 0,
		VI_PVI_FREQUENCY_BUTT,
	},
};

VI_DEV_ATTR_S DEV_ATTR_PS3210K_BAYER_1080P =
{
	VI_INTF_CAM0_BAYER_1080P,
	VI_PATH_ISP,
	VI_DATASHIFT_BAYER_2TO13,
	{
		VI_PVI_FORMAT_BUTT,
		0, 0, 0, 0,
		VI_PVI_FREQUENCY_BUTT,
	},
};

VI_DEV_ATTR_S DEV_ATTR_PS3210K_BAYER_720P =
{
	VI_INTF_CAM0_BAYER_720P,
	VI_PATH_ISP,
	VI_DATASHIFT_BAYER_2TO13,
	{
		VI_PVI_FORMAT_BUTT,
		0, 0, 0, 0,
		VI_PVI_FREQUENCY_BUTT,
	},
};

VI_DEV_ATTR_S DEV_ATTR_PS4100K_BAYER_720P =
{
	VI_INTF_CAM0_BAYER_720P,
	VI_PATH_ISP,
	VI_DATASHIFT_BAYER_2TO13,
	{
		VI_PVI_FORMAT_BUTT,
		0, 0, 0, 0,
		VI_PVI_FREQUENCY_BUTT,
	},
};

VI_DEV_ATTR_S DEV_ATTR_PS7100K_BAYER_720P =
{
	VI_INTF_CAM0_BAYER_720P,
	VI_PATH_ISP,
	VI_DATASHIFT_BAYER_2TO13,
	{
		VI_PVI_FORMAT_BUTT,
		0, 0, 0, 0,
		VI_PVI_FREQUENCY_BUTT,
	},
};

VI_DEV_ATTR_S DEV_ATTR_PO3100K_1080P =
{
	VI_INTF_CAM0_BT1120_1080P,
	VI_PATH_BYPASS,
	VI_DATASHIFT_BT1120_LOWCHIY,
	{
		VI_PVI_FORMAT_BUTT,
		0, 0, 0, 0,
		VI_PVI_FREQUENCY_BUTT,
	},
};

VI_DEV_ATTR_S DEV_ATTR_PO3100K_720P =
{
	VI_INTF_CAM0_BT1120_720P,
	VI_PATH_BYPASS,
	VI_DATASHIFT_BT1120_LOWCHIY,
	{
		VI_PVI_FORMAT_BUTT,
		0, 0, 0, 0,
		VI_PVI_FREQUENCY_BUTT,
	},
};

VI_DEV_ATTR_S DEV_ATTR_PC7030K_480I_0 =
{
	VI_INTF_CAM0_BT656_480I,
	VI_PATH_BYPASS,
	VI_DATASHIFT_BUTT,
	{
		VI_PVI_FORMAT_BUTT,
		0, 0, 0, 0,
		VI_PVI_FREQUENCY_BUTT,
	},
};

VI_DEV_ATTR_S DEV_ATTR_PC7030K_576I_0 =
{
	VI_INTF_CAM0_BT656_576I,
	VI_PATH_BYPASS,
	VI_DATASHIFT_BUTT,
	{
		VI_PVI_FORMAT_BUTT,
		0, 0, 0, 0,
		VI_PVI_FREQUENCY_BUTT,
	},
};

VI_DEV_ATTR_S DEV_ATTR_PC7030K_480I_1 =
{
	VI_INTF_CAM1_BT656_480I,
	VI_PATH_BYPASS,
	VI_DATASHIFT_BUTT,
	{
		VI_PVI_FORMAT_BUTT,
		0, 0, 0, 0,
		VI_PVI_FREQUENCY_BUTT,
	},
};

VI_DEV_ATTR_S DEV_ATTR_PC7030K_576I_1 =
{
	VI_INTF_CAM1_BT656_576I,
	VI_PATH_BYPASS,
	VI_DATASHIFT_BUTT,
	{
		VI_PVI_FORMAT_BUTT,
		0, 0, 0, 0,
		VI_PVI_FREQUENCY_BUTT,
	},
};

VI_DEV_ATTR_S DEV_ATTR_PC3089N_480I =
{
	VI_INTF_CAM1_BT601_480I,
	VI_PATH_BYPASS,
	VI_DATASHIFT_BUTT,
	{
		VI_PVI_FORMAT_BUTT,
		0, 0, 0, 0,
		VI_PVI_FREQUENCY_BUTT,
	},
};

VI_DEV_ATTR_S DEV_ATTR_PC3089N_480P =
{
	VI_INTF_CAM1_BT601_480P,
	VI_PATH_BYPASS,
	VI_DATASHIFT_BUTT,
	{
		VI_PVI_FORMAT_BUTT,
		0, 0, 0, 0,
		VI_PVI_FREQUENCY_BUTT,
	},
};


VI_DEV_ATTR_S DEV_ATTR_PVI_480I =
{
	VI_INTF_PVI_480I,
	VI_PATH_BYPASS,
	VI_DATASHIFT_BUTT,
	{
		VI_PVI_FORMAT_BUTT,
		720, 0, 480, 0,				// delay value need some tuning
		VI_PVI_FREQUENCY_BUTT,
	},
};

VI_DEV_ATTR_S DEV_ATTR_PVI_576I =
{
	VI_INTF_PVI_576I,
	VI_PATH_BYPASS,
	VI_DATASHIFT_BUTT,
	{
		VI_PVI_FORMAT_BUTT,
		720, 0, 576, 0,				// delay value need some tuning
		VI_PVI_FREQUENCY_BUTT,
	},
};

VI_DEV_ATTR_S DEV_ATTR_PVI_PVI_720P =
{
	VI_INTF_PVI_PVI_720P,
	VI_PATH_BYPASS,
	VI_DATASHIFT_BUTT,
	{
		VI_PVI_FORMAT_PVI,
		1280, 0, 720, 0,
#if defined(USE_PVI_25FPS)
		VI_PVI_FREQUENCY_25HZ,		// changable 25/30/50/60
#else
		VI_PVI_FREQUENCY_30HZ,		// changable 25/30/50/60
#endif		
	},
};

VI_DEV_ATTR_S DEV_ATTR_PVI_PVI_1080P =
{
	VI_INTF_PVI_PVI_1080P,
	VI_PATH_BYPASS,
	VI_DATASHIFT_BUTT,
	{
		VI_PVI_FORMAT_PVI,
		1920, 0, 1080, 0,
#if defined(USE_PVI_25FPS)
		VI_PVI_FREQUENCY_25HZ,		// changable 25/30/50/60
#else
		VI_PVI_FREQUENCY_30HZ,		// changable 25/30/50/60
#endif		
	},
};

VI_DEV_ATTR_S DEV_ATTR_PVI_CVI_720P =
{
	VI_INTF_PVI_CVI_720P,
	VI_PATH_BYPASS,
	VI_DATASHIFT_BUTT,
	{
		VI_PVI_FORMAT_CVI,
		1280, 0, 720, 0,
#if defined(USE_PVI_25FPS)
		VI_PVI_FREQUENCY_25HZ,		// changable 25/30/50/60
#else
		VI_PVI_FREQUENCY_30HZ,		// changable 25/30/50/60
#endif
	},
};

VI_DEV_ATTR_S DEV_ATTR_PVI_CVI_1080P =
{
	VI_INTF_PVI_CVI_1080P,
	VI_PATH_BYPASS,
	VI_DATASHIFT_BUTT,
	{
		VI_PVI_FORMAT_CVI,
		1920, 0, 1080, 0,
#if defined(USE_PVI_25FPS)
		VI_PVI_FREQUENCY_25HZ,		// changable 25/30/50/60
#else
		VI_PVI_FREQUENCY_30HZ,		// changable 25/30/50/60
#endif		
	},
};

VI_DEV_ATTR_S DEV_ATTR_PVI_HDA_720P =
{
	VI_INTF_PVI_HDA_720P,
	VI_PATH_BYPASS,
	VI_DATASHIFT_BUTT,
	{
		VI_PVI_FORMAT_HDA,
		1280, 0, 720, 0,
#if defined(USE_PVI_25FPS)
		VI_PVI_FREQUENCY_25HZ,		// changable 25/30/50/60
#else
		VI_PVI_FREQUENCY_30HZ,		// changable 25/30/50/60
#endif
	},
};

VI_DEV_ATTR_S DEV_ATTR_PVI_HDA_1080P =
{
	VI_INTF_PVI_HDA_1080P,
	VI_PATH_BYPASS,
	VI_DATASHIFT_BUTT,
	{
		VI_PVI_FORMAT_HDA,
		1920, 0, 1080, 0,
#if defined(USE_PVI_25FPS)
		VI_PVI_FREQUENCY_25HZ,		// changable 25/30/50/60
#else
		VI_PVI_FREQUENCY_30HZ,		// changable 25/30/50/60
#endif
	},
};

VI_DEV_ATTR_S DEV_ATTR_PVI_HDT_720P =
{
	VI_INTF_PVI_HDT_720P,
	VI_PATH_BYPASS,
	VI_DATASHIFT_BUTT,
	{
		VI_PVI_FORMAT_HDT,
		1280, 0, 720, 0,
#if defined(USE_PVI_25FPS)
		VI_PVI_FREQUENCY_25HZ,		// changable 25/30/50/60
#else
		VI_PVI_FREQUENCY_30HZ,		// changable 25/30/50/60
#endif
	},
};

VI_DEV_ATTR_S DEV_ATTR_PVI_HDT_1080P =
{
	VI_INTF_PVI_HDT_1080P,
	VI_PATH_BYPASS,
	VI_DATASHIFT_BUTT,
	{
		VI_PVI_FORMAT_HDT,
		1920, 0, 1080, 0,
#if defined(USE_PVI_25FPS)
		VI_PVI_FREQUENCY_25HZ,		// changable 25/30/50/60
#else
		VI_PVI_FREQUENCY_30HZ,		// changable 25/30/50/60
#endif
	},
};

#endif

VI_DEV_INPUT_E SAMPLE_COMM_VI_GetDevType(SAMPLE_VI_MODE_E enViMode)
{
	switch (enViMode) {
    	case LVDS_PS1210K_LVDS_1080P_30FPS:
		case LVDS_PS1210K_LVDS_720P_30FPS:
    	case LVDS_PS3210M_LVDS_1080P_30FPS:
		case LVDS_PS3210M_LVDS_720P_30FPS:
    	case LVDS_PS3210K_LVDS_1080P_30FPS:
		case LVDS_PS3210K_LVDS_720P_30FPS:
		case LVDS_PS4100K_LVDS_720P_30FPS:
		case LVDS_PS7100K_LVDS_720P_30FPS:
			return VI_DEV_INPUT_LVDS;
    	
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
			return VI_DEV_INPUT_BAYER;
		
		case CAM0_PO3100K_BT1120_1080P_30FPS:
		case CAM0_PO3100K_BT1120_720P_30FPS:
		case CAM0_PC7030K_BT656_480I_30FPS:
		case CAM0_PC7030K_BT656_576I_30FPS:
			return VI_DEV_INPUT_CAM0;
		
		case CAM1_PC7030K_BT656_480I_30FPS:
		case CAM1_PC7030K_BT656_576I_30FPS:
		case CAM1_PC3089N_BT601_480I_30FPS:
		case CAM1_PC3089N_BT601_480P_30FPS:
			return VI_DEV_INPUT_CAM1;
		
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
			return VI_DEV_INPUT_PVI;

		default:
			return VI_DEV_INPUT_BUTT;			
	}
	return VI_DEV_INPUT_BUTT;			
}

BD_S32 SAMPLE_COMM_VI_Init(SAMPLE_VI_MODE_E enViMode0, SAMPLE_VI_MODE_E enViMode1, SAMPLE_VI_MODE_E enViMode2)
{
	BD_S32 s32Ret;
	VI_DEV_INPUT_E InputType0 = VI_DEV_INPUT_BUTT, InputType1 = VI_DEV_INPUT_BUTT, InputType2 = VI_DEV_INPUT_BUTT;
	VI_DEV_INPUT_E VIMuxInputTypes[VIU_MAX_DEV_NUM] = {VI_DEV_INPUT_BUTT, VI_DEV_INPUT_BUTT};

	if (enViMode0 != VI_MODE_BUTT)
		InputType0 = SAMPLE_COMM_VI_GetDevType(enViMode0);
	if (enViMode1 != VI_MODE_BUTT)
		InputType1 = SAMPLE_COMM_VI_GetDevType(enViMode1);
	if (enViMode2 != VI_MODE_BUTT)
		InputType2 = SAMPLE_COMM_VI_GetDevType(enViMode2);


	/* mux 1 has 2 channel and share it's source */
	/* should check about different source */
	if (InputType0 != InputType1) {
		if (InputType0 != VI_DEV_INPUT_BUTT &&
			InputType1 != VI_DEV_INPUT_BUTT) {
			SAMPLE_PRT("invalid enViMode 1st param should be 2nd param except VI_MODE_BUTT\n");
			SAMPLE_PRT("mux : %d / %d / %d \n", InputType0, InputType1, InputType2);
			return -1;
		}
	}

	/* mux 1's channel 0 and 1 is same source or butt */
	if (InputType0 == VI_DEV_INPUT_BUTT) {
		VIMuxInputTypes[0] = InputType1;
	}
	else {
		VIMuxInputTypes[0] = InputType0;
	}
	VIMuxInputTypes[1] = InputType2;

	s32Ret = BD_MPI_VI_InitDev(VIMuxInputTypes[0], VIMuxInputTypes[1]);
	if (s32Ret != BD_SUCCESS)
	{
		SAMPLE_PRT("BD_MPI_VI_SetDevAllInit failed with %#x!\n",\
				s32Ret);
		return BD_FAILURE;
	}
	return BD_SUCCESS;
}

BD_VOID SAMPLE_COMM_VI_Exit(BD_VOID)
{
	SAMPLE_PRT("BD_MPI_VI_ExitDev() process..\n");
	BD_MPI_VI_ExitDev();
	SAMPLE_PRT("BD_MPI_VI_ExitDev() done\n");
}

/*****************************************************************************
* function : Get Vi Dev No. according to Vi_Chn No.
*****************************************************************************/
VI_DEV SAMPLE_COMM_VI_GetDev(VI_CHN ViChn)
{
	return (VI_DEV)VIU_CHN2DEV(ViChn);
}

/*****************************************************************************
* function : star vi dev (cfg vi_dev_attr; set_dev_cfg; enable dev)
*****************************************************************************/
BD_S32 SAMPLE_COMM_VI_StartDev(VI_DEV ViDev, SAMPLE_VI_MODE_E enViMode)
{
    BD_S32 s32Ret;

#if 0
    VI_DEV_ATTR_S    stViDevAttr;
   
    memset(&stViDevAttr,0,sizeof(stViDevAttr));

    switch (enViMode)
    {
		case LVDS_PS1210K_LVDS_1080P_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PS1210K_LVDS_1080P,sizeof(stViDevAttr));
            break;
		case LVDS_PS1210K_LVDS_720P_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PS1210K_LVDS_720P,sizeof(stViDevAttr));
            break;
		case LVDS_PS3210M_LVDS_1080P_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PS3210M_LVDS_1080P,sizeof(stViDevAttr));
            break;
		case LVDS_PS3210M_LVDS_720P_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PS3210M_LVDS_720P,sizeof(stViDevAttr));
            break;
		case LVDS_PS3210K_LVDS_1080P_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PS3210K_LVDS_1080P,sizeof(stViDevAttr));
            break;
		case LVDS_PS3210K_LVDS_720P_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PS3210K_LVDS_720P,sizeof(stViDevAttr));
            break;
		case LVDS_PS4100K_LVDS_720P_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PS4100K_LVDS_720P,sizeof(stViDevAttr));
            break;
		case LVDS_PS7100K_LVDS_720P_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PS7100K_LVDS_720P,sizeof(stViDevAttr));
            break;
		case CAM0_PS1210K_BAYER_1080P_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PS1210K_BAYER_1080P,sizeof(stViDevAttr));
            break;
		case CAM0_PS1210K_BAYER_720P_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PS1210K_BAYER_720P,sizeof(stViDevAttr));
            break;
		case CAM0_PS1210M_BAYER_1080P_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PS1210M_BAYER_1080P,sizeof(stViDevAttr));
            break;
		case CAM0_PS1210M_BAYER_720P_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PS1210M_BAYER_720P,sizeof(stViDevAttr));
            break;
		case CAM0_PS3210M_BAYER_1080P_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PS3210M_BAYER_1080P,sizeof(stViDevAttr));
            break;
		case CAM0_PS3210M_BAYER_720P_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PS3210M_BAYER_720P,sizeof(stViDevAttr));
            break;
		case CAM0_PS3210P_BAYER_1080P_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PS3210P_BAYER_1080P,sizeof(stViDevAttr));
            break;
		case CAM0_PS3210P_BAYER_720P_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PS3210P_BAYER_720P,sizeof(stViDevAttr));
            break;
		case CAM0_PS3210K_BAYER_1080P_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PS3210K_BAYER_1080P,sizeof(stViDevAttr));
            break;
		case CAM0_PS3210K_BAYER_720P_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PS3210K_BAYER_720P,sizeof(stViDevAttr));
            break;
		case CAM0_PS4100K_BAYER_720P_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PS4100K_BAYER_720P,sizeof(stViDevAttr));
            break;
		case CAM0_PS7100K_BAYER_720P_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PS7100K_BAYER_720P,sizeof(stViDevAttr));
            break;
		case CAM0_PO3100K_BT1120_1080P_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PO3100K_1080P,sizeof(stViDevAttr));
            break;
		case CAM0_PO3100K_BT1120_720P_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PO3100K_720P,sizeof(stViDevAttr));
            break;
		case CAM0_PC7030K_BT656_480I_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PC7030K_480I_0,sizeof(stViDevAttr));
            break;
		case CAM0_PC7030K_BT656_576I_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PC7030K_576I_0,sizeof(stViDevAttr));
            break;
		case CAM1_PC7030K_BT656_480I_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PC7030K_480I_1,sizeof(stViDevAttr));
            break;
		case CAM1_PC7030K_BT656_576I_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PC7030K_576I_1,sizeof(stViDevAttr));
            break;
		case CAM1_PC3089N_BT601_480I_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PC3089N_480I,sizeof(stViDevAttr));
            break;
		case CAM1_PC3089N_BT601_480P_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PC3089N_480P,sizeof(stViDevAttr));
            break;
		case PVI_ANALOG_SD_480I_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PVI_480I,sizeof(stViDevAttr));
			break;
		case PVI_ANALOG_SD_576I_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PVI_576I,sizeof(stViDevAttr));
			break;
		case PVI_ANALOG_PVI_720P_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PVI_PVI_720P,sizeof(stViDevAttr));
			break;
		case PVI_ANALOG_PVI_1080P_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PVI_PVI_1080P,sizeof(stViDevAttr));
			break;
		case PVI_ANALOG_CVI_720P_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PVI_CVI_720P,sizeof(stViDevAttr));
			break;
		case PVI_ANALOG_CVI_1080P_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PVI_CVI_1080P,sizeof(stViDevAttr));
			break;
		case PVI_ANALOG_HDA_720P_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PVI_HDA_720P,sizeof(stViDevAttr));
			break;
		case PVI_ANALOG_HDA_1080P_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PVI_HDA_1080P,sizeof(stViDevAttr));
			break;
		case PVI_ANALOG_HDT_720P_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PVI_HDT_720P,sizeof(stViDevAttr));
			break;
		case PVI_ANALOG_HDT_1080P_30FPS:
            memcpy(&stViDevAttr,&DEV_ATTR_PVI_HDT_1080P,sizeof(stViDevAttr));
			break;

        default:
	        SAMPLE_PRT("invalid dev attribute enViMode %d!\n", enViMode);
	        return BD_FAILURE;
    }
    
	s32Ret = BD_MPI_VI_SetDevAttr(ViDev, &stViDevAttr);
    if (s32Ret != BD_SUCCESS)
    {
        SAMPLE_PRT("BD_MPI_VI_SetDevAttr failed with %#x!\n", s32Ret);
        return BD_FAILURE;
    }
#else	
	if (enViMode >= VI_MODE_BUTT) {
		SAMPLE_PRT("invalid dev attribute enViMode %d!\n", enViMode);
		return BD_FAILURE;
	}

    s32Ret = BD_MPI_VI_SetDevAttr(ViDev, &DEV_ATTR_INTERFACE[enViMode]);
    if (s32Ret != BD_SUCCESS)
    {
        SAMPLE_PRT("BD_MPI_VI_SetDevAttr failed with %#x!\n", s32Ret);
        return BD_FAILURE;
    }
#endif	

    s32Ret = BD_MPI_VI_EnableDev(ViDev);
    if (s32Ret != BD_SUCCESS)
    {
        SAMPLE_PRT("BD_MPI_VI_EnableDev failed with %#x!\n", s32Ret);
        return BD_FAILURE;
    }

    return BD_SUCCESS;
}

/*****************************************************************************
* function : star vi chn
*****************************************************************************/
BD_S32 SAMPLE_COMM_VI_StartChn(VI_CHN ViChn, SIZE_S *pstTarSize, SAMPLE_VI_CONFIG_S* pstViConfig)
{
    BD_S32 s32Ret;
    VI_CHN_ATTR_S stChnAttr;

    /* step  5: config & start vicap dev */
    stChnAttr.stDestSize.u32Width = pstTarSize->u32Width;
    stChnAttr.stDestSize.u32Height = pstTarSize->u32Height;
#if defined(USE_SENSOR_25FPS)	
    stChnAttr.s32SrcFrameRate = 25;
#else	
    stChnAttr.s32SrcFrameRate = 30;
#endif
    stChnAttr.s32FrameRate = pstViConfig->s32FrameRate;

    s32Ret = BD_MPI_VI_SetChnAttr(ViChn, &stChnAttr);
    if (s32Ret != BD_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return BD_FAILURE;
    }

   	s32Ret = BD_MPI_VI_SetFrameDepth(ViChn, pstViConfig->u32FrameDepth);
    if (s32Ret != BD_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return BD_FAILURE;
    }
    
    s32Ret = BD_MPI_VI_EnableChn(ViChn);
    if (s32Ret != BD_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return BD_FAILURE;
    }

    return BD_SUCCESS;
}

/*****************************************************************************
* function : star vi according to product type
*****************************************************************************/
BD_S32 SAMPLE_COMM_VI_StartADC(VI_CHN ViChn, SAMPLE_VI_CONFIG_S* pstViConfig)
{
    BD_S32 s32Ret = BD_SUCCESS;
    VI_DEV ViDev;
    SAMPLE_VI_MODE_E enViMode;
	SIZE_S stTargetSize;

	if (ViChn >= VIU_MAX_CHN_NUM) {
        SAMPLE_PRT("%s: invalid chn %d (max %d)\n", __FUNCTION__, ViChn, VIU_MAX_CHN_NUM);
        return BD_FAILURE;
	}

    if (!pstViConfig)
    {
        SAMPLE_PRT("%s: null ptr\n", __FUNCTION__);
        return BD_FAILURE;
    }

    enViMode = pstViConfig->enViMode;
    
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(pstViConfig->enNorm, pstViConfig->enPicSize, &stTargetSize);
    if (BD_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("vi get size failed!\n");
        return BD_FAILURE;
    }

    /******************************************
     step 1: configure analog TV in or PVI (skip).
    ******************************************/
	
	/******************************************************
     step 2 : config & start vicap dev
    ******************************************************/
	ViDev = SAMPLE_COMM_VI_GetDev(ViChn);
	s32Ret = SAMPLE_COMM_VI_StartDev(ViDev, enViMode);
	if (BD_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("%s: start vi dev[%d] failed!\n", __FUNCTION__, ViDev);
		return BD_FAILURE;
	}

    /******************************************************
    * Step 4: config & start vicap chn (max 1) 
    ******************************************************/
	s32Ret = SAMPLE_COMM_VI_StartChn(ViChn, &stTargetSize, pstViConfig);
	if (BD_SUCCESS != s32Ret)
	{
		return BD_FAILURE;
	}
	    
    return s32Ret;
}

/*****************************************************************************
* function : stop vi accroding to product type
*****************************************************************************/
BD_S32 SAMPLE_COMM_VI_StopADC(VI_CHN ViChn, SAMPLE_VI_CONFIG_S* pstViConfig)
{
    VI_DEV ViDev;
    BD_S32 s32Ret;
	BD_U32 u32DisableTimeout = 5000;

	if (ViChn >= VIU_MAX_CHN_NUM) {
        SAMPLE_PRT("%s: invalid chn %d (max %d)\n", __FUNCTION__, ViChn, VIU_MAX_CHN_NUM);
        return BD_FAILURE;
	}

    if (!pstViConfig)
    {
        SAMPLE_PRT("%s: null ptr\n", __FUNCTION__);
        return BD_FAILURE;
    }

	SAMPLE_PRT("BD_MPI_VI_DisableChn VI_%d\n", ViChn);
	s32Ret = BD_MPI_VI_DisableChn(ViChn);
	if (BD_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("BD_MPI_VI_DisableChn failed with %#x\n",s32Ret);
		return BD_FAILURE;
	}

    /*** Stop VI Dev ***/
	ViDev = SAMPLE_COMM_VI_GetDev(ViChn);
	s32Ret = BD_MPI_VI_DisableDev(ViDev, u32DisableTimeout);
	if (BD_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("BD_MPI_VI_DisableDev failed with %#x\n", s32Ret);
		return BD_FAILURE;
	}

    return BD_SUCCESS;
}

BD_BOOL SAMPLE_COMM_VI_IsSensorInput(SAMPLE_VI_MODE_E enViMode)
{
    BD_BOOL bRet = BD_TRUE;

    switch (enViMode)
    {
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
		case VI_MODE_BUTT:
            bRet = BD_FALSE;
            break;
        default:
            break;
    }

    return bRet;    
}

BD_BOOL SAMPLE_COMM_VI_IsISPInput(SAMPLE_VI_MODE_E enViMode)
{
	BD_BOOL bRet = BD_FALSE;

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
			bRet = BD_TRUE;
			break;
		default:
			break;
	}

	return bRet;
}

BD_S32 SAMPLE_COMM_VI_BindVenc(SAMPLE_VI_MODE_E enViMode)
{
    BD_S32 j, s32Ret;
    VENC_GRP  VencGrp;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;
    SAMPLE_VI_PARAM_S stViParam;
    VI_CHN ViChn;

    VencGrp = 0;
    for (j=0; j<stViParam.s32ViChnCnt; j++)
    {
        ViChn = j * stViParam.s32ViChnInterval;
        
        stSrcChn.enModId = BD_ID_VIU;
        stSrcChn.s32DevId = 0;
        stSrcChn.s32ChnId = ViChn;
    
        stDestChn.enModId = BD_ID_GROUP;
        stDestChn.s32DevId = VencGrp;
        stDestChn.s32ChnId = 0;
    
        s32Ret = BD_MPI_SYS_Bind(&stSrcChn, &stDestChn);
        if (s32Ret != BD_SUCCESS)
        {
            SAMPLE_PRT("failed with %#x!\n", s32Ret);
            return BD_FAILURE;
        }
        
        VencGrp ++;
    }
    
    return BD_SUCCESS;
}

BD_S32 SAMPLE_COMM_VI_StartNoIsp(VI_CHN ViChn, SAMPLE_VI_CONFIG_S* pstViConfig)
{
    BD_S32 s32Ret = BD_SUCCESS;
    VI_DEV ViDev;
    SAMPLE_VI_MODE_E enViMode;
	VIDEO_NORM_E enNorm;
	SIZE_S stTargetSize;

	if (ViChn >= VIU_MAX_CHN_NUM) {
        SAMPLE_PRT("%s: invalid chn %d (max %d)\n", __FUNCTION__, ViChn, VIU_MAX_CHN_NUM);
        return BD_FAILURE;
	}

    if (!pstViConfig)
    {
        SAMPLE_PRT("%s: null ptr\n", __FUNCTION__);
        return BD_FAILURE;
    }

    enViMode = pstViConfig->enViMode;
	enNorm = pstViConfig->enNorm;
    
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(pstViConfig->enNorm, pstViConfig->enPicSize, &stTargetSize);
    if (BD_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("vi get size failed!\n");
        return BD_FAILURE;
    }

    /******************************************
     step 1: configure sensor.
    ******************************************/
    s32Ret = SAMPLE_COMM_ISP_SensorInit(enViMode, enNorm);
    if (BD_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("%s: Sensor init failed!\n", __FUNCTION__);
        return BD_FAILURE;
    }

    /******************************************************
     step 2 : config & start vicap dev
    ******************************************************/
	ViDev = SAMPLE_COMM_VI_GetDev(ViChn);
	s32Ret = SAMPLE_COMM_VI_StartDev(ViDev, enViMode);
	if (BD_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("%s: start vi dev[%d] failed!\n", __FUNCTION__, ViDev);
		return BD_FAILURE;
	}

    /******************************************************
    * Step 4: config & start vicap chn (max 1) 
    ******************************************************/
	s32Ret = SAMPLE_COMM_VI_StartChn(ViChn, &stTargetSize, pstViConfig);
	if (BD_SUCCESS != s32Ret)
	{
		return BD_FAILURE;
	}

    return s32Ret;
}


BD_S32 SAMPLE_COMM_VI_StartIsp(VI_CHN ViChn, SAMPLE_VI_CONFIG_S* pstViConfig, SAMPLE_ISPADDON_CONFIG_S* pstISPAddonConfig)
{
    BD_S32 s32Ret = BD_SUCCESS;
    VI_DEV ViDev;
    SAMPLE_VI_MODE_E enViMode;
	VIDEO_NORM_E enNorm;
	SIZE_S stTargetSize;

	if (ViChn >= VIU_MAX_CHN_NUM) {
        SAMPLE_PRT("%s: invalid chn %d (max %d)\n", __FUNCTION__, ViChn, VIU_MAX_CHN_NUM);
        return BD_FAILURE;
	}

    if (!pstViConfig)
    {
        SAMPLE_PRT("%s: null ptr\n", __FUNCTION__);
        return BD_FAILURE;
    }
    
	enViMode = pstViConfig->enViMode;
	enNorm = pstViConfig->enNorm;

    s32Ret = SAMPLE_COMM_SYS_GetPicSize(pstViConfig->enNorm, pstViConfig->enPicSize, &stTargetSize);
    if (BD_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("vi get size failed!\n");
        return BD_FAILURE;
    }

    /******************************************
     step 1: configure sensor.
    ******************************************/
	s32Ret = SAMPLE_COMM_ISP_SensorInit(enViMode, enNorm);
	if (BD_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("%s: Sensor init failed!\n", __FUNCTION__);
		return BD_FAILURE;
	}

    /******************************************************
     step 2 : config & start vicap dev
    ******************************************************/
	ViDev = SAMPLE_COMM_VI_GetDev(ViChn);
	s32Ret = SAMPLE_COMM_VI_StartDev(ViDev, enViMode);
	if (BD_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("%s: start vi dev[%d] failed!\n", __FUNCTION__, ViDev);
		return BD_FAILURE;
	}
	
	/******************************************
     step 3: configure & run isp thread 
     note: you can jump over this step, if you do not use Hi3518 interal isp. 
     note: isp run must at this step -- after vi dev enable, before vi chn enable 
    ******************************************/
    s32Ret = SAMPLE_COMM_ISP_Run(enViMode, pstISPAddonConfig);
    if (BD_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("%s: ISP init failed!\n", __FUNCTION__);
	/* disable videv */
        return BD_FAILURE;
    }
	
	/******************************************************
    * Step 4: config & start vicap chn (max 1) 
    ******************************************************/
	s32Ret = SAMPLE_COMM_VI_StartChn(ViChn, &stTargetSize, pstViConfig);
	if (BD_SUCCESS != s32Ret)
	{
		return BD_FAILURE;
	}

    return s32Ret;
}

BD_S32 SAMPLE_COMM_VI_StopNoIsp(VI_CHN ViChn, SAMPLE_VI_CONFIG_S* pstViConfig)
{
    VI_DEV ViDev;
    BD_S32 s32Ret;
    SAMPLE_VI_MODE_E enViMode;
	BD_U32 u32DisableTimeout = 5000;

	if (ViChn >= VIU_MAX_CHN_NUM) {
        SAMPLE_PRT("%s: invalid chn %d (max %d)\n", __FUNCTION__, ViChn, VIU_MAX_CHN_NUM);
        return BD_FAILURE;
	}

    if (!pstViConfig)
    {
        SAMPLE_PRT("%s: null ptr\n", __FUNCTION__);
        return BD_FAILURE;
    }
    
	enViMode = pstViConfig->enViMode;

	s32Ret = BD_MPI_VI_DisableChn(ViChn);
	if (BD_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("BD_MPI_VI_DisableChn failed with %#x\n",s32Ret);
		return BD_FAILURE;
	}

    /*** Stop VI Dev ***/
	ViDev = SAMPLE_COMM_VI_GetDev(ViChn);
	s32Ret = BD_MPI_VI_DisableDev(ViDev, u32DisableTimeout);
	if (BD_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("BD_MPI_VI_DisableDev failed with %#x\n", s32Ret);
		return BD_FAILURE;
	}

 	SAMPLE_COMM_ISP_SensorExit(enViMode);
    return BD_SUCCESS;
}

BD_S32 SAMPLE_COMM_VI_StopIsp(VI_CHN ViChn, SAMPLE_VI_CONFIG_S* pstViConfig)
{
    VI_DEV ViDev;
    BD_S32 s32Ret;
    SAMPLE_VI_MODE_E enViMode;
	BD_U32 u32DisableTimeout = 5000;

	if (ViChn >= VIU_MAX_CHN_NUM) {
        SAMPLE_PRT("%s: invalid chn %d (max %d)\n", __FUNCTION__, ViChn, VIU_MAX_CHN_NUM);
        return BD_FAILURE;
	}

    if (!pstViConfig)
    {
        SAMPLE_PRT("%s: null ptr\n", __FUNCTION__);
        return BD_FAILURE;
    }
    
	enViMode = pstViConfig->enViMode;
    
	SAMPLE_PRT("BD_MPI_VI_DisableChn VI_%d\n", ViChn);
	s32Ret = BD_MPI_VI_DisableChn(ViChn);
	if (BD_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("BD_MPI_VI_DisableChn failed with %#x\n",s32Ret);
		return BD_FAILURE;
	}

    /*** Stop VI Dev ***/
	ViDev = SAMPLE_COMM_VI_GetDev(ViChn);
	s32Ret = BD_MPI_VI_DisableDev(ViDev, u32DisableTimeout);
	if (BD_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("BD_MPI_VI_DisableDev failed with %#x\n", s32Ret);
		return BD_FAILURE;
	}

    SAMPLE_COMM_ISP_Stop();
 	SAMPLE_COMM_ISP_SensorExit(enViMode);
    return BD_SUCCESS;
}

BD_S32 SAMPLE_COMM_VI_StartVi(VI_CHN ViChn, SAMPLE_VI_CONFIG_S* pstViConfig, SAMPLE_ISPADDON_CONFIG_S* pstISPAddonConfig)
{
    BD_S32 s32Ret = BD_SUCCESS;
    SAMPLE_VI_MODE_E enViMode;

	if (ViChn >= VIU_MAX_CHN_NUM) {
        SAMPLE_PRT("%s: ViChn (%d) is invalid (max %d)\n", __FUNCTION__, ViChn, VIU_MAX_CHN_NUM);
        return BD_FAILURE;
	}

    if (!pstViConfig) {
        SAMPLE_PRT("%s: null ptr\n", __FUNCTION__);
        return BD_FAILURE;
    }

    enViMode = pstViConfig->enViMode;

    if (!SAMPLE_COMM_VI_IsSensorInput(enViMode)) {
        s32Ret = SAMPLE_COMM_VI_StartADC(ViChn, pstViConfig);	// HDTN
    }
    else {
		if (!SAMPLE_COMM_VI_IsISPInput(enViMode)) {
			s32Ret = SAMPLE_COMM_VI_StartNoIsp(ViChn, pstViConfig);	// BT.1120, BT.656 or BT.601
		}
		else {
	        s32Ret = SAMPLE_COMM_VI_StartIsp(ViChn, pstViConfig, pstISPAddonConfig);	// LVDS or Bayer
		}
    }


    return s32Ret; 
}

BD_S32 SAMPLE_COMM_VI_StopVi(VI_CHN ViChn, SAMPLE_VI_CONFIG_S* pstViConfig)
{
    BD_S32 s32Ret = BD_SUCCESS;
    SAMPLE_VI_MODE_E enViMode;

    if(!pstViConfig)
    {
        SAMPLE_PRT("%s: null ptr\n", __FUNCTION__);
        return BD_FAILURE;
    }
    enViMode = pstViConfig->enViMode;
    
    if (!SAMPLE_COMM_VI_IsSensorInput(enViMode))
    {
        s32Ret = SAMPLE_COMM_VI_StopADC(ViChn, pstViConfig);        
    }
    else
    {
		if (!SAMPLE_COMM_VI_IsISPInput(enViMode)) {
	        s32Ret = SAMPLE_COMM_VI_StopNoIsp(ViChn, pstViConfig);        
		}
		else {
	        s32Ret = SAMPLE_COMM_VI_StopIsp(ViChn, pstViConfig);        
		}
    }
    
    return s32Ret;
}

BD_S32 SAMPLE_COMM_VI_SetFrameRate(VI_CHN ViChn, BD_S32 s32SrcFrameRate, BD_S32 s32FrameRate)
{
    BD_S32 s32Ret = BD_SUCCESS;

	if (ViChn >= VIU_MAX_CHN_NUM) {
        SAMPLE_PRT("%s: ViChn (%d) is invalid (max %d)\n", __FUNCTION__, ViChn, VIU_MAX_CHN_NUM);
        return BD_FAILURE;
	}

	if (s32SrcFrameRate != 30) {
        SAMPLE_PRT("%s: ViChn (%d) src frame rate should be 30 (curr %d)\n", __FUNCTION__, ViChn, s32SrcFrameRate);
        return BD_FAILURE;
	}
	
	if (s32FrameRate > s32SrcFrameRate) {
        SAMPLE_PRT("%s: ViChn (%d) frame rate (%d) should be under or same src-frame rate (curr %d)\n", 
			__FUNCTION__, ViChn, s32FrameRate, s32SrcFrameRate);
        return BD_FAILURE;
	}

	s32Ret = BD_MPI_VI_SetFrameRate(ViChn, s32SrcFrameRate, s32FrameRate);	// HDTN
	if (s32Ret != BD_SUCCESS) {
		SAMPLE_PRT("failed with %#x!\n", s32Ret);
		return BD_FAILURE;
	}
                
    return BD_SUCCESS;
}


BD_S32 SAMPLE_COMM_VI_UnBindVenc(VI_CHN ViChn, VENC_GRP VencGrp)
{
    BD_S32 s32Ret;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

	stSrcChn.enModId = BD_ID_VIU;
	stSrcChn.s32DevId = ((ViChn == 2)?VIU_SECONDARY_CHN_DEV:VIU_PRIMARY_CHN_DEV);
	stSrcChn.s32ChnId = ViChn;

	stDestChn.enModId = BD_ID_GROUP;
	stDestChn.s32DevId = VencGrp;
	stDestChn.s32ChnId = 0;

	s32Ret = BD_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
	if (s32Ret != BD_SUCCESS)
	{
		SAMPLE_PRT("failed with %#x!\n", s32Ret);
		return BD_FAILURE;
	}
                
    return BD_SUCCESS;
}

/******************************************************************************
* function : Get from YUV
******************************************************************************/

BD_S32 SAMPLE_COMM_VI_InitDump(const BD_CHAR* FuncName, VI_CHN ViChn, SAMPLE_VI_CONFIG_S* pstViConfig, BD_U32 args)
{
	BD_S32 s32Ret = BD_SUCCESS;
	BD_CHAR name[256];
	SIZE_S stTargetSize;

    s32Ret = SAMPLE_COMM_SYS_GetPicSize(pstViConfig->enNorm, pstViConfig->enPicSize, &stTargetSize);
    if (BD_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("vi get size failed!\n");
        return BD_FAILURE;
    }

	sprintf(name, "./%s_ch%d_%d_%dx%d.yuv", FuncName, ViChn, args, 
		stTargetSize.u32Width, stTargetSize.u32Height);

	pstViConfig->dumpfp = fopen(name, "w+");
	if (pstViConfig->dumpfp) {
		return BD_SUCCESS;
	}

	SAMPLE_PRT("failed to open dump file\n");
	return BD_FAILURE;
}

BD_S32 SAMPLE_COMM_VI_ExitDump(SAMPLE_VI_CONFIG_S* pstViConfig)
{
	if (pstViConfig->dumpfp) {
		//fflush(pstViConfig->dumpfp);
		fclose(pstViConfig->dumpfp);
		pstViConfig->dumpfp = NULL;
	}
	return BD_SUCCESS;
}

BD_S32 SAMPLE_COMM_VI_WriteDump(FILE* fp, const BD_VOID* ptr, BD_U32 u32Size)
{
	if (!fp)
		return 0;
	return fwrite(ptr, u32Size, 1, fp);
}

BD_S32 SAMPLE_COMM_VI_GetVFrameFromYUV(VI_CHN ViChn, SAMPLE_VI_CONFIG_S* pstViConfig, VIDEO_FRAME_INFO_S *pstVFrameInfo)
{
	BD_S32 s32Ret;

//	srandom(clock());

	if (ViChn >= VIU_MAX_CHN_NUM) {
        SAMPLE_PRT("%s: ViChn (%d) is invalid (max %d)\n", __FUNCTION__, ViChn, VIU_MAX_CHN_NUM);
        return BD_FAILURE;
	}

//	s32Ret = BD_MPI_VI_GetFrameTimeOut(ViChn, pstVFrameInfo, 200);
	s32Ret = BD_MPI_VI_GetFrame(ViChn, pstVFrameInfo);
	if (s32Ret == BD_SUCCESS) {
		SAMPLE_PRT("VIChn (%d) get frame\n", ViChn);
		if (pstViConfig->dumpfp) {
//			SAMPLE_PRT("VIChn (%d) dump fp valid\n", ViChn);
			s32Ret = SAMPLE_COMM_VI_WriteDump(pstViConfig->dumpfp, pstVFrameInfo->pVirAddr[0], 
					(((pstVFrameInfo->u32Width * pstVFrameInfo->u32Height)/2)*3));
			if (s32Ret) {
				SAMPLE_PRT("VIChn (%d) dump done\n", ViChn);
			}
		}

//		usleep(random() & 0xfffff);

//		SAMPLE_PRT("call BD_MPI_VI_ReleaseFrame\n");
		s32Ret = BD_MPI_VI_ReleaseFrame(ViChn, pstVFrameInfo);
		if (s32Ret != BD_SUCCESS) {
			SAMPLE_PRT("failed to release frame\n");
			return BD_FAILURE;
		}
	}
	else {
		SAMPLE_PRT("failed to get frame\n");
	}
	return BD_SUCCESS;
}

BD_CHAR* SAMPLE_COMM_VI_GetModeString(SAMPLE_VI_MODE_E enViMode)
{
    switch (enViMode)
    {
		case LVDS_PS1210K_LVDS_1080P_30FPS:
			return "LVDS_PS1210K_LVDS_1080P";
		case LVDS_PS1210K_LVDS_720P_30FPS:
			return "LVDS_PS1210K_LVDS_720P";
		case LVDS_PS3210M_LVDS_1080P_30FPS:
			return "LVDS_PS3210M_LVDS_1080P";
		case LVDS_PS3210M_LVDS_720P_30FPS:
			return "LVDS_PS3210M_LVDS_720P";
		case LVDS_PS3210K_LVDS_1080P_30FPS:
			return "LVDS_PS3210K_LVDS_1080P";
		case LVDS_PS3210K_LVDS_720P_30FPS:
			return "LVDS_PS3210K_LVDS_720P";
		case LVDS_PS4100K_LVDS_720P_30FPS:
			return "LVDS_PS4100K_LVDS_720P";
		case LVDS_PS7100K_LVDS_720P_30FPS:
			return "LVDS_PS7100K_LVDS_720P";
		case CAM0_PS1210K_BAYER_1080P_30FPS:
			return "CAM0_PS1210K_BAYER_1080P";
		case CAM0_PS1210K_BAYER_720P_30FPS:
			return "CAM0_PS1210K_BAYER_720P";
		case CAM0_PS1210M_BAYER_1080P_30FPS:
			return "CAM0_PS1210M_BAYER_1080P";
		case CAM0_PS1210M_BAYER_720P_30FPS:
			return "CAM0_PS1210M_BAYER_720P";
		case CAM0_PS3210M_BAYER_1080P_30FPS:
			return "CAM0_PS3210M_BAYER_1080P";
		case CAM0_PS3210M_BAYER_720P_30FPS:
			return "CAM0_PS3210M_BAYER_720P";
		case CAM0_PS3210P_BAYER_1080P_30FPS:
			return "CAM0_PS3210P_BAYER_1080P";
		case CAM0_PS3210P_BAYER_720P_30FPS:
			return "CAM0_PS3210P_BAYER_720P";
		case CAM0_PS3210K_BAYER_1080P_30FPS:
			return "CAM0_PS3210K_BAYER_1080P";
		case CAM0_PS3210K_BAYER_720P_30FPS:
			return "CAM0_PS3210K_BAYER_720P";
		case CAM0_PS4100K_BAYER_720P_30FPS:
			return "CAM0_PS4100K_BAYER_720P";
		case CAM0_PS7100K_BAYER_720P_30FPS:
			return "CAM0_PS7100K_BAYER_720P";
		case CAM0_PO3100K_BT1120_1080P_30FPS:
			return "CAM0_PO3100K_BT1120_1080P";
		case CAM0_PO3100K_BT1120_720P_30FPS:
			return "CAM0_PO3100K_BT1120_720P";
		case CAM0_PC7030K_BT656_480I_30FPS:
			return "CAM0_PC7030K_BT656_480I";
		case CAM0_PC7030K_BT656_576I_30FPS:
			return "CAM0_PC7030K_BT656_576I";
		case CAM1_PC7030K_BT656_480I_30FPS:
			return "CAM1_PC7030K_BT656_480I";
		case CAM1_PC7030K_BT656_576I_30FPS:
			return "CAM1_PC7030K_BT656_576I";
		case CAM1_PC3089N_BT601_480I_30FPS:
			return "CAM1_PC3089N_BT601_480I";
		case CAM1_PC3089N_BT601_480P_30FPS:
			return "CAM1_PC3089N_BT601_480P";
		case PVI_ANALOG_SD_480I_30FPS:
			return "PVI_ANALOG_SD_480I";
		case PVI_ANALOG_SD_576I_30FPS:
			return "PVI_ANALOG_SD_576I";
		case PVI_ANALOG_PVI_720P_30FPS:
			return "PVI_ANALOG_PVI_720P";
		case PVI_ANALOG_PVI_1080P_30FPS:
			return "PVI_ANALOG_PVI_1080P";
		case PVI_ANALOG_CVI_720P_30FPS:
			return "PVI_ANALOG_CVI_720P";
		case PVI_ANALOG_CVI_1080P_30FPS:
			return "PVI_ANALOG_CVI_1080P";
		case PVI_ANALOG_HDA_720P_30FPS:
			return "PVI_ANALOG_HDA_720P";
		case PVI_ANALOG_HDA_1080P_30FPS:
			return "PVI_ANALOG_HDA_1080P";
		case PVI_ANALOG_HDT_720P_30FPS:
			return "PVI_ANALOG_HDT_720P";
		case PVI_ANALOG_HDT_1080P_30FPS:
			return "PVI_ANALOG_HDT_1080P";

        default:
	        return NULL;
    }
	return NULL;
}	


BD_S32 SAMPLE_COMM_VI_FillConfig(SAMPLE_VI_CONFIG_S *pstViConfig, SAMPLE_VI_MODE_E enViMode, PIC_SIZE_E enPicSize, VIDEO_NORM_E enNorm, BD_S32 s32FrameRate, BD_U32 u32FrameDepth)
{
	if (!pstViConfig) {
		SAMPLE_PRT("invalid VIConfig\n");
		return BD_FAILURE;
	}
   
   	memset(pstViConfig, 0, sizeof(SAMPLE_VI_CONFIG_S));
	pstViConfig->enPicSize = enPicSize;
	pstViConfig->enNorm = enNorm;	
	pstViConfig->enRotate = ROTATE_NONE;
	pstViConfig->enViChnSet = VI_CHN_SET_NORMAL;	
   	pstViConfig->enViMode = enViMode;
	pstViConfig->s32FrameRate = s32FrameRate;
	pstViConfig->u32FrameDepth = u32FrameDepth;

	return BD_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
