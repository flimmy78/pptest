/** \enum RTSP_ERROR_CODE 
*	@brief RTSP Server의 에러코드에 대하여 정의한다.
*/
enum RTSP_ERROR_CODE
{
	RTSP_ERROR_NONE = 0,				/**< no error */
	RTSP_ERROR_INVALID_HANDLE,			/**< invalid handle */
	RTSP_ERROR_INVALID_KEY,				/**< input key is invalid */
	RTSP_ERROR_INVALID_VALUE,			/**< input value is invalid */
	RTSP_ERROR_INVALID_ARGS,			/**< input arguments are invalid */
	RTSP_ERROR_SOCKET_BIND_FAIL,		/**< rtsp server socket bind error */
	RTSP_ERROR_UNEXPECTED,				/**< unexpected error */
};

/**	\enum RTSP_LOG_LEVEL
*	@brief RTSP Server의 log level에 대해서 정의한다.
*/
enum RTSP_LOG_LEVEL
{
	RTSP_LOG_LEVEL_VERBOSE = 2,			/**< 모든 로그 출력 */
	RTSP_LOG_LEVEL_DEBUG,				/**< debug 로그 출력 */
	RTSP_LOG_LEVEL_INFO,				/**< info 로그 출력 */
	RTSP_LOG_LEVEL_WARN,				/**< warning 로그 출력 */
	RTSP_LOG_LEVEL_ERROR,				/**< error 로그 출력 */
};

/**	\enum RTSP_LOG_OUTPUT
*	@brief RTSP Server의 log output에 대해서 정의한다.
*/
enum RTSP_LOG_OUTPUT
{
	RTSP_LOG_OUTPUT_NONE			= 0x00000000,	/**< 로그 출력 하지 않음. */
    RTSP_LOG_OUTPUT_CONSOLE         = 0x00000001,	/**< 로그 콘솔 출력 */
    RTSP_LOG_OUTPUT_FILE            = 0x00000002,	/**< 로그 파일 출력 */
	RTSP_LOG_OUTPUT_DATE_DIVISION	= 0x00000004,	/**< 로그 파일을 하루 나누어서 저장 */
	RTSP_LOG_OUTPUT_ALL				= 0xFFFFFFFF,	/**< 콘솔, 파일, 파일 나누기 모두 적용 */
};

/**	\enum RTSP_CHANNEL
*	@brief RTSP Server의 live channel에 대해서 정의한다.
*/
enum RTSP_CHANNEL
{
	RTSP_CHANNEL_FHD = 0,	/**< full hd channel */
    RTSP_CHANNEL_HD,		/**< hd channel */
    RTSP_CHANNEL_SD,		/**< sd channel */
};

/**
  *  @brief rtsp server를 초기화 한다.
  *
  *  @return 없음.
 */
void rtsp_initialize();

/**
  *  @brief rtsp server를 종료 한다.
  *
  *  @return 없음.
 */
void rtsp_finalize();

/**
  *  @brief rtsp server의 포트를 설정한다.
  *	 @note 설정하지 않을 경우 default 18001로 설정된다.
  *  @param[in] 접속을 대기할 port
  *  @return 이상 없을 경우 RTSP_ERROR_NONE, 이상이 있을 경우 적절한 RTSP_ERROR_CODE가 리턴 된다.
 */
int rtsp_set_server_port(int port);

/**
  *  @brief rtsp server의 포트를 설정한다.
  *	 @note 설정하지 않을 경우 fhd : 19001, hd : 19002, sd : 19003으로 설정된다.
  *  @param[in] fhd, hd, sd 채널 중 하나
  *  @param[in] udp로 data를 수신할 포트
  *  @return 이상 없을 경우 RTSP_ERROR_NONE, 이상이 있을 경우 적절한 RTSP_ERROR_CODE가 리턴 된다.
 */
int rtsp_set_recv_port(int channel, int port);

/**
  *  @brief rtsp server의 log level을 설정한다.
  *	 @note 설정하지 않을 경우 RTSP_LOG_LEVEL_ERROR
  *  @param[in] enum RTSP_LOG_LEVEL의 값들중 하나를 선택한다.
  *  @return 이상 없을 경우 RTSP_ERROR_NONE, 이상이 있을 경우 적절한 RTSP_ERROR_CODE가 리턴 된다.
 */
int rtsp_set_log_level(int level);

/**
  *  @brief rtsp server의 log output을 설정한다.
  *	 @note 설정하지 않을 경우 RTSP_LOG_OUTPUT_NONE
  *  @param[in] enum RTSP_LOG_OUTPUT 값들중 하나를 선택한다.
  *  @return 이상 없을 경우 RTSP_ERROR_NONE, 이상이 있을 경우 적절한 RTSP_ERROR_CODE가 리턴 된다.
 */
int rtsp_set_log_output(int output);

/**
  *  @brief rtsp server의 log output 설정이 RTSP_LOG_OUTPUT_FILE 일경우 로그가 남겨질 파일 이름을 설정한다.
  *	 @note 설정하지 않을 경우 log output 설정이 RTSP_LOG_OUTPUT_FILE 이더라도 로그가 파일에 남지 않는다.
  *  @param[in] 로그가 남겨질 파일이름의 절대 경로
  *  @return 이상 없을 경우 RTSP_ERROR_NONE, 이상이 있을 경우 적절한 RTSP_ERROR_CODE가 리턴 된다.
 */
int rtsp_set_log_output(const char* filename);

/**
  *  @brief rtsp server socket을 열고, 설정된 rtsp server port로 연결 수신을 대기한다.
  *  @return 이상 없을 경우 RTSP_ERROR_NONE, 이상이 있을 경우 적절한 RTSP_ERROR_CODE가 리턴 된다.
 */
int rtsp_start();

/**
  *  @brief rtsp server port의 연결 수신을 종료하고, rtsp server socket을 닫는다.
  *  @return 이상 없을 경우 RTSP_ERROR_NONE, 이상이 있을 경우 적절한 RTSP_ERROR_CODE가 리턴 된다.
 */
int rtsp_stop();

/** @file */
