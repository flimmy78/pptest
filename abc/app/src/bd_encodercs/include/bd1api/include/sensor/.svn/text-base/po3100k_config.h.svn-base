#ifndef SENSOR_PO3100K_H_
#define SENSOR_PO3100K_H_ 1

#include <bd_sns_ctrl.h>

#if defined(SENSOR_ESTYPE)
static const unsigned short init_ntsc_bt1120_config[] = {
     0xFFFF, 
};
#else
static const unsigned short init_ntsc_bt1120_config[] = {
     0xFFFF, 
};
#endif

/* 1920x1080 */
static struct sensor_ctrl_info init_sensor_values = {
	.chip_id = 0x3100,
	.name = "po3100k",
	.chip_addr = 0x77,
	.use_16bit_addr = 0,
	.use_16bit_data = 0,
	.scan_supported = SNS_SUPPORT_NTSC,
	.scan_mode = {
		[SNS_SCAN_NTSC] = {
			.intf_supported = (SNS_SUPPORT_BT1120),
			.intf_mode = 
			{
				0, 0, init_ntsc_bt1120_config, 0, 0,
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

#endif // SENSOR_PO3100K_H_
