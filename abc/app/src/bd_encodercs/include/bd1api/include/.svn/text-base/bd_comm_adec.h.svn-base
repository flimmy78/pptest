#ifndef __BD_COMM_ADEC_H__
#define __BD_COMM_ADEC_H__

#include "bd_type.h"
#include "bd_common.h"
#include "bd_comm_aio.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef struct hiADEC_ATTR_G726_S
{
    G726_BPS_E enG726bps; 
}ADEC_ATTR_G726_S;

typedef struct bdADEC_CH_ATTR_S 
{ 
    PAYLOAD_TYPE_E enType; 
	BD_U32 		   u32PtNumPerFrm;
    BD_U32         u32BufSize;  /*buf size[2~MAX_AUDIO_FRAME_NUM]*/
//    ADEC_MODE_E    enMode;      /*decode mode*/
    BD_VOID       *pValue; 
}ADEC_CHN_ATTR_S;

typedef enum bdEN_ADEC_ERR_CODE_E
{
    ADEC_ERR_DECODER_ERR     = 64,   
    ADEC_ERR_BUF_LACK,   
    ADEC_ERR_VOICE_DEC_TYPE,   
    ADEC_ERR_VOICE_DEC_FRAMESIZE,       
    ADEC_ERR_VOICE_DEC_FRAMETYPE,     
    ADEC_ERR_VOICE_INVALID_DEVICE,
    ADEC_ERR_VOICE_INVALID_INBUF,
    ADEC_ERR_VOICE_INVALID_OUTBUF,
    ADEC_ERR_VOICE_TRANS_DEVICE,
    ADEC_ERR_VOICE_TRANS_TYPE,

} EN_ADEC_ERR_CODE_E;


/* invlalid device ID */
#define BD_ERR_ADEC_INVALID_DEVID     BD_DEF_ERR(BD_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_DEVID)
/* invlalid channel ID */
#define BD_ERR_ADEC_INVALID_CHNID     BD_DEF_ERR(BD_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID)
/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define BD_ERR_ADEC_ILLEGAL_PARAM     BD_DEF_ERR(BD_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
/* channel exists */
#define BD_ERR_ADEC_EXIST             BD_DEF_ERR(BD_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_EXIST)
/* channel unexists */
#define BD_ERR_ADEC_UNEXIST           BD_DEF_ERR(BD_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST)
/* using a NULL point */
#define BD_ERR_ADEC_NULL_PTR          BD_DEF_ERR(BD_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
/* try to enable or initialize system,device or channel, before configing attribute */
#define BD_ERR_ADEC_NOT_CONFIG        BD_DEF_ERR(BD_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_CONFIG)
/* operation is not supported by NOW */
#define BD_ERR_ADEC_NOT_SUPPORT       BD_DEF_ERR(BD_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
/* operation is not permitted ,eg, try to change stati attribute */
#define BD_ERR_ADEC_NOT_PERM          BD_DEF_ERR(BD_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)
/* failure caused by malloc memory */
#define BD_ERR_ADEC_NOMEM             BD_DEF_ERR(BD_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
/* failure caused by malloc buffer */
#define BD_ERR_ADEC_NOBUF             BD_DEF_ERR(BD_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF)
/* no data in buffer */
#define BD_ERR_ADEC_BUF_EMPTY         BD_DEF_ERR(BD_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY)
/* no buffer for new data */
#define BD_ERR_ADEC_BUF_FULL          BD_DEF_ERR(BD_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL)
/* system is not ready,had not initialed or loaded*/
#define BD_ERR_ADEC_SYS_NOTREADY      BD_DEF_ERR(BD_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)
/* decoder internal err */ 
#define BD_ERR_ADEC_DECODER_ERR       BD_DEF_ERR(BD_ID_ADEC, EN_ERR_LEVEL_ERROR, ADEC_ERR_DECODER_ERR)
/* input buffer not enough to decode one frame */ 
#define BD_ERR_ADEC_BUF_LACK          BD_DEF_ERR(BD_ID_ADEC, EN_ERR_LEVEL_ERROR, ADEC_ERR_BUF_LACK)



#define BD_ERR_ADEC_DEC_TYPE		  BD_DEF_ERR(BD_ID_ADEC, EN_ERR_LEVEL_ERROR, ADEC_ERR_VOICE_DEC_TYPE)
#define BD_ERR_ADEC_DEC_FRAMESIZE	  BD_DEF_ERR(BD_ID_ADEC, EN_ERR_LEVEL_ERROR, ADEC_ERR_VOICE_DEC_FRAMESIZE)
#define BD_ERR_ADEC_DEC_FRAMETYPE	  BD_DEF_ERR(BD_ID_ADEC, EN_ERR_LEVEL_ERROR, ADEC_ERR_VOICE_DEC_FRAMETYPE)
#define BD_ERR_ADEC_INVALID_DEVICE	  BD_DEF_ERR(BD_ID_ADEC, EN_ERR_LEVEL_ERROR, ADEC_ERR_VOICE_INVALID_DEVICE)
#define BD_ERR_ADEC_INVALID_INBUF	  BD_DEF_ERR(BD_ID_ADEC, EN_ERR_LEVEL_ERROR, ADEC_ERR_VOICE_INVALID_INBUF)
#define BD_ERR_ADEC_INVALID_OUTBUF	  BD_DEF_ERR(BD_ID_ADEC, EN_ERR_LEVEL_ERROR, ADEC_ERR_VOICE_INVALID_OUTBUF)
#define BD_ERR_ADEC_TRANS_DEVICE	  BD_DEF_ERR(BD_ID_ADEC, EN_ERR_LEVEL_ERROR, ADEC_ERR_VOICE_TRANS_DEVICE)
#define BD_ERR_ADEC_TRANS_TYPE		  BD_DEF_ERR(BD_ID_ADEC, EN_ERR_LEVEL_ERROR, ADEC_ERR_VOICE_TRANS_TYPE)





#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif
