#include "stdafx.h"
#include "ontime.h"
#include <time.h>
#include "windows.h"
#include "wininet.h"

/* Static link wininet static library */
#pragma comment(lib,"wininet.lib")

/* Convert UNIX time truncation to Windows SYSTEMTIME time structure */
BOOL TimecutToSystemTime(time_t tCurrentTimecut, LPSYSTEMTIME lpSystemTime, BOOL bLocalTime)
{
	BOOL bResult = FALSE;
	if (tCurrentTimecut && lpSystemTime)
	{
		errno_t eResult = NULL;
		tm obStanTime = { NULL };
		if (bLocalTime)
			eResult = localtime_s(&obStanTime, &tCurrentTimecut);
		else
			eResult = gmtime_s(&obStanTime, &tCurrentTimecut);
		if (eResult == 0)
		{
			lpSystemTime->wYear = 1900 + obStanTime.tm_year;
			lpSystemTime->wMonth = 1 + obStanTime.tm_mon;
			lpSystemTime->wDay = obStanTime.tm_mday;
			lpSystemTime->wDayOfWeek = obStanTime.tm_wday;
			lpSystemTime->wHour = obStanTime.tm_hour;
			lpSystemTime->wMinute = obStanTime.tm_min;
			lpSystemTime->wSecond = obStanTime.tm_sec;
			bResult = TRUE;
		}
	}
	return bResult;
}

/* UTF8 decoding routine () */
BOOL InternetCharToUnicode(LPCSTR lpcInternetChar, DWORD dwCharNumberOfByte,
	LPWSTR lpUnicodeBuffer, LPDWORD lpBufferNumberOfByte)
{
	BOOL bResult = FALSE;
	if (lpcInternetChar && lpUnicodeBuffer && lpBufferNumberOfByte)
	{
		DWORD dwNeedSize = MultiByteToWideChar(CP_UTF8, NULL, lpcInternetChar, dwCharNumberOfByte, NULL, NULL);
		if (*lpBufferNumberOfByte >= dwNeedSize) // buffer size verification
		{
			DWORD dwWriteSize = MultiByteToWideChar(CP_UTF8, NULL, lpcInternetChar, dwCharNumberOfByte,
				lpUnicodeBuffer, *lpBufferNumberOfByte);
			bResult = (dwWriteSize > 0);
			if (bResult) *lpBufferNumberOfByte = dwWriteSize;
		}
		else *lpBufferNumberOfByte = dwNeedSize;
	}
	return bResult;
}

/* Get current UTC UNIX time truncation through SMALLSO Time Server */
time_t GetInternetUtcTimecut(VOID)
{
	time_t tResult = INVALID_TIMECUT;
	HINTERNET hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);
	if (hInternet)
	{
		HINTERNET hUrl = InternetOpenUrl(hInternet, INTERNET_API_URL, NULL, NULL, INTERNET_FLAG_IGNORE_CERT_CN_INVALID, NULL);
		if (hUrl)
		{
			CHAR szContextBuffer[32] = { NULL };
			DWORD dwReadSize = NULL;
			if (InternetReadFile(hUrl, szContextBuffer, sizeof(szContextBuffer), &dwReadSize)
				&& dwReadSize > 0)
			{
				WCHAR szInternetTimecut[32] = { NULL };
				DWORD dwTimecutBufferSize = sizeof(szInternetTimecut);
				if (InternetCharToUnicode(szContextBuffer, dwReadSize, szInternetTimecut, &dwTimecutBufferSize))
					tResult = (time_t)_wtoi64(szInternetTimecut);
			}
			InternetCloseHandle(hUrl);
		}
		InternetCloseHandle(hInternet);
	}
	return tResult;
}
