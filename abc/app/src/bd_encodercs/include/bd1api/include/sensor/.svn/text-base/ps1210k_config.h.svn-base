#ifndef SENSOR_PS1210K_H_
#define SENSOR_PS1210K_H_ 1

#include <bd_sns_ctrl.h>

#if defined(SENSOR_ESTYPE)
static const unsigned short init_ntsc_bayer_config[] = {
	0x0300, 
	0x2998, 
	0x0300, 
	0x0503,
#if 1	
	0x0608, 
	0x0797, 
#else
	0x060A, 
	0x074F, 
#endif
	0x0804, 
	0x0964, 
	0x0a04, 
	0x0b64, 
	0x0c00, 
	0x0d01, 
	0x0e00, 
	0x0f05, 
	0x1007, 
	0x118C, 
	0x1204, 
	0x134C, 
	0x1400, 
	0x150A, 
	0x1604, 
	0x1751, 
	0x2510, 
	0x3301, 
	0x3402, 
	0x36C8, 
	0x3848, 
	0x3A22, 
	0x4121, 
	0x4204, 
	0x4010, 
	0xFF32, 
	0x4000, 
	0x0301, 
	0x2603, 
	0x0301, 
	0xC004, 
	0xC15F, 
	0xC200, 
	0xC300, 
	0xC440, 
	0x0301, 
	0x19C3, 
	0x0302, 
	0x9B20, 
	0xFFFF, 
};

static const unsigned short init_ntsc_lvds_config[] = {
	//////////////////////// BANK A
	0x0300,
	0x0503, // mirror/flip

	0x0608, // framewidth_h
	0x0797, // framewidth_l

	0x2509, // clkdiv1
	0x268B, // clkdiv2

	0x3301, // pixelbias
	0x3402, // compbias

	0x3688, // TX_Bias; DCDC 4.96 V, LDO 4.37 V => DCDC 4.48 V, LDO 4.37 V ( C8h => 88h )
	0x3848, // black_bias,range_sel 0.4 V
	0x3A22, // main regulator output

	0x4121, // pll_m_cnt (21)
	0x4204, // pll_r_cnt (04)

	0x4010, // pll_control   
	0xFF0A, // delay 10ms
	0x4000, // pll_control on

	//////////////////////// BANK B
	0x0301,
	0x1644, // bayer_control_01 (frmvar_en,led_dsel)
	0x19C3, // bayer_control_04
	0x1E0E, // bayer_control_09 (bsmode)
	0x0301,
	0x2603, // blacksun_th_h
	0x0302,
	0x9B20,	// hsyncAllLines
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// LVDS control
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////// BANK D
	0x0303,
	0x0504,	// lvds_control_1
	0x0604,	// lvds_control_2
	0x0700,	// lvds_control_3
	0x0800,	// lvds_control_4
	0x1303,  // lvds_bias_0 lvds_bgr 33
	0x0303,
	0x0402, // LVDS off, power down
	0xFF0A, // delay 10ms
	0x0442, // LVDS on
	0xFFFF,
};

#else

static const unsigned short init_ntsc_bayer_config[] = {
	0x0300, 
#if 0	
	0x28f0,
	0x29F8, 
#else
	0x2842,
	0x2998, 
#endif
	0x0300, 
	0x0503, 
	0x0608,
	0x0797,
	0x0804,
	0x0964,
	0x0a04, 
	0x0b64, 
	0x0c00,	// # windowx1_h   , default(00), pi2008(00)
	0x0d01,	// # windowx1_l   , default(05)  pi2008(01)
	0x0e00,	// # windowy1_h   , default(00)  pi2008(00)
	0x0f05,	// # windowy1_l   , default(05)  pi2008(01)
	0x1007,	// # windowx2_h   , default(07)  pi2008(07)
	0x1190,	// # windowx2_l   , default(84)  pi2008(80)
	0x1204,	// # windowy2_h   , default(04)  pi2008(04)
	0x134C,	// # windowy2_l   , default(3C)  pi2008(38)
	0x1400,	// # vsyncstartrow_f0_h  , default(00)  
	0x150A,	// # vsyncstartrow_f0_l  , default(1a)   , pi2008(0b)
	0x1604,	// # vsyncstoprow_f0_h   , default(04)   
	0x1751,	// # vsyncstoprow_f0_l   , default(51)   , pi2008(43)
	0x2510, 
	0x3301, 
	0x3402, 
	0x36C8, 
	0x3848, 
	0x3A22, 
	0x4121, 
	0x4204, 
	0x4010, 
	0xFF32, 
	0x4000, 
	0x0301, 
	0x2603, 
	0x0301, 
	0xC004, 
	0xC15F, 
	0xC200, 
	0xC300, 
	0xC440, 
	0x0301, 
	0x19C3, 
	0x0302, 
	0x9B20, 
	0xFFFF, 
};

static const unsigned short init_ntsc_lvds_config[] = {
	0x0300,
	0x0503,	// # mirror/flip
	0x0608,
	0x0797,
	0x0804,
	0x0964,
	0x0a04, 
	0x0b64, 
	0x0c00,	// # windowx1_h   , default(00), pi2008(00)
	0x0d01,	// # windowx1_l   , default(05)  pi2008(01)
	0x0e00,	// # windowy1_h   , default(00)  pi2008(00)
	0x0f05,	// # windowy1_l   , default(05)  pi2008(01)
	0x1007,	// # windowx2_h   , default(07)  pi2008(07)
	0x1190,	// # windowx2_l   , default(84)  pi2008(80)
	0x1204,	// # windowy2_h   , default(04)  pi2008(04)
	0x134C,	// # windowy2_l   , default(3C)  pi2008(38)
	0x1400,	// # vsyncstartrow_f0_h  , default(00)  
	0x150A,	// # vsyncstartrow_f0_l  , default(1a)   , pi2008(0b)
	0x1604,	// # vsyncstoprow_f0_h   , default(04)   
	0x1751,	// # vsyncstoprow_f0_l   , default(51)   , pi2008(43)
	0x2509,	// # clkdiv1
	0x268B,	// # clkdiv2
	0x3301,	// # pixelbias
	0x3402,	// # compbias
	0x36C8,	// # TX_Bias; DCDC 4.96 V, LDO 4.37 V
	0x3848,	// # black_bias,range_sel 0.4 V
	0x3A22,	// # main regulator output
	0x4010,	// #pll_control			// bypass mode????
	0x0301,
	0x19C3,	// # bayer_control_04
	0xC004, 	// # inttime_h  
	0xC14F,	// # inttime_m  
	0xC200,	// # inttime_l  
	0xC300,	// # globalgain 
	0xC440,	// # digitalgain
	0x0301,	
	0x2603,	// # blacksun_th_h
	0x0302,
	0x9B20,	// sync control
	0x0303,
	0x0504,	//	# lvds_control_1
	0x0604,	//	# lvds_control_2
	0x0700,	//	# lvds_control_3
	0x0800,	//	# lvds_control_4
	0x1303,	// # lvds_bias_0 lvds_bgr 33
	0x0300,
	0x4000, // #pll_control
	0x0303,
	0x0402,	// # LVDS off, power down
	0xFF0F,
	0x0442,	// # LVDS on
	0xFFFF,
};

#endif

/* 1920x1080 */
static struct sensor_ctrl_info init_sensor_values = {
	.chip_id = 0x1210,
	.name = "ps1210k",
	.chip_addr = 0x77,
	.use_16bit_addr = 0,
	.use_16bit_data = 0,
	.scan_supported = (SNS_SUPPORT_NTSC),
	.scan_mode = {
		[SNS_SCAN_NTSC] = {
#if defined(SENSOR_ESTYPE)
			.intf_supported = (SNS_SUPPORT_BAYER),
#else
			.intf_supported = (SNS_SUPPORT_BAYER | SNS_SUPPORT_LVDS),
#endif
			.intf_mode = 
			{
				init_ntsc_bayer_config, init_ntsc_lvds_config, 0, 0, 0,
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


#endif // SENSOR_PS1210K_H_
