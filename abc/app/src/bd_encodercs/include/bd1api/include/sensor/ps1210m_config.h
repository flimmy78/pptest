#ifndef SENSOR_PS1210M_H_
#define SENSOR_PS1210M_H_ 1

#include <bd_sns_ctrl.h>

#if defined(SENSOR_ESTYPE)

#define USE_27MHZ_BYPASS	1
//#define USE_HALF_CLK		1

static const unsigned short init_ntsc_bayer_config[] = {
	0x0300 , 
	0x2998 , 
	0x0C00 , 
	0x0D01 , 
	0x0E00 , 
	0x0F02 , 
	0x1007 , 
	0x1190 , 
	0x1204 , 
	0x1347 , 
#ifdef USE_HALF_CLK
	0x2502 , 	// orig
#else
	0x2500 , 	// orig
#endif	
	0x26C3 ,
#ifdef USE_27MHZ_BYPASS
	0x4010 ,	// pll bypass mode
#else	
	/* PLL setting 27Mhz --> 74.25Mhz */
	0x4104 ,
	0x420b ,
	0x403C ,
	0xFFFE ,
	0x4028 ,
#endif
	0x0503 , 
	0x0302 , 
	0x9B20 ,	// # sync_control_1[5] : hsyncAllLines       
	0x0301 , 
	0x19C3 , 
	0x0300 , 
	0x3301 , 
	0x3402 , 
	0x0301 , 
	0x2603 , 
	0x0301 , 
	0xC004 , 
	0xC14F , 
	0xC200 , 
	0xC300 , 
	0xC440 , 
	0xFFFF , 
};

#else

static const unsigned short init_ntsc_bayer_config[] = {
	0xFFFF , 
};

#endif


/* 1920x1080 */
static struct sensor_ctrl_info init_sensor_values = {
	.chip_id = 0x1210,
	.name = "ps1210m",
	.chip_addr = 0x77,
	.use_16bit_addr = 0,
	.use_16bit_data = 0,
	.scan_supported = (SNS_SUPPORT_NTSC),
	.scan_mode = {
		[SNS_SCAN_NTSC] = {
			.intf_supported = (SNS_SUPPORT_BAYER),
			.intf_mode = 
			{
				init_ntsc_bayer_config, 0, 0, 0, 0,
			},
		},
		[SNS_SCAN_PAL] = {
			.intf_supported = 0,
			.intf_mode = {
				0, 0, 0, 0, 0,
			},
		},
	},
};

#endif // SENSOR_PS1210M_H_
