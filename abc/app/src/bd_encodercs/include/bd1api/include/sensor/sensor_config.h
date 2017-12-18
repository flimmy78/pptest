#ifndef SENSOR_CONFIGS_CS_H_
#define SENSOR_CONFIGS_CS_H_

#include <bd_sns_ctrl.h>

#define DATA_END_FLAG	0xFFFF

static const unsigned short frm_attr_1080[] = {
#if defined(SUPPORT_PS1210K) || defined(SUPPORT_PS1210M) || defined(SUPPORT_PS3210M) || defined(SUPPORT_PS3210P) || defined(SUPPORT_PV1219M) || defined(SUPPORT_PS3210K)	
	0x0300,                     // Bank-A 
	0x2900, // pad_control3 (v/h/d pad enable)
#if defined(USE_SENSOR_25FPS)	
	0x060A,	// framewidth_h		
	0x074F,	// framewidth_l		// 0xA4F (2639)
#else
	0x0608, // framewidth_h		
	0x0797, // framewidth_l		// 0x897 (2199)						
#endif
	0x0804,	// frame_height_a_h
	0x0964,	// frame_height_a_l	// 0x446 (1124)
	0x0a04, // frame_height_b_h	
	0x0b64, // frame_height_b_l
	0x0c00,	// windowx1_h 
	0x0d01,	// windowx1_l 
	0x0e00,	// windowy1_h 
	0x0f05,	// windowy1_l 
	0x1007,	// windowx2_h 
	0x1190,	// windowx2_l 
	0x1204,	// windowy2_h 
	0x134C,	// windowy2_l 
	0x1400,	// vsyncstartrow_f0_h 
	0x150A,	// vsyncstartrow_f0_l 
	0x1604,	// vsyncstoprow_f0_h  
	0x1751,	// vsyncstoprow_f0_l  
	0x1800, // vsynccolume_h
	0x1902, // vsynccolume_l
#endif	
	DATA_END_FLAG ,
};

static const unsigned short frm_attr_720[] = {
#if defined(SUPPORT_PO3100K) || defined(SUPPORT_PV2109M) || defined(SUPPORT_PS4100K)
	0x0300,                     // Bank-A 
	0x2900, // pad_control3 (v/h/d pad enable)
#if defined(USE_SENSOR_25FPS)	
	0x0607,	// framewidth_h   
	0x07BB,	// framewidth_l  	// 0x7BB	(1979)
#else
	0x0606,	// framewidth_h   
	0x0771,	// framewidth_l  	// 0x671	(1649)
#endif
	0x0802,	// fd_fheight_a_h 
	0x09ED,	// fd_fheight_a_l 	// 0x2ED	(749)
	0x0a02,	// fd_fheight_b_h 
	0x0bED,	// fd_fheight_b_l 
	
	0x0c00,	// windowx1_h   
	0x0d01,	// windowx1_l   
	0x0e00,	// windowy1_h   
	0x0f01,	// windowy1_l  		 
	0x1005,	// windowx2_h   
	0x1110,	// windowx2_l   
	0x1202,	// windowy2_h   
	0x13E1,	// windowy2_l  
	0x1400,	// vsyncstartrow_f0_h   
	0x150F,	// vsyncstartrow_f0_l   
	0x1602,	// vsyncstoprow_f0_h    
	0x17EF,	// vsyncstoprow_f0_l    
	0x1800,	// vsynccolume    
	0x1902,	// vsynccolume    
#endif	
	DATA_END_FLAG ,
};

static const unsigned short frm_attr_720_var[] = {
#if defined(SUPPORT_PS7100K)
	//////////////////// BGR start-up //////////////////////
	0x0300,
	0x3E38,
	0x3E30,

	0x0300,
	0x2998,	// output Hi-z release

#if defined(USE_SENSOR_25FPS)	
	0x060B,
	0x073F,
#else
	0x0609,	// framewidth_h        (0B)
	0x075F,	// framewidth_l        (21)
//	0x0606,	// framewidth_h        (0B)
//	0x0771,	// framewidth_l        (21)
#endif

	0x0802,	// // fd_fheight_a_h 
	0x09ED,	// // fd_fheight_a_l 	// 0x2ED	(749)
	0x0a02,	// // fd_fheight_b_h 
	0x0bED,	// // fd_fheight_b_l 
	
	0x0c00,	// // windowx1_h   
	0x0d01,	// // windowx1_l   			(1296, 1280 + 16)	
	0x0e00,	// // windowy1_h   
	0x0f01,	// // windowy1_l  		 
	0x1005,	// // windowx2_h   
	0x1110,	// // windowx2_l   
	0x1202,	// // windowy2_h   
	0x13E3,	// // windowy2_l   		(736, 720 + 16)
	
	0x1400,	// vsyncstartrow_f0_h	(00)
	0x150D,	// vsyncstartrow_f0_l	(0D)
	0x1602,	// vsyncstoprow_f0_h		(02)
	0x17ED,	// vsyncstoprow_f0_l		(EB)
	
	0x1800,	// // vsynccolume   , 
	0x1902,	// // vsynccolume   , 
#endif

	DATA_END_FLAG ,
};


static const unsigned short flp_attr_hor[] = {
	0x0300,                     // Bank-A
	0x0501,
	DATA_END_FLAG ,
};

static const unsigned short flp_attr_ver[] = {
	0x0300,                     // Bank-A
	0x0502,
	DATA_END_FLAG ,
};

static const unsigned short flp_attr_all[] = {
	0x0300,                     // Bank-A
	0x0503,
	DATA_END_FLAG ,
};

static const unsigned short flp_attr_non[] = {
	0x0300,                     // Bank-A
	0x0500,
	DATA_END_FLAG ,
};

static const unsigned short bsq_attr_rggb[] = {
	0x0302,						// Bank-C
	0x05DC,
	DATA_END_FLAG ,
};

static const unsigned short bsq_attr_gbrg[] = {
	0x0302,			// Bank-C
	0x05DD,
	DATA_END_FLAG ,
};

static const unsigned short bsq_attr_grbg[] = {
	0x0302,			// Bank-C
	0x05DE,
	DATA_END_FLAG ,
};

static const unsigned short bsq_attr_bggr[] = {
	0x0302,			// Bank-C
	0x05DF,
	DATA_END_FLAG ,
};

static const unsigned short clk_attr_hallline[] = {
	0x0302,			// Bank-C
	0x9B20,
	DATA_END_FLAG ,
};

/* PC3089N */
static const unsigned short pc3089n_ntsc_bt601_attr[] = {
	DATA_END_FLAG ,
};

/* PC7030K */
static const unsigned short pc7030k_ntsc_bt656_attr[] = {
	DATA_END_FLAG ,
};

static const unsigned short pc7030k_pal_bt656_attr[] = {
	DATA_END_FLAG ,
};

/* PO3100K */
static const unsigned short po3100k_ntsc_bt1120_attr[] = {
	DATA_END_FLAG, 
};

/* PS1210K */
static const unsigned short ps1210k_ntsc_bayer_attr[] = {		// cs
#if defined(SUPPORT_PS1210K)
	/************** PLL config ***************/	
	0x0300,                     // Bank-A 
	0x4010, // # pll_control1 off
	0x2510, // # clkdiv1
	0x26C3,	// # clkdiv2
	0x4121, // # pll_m_cnt
	0x4204, // # pll_n_cnt
	0xFF32, 
	0x4000, // # pll_control1 on
	/************** PLL config ***************/	

	0x0300,      //////////////////////////////////  Bank-A
	0x2842, //
	0x3301, //
	0x3402, //
	0x36C8, //
	0x3848, //
	0x3A22, //

	0x0301,      //////////////////////////////////  Bank-B
	0x19C3, //
	0x2603, //
	0xC004, //
	0xC15F, //
	0xC200, //
	0xC300, //
	0xC440, //
	
	0x0300,                     // Bank-A 
	0x28F0,
	0x29F8, // pad_control3 (v/h/d pad enable)
#endif
	DATA_END_FLAG, 
};

static const unsigned short ps1210k_ntsc_lvds_attr[] = {			// cs
#if defined(SUPPORT_PS1210K)
	/************** PLL config ***************/	
	0x0300,                     // Bank-A
	0x4010,	// #pll_control			// bypass mode????
	0x2509,	// # clkdiv1
	0x268B,	// # clkdiv2
	0x4121, // # pll_m_cnt
	0x4204, // # pll_n_cnt
	0xFF32, 
	0x4000, // #pll_control
	/************** PLL config ***************/	

	0x0300,      //////////////////////////////////  Bank-A
	0x3301, //
	0x3402, //
	0x36C8, //
	0x3848, //
	0x3A22, //

	0x0301,      //////////////////////////////////  Bank-B
//	0x1645,	// frmvar_en : 1
	0x19C3, //
	0x2603, //
	0xC004, //
	0xC14F, //
	0xC200, //
	0xC300, //
	0xC440, //

	0x0303,      //////////////////////////////////  Bank-D
	0x0504, //
	0x0604, //
	0x0700, //
	0x0800, //
	0x1303, //
	
	/************** LVDS config **************/	
	0x0303,                     // Bank-D
	0x0402,	// # LVDS off, power down
	0xFF0F,
	0x0442,	// # LVDS on
	/************** LVDS config **************/	
#endif	
	DATA_END_FLAG,
};

/* PS3210M */	
static const unsigned short ps3210m_ntsc_bayer_attr[] = {
#if defined(SUPPORT_PS3210M)
	/************** PLL config ***************/	
	0x0300,                     // Bank-A 
	0x4010, // pll_control  off
	0x2569, // clkdiv1
	0x268B, // clkdiv2
	0x4121, // pll_m_cnt (21)
	0x4204, // pll_r_cnt (04)
	0xFF0A, // delay 10ms                
	0x4000, // pll_control  on
	/************** PLL config ***************/	

	0x0300,      //////////////////////////////////  Bank-A
	0x3301, //
	0x3402, //

	0x0301,      //////////////////////////////////  Bank-B
	0x19C3, //
	0x2603, //
	0xC004, //
	0xC14F, //
	0xC200, //
	0xC300, //
	0xC440, //
	
	0x0300,                     // Bank-A 
	0x2998, // pad_control3 (v/h/d pad enable)
#endif	
	DATA_END_FLAG, 
};

static const unsigned short ps3210m_ntsc_lvds_attr[] = {
#if defined(SUPPORT_PS3210M)
	/************** PLL config ***************/	
	0x0300,                     // Bank-A
	0x4010, // pll_control off
	0x2569, // clkdiv1
	0x268B, // clkdiv2
	0x4121, // # pll_m_cnt
	0x4204, // # pll_n_cnt
	0xFF0A, // delay 10ms                
	0x4000, // pll_control on
	/************** PLL config ***************/	
	
	0x0301,      //////////////////////////////////  Bank-B
	0x1604, //
	0x1E0E, //
	0x20CB, //
	0x3600, //
	0x37A2, //
	0x3808, //
	0x3932, //
	0x3E00, //
	0x3FA2, //
	0x4008, //
	0x4132, //
	0xA10B, //
	0xA2A2, //
	0xB730, //
	0xF601, //
	0xF701, //
	0xF801, //
	0xF901, //
	0xFA20, //
	0xFB2E, //
	0xFC2E, //
	0xFD3F, //
	0xFE00, //

	0x0303,      //////////////////////////////////  Bank-D
	0x0504, //
	0x0604, //
	0x0700, //
	0x0800, //
	0x1303, //
	
	/************** LVDS config **************/	
	0x0303,                     // Bank-D
	0x040E, // LVDS off
	0x0303,                     // Bank-D
	0x044E, // LVDS on
	/************** LVDS config **************/	
#endif	
	DATA_END_FLAG, 
};

/* PS3210P */	

static const unsigned short ps3210p_ntsc_bayer_attr[] = {
#if defined(SUPPORT_PS3210P)
	/************** PLL config ***************/	
	0x0300,                     // Bank-A //
	0x4010, // # pll_control off
	0x2549, // # clkdiv1
	0x26C0, // # clkdiv2
	0x4121, // # pll_m_cnt (21)
	0x4204, // # pll_r_cnt (04)
	0xFF0A,
	0x4000, // # pll_control on
	/************** PLL config ***************/	

	0x0300,      //////////////////////////////////  Bank-A
	0x2842, //
	0xC500, //
	0xC612, //
	0xC700, //
	0xC8A9, //
	0xC908, //
	0xCA70, //
	0xCB08, //
	0xCC5F, //
	0xCD08, //
	0xCE89, //
	0xCF08, //
	0xD07F, //
	0xD100, // // 1 overwrited
	0xD200, // // 1 overwrited
	0xD308, // // 1 overwrited
	0xD4FC, // // 1 overwrited

	0x0301,      //////////////////////////////////  Bank-B
	0x1843, //
	0x3600, //
	0x37FA, //
	0x3808, //
	0x398A, //
	0x3E00, //
	0x3FFA, //
	0x4008, //
	0x418A, //
	0x4200, //
	0x4396, //
	0x4400, //
	0x45F6, //
	0x5300, //
	0x5401, //
	0x5508, //
	0x5656, //
	0x5708, //
	0x5887, //
	0x5908, //
	0x5A97, //
	0x5B00, //
	0x5C0D, //
	0x5D08, //
	0x5E48, //
	0x5F00, //
	0x6012, //
	0x6100, //
	0x6295, //
	0x6700, //
	0x68AF, //
	0x6900, //
	0x6AE1, //
	0x6F00, //
	0x70EE, //
	0x7107, //
	0x72D2, //
	0x7300, //
	0x74F0, //
	0x7507, //
	0x76D0, //
	0x7708, //
	0x7894, //
	0x7908, //
	0x7A96, //
	0x7B00, //
	0x7C00, //
	0x7D08, //
	0x7E98, //
	0x7F08, //
	0x805C, //
	0x8108, //
	0x828F, //
	0x8308, //
	0x845C, //
	0x8508, //
	0x868F, //
	0x8708, //
	0x888C, //
	0x8908, //
	0x8A92, //
	0x8B01, //
	0x8C07, //
	0x8D08, //
	0x8E3E, //
	0x8F00, //
	0x90F3, //
	0x9108, //
	0x925C, //
	0x9308, //
	0x9497, //
	0x9508, //
	0x965C, //
	0x9708, //
	0x988F, //
	0x9900, //
	0x9AAF, //
	0x9B08, //
	0x9C48, //
	0xA10B, // // 1 overwrited
	0xA244, // // 1 overwrited
	0xB100, //
	0xB29B, //
	0xB400, //
	0xB908, //
	0xBA5C, //
	0xBB08, //
	0xBC8F, //
	0xC004, //
	0xC15E, //
	0xC200, //
	0xC360, //
	
	0x0300,                     // Bank-A 
	0x2998, // pad_control3 (v/h/d pad enable)
//	0x0300,                     // Bank-A 
//	0x28F0,
//	0x29F8, // pad_control3 (v/h/d pad enable)
#endif
	DATA_END_FLAG, 
};

/* PS3210K */
static const unsigned short ps3210k_ntsc_bayer_attr[] = {		// cs
#if defined(SUPPORT_PS3210K)
	/* ps3210k_bayer_parallel_cdsBias_Rev1.1_160725.ccf file */	

	/************** PLL config ***************/	
	0x0300,                     // Bank-A
	0x4010, // pll_control off
	0x2500, // clkdiv1 (49)
	0x26C0, // clkdiv2
	0x410B, // pll_m_cnt (21)
	0x4204, // pll_r_cnt (04)
	0xFF10,
	0x4000, // pll_control on
	/************** PLL config ***************/	

	0x0300,      //////////////////////////////////  Bank-A
	0x2801, // 
	0x3102, // 
	0x3305, // 
	0x3408, // 
	0x36A2, // 
	0x394B, // 
	0x6F01, // 
	0x7101, // 
	0x721A, // 
	0x7401, // 
	0x7501, // 
	0x7604, // 
	0x7704, // 
	0x7801, // 
	0x7B01, // 
	0x7C01, // 
	0x7E01, // 
	0x7F03, // 
	0x8001, // 
	0xC908, // 
	0xCA6A, // 
	0xCB08, // 
	0xCC64, // 
	0xCD08, // 
	0xCE92, // 
	0xCF08, // 
	0xD096, // 
	0xD108, // 
	0xD294, // 
	0xD308, // 
	0xD496, // 
	0xD555, // 
	0xD655, // 
	0xD702, // 
	0xD852, // 
	0xD907, // 
	0xDA54, // 
	0xDB00, // 
	0xDCFB, // 
	0xDD08, // 
	0xDE63, // 

	0x0301,      //////////////////  Bank-B
	0x04E9, // 
	0x0621, // 
	0x090A, // 
	0x1644, // 
	0x20C3, // 
	0x3600, // 
	0x37FA, // 
	0x3808, // 
	0x398A, // 
	0x3E00, // 
	0x3FFA, // 
	0x4008, // 
	0x418A, // 
	0x5300, // 
	0x5411, // 
	0x5508, // 
	0x5631, // 
	0x5700, // 
	0x580D, // 
	0x5908, // 
	0x5A73, // 
	0x5B00, // 
	0x5C14, // 
	0x5D08, // 
	0x5E48, // 
	0x5F00, // 
	0x6029, // 
	0x6100, // 
	0x62F6, // 
	0x6700, // 
	0x68FD, // 
	0x6901, // 
	0x6A10, // 
	0x6F01, // 
	0x70D2, // 
	0x7107, // 
	0x7257, // 
	0x7301, // 
	0x74D6, // 
	0x7507, // 
	0x7652, // 
	0x7708, // 
	0x7894, // 
	0x7908, // 
	0x7A96, // 
	0x7F08, // 
	0x8084, // 
	0x8108, // 
	0x828B, // 
	0x8308, // 
	0x8484, // 
	0x8508, // 
	0x868B, // 
	0x8708, // 
	0x888C, // 
	0x8908, // 
	0x8A92, // 
	0x8B02, // 
	0x8C4F, // 
	0x8D07, // 
	0x8E75, // 
	0x8F01, // 
	0x90C2, // 
	0x9108, // 
	0x9262, // 
	0x9308, // 
	0x9473, // 
	0x9508, // 
	0x965B, // 
	0x9708, // 
	0x9880, // 
	0x9900, // 
	0x9AFD, // 
	0x9B08, // 
	0x9C48, // 
	0xA10B, // // 1 overwrited 
	0xA244, // // 1 overwrited 
	0xB100, // 
	0xB2FA, // 
	0xB760, // 
	0xB908, // 
	0xBA84, // 
	0xBB08, // 
	0xBC8B, // 
	0xC004, // 
	0xC15F, // 
	0xC200, // 
	0xC360, // 
	0xC440, // 

	0x0302,      //////////////////  Bank-C
	0x3314, // 
	
//	0x0300,                     // Bank-A 
//	0x2998, // pad_control3 (v/h/d pad enable)
	0x0300,                     // Bank-A 
	0x28F0,
	0x29F8, // pad_control3 (v/h/d pad enable)
#endif
	DATA_END_FLAG, 
};

static const unsigned short ps3210k_ntsc_lvds_attr[] = {			// cs
#if defined(SUPPORT_PS3210K)
	//// cds(cds_gs_rev0.8적용) ps3210k_bayer_parallel_30fps_cdsBias_Rev0.9_ablc_160721_test.ccf 적용
	//// ABLC 160706//////////////////////
	
	/************** PLL config ***************/	
	0x0300,                     // Bank-A
	0x4050, // pll_control1 
	0x2501, // clkdiv1      
	0x2685, // clkdiv2      
	0x4208, // pll_r_cnt    
	0x4121, // pll_m_cnt    
	0xFFFE, // delay 300msec
	0x4040, // pll_control1
	/************** PLL config ***************/	

	0x0300,      //////////////////  Bank-A
	0x0500, // 
	0x2801, // 
	0x3102, // 
	0x3305, // 
	0x3408, // 
	0x3640, // // 1 overwrited 
	0x394B, // 
	0x3B93, // 
	0x6F01, // 
	0x7101, // 
	0x721A, // 
	0x7401, // 
	0x7501, // 
	0x7604, // 
	0x7704, // 
	0x7801, // 
	0x7B01, // 
	0x7C01, // 
	0x7E01, // 
	0x7F03, // 
	0x8001, // 
	0xC908, // 
	0xCA6A, // 
	0xCB08, // 
	0xCC64, // 
	0xCD08, // 
	0xCE92, // 
	0xCF08, // 
	0xD096, // 
	0xD108, // 
	0xD294, // 
	0xD308, // 
	0xD496, // 
	0xD555, // 
	0xD655, // 
	0xD701, // 
	0xD88A, // 
	0xD906, // 
	0xDA8C, // 
	0xDB00, // 
	0xDCFB, // 
	0xDD08, // 
	0xDE63, // 
	0xF500, // 
	0xF603, // 
	0xF709, // 
	0xF81C, // 
	0xF958, // 
	0xFA00, // 
	0xFB28, // 
	0xFC29, // 
	0xFD30, // 
	0xFE36, // 

	0x0301,      //////////////////  Bank-B
	0x04E9, // 
	0x0621, // 
	0x090A, // 
	0x1F41, // 
	0x20C3, // // 1 overwrited 
	0x3600, // 
	0x37FA, // 
	0x3808, // 
	0x398A, // 
	0x3E00, // 
	0x3FFA, // 
	0x4008, // 
	0x418A, // 
	0x5300, // 
	0x5406, // 
	0x5508, // 
	0x5631, // 
	0x5700, // 
	0x580A, // 
	0x5908, // 
	0x5A73, // 
	0x5B00, // 
	0x5C12, // 
	0x5D08, // 
	0x5E48, // 
	0x5F00, // 
	0x6026, // 
	0x6100, // 
	0x62F6, // 
	0x6700, // 
	0x68FD, // 
	0x6901, // 
	0x6A10, // 
	0x6F01, // 
	0x700A, // 
	0x7106, // 
	0x728F, // 
	0x7301, // 
	0x740E, // 
	0x7506, // 
	0x768A, // 
	0x7708, // 
	0x7894, // 
	0x7908, // 
	0x7A96, // 
	0x7F08, // 
	0x8084, // 
	0x8108, // 
	0x828B, // 
	0x8308, // 
	0x8484, // 
	0x8508, // 
	0x868B, // 
	0x8708, // 
	0x888C, // 
	0x8908, // 
	0x8A92, // 
	0x8B01, // 
	0x8C87, // 
	0x8D06, // 
	0x8EAD, // 
	0x8F00, // 
	0x90FA, // 
	0x9108, // 
	0x9262, // 
	0x9308, // 
	0x9473, // 
	0x9508, // 
	0x965B, // 
	0x9708, // 
	0x9880, // 
	0x9900, // 
	0x9AFD, // 
	0x9B08, // 
	0x9C48, // 
	0xA10B, // // 1 overwrited 
	0xA244, // // 1 overwrited 
	0xA3FA, // 
	0xA470, // 
	0xA800, // 
	0xB100, // 
	0xB2FA, // 
	0xB760, // 
	0xB908, // 
	0xBA84, // 
	0xBB08, // 
	0xBC8B, // 

	0x0302,      //////////////////  Bank-C
	0x3314, // 

	/************** LVDS config **************/	
	0x0303,                     // Bank-D 
	0x2900, // mipi_read_th_h    
	0x2A0A, // mipi_read_th_l    
	0x0440, // mipi_control_0    
	0x0508, // mipi_control_1    
	0x0600, // mipi_control_2    
	0x0700, // mipi_control_3    
	0x083D, // mipi_control_4    
	0x0410, // mipi_control0: mipi_en
	0x0440, // mipi_control0: mipi_en
	/************** LVDS config **************/	
	
#if 0
	/* test for frmvar_en mode */	
	0x0301,
	0x1644,
	0xC00D,
	0xC11D,
	0xC200,
#endif

#endif

	DATA_END_FLAG,
};

/* PS4100K */
static const unsigned short ps4100k_ntsc_bayer_attr[] = {		// cs
#if defined(SUPPORT_PS4100K)	

	/************** PLL config ***************/	
	0x0300,                     // Bank-A
	0x4010, // pll_control
	0x2512, // clkdiv1			// ppclk: mclk/2 	= 37.125Mhz, 	mclk: vco/3 = 74.25Mhz
	0x26CB, // clkdiv2			// pclk: ppclk 		= 37.125Mhz
	0x4121, // pll_m_cnt (21)
	0x4204, // pll_r_cnt (04)
	0xFFFE,
//	0xFFFE,
	0x4000, // pll_control on
	/************** PLL config ***************/	

	0x0300,      //////////////////  Bank-A
	0x3303, // 
	0x3403, // 
	0x3620, // 
	0x385F, // 

	0x0301,      //////////////////  Bank-B
	0x1645, // 
	0x2603, // 
	0x3600, // 
	0x37B8, // 
	0x3805, // 
	0x39C8, // 
	0x5105, // 
	0x521E, // 
	0x5300, // 
	0x5402, // 
	0x5506, // 
	0x5636, // 
	0x5706, // 
	0x5836, // 
	0x5906, // 
	0x5A5E, // 
	0x5D06, // 
	0x5E2C, // 
	0x6100, // 
	0x6278, // 
	0x6700, // 
	0x6894, // 
	0x6900, // 
	0x6AA8, // 
	0x7106, // 
	0x722A, // 
	0x7506, // 
	0x7628, // 
	0x7706, // 
	0x7854, // 
	0x7906, // 
	0x7A59, // 
	0x7D06, // 
	0x7E26, // 
	0x7F06, // 
	0x8054, // 
	0x8106, // 
	0x8259, // 
	0x8506, // 
	0x8624, // 
	0x8706, // 
	0x8854, // 
	0x8906, // 
	0x8A59, // 
	0x8D06, // 
	0x8E22, // 
	0x8F06, // 
	0x9054, // 
	0x9106, // 
	0x9259, // 
	0x9506, // 
	0x962C, // 
	0x9706, // 
	0x9840, // 
	0x9906, // 
	0x9A68, // 
	0x9B06, // 
	0x9C40, // 
	0x9D06, // 
	0x9E68, // 
	0x9F06, // 
	0xA03B, // 
	0xA106, // 
	0xA23D, // 
	0xA301, // 
	0xA422, // 
	0xA506, // 
	0xA640, // 
	0xA700, // 
	0xA88C, // 
	0xA906, // 
	0xAA40, // 
	0xAB06, // 
	0xAC71, // 
	0xAD06, // 
	0xAE40, // 
	0xAF06, // 
	0xB052, // 
	0xB306, // 
	0xB436, // 
	0xB904, // 
	0xBAC8, // 
	0xBB06, // 
	0xBC40, // 
	0xBD06, // 
	0xBE68, // 
	0xC430, // 
	0xC806, // 
	0xC940, // 
	0xCA06, // 
	0xCB68, // 
	0xCF02, // 
	0xD0E8, // 
	0xD100, // 
	0xD200, // 
	0xD340, // 
	
	0x0300,                     // Bank-A 
	0x28F0,
	0x29F8, // pad_control3 (v/h/d pad enable)
#endif	
	DATA_END_FLAG, 
};

static const unsigned short ps4100k_ntsc_lvds_attr[] = {			// cs
	/* CIF LVDS RX not support 720P */
	DATA_END_FLAG,
};


/* PS7100K */
static const unsigned short ps7100k_ntsc_bayer_attr[] = {		// cs
#if defined(SUPPORT_PS7100K)	
	0x0300,                     // Bank-A
	0x2514, // clkdiv1 (14)

	0x4110, // pll_m_cnt (13)
	0x4204, // pll_r_cnt (04)

	0x4050, // pll_control
	0xFF10,
	0x4040, // pll_control on
	
	//////////////////////// BANK B
	0x0301,

	0x1644, // bayer_control_01 (frmvar_en,led_dsel)
	0x1E0E, // bayer_control_09 (bsmode)

	0xB730, // adcoffset

	0x0301,
	0xC002, // max=02
	0xC1E8, // max=E8
	0xC200,
	0xC360,

	////////////////////BIAS//////////////////////
	0x0300,
	0x32B5, // rampbuf bias=1011b 
	0x3306, // pixelbias
	0x3408, // compbias
	0x36B6, // pcp ldo 

	0x376B, // blacksun 0Bh(170425 DEP Tr)
	0x3A6A, // Internal DVDD 1.48V

	0x3D35, // range_sel=110101b 1.05V
	0x3C40, // rampcap_sel
	0x385A, // sample drv

	////////////////////LimiterFitting//////////////////////
	0x0301,
	0x20AB, // BayerCon11<3>=lim Fit en 0-->1, default=A3h 
	0xF601,	// trim0 x1~x2 00~10 
	0xF70A,	// trim1 x2~x4 10~20
	0xF812,	// trim2 x4~x8 20~30
	0xF916,	// trim3 x8~x16 30~40
	0xFA16,	// trim4 x16~x32 40~50
	0xFB16,	// trim5 x32~x64 50~60
	0xFC16,	// trim6 X64~     60

	////////////////////////cds////////////////////
	0x0301,	// B bank

	0x5706,	// lsenb edge1 H (1650)
	0x5872,	// lsenb edge1 L
	0x5906,	// lsenb edge2 H (1648)
	0x5A70,	// lsenb edge2 L

	0x5300,	// ls_rst edge1 H (22)
	0x5416,	// ls_rst edge1 L
	0x5507,	// ls_rst edge2 H (1860)
	0x5644,	// ls_rst edge2 L

	0x6701,	// ls_txb edge1 H (260)
	0x6804,	// ls_txb edge1 L
	0x6901,	// ls_txb edge2 H (275)
	0x6A13,	// ls_txb edge2 L

	0x5B00,	// sample edge1 H (164)
	0x5CA4,	// sample edge1 L
	0x5D01,	// sample edge2 H (257)
	0x5E01,	// sample edge2 L


	0x0300,	// A bank

	0xD201,	// sample2 edge1 H (424)
	0xD3A8,	// sample2 edge1 L
	0xD407,	// sample2 edge2 H (1855)
	0xD53F,	// sample2 edge2 L

	0xCD01,	// balck_en edge1 H (256)
	0xCE00,	// balck_en edge1 L
	0xCF07,	// balck_en edge2 H (1855)
	0xD03F,	// balck_en edge2 L


	0x0301,	// B bank

	0x5F00,	// store edge1 H (0)
	0x6000,	// store edge1 L
	0x6100,	// store edge2 H (168)
	0x62A8,	// store edge2 L


	0x0300,	// A bank

	0xD600,	// store2 edge1 H (5)
	0xD705,	// store2 edge1 L
	0xD800,	// store2 edge2 H (250)
	0xD9FA,	// store2 edge2 L


	0x0301,	// B bank

	0x9901,	// tx_illum edge1 H (264)
	0x9A08,	// tx_illum edge1 L
	0x9B06,	// tx_illum edge2 H (1751)
	0x9CD7,	// tx_illum edge2 L

	0x6F01,	// load edge1 H (430)
	0x70AE,	// load edge1 L
	0x7106,	// load edge2 H (1738)
	0x72CA,	// load edge2 L

	0x7301,	// latch_en edge1 H (435)
	0x74B3,	// latch_en edge1 L
	0x7506,	// latch_en edge2 H (1736)
	0x76C8,	// latch_en edge2 L
	0x7709,	// latch_en edge3 H (2375)
	0x7847,	// latch_en edge3 L
	0x7909,	// latch_en edge4 H (2385)
	0x7A51,	// latch_en edge4 L

	0x8F01,	// vramp edge1 H (418)
	0x90A2,	// vramp edge1 L
	0x8B01,	// vramp edge2 H (442)
	0x8CBA,	// vramp edge2 L
	0x8D06,	// vramp edge4 H (1746)
	0x8ED2,	// vramp edge4 L

	0x8709,	// transfer edge1 H (2360)
	0x8838,	// transfer edge1 L
	0x8909,	// transfer edge2 H (2370)
	0x8A42,	// transfer edge2 L

	0x9509,	// refhold edge1 H (2380)
	0x964C,	// refhold edge1 L
	0x9709,	// refhold edge2 H (2391)
	0x9857,	// refhold edge2 L

	0x9109,	// atten_rst edge1 H (2353)
	0x9231,	// atten_rst edge1 L
	0x9309,	// atten_rst edge2 H (2370)
	0x9442,	// atten_rst edge2 L

	0x7F09,	// pbhold edge1 H (2380)
	0x804C,	// pbhold edge1 L
	0x8109,	// pbhold edge2 H (2391)
	0x8257,	// pbhold edge2 L

	0x8309,	// col_pbhold edge1 H (2380)
	0x844C,	// col_pbhold edge1 L
	0x8509,	// col_pbhold edge2 H (2391)
	0x8657,	// col_pbhold edge2 L

	0xB909,	// col_pbhold2 edge1 H (2380)
	0xBA4C,	// col_pbhold2 edge1 L
	0xBB09,	// col_pbhold2 edge2 H (2391)
	0xBC57,	// col_pbhold2 edge2 L

	0x3604,	// tg_hsync edge1 H (1050)
	0x371A,	// tg_hsync edge1 L
	0x3809,	// tg_hsync edge2 H (2346)
	0x392A,	// tg_hsync edge2 L

	0xA109,	// tg_hsync COffset H (2408)
	0xA268,	// tg_hsync COffset L

	0x3E04,	// bl_hsync edge1 H (1075)
	0x3F33,	// bl_hsync edge1 L
	0x4009,	// bl_hsync edge2 H (2324)
	0x4114,	// bl_hsync edge2 L

	//////////////////////////////////////////////////////////////////////////
	//ablc 1.0 control
	0x0300,
	0x36B4,
	0x3B8C,

	0x0301,
	0x1F51,

	0x0301,
	0xA3D0,
	0xA470,
	0xA800,

	//ablc 2.0 control
	0x0301,
	0x20AB,

	0x0300,
	0xF300,
	0xF403,
	0xF510,
	0xF62A,
	0xF762,

	0x0300,
	0xF810,
	0xF912,
	0xFA16,
	0xFB20,
	0xFC2C,
	
	
	0x0302, 
	0x9B20, 

#endif	
	DATA_END_FLAG, 
};

static const unsigned short ps7100k_ntsc_lvds_attr[] = {			// cs
	/* CIF LVDS RX not support 720P */
	DATA_END_FLAG,
};


/* 720x480i */
static struct sensor_ctrl_info init_sensor_values[] = {
	/* PC3089N */
	[SNS_PC3089N] = {
		.chip_id = 0x3089,
		.name = "pc3089n",
		.chip_addr = 0x33,
		.use_16bit_addr = 0,
		.use_16bit_data = 0,
#if defined(SUPPORT_PC3089N)		
		.scan_supported = (SNS_SUPPORT_NTSC),
#else
		.scan_supported = 0,
#endif
		.scan_mode = {
			[SNS_SCAN_NTSC] = {
				.intf_supported = (SNS_SUPPORT_BT601),
				.intf_mode = 
				{
					0, 0, 0, 0, pc3089n_ntsc_bt601_attr,
				},
				.frm_attr = 0,
				.bsq_attr = 0,
				.flp_attr = 0,
				.clk_attr = 0,
			},
			[SNS_SCAN_PAL] = {
				.intf_supported = 0,
				.intf_mode = {
					0, 0, 0, 0, 0,
				},
				.frm_attr = 0,
				.bsq_attr = 0,
				.flp_attr = 0,
				.clk_attr = 0,
			},
		},
		.set_image_attr = 0,
		.set_gain_attr = 0,
	},
	/* PC7030K */
	[SNS_PC7030K] = {
		.chip_id = 0x7030,
		.name = "pc7030k",
		.chip_addr = 0x32,
		.use_16bit_addr = 0,
		.use_16bit_data = 0,
#if defined(SUPPORT_PC7030K)		
		.scan_supported = (SNS_SUPPORT_NTSC|SNS_SCAN_PAL),
#else
		.scan_supported = 0,
#endif
		.scan_mode = {
			[SNS_SCAN_NTSC] = {
				.intf_supported = (SNS_SUPPORT_BT656),
				.intf_mode = 
				{
					0, 0, 0, pc7030k_ntsc_bt656_attr, 0,
				},
				.frm_attr = 0,
				.bsq_attr = 0,
				.flp_attr = 0,
				.clk_attr = 0,
			},
			[SNS_SCAN_PAL] = {
				.intf_supported = (SNS_SUPPORT_BT656),
				.intf_mode = {
					0, 0, 0, pc7030k_pal_bt656_attr, 0,
				},
				.frm_attr = 0,
				.bsq_attr = 0,
				.flp_attr = 0,
				.clk_attr = 0,
			},
		},
		.set_image_attr = 0,
		.set_gain_attr = 0,
	},
	/* PO3100K */
	[SNS_PO3100K] = {
		.chip_id = 0x3100,
		.name = "po3100k",
		.chip_addr = 0x77,
		.use_16bit_addr = 0,
		.use_16bit_data = 0,
#if defined(SUPPORT_PO3100K)		
		.scan_supported = SNS_SUPPORT_NTSC,
#else
		.scan_supported = 0,
#endif
		.scan_mode = {
			[SNS_SCAN_NTSC] = {
				.intf_supported = (SNS_SUPPORT_BT1120),
				.intf_mode = 
				{
					0, 0, po3100k_ntsc_bt1120_attr, 0, 0,
				},
				.frm_attr = 0,
				.bsq_attr = 0,
				.flp_attr = 0,
				.clk_attr = 0,
			},
			[SNS_SCAN_PAL] = {
				.intf_supported = 0,
				.intf_mode = {
					0, 0, 0, 0, 0,
				},
				.frm_attr = 0,
				.bsq_attr = 0,
				.flp_attr = 0,
				.clk_attr = 0,
			},
		},
		.set_image_attr = 0,
		.set_gain_attr = 0,
	},
	/* PS1210K */
	[SNS_PS1210K] = {
		.chip_id = 0x1210,
		.name = "ps1210k",
		.chip_addr = 0x77,
		.use_16bit_addr = 0,
		.use_16bit_data = 0,
#if defined(SUPPORT_PS1210K)		
		.scan_supported = (SNS_SUPPORT_NTSC),
#else
		.scan_supported = 0,
#endif
		.scan_mode = {
			[SNS_SCAN_NTSC] = {
				.intf_supported = (SNS_SUPPORT_BAYER | SNS_SUPPORT_LVDS),
				.intf_mode = 
				{
					ps1210k_ntsc_bayer_attr, ps1210k_ntsc_lvds_attr, 0, 0, 0,
				},
				.frm_attr = frm_attr_1080,
				.bsq_attr = bsq_attr_gbrg,
				.flp_attr = flp_attr_all,
				.clk_attr = 0,
			},
			[SNS_SCAN_PAL] = {
				.intf_supported = 0,
				.intf_mode = {
					0, 0, 0, 0, 0,
				},
				.frm_attr = 0,
				.bsq_attr = 0,
				.flp_attr = 0,
				.clk_attr = 0,
			},
		},
		.set_image_attr = 0,
		.set_gain_attr = 0,
	},
	/* PS1210M */
	[SNS_PS1210M] = {
		.chip_id = 0x1210,
		.name = "ps1210m",
		.chip_addr = 0x77,
		.use_16bit_addr = 0,
		.use_16bit_data = 0,
#if defined(SUPPORT_PS1210M)		
		.scan_supported = (SNS_SUPPORT_NTSC),
#else
		.scan_supported = 0,
#endif
		.scan_mode = {
			[SNS_SCAN_NTSC] = {
				.intf_supported = 0,
				.intf_mode = 
				{
					0, 0, 0, 0, 0,
				},
				.frm_attr = 0,
				.bsq_attr = 0,
				.flp_attr = 0,
				.clk_attr = 0,
			},
			[SNS_SCAN_PAL] = {
				.intf_supported = 0,
				.intf_mode = {
					0, 0, 0, 0, 0,
				},
				.frm_attr = 0,
				.bsq_attr = 0,
				.flp_attr = 0,
				.clk_attr = 0,
			},
		},
		.set_image_attr = 0,
		.set_gain_attr = 0,
	},
	/* PS3210M */
	[SNS_PS3210M] = {
		//	.chip_id = 0x3210,
		.chip_id = 0x3120,
		.name = "ps3210m",
		.chip_addr = 0x77,
		.use_16bit_addr = 0,
		.use_16bit_data = 0,
#if defined(SUPPORT_PS3210M)		
		.scan_supported = (SNS_SUPPORT_NTSC),
#else
		.scan_supported = 0,
#endif
		.scan_mode = {
			[SNS_SCAN_NTSC] = {
				.intf_supported = (SNS_SUPPORT_BAYER | SNS_SUPPORT_LVDS),
				.intf_mode = 
				{
					ps3210m_ntsc_bayer_attr, ps3210m_ntsc_lvds_attr, 0, 0, 0,
				},
				.frm_attr = frm_attr_1080,
				.bsq_attr = bsq_attr_grbg,
				.flp_attr = flp_attr_all,
				.clk_attr = clk_attr_hallline,
			},
			[SNS_SCAN_PAL] = {
				.intf_supported = 0,
				.intf_mode = {
					0, 0, 0, 0, 0,
				},
				.frm_attr = 0,
				.bsq_attr = 0,
				.flp_attr = 0,
				.clk_attr = 0,
			},
		},
		.set_image_attr = 0,
		.set_gain_attr = 0,
	},
	/* PS3210P */	
	[SNS_PS3210P] = {
		.chip_id = 0x3210,
		.name = "ps3210p",
		.chip_addr = 0x77,
		.use_16bit_addr = 0,
		.use_16bit_data = 0,
#if defined(SUPPORT_PS3210P)		
		.scan_supported = (SNS_SUPPORT_NTSC),
#else
		.scan_supported = 0,
#endif
		.scan_mode = {
			[SNS_SCAN_NTSC] = {
				.intf_supported = (SNS_SUPPORT_BAYER),
				.intf_mode = 
				{
					ps3210p_ntsc_bayer_attr, 0, 0, 0, 0,
				},
				.frm_attr = frm_attr_1080,
				.bsq_attr = bsq_attr_grbg,
				.flp_attr = flp_attr_all,
				.clk_attr = clk_attr_hallline,
			},
			[SNS_SCAN_PAL] = {
				.intf_supported = 0,
				.intf_mode = {
					0, 0, 0, 0, 0,
				},
				.frm_attr = 0,
				.bsq_attr = 0,
				.flp_attr = 0,
				.clk_attr = 0,
			},
		},
		.set_image_attr = 0,
		.set_gain_attr = 0,
	},
	/* PV1219M */	
	[SNS_PV1219M] = {
		.chip_id = 0x1219,
		.name = "pv1219m",
		.chip_addr = 0x76,
		.use_16bit_addr = 0,
		.use_16bit_data = 0,
#if defined(SUPPORT_PV1219M)		
		.scan_supported = (SNS_SUPPORT_NTSC),
#else
		.scan_supported = 0,
#endif
		.scan_mode = {
			[SNS_SCAN_NTSC] = {
				.intf_supported = 0,
				.intf_mode = 
				{
					0, 0, 0, 0, 0,
				},
				.frm_attr = 0,
				.bsq_attr = 0,
				.flp_attr = 0,
				.clk_attr = 0,
			},
			[SNS_SCAN_PAL] = {
				.intf_supported = 0,
				.intf_mode = {
					0, 0, 0, 0, 0,
				},
				.frm_attr = 0,
				.bsq_attr = 0,
				.flp_attr = 0,
				.clk_attr = 0,
			},
		},
		.set_image_attr = 0,
		.set_gain_attr = 0,
	},
	/* PV2109M */
	[SNS_PV2109M] = {
		.chip_id = 0x2109,
		.name = "pv2109m",
		.chip_addr = 0x32,
		.use_16bit_addr = 0,
		.use_16bit_data = 0,
#if defined(SUPPORT_PV2109M)		
		.scan_supported = (SNS_SUPPORT_NTSC),
#else
		.scan_supported = 0,
#endif
		.scan_mode = {
			[SNS_SCAN_NTSC] = {
				.intf_supported = 0,
				.intf_mode = 
				{
					0, 0, 0, 0, 0,
				},
				.frm_attr = 0,
				.bsq_attr = 0,
				.flp_attr = 0,
				.clk_attr = 0,
			},
			[SNS_SCAN_PAL] = {
				.intf_supported = 0,
				.intf_mode = {
					0, 0, 0, 0, 0,
				},
				.frm_attr = 0,
				.bsq_attr = 0,
				.flp_attr = 0,
				.clk_attr = 0,
			},
		},
		.set_image_attr = 0,
		.set_gain_attr = 0,
	},
	/* PS3210K */	
	[SNS_PS3210K] = {
		.chip_id = 0x3210,
		.name = "ps3210k",
		.chip_addr = 0x77,
		.use_16bit_addr = 0,
		.use_16bit_data = 0,
#if defined(SUPPORT_PS3210K)		
		.scan_supported = (SNS_SUPPORT_NTSC),
#else
		.scan_supported = 0,
#endif
		.scan_mode = {
			[SNS_SCAN_NTSC] = {
				.intf_supported = (SNS_SUPPORT_BAYER|SNS_SUPPORT_LVDS),
				.intf_mode = 
				{
					ps3210k_ntsc_bayer_attr, ps3210k_ntsc_lvds_attr, 0, 0, 0,
				},
				.frm_attr = frm_attr_1080,
				.bsq_attr = bsq_attr_gbrg,
				.flp_attr = flp_attr_all,
				.clk_attr = clk_attr_hallline,
			},
			[SNS_SCAN_PAL] = {
				.intf_supported = 0,
				.intf_mode = {
					0, 0, 0, 0, 0,
				},
				.frm_attr = 0,
				.bsq_attr = 0,
				.flp_attr = 0,
				.clk_attr = 0,
			},
		},
		.set_image_attr = 0,
		.set_gain_attr = 0,
	},

	/* PS4100K */	
	[SNS_PS4100K] = {
		.chip_id = 0x4100,
		.name = "ps4100k",
		.chip_addr = 0x32,
		.use_16bit_addr = 0,
		.use_16bit_data = 0,
#if defined(SUPPORT_PS4100K)		
		.scan_supported = (SNS_SUPPORT_NTSC),
#else
		.scan_supported = 0,
#endif
		.scan_mode = {
			[SNS_SCAN_NTSC] = {
				/* 
				   PM1208K LVDS Rx not support other PCLK, except 74.25Mhz 
				 */
				.intf_supported = (SNS_SUPPORT_BAYER/*|SNS_SUPPORT_LVDS*/),		
				.intf_mode = 
				{
					ps4100k_ntsc_bayer_attr, ps4100k_ntsc_lvds_attr, 0, 0, 0,
				},
				.frm_attr = frm_attr_720,
				.bsq_attr = bsq_attr_bggr,
				.flp_attr = flp_attr_ver,
				.clk_attr = clk_attr_hallline,
			},
			[SNS_SCAN_PAL] = {
				.intf_supported = 0,
				.intf_mode = {
					0, 0, 0, 0, 0,
				},
				.frm_attr = 0,
				.bsq_attr = 0,
				.flp_attr = 0,
				.clk_attr = 0,
			},
		},
		.set_image_attr = 0,
		.set_gain_attr = 0,
	},
	
	/* PS7100K */	
	[SNS_PS7100K] = {
		.chip_id = 0x7100,
		.name = "ps7100k",
		.chip_addr = 0x32,
		.use_16bit_addr = 0,
		.use_16bit_data = 0,
#if defined(SUPPORT_PS7100K)		
		.scan_supported = (SNS_SUPPORT_NTSC),
#else
		.scan_supported = 0,
#endif
		.scan_mode = {
			[SNS_SCAN_NTSC] = {
				/* 
				   PM1208K LVDS Rx not support other PCLK, except 74.25Mhz 
				 */
				.intf_supported = (SNS_SUPPORT_BAYER/*|SNS_SUPPORT_LVDS*/),		
				.intf_mode = 
				{
					ps7100k_ntsc_bayer_attr, ps7100k_ntsc_lvds_attr, 0, 0, 0,
				},
				.frm_attr = frm_attr_720_var,
				.bsq_attr = bsq_attr_bggr,
				.flp_attr = flp_attr_all,
				.clk_attr = clk_attr_hallline,
			},
			[SNS_SCAN_PAL] = {
				.intf_supported = 0,
				.intf_mode = {
					0, 0, 0, 0, 0,
				},
				.frm_attr = 0,
				.bsq_attr = 0,
				.flp_attr = 0,
				.clk_attr = 0,
			},
		},
		.set_image_attr = 0,
		.set_gain_attr = 0,
	},
};


#endif	// SENSOR_CONFIGS_CS_H_
