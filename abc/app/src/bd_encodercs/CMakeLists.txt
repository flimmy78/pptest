cmake_minimum_required (VERSION 2.6)

#Set project name to generate executable.
project (bd_enc)


set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set (PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (PROJECT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)


include_directories(
		${PROJECT_INCLUDE_DIR}
		${PROJECT_INCLUDE_DIR}/utility
		${PROJECT_INCLUDE_DIR}/bd1api/include
)

link_directories(
	${PROJECT_LIB_DIR}
	${PROJECT_LIB_DIR}/bd1api/lib
)

set(SRCS
	${PROJECT_SOURCE_DIR}/bd_encoder.c
	${PROJECT_SOURCE_DIR}/bd_internal.c
	${PROJECT_SOURCE_DIR}/sample_comm_sys.c
	${PROJECT_SOURCE_DIR}/sample_comm_vi.c
	${PROJECT_SOURCE_DIR}/sample_comm_isp.c
	${PROJECT_SOURCE_DIR}/sample_comm_venc.c
	${PROJECT_SOURCE_DIR}/sample_comm_audio.c
	${PROJECT_SOURCE_DIR}/utility/FrameSender.c
	${PROJECT_SOURCE_DIR}/utility/SharedMemQue.c
)

add_library( ${PROJECT_NAME} STATIC ${SRCS} )
target_link_libraries( ${PROJECT_NAME}
	sys
	isp
	pthread
	m
	venc
	vi
	EncodeApi
	KernelDevice_TopazHP
	KernelComponent
	PortFramework
	SECURE_TAL
	Target
	ListUtils
	sns
	OSA
	ive
	audio
	common
	mi
	)

#Specify the cross compiler for mips
SET( CMAKE_C_FLAGS  "-EL -g  -O2 -DHW_3_X -DDMABUF_BUFFERS -DSENSOR_TYPE=LVDS_PS3210K_LVDS_1080P_30FPS -DSENSOR_CAM0_ADDR=0xFF -DSENSOR_CAM1_ADDR=0xFF -DSENSOR_SLOT=0 -DSENSOR_CAM0_I2C=0 -DSENSOR_CAM0_SPWDN=1"  )

