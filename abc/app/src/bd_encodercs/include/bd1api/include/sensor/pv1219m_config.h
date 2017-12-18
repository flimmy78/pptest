#ifndef SENSOR_PV1219M_H_
#define SENSOR_PV1219M_H_ 1

#include <bd_sns_ctrl.h>

#if defined(SENSOR_ESTYPE)
static const unsigned short initial_ntsc_bayer_config[] = {
	0x0300, 
	0x2F01,  // pad_control7        (01)
	0x2A01,  // pad_control2        (00)
	0x2B9C,  // pad_control3        (00)
	0x2E03,  // pad_control6        (00)
	0x30FF,  // pad_control8        (00)
	0x31FF,  // pad_control9        (00)
	0x3B45,  // analog_control_05   (41)
	0x2500,  // clkdiv2							(01)
	0x3F50,  // pll0_control1
	0xFF10, 
	0x3F40,  // pll0_control1
	0x0300, 
	0x0608,  // framewidth_h
	0x0797,  // framewidth_l
	0x400B,  // pll0_m_cnt  
	0x4101,  // pll0_r_cnt  
	0x24C1,  // clkdiv1     
	0x4A00,  // flicker_control1
	0x5401,  // fd_period_a_h
	0x5517,  // fd_period_a_m
	0x56C0,  // fd_period_a_l
	0x5701,  // fd_period_b_h
	0x5851,  // fd_period_b_m
	0x5980,  // fd_period_b_l
	0x4C03,  // fd_a_step_h  
	0x4DA4,  // fd_a_step_l  
	0x4E03,  // fd_b_step_h  
	0x4F09,  // fd_b_step_l  
	0x0300, 
	0x0402, 	// chip mode (00 : SMPTE, 01 : sampling, 02 : ccir601)
	0x0503, 
//	0x1517,  // vsyncstartrow_l
	0x2442,  // clkdiv1 [7]:p_pp_equal, adc_clkdiv
	0x0303, 	//Image format control
	0xB681, 
	0x0300, 
	0x1E04,  // monitor
	0x0307, 
	0x14C0,  // smp296_ctrl1 
	0x0300,  ////////////////////////// Start Settings //////////////////////////////// //Bias control
	0x3503,  // pixelbias
	0x3603,  // compbias
	0x37E8,  // tx_bypass
	0x385E,   // PCP 010 4.06v, PCP Ldo 111 4.41v
	0x39BC,   // NCP 10100 -1.105v
	0x3B80,   // NCP Ldo 11000 -0.97v 
	0x3A40,   // Rangesel 000000
	0x0301, 
	0xB740,  // adcoffset
	0x0301, 
	0xF63C,  //bs_ofst0	
	0x0301, 	// B bank	//////////////////////////////////////////////////////////////////////// cds
	0x5708, 	// lsenb edge1 H (2175)
	0x587F, 	// lsenb edge1 L
	0x5908, 	// lsenb edge2 H (2195)
	0x5A93, 	// lsenb edge2 L
	0x5300, 	// ls_rst edge1 H (20)
	0x5414, 	// ls_rst edge1 L
	0x5508, 	// ls_rst edge2 H (2175)
	0x567F, 	// ls_rst edge2 L
	0x6700, 	// ls_txb edge1 H (230)
	0x68E6, 	// ls_txb edge1 L
	0x6901, 	// ls_txb edge2 H (260)
	0x6A04, 	// ls_txb edge2 L
	0x5B00, 	// sample edge1 H (0)
	0x5C00, 	// sample edge1 L
	0x5D03, 	// sample edge2 H (800)
	0x5E20, 	// sample edge2 L
	0x5F00, 	// store edge1 H (0)
	0x6000, 	// store edge1 L
	0x6100, 	// store edge2 H (220)
	0x62DC, 	// store edge2 L
	0x9900, 	// tx_illum edge1 H (230)
	0x9AE6, 	// tx_illum edge1 L
	0x9B03, 	// tx_illum edge2 H (800)
	0x9C20, 	// tx_illum edge2 L
	0x7B03, 	// inv_en edge1 H (870)
	0x7C66, 	// inv_en edge1 L
	0x7D08, 	// inv_en edge2 H (2174)
	0x7E7E, 	// inv_en edge2 L
	0x6F03, 	// load edge1 H (872)
	0x7068, 	// load edge1 L
	0x7108, 	// load edge2 H (2172)
	0x727C, 	// load edge2 L
	0x7303, 	// latch_en edge1 H (874)
	0x746A, 	// latch_en edge1 L
	0x7508, 	// latch_en edge2 H (2170)
	0x767A, 	// latch_en edge2 L
	0x7708, 	// latch_en edge3 H (2196)
	0x7894, 	// latch_en edge3 L
	0x7908, 	// latch_en edge4 H (2198)
	0x7A96, 	// latch_en edge4 L
	0x8F00, 	// vramp edge1 H (225)
	0x90E1, 	// vramp edge1 L
	0x8B03, 	// vramp edge2 H (890)
	0x8C7A, 	// vramp edge2 L
	0x8D08, 	// vramp edge4 H (2175)
	0x8E7F, 	// vramp edge4 L
	0x8708, 	// transfer edge1 H (2175)
	0x887F, 	// transfer edge1 L
	0x8908, 	// transfer edge2 H (2180)
	0x8A84, 	// transfer edge2 L
	0x9500, 	// refhold edge1 H (2)
	0x9602, 	// refhold edge1 L
	0x9700, 	// refhold edge2 H (30)
	0x981E, 	// refhold edge2 L
	0x9100, 	// atten_rst edge1 H (0)
	0x9200, 	// atten_rst edge1 L
	0x9300, 	// atten_rst edge2 H (50)
	0x9432, 	// atten_rst edge2 L
	0x7F00, 	// pbhold edge1 H (2)
	0x8002, 	// pbhold edge1 L
	0x8100, 	// pbhold edge2 H (30)
	0x821E, 	// pbhold edge2 L
	0x8300, 	// col_pbhold edge1 H (2)
	0x8402, 	// col_pbhold edge1 L
	0x8500, 	// col_pbhold edge2 H (30)
	0x861E, 	// col_pbhold edge2 L
	0xB900, 	// col_pbhold2 edge1 H (2)
	0xBA02, 	// col_pbhold2 edge1 L
	0xBB00, 	// col_pbhold2 edge2 H (30)
	0xBC1E, 	// col_pbhold2 edge2 L
	0x3600, 	// tg hsync edge1 H (226)
	0x37E2, 	// tg hsync edge1 L
	0x3808, 	// tg hsync edge2 H (2162)
	0x3972, 	// tg hsync edge2 L
	0xA10B, 	// tg hsync COffset H (2912)
	0xA260, 	// tg hsync COffset L
	0x3E00, 	// blhstart H
	0x3FE2, 	// blhstart L
	0x4008, 	// blhstop H
	0x4172, 	// blhstop L
	0xFFFF,  
};

#else

static const unsigned short initial_ntsc_bayer_config[] = {
	0xFFFF,  
};

#endif

/* 1920x1080 */
static struct sensor_ctrl_info init_sensor_values = {
	.chip_id = 0x1219,
	.name = "pv1219m",
	.chip_addr = 0x76,
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


#endif // SENSOR_PV1219M_H_
