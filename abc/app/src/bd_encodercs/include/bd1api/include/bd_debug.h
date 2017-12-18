#ifndef __BD_DEBUG_H_
#define __BD_DEBUG_H_

#if defined(MIN_DGB_MESG)
#define bd1_sdk_dbg(fmt, arg...) 
#define bd1_sdk_prt(fmt, arg...) 
#define bd1_sdk_err(fmt, arg...) \
		fprintf(stderr, "ERR"fmt, ## arg);
#define bd1_sdk_warn(fmt, arg...) 
#define bd1_sdk_check_1(fmt, arg...) 
#define bd1_sdk_check_2(fmt, arg...) 
#define bd1_sdk_check_3(fmt, arg...) 
#else
#define RED_COLOR   	fprintf(stderr, "\033[0;31m")        /* 0 -> normal ;  31 -> red */
#define CYAN_COLOR  	fprintf(stderr, "\033[1;36m")        /* 1 -> bold ;  36 -> cyan */
#define GREEN_COLOR 	fprintf(stderr, "\033[0;32m")       /* 4 -> underline ;  32 -> green */
#define BLUE_COLOR  	fprintf(stderr, "\033[9;34m")        /* 9 -> strike ;  34 -> blue */
#define BLACK_COLOR  	fprintf(stderr, "\033[0;30m")
#define BROWN_COLOR  	fprintf(stderr, "\033[0;33m") 
#define MAGENTA_COLOR 	fprintf(stderr, "\033[0;35m")
#define GRAY_COLOR 		fprintf(stderr, "\033[0;37m") 
#define NONE_COLOR   	fprintf(stderr, "\033[0m")        /* to flush the previous property */

#if 0
#define bd1_sdk_dbg(fmt, arg...) \
	{\
		fprintf(stderr, "[%s:%d] "fmt, __func__, __LINE__, ## arg);\
	}
#define bd1_sdk_err(fmt, arg...) \
	{\
		RED_COLOR;\
		fprintf(stderr, "ERROR :: [%s:%d] "fmt , __func__, __LINE__, ## arg);\
		NONE_COLOR;\
	}
#define bd1_sdk_warn(fmt, arg...) \
	{\
		RED_COLOR;\
		fprintf(stderr, "WARN :: [%s:%d] "fmt , __func__, __LINE__, ## arg);\
		NONE_COLOR;\
	}
#else
#define bd1_sdk_dbg(fmt, arg...) \
		fprintf(stderr, "[%s:%d] "fmt, __func__, __LINE__, ## arg);
#define bd1_sdk_prt(fmt, arg...) \
		fprintf(stderr, fmt, ## arg);
///#define bd1_sdk_prt(fmt, arg...) 
#define bd1_sdk_err(fmt, arg...) \
		fprintf(stderr, "\033[0;31m""ERROR :: [%s:%d] "fmt"\033[0m" , __func__, __LINE__, ## arg);
#define bd1_sdk_warn(fmt, arg...) \
		fprintf(stderr, "\033[0;33m""WARN :: [%s:%d] "fmt"\033[0m" , __func__, __LINE__, ## arg);
#define bd1_sdk_check_1(fmt, arg...) \
		fprintf(stderr, "\033[0;32m""CHECK :: [%s:%d] "fmt"\033[0m" , __func__, __LINE__, ## arg);
#define bd1_sdk_check_2(fmt, arg...) \
		fprintf(stderr, "\033[0;34m""CHECK :: [%s:%d] "fmt"\033[0m" , __func__, __LINE__, ## arg);
#define bd1_sdk_check_3(fmt, arg...) \
		fprintf(stderr, "\033[0;35m""CHECK :: [%s:%d] "fmt"\033[0m" , __func__, __LINE__, ## arg);
#endif

#endif

#endif