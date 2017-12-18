#ifndef SENSOR_PV2109M_H_
#define SENSOR_PV2109M_H_ 1

#include <bd_sns_ctrl.h>

#if defined(SENSOR_ESTYPE)
static const unsigned short init_ntsc_bayer_config[] = {
	/*20151221*/
	0x0300,
	0x2F01, // pad_control7        (01)
	0x2A01, // pad_control2        (00)
	0x2B9C, // pad_control3        (00)
	0x2E03, // pad_control6        (00)
	0x30FF, // pad_control8        (00)
	0x31FF, // pad_control9        (00)
	0x3B45, // analog_control_05   (41)
	0x2500, // clkdiv2							(01)
	0x3F50, // pll0_control1
	0xFF10,
	0x3F40, // pll0_control1
	0x0300,
	0x0606, // framewidth_h
	0x0771, // framewidth_l
	0x400B, // pll0_m_cnt  
	0x4102, // pll0_r_cnt  
	0x2462, // clkdiv1     
	0x4A08, // flicker_control1
	0x5400, // fd_period_a_h
	0x55BA, // fd_period_a_m
	0x5600, // fd_period_a_l
	0x5700, // fd_period_b_h
	0x58E1, // fd_period_b_m
	0x5900, // fd_period_b_l
	0x4C05, // fd_a_step_h  
	0x4D79, // fd_a_step_l  
	0x4E04, // fd_b_step_h  
	0x4F8D, // fd_b_step_l  
	0x0300,
	0x0402,	// chip mode (00 : SMPTE, 01 : sampling, 02 : ccir601)
	0x0503,
	0x1517, // vsyncstartrow_l
	0x2442, // clkdiv1 [7]:p_pp_equal, adc_clkdiv
	0x0303,	//Image format control
	0xB681,
	0x0300,
	0x1E04, // monitor
	0x0301,
	0x1604, // led_dsel
	0x0302,
	0x1C14, // dpc_offset
	0x0300,
	0x3501, // pixelbias
	0x3603, // compbias
	0x374F, // only PCP bypass
	0x39BC,  // NCP 10111 about -1.2v
	0x3BB8,  // NCP Ldo 10111 -0.95v 
	0x3A40,  // Rangesel 000000
	0x0301,
	0xB740, // adcoffset
	0x0301,
	0xF630, //bs_ofst0	
	0x0301,	// B bank	//////////////////////////////////////////////////////////////////////// cds
	0x5706,	// lsenb edge1 H (1631)
	0x585F,	// lsenb edge1 L
	0x5906,	// lsenb edge2 H (1648)
	0x5A70,	// lsenb edge2 L
	0x5300,	// ls_rst edge1 H (2)
	0x5402,	// ls_rst edge1 L
	0x5506,	// ls_rst edge2 H (1631)
	0x565F,	// ls_rst edge2 L
	0x6700,	// ls_txb edge1 H (135)
	0x6887,	// ls_txb edge1 L
	0x6900,	// ls_txb edge2 H (175)
	0x6AAF,	// ls_txb edge2 L
	0x5B00,	// sample edge1 H (0)
	0x5C00,	// sample edge1 L
	0x5D06,	// sample edge2 H (1631)
	0x5E5F,	// sample edge2 L
	0x5F00,	// store edge1 H (0)
	0x6000,	// store edge1 L
	0x6100,	// store edge2 H (125)
	0x627D,	// store edge2 L
	0x9900,	// tx_illum edge1 H (125)
	0x9A7D,	// tx_illum edge1 L
	0x9B06,	// tx_illum edge2 H (1631)
	0x9C5F,	// tx_illum edge2 L
	0x7B00,	// inv_en edge1 H (185)
	0x7CB9,	// inv_en edge1 L
	0x7D06,	// inv_en edge2 H (1615)
	0x7E4F,	// inv_en edge2 L
	0x6F00,	// load edge1 H (187)
	0x70BB,	// load edge1 L
	0x7106,	// load edge2 H (1613)
	0x724D,	// load edge2 L
	0x7300,	// latch_en edge1 H (190)
	0x74BE,	// latch_en edge1 L
	0x7506,	// latch_en edge2 H (1611)
	0x764B,	// latch_en edge2 L
	0x7706,	// latch_en edge3 H (1647)
	0x786F,	// latch_en edge3 L
	0x7906,	// latch_en edge4 H (1648)
	0x7A70,	// latch_en edge4 L
	0x8F00,	// vramp edge1 H (130)
	0x9082,	// vramp edge1 L
	0x8B00,	// vramp edge2 H (190)
	0x8CBE,	// vramp edge2 L
	0x8D06,	// vramp edge4 H (1632)
	0x8E60,	// vramp edge4 L
	0x8706,	// transfer edge1 H (1621)
	0x8855,	// transfer edge1 L
	0x8906,	// transfer edge2 H (1628)
	0x8A5C,	// transfer edge2 L
	0x9506,	// refhold edge1 H (1632)
	0x9660,	// refhold edge1 L
	0x9706,	// refhold edge2 H (1643)
	0x986B,	// refhold edge2 L
	0x9106,	// atten_rst edge1 H (1630)
	0x925E,	// atten_rst edge1 L
	0x9306,	// atten_rst edge2 H (1649)
	0x9471,	// atten_rst edge2 L
	0x7F06,	// pbhold edge1 H (1632)
	0x8060,	// pbhold edge1 L
	0x8106,	// pbhold edge2 H (1643)
	0x826B,	// pbhold edge2 L
	0x8306,	// col_pbhold edge1 H (1632)
	0x8460,	// col_pbhold edge1 L
	0x8506,	// col_pbhold edge2 H (1643)
	0x866B,	// col_pbhold edge2 L
	0xB906,	// col_pbhold2 edge1 H (1632)
	0xBA60,	// col_pbhold2 edge1 L
	0xBB06,	// col_pbhold2 edge2 H (1643)
	0xBC6B,	// col_pbhold2 edge2 L
	0x3600,	// tg hsync edge1 H (190)
	0x37BE,	// tg hsync edge1 L
	0x3805,	// tg hsync edge2 H (1486)
	0x39CE,	// tg hsync edge2 L
	0xA10C,	// tg hsync COffset H (3268)
	0xA2C4,	// tg hsync COffset L
	0xFFFF , 
};
#else
static const unsigned short init_ntsc_bayer_config[] = {
	0xFFFF , 
};
#endif

/* 1280x720 */
static struct sensor_ctrl_info init_sensor_values = {
	.chip_id = 0x2109,
	.name = "pv2109m",
	.chip_addr = 0x32,
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


#endif // SENSOR_PV2109M_H_
