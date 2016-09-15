#pragma once

//typedef long clock_t;
//
//#define CLOCKS_PER_SEC ((clock_t)1000)

#if defined _USE_32BIT_TIME_T && defined _WIN64
#error You cannot use 32-bit time_t (_USE_32BIT_TIME_T) with _WIN64
#endif

typedef long    __time32_t;
typedef __int64 __time64_t;

#ifdef _USE_32BIT_TIME_T
typedef __time32_t time_t;
#else
typedef __time64_t time_t;
#endif

typedef struct tm
{
	int tm_sec;   // seconds after the minute - [0, 60] including leap second
	int tm_min;   // minutes after the hour - [0, 59]
	int tm_hour;  // hours since midnight - [0, 23]
	int tm_mday;  // day of the month - [1, 31]
	int tm_mon;   // months since January - [0, 11]
	int tm_year;  // years since 1900
	int tm_wday;  // days since Sunday - [0, 6]
	int tm_yday;  // days since January 1 - [0, 365]
	int tm_isdst; // daylight savings time flag. The value is positive if DST is in effect, zero if not and negative if no information is available
};

typedef struct timespec
{
	time_t tv_sec;  // Seconds - >= 0
	long   tv_nsec; // Nanoseconds - [0, 999999999]
};