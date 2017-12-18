#ifndef __BD_COMM_AIO_H__
#define __BD_COMM_AIO_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define MAX_AUDIO_FRAME_NUM    50       /*max count of audio frame in Buffer */

typedef enum bdAUDIO_SAMPLE_RATE_E 
{ 
    AUDIO_SAMPLE_RATE_8000   = 8000,    /* 8K samplerate*/
    AUDIO_SAMPLE_RATE_12000  = 12000,   /* 12K samplerate*/    
    AUDIO_SAMPLE_RATE_11025  = 11025,   /* 11.025K samplerate*/
    AUDIO_SAMPLE_RATE_16000  = 16000,   /* 16K samplerate*/
    AUDIO_SAMPLE_RATE_22050  = 22050,   /* 22.050K samplerate*/
    AUDIO_SAMPLE_RATE_24000  = 24000,   /* 24K samplerate*/
    AUDIO_SAMPLE_RATE_32000  = 32000,   /* 32K samplerate*/
    AUDIO_SAMPLE_RATE_44100  = 44100,   /* 44.1K samplerate*/
    AUDIO_SAMPLE_RATE_48000  = 48000,   /* 48K samplerate*/
    AUDIO_SAMPLE_RATE_BUTT,
} AUDIO_SAMPLE_RATE_E; 


typedef enum bdAUDIO_BIT_WIDTH_E
{

#if 0
    AUDIO_BIT_WIDTH_8   = 0,   /* 8bit width */
    AUDIO_BIT_WIDTH_16  = 1,   /* 16bit width*/
    AUDIO_BIT_WIDTH_32  = 2,   /* 32bit width*/
#endif
    AUDIO_BIT_WIDTH_16  = 0,   /* 16bit width*/
    AUDIO_BIT_WIDTH_24  = 1,   /* 24bit width*/
    AUDIO_BIT_WIDTH_32  = 2,   /* 32bit width*/
    AUDIO_BIT_WIDTH_BUTT,
    
} AUDIO_BIT_WIDTH_E;

typedef enum bdAIO_MODE_E
{
    AIO_MODE_I2S_MASTER  = 0,   /* SIO I2S master mode */
    AIO_MODE_I2S_SLAVE,         /* SIO I2S slave mode */
    AIO_MODE_BUTT    
} AIO_MODE_E;

typedef enum bdAIO_SOUND_MODE_E
{
    AUDIO_SOUND_MODE_MONO   =0,/*mono*/
    AUDIO_SOUND_MODE_STEREO =1,/*stereo*/
    AUDIO_SOUND_MODE_BUTT    
} AUDIO_SOUND_MODE_E;

typedef enum bdG726_BPS_E
{
    G726_16K = 0,       /* G726 16kbps, see RFC3551.txt  4.5.4 G726-16 */ 
    G726_32K,           /* G726 32kbps, see RFC3551.txt  4.5.4 G726-32 */
    G726_BUTT,
} G726_BPS_E;

typedef struct bdAIO_ATTR_S
{
    AUDIO_SAMPLE_RATE_E enSamplerate;   /* sample rate*/
    AUDIO_BIT_WIDTH_E   enBitwidth;     /* bitwidth*/
    AIO_MODE_E          enWorkmode;     /* master or slave mode*/
    AUDIO_SOUND_MODE_E  enSoundmode;    /* mono or stereo*/
    BD_U32              u32FrmNum;      /* frame num in buf[2,MAX_AUDIO_FRAME_NUM]*/
    BD_U32              u32PtNumPerFrm; /* point num per frame (80/160/240/320/480/1024/2048)*/
    BD_U32              u32ChnCnt;      /* channle number on FS, valid value:2  */
} AIO_ATTR_S;


typedef struct bdAUDIO_FRAME_S
{ 
    AUDIO_BIT_WIDTH_E   enBitwidth;     /*audio frame bitwidth*/
    AUDIO_SOUND_MODE_E  enSoundmode;    /*audio frame momo or stereo mode*/
    BD_VOID *pVirAddr[2];
    BD_U32  u32PhyAddr[2];
    BD_U64  u64TimeStamp;                /*audio frame timestamp*/
    BD_U32  u32Seq;                      /*audio frame seq*/
    BD_U32  u32Len;                      /*data lenth per channel in frame*/
} AUDIO_FRAME_S; 

typedef struct bdAEC_FRAME_S
{
    AUDIO_FRAME_S   stRefFrame;    /* AEC reference audio frame */
    BD_BOOL         bValid;        /* whether frame is valid */
} AEC_FRAME_S;

typedef struct bdAI_CHN_PARAM_S
{
    BD_U32 u32UsrFrmDepth;
} AI_CHN_PARAM_S;


typedef struct bdAO_CHN_STATE_S
{
    BD_U32                  u32ChnTotalNum;    /* total number of channel buffer */
    BD_U32                  u32ChnFreeNum;     /* free number of channel buffer */
    BD_U32                  u32ChnBusyNum;     /* busy number of channel buffer */
} AO_CHN_STATE_S;


typedef enum bdAUDIO_FADE_RATE_E 
{ 
    AUDIO_FADE_RATE_1	= 0,
    AUDIO_FADE_RATE_2	= 1,
    AUDIO_FADE_RATE_4	= 2,
    AUDIO_FADE_RATE_8   = 3,
    AUDIO_FADE_RATE_16  = 4,
    AUDIO_FADE_RATE_32  = 5,
    AUDIO_FADE_RATE_64  = 6,
    AUDIO_FADE_RATE_128 = 7,	
    
    AUDIO_FADE_RATE_BUTT
} AUDIO_FADE_RATE_E; 


typedef struct bdAUDIO_STREAM_S 
{ 
    BD_U8 *pStream[2];         /* the virtual address of stream */ 
	AUDIO_SOUND_MODE_E  enSoundmode;    /*audio frame momo or stereo mode*/
//    BD_U32 u32PhyAddr;      /* the physics address of stream */
    BD_U32 u32Len;          /* stream lenth, by bytes */
    BD_U64 u64TimeStamp;    /* frame time stamp*/
    BD_U32 u32Seq;          /* frame seq,if stream is not a valid frame,u32Seq is 0*/
} AUDIO_STREAM_S;


/* invlalid device ID */
#define BD_ERR_AI_INVALID_DEVID     BD_DEF_ERR(BD_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_DEVID)
/* invlalid channel ID */
#define BD_ERR_AI_INVALID_CHNID     BD_DEF_ERR(BD_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID)
/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define BD_ERR_AI_ILLEGAL_PARAM     BD_DEF_ERR(BD_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
/* using a NULL point */
#define BD_ERR_AI_NULL_PTR          BD_DEF_ERR(BD_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
/* try to enable or initialize system,device or channel, before configing attribute */
#define BD_ERR_AI_NOT_CONFIG        BD_DEF_ERR(BD_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_CONFIG)
/* operation is not supported by NOW */
#define BD_ERR_AI_NOT_SUPPORT       BD_DEF_ERR(BD_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
/* operation is not permitted ,eg, try to change stati attribute */
#define BD_ERR_AI_NOT_PERM          BD_DEF_ERR(BD_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)
/* the devide is not enabled  */
#define BD_ERR_AI_NOT_ENABLED       BD_DEF_ERR(BD_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST)
/* failure caused by malloc memory */
#define BD_ERR_AI_NOMEM             BD_DEF_ERR(BD_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
/* failure caused by malloc buffer */
#define BD_ERR_AI_NOBUF             BD_DEF_ERR(BD_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF)
/* no data in buffer */
#define BD_ERR_AI_BUF_EMPTY         BD_DEF_ERR(BD_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY)
/* no buffer for new data */
#define BD_ERR_AI_BUF_FULL          BD_DEF_ERR(BD_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL)
/* system is not ready,had not initialed or loaded*/
#define BD_ERR_AI_SYS_NOTREADY      BD_DEF_ERR(BD_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)

#define BD_ERR_AI_BUSY              BD_DEF_ERR(BD_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)

/* invlalid device ID */
#define BD_ERR_AO_INVALID_DEVID     BD_DEF_ERR(BD_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_DEVID)
/* invlalid channel ID */
#define BD_ERR_AO_INVALID_CHNID     BD_DEF_ERR(BD_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID)
/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define BD_ERR_AO_ILLEGAL_PARAM     BD_DEF_ERR(BD_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
/* using a NULL point */
#define BD_ERR_AO_NULL_PTR          BD_DEF_ERR(BD_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
/* try to enable or initialize system,device or channel, before configing attribute */
#define BD_ERR_AO_NOT_CONFIG        BD_DEF_ERR(BD_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_CONFIG)
/* operation is not supported by NOW */
#define BD_ERR_AO_NOT_SUPPORT       BD_DEF_ERR(BD_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
/* operation is not permitted ,eg, try to change stati attribute */
#define BD_ERR_AO_NOT_PERM          BD_DEF_ERR(BD_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)
/* the devide is not enabled  */
#define BD_ERR_AO_NOT_ENABLED       BD_DEF_ERR(BD_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST)
/* failure caused by malloc memory */
#define BD_ERR_AO_NOMEM             BD_DEF_ERR(BD_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
/* failure caused by malloc buffer */
#define BD_ERR_AO_NOBUF             BD_DEF_ERR(BD_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF)
/* no data in buffer */
#define BD_ERR_AO_BUF_EMPTY         BD_DEF_ERR(BD_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY)
/* no buffer for new data */
#define BD_ERR_AO_BUF_FULL          BD_DEF_ERR(BD_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL)
/* system is not ready,had not initialed or loaded*/
#define BD_ERR_AO_SYS_NOTREADY      BD_DEF_ERR(BD_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)

#define BD_ERR_AO_BUSY              BD_DEF_ERR(BD_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif


