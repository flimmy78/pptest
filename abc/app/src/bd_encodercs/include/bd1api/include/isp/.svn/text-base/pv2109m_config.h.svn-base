#ifndef ISP_PV2109M_H_
#define ISP_PV2109M_H_ 1

#include <bd_sns_ctrl.h>

#if defined(SENSOR_ESTYPE)
static const unsigned short init_p2109m_bayer_config[] = {
	/*20151221*/
	0x0300,	// // bank A
	0x0606,	// // framewidth_h   
	0x0771,	// // framewidth_l  
	0x0802,	// // fd_fheight_a_h 
	0x09ED,	// // fd_fheight_a_l 
	0x0a02,	// // fd_fheight_b_h 
	0x0bED,	// // fd_fheight_b_l 
	0x0300,	// // bank A
	//0x2354, // i2c_control_1 54
	0x2500,	// // clkdiv1 , mclk_div 00b:1/1 ,pclk_div = 1/1                   
	//0x2502,	// // clkdiv1 , mclk_div 00b:1/1 ,pclk_div = 1/1                   
	0x2681,	// // clkdiv2 , p_pp_equal => 1b:pclk = ppclk, dclk enable                  
	0x0302,	// // bank C
	//0x05DF,	// // isp_func_1 (default(DC), bayer sequence	//
	0x05DD,	// // isp_func_1 (default(DC), bayer sequence	// need to set sync_control_1 to 0x20DC ==> 0x24
	0x0300,	// // bank A      
	0x0c00,	// // windowx1_h   , default(00), pi2008(00)
	0x0d05,	// // windowx1_l   , default(05)  pi2008(01)
	0x0e00,	// // windowy1_h   , default(00)  pi2008(00)
	0x0f05,	// // windowy1_l   , default(05)  pi2008(01)
	0x1005,	// // windowx2_h   , default(07)  pi2008(07)
	0x1104,	// // windowx2_l   , default(84)  pi2008(80)
	0x1202,	// // windowy2_h   , default(04)  pi2008(04)
	0x13d4,	// // windowy2_l   , default(3C)  pi2008(38)
	0x1400,	// // vsyncstartrow_f0_h  , default(00)  
	0x150B,	// // vsyncstartrow_f0_l  , default(1a)   , pi2008(0b)
	0x1602,	// // vsyncstoprow_f0_h   , default(04)   
	0x17E9,	// // vsyncstoprow_f0_l   , default(51)   , pi2008(43)
	0x0300, 
	0xD405,	// zoom_x_h
	0xD500,	//# # zoom_x_l       
	0xD602,	//# # zoom_y_h         
	0xD7D0,	//# # zoom_y_l 
	0x0300,	// // bank A
	0x0400,	// // chip_mode : 20b smpte296
	0x0300,	// // bank A
	0xE300,	// // initset_fcnt
	0x0300,	// // bank A
	//0xD381,	// // smp296_ctrl1 => smpte296mode : 1b : 60fps			// problem......
	0xD3C2,	// // smp296_ctrl1 => smpte296mode : 1b : 60fps
	//0x0300,	// // bank A
	//0x2350,	// // i2c_control_1
	0x0301,	// // bank B
#if 0
	0x5964,	// i2c_slave_addr		0x32 (0x64)

	0x0302,	//   // bank C
	0xB300,	//	// ae_fwx1_h 
	0xB401,	//	// ae_fwx1_l 
	0xB505,	//	// ae_fwx2_h 
	0xB600,	//	// ae_fwx2_l 
	0xB700,	//	// ae_fwy1_h 
	0xB801,	//	// ae_fwy1_l 
	0xB902,	//	// ae_fwy2_h 
	0xBAD0,	//	// ae_fwy2_l 
	0xBB01,	//	// ae_cwx1_h 
	0xBCAB,	//	// ae_cwx1_l 
	0xBD03,	//	// ae_cwx2_h 
	0xBE56,	//	// ae_cwx2_l 
	0xBF00,	//	// ae_cwy1_h 
	0xC0F1,	//	// ae_cwy1_l 
	0xC101,	//	// ae_cwy2_h 
	0xC2F0,	//	// ae_cwy2_l 
	0xC302,	//	// ae_xaxis_h
	0xC481,	//	// ae_xaxis_l
	0xC501,	//	// ae_yaxis_h
	0xC669,	//	// ae_yaxis_l
	0xC700,	//	// awb_wx1_h 
	0xC801,	//	// awb_wx1_l 
	0xC905,	//	// awb_wx2_h 
	0xCA00,	//	// awb_wx2_l 
	0xCB00,	//	// awb_wy1_h 
	0xCC01,	//	// awb_wy1_l 
	0xCD02,	//	// awb_wy2_h 
	0xCED0,	//	// awb_wy2_l             	
#endif
#if 0
	0x0305,
	0xE464, // cis_chip_addr
	0xE301, // auto_cis_set_en
	0x6504, // sif_control0
	0x0301,
	0xA501, // core_set0_addr_h  inttime_h
	0xA631, // core_set0_addr_l
	0xA701, // cis_set0_addr_h
	0xA8C0, // cis_set0_addr_l
	0xA901, // core_set1_addr_h  inttime_m
	0xAA32, // core_set1_addr_l
	0xAB01, // cis_set1_addr_h
	0xACC1, // cis_set1_addr_l
	0xAD01, // core_set2_addr_h  inttime_l
	0xAE33, // core_set2_addr_l
	0xAF01, // cis_set2_addr_h
	0xB0C2, // cis_set2_addr_l
	0xB101, // core_set3_addr_h  globalgain
	0xB234, // core_set3_addr_l
	0xB301, // cis_set3_addr_h
	0xB4C3, // cis_set3_addr_l
	0xB501, // core_set4_addr_h  digitalgain
	0xB635, // core_set4_addr_l
	0xB701, // cis_set4_addr_h
	0xB8C4, // cis_set4_addr_l
	0xE502, // cis_set_startrow0_h
	0xE6C3, // cis_set_startrow0_l
	0xE950, // num_cis_set_reg
	0xEB02, // cis_read_startrow_h
	0xECA9, // cis_read_startrow_l
	0xED01, // cis_led_addr_h
	0xEEDB, // cis_led_addr_l
	0xEF01, // led_read_en
	0xF001, // cis_obp_addr_h
	0xF1DD, // cis_obp_addr_l
	0xF201, // obp_read_en
#endif
	0xFFFF, 
};   

#else

static const unsigned short init_p2109m_bayer_config[] = {
	0xFFFF, 
};   

#endif

/* 1280x720 */
static struct isp_ctrl_info init_p2109m_values = {
#if defined(SENSOR_ESTYPE)
	.chip_id = 0xbd01,
#else
	.chip_id = 0x2210,
#endif
	.name = "pv2109",
	.chip_addr = 0x30,
	.intf_supported = (ISP_SUPPORT_BAYER),
	.intf_mode = {
		init_p2109m_bayer_config, 0,
	},
};

#endif // ISP_PV2109M_H_
