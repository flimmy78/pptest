#!/bin/sh
export DEMO_PATH=/root/video_server
export PATH=$PATH:$DEMO_PATH

touch /tmp/.bd_shared_mem_video_0
touch /tmp/.bd_shared_mem_video_1
touch /tmp/.bd_shared_mem_video_2

server&
bd_encoder&
