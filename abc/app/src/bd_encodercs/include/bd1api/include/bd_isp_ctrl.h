#ifndef __BD_ISP_CTRL_H_
#define __BD_ISP_CTRL_H_	1

#include <bd_sns_ctrl.h>

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


#define ISP_INTF_BAYER			0
#define ISP_INTF_LVDS			1 
#define ISP_INTF_MAX			(ISP_INTF_LVDS+1)

#define ISP_SUPPORT_BAYER		0x01
#define ISP_SUPPORT_LVDS		0x02

struct isp_image_attr {
	unsigned char seq;
	unsigned short startx;
	unsigned short width;
	unsigned short delayx;
	unsigned short starty;
	unsigned short height;
	unsigned short delayy;
};

struct isp_gain_attr {
	unsigned short dagin;
	unsigned short again;
};


struct isp_3dnr_attr {
	const unsigned short *on;
	const unsigned short *off; 
};

struct isp_ctrl_info {
	const unsigned short chip_id;			// chip id
	const char *name;						// isp name
	const unsigned char chip_addr;			// isp i2c address

	const unsigned char intf_supported;
	const unsigned short *intf_mode[ISP_INTF_MAX];
	const struct isp_3dnr_attr *intf_3dnr[ISP_INTF_MAX];
	const unsigned short *frm_attr;
	const unsigned short *bsq_attr;
	const unsigned short *flp_attr;
	const unsigned short *clk_attr;
	const unsigned short *ae_attr; 

	void (*set_image_attr)(struct isp_image_attr* attr);	
	int (*set_gain_attr)(struct isp_gain_attr *attr);
};


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif
