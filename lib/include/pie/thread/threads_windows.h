#pragma once

#include <Windows.h>

#if NTDDI_VERSION >= NTDDI_VISTA
#	define WINDOWS_HAS_NATIVE_CONDITION_VARIABLE
#endif

//typedef long __time32_t;
//typedef __int64 __time64_t;
//
//#ifdef _WIN64
//typedef __time64_t time_t;
//#else
//typedef __time32_t time_t;
//#endif

#define PIECALL __stdcall

typedef CRITICAL_SECTION mtx_t;

#ifdef WINDOWS_HAS_NATIVE_CONDITION_VARIABLE
typedef CONDITION_VARIABLE cnd_t;
#else
typedef HANDLE cnd_t;
#endif

typedef DWORD tss_t;

typedef void(PIECALL *tss_dtor_t)(void *);

struct thrd_t
{
	HANDLE handle;
	DWORD id;
	tss_t key;

	void * params;
};

struct tss_destructor_entry
{
	DWORD key;
	tss_dtor_t destructor;
};
