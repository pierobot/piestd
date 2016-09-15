#include <pie/threads.h>

int thrd_create(thrd_t * thread, thrd_start_t function, void * args)
{
	if (thread == NULL)
		return thrd_error;

	thread->handle = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)function, args, 0, &thread->id);
	if (thread->handle == NULL)
		return thrd_error;

	return thrd_success;
}

int thrd_equal(thrd_t * left, thrd_t * right)
{
	return left->id == right->id;
}

thrd_t thrd_current()
{
	thrd_t thread;

	thread.handle = ::GetCurrentThread();
	thread.id = ::GetCurrentThreadId();
	thread.params = NULL;

	return thread;
}

// UNIMPLEMENTED
int thrd_sleep(struct timespec const * time_point, struct timespec * remaining)
{
	return 0;
}

void thrd_yield()
{
	::Sleep(0);
}

void thrd_exit(int result)
{
	// DO NOT FORGET TO DESTROY THREAD LOCAL OBJECTS
	// THREAD LOCAL OBJECTS ARE NOT IMPLEMENTED (YET)

	::ExitThread(result);
}

// UNIMPLEMENTED
int	thrd_detach(thrd_t thread)
{
	return 0;
}

int thrd_join(thrd_t thread, int * result)
{
	if (result == NULL)
		return thrd_error;

	// Wait for the thread to finish executing
	int code = ::WaitForSingleObject(thread.handle, INFINITE);
	// Any value other than WAIT_OBJECT_0 indicates a failure
	if (code != WAIT_OBJECT_0)
		return thrd_error;
	// Get the result code of the thread
	::GetExitCodeThread(thread.handle, (DWORD*)result);
	// And close the handle
	::CloseHandle(thread.handle);

	return thrd_success;
}


inline int mtx_init_default(mtx_t * mutex)
{
	::InitializeCriticalSection(mutex);

	// InitializeCriticalSection does not have a return value but we can check if LockSemaphore is a valid handle for success
	return mutex->LockSemaphore != INVALID_HANDLE_VALUE ? thrd_success: thrd_error;
}

int mtx_init(mtx_t * mutex, int type)
{
	if (mutex == NULL)
		return thrd_error;

	// Initialize the mutex based on the type
	switch (type)
	{
	case mtx_timed:
	case mtx_plain:
	case mtx_timed | mtx_recursive:
	case mtx_plain | mtx_recursive:
		return mtx_init_default(mutex);

	default:
		break;
	}

	// Unsupported type
	return thrd_error;
}

int mtx_lock(mtx_t * mutex)
{
	if (mutex == NULL)
		return thrd_error;

	// EnterCriticalSection does not return a value but we can check if the lock count increased for success
	LONG count = ::InterlockedOr(&mutex->LockCount, 0);
	::EnterCriticalSection(mutex);
	
	return count + 1 == ::InterlockedOr(&mutex->LockCount, 0) ? thrd_success : thrd_error;
}

// UNIMPLEMENTED
int mtx_timedlock(mtx_t * mutex, struct timespec const * time_point)
{
	if (mutex == NULL)
		return thrd_error;

	return 0;
}

int mtx_trylock(mtx_t * mutex)
{
	if (mutex == NULL)
		return thrd_error;

	LONG count = ::InterlockedOr(&mutex->LockCount, 0);
	// If another thread already owns the critical section, the return value is zero.
	if (::TryEnterCriticalSection(mutex) == FALSE)
		return thrd_busy;

	return count + 1 == ::InterlockedOr(&mutex->LockCount, 0) ? thrd_success : thrd_error;
}

int mtx_unlock(mtx_t * mutex)
{
	if (mutex == NULL)
		return thrd_error;

	LONG count = ::InterlockedOr(&mutex->LockCount, 0);
	::LeaveCriticalSection(mutex);

	return count - 1 == ::InterlockedOr(&mutex->LockCount, 0) ? thrd_success : thrd_error;
}

int mtx_destroy(mtx_t * mutex)
{
	if (mutex == NULL)
		return thrd_error;

	::DeleteCriticalSection(mutex);

	return thrd_success;
}




void call_once(once_flag * flag, void(PIECALL *function)(void))
{
	// Set the flag if not already set
	if (::InterlockedCompareExchange(flag, 1, 0) == 0)
	{
		// Execute the function
		function();
	}
}

int cnd_init(cnd_t * condition)
{
	if (condition == NULL)
		return thrd_error;

#ifdef WINDOWS_HAS_NATIVE_CONDITION_VARIABLE
	::InitializeConditionVariable(condition);
	
	return thrd_success;
#else
#endif
}

int cnd_signal(cnd_t * condition)
{
	if (condition == NULL)
		return thrd_error;
#ifdef WINDOWS_HAS_NATIVE_CONDITION_VARIABLE
	::WakeConditionVariable(condition);
	
	// WakeConditionVariable does not return a value
	return thrd_success;
#else
	
#endif
}

int cnd_broadcast(cnd_t * condition)
{
	if (condition == NULL)
		return thrd_error;
#ifdef WINDOWS_HAS_NATIVE_CONDITION_VARIABLE
	::WakeAllConditionVariable(condition);

	// WakeConditionVariable does not return a value
	return thrd_success;
#endif
}

int cnd_wait(cnd_t * condition, mtx_t * mutex)
{
	if (condition == NULL || mutex == NULL)
		return thrd_error;

#ifdef WINDOWS_HAS_NATIVE_CONDITION_VARIABLE
	if (::SleepConditionVariableCS(condition, mutex, INFINITE) == FALSE)
		return ::GetLastError() == ERROR_TIMEOUT ? thrd_timedout : thrd_error;

	return thrd_success;
#endif
}

int cnd_timedwait(cnd_t * condition, mtx_t * mutex, struct timespec const * time_point)
{
	if (condition == NULL && mutex == NULL && time_point == NULL)
		return thrd_error;

#ifdef WINDOWS_HAS_NATIVE_CONDITION_VARIABLE

#endif

	return 0;
}

void cnd_destroy(cnd_t * condition)
{
#ifdef WINDOWS_HAS_NATIVE_CONDITION_VARIABLE
	// Empty
	// http://stackoverflow.com/a/28981408
#endif
}



static tss_destructor_entry tss_table[TLS_MINIMUM_AVAILABLE];

int tss_create(tss_t * key, tss_dtor_t destructor)
{
	if (key == NULL || destructor == NULL)
		return thrd_error;

	*key = ::TlsAlloc();
	if (*key == TLS_OUT_OF_INDEXES)
		return thrd_error;

	// Make sure the key/index is within the bounds of our table
	if (*key > TLS_MINIMUM_AVAILABLE)
		return thrd_error;

	tss_table[*key].destructor = destructor;
	return thrd_success;
}

void * tss_get(tss_t key)
{
	return ::TlsGetValue(key);
}

int tss_set(tss_t key, void * value)
{
	return ::TlsSetValue(key, value) != 0 ? thrd_success : thrd_error;
}

void tss_delete(tss_t key)
{
	::TlsFree(key);
}