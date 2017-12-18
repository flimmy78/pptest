#ifndef __BD_FONTDRAW_H__
#define __BD_FONTDRAW_H__

#include "bd_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


#define MAX_FONT_SIZE_NUM		3

#define FT_ATTR_SMALL		0
#define FT_ATTR_MIDIUM		1
#define FT_ATTR_LARGE		2

#define FT_ATTR_NORMAL		0
#define FT_ATTR_OUTLINE		1


int drawlib_initialize(char* ft_name, unsigned int ft_size, unsigned int ft_outline);
int drawlib_getfontsizenum(void);
int drawlib_setfontsize(unsigned int ft_size);
int drawlib_getfontsize(unsigned int ft_size);
int drawlib_setfontoutline(int enable);
int drawlib_terminate(void);
int drawlib_string(char *str, unsigned char* bg_buf, unsigned int bg_width, unsigned bg_height, unsigned int fg_color, unsigned int bg_color);
int drawlib_string_gray(char *str, unsigned char* bg_buf, unsigned int bg_width, unsigned bg_height);
int drawlib_getstringsize(const char* str, unsigned int* str_width, unsigned int* str_height);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __BD_MATH_H__ */

