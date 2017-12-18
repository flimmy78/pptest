#ifndef ISP_CONFIGS_CS_H_
#define ISP_CONFIGS_CS_H_ 1

#include <bd_sns_ctrl.h>

#define INVALID_CHIP_ID	0xFFFF
#define DATA_END_FLAG	0xFFFF


static const unsigned short frm_attr_1080[] = {
//	0x0302, // Bank-C
//	0x8B01,	// i2c_control_1	(not aev)
#if defined(SUPPORT_PS1210K) || defined(SUPPORT_PS1210M) || defined(SUPPORT_PS3210M) || defined(SUPPORT_PS3210P) || defined(SUPPORT_PV1219M) || defined(SUPPORT_PS3210K)	
	0x0300, // Bank-A
	0x22F0,	// i2c_control_1	(not aev)
#if defined(USE_SENSOR_25FPS)	
	0x060A,	// framewidth_h   
	0x074F,	// framewidth_l  	// 0xA4F	(2639)
#else                                            
	0x0608,	// framewidth_h   
	0x0797,	// framewidth_l  	// 0x897	(2199)
#endif	
	0x0804,	// fd_fheight_a_h 
	0x0964,	// fd_fheight_a_l 
	0x0a04,	// fd_fheight_b_h 
	0x0b64,	// fd_fheight_b_l 
	0x0c00,	// windowx1_h 
	0x0d05,	// windowx1_l 
	0x0e00,	// windowy1_h 
	0x0f05,	// windowy1_l 
	0x1007,	// windowx2_h 
	0x1184,	// windowx2_l 
	0x1204,	// windowy2_h 
	0x133C,	// windowy2_l 
	0x1400,	// vsyncstartrow_f0_h 
	0x151B,	// vsyncstartrow_f0_l 		// 0x01A -> 0x01B	(shift vsync 1 line)
	0x1604,	// vsyncstoprow_f0_h  
	0x1752,	// vsyncstoprow_f0_l  		// 0x451 -> 0x452
	0x1800,	// vsynccolume 
	0x1902,	// vsynccolume 

	0x4C04,	// fd_a_step_h
	0x4D60,	// fd_a_step_l
	0x4E03, // fd_b_step_h
	0x4FAB, // fd_b_step_l
	0x5040, // fd_winheight
	0x5100, // fd_a_delay
	0x5200, // fd_b_delay
	0x5340, // fd_th
	0x5400, // fd_period_a_h
	0x55EA, // fd_period_a_m
	0x5660, // fd_period_a_l
	0x5701, // fd_period_b_h
	0x5817, // fd_period_b_m
	0x5980,	// fd_period_b_l
	0x5A05, // fd_period_c_h
	0x5B7E, // fd_period_c_m

	0x0302,	// Bank-C
	// ae vsync start
	0xB004,	// (1649/2) = 0x338
	0xB14B,
	// ae full
	0xB300,	// x1 position
	0xB405,	
	0xB507,	// x2 position
	0xB684,
	0xB700,	// y1 position
	0xB805,
	0xB904,	// y2 position
	0xBA3C,
	// ae center		
	0xBB02,
	0xBC85,
	0xBD05,
	0xBE04,
	0xBF01,
	0xC06D,
	0xC102,
	0xC2D4,
	// ae x,y			
	0xC303,
	0xC4C5,
	0xC502,
	0xC621,
	// awb		
	0xC700,
	0xC805,
	0xC907,
	0xCA84,
	0xCB00,
	0xCC05,
	0xCD04,
	0xCE3C,

	// af center		
	0xD802,
	0xD985,
	0xDA05,
	0xDB04,
	0xDC01,
	0xDD6D,
	0xDE02,
	0xDFD4,
	
	0x0304,	// Bank-E
	0x1204,	// expfrmh_h
	0x1352,	// expfrmh_l
	0x1404,	// midfrmheight_h
	0x155E,	// midfrmheight_l
	0x1604,	// maxfrmheight_h
	0x175E,	// maxfrmheight_l
	
	0x1800,
	0x1900,
	0x1A20,
	0x1B00,
	0x1C8B,
	0x1DC0,
	0x1E00,
	0x1F8B,
	0x20C0,

	0x0307, // Bank-H
	0x1507,	// zoom_x_h		(1920)
	0x1680,	// zoom_x_l       
	0x1704,	// zoom_y_h     (1080)    
	0x1838,	// zoom_y_l 

#endif
	DATA_END_FLAG ,
};

static const unsigned short frm_attr_720[] = {
//	0x0302, // Bank-C
//	0x8B01,	// i2c_control_1	(not aev)
#if defined(SUPPORT_PO3100K) || defined(SUPPORT_PV2109M) || defined(SUPPORT_PS4100K)
	0x0300,     // Bank-A	
	0x22F0,	// i2c_control_1	(not aev)
#if defined(USE_SENSOR_25FPS)	
	0x0607,	// framewidth_h   
	0x07BB,	// framewidth_l  	// 0x7BB	(1979)
#else
	0x0606,	// framewidth_h   
	0x0771,	// framewidth_l  	// 0x671	(1649)
#endif	
	0x0802,	// fd_fheight_a_h 
	0x09ED,	// fd_fheight_a_l 
	0x0A02,	// fd_fheight_b_h 
	0x0BED,	// fd_fheight_b_l 
	0x0C00,	// windowx1_h   
	0x0D08,	// windowx1_l   
	0x0E00,	// windowy1_h   
	0x0F08,	// windowy1_l   
	
	0x1005,	// windowx2_h   
	0x1107,	// windowx2_l   
	0x1202,	// windowy2_h   
	0x13D7,	// windowy2_l   
	0x1400,	// vsyncstartrow_f0_h
	0x150D,	// vsyncstartrow_f0_l
	0x1602,	// vsyncstoprow_f0_h 
	0x17ED,	// vsyncstoprow_f0_l 
	0x1800,	// vsynccolume 
	0x1902,	// vsynccolume 

	0x4C05,	// fd_a_step_h
	0x4D79,	// fd_a_step_l
	0x4E04, // fd_b_step_h
	0x4F8D, // fd_b_step_l
	0x5040, // fd_winheight
	0x5100, // fd_a_delay
	0x5200, // fd_b_delay
	0x5340, // fd_th
	0x5400, // fd_period_a_h
	0x55BB, // fd_period_a_m
	0x5680, // fd_period_a_l
	0x5700, // fd_period_b_h
	0x58E1, // fd_period_b_m
	0x5900,	// fd_period_b_l
	0x5A04, // fd_period_c_h
	0x5B65, // fd_period_c_m

	0x0302,	// Bank-C
	// ae vsync start
	0xB003,	// (1649/2) = 0x338
	0xB138,
	// ae full
	0xB300,	// x1 position
	0xB400,	
	0xB505,	// x2 position
	0xB610,
	0xB700,	// y1 position
	0xB800,
	0xB902,	// y2 position
	0xBAE0,
	// ae center		
	0xBB01,
	0xBCB0,
	0xBD03,
	0xBE60,
	0xBF00,
	0xC0F5,
	0xC101,
	0xC2EA,
	// ae x,y			
	0xC302,
	0xC488,
	0xC501,
	0xC670,
	// awb		
	0xC700,
	0xC80A,
	0xC905,
	0xCA05,
	0xCB00,
	0xCC0A,
	0xCD02,
	0xCED5,

	// af center		
	0xD801,
	0xD9AF,
	0xDA03,
	0xDB5A,
	0xDC00,
	0xDDF5,
	0xDE01,
	0xDFE4,
	
	0x0304,	// Bank-E
	0x1202,	// expfrmh_h
	0x13E8,	// expfrmh_l
	0x1402,	// midfrmheight_h
	0x15E8,	// midfrmheight_l
	0x1602,	// maxfrmheight_h
	0x17E8,	// maxfrmheight_l
	
	0x1800,
	0x1900,
	0x1A20,
	0x1B00,
	0x1C5D,
	0x1D00,
	0x1E00,
	0x1F5D,
	0x2000,

	0x0307, // Bank-H
	0x1505,	// zoom_x_h		(1280)
	0x1600,	// zoom_x_l       
	0x1702,	// zoom_y_h     (720)    
	0x18D0,	// zoom_y_l 
	0x1900,	//
	0x1A00,	//
	0x1B00,	//
	0x1C00,	//
	0x1D02,	//        
	0x1EEE,	//
	0x1F00,	//
	0x2000,	//
#endif
	DATA_END_FLAG ,
};


static const unsigned short frm_attr_720_var[] = {
//	0x0302, // Bank-C
//	0x8B01,	// i2c_control_1	(not aev)
#if defined(SUPPORT_PS4100K)
	0x0300,     // Bank-A	
	0x22F0,	// i2c_control_1	(not aev)
#if defined(USE_SENSOR_25FPS)	
	0x060B,
	0x073F,
#else
	0x0609,	// framewidth_h        (0B)
	0x075F,	// framewidth_l        (21)
//	0x0606,	// framewidth_h        (0B)
//	0x0771,	// framewidth_l        (21)
#endif	
	0x0802,	// fd_fheight_a_h 
	0x09ED,	// fd_fheight_a_l 
	0x0A02,	// fd_fheight_b_h 
	0x0BED,	// fd_fheight_b_l 
	0x0C00,	// windowx1_h   
	0x0D01,	// windowx1_l   
	0x0E00,	// windowy1_h   
	0x0F01,	// windowy1_l   
	
	0x1005,	// windowx2_h   
	0x1100,	// windowx2_l   
	0x1202,	// windowy2_h   
	0x13D0,	// windowy2_l   
	0x1400,	// vsyncstartrow_f0_h
	0x150D,	// vsyncstartrow_f0_l
	0x1602,	// vsyncstoprow_f0_h 
	0x17ED,	// vsyncstoprow_f0_l 
	0x1800,	// vsynccolume 
	0x1902,	// vsynccolume 

#if 0
	0x4C05,	// fd_a_step_h
	0x4D79,	// fd_a_step_l
	0x4E04, // fd_b_step_h
	0x4F8D, // fd_b_step_l
	0x5040, // fd_winheight
	0x5100, // fd_a_delay
	0x5200, // fd_b_delay
	0x5340, // fd_th
	0x5400, // fd_period_a_h
	0x55BB, // fd_period_a_m
	0x5680, // fd_period_a_l
	0x5700, // fd_period_b_h
	0x58E1, // fd_period_b_m
	0x5900,	// fd_period_b_l
	0x5A04, // fd_period_c_h
	0x5B65, // fd_period_c_m
#endif
	0x0302,	// Bank-C
	// ae vsync start
	0xB003,	// (1649/2) = 0x338
	0xB138,
	// ae full
	0xB300,	// x1 position
	0xB408,	
	0xB505,	// x2 position
	0xB608,
	0xB700,	// y1 position
	0xB808,
	0xB902,	// y2 position
	0xBAD8,
	// ae center		
	0xBB01,
	0xBCB0,
	0xBD03,
	0xBE60,
	0xBF00,
	0xC0F5,
	0xC101,
	0xC2EA,
	// ae x,y			
	0xC302,
	0xC488,
	0xC501,
	0xC670,
	// awb		
#if 0	
	0xC700,
	0xC808,
	0xC905,
	0xCA08,
	0xCB00,
	0xCC08,
	0xCD02,
	0xCED8,
#else
	0xC700,
	0xC805,
	0xC905,
	0xCA04,
	0xCB00,
	0xCC05,
	0xCD02,
	0xCED4,
#endif
	// af center		
	0xD801,
	0xD9B0,
	0xDA03,
	0xDB60,
	0xDC00,
	0xDDF5,
	0xDE01,
	0xDFEA,
	
	0x0304,	// Bank-E
	0x1202,	// expfrmh_h
	0x13E8,	// expfrmh_l
	0x1402,	// midfrmheight_h
	0x15E8,	// midfrmheight_l
	0x1602,	// maxfrmheight_h
	0x17E8,	// maxfrmheight_l
	
	0x1800,
	0x1900,
	0x1A20,
	0x1B00,
	0x1C5D,
	0x1D00,
	0x1E00,
	0x1F5D,
	0x2000,

	0x0307, // Bank-H
	0x1505,	// zoom_x_h		(1280)
	0x1600,	// zoom_x_l       
	0x1702,	// zoom_y_h     (720)    
	0x18D0,	// zoom_y_l 
	0x1900,	//
	0x1A00,	//
	0x1B00,	//
	0x1C00,	//
	0x1D02,	//        
	0x1EEE,	//
	0x1F00,	//
	0x2000,	//
#endif
	DATA_END_FLAG ,
};

static const unsigned short flp_attr_hor[] = {
	0x0300,     // Bank-A	
	0x0501,
	DATA_END_FLAG ,
};

static const unsigned short flp_attr_ver[] = {
	0x0300,     // Bank-A	
	0x0502,
	DATA_END_FLAG ,
};

static const unsigned short flp_attr_all[] = {
	0x0300,     // Bank-A	
	0x0503,
	DATA_END_FLAG ,
};

static const unsigned short flp_attr_non[] = {
	0x0300,     // Bank-A	
	0x0500,
	DATA_END_FLAG ,
};

static const unsigned short bsq_attr_rggb[] = {
	0x0302,			// Bank-C
	0x05F8,
	DATA_END_FLAG ,
};

static const unsigned short bsq_attr_gbrg[] = {
	0x0302,			// Bank-C
	0x05F9,
	DATA_END_FLAG ,
};

static const unsigned short bsq_attr_grbg[] = {
	0x0302,			// Bank-C
	0x05FA,
	DATA_END_FLAG ,
};

static const unsigned short bsq_attr_bggr[] = {
	0x0302,			// Bank-C
	0x05FB,
	DATA_END_FLAG ,
};

static const unsigned short clk_attr_hallline[] = {
	0x0302,     // Bank-C
	0x9B24, 	// !!!!! CS ISP should be pclk inverted !!!!!!
	0x0300,     // Bank-A	
	0x2250,		// i2c_control_1	(use aev, default)
	
//	0x0302, 	// window display
//	0x0804,
	DATA_END_FLAG ,
};

static const unsigned short clk_attr_hnormal[] = {
	0x0302,     // Bank-C
	0x9B04, 	// !!!!! CS ISP should be pclk inverted !!!!!!
	0x0300,     // Bank-A	
	0x2250,		// i2c_control_1	(use aev, default)
	
//	0x0302, 	// window display
//	0x0804,
	DATA_END_FLAG ,
};

/* PS1210K */
static const unsigned short ps1210k_bayer_attr[] = {		// cs 
#if defined(SUPPORT_PS1210K)
	0x0301, ////////////  Bank-B
	0x59EE, // 

	0x0302, ////////////  Bank-C
	0x0900, // 
	0x0B82, // // 1 overwrited 
	0x3332, // 
	0x3487, // 
	0x358B, // 
	0x368E, // 
	0x373D, // 
	0x388F, // 
	0x3992, // 
	0x3A85, // 
	0x3B37, // 
	0x5B00, // 
	0x5C04, // 
	0x5D11, // 
	0x5E20, // 
	0x5F2D, // 
	0x6042, // 
	0x6152, // 
	0x626A, // 
	0x637D, // 
	0x649B, // 
	0x65B4, // 
	0x66C9, // 
	0x67DD, // 
	0x68EF, // 
	0x69FF, // 
	0x801F, // 
	0x8103, // 
	0x8283, // 
	0x831F, // 
	0x9500, // 
	0x9600, // 
	0x9700, // 

	0x0303, ////////////  Bank-D
	0x0D20, // 
	0x1040, // 
	0x1440, // 
	0x1A00, // 
	0x1B04, // 
	0x1C7F, // 
	0x1E00, // 
	0x1F08, // 
	0x2018, // 
	0x241C, // 
	0x251C, // 
	0x267F, // 
	0x2808, // 
	0x2908, // 
	0x2A7F, // 
	0x3F38, // 
	0x4010, // 
	0x4100, // 
	0x4831, // // 1 overwrited 
	0x4924, // // 1 overwrited 
	0x4A21, // // 1 overwrited 
	0x4B20, // // 1 overwrited 
	0x4C18, // // 1 overwrited 
	0x4D10, // // 1 overwrited 
	0x4E10, // // 1 overwrited 
	0x4F00, // 
	0x5000, // 
	0x5100, // 
	0x5310, // // 1 overwrited 
	0x540B, // // 1 overwrited 
	0x5508, // // 1 overwrited 
	0x5700, // // 1 overwrited 
	0x5800, // // 1 overwrited 
	0x5900, // // 1 overwrited 
	0x5B10, // 
	0x5C10, // 
	0x5D10, // 
	0x6004, // 
	0x6104, // 
	0x6204, // 
	0x6404, // 
	0x6504, // 
	0x6604, // 
	0x6840, // 
	0x6910, // 
	0x6A08, // 
	0x6C40, // 
	0x6D20, // 
	0x6E08, // 
	0x7040, // 
	0x7140, // 
	0x7A40, // 
	0x7B40, // 
	0x7C80, // 
	0x8AF0, // 
	0xE838, // // 1 overwrited 
	0xE940, // // 1 overwrited 
	0xEA40, // // 1 overwrited 

	0x0304, ////////////  Bank-E
	0x06A0, // 
	0x0E81, // 
	0x0FC0, // 
	0x10A0, // 
	0x3B70, // 
	0x3C68, // 
	0x3D48, // 
	0x3E68, // 
	0x3F48, // 
	0x4048, // 
	0x4100, // 
	0x4200, // 
	0x4314, // 
	0x4400, // 
	0x4502, // 
	0x46E8, // 
	0x4798, // 
	0x4845, // 
	0x49E0, // 
	0x4A00, // 
	0x4B8B, // 
	0x4CC0, // 
	0x7380, // 
	0x7480, // 
	0x7580, // 
	0x7680, // 
	0x7778, // 
	0x787C, // 
	0x9400, // 
	0x9800, // 
	0x9904, // 
	0x9A00, // 
	0x9B10, // 
	0x9C00, // 
	0x9D40, // 

	0x0309, ////////////  Bank-J
	0x0403, // 
	0x4930, // 
	0x4A10, // 
	0x4B40, // 
	0x6D04, // 
	0x8E00, // 
	0x8F04, // 
	0x9006, // 
	0x9106, // 
	0x9204, // 
	0x9303, // 
	0x9401, // 
	0x9500, // 
	0xAD08, // 
	0xAE10, // 
	0xAF20, // 
	0xB010, // 
	0xB108, // 
	0xB204, // 
	0xB360, // 
	0xB460, // 
	0xB540, // 
	0xB740, // 
	0xB803, // 
	0xB902, // 
	0xBB04, // 
#endif	
	DATA_END_FLAG , 
};


static const unsigned short ps1210k_lvds_attr[] = {		// cs
#if defined(SUPPORT_PS1210K)
	// FIRST_TUNE_20160517_ADD_CSHIFT	
	// SECOND_TUNE_20160518		
	// 160426
	// 160513

	0x0301, ////////////  Bank-B
	0x59EE, // 

	0x0302, ////////////  Bank-C
	0x0900, // 
	0x0B82, // // 1 overwrited 
	0x3332, // 
	0x3487, // 
	0x358B, // 
	0x368E, // 
	0x373D, // 
	0x388F, // 
	0x3992, // 
	0x3A85, // 
	0x3B37, // 
	0x5B00, // 
	0x5C04, // 
	0x5D11, // 
	0x5E20, // 
	0x5F2D, // 
	0x6042, // 
	0x6152, // 
	0x626A, // 
	0x637D, // 
	0x649B, // 
	0x65B4, // 
	0x66C9, // 
	0x67DD, // 
	0x68EF, // 
	0x69FF, // 
	0x801F, // 
	0x8103, // 
	0x8283, // 
	0x831F, // 
	0x9500, // 
	0x9600, // 
	0x9700, // 

	0x0303, ////////////  Bank-D
	0x0D20, // 
	0x1040, // 
	0x1440, // 
	0x1A00, // 
	0x1B04, // 
	0x1C7F, // 
	0x1E00, // 
	0x1F08, // 
	0x2018, // 
	0x241C, // 
	0x251C, // 
	0x267F, // 
	0x2808, // 
	0x2908, // 
	0x2A7F, // 
	0x3F38, // 
	0x4010, // 
	0x4100, // 
	0x4831, // // 1 overwrited 
	0x4924, // // 1 overwrited 
	0x4A21, // // 1 overwrited 
	0x4B20, // // 1 overwrited 
	0x4C18, // // 1 overwrited 
	0x4D10, // // 1 overwrited 
	0x4E10, // // 1 overwrited 
	0x4F00, // 
	0x5000, // 
	0x5100, // 
	0x5310, // // 1 overwrited 
	0x540B, // // 1 overwrited 
	0x5508, // // 1 overwrited 
	0x5700, // // 1 overwrited 
	0x5800, // // 1 overwrited 
	0x5900, // // 1 overwrited 
	0x5B10, // 
	0x5C10, // 
	0x5D10, // 
	0x6004, // 
	0x6104, // 
	0x6204, // 
	0x6404, // 
	0x6504, // 
	0x6604, // 
	0x6840, // 
	0x6910, // 
	0x6A08, // 
	0x6C40, // 
	0x6D20, // 
	0x6E08, // 
	0x7040, // 
	0x7140, // 
	0x7A40, // 
	0x7B40, // 
	0x7C80, // 
	0x8AF0, // 
	0xE838, // // 1 overwrited 
	0xE940, // // 1 overwrited 
	0xEA40, // // 1 overwrited 

	0x0304, ////////////  Bank-E
	0x06A0, // 
	0x0E81, // 
	0x0FC0, // 
	0x10A0, // 
	0x3B70, // 
	0x3C68, // 
	0x3D48, // 
	0x3E68, // 
	0x3F48, // 
	0x4048, // 
	0x4100, // 
	0x4200, // 
	0x4314, // 
	0x4400, // 
	0x4502, // 
	0x46E8, // 
	0x4798, // 
	0x4845, // 
	0x49E0, // 
	0x4A00, // 
	0x4B8B, // 
	0x4CC0, // 
	0x7380, // 
	0x7480, // 
	0x7580, // 
	0x7680, // 
	0x7778, // 
	0x787C, // 
	0x9400, // 
	0x9800, // 
	0x9904, // 
	0x9A00, // 
	0x9B10, // 
	0x9C00, // 
	0x9D40, // 

	0x0309, ////////////  Bank-J
	0x0403, // 
	0x4930, // 
	0x4A10, // 
	0x4B40, // 
	0x6D04, // 
	0x8E00, // 
	0x8F04, // 
	0x9006, // 
	0x9106, // 
	0x9204, // 
	0x9303, // 
	0x9401, // 
	0x9500, // 
	0xAD08, // 
	0xAE10, // 
	0xAF20, // 
	0xB010, // 
	0xB108, // 
	0xB204, // 
	0xB360, // 
	0xB460, // 
	0xB540, // 
	0xB740, // 
	0xB803, // 
	0xB902, // 
	0xBB04, // 
#endif
	DATA_END_FLAG, 
};

static const unsigned short ps1210k_bayer_3dnr_on_attr[] = {
	DATA_END_FLAG,
};

static const unsigned short ps1210k_bayer_3dnr_off_attr[] = {
	DATA_END_FLAG,
};

static const unsigned short ps1210k_lvds_3dnr_on_attr[] = {			// cs
	DATA_END_FLAG,
};

static const unsigned short ps1210k_lvds_3dnr_off_attr[] = {
	DATA_END_FLAG,
};

static const struct isp_3dnr_attr ps1210k_bayer_3dnr_attr = {
	.on = ps1210k_bayer_3dnr_on_attr,
	.off = ps1210k_bayer_3dnr_off_attr,
};

static const struct isp_3dnr_attr ps1210k_lvds_3dnr_attr = {
	.on = ps1210k_lvds_3dnr_on_attr,
	.off = ps1210k_lvds_3dnr_off_attr,
};

static const unsigned short ps1210k_ae_attr[] = {
	0x0301,     // Bank-B
	0x4FB7,
	0x50C0,
	0x51C1,
	0x52C2,
	0x53C3,
	0x54C4,
	0x55C5,
	DATA_END_FLAG, 
};


/* PS3210M */

/* 20151222 pc1210K based */
static const unsigned short ps3210m_bayer_attr[] = {		// cs
#if defined(SUPPORT_PS3210M)
	0x0301, ////////////  Bank-B
	0x59EE, // 

	0x0302, ////////////  Bank-C
	0x0900, // 
	0x0B82, // // 1 overwrited 
	0x3332, // 
	0x3487, // 
	0x358B, // 
	0x368E, // 
	0x373D, // 
	0x388F, // 
	0x3992, // 
	0x3A85, // 
	0x3B37, // 
	0x5B00, // 
	0x5C04, // 
	0x5D11, // 
	0x5E20, // 
	0x5F2D, // 
	0x6042, // 
	0x6152, // 
	0x626A, // 
	0x637D, // 
	0x649B, // 
	0x65B4, // 
	0x66C9, // 
	0x67DD, // 
	0x68EF, // 
	0x69FF, // 
	0x801F, // 
	0x8103, // 
	0x8283, // 
	0x831F, // 
	0x9500, // 
	0x9600, // 
	0x9700, // 

	0x0303, ////////////  Bank-D
	0x0D20, // 
	0x1040, // 
	0x1440, // 
	0x1A00, // 
	0x1B04, // 
	0x1C7F, // 
	0x1E00, // 
	0x1F08, // 
	0x2018, // 
	0x241C, // 
	0x251C, // 
	0x267F, // 
	0x2808, // 
	0x2908, // 
	0x2A7F, // 
	0x3F38, // 
	0x4010, // 
	0x4100, // 
	0x4831, // // 1 overwrited 
	0x4924, // // 1 overwrited 
	0x4A21, // // 1 overwrited 
	0x4B20, // // 1 overwrited 
	0x4C18, // // 1 overwrited 
	0x4D10, // // 1 overwrited 
	0x4E10, // // 1 overwrited 
	0x4F00, // 
	0x5000, // 
	0x5100, // 
	0x5310, // // 1 overwrited 
	0x540B, // // 1 overwrited 
	0x5508, // // 1 overwrited 
	0x5700, // // 1 overwrited 
	0x5800, // // 1 overwrited 
	0x5900, // // 1 overwrited 
	0x5B10, // 
	0x5C10, // 
	0x5D10, // 
	0x6004, // 
	0x6104, // 
	0x6204, // 
	0x6404, // 
	0x6504, // 
	0x6604, // 
	0x6840, // 
	0x6910, // 
	0x6A08, // 
	0x6C40, // 
	0x6D20, // 
	0x6E08, // 
	0x7040, // 
	0x7140, // 
	0x7A40, // 
	0x7B40, // 
	0x7C80, // 
	0x8AF0, // 
	0xE838, // // 1 overwrited 
	0xE940, // // 1 overwrited 
	0xEA40, // // 1 overwrited 

	0x0304, ////////////  Bank-E
	0x06A0, // 
	0x0E81, // 
	0x0FC0, // 
	0x10A0, // 
	0x3B70, // 
	0x3C68, // 
	0x3D48, // 
	0x3E68, // 
	0x3F48, // 
	0x4048, // 
	0x4100, // 
	0x4200, // 
	0x4314, // 
	0x4400, // 
	0x4502, // 
	0x46E8, // 
	0x4798, // 
	0x4845, // 
	0x49E0, // 
	0x4A00, // 
	0x4B8B, // 
	0x4CC0, // 
	0x7380, // 
	0x7480, // 
	0x7580, // 
	0x7680, // 
	0x7778, // 
	0x787C, // 
	0x9400, // 
	0x9800, // 
	0x9904, // 
	0x9A00, // 
	0x9B10, // 
	0x9C00, // 
	0x9D40, // 

	0x0309, ////////////  Bank-J
	0x0403, // 
	0x4930, // 
	0x4A10, // 
	0x4B40, // 
	0x6D04, // 
	0x8E00, // 
	0x8F04, // 
	0x9006, // 
	0x9106, // 
	0x9204, // 
	0x9303, // 
	0x9401, // 
	0x9500, // 
	0xAD08, // 
	0xAE10, // 
	0xAF20, // 
	0xB010, // 
	0xB108, // 
	0xB204, // 
	0xB360, // 
	0xB460, // 
	0xB540, // 
	0xB740, // 
	0xB803, // 
	0xB902, // 
	0xBB04, // 
#endif
	DATA_END_FLAG, 
};

/* 20160325 ps3210m LVDS based */
static const unsigned short ps3210m_lvds_attr[] = {		// cs
#if defined(SUPPORT_PS3210M)
	0x0301, ////////////  Bank-B
	0x59EE, // 

	0x0302, ////////////  Bank-C
	0x0900, // 
	0x0B82, // // 1 overwrited 
	0x3332, // 
	0x3487, // 
	0x358B, // 
	0x368E, // 
	0x373D, // 
	0x388F, // 
	0x3992, // 
	0x3A85, // 
	0x3B37, // 
	0x5B00, // 
	0x5C04, // 
	0x5D11, // 
	0x5E20, // 
	0x5F2D, // 
	0x6042, // 
	0x6152, // 
	0x626A, // 
	0x637D, // 
	0x649B, // 
	0x65B4, // 
	0x66C9, // 
	0x67DD, // 
	0x68EF, // 
	0x69FF, // 
	0x801F, // 
	0x8103, // 
	0x8283, // 
	0x831F, // 
	0x9500, // 
	0x9600, // 
	0x9700, // 

	0x0303, ////////////  Bank-D
	0x0D20, // 
	0x1040, // 
	0x1440, // 
	0x1A00, // 
	0x1B04, // 
	0x1C7F, // 
	0x1E00, // 
	0x1F08, // 
	0x2018, // 
	0x241C, // 
	0x251C, // 
	0x267F, // 
	0x2808, // 
	0x2908, // 
	0x2A7F, // 
	0x3F38, // 
	0x4010, // 
	0x4100, // 
	0x4831, // // 1 overwrited 
	0x4924, // // 1 overwrited 
	0x4A21, // // 1 overwrited 
	0x4B20, // // 1 overwrited 
	0x4C18, // // 1 overwrited 
	0x4D10, // // 1 overwrited 
	0x4E10, // // 1 overwrited 
	0x4F00, // 
	0x5000, // 
	0x5100, // 
	0x5310, // // 1 overwrited 
	0x540B, // // 1 overwrited 
	0x5508, // // 1 overwrited 
	0x5700, // // 1 overwrited 
	0x5800, // // 1 overwrited 
	0x5900, // // 1 overwrited 
	0x5B10, // 
	0x5C10, // 
	0x5D10, // 
	0x6004, // 
	0x6104, // 
	0x6204, // 
	0x6404, // 
	0x6504, // 
	0x6604, // 
	0x6840, // 
	0x6910, // 
	0x6A08, // 
	0x6C40, // 
	0x6D20, // 
	0x6E08, // 
	0x7040, // 
	0x7140, // 
	0x7A40, // 
	0x7B40, // 
	0x7C80, // 
	0x8AF0, // 
	0xE838, // // 1 overwrited 
	0xE940, // // 1 overwrited 
	0xEA40, // // 1 overwrited 

	0x0304, ////////////  Bank-E
	0x06A0, // 
	0x0E81, // 
	0x0FC0, // 
	0x10A0, // 
	0x3B70, // 
	0x3C68, // 
	0x3D48, // 
	0x3E68, // 
	0x3F48, // 
	0x4048, // 
	0x4100, // 
	0x4200, // 
	0x4314, // 
	0x4400, // 
	0x4502, // 
	0x46E8, // 
	0x4798, // 
	0x4845, // 
	0x49E0, // 
	0x4A00, // 
	0x4B8B, // 
	0x4CC0, // 
	0x7380, // 
	0x7480, // 
	0x7580, // 
	0x7680, // 
	0x7778, // 
	0x787C, // 
	0x9400, // 
	0x9800, // 
	0x9904, // 
	0x9A00, // 
	0x9B10, // 
	0x9C00, // 
	0x9D40, // 

	0x0309, ////////////  Bank-J
	0x0403, // 
	0x4930, // 
	0x4A10, // 
	0x4B40, // 
	0x6D04, // 
	0x8E00, // 
	0x8F04, // 
	0x9006, // 
	0x9106, // 
	0x9204, // 
	0x9303, // 
	0x9401, // 
	0x9500, // 
	0xAD08, // 
	0xAE10, // 
	0xAF20, // 
	0xB010, // 
	0xB108, // 
	0xB204, // 
	0xB360, // 
	0xB460, // 
	0xB540, // 
	0xB740, // 
	0xB803, // 
	0xB902, // 
	0xBB04, // 
#endif
	DATA_END_FLAG, 
};

static const unsigned short ps3210m_bayer_3dnr_on_attr[] = {
	DATA_END_FLAG,
};

static const unsigned short ps3210m_bayer_3dnr_off_attr[] = {
	DATA_END_FLAG,
};

static const unsigned short ps3210m_lvds_3dnr_on_attr[] = {
	DATA_END_FLAG,
};

static const unsigned short ps3210m_lvds_3dnr_off_attr[] = {
	DATA_END_FLAG,
};

static const struct isp_3dnr_attr ps3210m_bayer_3dnr_attr = {
	.on = ps3210m_bayer_3dnr_on_attr,
	.off = ps3210m_bayer_3dnr_off_attr,
};

static const struct isp_3dnr_attr ps3210m_lvds_3dnr_attr = {
	.on = ps3210m_lvds_3dnr_on_attr,
	.off = ps3210m_lvds_3dnr_off_attr,
};

static const unsigned short ps3210m_ae_attr[] = {
	0x0301,     // Bank-B
	0x4FB7,
	0x50C0,
	0x51C1,
	0x52C2,
	0x53C3,
	0x54C4,
	0x55C5,
	DATA_END_FLAG, 
};

/* PS3210P */


static const unsigned short ps3210p_bayer_attr[] = {				// cs
#if defined(SUPPORT_PS3210P)
	// FIRST_TUNE_20160426_ADD_CSHIFT	

	0x0301, ////////////  Bank-B
	0x59EE, // 

	0x0302, ////////////  Bank-C
	0x0900, // // 1 overwrited 
	0x0B82, // // 2 overwrited 
	0x0D11, // // 1 overwrited 
	0x0E11, // // 1 overwrited 
	0x0F11, // // 1 overwrited 
	0x1011, // // 1 overwrited 
	0x1111, // // 1 overwrited 
	0x1211, // // 1 overwrited 
	0x1311, // // 1 overwrited 
	0x1411, // // 1 overwrited 
	0x1511, // // 1 overwrited 
	0x1611, // // 1 overwrited 
	0x1711, // // 1 overwrited 
	0x1811, // // 1 overwrited 
	0x1911, // // 1 overwrited 
	0x1A11, // // 1 overwrited 
	0x1B11, // // 1 overwrited 
	0x1C11, // // 1 overwrited 
	0x333D, // // 1 overwrited 
	0x3487, // // 1 overwrited 
	0x358A, // // 1 overwrited 
	0x368E, // // 1 overwrited 
	0x3746, // // 1 overwrited 
	0x388F, // // 1 overwrited 
	0x3992, // // 1 overwrited 
	0x3A85, // // 1 overwrited 
	0x3B41, // // 1 overwrited 
	0x3D00, // 
	0x3E0F, // 
	0x3F26, // 
	0x4037, // 
	0x4143, // 
	0x4254, // 
	0x4362, // 
	0x4477, // 
	0x4588, // 
	0x46A4, // 
	0x47BB, // 
	0x48CF, // 
	0x49E0, // 
	0x4AF1, // 
	0x4BFF, // 
	0x4C00, // 
	0x4D20, // 
	0x4E2D, // 
	0x4F37, // 
	0x5040, // 
	0x514E, // 
	0x525A, // 
	0x536F, // 
	0x5480, // 
	0x559C, // 
	0x56B5, // 
	0x57CA, // 
	0x58DD, // 
	0x59EF, // 
	0x5AFF, // 
	0x5B00, // // 1 overwrited 
	0x5C20, // // 1 overwrited 
	0x5D2D, // // 1 overwrited 
	0x5E37, // // 1 overwrited 
	0x5F40, // // 1 overwrited 
	0x604E, // // 1 overwrited 
	0x615A, // // 1 overwrited 
	0x626F, // // 1 overwrited 
	0x6380, // // 1 overwrited 
	0x649C, // // 1 overwrited 
	0x65B5, // // 1 overwrited 
	0x66CA, // // 1 overwrited 
	0x67DD, // // 1 overwrited 
	0x68EF, // // 1 overwrited 
	0x69FF, // // 1 overwrited 
	0x6A00, // 
	0x6B30, // 
	0x6C40, // 
	0x6D4B, // 
	0x6E54, // 
	0x6F63, // 
	0x706F, // 
	0x7183, // 
	0x7293, // 
	0x73AD, // 
	0x74C2, // 
	0x75D4, // 
	0x76E4, // 
	0x77F2, // 
	0x78FF, // 
	0x8026, // // 1 overwrited 
	0x8108, // // 1 overwrited 
	0x8288, // // 1 overwrited 
	0x8324, // // 1 overwrited 
	0x9500, // // 1 overwrited 
	0x9600, // // 1 overwrited 
	0x9700, // // 1 overwrited 

	0x0303, ////////////  Bank-D
	0x0D26, // // 1 overwrited 
	0x1040, // 
	0x1440, // 
	0x1A00, // // 1 overwrited 
	0x1B04, // // 1 overwrited 
	0x1C7F, // // 1 overwrited 
	0x1E00, // // 1 overwrited 
	0x1F08, // // 1 overwrited 
	0x2018, // // 1 overwrited 
	0x241C, // // 1 overwrited 
	0x251C, // // 1 overwrited 
	0x267F, // // 1 overwrited 
	0x2808, // // 1 overwrited 
	0x2908, // // 1 overwrited 
	0x2A7F, // // 1 overwrited 
	0x3F38, // // 1 overwrited 
	0x4010, // // 1 overwrited 
	0x4100, // // 1 overwrited 
	0x4327, // 
	0x4880, // // 2 overwrited 
	0x495E, // // 2 overwrited 
	0x4A3A, // // 2 overwrited 
	0x4B28, // // 2 overwrited 
	0x4C18, // // 2 overwrited 
	0x4D10, // // 2 overwrited 
	0x4E0D, // // 2 overwrited 
	0x4F00, // // 1 overwrited 
	0x5000, // // 1 overwrited 
	0x5100, // // 1 overwrited 
	0x5310, // // 2 overwrited 
	0x5410, // // 2 overwrited 
	0x5508, // // 2 overwrited 
	0x5708, // // 2 overwrited 
	0x5808, // // 2 overwrited 
	0x5908, // // 2 overwrited 
	0x5B18, // // 1 overwrited 
	0x5C10, // // 1 overwrited 
	0x5D10, // // 1 overwrited 
	0x6004, // // 1 overwrited 
	0x6104, // // 1 overwrited 
	0x6204, // // 1 overwrited 
	0x6404, // // 1 overwrited 
	0x6504, // // 1 overwrited 
	0x6604, // // 1 overwrited 
	0x6840, // // 1 overwrited 
	0x6910, // // 1 overwrited 
	0x6A08, // // 1 overwrited 
	0x6C40, // // 1 overwrited 
	0x6D20, // // 1 overwrited 
	0x6E08, // // 1 overwrited 
	0x7040, // // 1 overwrited 
	0x7140, // // 1 overwrited 
	0x7A80, // // 1 overwrited 
	0x7B80, // // 1 overwrited 
	0x7C80, // // 1 overwrited 
	0x7E00, // 
	0x7F2E, // 
	0x803C, // 
	0x8203, // 
	0x8302, // 
	0x8401, // 
	0x8653, // 
	0x8746, // 
	0x8822, // 
	0x8AF0, // 
	0xE838, // // 2 overwrited 
	0xE93C, // // 2 overwrited 
	0xEA40, // // 2 overwrited 

	0x0304, ////////////  Bank-E
	0x0564, // 
	0x06A0, // // 1 overwrited 
	0x0E83, // // 1 overwrited 
	0x0FA0, // // 1 overwrited 
	0x1090, // // 1 overwrited 
	0x3B88, // // 1 overwrited 
	0x3C70, // // 1 overwrited 
	0x3D60, // // 1 overwrited 
	0x3E70, // // 1 overwrited 
	0x3F68, // // 1 overwrited 
	0x4048, // // 1 overwrited 
	0x4184, // // 1 overwrited 
	0x4200, // // 1 overwrited 
	0x4314, // // 1 overwrited 
	0x4400, // // 1 overwrited 
	0x4502, // // 1 overwrited 
	0x46E8, // // 1 overwrited 
	0x4798, // // 1 overwrited 
	0x4845, // // 1 overwrited 
	0x49E0, // // 1 overwrited 
	0x4A00, // // 1 overwrited 
	0x4B8B, // // 1 overwrited 
	0x4CC0, // // 1 overwrited 
	0x737A, // // 1 overwrited 
	0x7478, // // 1 overwrited 
	0x7574, // // 1 overwrited 
	0x7673, // // 1 overwrited 
	0x7778, // // 1 overwrited 
	0x7878, // // 1 overwrited 
	0x7940, // 
	0x7A5A, // 
	0x7B60, // 
	0x9400, // 
	0x9800, // // 1 overwrited 
	0x9904, // // 1 overwrited 
	0x9A00, // // 1 overwrited 
	0x9B10, // // 1 overwrited 
	0x9C00, // // 1 overwrited 
	0x9D40, // // 1 overwrited 

	0x0309, ////////////  Bank-J
	0x0403, // // 1 overwrited 
	0x4930, // // 1 overwrited 
	0x4A10, // // 1 overwrited 
	0x4B40, // // 1 overwrited 
	0x6D04, // // 1 overwrited 
	0x8E00, // // 1 overwrited 
	0x8F04, // // 1 overwrited 
	0x9006, // // 1 overwrited 
	0x9106, // // 1 overwrited 
	0x9204, // // 1 overwrited 
	0x9303, // // 1 overwrited 
	0x9401, // // 1 overwrited 
	0x9500, // // 1 overwrited 
	0xAD08, // // 1 overwrited 
	0xAE10, // // 1 overwrited 
	0xAF20, // // 1 overwrited 
	0xB010, // // 1 overwrited 
	0xB108, // // 1 overwrited 
	0xB204, // // 1 overwrited 
	0xB360, // // 1 overwrited 
	0xB460, // // 1 overwrited 
	0xB540, // // 1 overwrited 
	0xB740, // // 1 overwrited 
	0xB803, // // 1 overwrited 
	0xB902, // // 1 overwrited 
	0xBB04, // // 1 overwrited 
#endif
	DATA_END_FLAG, 
};

static const unsigned short ps3210p_lvds_attr[] = {
	DATA_END_FLAG,
};

static const unsigned short ps3210p_bayer_3dnr_on_attr[] = {
	DATA_END_FLAG,
};

static const unsigned short ps3210p_bayer_3dnr_off_attr[] = {
	DATA_END_FLAG,
};

static const unsigned short ps3210p_lvds_3dnr_on_attr[] = {
	DATA_END_FLAG,
};

static const unsigned short ps3210p_lvds_3dnr_off_attr[] = {
	DATA_END_FLAG,
};

static const struct isp_3dnr_attr ps3210p_bayer_3dnr_attr = {
	.on = ps3210p_bayer_3dnr_on_attr,
	.off = ps3210p_bayer_3dnr_off_attr,
};

static const struct isp_3dnr_attr ps3210p_lvds_3dnr_attr = {
	.on = ps3210p_lvds_3dnr_on_attr,
	.off = ps3210p_lvds_3dnr_off_attr,
};

static const unsigned short ps3210p_ae_attr[] = {
	0x0301,     // Bank-B
	0x4FB7,
	0x50C0,
	0x51C1,
	0x52C2,
	0x53C3,
	0x54C4,
	0x55C5,
	DATA_END_FLAG, 
};


/* PS3210K */

static const unsigned short ps3210k_bayer_attr[] = {		// cs 
#if defined(SUPPORT_PS3210K)
	// PS3210K_PM1208K_17_test_12.ccf
	/* PS3210K seyeon board tunning value */

	0x0301, ////////////  Bank-B
	0x59EE, // 
	0xC004, // 
	0xC14F, // 
	0xC200, // 
	0xC300, // 
	0xC440, // 

	0x0302, ////////////  Bank-C
	0x05F8, // // 2 overwrited 
	0x0982, // // 1 overwrited 
	0x0B82, // // 2 overwrited 
	0x0D0A, // // 2 overwrited 
	0x0E16, // // 2 overwrited 
	0x0F11, // // 2 overwrited 
	0x1019, // // 2 overwrited 
	0x1107, // // 2 overwrited 
	0x120E, // // 2 overwrited 
	0x1311, // // 2 overwrited 
	0x1411, // // 2 overwrited 
	0x150B, // // 2 overwrited 
	0x160E, // // 2 overwrited 
	0x1711, // // 2 overwrited 
	0x1811, // // 2 overwrited 
	0x1901, // // 2 overwrited 
	0x1A01, // // 2 overwrited 
	0x1B11, // // 2 overwrited 
	0x1C11, // // 2 overwrited 
	0x1D1E, // 
	0x1E20, // 
	0x1F20, // 
	0x200A, // 
	0x2140, // 
	0x2340, // 
	0x2540, // 
	0x2740, // 
	0x3332, // 
	0x3487, // 
	0x358B, // 
	0x368E, // 
	0x373D, // 
	0x388F, // 
	0x3992, // 
	0x3A85, // 
	0x3B37, // 
	0x3D00, // // 1 overwrited 
	0x3E30, // // 1 overwrited 
	0x3F40, // // 1 overwrited 
	0x404B, // // 1 overwrited 
	0x4154, // // 1 overwrited 
	0x4263, // // 1 overwrited 
	0x436F, // // 1 overwrited 
	0x4483, // // 1 overwrited 
	0x4593, // // 1 overwrited 
	0x46AD, // // 1 overwrited 
	0x47C2, // // 1 overwrited 
	0x48D4, // // 1 overwrited 
	0x49E4, // // 1 overwrited 
	0x4AF2, // // 1 overwrited 
	0x4BFF, // // 1 overwrited 
	0x4C00, // 
	0x4D02, // 
	0x4E0A, // 
	0x4F15, // 
	0x5021, // 
	0x5138, // 
	0x524A, // 
	0x5365, // 
	0x5479, // 
	0x5599, // 
	0x56B3, // 
	0x57C9, // 
	0x58DD, // 
	0x59EF, // 
	0x5AFF, // 
	0x5B00, // // 2 overwrited 
	0x5C0F, // // 2 overwrited 
	0x5D26, // // 2 overwrited 
	0x5E37, // // 2 overwrited 
	0x5F43, // // 2 overwrited 
	0x6054, // // 2 overwrited 
	0x6162, // // 2 overwrited 
	0x6277, // // 2 overwrited 
	0x6388, // // 2 overwrited 
	0x64A4, // // 2 overwrited 
	0x65BB, // // 2 overwrited 
	0x66CF, // // 2 overwrited 
	0x67E0, // // 2 overwrited 
	0x68F1, // // 2 overwrited 
	0x69FF, // // 2 overwrited 
	0x6A00, // // 1 overwrited 
	0x6B0E, // // 1 overwrited 
	0x6C17, // // 1 overwrited 
	0x6D1E, // // 1 overwrited 
	0x6E25, // // 1 overwrited 
	0x6F31, // // 1 overwrited 
	0x703C, // // 1 overwrited 
	0x714F, // // 1 overwrited 
	0x7261, // // 1 overwrited 
	0x7381, // // 1 overwrited 
	0x749D, // // 1 overwrited 
	0x75B8, // // 1 overwrited 
	0x76D1, // // 1 overwrited 
	0x77E9, // // 1 overwrited 
	0x78FF, // // 1 overwrited 
	0x8025, // // 2 overwrited 
	0x8100, // // 2 overwrited 
	0x8200, // // 2 overwrited 
	0x8325, // // 2 overwrited 
	0x8A01, // 
	0x95ED, // // 2 overwrited 
	0x96F7, // // 2 overwrited 
	0x9715, // // 2 overwrited 

	0x0303, ////////////  Bank-D
	0x0419, // 
	0x050F, // 
	0x0602, // 
	0x0700, // 
	0x0811, // 
	0x0911, // 
	0x0D25, // // 2 overwrited 
	0x1040, // 
	0x1440, // 
	0x1A00, // 
	0x1B04, // 
	0x1C7F, // 
	0x1E00, // 
	0x1F08, // 
	0x2018, // 
	0x241C, // 
	0x251C, // 
	0x267F, // 
	0x2808, // 
	0x2908, // 
	0x2A7F, // 
	0x3340, // 
	0x3440, // 
	0x3540, // 
	0x3F19, // // 2 overwrited 
	0x400B, // // 1 overwrited 
	0x4100, // // 1 overwrited 
	0x4886, // // 3 overwrited 
	0x495B, // // 3 overwrited 
	0x4A4C, // // 3 overwrited 
	0x4B42, // // 3 overwrited 
	0x4C31, // // 3 overwrited 
	0x4D27, // // 3 overwrited 
	0x4E10, // // 3 overwrited 
	0x4F00, // 
	0x5000, // 
	0x5100, // 
	0x5324, // // 2 overwrited 
	0x5414, // // 2 overwrited 
	0x5508, // // 2 overwrited 
	0x5717, // // 2 overwrited 
	0x580A, // // 2 overwrited 
	0x5900, // // 2 overwrited 
	0x5B18, // // 1 overwrited 
	0x5C10, // // 1 overwrited 
	0x5D10, // // 1 overwrited 
	0x6004, // 
	0x6104, // 
	0x6204, // 
	0x6404, // 
	0x6504, // 
	0x6604, // 
	0x6840, // 
	0x6910, // 
	0x6A08, // 
	0x6C40, // 
	0x6D20, // 
	0x6E08, // 
	0x7040, // 
	0x7140, // 
	0x7200, // 
	0x7310, // 
	0x7413, // 
	0x7A40, // // 1 overwrited 
	0x7B2B, // // 1 overwrited 
	0x7C0F, // // 1 overwrited 
	0x8200, // 
	0x8314, // 
	0x8415, // 
	0x862F, // 
	0x8726, // 
	0x8819, // 
	0x8A90, // // 1 overwrited 
	0x933F, // 
	0x9412, // // 1 overwrited 
	0x9518, // // 1 overwrited 
	0x9824, // // 1 overwrited 
	0x992E, // // 1 overwrited 
	0x9C3B, // // 1 overwrited 
	0x9D47, // // 1 overwrited 
	0xA041, // // 1 overwrited 
	0xA15F, // // 1 overwrited 
	0xA466, // // 1 overwrited 
	0xA574, // // 1 overwrited 
	0xA876, // // 1 overwrited 
	0xA980, // // 1 overwrited 
	0xAD88, // 
	0xAE81, // // 1 overwrited 
	0xB004, // 
	0xB187, // // 1 overwrited 
	0xB30B, // 
	0xB401, // // 1 overwrited 
	0xB600, // 
	0xB782, // // 1 overwrited 
	0xB907, // 
	0xBA85, // // 1 overwrited 
	0xBC04, // 
	0xBD00, // // 1 overwrited 
	0xBF0B, // 
	0xC005, // // 1 overwrited 
	0xC20B, // 
	0xC305, // // 1 overwrited 
	0xC583, // 
	0xC604, // // 1 overwrited 
	0xC804, // 
	0xC904, // // 1 overwrited 
	0xCB84, // 
	0xCC00, // // 1 overwrited 
	0xCE04, // 
	0xCF04, // // 1 overwrited 
	0xE843, // // 4 overwrited 
	0xE940, // // 4 overwrited 
	0xEA3B, // // 4 overwrited 

	0x0304, ////////////  Bank-E
	0x0564, // 
	0x06A0, // // 1 overwrited 
	0x0E81, // 
	0x0FC0, // 
	0x10A0, // 
	0x1204, // 
	0x135E, // 
	0x1404, // 
	0x155E, // 
	0x1604, // 
	0x175E, // 
	0x1B00, // 
	0x1C68, // 
	0x1DD0, // 
	0x1E00, // 
	0x1F68, // 
	0x20D0, // 
	0x3B70, // // 2 overwrited 
	0x3C58, // // 2 overwrited 
	0x3D48, // // 2 overwrited 
	0x3E60, // // 2 overwrited 
	0x3F50, // // 2 overwrited 
	0x4040, // // 2 overwrited 
	0x4100, // // 2 overwrited 
	0x4200, // // 2 overwrited 
	0x4314, // // 2 overwrited 
	0x4400, // // 2 overwrited 
	0x4502, // // 2 overwrited 
	0x46E8, // // 2 overwrited 
	0x4798, // // 2 overwrited 
	0x4845, // // 2 overwrited 
	0x49E0, // // 2 overwrited 
	0x4A00, // // 2 overwrited 
	0x4B8B, // // 2 overwrited 
	0x4CC0, // // 2 overwrited 
	0x7380, // // 2 overwrited 
	0x7460, // // 2 overwrited 
	0x7580, // // 2 overwrited 
	0x7680, // // 2 overwrited 
	0x7778, // // 2 overwrited 
	0x7880, // // 2 overwrited 
	0x9400, // 
	0x9800, // // 1 overwrited 
	0x9902, // // 1 overwrited 
	0x9A00, // // 1 overwrited 
	0x9B0A, // // 1 overwrited 
	0x9C00, // // 1 overwrited 
	0x9D18, // // 1 overwrited 

	0x0309, ////////////  Bank-J
	0x0403, // 
	0x4930, // 
	0x4A10, // 
	0x4B40, // 
	0x6D04, // 
	0x8E00, // 
	0x8F04, // 
	0x9006, // 
	0x9106, // 
	0x9204, // 
	0x9303, // 
	0x9401, // 
	0x9500, // 
	0xAD08, // 
	0xAE10, // 
	0xAF20, // 
	0xB010, // 
	0xB108, // 
	0xB204, // 
	0xB360, // 
	0xB460, // 
	0xB540, // 
	0xB740, // 
	0xB803, // 
	0xB902, // 
	0xBB04, // 
#endif
	DATA_END_FLAG, 
};

static const unsigned short ps3210k_lvds_attr[] = {		// cs
	// PS3210K_PM1208K_17_test_12.ccf
	/* PS3210K seyeon board tunning value */
#if defined(SUPPORT_PS3210K)
	0x0301, ////////////  Bank-B
	0x59EE, // 
	0xC004, // 
	0xC14F, // 
	0xC200, // 
	0xC300, // 
	0xC440, // 

	0x0302, ////////////  Bank-C
	0x0982, // // 2 overwrited 
	0x0B82, // // 2 overwrited 
	0x0D0A, // // 2 overwrited 
	0x0E16, // // 2 overwrited 
	0x0F11, // // 2 overwrited 
	0x1019, // // 2 overwrited 
	0x1107, // // 2 overwrited 
	0x120E, // // 2 overwrited 
	0x1311, // // 2 overwrited 
	0x1411, // // 2 overwrited 
	0x150B, // // 2 overwrited 
	0x160E, // // 2 overwrited 
	0x1711, // // 2 overwrited 
	0x1811, // // 2 overwrited 
	0x1901, // // 2 overwrited 
	0x1A01, // // 2 overwrited 
	0x1B11, // // 2 overwrited 
	0x1C11, // // 2 overwrited 
	0x1D1E, // 
	0x1E20, // 
	0x1F20, // 
	0x200A, // 
	0x2140, // 
	0x2340, // 
	0x2540, // 
	0x2740, // 
	0x3332, // 
	0x3487, // 
	0x358B, // 
	0x368E, // 
	0x373D, // 
	0x388F, // 
	0x3992, // 
	0x3A85, // 
	0x3B37, // 
	0x3D00, // // 2 overwrited 
	0x3E12, // // 2 overwrited 
	0x3F2D, // // 2 overwrited 
	0x4040, // // 2 overwrited 
	0x414D, // // 2 overwrited 
	0x425F, // // 2 overwrited 
	0x436C, // // 2 overwrited 
	0x4481, // // 2 overwrited 
	0x4592, // // 2 overwrited 
	0x46AC, // // 2 overwrited 
	0x47C1, // // 2 overwrited 
	0x48D3, // // 2 overwrited 
	0x49E4, // // 2 overwrited 
	0x4AF2, // // 2 overwrited 
	0x4BFF, // // 2 overwrited 
	0x4C00, // 
	0x4D02, // 
	0x4E0A, // 
	0x4F15, // 
	0x5021, // 
	0x5138, // 
	0x524A, // 
	0x5365, // 
	0x5479, // 
	0x5599, // 
	0x56B3, // 
	0x57C9, // 
	0x58DD, // 
	0x59EF, // 
	0x5AFF, // 
	0x5B00, // // 3 overwrited 
	0x5C15, // // 3 overwrited 
	0x5D20, // // 3 overwrited 
	0x5E29, // // 3 overwrited 
	0x5F30, // // 3 overwrited 
	0x603E, // // 3 overwrited 
	0x6149, // // 3 overwrited 
	0x625E, // // 3 overwrited 
	0x636F, // // 3 overwrited 
	0x648E, // // 3 overwrited 
	0x65A9, // // 3 overwrited 
	0x66C1, // // 3 overwrited 
	0x67D7, // // 3 overwrited 
	0x68EC, // // 3 overwrited 
	0x69FF, // // 3 overwrited 
	0x6A00, // // 1 overwrited 
	0x6B0E, // // 1 overwrited 
	0x6C17, // // 1 overwrited 
	0x6D1E, // // 1 overwrited 
	0x6E25, // // 1 overwrited 
	0x6F31, // // 1 overwrited 
	0x703C, // // 1 overwrited 
	0x714F, // // 1 overwrited 
	0x7261, // // 1 overwrited 
	0x7381, // // 1 overwrited 
	0x749D, // // 1 overwrited 
	0x75B8, // // 1 overwrited 
	0x76D1, // // 1 overwrited 
	0x77E9, // // 1 overwrited 
	0x78FF, // // 1 overwrited 
	0x8025, // // 2 overwrited 
	0x8100, // // 2 overwrited 
	0x8200, // // 2 overwrited 
	0x8325, // // 2 overwrited 
	0x8A01, // 
	0x95E0, // // 3 overwrited 
	0x96F7, // // 3 overwrited 
	0x9715, // // 3 overwrited 

	0x0303, ////////////  Bank-D
	0x0419, // 
	0x050F, // 
	0x0602, // 
	0x0700, // 
	0x0811, // 
	0x0911, // 
	0x0D25, // // 2 overwrited 
	0x1040, // 
	0x1440, // 
	0x1A00, // 
	0x1B04, // 
	0x1C7F, // 
	0x1E00, // 
	0x1F08, // 
	0x2018, // 
	0x241C, // 
	0x251C, // 
	0x267F, // 
	0x2808, // 
	0x2908, // 
	0x2A7F, // 
	0x3340, // 
	0x3440, // 
	0x3540, // 
	0x3F19, // // 2 overwrited 
	0x400B, // // 1 overwrited 
	0x4100, // // 1 overwrited 
	0x4320, // 
	0x4886, // // 3 overwrited 
	0x495B, // // 3 overwrited 
	0x4A4C, // // 3 overwrited 
	0x4B42, // // 3 overwrited 
	0x4C31, // // 3 overwrited 
	0x4D27, // // 3 overwrited 
	0x4E10, // // 3 overwrited 
	0x4F00, // 
	0x5000, // 
	0x5100, // 
	0x5312, // // 3 overwrited 
	0x5411, // // 3 overwrited 
	0x5508, // // 3 overwrited 
	0x5712, // // 3 overwrited 
	0x580A, // // 3 overwrited 
	0x5900, // // 3 overwrited 
	0x5B14, // // 2 overwrited 
	0x5C10, // // 2 overwrited 
	0x5D10, // // 2 overwrited 
	0x6004, // 
	0x6104, // 
	0x6204, // 
	0x6404, // 
	0x6504, // 
	0x6604, // 
	0x6840, // 
	0x6910, // 
	0x6A08, // 
	0x6C40, // 
	0x6D20, // 
	0x6E08, // 
	0x7040, // // 1 overwrited 
	0x7140, // // 1 overwrited 
	0x7200, // 
	0x7310, // 
	0x7413, // 
	0x7A40, // // 1 overwrited 
	0x7B2B, // // 1 overwrited 
	0x7C0F, // // 1 overwrited 
	0x8200, // 
	0x8314, // 
	0x8415, // 
	0x862F, // 
	0x8726, // 
	0x8819, // 
	0x8A90, // // 1 overwrited 
	0x933F, // 
	0x9412, // // 1 overwrited 
	0x9518, // // 1 overwrited 
	0x9824, // // 1 overwrited 
	0x992E, // // 1 overwrited 
	0x9C3B, // // 1 overwrited 
	0x9D47, // // 1 overwrited 
	0xA041, // // 1 overwrited 
	0xA15F, // // 1 overwrited 
	0xA466, // // 1 overwrited 
	0xA574, // // 1 overwrited 
	0xA876, // // 1 overwrited 
	0xA980, // // 1 overwrited 
	0xAD88, // 
	0xAE81, // // 1 overwrited 
	0xB004, // 
	0xB187, // // 1 overwrited 
	0xB30B, // 
	0xB401, // // 1 overwrited 
	0xB600, // 
	0xB782, // // 1 overwrited 
	0xB907, // 
	0xBA85, // // 1 overwrited 
	0xBC04, // 
	0xBD00, // // 1 overwrited 
	0xBF0B, // 
	0xC005, // // 1 overwrited 
	0xC20B, // 
	0xC305, // // 1 overwrited 
	0xC583, // 
	0xC604, // // 1 overwrited 
	0xC804, // 
	0xC904, // // 1 overwrited 
	0xCB84, // 
	0xCC00, // // 1 overwrited 
	0xCE04, // 
	0xCF04, // // 1 overwrited 
	0xE840, // // 5 overwrited 
	0xE940, // // 5 overwrited 
	0xEA3B, // // 5 overwrited 

	0x0304, ////////////  Bank-E
	0x0564, // 
	0x06A0, // // 1 overwrited 
	0x0E81, // 
	0x0FC0, // 
	0x10A0, // 
	0x1204, // 
	0x135E, // 
	0x1404, // 
	0x155E, // 
	0x1604, // 
	0x175E, // 
	0x1B00, // 
	0x1C68, // 
	0x1DD0, // 
	0x1E00, // 
	0x1F68, // 
	0x20D0, // 
	0x3B90, // // 3 overwrited 
	0x3C68, // // 3 overwrited 
	0x3D48, // // 3 overwrited 
	0x3E80, // // 3 overwrited 
	0x3F50, // // 3 overwrited 
	0x4040, // // 3 overwrited 
	0x4100, // // 3 overwrited 
	0x4200, // // 3 overwrited 
	0x4314, // // 3 overwrited 
	0x4400, // // 3 overwrited 
	0x4502, // // 3 overwrited 
	0x46E8, // // 3 overwrited 
	0x4798, // // 3 overwrited 
	0x4845, // // 3 overwrited 
	0x49E0, // // 3 overwrited 
	0x4A00, // // 3 overwrited 
	0x4B8B, // // 3 overwrited 
	0x4CC0, // // 3 overwrited 
	0x7380, // // 2 overwrited 
	0x7460, // // 2 overwrited 
	0x7580, // // 2 overwrited 
	0x7680, // // 2 overwrited 
	0x7778, // // 2 overwrited 
	0x7880, // // 2 overwrited 
	0x9400, // 
	0x9800, // // 1 overwrited 
	0x9902, // // 1 overwrited 
	0x9A00, // // 1 overwrited 
	0x9B0A, // // 1 overwrited 
	0x9C00, // // 1 overwrited 
	0x9D18, // // 1 overwrited 

	0x0309, ////////////  Bank-J
	0x0403, // 
	0x4930, // 
	0x4A10, // 
	0x4B40, // 
	0x6D04, // 
	0x8E00, // 
	0x8F04, // 
	0x9006, // 
	0x9106, // 
	0x9204, // 
	0x9303, // 
	0x9401, // 
	0x9500, // 
	0xAD08, // 
	0xAE10, // 
	0xAF20, // 
	0xB010, // 
	0xB108, // 
	0xB204, // 
	0xB360, // 
	0xB460, // 
	0xB540, // 
	0xB740, // 
	0xB803, // 
	0xB902, // 
	0xBB04, // 
#endif
	DATA_END_FLAG, 
};

static const unsigned short ps3210k_bayer_3dnr_on_attr[] = {
	DATA_END_FLAG,
};

static const unsigned short ps3210k_bayer_3dnr_off_attr[] = {
	DATA_END_FLAG,
};

static const unsigned short ps3210k_lvds_3dnr_on_attr[] = {
	DATA_END_FLAG,
};

static const unsigned short ps3210k_lvds_3dnr_off_attr[] = {
	DATA_END_FLAG,
};

static const struct isp_3dnr_attr ps3210k_bayer_3dnr_attr = {
	.on = ps3210k_bayer_3dnr_on_attr,
	.off = ps3210k_bayer_3dnr_off_attr,
};

static const struct isp_3dnr_attr ps3210k_lvds_3dnr_attr = {
	.on = ps3210k_lvds_3dnr_on_attr,
	.off = ps3210k_lvds_3dnr_off_attr,
};

static const unsigned short ps3210k_ae_attr[] = {
	0x0301,     // Bank-B
	0x4FB7,
	0x50C0,
	0x51C1,
	0x52C2,
	0x53C3,
	0x54C4,
	0x55C5,

	DATA_END_FLAG, 
};


/* PS4100K */
static const unsigned short ps4100k_bayer_attr[] = {					// cs
#if defined(SUPPORT_PS4100K)	
	0x0301,     // Bank-B
	0x5964, 	
#endif	
	DATA_END_FLAG, 
};   

static const unsigned short ps4100k_lvds_attr[] = {
#if defined(SUPPORT_PS4100K)	
	/* CIF LVDS RX not support 720P */
#endif	
	DATA_END_FLAG,
};

static const unsigned short ps4100k_bayer_3dnr_on_attr[] = {			// es
	DATA_END_FLAG,
};

static const unsigned short ps4100k_bayer_3dnr_off_attr[] = {
	DATA_END_FLAG,
};

static const unsigned short ps4100k_lvds_3dnr_on_attr[] = {
	DATA_END_FLAG,
};

static const unsigned short ps4100k_lvds_3dnr_off_attr[] = {
	DATA_END_FLAG,
};

static const struct isp_3dnr_attr ps4100k_bayer_3dnr_attr = {
	.on = ps4100k_bayer_3dnr_on_attr,
	.off = ps4100k_bayer_3dnr_off_attr,
};

static const struct isp_3dnr_attr ps4100k_lvds_3dnr_attr = {
	.on = ps4100k_lvds_3dnr_on_attr,
	.off = ps4100k_lvds_3dnr_off_attr,
};

static const unsigned short ps4100k_ae_attr[] = {
	
	0x0301,     // Bank-B	
	0x4FC4,
	0x50CF,
	0x51D0,
	0x52D1,
	0x53D2,
	0x54D3,
	0x55D4,		
	DATA_END_FLAG, 
};

/* PS7100K */
static const unsigned short ps7100k_bayer_attr[] = {					// cs
#if defined(SUPPORT_PS7100K)	
	0x0301,     // Bank-B
	0x5964, 	
#endif	
	DATA_END_FLAG, 
};   

static const unsigned short ps7100k_lvds_attr[] = {
#if defined(SUPPORT_PS7100K)	
	/* CIF LVDS RX not support 720P */
#endif	
	DATA_END_FLAG,
};

static const unsigned short ps7100k_bayer_3dnr_on_attr[] = {			// es
	DATA_END_FLAG,
};

static const unsigned short ps7100k_bayer_3dnr_off_attr[] = {
	DATA_END_FLAG,
};

static const unsigned short ps7100k_lvds_3dnr_on_attr[] = {
	DATA_END_FLAG,
};

static const unsigned short ps7100k_lvds_3dnr_off_attr[] = {
	DATA_END_FLAG,
};

static const struct isp_3dnr_attr ps7100k_bayer_3dnr_attr = {
	.on = ps7100k_bayer_3dnr_on_attr,
	.off = ps7100k_bayer_3dnr_off_attr,
};

static const struct isp_3dnr_attr ps7100k_lvds_3dnr_attr = {
	.on = ps7100k_lvds_3dnr_on_attr,
	.off = ps7100k_lvds_3dnr_off_attr,
};

static const unsigned short ps7100k_ae_attr[] = {
	
	0x0301,     // Bank-B	
	0x4FB7,
	0x50C0,
	0x51C1,
	0x52C2,
	0x53C3,
	0x54C4,
	0x55C5,		// no hs_en register
	DATA_END_FLAG, 
};

static struct isp_ctrl_info init_isp_values[] = {
	/* PC3089N : no bayer output */
	[SNS_PC3089N] = {
		.chip_id = INVALID_CHIP_ID,
		.name = "pc3089n",
		.chip_addr = 0xff,
		.intf_supported = 0,
		.intf_mode = {
			0, 0
		},
		.intf_3dnr = {
			0, 0
		},
		.frm_attr = 0,
		.bsq_attr = 0,
		.flp_attr = 0,
		.clk_attr = 0,
		.ae_attr = 0,
	},
	/* PC7030K : no bayer output */
	[SNS_PC7030K] = {
		.chip_id = INVALID_CHIP_ID,
		.name = "pc7030k",
		.chip_addr = 0xff,
		.intf_supported = 0,
		.intf_mode = {
			0, 0
		},
		.intf_3dnr = {
			0, 0
		},
		.frm_attr = 0,
		.bsq_attr = 0,
		.flp_attr = 0,
		.clk_attr = 0,
		.ae_attr = 0,
	},
	/* PO3100K : sensor output has BT1120 */
	[SNS_PO3100K] = {
		.chip_id = INVALID_CHIP_ID,
		.name = "po3100k",
		.chip_addr = 0xff,
		.intf_supported = 0,
		.intf_mode = {
			0, 0
		},
		.intf_3dnr = {
			0, 0
		},
		.frm_attr = 0,
		.bsq_attr = 0,
		.flp_attr = 0,
		.clk_attr = 0,
		.ae_attr = 0,
	},
	/* PS1210K */
	[SNS_PS1210K] = {
		.chip_id = 0x2210,
		.name = "ps1210k",
		.chip_addr = 0x30,
#if defined(SUPPORT_PS1210K)		
		.intf_supported = (ISP_SUPPORT_BAYER | ISP_SUPPORT_LVDS),
#else
		.intf_supported = 0,
#endif
		.intf_mode = {
			ps1210k_bayer_attr, ps1210k_lvds_attr,
		},
		.intf_3dnr = {
			&ps1210k_bayer_3dnr_attr, &ps1210k_lvds_3dnr_attr,
		},
		.frm_attr = frm_attr_1080,
		.bsq_attr = bsq_attr_gbrg,
		.flp_attr = flp_attr_all,
		.clk_attr = clk_attr_hallline,
		.ae_attr = ps1210k_ae_attr,
	},
	/* PS1210M */
	[SNS_PS1210M] = {
		.chip_id = 0x2210,
		.name = "ps1210m",
		.chip_addr = 0x30,
		.intf_supported = 0,
		.intf_mode = {
			0, 0,
		},
		.intf_3dnr = {
			0, 0,
		},
		.frm_attr = 0,
		.bsq_attr = 0,
		.flp_attr = 0,
		.clk_attr = 0,
		.ae_attr = 0,
	},
	/* PS3210M */
	[SNS_PS3210M] = {
		.chip_id = 0x2210,
		.name = "ps3210m",
		.chip_addr = 0x30,
#if defined(SUPPORT_PS3210M)		
		.intf_supported = (ISP_SUPPORT_BAYER | ISP_SUPPORT_LVDS),
#else
		.intf_supported = 0,
#endif
		.intf_mode = {
			ps3210m_bayer_attr, ps3210m_lvds_attr,
		},
		.intf_3dnr = {
			&ps3210m_bayer_3dnr_attr, &ps3210m_lvds_3dnr_attr,
		},
		.frm_attr = frm_attr_1080,
		.bsq_attr = bsq_attr_grbg,
		.flp_attr = flp_attr_all,
		.clk_attr = clk_attr_hallline,
		.ae_attr = ps3210m_ae_attr,
	},
	/* PS3210P */
	[SNS_PS3210P] = {
		.chip_id = 0x2210,
		.name = "ps3210p",
		.chip_addr = 0x30,
#if defined(SUPPORT_PS3210P)		
		.intf_supported = (ISP_SUPPORT_BAYER),
#else
		.intf_supported = 0,
#endif
		.intf_mode = {
			ps3210p_bayer_attr, ps3210p_lvds_attr,
		},
		.intf_3dnr = {
			&ps3210p_bayer_3dnr_attr, &ps3210p_lvds_3dnr_attr,
		},
		.frm_attr = frm_attr_1080,
		.bsq_attr = bsq_attr_grbg,
		.flp_attr = flp_attr_all,
		.clk_attr = clk_attr_hallline,
		.ae_attr = ps3210p_ae_attr,
	},
	/* PV1219M */
	[SNS_PV1219M] = {
		.chip_id = 0x2210,
		.name = "pv1219m",
		.chip_addr = 0x30,
		.intf_supported = 0,
		.intf_mode = {
			0, 0,
		},
		.intf_3dnr = {
			0, 0,
		},
		.frm_attr = 0,
		.bsq_attr = 0,
		.flp_attr = 0,
		.clk_attr = 0,
		.ae_attr = 0,
	},
	/* PV2109M */
	[SNS_PV2109M] = {
		.chip_id = 0x2210,
		.name = "pv2109m",
		.chip_addr = 0x30,
		.intf_supported = 0,
		.intf_mode = {
			0, 0,
		},
		.intf_3dnr = {
			0, 0,
		},
		.frm_attr = 0,
		.bsq_attr = 0,
		.flp_attr = 0,
		.clk_attr = 0,
		.ae_attr = 0,
	},

	/* PS3210K */
	[SNS_PS3210K] = {
		.chip_id = 0x2210,
		.name = "ps3210k",
		.chip_addr = 0x30,
#if defined(SUPPORT_PS3210K)
		.intf_supported = (ISP_SUPPORT_BAYER | ISP_SUPPORT_LVDS),
#else
		.intf_supported = 0,
#endif
		.intf_mode = {
			ps3210k_bayer_attr, ps3210k_lvds_attr,
		},
		.intf_3dnr = {
			&ps3210k_bayer_3dnr_attr, &ps3210k_lvds_3dnr_attr,
		},
		.frm_attr = frm_attr_1080,
		.bsq_attr = bsq_attr_gbrg,
		.flp_attr = flp_attr_all,
		.clk_attr = clk_attr_hallline,
		.ae_attr = ps3210k_ae_attr,
	},

	/* PS4100K */
	[SNS_PS4100K] = {
		.chip_id = 0x2210,
		.name = "ps4100k",
		.chip_addr = 0x30,
#if defined(SUPPORT_PS4100K)		
		.intf_supported = (ISP_SUPPORT_BAYER/* | ISP_SUPPORT_LVDS*/),
#else
		.intf_supported = 0,
#endif
		.intf_mode = {
			ps4100k_bayer_attr, ps4100k_lvds_attr,
		},
		.intf_3dnr = {
			&ps4100k_bayer_3dnr_attr, &ps4100k_lvds_3dnr_attr,
		},
		.frm_attr = frm_attr_720,
		.bsq_attr = bsq_attr_bggr,
		.flp_attr = flp_attr_ver,
		.clk_attr = clk_attr_hnormal,
		.ae_attr = ps4100k_ae_attr,
	},
	
	/* PS7100K */
	[SNS_PS7100K] = {
		.chip_id = 0x2210,
		.name = "ps7100k",
		.chip_addr = 0x30,
#if defined(SUPPORT_PS7100K)		
		.intf_supported = (ISP_SUPPORT_BAYER/* | ISP_SUPPORT_LVDS*/),
#else
		.intf_supported = 0,
#endif
		.intf_mode = {
			ps7100k_bayer_attr, ps7100k_lvds_attr,
		},
		.intf_3dnr = {
			&ps7100k_bayer_3dnr_attr, &ps7100k_lvds_3dnr_attr,
		},
		.frm_attr = frm_attr_720_var,
		.bsq_attr = bsq_attr_bggr,
		.flp_attr = flp_attr_all,
		.clk_attr = clk_attr_hnormal,
		.ae_attr = ps7100k_ae_attr,
	},

};



#endif // ISP_CONFIGS_CS_H_
