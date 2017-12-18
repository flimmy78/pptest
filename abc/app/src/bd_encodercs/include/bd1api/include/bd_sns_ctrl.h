#ifndef __BD_SNS_CTRL_H_
#define __BD_SNS_CTRL_H_	1

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/* do not change this */
#define	SNS_PC3089N  	0
#define SNS_PC7030K	 	1
#define SNS_PO3100K	 	2
#define SNS_PS1210K	 	3
#define SNS_PS1210M	 	4
#define SNS_PS3210M  	5
#define SNS_PS3210P  	6	
#define SNS_PV1219M  	7	
#define SNS_PV2109M  	8
#define SNS_PS3210K		9	
#define SNS_PS4100K		10	
#define SNS_PS7100K		11	

#define get_sns_str(x)	\
	(x == SNS_PC3089N)?"PC3089N":\
	(x == SNS_PC7030K)?"PC7030K":\
	(x == SNS_PO3100K)?"PO3100K":\
	(x == SNS_PS1210K)?"PS1210K":\
	(x == SNS_PS1210M)?"PS1210M":\
	(x == SNS_PS3210M)?"PS3210M":\
	(x == SNS_PS3210P)?"PS3210P":\
	(x == SNS_PV1219M)?"PV1219M":\
	(x == SNS_PV2109M)?"PV2109M":\
	(x == SNS_PS3210K)?"PS3210K":\
	(x == SNS_PS4100K)?"PS4100K":\
	(x == SNS_PS7100K)?"PS7100K":"UNKNOWN"

#define SNS_INTF_ISPMASK	2
#define SNS_INTF_BAYER		0
#define SNS_INTF_LVDS		1
#define SNS_INTF_BT1120		2
#define SNS_INTF_BT656		3
#define SNS_INTF_BT601		4
#define SNS_INTF_PVI		5
#define SNS_INTF_MAX		(SNS_INTF_PVI+1)

#define get_intf_str(x)	\
	(x == SNS_INTF_BAYER)?"BAYER":\
	(x == SNS_INTF_LVDS)?"LVDS":\
	(x == SNS_INTF_BT1120)?"BT1120":\
	(x == SNS_INTF_BT656)?"BT656":\
	(x == SNS_INTF_BT601)?"BT601":\
	(x == SNS_INTF_PVI)?"PVI":"UNKNOWN"	 

#define SNS_SUPPORT_BAYER	0x01
#define SNS_SUPPORT_LVDS	0x02
#define SNS_SUPPORT_BT1120	0x04
#define SNS_SUPPORT_BT656	0x08
#define SNS_SUPPORT_BT601	0x10
#define SNS_SUPPORT_PVI		0x20

#define SNS_SUPPORT_NTSC	0x01
#define SNS_SUPPORT_PAL		0x02

#define SNS_SCAN_NTSC	0
#define SNS_SCAN_PAL		1
#define SNS_SCAN_MAX		(SNS_SCAN_PAL+1)

#define get_scan_str(x)	\
	(x == SNS_SCAN_NTSC)?"NTSC":\
	(x == SNS_SCAN_PAL)?"PAL":"UNKNOWN"

#define IsSnsNeedIsp(x)		((unsigned char)x < SNS_INTF_ISPMASK)

struct sensor_image_attr {
	unsigned char seq;
	unsigned short startx;
	unsigned short width;
	unsigned short delayx;
	unsigned short starty;
	unsigned short height;
	unsigned short delayy;
};

struct sensor_gain_attr {
	unsigned short dagin;
	unsigned short again;
};

struct sensor_init_info {
	const unsigned char intf_supported;
	const unsigned short *intf_mode[SNS_INTF_MAX];
	const unsigned short *frm_attr;
	const unsigned short *bsq_attr;
	const unsigned short *flp_attr;
	const unsigned short *clk_attr;
};

struct sensor_ctrl_info {
	const unsigned short chip_id;				// chip id
	const char *name;				// sensor name
	const unsigned char chip_addr;			// sensor i2c address
	const unsigned char use_16bit_addr;		// sensor has 16bit address
	const unsigned char use_16bit_data;		// sensor has 16bit data
	const unsigned char scan_supported;
	const struct sensor_init_info scan_mode[SNS_SCAN_MAX];

	void (*set_image_attr)(struct sensor_image_attr* attr);	
	int (*set_gain_attr)(struct sensor_gain_attr *attr);
};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
