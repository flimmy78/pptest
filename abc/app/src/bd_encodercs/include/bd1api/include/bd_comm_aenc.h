#ifndef __BD_COMM_AENC_H__
#define __BD_COMM_AENC_H__

#include "bd_common.h"
#include "bd_comm_aio.h"
#include "bd_type.h"
#include "bd_errno.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


typedef struct bdAENC_ATTR_G726_S
{
    G726_BPS_E enG726bps; 
}AENC_ATTR_G726_S;

typedef struct bdAENC_CHN_ATTR_S 
{ 
    PAYLOAD_TYPE_E      enType;         /*payload type ()*/ 
	BD_U32 				u32PtNumPerFrm;
    BD_U32              u32BufSize;     /*buf size [2~MAX_AUDIO_FRAME_NUM]*/
    BD_VOID             *pValue;  		/*point to attribute of definite audio encoder*/       
}AENC_CHN_ATTR_S;


/* invlalid device ID */
#define BD_ERR_AENC_INVALID_DEVID     BD_DEF_ERR(BD_ID_AENC, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_DEVID)
/* invlalid channel ID */
#define BD_ERR_AENC_INVALID_CHNID     BD_DEF_ERR(BD_ID_AENC, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID)
/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define BD_ERR_AENC_ILLEGAL_PARAM     BD_DEF_ERR(BD_ID_AENC, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
/* channel exists */
#define BD_ERR_AENC_EXIST             BD_DEF_ERR(BD_ID_AENC, EN_ERR_LEVEL_ERROR, EN_ERR_EXIST)
/* channel unexists */
#define BD_ERR_AENC_UNEXIST           BD_DEF_ERR(BD_ID_AENC, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST)
/* using a NULL point */
#define BD_ERR_AENC_NULL_PTR          BD_DEF_ERR(BD_ID_AENC, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
/* try to enable or initialize system,device or channel, before configing attribute */
#define BD_ERR_AENC_NOT_CONFIG        BD_DEF_ERR(BD_ID_AENC, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_CONFIG)
/* operation is not supported by NOW */
#define BD_ERR_AENC_NOT_SUPPORT       BD_DEF_ERR(BD_ID_AENC, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
/* operation is not permitted ,eg, try to change static attribute */
#define BD_ERR_AENC_NOT_PERM          BD_DEF_ERR(BD_ID_AENC, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)
/* failure caused by malloc memory */
#define BD_ERR_AENC_NOMEM             BD_DEF_ERR(BD_ID_AENC, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
/* failure caused by malloc buffer */
#define BD_ERR_AENC_NOBUF             BD_DEF_ERR(BD_ID_AENC, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF)
/* no data in buffer */
#define BD_ERR_AENC_BUF_EMPTY         BD_DEF_ERR(BD_ID_AENC, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY)
/* no buffer for new data */
#define BD_ERR_AENC_BUF_FULL          BD_DEF_ERR(BD_ID_AENC, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL)
/* system is not ready,had not initialed or loaded*/
#define BD_ERR_AENC_SYS_NOTREADY      BD_DEF_ERR(BD_ID_AENC, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)
/* encoder internal err */ 
#define BD_ERR_AENC_ENCODER_ERR       BD_DEF_ERR(BD_ID_AENC, EN_ERR_LEVEL_ERROR, ADEC_ERR_ENCODER_ERR)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif

