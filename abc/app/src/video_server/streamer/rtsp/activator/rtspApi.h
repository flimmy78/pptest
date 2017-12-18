/** \enum RTSP_ERROR_CODE 
*	@brief RTSP Server�� �����ڵ忡 ���Ͽ� �����Ѵ�.
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
*	@brief RTSP Server�� log level�� ���ؼ� �����Ѵ�.
*/
enum RTSP_LOG_LEVEL
{
	RTSP_LOG_LEVEL_VERBOSE = 2,			/**< ��� �α� ��� */
	RTSP_LOG_LEVEL_DEBUG,				/**< debug �α� ��� */
	RTSP_LOG_LEVEL_INFO,				/**< info �α� ��� */
	RTSP_LOG_LEVEL_WARN,				/**< warning �α� ��� */
	RTSP_LOG_LEVEL_ERROR,				/**< error �α� ��� */
};

/**	\enum RTSP_LOG_OUTPUT
*	@brief RTSP Server�� log output�� ���ؼ� �����Ѵ�.
*/
enum RTSP_LOG_OUTPUT
{
	RTSP_LOG_OUTPUT_NONE			= 0x00000000,	/**< �α� ��� ���� ����. */
    RTSP_LOG_OUTPUT_CONSOLE         = 0x00000001,	/**< �α� �ܼ� ��� */
    RTSP_LOG_OUTPUT_FILE            = 0x00000002,	/**< �α� ���� ��� */
	RTSP_LOG_OUTPUT_DATE_DIVISION	= 0x00000004,	/**< �α� ������ �Ϸ� ����� ���� */
	RTSP_LOG_OUTPUT_ALL				= 0xFFFFFFFF,	/**< �ܼ�, ����, ���� ������ ��� ���� */
};

/**	\enum RTSP_CHANNEL
*	@brief RTSP Server�� live channel�� ���ؼ� �����Ѵ�.
*/
enum RTSP_CHANNEL
{
	RTSP_CHANNEL_FHD = 0,	/**< full hd channel */
    RTSP_CHANNEL_HD,		/**< hd channel */
    RTSP_CHANNEL_SD,		/**< sd channel */
};

/**
  *  @brief rtsp server�� �ʱ�ȭ �Ѵ�.
  *
  *  @return ����.
 */
void rtsp_initialize();

/**
  *  @brief rtsp server�� ���� �Ѵ�.
  *
  *  @return ����.
 */
void rtsp_finalize();

/**
  *  @brief rtsp server�� ��Ʈ�� �����Ѵ�.
  *	 @note �������� ���� ��� default 18001�� �����ȴ�.
  *  @param[in] ������ ����� port
  *  @return �̻� ���� ��� RTSP_ERROR_NONE, �̻��� ���� ��� ������ RTSP_ERROR_CODE�� ���� �ȴ�.
 */
int rtsp_set_server_port(int port);

/**
  *  @brief rtsp server�� ��Ʈ�� �����Ѵ�.
  *	 @note �������� ���� ��� fhd : 19001, hd : 19002, sd : 19003���� �����ȴ�.
  *  @param[in] fhd, hd, sd ä�� �� �ϳ�
  *  @param[in] udp�� data�� ������ ��Ʈ
  *  @return �̻� ���� ��� RTSP_ERROR_NONE, �̻��� ���� ��� ������ RTSP_ERROR_CODE�� ���� �ȴ�.
 */
int rtsp_set_recv_port(int channel, int port);

/**
  *  @brief rtsp server�� log level�� �����Ѵ�.
  *	 @note �������� ���� ��� RTSP_LOG_LEVEL_ERROR
  *  @param[in] enum RTSP_LOG_LEVEL�� ������ �ϳ��� �����Ѵ�.
  *  @return �̻� ���� ��� RTSP_ERROR_NONE, �̻��� ���� ��� ������ RTSP_ERROR_CODE�� ���� �ȴ�.
 */
int rtsp_set_log_level(int level);

/**
  *  @brief rtsp server�� log output�� �����Ѵ�.
  *	 @note �������� ���� ��� RTSP_LOG_OUTPUT_NONE
  *  @param[in] enum RTSP_LOG_OUTPUT ������ �ϳ��� �����Ѵ�.
  *  @return �̻� ���� ��� RTSP_ERROR_NONE, �̻��� ���� ��� ������ RTSP_ERROR_CODE�� ���� �ȴ�.
 */
int rtsp_set_log_output(int output);

/**
  *  @brief rtsp server�� log output ������ RTSP_LOG_OUTPUT_FILE �ϰ�� �αװ� ������ ���� �̸��� �����Ѵ�.
  *	 @note �������� ���� ��� log output ������ RTSP_LOG_OUTPUT_FILE �̴��� �αװ� ���Ͽ� ���� �ʴ´�.
  *  @param[in] �αװ� ������ �����̸��� ���� ���
  *  @return �̻� ���� ��� RTSP_ERROR_NONE, �̻��� ���� ��� ������ RTSP_ERROR_CODE�� ���� �ȴ�.
 */
int rtsp_set_log_output(const char* filename);

/**
  *  @brief rtsp server socket�� ����, ������ rtsp server port�� ���� ������ ����Ѵ�.
  *  @return �̻� ���� ��� RTSP_ERROR_NONE, �̻��� ���� ��� ������ RTSP_ERROR_CODE�� ���� �ȴ�.
 */
int rtsp_start();

/**
  *  @brief rtsp server port�� ���� ������ �����ϰ�, rtsp server socket�� �ݴ´�.
  *  @return �̻� ���� ��� RTSP_ERROR_NONE, �̻��� ���� ��� ������ RTSP_ERROR_CODE�� ���� �ȴ�.
 */
int rtsp_stop();

/** @file */
