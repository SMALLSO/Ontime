#pragma once

/* Contains C Standard Library header files */
#include <time.h>

/* Contains the necessary Windows SDK header files */
#include "windows.h"

/* Define the API call URL for SMALLSO Time Server */
#ifdef _UNICODE
	#define INTERNET_API_URL L"https://api.xiaoyy.org/public/v1/GetCurrentTime?utc=true"
#else
	#define INTERNET_API_URL "https://api.xiaoyy.org/public/v1/GetCurrentTime?utc=true"
#endif

/* Define the required macro */
#define INVALID_TIMECUT 0

/* Function that defines the output */
BOOL 
TimecutToSystemTime(
	_In_			time_t						tCurrentTimecut,
	_Out_		LPSYSTEMTIME		lpSystemTime,
	_In_			BOOL						bLocalTime
);

time_t 
GetInternetUtcTimecut(
	VOID
);
