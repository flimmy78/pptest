#ifndef __NOMAD_LOG_H__
#define __NOMAD_LOG_H__

#include <string.h>
#include "Logger.h"
#include "CMSLogger.h"

#define NOMAD_LOG_VERBOSE	1
#define NOMAD_LOG_DEBUG	1
#define NOMAD_LOG_INFO	1
#define NOMAD_LOG_WARN	1
#define NOMAD_LOG_ERROR	1

#ifdef WIN32
#define PATH_DELIMITER '\\'
#else
#define PATH_DELIMITER '/'
#endif

#define __FILENAME__	((strrchr(__FILE__, PATH_DELIMITER) == NULL) ? __FILE__ : strrchr(__FILE__, PATH_DELIMITER) + 1)


#if NOMAD_LOG_VERBOSE
#define LOGV(FILTER, ...) \
	getLogger().print(eLL_VERBOSE, FILTER, __VA_ARGS__);
#define LOGV_CALL(FILTER, MSG) \
	getLogger().print(eLL_VERBOSE, FILTER, "%s(%d) %s: %s", __FILENAME__, __LINE__, __FUNCTION__, MSG);
#define LOGV_LINE(FILTER) \
	getLogger().print(eLL_VERBOSE, FILTER, "%s(%d) %s", __FILENAME__, __LINE__, __FUNCTION__);
#else
#define LOGV(FILTER, ...) 0
#define LOGV_CALL(FILTER, MSG) 0
#define LOGV_LINE(FILTER) 0
#endif

#if NOMAD_LOG_DEBUG
#define LOGD(FILTER, ...) \
	getLogger().print(eLL_DEBUG, FILTER, __VA_ARGS__);
#define LOGD_CALL(FILTER, MSG) \
	getLogger().print(eLL_DEBUG, FILTER, "%s(%d) %s: %s", __FILENAME__, __LINE__, __FUNCTION__, MSG);
#define LOGD_LINE(FILTER) \
	getLogger().print(eLL_DEBUG, FILTER, "%s(%d) %s", __FILENAME__, __LINE__, __FUNCTION__);
#else
#define LOGD(FILTER, ...) 0
#define LOGD_CALL(FILTER, MSG) 0
#define LOGD_LINE(FILTER) 0
#endif

#if NOMAD_LOG_INFO
#define LOGI(FILTER, ...) \
	getLogger().print(eLL_INFO, FILTER, __VA_ARGS__);
#define LOGI_CALL(FILTER, MSG) \
	getLogger().print(eLL_INFO, FILTER, "%s(%d) %s: %s", __FILENAME__, __LINE__, __FUNCTION__, MSG);
#define LOGI_LINE(FILTER) \
	getLogger().print(eLL_INFO, FILTER, "%s(%d) %s", __FILENAME__, __LINE__, __FUNCTION__);
#else
#define LOGI(FILTER, ...) 0
#define LOGI_CALL(FILTER, MSG) 0
#define LOGI_LINE(FILTER) 0
#endif

#if NOMAD_LOG_WARN
#define LOGW(FILTER, ...) \
	getLogger().print(eLL_WARN, FILTER, __VA_ARGS__);
#define LOGW_CALL(FILTER, MSG) \
	getLogger().print(eLL_WARN, FILTER, "%s(%d) %s: %s", __FILENAME__, __LINE__, __FUNCTION__, MSG);
#define LOGW_LINE(FILTER) \
	getLogger().print(eLL_WARN, FILTER, "%s(%d) %s", __FILENAME__, __LINE__, __FUNCTION__);
#else
#define LOGW(FILTER, ...) 0
#define LOGW_CALL(FILTER, MSG) 0
#define LOGW_LINE(FILTER) 0
#endif

#if NOMAD_LOG_ERROR
#define LOGE(FILTER, ...) \
	getLogger().print(eLL_ERROR, FILTER, __VA_ARGS__);
#define LOGE_CALL(FILTER, MSG) \
	getLogger().print(eLL_ERROR, FILTER, "%s(%d) %s: %s", __FILENAME__, __LINE__, __FUNCTION__, MSG);
#define LOGE_LINE(FILTER) \
	getLogger().print(eLL_ERROR, FILTER, "%s(%d) %s", __FILENAME__, __LINE__, __FUNCTION__);

#if !defined(ASSERT)
#define ASSERT(x) \
if(!(x)) \
{ \
	getLogger().print(eLL_ERROR, "ERROR", "%s(%d) %s", __FILENAME__, __LINE__, __FUNCTION__); \
}
#endif
#else
#define LOGE(FILTER, ...) 0
#define LOGE_CALL(FILTER, MSG) 0
#define LOGE_LINE(FILTER) 0
#define ASSERT(x) 0
#endif

#ifdef SLAVE
#define LOG_CMS(...) getCMSLogger().print(__VA_ARGS__);
#define LOG_CMS_DEVIDE() getCMSLogger().checkDevide();
#define CMS_LOG_FORMAT "%s:%s:%s"
#else
#define LOG_CMS(...) 0
#define LOG_CMS_DEVIDE() 0
#define CMS_LOG_FORMAT "%s:%s:%s"
#endif

#define __CALL_INFO__ __FILENAME__, __LINE__, __FUNCTION__

#endif // __NOMAD_LOG_H__
