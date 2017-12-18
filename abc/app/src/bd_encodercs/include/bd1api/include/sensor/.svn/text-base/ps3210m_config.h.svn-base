#ifndef SENSOR_PS3210M_H_
#define SENSOR_PS3210M_H_ 1

#include <bd_sns_ctrl.h>

#if defined(SENSOR_ESTYPE)
static const unsigned short init_ntsc_bayer_config[] = {
	/* 20151222 */
	0x0300,
	0x2998,	// output Hi-z release
	0x0300,
	0x0503,	// mirror/flip
	0x0608,	// framewidth_h        (08)
	0x0797,	// framewidth_l        (97)
	0x0C00,	// windowx1_h					(00)
	0x0D0D,	// windowx1_l					(01)
	0x0E00,	// windowy1_h					(00)
	0x0F0E,	// windowy1_l					(02)
	0x1007,	// windowx2_h					(07)
	0x118C,	// windowx2_l					(8C)
	0x1204,	// windowy2_h					(04)
	0x1345,	// windowy2_l					(45)
	0x1400,	// vsyncstartrow_f0_h	(00)
	0x1519,	// vsyncstartrow_f0_l	(0D)
	0x1604,	// vsyncstoprow_f0_h		(04)
	0x1753,	// vsyncstoprow_f0_l		(53)
	0x2521, // clkdiv1 (21)
	0x4121, // pll_m_cnt (21)
	0x4208, // pll_r_cnt (04)
	0x4010, // pll_control
	0xFF10,
	0x4000, // pll_control on
	0x0301,
	0xC004,
	0xC15F,
	0xC200,
	0xC300,
	0x0301,	//////////////////////// BANK B
	0x1644, // bayer_control_01 (frmvar_en,led_dsel)
	0x1E0E, // bayer_control_09 (bsmode)
	0x2604, // blacksunth_h
	0xB740, // adcoffset
	0x0301,	// B bank									//////////////////////////////////////////////////////// CDS
	0x5708,	// lsenb start H									
	0x587F,	// lsenb start L									
	0x5908,	// lsenb stop H									
	0x5A6F,	// lsenb stop L									
	0x5300,	// ls rst start H									
	0x5402,	// ls rst start L									
	0x5508,	// ls rst stop H									
	0x567F,	// ls rst stop L									
	0x6700,	// ls txb start H									
	0x68DC,	// ls txb start L									
	0x6900,	// ls txb stop H									
	0x6AF0,	// ls txb stop L									
	0x5B00,	// sample start H									
	0x5C00,	// sample start L									
	0x5D08,	// sample stop H									
	0x5E7F,	// sample stop L									
	0x5F00,	// store start H									
	0x6000,	// store start L									
	0x6100,	// store stop H									
	0x62C8,	// store stop L									
	0x9900,	// tx illum start H									
	0x9ADC,	// tx illum start L									
	0x9B08,	// tx illum stop H									
	0x9C7F,	// tx illum stop L
	0x7B02,	// inven start H									
	0x7CA8,	// inven start L									
	0x7D07,	// inven stop H									
	0x7EDA,	// inven stop L									
	0x6F02,	// load start H									
	0x70A8,	// load start L									
	0x7107,	// load stop H									
	0x72DA,	// load stop L									
	0x7302,	// latch en start H									
	0x74B2,	// latch en start L									
	0x7507,	// latch en stop H									
	0x76D0,	// latch en stop L									
	0x7708,	// latch en start2 H									
	0x7895,	// latch en start2 L									
	0x7908,	// latch en stop2 H									
	0x7A96,	// latch en stop2 L									
	//0xB740	// adc offset									
	0x8F00,	// adc offset stop H									
	0x90D2,	// adc offset stop L									
	0x8B02,	// ramp start H									
	0x8CBC,	// ramp start L									
	0x8D08,	// ramp stop H									
	0x8E48,	// ramp stop L									
	0x8708,	// transfer start H									
	0x8834,	// transfer start L									
	0x8908,	// transfer stop H									
	0x8A3E,	// transfer stop L									
	0x9508,	// refhold start H									
	0x965C,	// refhold start L									
	0x9708,	// refhold stop H									
	0x988F,	// refhold stop L									
	0x9108,	// atten rst start H									
	0x925C,	// atten rst start L									
	0x9308,	// atten rst stop H									
	0x9497,	// atten rst stop L									
	0x7F08,	// pbhold start H									
	0x805C,	// pbhold start L									
	0x8108,	// pbhold stop H									
	0x828F,	// pbhold stop L									
	0x8308,	// col pbhold start H									
	0x845C,	// col pbhold start L									
	0x8508,	// col pbhold stop H									
	0x868F,	// col pbhold stop L									
	0xB908,	// col pbhold2 start H									
	0xBA5C,	// col pbhold2 start L									
	0xBB08,	// col pbhold2 stop H									
	0xBC8F,	// col pbhold2 stop L									
	0xA10B,	// COFFSET H									
	0xA2A0,	// COFFSET L									
	0x3600,	// TG hsync start H									
	0x37A2,	// TG hsync start L									
	0x3808,	// TG hsync stop H									
	0x3932,	// TG hsync stop L
	0x3E00,	// blhstart_h
	0x3FA2,	// blhstart_l
	0x4008,	// blhstop_h
	0x4132,	// blhstop_l									
	0x0300,	////////////////////////////////////////////////////////////////////////////////////////////// ablc
	0x3695, // ablc_pd [1], ablc_res_con [0] '0'
	0x3B27, // ablc_step_con [5:3] '011'	 // 'x1 x2 x3' (5, 2.5, 1.25), calc : 1.25 + 5*x1 + 2.5*x2 + 1.25*x3
	0x0301,
	0x1F51,
	0x20A9,
	0x0300,	//////////////////////////////////////////////// gain 32x
	0xF300,	// overOBP_x0
	0xF404,	// overOBP_x1
	0xF518,	// overOBP_x2
	0xF620,	// overOBP_x3
	0xF740,	// overOBP_x4
	0xF800,	// overOBP_wt0
	0xF918,	// overOBP_wt1
	0xFA20,	// overOBP_wt2
	0xFB20,	// overOBP_wt3
	0xFC30,	// overOBP_wt4
	0x0301,
	0xA3E0, // blc_top_th
	0xA470, // blc_bot_th
	0xA502, // ablc_update
	0x0301,	// Limiter reference fitting due to gain
	0xF601,	// bs_ofst0
	0xF70C,	// bs_ofst1
	0xF81A,	// bs_ofst2
	0xF924,	// bs_ofst3
	0xFA28,	// bs_ofst4
	0xFB2A,	// bs_ofst5
	0xFC2C,	// bs_ofst6
	0xFD3F,	// bs_ofst_max
	0xFE00,	// bs_ofst_min
	0x0302,	////////////////////////////////////////////////////////////////////////////////////////////// ablc
	0x05DF,	////////////////////////////////////////////////////////////////////////////////////////////// ablc
	0xFFFF, 
};

static const unsigned short init_ntsc_lvds_config[] = {
	//////////////////////////////////////// start up ////////////////////////////////////////
	//////////////////////// BANK A
	0x0300,
	0x0503, // mirror
	0x0608, // framewidth_h
	0x0797, // framewidth_l
	0x2569, // clkdiv1
	0x268B, // clkdiv2
	0x1F04, // monitor
	0x4121, // pll_m_cnt (21)
	0x4204, // pll_r_cnt (04)
	0x4010, // pll_control   
	0xFF0A, // delay 10ms                
	0x4000, // pll_control on
	//////////////////////// BANK B
	//////////////////////////////////////////////////////// CDS
	0x0301,	// B bank									
	0x5708,	// lsenb start H									
	0x587F,	// lsenb start L									
	0x5908,	// lsenb stop H									
	0x5A6F,	// lsenb stop L									
	0x5300,	// ls rst start H									
	0x5402,	// ls rst start L									
	0x5508,	// ls rst stop H									
	0x567F,	// ls rst stop L									
	0x6700,	// ls txb start H									
	0x68DC,	// ls txb start L									
	0x6900,	// ls txb stop H									
	0x6AF0,	// ls txb stop L									
	0x5B00,	// sample start H									
	0x5C00,	// sample start L									
	0x5D08,	// sample stop H									
	0x5E7F,	// sample stop L									
	0x5F00,	// store start H									
	0x6000,	// store start L									
	0x6100,	// store stop H									
	0x62C8,	// store stop L									
	0x9900,	// tx illum start H									
	0x9ADC,	// tx illum start L									
	0x9B08,	// tx illum stop H									
	0x9C7F,	// tx illum stop L
	0x7B02,	// inven start H									
	0x7CA8,	// inven start L									
	0x7D07,	// inven stop H									
	0x7EDA,	// inven stop L									
	0x6F02,	// load start H									
	0x70A8,	// load start L									
	0x7107,	// load stop H									
	0x72DA,	// load stop L									
	0x7302,	// latch en start H									
	0x74B2,	// latch en start L									
	0x7507,	// latch en stop H									
	0x76D0,	// latch en stop L									
	0x7708,	// latch en start2 H									
	0x7895,	// latch en start2 L									
	0x7908,	// latch en stop2 H									
	0x7A96,	// latch en stop2 L									
	//0xB740,	// adc offset									
	0x8F00,	// adc offset stop H									
	0x90D2,	// adc offset stop L									
	0x8B02,	// ramp start H									
	0x8CBC,	// ramp start L									
	0x8D08,	// ramp stop H									
	0x8E48,	// ramp stop L									
	0x8708,	// transfer start H									
	0x8834,	// transfer start L									
	0x8908,	// transfer stop H									
	0x8A3E,	// transfer stop L									
	0x9508,	// refhold start H									
	0x965C,	// refhold start L									
	0x9708,	// refhold stop H									
	0x988F,	// refhold stop L									
	0x9108,	// atten rst start H									
	0x925C,	// atten rst start L									
	0x9308,	// atten rst stop H									
	0x9497,	// atten rst stop L									
	0x7F08,	// pbhold start H									
	0x805C,	// pbhold start L									
	0x8108,	// pbhold stop H									
	0x828F,	// pbhold stop L									
	0x8308,	// col pbhold start H									
	0x845C,	// col pbhold start L									
	0x8508,	// col pbhold stop H									
	0x868F,	// col pbhold stop L									
	0xB908,	// col pbhold2 start H									
	0xBA5C,	// col pbhold2 start L									
	0xBB08,	// col pbhold2 stop H									
	0xBC8F,	// col pbhold2 stop L									
	0xA10B,	// COFFSET H									
	0xA2A0,	// COFFSET L									
	0x3600,	// TG hsync start H									
	0x37A2,	// TG hsync start L									
	0x3808,	// TG hsync stop H									
	0x3932,	// TG hsync stop L
	0x3E00,	// blhstart_h
	0x3FA2,	// blhstart_l
	0x4008,	// blhstop_h
	0x4132,	// blhstop_l
	//////////////////////////////////////////////////////////////////////////////////////////////
	0x0301,
	0x1604, // bayer_control_01 (led_dsel)
	0x1E0E, // bayer_control_09 (bsmode)
	0x2604, // blacksunth_h
	0xB740, // adcoffset
	////////////////////////////////////////////////////////////////////////////////////////////// ablc
	0x0300,
	0x3695, // ablc_pd [1], ablc_res_con [0]
	0x3B27, // ablc_step_con [5:3]
	0x0301,
	0x1F51, // bayer_control_10 (ablc_en)
	0x20A9,	// bayer_control_11 (bs_ofst_en)
	0x0300,
	0xF300,	// overOBP_x0
	0xF404,	// overOBP_x1
	0xF518,	// overOBP_x2
	0xF620,	// overOBP_x3
	0xF740,	// overOBP_x4
	0xF800,	// overOBP_wt0
	0xF918,	// overOBP_wt1
	0xFA20,	// overOBP_wt2
	0xFB20,	// overOBP_wt3
	0xFC30,	// overOBP_wt4
	0x0301,
	0xA3E0, // blc_top_th
	0xA470, // blc_bot_th
	0xA502, // ablc_update
	////////////////////////////////////////////////////////////////////////////////////////////// limiter
	0x0301,	// Limiter reference fitting due to gain
	0xF601,	// bs_ofst0
	0xF70C,	// bs_ofst1
	0xF81A,	// bs_ofst2
	0xF924,	// bs_ofst3
	0xFA28,	// bs_ofst4
	0xFB2A,	// bs_ofst5
	0xFC2C,	// bs_ofst6
	0xFD3F,	// bs_ofst_max
	0xFE00,	// bs_ofst_min
	//////////////////////// BANK C
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
	0x1303, // lvds_bias_0 lvds_bgr 33
	0x0303,
	0x040E, // LVDS off
	0xFF0A, // delay 10ms
	0x044E, // LVDS on
	0xFFFF, 
};

#else

static const unsigned short init_ntsc_bayer_config[] = {
	0x0300, 
	0x0503, 
	0x0608, // framewidth_h
	0x0797, // framewidth_l
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
	0x0300,
	0x2998,// output Hi-z release, pae_en
	0x0302,
	0x9B20, // sync_control_1[5] : hsyncAllLines       
	0x0301,
	0x19C3, // bayer_control_04
	0x0300,
	0x3301, // pixelbias
	0x3402, // compbias
	0x0301,
	0x2603, // blacksun_th_h
	0x0301,
	0xC004, // inttime_h  
	0xC14F, // inttime_m  
	0xC200, // inttime_l  
	0xC300, // globalgain 
	0xC440, // digitalgain
	0x0302,
	0x9B20, 
	0x0300, 
	0x2569, // clkdiv1
	0x268B, // clkdiv2
	0x4121, // pll_m_cnt (21)
	0x4204, // pll_r_cnt (04)
	0x4010, // pll_control   
	0xFF0A, // delay 10ms                
	0x4000, //pll_control
	0xFFFF, 
};

static const unsigned short init_ntsc_lvds_config[] = {
	0x0300,
	0x0503, // mirror
	0x0608, // framewidth_h
	0x0797, // framewidth_l
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
	0x2569, // clkdiv1
	0x268B, // clkdiv2
	0x4010, //pll_control
	0x0301,
	0x3600, //tghstart_h	00  
	0x37A2, //tghstart_l	C2
	0x3808, //tghstop_h	08    
	0x3932, //tghstop_l	52  
	0x3E00, // blhstart_h
	0x3FA2, // blhstart_l
	0x4008, // blhstop_h
	0x4132, // blhstop_l
	0xA10B, // coffset_h 0B
	0xA2A2, // coffset_l 82
	0x0301,
	0x1604, // bayer_control_01 (led_dsel)
	0x1E0E, // bayer_control_09 (bsmode)
	0x0301,
	0x20CB,	// bs_ofst_en
	0x0301,	// Limiter reference fitting due to gain
	0xF601,	// bs_ofst0
	0xF701,	// bs_ofst1
	0xF801,	// bs_ofst2
	0xF901,	// bs_ofst3
	0xFA20,	// bs_ofst4
	0xFB2E,	// bs_ofst5
	0xFC2E,	// bs_ofst6 
	0xFD3F,	// bs_ofst_max
	0xFE00,	// bs_ofst_min
	0xB730, // adcoffset
	0x0302,
	0x9B20,	// sync control
	0x0303,
	0x0504,	// lvds_control_1
	0x0604,	// lvds_control_2
	0x0700,	// lvds_control_3
	0x0800,	// lvds_control_4
	0x1303, // lvds_bias_0 lvds_bgr 33
	0x0300,
	0x4000, //pll_control
	0x0303,
	0x040E, // LVDS off
	0x0303,
	0x044E, // LVDS on
	0xFFFF, 
};

#endif

/* 1920x1080 */
static struct sensor_ctrl_info init_sensor_values = {
//	.chip_id = 0x3210,
	.chip_id = 0x3120,
	.name = "ps3210m",
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

#endif // SENSOR_PS3210M_H_
