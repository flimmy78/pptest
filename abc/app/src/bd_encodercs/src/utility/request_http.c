/**
  * @file  request_http.c
  * @brief Request HTTP module for IVManager.  
  * Copyright (C) PixelPlus - All Rights Reserved
  * Written by Jinho Yoo, Feb 2015
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "request_http.h"
#include <curl/curl.h>

// return 0 : success
int request_http(const char* url, const char* requestJSON)
{
	CURL *curl;
	CURLcode res;


	/* get a curl handle */ 
	curl = curl_easy_init();
	if(curl){
		curl_easy_setopt(curl, CURLOPT_URL, url);

		// header
		struct curl_slist * headers = NULL;
		headers = curl_slist_append(headers, "content-type : application/json");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		/* Now specify we want to POST data */ 
		curl_easy_setopt(curl, CURLOPT_POST, 1L);		
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestJSON);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(requestJSON));

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */ 
		if(res != CURLE_OK)
		{	
			printf("curl_easy_perform() failed: code(%d), message(%s)\n", 
				res, curl_easy_strerror(res));
		}

		/* always cleanup */ 
		curl_easy_cleanup(curl);
	}

	// return http response
	return res;
}