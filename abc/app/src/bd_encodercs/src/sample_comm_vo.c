#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include <linux/fb.h>

#include "sample_comm.h"

#define BD1_FB_RES_X_VIRT	1280
#define BD1_FB_RES_Y_VIRT	720
#define BD1_FB_RES_X		480
#define BD1_FB_RES_Y		320
#define BD1_FB_BPP			16

struct SAMPLE_FB_S
{
    BD_BOOL bStart;
    pthread_t stFbPid;
	BD_U32 u32PhyAddr;
	BD_VOID *VirtAddr;
	BD_S32 fd;
	BD_U32 u32FbLen;
	
};

struct fbcolorfmt
{
	struct fb_bitfield red;
	struct fb_bitfield green;
	struct fb_bitfield blue;
	struct fb_bitfield transp;
};

static struct fbcolorfmt gColorFmt = {
	// RGB565
	{11, 5, 0},
	{5, 6, 0},
	{0, 5, 0},
	{0, 0, 0},

	// RGB888
//	{16, 8, 0},
//	{8, 8, 0},
//	{0, 8, 0},
//	{0, 0, 0},

};

static struct SAMPLE_FB_S gstFb;

struct PM1208FB_BUFFER_S
{
	unsigned int phyaddr;
	unsigned int len;
};

#define PM1208_FBIO_IOCTL_MAGIC	'f'
#define FBIOSET_CANVAS_BUF			_IOW(PM1208_FBIO_IOCTL_MAGIC, 0, struct PM1208FB_BUFFER_S)
#define FBIO_REFRESH_PM1208_FB		_IO(PM1208_FBIO_IOCTL_MAGIC, 1)
#define PM1208_FBIO_IOCTL_MAXNR	2


#define PM1208_SPI_XFER_RW			1
#define PM1208_SPI_SS1_DEV_NAME		"pm1208-spiss1"
#define PM1208_SPI_IOCTL_MAGIC			'b'

#if PM1208_SPI_XFER_RW
#define PM1208_SPI_MSGSIZE(N) \
	((((N)*(sizeof (struct pm1208_spi_transfer))) < (1 << _IOC_SIZEBITS)) \
		? ((N)*(sizeof (struct pm1208_spi_transfer))) : 0)
#define PM1208_SPI_IOCTL_MESSAGE(N) _IOW(PM1208_SPI_IOCTL_MAGIC, 0, char[PM1208_SPI_MSGSIZE(N)])
#define PM1208_SPI_IOCTL_MAXNR			1

struct pm1208_spi_transfer {
	__u64		tx_buf;
	__u64		rx_buf;

	__u32		len;
	__u32		speed_hz;

	__u16		delay_usecs;
	__u8		bits_per_word;
	__u8		cs_change;
	__u32		pad;
};

#else

#define PM1208_SPI_IOCTL_TRANSFER			_IOR(PM1208_SPI_IOCTL_MAGIC, 0, struct pm1208_spi_transfer)
#define PM1208_SPI_IOCTL_MAXNR			1

struct pm1208_spi_transfer{
	const void	*txbuf;
	void	*rxbuf;
	u32		len;
	u32		bits_per_word;
};
#endif


#define HX8357_EXIT_SLEEP_MODE          	0x11
#define HX8357_SET_DISPLAY_OFF          	0x28
#define HX8357_SET_DISPLAY_ON           	0x29
#define HX8357_SET_COLUMN_ADDRESS       	0x2A
#define HX8357_SET_PAGE_ADDRESS         	0x2B
#define HX8357_WRITE_MEMORY_START       	0x2C
#define HX8357_READ_MEMORY_START        	0x2E
#define HX8357_SET_TEAR_ON              	0x35
#define HX8357_SET_ADDRESS_MODE         	0x36
#define HX8357_SET_PIXEL_FORMAT         	0x3A
#define HX8357_WRITE_MEMORY_CONTINUE    	0x3C
#define HX8357_READ_MEMORY_CONTINUE     	0x3E
#define HX8357_SET_INTERNAL_OSCILLATOR   	0xB0
#define HX8357_SET_POWER_CONTROL         	0xB1
#define HX8357_SET_COLOR_FORMAT          	0xB3
#define HX8357_SET_DISPLAY_MODE          	0xB4
#define HX8357_SET_VCOM_VOLTAGE         	0xB6
#define HX8357_ENABLE_EXTENSION_COMMAND 	0xB9
#define HX8357_SET_PANEL_DRIVING        	0xC0    // not documented!
#define HX8357_SET_PANEL_CHARACTERISTIC  	0xCC
#define HX8357_SET_GAMMA_CURVE           	0xE0

#define HX8357_INVON 						0x21
#define HX8357_INVOFF						0x20
#define HX8357_DISPLAYOFF					0x28
#define HX8357_DISPLAYON					0x29

#define CMD	0x0
#define DAT	0x1

#define BYTES_PER_WORD	2

int fd = -1;
char *dev_file = "/dev/pm1208-spiss1";

static int rwlen = 0;
static unsigned short rwdata[256];

static void spi_push(unsigned char cmd, unsigned char data)
{
	rwdata[rwlen++] = (cmd<<8|data);
}

static void spi_write(void)
{
	struct pm1208_spi_transfer xfer;

	memset(&xfer, 0, sizeof(xfer));
	
	xfer.tx_buf = (unsigned long)rwdata;
	xfer.len = rwlen * BYTES_PER_WORD;
	xfer.bits_per_word = 9;//BYTES_PER_WORD * 8;

	ioctl(fd, PM1208_SPI_IOCTL_MESSAGE(1), &xfer);
	
	rwlen = 0;
	
}

#if defined(USE_TEST_LCD)

#define ili9806_bank_select(a)	\
{	\
	spi_push(0, 0xFF);	\
	spi_push(0, 0xFF);	\
	spi_push(0, 0x98);	\
	spi_push(0, 0x06);	\
	spi_push(0, 0x04);	\
	spi_push(0, a);		\
	spi_write();		\
	usleep(10000);		\
}

#define ili9806_write(a, b)	\
{	\
	spi_push(0, a);		\
	spi_push(0, b);		\
	spi_write();		\
	usleep(10000);		\
}


static BD_VOID SAMPLE_COMM_VO_ILI9608Init(BD_VOID)
{
	fd = open(dev_file, O_RDWR);
	
	ili9806_bank_select(0x00);	// Change to Page 0
	ili9806_write(0x28, 0x00);	// display off (no parameter)	
	usleep(120*1000);
	ili9806_write(0x10, 0x00);	// sleep on (no parameter)

	//********************************************************************//
	//****************************** Page 1 Command 
	//********************************************************************//
	ili9806_bank_select(0x01);  // Change to Page 1
	ili9806_write(0x08, 0x10);  // output SDA
	ili9806_write(0x21, (1<<3)|(1<<2)|(1<<1)|(1<<0));   // VS[3], HS[2], PCLK[1], DE[0] = 1 high , (default 0x01)
	ili9806_write(0x30, 0x02);    // 480 X 800		(864:0, 854:1, 800:2, 640:3:720:3)
	ili9806_write(0x31, 0x00);                 // colume inversion (0:colume, 1:1dot, 2:2dot, 3:3dot, 4:4dot)
	ili9806_write(0x40, 0x16);                // BT  ([7] 0: internal chargepump, 1:external chargepump, [5:4] 0: 1.5, 1:2 2:3 3:powerdown, [3:0] VCI 2.5 -3)
	ili9806_write(0x41, 0x33);                 // DVDDH DVDDL clamp (5.2 / -5.2)
	ili9806_write(0x42, 0x03);                 // VGH (2DDVDHH-DDVHL)/VGL (DDVDL-DDVDH) 
	ili9806_write(0x43, 0x09);                 // VGH_CLAMP 0FF (default) ;
	ili9806_write(0x44, 0x07);                 // VGL_CLAMP OFF (disable VGL clamp, level -11.0); 
	ili9806_write(0x45, 0x16);                 // VGL_REG  (VGH op-VOL 9.0, VGL op-VOL -11V)
	ili9806_write(0x50, 0x78);                   // VREG1 4.5V (default)
	ili9806_write(0x51, 0x78);                   // VREG2 -4.5V (default)
	ili9806_write(0x52, 0x00);                   //Flicker VCOM1 (default)
	ili9806_write(0x53, 0x4A);                   //Flicker VCOM2 (-1.1125)
	ili9806_write(0x57, 0x50);                   //Low voltage detect (core 1.60V, LOW 1.8V)
	ili9806_write(0x60, 0x07);                 // SDTI (internal source SDT timing adjust 7, default 5)
	ili9806_write(0x61, 0x00);                // CRTI (internal source CR timing adjust 0, default 5)
	ili9806_write(0x62, 0x08);                 // EQTI (internal source EQ timing adjust 8, defalt E)
	ili9806_write(0x63, 0x00);                // PCTI (internal source PC timinig adjust 0, default 5)
	//++++++++++++++++++ Gamma Setting ++++++++++++++++++//

	ili9806_write(0xA0, 0x00);  // Gamma 255 
	ili9806_write(0xA1, 0x09);  // Gamma 251 
	ili9806_write(0xA2, 0x0F);  // Gamma 247
	ili9806_write(0xA3, 0x0B);  // Gamma 239
	ili9806_write(0xA4, 0x06);  // Gamma 231
	ili9806_write(0xA5, 0x09);  // Gamma 203
	ili9806_write(0xA6, 0x07);  // Gamma 175
	ili9806_write(0xA7, 0x05);  // Gamma 147
	ili9806_write(0xA8, 0x08);  // Gamma 108
	ili9806_write(0xA9, 0x0C);  // Gamma 80
	ili9806_write(0xAA, 0x12);  // Gamma 52
	ili9806_write(0xAB, 0x08);  // Gamma 24
	ili9806_write(0xAC, 0x0D);  // Gamma 16
	ili9806_write(0xAD, 0x17);  // Gamma 8
	ili9806_write(0xAE, 0x0E);  // Gamma 4
	ili9806_write(0xAF, 0x00);  // Gamma 0
	///==============Nagitive Gamma Setting ++++++++++++++++++//
	ili9806_write(0xC0, 0x00);  // Gamma 255 
	ili9806_write(0xC1, 0x08);  // Gamma 251
	ili9806_write(0xC2, 0x0E);  // Gamma 247
	ili9806_write(0xC3, 0x0B);  // Gamma 239
	ili9806_write(0xC4, 0x05);  // Gamma 231
	ili9806_write(0xC5, 0x09);  // Gamma 203
	ili9806_write(0xC6, 0x07);  // Gamma 175
	ili9806_write(0xC7, 0x04);  // Gamma 147
	ili9806_write(0xC8, 0x08);  // Gamma 108
	ili9806_write(0xC9, 0x0C);  // Gamma 80
	ili9806_write(0xCA, 0x11);  // Gamma 52
	ili9806_write(0xCB, 0x07);  // Gamma 24
	ili9806_write(0xCC, 0x0D);  // Gamma 16
	ili9806_write(0xCD, 0x17);  // Gamma 8
	ili9806_write(0xCE, 0x0E);  // Gamma 4
	ili9806_write(0xCF, 0x00);  // Gamma 0


	//****************************************************************************//
	//****************************** Page 6 Command ******************************//
	//****************************************************************************//
	ili9806_bank_select(0x06);	// Change to Page 6

	///==============Gate control set 0 ++++++++++++++++++//
	ili9806_write(0x00, 0x21);
	ili9806_write(0x01, 0x0A);
	ili9806_write(0x02, 0x00);    
	ili9806_write(0x03, 0x00);
	ili9806_write(0x04, 0x01);
	ili9806_write(0x05, 0x01);
	ili9806_write(0x06, 0x80);    
	ili9806_write(0x07, 0x06);
	ili9806_write(0x08, 0x01);
	ili9806_write(0x09, 0x80);    
	ili9806_write(0x0A, 0x00);    
	ili9806_write(0x0B, 0x00);    
	ili9806_write(0x0C, 0x01);
	ili9806_write(0x0D, 0x01);
	ili9806_write(0x0E, 0x00);
	ili9806_write(0x0F, 0x00);
	ili9806_write(0x10, 0xF0);
	ili9806_write(0x11, 0xF4);
	ili9806_write(0x12, 0x04);
	ili9806_write(0x13, 0x00);
	ili9806_write(0x14, 0x00);
	ili9806_write(0x15, 0xC0);
	ili9806_write(0x16, 0x08);
	ili9806_write(0x17, 0x00);
	ili9806_write(0x18, 0x00);
	ili9806_write(0x19, 0x00);
	ili9806_write(0x1A, 0x00);
	ili9806_write(0x1B, 0x00);
	ili9806_write(0x1C, 0x00);
	ili9806_write(0x1D, 0x00);

	///==============Gate control set 1 ++++++++++++++++++//
	ili9806_write(0x20, 0x01);
	ili9806_write(0x21, 0x23);
	ili9806_write(0x22, 0x45);
	ili9806_write(0x23, 0x67);
	ili9806_write(0x24, 0x01);
	ili9806_write(0x25, 0x23);
	ili9806_write(0x26, 0x45);
	ili9806_write(0x27, 0x67);

	///==============Gate control set 2 ++++++++++++++++++//
	ili9806_write(0x30, 0x01);
	ili9806_write(0x31, 0x11);	//GOUT1
	ili9806_write(0x32, 0x00);	//GOUT2
	ili9806_write(0x33, 0xEE);	//GOUT3
	ili9806_write(0x34, 0xFF);	//GOUT4
	ili9806_write(0x35, 0xBB);	//GOUT5
	ili9806_write(0x36, 0xCA);	//GOUT6
	ili9806_write(0x37, 0xDD);	//GOUT7
	ili9806_write(0x38, 0xAC);	//GOUT8
	ili9806_write(0x39, 0x76);	//GOUT9
	ili9806_write(0x3A, 0x67);	//GOUT10
	ili9806_write(0x3B, 0x22);	//GOUT11
	ili9806_write(0x3C, 0x22);	//GOUT12
	ili9806_write(0x3D, 0x22);	//GOUT13
	ili9806_write(0x3E, 0x22);	//GOUT14
	ili9806_write(0x3F, 0x22);
	ili9806_write(0x40, 0x22);

	ili9806_write(0x52, 0x10);			// ????? no command																													>>>> 질문 필요함. datasheet 는 53, 54 register
	ili9806_write(0x53, 0x10);      //12:VGLO tie VGL_REG; 10: VGLO tie VGL (default 0x10)

	//****************************************************************************//
	//****************************** Page 7 Command ******************************//
	//****************************************************************************//
	ili9806_bank_select(0x07);	// Change to Page 7
	ili9806_write(0x17, 0x22);  // VGL_REG ON			(default 0x22)
	ili9806_write(0x02, 0x77);  // 																																							>>>> 질문 필요함. datasheet 는 register 정보 없음
	ili9806_write(0x06, 0x13);  //																																							>>>> 질문 필요함. datasheet 는 register 정보 없음
	ili9806_write(0xE1, 0x79);  // 																																							>>>> 질문 필요함. datasheet 는 register 정보 없음			
	//****************************************************************************//

	ili9806_bank_select(0x00);	 // Change to Page 0
	/*********** added configureation ***********/
	ili9806_write(0x3A, 0x50);									// set 16bit RGB
	/*********** added configureation ***********/
	ili9806_write(0x11, 0x00);                 // Sleep-Out
	usleep(120*1000);
	ili9806_write(0x29, 0x00);                 // Display on
	ili9806_write(0x35, 0x00);                 // Tearing effect line on

	close(fd);
	return;
}

#else

#if 1
static BD_VOID SAMPLE_COMM_VO_HX8357Init(BD_VOID)
{
	fd = open(dev_file, O_RDWR);
	
	spi_push(CMD, HX8357_ENABLE_EXTENSION_COMMAND);	// 0xB9
	spi_push(DAT, 0xFF);
	spi_push(DAT, 0x83);
	spi_push(DAT, 0x57);
	spi_write();
	usleep(1000);

	spi_push(CMD, HX8357_SET_POWER_CONTROL);	// 0xB1	
	spi_push(DAT, 0x00);
	spi_push(DAT, 0x14);
	spi_push(DAT, 0x1C);
	spi_push(DAT, 0x1C);
	spi_push(DAT, 0xC3);
	spi_push(DAT, 0x44);
	spi_write();
	usleep(5000);

	spi_push(CMD, 0xB3);    //SETRGB 
	//spi_push(DAT, 0x43);     //SDO_EN=1, BYPASS=1, RM=1, DM=1 
	spi_push(DAT, 0xC3);     //SDO_EN=1, BYPASS=0, RM=1, DM=1 
	spi_push(DAT, 0x00);      //DPL=1, HSPL=0, VSPL=0, EPL=0 
	spi_push(DAT, 0x03);      //HPL 
	spi_push(DAT, 0x03);      //VPL 
	spi_write();
	usleep(5000);

	spi_push(CMD, 0xB4);      //SETCYC 
	spi_push(DAT, 0x22);          //2-dot 
	spi_push(DAT, 0x40);      //RTN 
	spi_push(DAT, 0x00);      //DIV 
	spi_push(DAT, 0x2A);      //N_DUM 
	spi_push(DAT, 0x2A);      //I_DUM 
	spi_push(DAT, 0x20);      //GDON 
	spi_push(DAT, 0x4E);      //GDOFF 
	spi_write();
	usleep(5000);

	spi_push(CMD, 0xB6);    //VCOMDC 
	spi_push(DAT, 0x3C);    //5c   
	spi_write();
	usleep(5000); 

	spi_push(CMD, 0xC0);      //SETSTBA 
	spi_push(DAT, 0x34);      //N_OPON 
	spi_push(DAT, 0x34);      //I_OPON 
	spi_push(DAT, 0x02);      //STBA 
	spi_push(DAT, 0x3C);      //STBA 
	spi_push(DAT, 0xC8);      //STBA 
	spi_push(DAT, 0x08);      //GENON 
	spi_write();
	usleep(5000); 

	spi_push(CMD, 0xC2);      // Set Gate EQ 
	spi_push(DAT, 0x00);   
	spi_push(DAT, 0x08); 
	spi_push(DAT, 0x04);
	spi_write();
	usleep(5000);

	spi_push(CMD, 0xCC);    //Set Panel 
	spi_push(DAT, 0x01);      //SS_Panel = 1, BG
	spi_write();
	usleep(5000);
	
	//GAMMA 2.2"
	spi_push(CMD, 0xE0);      //Set Gamma 
	spi_push(DAT, 0x03);	  //VRP0[6:0]
	spi_push(DAT, 0x07);	  //VRP1[6:0]
	spi_push(DAT, 0x13);	  //VRP2[6:0]
	spi_push(DAT, 0x20);	  //VRP3[6:0]
	spi_push(DAT, 0x29);	  //VRP4[6:0]
	spi_push(DAT, 0x3C);	  //VRP5[6:0]
	spi_push(DAT, 0x49);	  //VRP6[6:0]
	spi_push(DAT, 0x52);	  //VRP7[6:0]
	spi_push(DAT, 0x47);	  //VRP8[6:0]
	spi_push(DAT, 0x40);	  //VRP9[6:0]
	spi_push(DAT, 0x3A);	  //VRP10[6:0]
	spi_push(DAT, 0x32);	  //VRP11[6:0]
	spi_push(DAT, 0x30);	  //VRP12[6:0]
	spi_push(DAT, 0x2B);	  //VRP13[6:0]
	spi_push(DAT, 0x27);	  //VRP14[6:0]
	spi_push(DAT, 0x1C);	  //VRP15[6:0]

	spi_push(DAT, 0x03);	  //VRP0[6:0]
	spi_push(DAT, 0x07);	  //VRP1[6:0]
	spi_push(DAT, 0x13);	  //VRP2[6:0]
	spi_push(DAT, 0x20);	  //VRP3[6:0]
	spi_push(DAT, 0x29);	  //VRP4[6:0]
	spi_push(DAT, 0x3C);	  //VRP5[6:0]
	spi_push(DAT, 0x49);	  //VRP6[6:0]
	spi_push(DAT, 0x52);	  //VRP7[6:0]
	spi_push(DAT, 0x47);	  //VRP8[6:0]
	spi_push(DAT, 0x40);	  //VRP9[6:0]
	spi_push(DAT, 0x3A);	  //VRP10[6:0]
	spi_push(DAT, 0x32);	  //VRP11[6:0]
	spi_push(DAT, 0x30);	  //VRP12[6:0]
	spi_push(DAT, 0x2B);	  //VRP13[6:0]
	spi_push(DAT, 0x27);	  //VRP14[6:0]
	spi_push(DAT, 0x1C);	  //VRP15[6:0]

	spi_push(DAT, 0x00);
	spi_push(DAT, 0x01);
	spi_write();
	usleep(5000);
	
	spi_push(CMD, 0x3A);      //COLMOD 
	spi_push(DAT, 0x66);        
	spi_write();
	usleep(5000);

	spi_push(CMD, 0x36); //MADCTRL
#define DRAW_MY		0x80
#define DRAW_MX		0x40	
#define DRAW_MV		0x20
	spi_push(DAT, DRAW_MY/*|DRAW_MX|DRAW_MV*//*0x00*/);  
	spi_write();
	usleep(10000);

	spi_push(CMD, 0x11);      //Sleep Out 
	spi_write();
	usleep(150000);
	
	spi_push(CMD, 0x29);        //Display on 
	spi_write();
	usleep(5000);

	close(fd);
	return;
}

#else
/* lcd type ili9488 : 320 x 480 */
static void SAMPLE_COMM_VO_ILI9488Init(void)
{
	fd = open(dev_file, O_RDWR);

	printf("Set to ili9488\n");
	spi_push(CMD, 0xF7);
	spi_push(DAT, 0xA9);
	spi_push(DAT, 0x51);
	spi_push(DAT, 0x2C);
	spi_push(DAT, 0x82);
	spi_write();
	usleep(5000);

	spi_push(CMD, 0xC0);	// Power Control 1
	spi_push(DAT, 0x11);	// VREG1OUT 1.25x3.70 = 4.6250
	spi_push(DAT, 0x09);	// VREG2OUT -1.25x3.30 = -4.1250
	spi_write();
	usleep(5000);

	spi_push(CMD, 0xC1);	// Power Control 2
	spi_push(DAT, 0x41);	// VGH:VCIx6 VGL:VCIx4
	spi_write();
	usleep(5000);

	spi_push(CMD, 0xC5);	// VCOM Control
	spi_push(DAT, 0x00);	
	spi_push(DAT, 0x0A);	// VCM_REG = -1.84375
	spi_push(DAT, 0x80);	// VCM_REG_EN: enable
	spi_write();
	usleep(5000);

	spi_push(CMD, 0xB1);	// Frame Rate Control (In Normal Mode/Full Colors)
	spi_push(DAT, 0xB0);	// FRS: CNT=15 TE OFF Frame rate(Hz) 68.36 / TE ON frame rate: 65.65
	spi_push(DAT, 0x11);	
	spi_write();
	usleep(5000);

	spi_push(CMD, 0xB4);	//  Display Inversion Control 
	spi_push(DAT, 0x02);	// 2 dot inversion
	spi_write();
	usleep(5000);

	spi_push(CMD, 0xB6);	// display function control
	spi_push(DAT, 0x34);	// RGB interface
	spi_push(DAT, 0x22);
	spi_push(DAT, 0x00);
	spi_write();
	usleep(5000);

/*
	spi_push(CMD, 0xB7);	//  Entry Mode Set
	spi_push(DAT, 0xC6);	// EPF=2 DSTB=1 GON=0 DTE=1 GAS=0
	spi_write();
	usleep(5000);
*/

	spi_push(CMD, 0xBE);	// HS Lanes Control
	spi_push(DAT, 0x00);	
	spi_push(DAT, 0x04);	// ENHIBIT: ESD protection.ON
	spi_write();
	usleep(5000);

	spi_push(CMD, 0xE9);	// Set Image Function 
	spi_push(DAT, 0x00);	// DB_EN: Off
	spi_write();
	usleep(5000);

	spi_push(CMD, 0x36);	// Memory Access Control 
	spi_push(DAT, 0x08);	// BGR=1 (0 = RGB color filter panel, 1 = BGR color filter panel)
	spi_write();
	usleep(5000);	

	spi_push(CMD, 0x3A);	//  Interface Pixel Format
	spi_push(DAT, 0x66);	// 18bit/pixel     18 bits/pixel
	spi_write();
	usleep(5000);

	spi_push(CMD, 0xE0);	// PGAMCTRL (Positive Gamma Control)
	spi_push(DAT, 0x00);	
	spi_push(DAT, 0x07);
	spi_push(DAT, 0x10);
	spi_push(DAT, 0x09);
	spi_push(DAT, 0x17);
	spi_push(DAT, 0x0B);
	spi_push(DAT, 0x41);
	spi_push(DAT, 0x89);
	spi_push(DAT, 0x4B);
	spi_push(DAT, 0x0A);
	spi_push(DAT, 0x0C);
	spi_push(DAT, 0x0E);
	spi_push(DAT, 0x18);
	spi_push(DAT, 0x1B);
	spi_push(DAT, 0x0F);
	spi_write();
	usleep(5000);

	spi_push(CMD, 0XE1);	//  NGAMCTRL (Negative Gamma Control) 
	spi_push(DAT, 0x00);	
	spi_push(DAT, 0x17);
	spi_push(DAT, 0x1A);
	spi_push(DAT, 0x04);
	spi_push(DAT, 0x0E);
	spi_push(DAT, 0x06);
	spi_push(DAT, 0x2F);
	spi_push(DAT, 0x45);
	spi_push(DAT, 0x43);
	spi_push(DAT, 0x02);
	spi_push(DAT, 0x0A);
	spi_push(DAT, 0x09);
	spi_push(DAT, 0x32);
	spi_push(DAT, 0x36);
	spi_push(DAT, 0x0F);
	spi_write();
	usleep(5000);

	spi_push(CMD, 0x11);
	spi_write();
	usleep(5000);

	spi_push(CMD, 0x29);
	spi_write();
	usleep(5000);

	close(fd);

}
#endif

#endif


static BD_VOID SAMPLE_COMM_VO_InitLCD(BD_VOID)
{
#if defined(USE_TEST_LCD)
	SAMPLE_COMM_VO_ILI9608Init();
#else
#if 1	
	SAMPLE_COMM_VO_HX8357Init();
#else	
	SAMPLE_COMM_VO_ILI9488Init();
#endif	
#endif
	return;
}

static BD_VOID SAMPLE_COMM_VO_InitDevConfig(VO_DEVICE_CONFIG_S *pdc, BD_S32 scen)
{
	VO_LCD_CONFIG_S *lcd = &pdc->stLCDConfig;
	VO_BTO_CONFIG_S *bto = &pdc->stBTOConfig;
	VO_TV_CONFIG_S  *tv  = &pdc->stTVConfig;
	VO_MEM_CONFIG_S *mem = &pdc->stMEMConfig;
#if defined(USE_TEST_LCD)
	lcd->u32Width		= 800;
	lcd->u32Height		= 480;
#else
	lcd->u32Width		= 480;
	lcd->u32Height		= 320;
#endif
	lcd->u32VBlank		= 0;
	lcd->u32HBlank		= 0;
	lcd->u32VBackPorch	= 0;
	lcd->u32HBackPorch	= 0;
	lcd->u32VFrontPorch = 0;
	lcd->u32HFrontPorch = 0;
	lcd->enLCDType		= LCD_TYPE_BT656;	/// LCD_TYPE_E
	lcd->bUseRMF		= BD_TRUE;
	lcd->bValid			= BD_TRUE;

	bto->u32Width		= 1920;
	bto->u32Height		= 1080;
	bto->u32VBackPorch	= 0;
	bto->u32HBackPorch	= 0;
	bto->u32VFrontPorch	= 0;
	bto->u32HFrontPorch	= 0;
	bto->enBTOType		= BT_TYPE_1120;		/// BTO_TYPE_E
	bto->bInterlaced	= BD_FALSE;			// 0:Progressive, 0:Interlaced
	bto->bUseRMF		= BD_FALSE;
	bto->bValid			= BD_TRUE;

	tv->bTV_PAL			= BD_FALSE;			// 0:NTSC, 1:PAL
	tv->bUseRMF			= BD_FALSE;
	tv->bValid			= BD_TRUE;

	mem->u32Width		= 1920;
	mem->u32Height		= 1080;
	mem->u32ColorFmt	= DU_FORMAT_YUV422P;
	mem->bUseRMF		= BD_FALSE;
	mem->bValid			= BD_FALSE;

	pdc->bDualOut		= BD_TRUE;
	pdc->bUsePIP		= ((scen & 4) ? BD_TRUE : BD_FALSE);

	if (lcd->bValid) SAMPLE_COMM_VO_InitLCD();
}

static BD_VOID SAMPLE_COMM_VO_InitConfig(VO_CONFIG_S *pvc, VO_DEVICE_CONFIG_S *pvdc, BD_S32 s32Scene, VO_DEVICE_TYPE_E enODev, STREAM_TYPE_E enPMSStreamType, PIC_SIZE_E enPMSSize, STREAM_TYPE_E enPPSStreamType, PIC_SIZE_E enPPSSize)
{
	VO_LCD_CONFIG_S *lcd = &pvdc->stLCDConfig;
	VO_BTO_CONFIG_S *bto = &pvdc->stBTOConfig;
///	VO_TV_CONFIG_S  *tv  = &pvdc->stTVConfig;
	VO_MEM_CONFIG_S *mem = &pvdc->stMEMConfig;
	SIZE_S	stTargetSize;

	memset(pvc, 0, sizeof(VO_CONFIG_S));

	STREAM_INFO_S *pms = &pvc->stMainStream;
	STREAM_INFO_S *pps = &pvc->stPipStream;
	STREAM_INFO_S *p2s = &pvc->stPip2Stream;
	GUI_INFO_S *pgi = &pvc->stGUIInfo;
	RMF_INFO_S *pri = &pvc->stRMFInfo;
	RMF_INFO_S *pdr = &pvc->stDualRMF;
	VODEV_INFO_S *pvi = &pvc->stVODEVInfo;

	SAMPLE_COMM_SYS_GetPicSize(VIDEO_ENCODING_MODE_AUTO, enPMSSize, &stTargetSize);

	pms->u32SrcWidth	= stTargetSize.u32Width;
	pms->u32SrcHeight	= stTargetSize.u32Height;
	pms->u32SrcStride	= stTargetSize.u32Width;
	pms->u32SrcColorStride = (stTargetSize.u32Width >> 1);
	pms->u32SrcCropWidth = 0;	
	pms->u32SrcCropHeight = 0;	
	pms->u32SrcCropOffsetX = 0;	
	pms->u32SrcCropOffsetY = 0;	
	pms->u32SrcEnCrop = 0;		
	pms->enStreamType	= enPMSStreamType;	/// STREAM_TYPE_E
	pms->u32ColorFmt	= DU_FORMAT_YUV420I;
	pps->u32ColorFmt	= DU_FORMAT_YUV420I;
	p2s->u32ColorFmt	= DU_FORMAT_YUV420I;

	SAMPLE_COMM_SYS_GetPicSize(VIDEO_ENCODING_MODE_AUTO, enPPSSize, &stTargetSize);
	pps->u32SrcWidth	= stTargetSize.u32Width;
	pps->u32SrcHeight	= stTargetSize.u32Height;
	pps->u32SrcStride	= stTargetSize.u32Width;
	pps->u32SrcColorStride	= (stTargetSize.u32Width >> 1);
	pps->u32SrcCropWidth = 0;	
	pps->u32SrcCropHeight = 0;	
	pps->u32SrcCropOffsetX = 0;	
	pps->u32SrcCropOffsetY = 0;	
	pps->u32SrcEnCrop = 0;		
	pps->enStreamType	= enPPSStreamType;	/// STREAM_TYPE_E
	
	p2s->u32SrcWidth	= 1280;
	p2s->u32SrcHeight	= 720;
	p2s->u32SrcStride	= 1280;
	p2s->enStreamType	= STREAM_TYPE_UVC_CAM0;		/// STREAM_TYPE_E
	p2s->u32SrcCropWidth = 0;	
	p2s->u32SrcCropHeight = 0;	
	p2s->u32SrcCropOffsetX = 0;	
	p2s->u32SrcCropOffsetY = 0;	
	p2s->u32SrcEnCrop = 0;		

	pgi->stGUIRect.s32X			= 0;
	pgi->stGUIRect.s32Y			= 0;
	pgi->stGUIRect.u32Width		= 480;
	pgi->stGUIRect.u32Height	= 320;
	pgi->enPixelFormat			= PIXEL_FORMAT_RGB_565;	/// PIXEL_FORMAT_E

	pms->bValid = ((s32Scene & 8) ? BD_TRUE : BD_FALSE);
	pps->bValid = ((s32Scene & 4) ? BD_TRUE : BD_FALSE);
	p2s->bValid = ((s32Scene & 16) ? BD_TRUE : BD_FALSE);
	pgi->bValid = ((s32Scene & 2) ? BD_TRUE : BD_FALSE);

///	pri->en8Rotate	= ROTATE_90;		/// see below
	pri->bEnMirror	= BD_FALSE;
	pri->bEnFlip	= BD_FALSE;
	pdr->en8Rotate	= ROTATE_90;		/// ROTATE_E
	pdr->bEnMirror	= BD_FALSE;
	pdr->bEnFlip	= BD_FALSE;

	pms->u32DstX	= 0;
	pms->u32DstY	= 0;
	switch (enODev) {
	case VO_DEVICE_TYPE_LCD:
		pms->u32DstWidth	= lcd->u32Width;
		pms->u32DstHeight	= lcd->u32Height;
		pms->u32DstStride	= lcd->u32Width;
		pri->en8Rotate		= ROTATE_90;				/// ROTATE_E
		pvi->enType			= VO_DEVICE_TYPE_LCD;		/// VO_DEVICE_TYPE_E
		break;
	case VO_DEVICE_TYPE_BTO:
		pms->u32DstWidth	= bto->u32Width;
		pms->u32DstHeight	= bto->u32Height;
		pms->u32DstStride	= bto->u32Width;
		pri->en8Rotate		= ROTATE_NONE;				/// ROTATE_E
		pvi->enType			= VO_DEVICE_TYPE_BTO;		/// VO_DEVICE_TYPE_E
		break;
	case VO_DEVICE_TYPE_TV:
		pms->u32DstWidth	= 720;
		pms->u32DstHeight	= 480;
		pms->u32DstStride	= 720;
		pri->en8Rotate		= ROTATE_NONE;				/// ROTATE_E
		pvi->enType			= VO_DEVICE_TYPE_TV;		/// VO_DEVICE_TYPE_E
		break;
	case VO_DEVICE_TYPE_MEM:
		pms->u32DstWidth	= mem->u32Width;
		pms->u32DstHeight	= mem->u32Height;
		pms->u32DstStride	= mem->u32Width;			/// ????????????
		pri->en8Rotate		= ROTATE_NONE;				/// ROTATE_E
		pvi->enType			= VO_DEVICE_TYPE_MEM;		/// VO_DEVICE_TYPE_E
		break;
	case VO_DEVICE_TYPE_BTO_LCD:
		pms->u32DstWidth	= bto->u32Width;
		pms->u32DstHeight	= bto->u32Height;
		pms->u32DstStride	= bto->u32Width;
		pri->en8Rotate		= ROTATE_NONE;				/// ROTATE_E
		pvi->enType			= VO_DEVICE_TYPE_BTO_LCD;	/// VO_DEVICE_TYPE_E
		break;
	case VO_DEVICE_TYPE_TV_LCD:
		pms->u32DstWidth	= 720;
		pms->u32DstHeight	= 480;
		pms->u32DstStride	= 720;
		pri->en8Rotate		= ROTATE_NONE;				/// ROTATE_E
		pvi->enType			= VO_DEVICE_TYPE_TV_LCD;	/// VO_DEVICE_TYPE_E
		break;
	default:
		break;
	}

	pps->u32DstX		= pms->u32DstWidth  / 2;
	pps->u32DstY		= pms->u32DstHeight / 2;
	pps->u32DstWidth	= pms->u32DstWidth  / 2;
	pps->u32DstHeight	= pms->u32DstHeight / 2;
	pps->u32DstStride	= pms->u32DstStride / 2;
	p2s->u32DstX		= 0;
	p2s->u32DstY		= 0;
	p2s->u32DstWidth	= pms->u32DstWidth  / 2;
	p2s->u32DstHeight	= pms->u32DstHeight / 2;
	p2s->u32DstStride	= pms->u32DstStride / 2;

	pvi->u32Reserved = 0;

	pvc->u32ViChnSync = 0;		/// display the screen if any channel is changed
}


BD_S32 SAMPLE_COMM_VO_StartVO(BD_S32 s32Scene, VO_DEVICE_TYPE_E enODev, STREAM_TYPE_E enPMSStreamType, PIC_SIZE_E enPMSSize, STREAM_TYPE_E enPPSStreamType, PIC_SIZE_E enPPSSize)
{
    BD_S32 s32Ret;
	VO_DEVICE_CONFIG_S stVoDeviceConfig;
	VO_CONFIG_S stVoConfig;

	s32Ret = BD_MPI_VO_Create();
	if (BD_SUCCESS != s32Ret) {
		SAMPLE_PRT("Fail to create vo with 0x%x!\n", s32Ret);
		goto END_START_VO_0;
	}

	SAMPLE_COMM_VO_InitDevConfig(
		&stVoDeviceConfig, 
		s32Scene);
	s32Ret = BD_MPI_VO_SetDeviceConfig(&stVoDeviceConfig);
	if (BD_SUCCESS != s32Ret) {
		SAMPLE_PRT("Fail to set device configuration with 0x%x!\n", s32Ret);
		goto END_START_VO_1;
	}

	SAMPLE_PRT("Used memory size: 0x%08x(%d) bytes\n", stVoDeviceConfig.u32MemUsed, stVoDeviceConfig.u32MemUsed);

	SAMPLE_COMM_VO_InitConfig(&stVoConfig, 
		&stVoDeviceConfig, 
		s32Scene, 
		enODev, 
		enPMSStreamType, 
		enPMSSize, 
		enPPSStreamType, 
		enPPSSize);
	s32Ret = BD_MPI_VO_SetConfig(&stVoConfig);
	if (BD_SUCCESS != s32Ret) {
		SAMPLE_PRT("Fail to set configuration with 0x%x!\n", s32Ret);
		goto END_START_VO_1;
	}

	s32Ret = BD_MPI_VO_Enable();
	if (BD_SUCCESS != s32Ret) {
		SAMPLE_PRT("Fail to start vo with 0x%x!\n", s32Ret);
		goto END_START_VO_1;
	}

	return s32Ret;

END_START_VO_1:
	BD_MPI_VO_Destroy();
END_START_VO_0:
	return s32Ret;
}

BD_S32 SAMPLE_COMM_VO_StopVO(BD_VOID)
{
	BD_MPI_VO_Disable();
	BD_MPI_VO_Destroy();

	return BD_SUCCESS;
}


static void *SAMPLE_FbProc(void *parg)
{
	struct SAMPLE_FB_S *pstFb = (struct SAMPLE_FB_S *)parg;
	FILE *fp = BD_NULL;
	BD_S32 i;
	BD_S32 s32ImgCnt = 0;;
	BD_U8 *u8Fbptr = (BD_U8 *)pstFb->VirtAddr;
	BD_CHAR *file_name1 = "tiger_480_320_RGB565.bin";
	BD_CHAR *file_name2 = "rush_hour_480_320_RGB565.bin";
	BD_U8 *u8Mem[2];	

	u8Mem[0] = (BD_U8 *)malloc(BD1_FB_RES_X * BD1_FB_RES_Y * BD1_FB_BPP / 8);
	u8Mem[1] = (BD_U8 *)malloc(BD1_FB_RES_X * BD1_FB_RES_Y * BD1_FB_BPP / 8);
	
	fp = fopen(file_name1,"rb");
	fread(u8Mem[0], BD1_FB_RES_X * BD1_FB_RES_Y * BD1_FB_BPP / 8, 1, fp);
	fclose(fp);
	fp = fopen(file_name2,"rb");
	fread(u8Mem[1], BD1_FB_RES_X * BD1_FB_RES_Y * BD1_FB_BPP / 8, 1, fp);
	fclose(fp);


	pstFb->bStart = BD_TRUE;
	do{
		for(i=0;i<BD1_FB_RES_Y;i++){
			memcpy(u8Fbptr + (BD1_FB_RES_X_VIRT * BD1_FB_BPP /8)*i, u8Mem[s32ImgCnt&1] + (BD1_FB_RES_X * BD1_FB_BPP /8)*i, BD1_FB_RES_X * BD1_FB_BPP /8);
		}
		ioctl(pstFb->fd, FBIO_REFRESH_PM1208_FB, BD_NULL);
		BD_MPI_VO_Refresh();
		s32ImgCnt++;
		sleep(3);
	}while(pstFb->bStart);

	free(u8Mem[0]);
	free(u8Mem[1]);
	return 0;
}


BD_S32 SAMPLE_COMM_FB_StartFb(BD_VOID)
{
	BD_S32 s32Ret = BD_SUCCESS;
	struct fb_var_screeninfo vinfo;
	char *dev_file = "/dev/fb0";
	struct PM1208FB_BUFFER_S stBuf;
	struct SAMPLE_FB_S *pstFb = &gstFb;

	pstFb->fd = -1;
	pstFb->VirtAddr = BD_NULL;
	pstFb->u32FbLen = BD1_FB_RES_X_VIRT * BD1_FB_RES_Y_VIRT * BD1_FB_BPP / 8;


	if((pstFb->fd = open(dev_file, O_RDWR)) < 0){
		bd1_sdk_err("fail to open dev file(%s)\n", dev_file);
		s32Ret = BD_FAILURE;
		goto ERR_EXIT1;
	}

	s32Ret = BD_MPI_SYS_MmzAlloc(&pstFb->u32PhyAddr, (BD_VOID**)&pstFb->VirtAddr, "notused", "notused", pstFb->u32FbLen);
	if (s32Ret != BD_SUCCESS) {
		bd1_sdk_err("-- failed to allocate src heap memory\n");
		goto ERR_EXIT2;
	}

	stBuf.phyaddr = pstFb->u32PhyAddr;
	stBuf.len = pstFb->u32FbLen;
	ioctl(pstFb->fd, FBIOSET_CANVAS_BUF, &stBuf);


	ioctl(pstFb->fd, FBIOGET_VSCREENINFO, &vinfo);
	//_debug("xres: %d, yres: %d\n", vinfo.xres, vinfo.yres);

	vinfo.xres = BD1_FB_RES_X;
	vinfo.yres = BD1_FB_RES_Y;
	vinfo.xres_virtual = BD1_FB_RES_X_VIRT;
	vinfo.yres_virtual = BD1_FB_RES_Y_VIRT;
	vinfo.xoffset = 0;
	vinfo.yoffset = 0;
	vinfo.bits_per_pixel = BD1_FB_BPP;
	vinfo.grayscale = 0;
	vinfo.red = gColorFmt.red;
	vinfo.green = gColorFmt.green;
	vinfo.blue = gColorFmt.blue;
	vinfo.transp = gColorFmt.transp;

	//_debug("sample xres: %d, yres: %d\n", vinfo.xres, vinfo.yres);
	ioctl(pstFb->fd, FBIOPUT_VSCREENINFO, &vinfo);

	pthread_create(&pstFb->stFbPid, 0, SAMPLE_FbProc, pstFb);

	return s32Ret;

ERR_EXIT2:
	if(pstFb->fd >= 0){
		close(pstFb->fd);
		pstFb->fd = -1;
	}
ERR_EXIT1:
	return s32Ret;
	
}

BD_VOID SAMPLE_COMM_FB_StopFb(BD_VOID)
{
	struct SAMPLE_FB_S *pstFb = &gstFb;

	pstFb->bStart = BD_FALSE;
	pthread_join(pstFb->stFbPid, 0);

	if(pstFb->VirtAddr){
		if (BD_MPI_SYS_MmzFree(pstFb->u32PhyAddr, (BD_VOID*)pstFb->VirtAddr) != BD_SUCCESS) {
			bd1_sdk_err("failed to free heap memory\n");
		}
	}

	if(pstFb->fd >= 0){
		close(pstFb->fd);
		pstFb->fd = -1;
	}

}




#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
