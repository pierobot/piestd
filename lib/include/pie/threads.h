#pragma once

#include <pie/time.h>

#ifdef _WIN32
#	include <pie/thread/threads_windows.h>
#else
#endif

#ifdef __cplusplus
#define BEGIN_EXTERN_C extern "C" {
#define END_EXTERN_C }
#else
#define BEGIN_EXTERN_C
#define END_EXTERN_C
#endif

enum
{
	thrd_success = 0,
	thrd_nomem,
	thrd_timedout,
	thrd_busy,
	thrd_error
};

typedef int(* PIECALL thrd_start_t)(void*);

BEGIN_EXTERN_C
	int    thrd_create(thrd_t * thread, thrd_start_t function, void * args);
	int    thrd_equal(thrd_t * left, thrd_t * right);
	thrd_t thrd_current();
	int    thrd_sleep(struct timespec const * time_point, struct timespec * remaining);
	void   thrd_yield();
	void   thrd_exit(int result);
	int	   thrd_detach(thrd_t thread);
	int    thrd_join(thrd_t thread, int * result);
END_EXTERN_C


BEGIN_EXTERN_C
	int mtx_init(mtx_t * mutex, int type);
	int mtx_lock(mtx_t * mutex);
	int mtx_timedlock(mtx_t * mutex, struct timespec const * time_point);
	int mtx_trylock(mtx_t * mutex);
	int mtx_unlock(mtx_t * mutex);
	int mtx_destroy(mtx_t * mutex);
END_EXTERN_C

enum
{
	mtx_plain = 0,
	mtx_recursive,
	mtx_timed
};

typedef volatile unsigned int once_flag;
#define ONCE_FLAG_INIT 0

BEGIN_EXTERN_C
	void call_once(once_flag * flag, void(PIECALL *function)(void));

	int  cnd_init(cnd_t * condition);
	int  cnd_signal(cnd_t * condition);
	int  cnd_broadcast(cnd_t * condition);
	int  cnd_wait(cnd_t * condition, mtx_t * mutex);
	int  cnd_timedwait(cnd_t * condition, mtx_t * mutex, struct timespec const * time_point);
	void cnd_destroy(cnd_t * condition);
END_EXTERN_C

#define TSS_DTOR_ITERATIONS 1

BEGIN_EXTERN_C
	int    tss_create(tss_t * tss, tss_dtor_t destructor);
	void * tss_get(tss_t tss);
	int    tss_set(tss_t tss, void * value);
	void   tss_delete(tss_t tss);
END_EXTERN_C
