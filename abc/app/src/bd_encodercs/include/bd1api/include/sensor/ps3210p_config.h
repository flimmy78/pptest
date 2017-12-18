#ifndef SENSOR_PS3210P_H_
#define SENSOR_PS3210P_H_ 1

#include <bd_sns_ctrl.h>

#if defined(SENSOR_ESTYPE)
#error "PS3210P does not support es-type"
#else

static const unsigned short init_ntsc_bayer_config[] = {
#if 0	
	0x0300,
	0x2998, // output Hi-z release
	0x2842, // pad_drv, pclk_delay

	0x0301,
	0x1843, // blacksun
	0xB400, // maxmbl

	0x0300,
	0x0503, // mirror/flip
#if 0	
	/* get value from system team */
	0x0608, // frame0xidth_h        (08)
	0x0797, // frame0xidth_l        (97)

	0x0C00, // 0xindo0xx1_h					(00)
	0x0D0D, // 0xindo0xx1_l					(01)
	0x0E00, // 0xindo0xy1_h					(00)
	0x0F0E, // 0xindo0xy1_l					(02)
	0x1007, // 0xindo0xx2_h					(07)
	0x118C, // 0xindo0xx2_l					(8C)
	0x1204, // 0xindo0xy2_h					(04)
	0x1345, // 0xindo0xy2_l					(45)

	0x1400, // vsyncstartro0x_f0_h	(00)
	0x1517, // vsyncstartro0x_f0_l	(0D)
	0x1604, // vsyncstopro0x_f0_h		(04)
	0x1753, // vsyncstopro0x_f0_l		(53)
#else
	/* from lvds output */	
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
#endif

	0x352F,
	0x4121, // pll_m_cnt (21)
	0x4204, // pll_r_cnt (04)

	0x4010, // pll_control

	0xff32,

	0x4000, // pll_control on

	0x0301,
	0xC004,
	0xC15E,
	0xC200,
	0xC360,

	0x0300,
	0xD100, // adc_rst_start_h (08)
	0xD200, // adc_rst_start_l (94)
	0xD30A, // adc_rst_stop_h  (08)
	0xD400, // adc_rst_stop_l  (96)


	0x0301, // B bank

	0x5708, // lsenb edge1 H (2137)
	0x5887, // lsenb edge1 L
	0x5908, // lsenb edge2 H (2199)
	0x5A97, // lsenb edge2 L

	0x5300, // ls_rst(rst_sel=1) edge1 H (1)
	0x5401, // ls_rst(rst_sel=1) edge1 L
	0x5508, // ls_rst(rst_sel=1) edge2 H (2175)
	0x5656, // ls_rst(rst_sel=1) edge2 L

	0x6700, // ls_txb edge1 H (175)
	0x68AF, // ls_txb edge1 L
	0x6900, // ls_txb edge2 H (225)
	0x6AE1, // ls_txb edge2 L

	0x5B00, // sample edge1 H (13)
	0x5C0D, // sample edge1 L
	0x5D08, // sample edge2 H (2120)
	0x5E48, // sample edge2 L

	0x5F00, // store edge1 H (18)
	0x6012, // store edge1 L
	0x6100, // store edge2 H (149)
	0x6295, // store edge2 L

	0xB100, // ablc_en edge1 H (155)
	0xB29B, // ablc_en edge1 L


	0x0300, // A bank

	0xC500, // store_cobp edge1 H (18)
	0xC612, // store_cobp edge1 L
	0xC700, // store_cobp edge2 H (169)
	0xC8A9, // store_cobp edge2 L


	0x0301, // B bank

	0x9900, // tx_illum edge1 H (175)
	0x9AAF, // tx_illum edge1 L
	0x9B08, // tx_illum edge2 H (2120)
	0x9C48, // tx_illum edge2 L


	0x0300, // A bank

	0xC908, // black_load edge1 H (2160)
	0xCA70, // black_load edge1 L
	0xCB08, // black_load edge2 H (2143)
	0xCC5F, // black_load edge2 L

	0xCD08, // limiter_en edge1 H (2185)
	0xCE89, // limiter_en edge1 L
	0xCF08, // limiter_en edge2 H (2175)
	0xD07F, // limiter_en edge2 L


	0x0301, // B bank

	0x7B00, // inv_en edge1 H (0)
	0x7C00, // inv_en edge1 L
	0x7D08, // inv_en edge2 H (2200)
	0x7E98, // inv_en edge2 L

	0x6F00, // load edge1 H (238)
	0x70EE, // load edge1 L
	0x7107, // load edge2 H (2002)
	0x72D2, // load edge2 L

	0x7300, // latch_en edge1 H (240)
	0x74F0, // latch_en edge1 L
	0x7507, // latch_en edge2 H (2000)
	0x76D0, // latch_en edge2 L
	0x7708, // latch_en edge3 H (2196)
	0x7894, // latch_en edge3 L
	0x7908, // latch_en edge4 H (2198)
	0x7A96, // latch_en edge4 L

	0x8F00, // vramp edge1 H (243)
	0x90F3, // vramp edge1 L
	0x8B01, // vramp edge2 H (263)
	0x8C07, // vramp edge2 L
	0x8D08, // vramp edge4 H (2110)
	0x8E3E, // vramp edge4 L


	0x0300, // A bank

	0xD100, // adc_rstb edge1 H (0)
	0xD200, // adc_rstb edge1 L
	0xD308, // adc_rstb edge2 H (2300)
	0xD4FC, // adc_rstb edge2 L


	0x0301, // B bank

	0x8708, // transfer edge1 H (2188)
	0x888C, // transfer edge1 L
	0x8908, // transfer edge2 H (2194)
	0x8A92, // transfer edge2 L

	0x9508, // refhold edge1 H (2140)
	0x965C, // refhold edge1 L
	0x9708, // refhold edge2 H (2191)
	0x988F, // refhold edge2 L

	0x9108, // atten_rst edge1 H (2140)
	0x925C, // atten_rst edge1 L
	0x9308, // atten_rst edge2 H (2199)
	0x9497, // atten_rst edge2 L

	0x7F08, // pbhold edge1 H (2140)
	0x805C, // pbhold edge1 L
	0x8108, // pbhold edge2 H (2191)
	0x828F, // pbhold edge2 L

	0x8308, // col_pbhold edge1 H (2140)
	0x845C, // col_pbhold edge1 L
	0x8508, // col_pbhold edge2 H (2191)
	0x868F, // col_pbhold edge2 L

	0xB908, // col_pbhold2 edge1 H (2140)
	0xBA5C, // col_pbhold2 edge1 L
	0xBB08, // col_pbhold2 edge2 H (2191)
	0xBC8F, // col_pbhold2 edge2 L

	0x3600, // hsync edge1 H (250)
	0x37FA, // hsync edge1 L
	0x3808, // hsync edge2 H (2186)
	0x398A, // hsync edge2 L

	0xA10B, // hsync COffset H (2884)
	0xA244, // hsync COffset L

	0x3E00, // blhsync edge1 H (250)
	0x3FFA, // blhsync edge1 L
	0x4008, // blhsync edge2 H (2186)
	0x418A, // blhsync edge2 L

	0xA10B, // blhsync COffset H (2884)
	0xA244, // blhsync COffset L

	0x4200, // cobp sync edge1 H (150)
	0x4396, // cobp sync edge1 L
	0x4400, // cobp sync edge2 H (246)
	0x45F6, // cobp sync edge2 L

#if defined(USE_INTERBOARD)
	0x0302,
	0x9B24,
#else	
	0x0302,
	0x9B20,
#endif
#else
	/* 7 team test code */
	0x0300, //    
	0x2998, //	# output Hi-z release
	0x2842, // # pad_drv, pclk_delay
	0x0301, //
	0x1843, // # blacksun
	0xB400, // # maxmbl
	0x0300, //
	0x0503, //	# mirror/flip
	
  0x0608,  //	# framewidth_h        (08)
  0x0797,  //	# framewidth_l        (97)
#if 0
	// 1936x1096
  0x0C00,  // # windowx1_h
  0x0D01,  // # windowx1_l
  0x0E00,  // # windowy1_h
  0x0F02,  // # windowy1_l
  0x1007,  // # windowx2_h
  0x118C,  // # windowx2_l
  0x1204,  // # windowy2_h
  0x1345,  // # windowy2_l
  0x1400,  // # vsyncstartrow_f0_h
  0x1514,  // # vsyncstartrow_f0_l
  0x1604,  // # vsyncstoprow_f0_h
  0x1758,  // # vsyncstoprow_f0_l
#else
	/* from lvds output */	
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
#endif
	0x4121, // # pll_m_cnt (21)
	0x4204, // # pll_r_cnt (04)
	0x4010, // # pll_control $0010
	0x4000, // # pll_control on
	0x0301, //
	0xC004, //
	0xC15E, //
	0xC200, //
	0xC360, //
	0x0300, //
	0xD100, //	# adc_rst_start_h (08)
	0xD200, //	# adc_rst_start_l (94)
	0xD30A, //	# adc_rst_stop_h  (08)
	0xD400, //	# adc_rst_stop_l  (96)
	0x0301, //	# B bank
	0x5708, //	# lsenb edge1 H (2137)
	0x5887, //	# lsenb edge1 L
	0x5908, //	# lsenb edge2 H (2199)
	0x5A97, //	# lsenb edge2 L
	0x5300, //	# ls_rst(rst_sel=1) edge1 H (1)
	0x5401, //	# ls_rst(rst_sel=1) edge1 L
	0x5508, //	# ls_rst(rst_sel=1) edge2 H (2175)
	0x5656, //	# ls_rst(rst_sel=1) edge2 L
	0x6700, //	# ls_txb edge1 H (175)
	0x68AF, //	# ls_txb edge1 L
	0x6900, //	# ls_txb edge2 H (225)
	0x6AE1, //	# ls_txb edge2 L
	0x5B00, //	# sample edge1 H (13)
	0x5C0D, //	# sample edge1 L
	0x5D08, //	# sample edge2 H (2120)
	0x5E48, //	# sample edge2 L
	0x5F00, //	# store edge1 H (18)
	0x6012, //	# store edge1 L
	0x6100, //	# store edge2 H (149)
	0x6295, //	# store edge2 L
	0xB100, //	# ablc_en edge1 H (155)
	0xB29B, //	# ablc_en edge1 L
	0x0300, //	# A bank
	0xC500, //	# store_cobp edge1 H (18)
	0xC612, //	# store_cobp edge1 L
	0xC700, //	# store_cobp edge2 H (169)
	0xC8A9, //	# store_cobp edge2 L
	0x0301, //	# B bank
	0x9900, //	# tx_illum edge1 H (175)
	0x9AAF, //	# tx_illum edge1 L
	0x9B08, //	# tx_illum edge2 H (2120)
	0x9C48, //	# tx_illum edge2 L
	0x0300, //	# A bank
	0xC908, //	# black_load edge1 H (2160)
	0xCA70, //	# black_load edge1 L
	0xCB08, //	# black_load edge2 H (2143)
	0xCC5F, //	# black_load edge2 L
	0xCD08, //	# limiter_en edge1 H (2185)
	0xCE89, //	# limiter_en edge1 L
	0xCF08, //	# limiter_en edge2 H (2175)
	0xD07F, //	# limiter_en edge2 L
	0x0301, //	# B bank
	0x7B00, //	# inv_en edge1 H (0)
	0x7C00, //	# inv_en edge1 L
	0x7D08, //	# inv_en edge2 H (2200)
	0x7E98, //	# inv_en edge2 L
	0x6F00, //	# load edge1 H (238)
	0x70EE, //	# load edge1 L
	0x7107, //	# load edge2 H (2002)
	0x72D2, //	# load edge2 L
	0x7300, //	# latch_en edge1 H (240)
	0x74F0, //	# latch_en edge1 L
	0x7507, //	# latch_en edge2 H (2000)
	0x76D0, //	# latch_en edge2 L
	0x7708, //	# latch_en edge3 H (2196)
	0x7894, //	# latch_en edge3 L
	0x7908, //	# latch_en edge4 H (2198)
	0x7A96, //	# latch_en edge4 L
	0x8F00, //	# vramp edge1 H (243)
	0x90F3, //	# vramp edge1 L
	0x8B01, //	# vramp edge2 H (263)
	0x8C07, //	# vramp edge2 L
	0x8D08, //	# vramp edge4 H (2110)
	0x8E3E, //	# vramp edge4 L
	0x0300, //	# A bank
	0xD100, //	# adc_rstb edge1 H (0)
	0xD200, //	# adc_rstb edge1 L
	0xD308, //	# adc_rstb edge2 H (2300)
	0xD4FC, //	# adc_rstb edge2 L
	0x0301, //	# B bank
	0x8708, //	# transfer edge1 H (2188)
	0x888C, //	# transfer edge1 L
	0x8908, //	# transfer edge2 H (2194)
	0x8A92, //	# transfer edge2 L
	0x9508, //	# refhold edge1 H (2140)
	0x965C, //	# refhold edge1 L
	0x9708, //	# refhold edge2 H (2191)
	0x988F, //	# refhold edge2 L
	0x9108, //	# atten_rst edge1 H (2140)
	0x925C, //	# atten_rst edge1 L
	0x9308, //	# atten_rst edge2 H (2199)
	0x9497, //	# atten_rst edge2 L
	0x7F08, //	# pbhold edge1 H (2140)
	0x805C, //	# pbhold edge1 L
	0x8108, //	# pbhold edge2 H (2191)
	0x828F, //	# pbhold edge2 L
	0x8308, //	# col_pbhold edge1 H (2140)
	0x845C, //	# col_pbhold edge1 L
	0x8508, //	# col_pbhold edge2 H (2191)
	0x868F, //	# col_pbhold edge2 L
	0xB908, //	# col_pbhold2 edge1 H (2140)
	0xBA5C, //	# col_pbhold2 edge1 L
	0xBB08, //	# col_pbhold2 edge2 H (2191)
	0xBC8F, //	# col_pbhold2 edge2 L
	0x3600, //	# hsync edge1 H (250)
	0x37FA, //	# hsync edge1 L
	0x3808, //	# hsync edge2 H (2186)
	0x398A, //	# hsync edge2 L
	0xA10B, //	# hsync COffset H (2884)
	0xA244, //	# hsync COffset L
	0x3E00, //	# blhsync edge1 H (250)
	0x3FFA, //	# blhsync edge1 L
	0x4008, //	# blhsync edge2 H (2186)
	0x418A, //	# blhsync edge2 L
	0xA10B, //	# blhsync COffset H (2884)
	0xA244, //	# blhsync COffset L
	0x4200, //	# cobp sync edge1 H (150)
	0x4396, //	# cobp sync edge1 L
	0x4400, //	# cobp sync edge2 H (246)
	0x45F6, //	# cobp sync edge2 L
#endif
	0xFFFF, 
};

#endif

/* 1920x1080 */
static struct sensor_ctrl_info init_sensor_values = {
	.chip_id = 0x3210,
	.name = "ps3210p",
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

#endif // SENSOR_PS3210P_H_
