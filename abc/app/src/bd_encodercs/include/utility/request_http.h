/**
  * @file  request_http.h
  * @brief Request HTTP module for IVManager.  
  * Copyright (C) PixelPlus - All Rights Reserved
  * Written by Jinho Yoo, Feb 2015
*/


#ifndef __IV_EVENT_H__
#define __IV_EVENT_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
  *  @brief http로 원하는 JSON data를 요청한다. 
  *
  *  @param[in] url  통신을 할 URL.
  *  @param[in] context  Network context의 pointer.
  *  @return http result. 정상적인 경우 0를 Return 한다. 그 외의 경우는 http://curl.haxx.se/libcurl/c/libcurl-errors.html 참조.
  */
 int request_http(const char* url, const char* requestJSON);

#ifdef __cplusplus
}
#endif

#endif