/**
  * @file  main.c
  * @brief Encoder process??main
  *
  *
  * Copyright (C) PixelPlus - All Rights Reserved
  * Written by Jinho Yoo, Mar 2015
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
//#include <cJSON.h>

#include "bd_encoder.h"



// my commnet

#define CONTEXT_CNT     2

#define ENCODER_PORT 10416



//Settings of UDP socket.
static BD_UDP_SOCKET g_UDPsocket[CONTEXT_CNT];

//Settings of shared memory
static BD_SHARED_MEM g_shared_memory[CONTEXT_CNT];

//Application context of encoder.
static BD_APP_CONTEXT g_app_context[CONTEXT_CNT];

PIC_SIZE_E g_video_size[]={PIC_HD1088,PIC_HD720,PIC_VGA};


//Function to run encoder. Implemented in run_encoder.c.
int run_encoder(
    BD_UDP_SOCKET *sockets,
    BD_SHARED_MEM *shared_mems,
    BD_APP_CONTEXT *app_contexts,
    char *udp_ip[],
    int udp_port[],
    PIC_SIZE_E video_size[],
    int channel_count
  );



int main ( int argc, char *argv[] ){

    //E_Result res;
    BD_S32 ret;
    printf("\nEncoder begins .....\n");


    //Initialize  encoder H/W.
    ret = bd_init_hw();
    if( ret != BD_SUCCESS){
        printf( "Error! Error code = %d", ret);
        return -1;
    }

    //Run encoder thread.
    char* udp_ip[]={"127.0.0.1","127.0.0.1","127.0.0.1"};
    int udp_port[]={19001,19002,19003};

    if( 0!=
          run_encoder(
            g_UDPsocket,
            NULL,
            g_app_context,
            udp_ip,
            udp_port,
            g_video_size,
            CONTEXT_CNT) ){
        return -1;
    }


    while(1){

	usleep(100);

    }

    return 0;
}

int run_encoder(
  BD_UDP_SOCKET *sockets,
  BD_SHARED_MEM *shared_mems,
  BD_APP_CONTEXT *app_contexts,
  char *udp_ip[],
  int udp_port[],
  PIC_SIZE_E video_size[],
  int channel_count){

    BD_S32 ret=0;
    int i=0;

    //Open UDP port.
    ret = BD_FAILURE;
    for( i=0; i<channel_count; ++i){

      ret
        = bd_init_UDP_socket(
              udp_ip[i],
              udp_port[i],
              &sockets[i]
      );

      if( BD_SUCCESS != ret ){
          printf("Error: Fail to open UDP port.\n");
          return -1;
      }

    }


    //Configure video input.
    ret = BD_FAILURE;
    for( i=0; i<channel_count; ++i){
      ret
        = bd_conf_video_input(
            i,
            video_size[i], //{PIC_HD1088, PIC_HD720, PIC_VGA}
            &app_contexts[i]
          );

      if( BD_SUCCESS != ret ){
          printf("Error: Fail to configure video input.\n");
          return -1;
      }

      //To use new shared memory or UDP socket.
      app_contexts[i].shared_mem = NULL;
      app_contexts[i].udp_socket = NULL;

    }





    //Open video input.
    ret = BD_FAILURE;
    ret
      = bd_open_video_input_devices(
          app_contexts,
          channel_count);
    if( BD_SUCCESS != ret ){
        printf("Error: Fail to configure video input.");
        return -1;
    }

    //Start to capture
    for (i = 0; i < channel_count; ++i) {
      ret = BD_FAILURE;
      ret = bd_start_to_capture(
          &app_contexts[i]
      );

      if( BD_SUCCESS != ret ){
          printf("Error: Fail to start video capture.\n");
          return -1;
      }
    }



    //Init encoder.
    for (i = 0; i < channel_count; ++i) {
      ret = BD_FAILURE;
      ret
        = bd_init_encoder(
            PT_H264,
            BD_CBR,
            NULL,
            &sockets[i],
            &app_contexts[i]
          );

      if( BD_SUCCESS != ret ){
          printf("Error: Fail to init encoder.\n");
          return -1;
      }

    }



    //Run encoder
    for (i = 0; i < channel_count; ++i) {
      ret
        = bd_run_encoder(
            &app_contexts[i]
          );

      if( BD_SUCCESS != ret ){
          printf("Error: Fail to run encoder!\n");
          return -1;
      }

    }


    printf("Succeed: Succeed to run encoder!\n");

	//Run Audio encoder & decoder
	ret = SAMPLE_AUDIO_AiAenc();
	if( BD_SUCCESS != ret ){
		printf("Error: Fail to run audio encoder!\n");
		return -1;
	}

    printf("Succeed: Succeed to run audio encoder!\n");

    return 0;
}
