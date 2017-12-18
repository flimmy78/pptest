#ifndef __BD_COMM_IVE_H__
#define __BD_COMM_IVE_H__

#include "bd_type.h"
#include "bd_common.h"
#include "bd_errno.h"


#ifdef __cplusplus
    #if __cplusplus
    extern "C"{
    #endif
#endif 

#define IVE_MAX_HANDLE_NR 0x0FFFFFFF
#define IVE_MASK_SIZE_3X3	1
#define IVE_MASK_SIZE_5X5	3

/*
*Describes the information about stored data.
*/
typedef struct bdIVE_DATA_ATTR_S
{
    BD_U32 u32PhyAddr;  /*Physical address of the data*/
    BD_U32 u32Stride;   /*Data stride*/
}IVE_MEM_INFO_S;

/*
*Type of the input data
*The input data is classified into three types:
*SP420, SP422, and single component (including the components Y, U, V, R, G, and B)
*/
typedef enum bdIVE_SRC_FMT_E
{
    IVE_SRC_FMT_SINGLE = 0, /*Single component*/
    IVE_SRC_FMT_SP420,      /*YUV SP420*/
    IVE_SRC_FMT_SP422,      /*YUV SP422*/
    IVE_SRC_FMT_BUTT
}IVE_SRC_FMT_E;

/*
*Input data format
*Information about the data formats supported by the IVE. The IVE supports the data in the following formats:
*1 YUV SP422/SP420
*2 RGB package/planar 
*3 Single component
*/
typedef struct bdIVE_SRC_INFO_S
{
    IVE_SRC_FMT_E  enSrcFmt;
    IVE_MEM_INFO_S stSrcMem; /*Data information*/
    
    BD_U32 u32Height;   /*Data height*/
    BD_U32 u32Width;    /*Data width*/
}IVE_SRC_INFO_S;

/*
*Filter control parameters
*You need to set these parameters when using the filter operator.
*/
typedef struct bdIVE_FILTER_CTRL_S
{
	BD_S8 MaskSize;
    BD_S8 as8Mask[25];        /*Template parameter filter coefficient*/
    BD_S8 u8Norm;            /*Sum of all mask parameter values for normalization*/
}IVE_FILTER_CTRL_S;


/*Invalid device ID*/
#define BD_ERR_IVE_INVALID_DEVID     BD_DEF_ERR(BD_ID_IVE, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_DEVID)
/*At least one parameter is illegal. For example, an illegal enumeration value exists.*/
#define BD_ERR_IVE_ILLEGAL_PARAM     BD_DEF_ERR(BD_ID_IVE, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
/*A null point is used.*/
#define BD_ERR_IVE_NULL_PTR          BD_DEF_ERR(BD_ID_IVE, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
/*Try to enable or initialize the system, device, or channel before configuring attributes.*/
#define BD_ERR_IVE_NOT_CONFIG        BD_DEF_ERR(BD_ID_IVE, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_CONFIG)
/*The operation is not supported currently.*/
#define BD_ERR_IVE_NOT_SURPPORT      BD_DEF_ERR(BD_ID_IVE, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
/*The operation, changing static attributes for example, is not permitted.*/
#define BD_ERR_IVE_NOT_PERM          BD_DEF_ERR(BD_ID_IVE, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)
/*A failure caused by the malloc memory occurs.*/
#define BD_ERR_IVE_NOMEM             BD_DEF_ERR(BD_ID_IVE, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
/*A failure caused by the malloc buffer occurs.*/
#define BD_ERR_IVE_NOBUF             BD_DEF_ERR(BD_ID_IVE, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF)
/*The buffer is empty.*/
#define BD_ERR_IVE_BUF_EMPTY         BD_DEF_ERR(BD_ID_IVE, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY)
/*No buffer is provided for storing new data.*/
#define BD_ERR_IVE_BUF_FULL          BD_DEF_ERR(BD_ID_IVE, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL)
/*The source address or target address is incorrect during the operations such as calling copy_from_user or copy_to_user.*/
#define BD_ERR_IVE_BADADDR           BD_DEF_ERR(BD_ID_IVE, EN_ERR_LEVEL_ERROR, EN_ERR_BADADDR)
/*The resource is busy during the operations such as destroying a VENC channel without deregistering it.*/
#define BD_ERR_IVE_BUSY              BD_DEF_ERR(BD_ID_IVE, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)

/*The system is not ready because it may be not initialized or loaded.
 *The error code is returned when a device file fails to be opened.
 */
#define BD_ERR_IVE_NOTREADY          BD_DEF_ERR(BD_ID_IVE, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)


#ifdef __cplusplus
    #if __cplusplus
}
    #endif
#endif
#endif/*__HI_COMM_IVE_H__*/



