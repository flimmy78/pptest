
#ifndef __BD_ENCODER_H__
#define __BD_ENCODER_H__

#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include "sample_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

//MAX channel number
#define MAX_CHANNEL_NUM 3

//Thread run or stop
#define THREAD_RUN	0
#define THREAD_STOP	1


/*! Bitrate control enumeration  */
typedef enum {
  BD_CBR = 0,
  BD_VBR,
  BD_FIXQP
}BDBitrateControl;


/*! Encoding된 영상을 shared memory로 보내기 위한 정보를 담고 있는 구조체.
*
* @note 직접 편집하지 않고 bd_init_shared_mem()으로 설정한다.
*/
typedef struct _tag_shared_mem {
   int channel_id;
   const char	*shm_name; /*!< Shared memory의 Path. ex)"/tmp/.bd_shared_mem_video_0" */
   uint32_t	shm_size;  /*!< Shared memory의 크기(byte) */
   void* shared_mem_handle;
}BD_SHARED_MEM;


/*! Encoding된 영상을 UDP stream으로 보내기 위한 정보를 담고 있는 구조체.
*
* @note 직접 편집하지 않고 bd_UDP_socket()으로 설정한다.
*/
typedef struct _tag_udp_socket{
   int 			udp_socket_port; /*!< Port번호  */
   struct sockaddr_in	*client_addr; /*!< 보내줄 IP address */
}BD_UDP_SOCKET;


/*! 영상 Encoding처리를 위한 전반적인 정보를 담고 있는 구조체.
*
* @note 직접 편집하지 않고 bd_init_context()로 설정한다.
*/

typedef struct _tag_app_context {

    //Video input source configuation
    VI_CHN viChn; //Video channel number.
    SAMPLE_VI_CONFIG_S stViConfig; //Video input configuration
    BD_S32 chnNum; //Number or channel.

    //Encoder configuration.
    VI_DEV viDev;
    VENC_GRP vencGrp;
    VENC_CHN vencChn;
    PAYLOAD_TYPE_E enPayLoad; //H.264 or other encoding
    BDBitrateControl bitrateControl; //Bitrate control

    //Capture thread
    pthread_t	capture_thread_id;
    int 		run_thread;
    pthread_t	mv_thread_id;

    //Shared memory
    BD_SHARED_MEM   *shared_mem;
    void            *pfs;  // Frame Sender through shared memory

    //UDP socket
    BD_UDP_SOCKET	*udp_socket;

} BD_APP_CONTEXT;



/**
 *  @brief  Encoder H/W를 초기화 한다. 제일 먼저 불러줘야 한다.
 *
 *  @return 아무 이상이 없으면 BD_SUCCESS를 호출한다.
*/
BD_S32 bd_init_hw();


/**
 *  @brief Shared memory정보를 담은 구조체를 초기화한다.
 *
 *  @param[in] channel_id Channel의 ID를 담고 있는 숫자
 *  @param[in] shared_memory_name Shared memory의 이름
 *  @param[in] size_in_byte Shared memory의 크기 (Byte)
 *  @param[out] sharedMem   모든 설정 데이타들을 가지고 있는 구조체의 pointer
 *  @return 아무 이상이 없으면 BD_SUCCESS를 호출한다.
*/
BD_S32 bd_init_shared_mem(
       int channel_id,
       const char *shared_memory_name,
       int size_in_byte,
       BD_SHARED_MEM *sharedMem
);


/**
 *  @brief UDP 전송에 대한 정보를 담은 구조체를 초기화 한다.
 *
 *  @param[in] address UDP IP address. 보통은 자기 자신 ("127.0.0.1")으로 설정한다.
 *  @param[in] port UDP port.
 *  @param[out]  BDUdpSocket   모든 설정 데이타들을 가지고 있는 구조체의 pointer.
 *  @return 아무 이상이 없으면 BD_SUCCESS를 호출한다.
*/
BD_S32 bd_init_UDP_socket(
       const char  *address,
       int port,
       BD_UDP_SOCKET *udp_socket
);


/**
*  @brief Video input 정보를 한꺼번에 설정한다.
*
*  @param[in] app_contexts  모든 설정 데이타들을 가지고 있는 구조체의 pointer들을 담은 Array.
*  @param[in] num_of_contexts app_context의 길이
*  @return 아무 이상이 없으면 BD_SUCCESS를 호출한다.
*/

BD_S32 bd_open_video_input_devices(
		BD_APP_CONTEXT *app_contexts,
    int num_of_contexts
	);


/**
*  @brief Video input 정보를 설정한다.
*
*  @param[in] channel_index 해당 Channel의 index
*  @param[in] enSize Video device의 크기: PIC_HD1088, PIC_HD720, PIC_VGA
*  @param[out]  app_context   모든 설정 데이타들을 가지고 있는 구조체의 pointer .
*  @return 아무 이상이 없으면 BD_SUCCESS를 호출한다.
*/
BD_S32 bd_conf_video_input(
    int channel_index,
    PIC_SIZE_E enSize,
    BD_APP_CONTEXT *app_context
);



/**
*  @brief Video input을 연다.
*
*  @param[in] app_context1 Video Input설정을 담고 있는 구조체. 없으면 NULL을 넣어준다.
*  @param[in] app_context2 Video Input설정을 담고 있는 구조체. 없으면 NULL을 넣어준다.
*  @param[in] app_context3 Video Input설정을 담고 있는 구조체. 없으면 NULL을 넣어준다.
*  @return 아무 이상이 없으면 BD_SUCCESS를 호출한다.
*  @note 최대 3개의 Video input을 받을 수 있다.
*/
BD_S32 bd_open_video_input_device(
    BD_APP_CONTEXT *app_context1,
    BD_APP_CONTEXT *app_context2,
    BD_APP_CONTEXT *app_context3
);


/**
*  @brief 열린 device에서 Capture를 시작한다.
*
*  @param[in] app_context Video Input설정을 담고 있는 구조체.
*  @return 아무 이상이 없으면 BD_SUCCESS를 호출한다.
*/
BD_S32 bd_start_to_capture(
    BD_APP_CONTEXT *app_context
);



/**
*  @brief H/W에 있는 Encoder를 설정한다.
*
*  @param[in] codec 어떤 Codec을 이용할지 결정: PT_H264 / PT_MJPEG / PT_JPEG
*  @param[in] bitrate_control bitrate contron을 어떻게 할지 결정:  BD_CBR / BD_VBR / BD_FIXQP
*  @param[in] shared_mem Encoding 된 동영상 frame을 shared memory로 보내기 위한 설정들을 담고 있는 구조체의 포인터.
*  @param[in] udp_socket Encoding 된 동영상 frame을 UDP socket으로 보내기 위한 설정들을 담고 있는 구조체의 포인터.
*  @param[out] app_context Video Input설정을 담고 있는 구조체.
*  @return 아무 이상이 없으면 BD_SUCCESS를 호출한다.
*/
BD_S32 bd_init_encoder(
    PAYLOAD_TYPE_E codec,
    BDBitrateControl bitrate_control,
    BD_SHARED_MEM *shared_mem,
    BD_UDP_SOCKET *udp_socket,
    BD_APP_CONTEXT *app_context
);



/**
*  @brief Capture하고 encoding해서 shared memory에 전달하는 thread를 실행한다.
*
*  @param[in]  app_contexts  	모든 설정 데이타들을 가지고 있는 구조체.
*  @return 아무 이상이 없으면 BD_SUCCESS를 호출한다.
*/
BD_S32 bd_run_encoder(
    BD_APP_CONTEXT *app_context
);


/**
 *  @brief Width/height resolution 값을 설정한다.
 *
 *  @param[in] index  해당 context의 channel index.
 *  @param[in] width  동영상의 가로 Pixel크기
 *  @param[in] height  동영상의 세로 Pixel크기
 *  @param[in out]  app_contexts  	모든 설정 데이타들을 가지고 있는 구조체들의 배열 .
 *  @return 아무 이상이 없으면 BD_SUCCESS를 돌려준다.
 */

BD_S32 bd_set_resolution(
		int index,
		int width,
		int height,
		BD_APP_CONTEXT  *app_context);


/**
 *  @brief 현재 Channel의 bitrate 값을 읽어온다.
 *
 *  @param[in]  app_contexts  	모든 설정 데이타들을 가지고 있는 구조체들의 배열 .
 *  @return 아무 이상이 없으면 해당 context의 bitrate값을 넘겨준다.
 */

int bd_get_bitrate(
		BD_APP_CONTEXT  *app_context
);

/**
 *  @brief 현재 Channel의 bitrate 값을 설정한다.
 *
 *  @param[in]  bitrate_value 설정하고 싶은 bitrate 값.
 *  @param[in]  app_contexts  모든 설정 데이타들을 가지고 있는 구조체들의 배열.
 *  @return 아무 이상이 없으면 해당 context의 현재 설정된 bitrate값을 넘겨준다. 문제가 있을때는 -1 을 되돌려준다.
 */

int bd_set_bitrate(
	int bitrate_value,
	BD_APP_CONTEXT  *app_context
);

/**
 *  @brief 현재 Channel의 framerate 값을 읽어온다.
 *
 *  @param[in]  app_contexts  	모든 설정 데이타들을 가지고 있는 구조체들의 배열 .
 *  @return 해당 context가 설정하고 있는 framerate 값을 넘겨준다.
 */
BD_FR32 bd_get_framerate(
		BD_APP_CONTEXT  *app_context
);


/**
 *  @brief 현재 Channel의 framerate 값을 설정한다.
 *
 *  @param[in]  framerate       설정할 framerate 값.
 *  @param[in]  channel         변경할 channel 값.
 *  @param[in]  video_size      설정 된 video size값.
 *  @param[in]  app_contexts  	모든 설정 데이타들을 가지고 있는 구조체들의 배열.
 *  @param[in]  num_of_contexts 전체 app contexte들의 개수
 *  @return  아무 이상이 없으면 BD_SUCCESS를 돌려준다.
 */
BD_S32 bd_set_framerate(
  BD_FR32 framerate,
  int channel,
  PIC_SIZE_E video_size[],
  BD_APP_CONTEXT  *app_contexts,
  int num_of_contexts
);



/**
 *  @brief 현재 Channel의 GOP 값을 읽어온다.
 *
 *  @param[in]  app_contexts  	모든 설정 데이타들을 가지고 있는 구조체들의 배열 .
 *  @return 아무 이상이 없으면 해당 context의 bitrate값을 넘겨준다.
 */

int bd_get_GOP(
		BD_APP_CONTEXT  *app_context
);

/**
 *  @brief 현재 Channel의 GOP 값을 설정한다.
 *
 *  @param[in]  GOPval          설정할 GOP 값.
 *  @param[in]  channel         변경할 channel 값.
 *  @param[in]  video_size      설정 된 video size값.
 *  @param[in]  app_contexts  	모든 설정 데이타들을 가지고 있는 구조체들의 배열.
 *  @param[in]  num_of_contexts 전체 app contexte들의 개수
 *  @return  아무 이상이 없으면 BD_SUCCESS를 돌려준다.
 */
BD_S32 bd_set_GOP(
  int GOPval,
  int channel,
  PIC_SIZE_E video_size[],
  BD_APP_CONTEXT  *app_contexts,
  int num_of_contexts
);



/**
*  @brief Video input과 encoding을 중지한다.
*
*  @param[in]  app_contexts  모든 설정 데이타들을 가지고 있는 구조체들의 배열 .
*  @return 아무 이상이 없으면 BD_SUCCESS를 호출한다.
*/
BD_S32 bd_stop_video_input_and_encoding(
    BD_APP_CONTEXT  *app_context
);


/**
*  @brief Hardware  H/W를 종료한다.
*/
void bd_terminate_hw();






BD_S32 SAMPLE_AUDIO_AiAenc(BD_VOID);








#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif /* End of #ifndef __BD_ENCODER_H__ */
