#include <pie/threads.h>

extern "C" int __cdecl atexit(void(__cdecl * function)(void));

#define BEGIN_EXTERN_C extern "C" {
#define END_EXTERN_C }

int const pie_uninitialized = 0;
int const pie_initializing = -1;
int const pie_epoch_start = -1;

BEGIN_EXTERN_C

int _Init_global_epoch = pie_epoch_start;
// Name must be exact - required by compiler
__declspec(thread) int _Init_thread_epoch = pie_epoch_start;

END_EXTERN_C

static mtx_t tss_mutex;
static cnd_t tss_cnd;

void __cdecl pie_finalize_statics()
{
	mtx_destroy(&tss_mutex);
	cnd_destroy(&tss_cnd);
}

void __cdecl pie_initialize_statics()
{
	mtx_init(&tss_mutex, mtx_plain);
	cnd_init(&tss_cnd);

	atexit(pie_finalize_statics);
}

BEGIN_EXTERN_C

// Called at the beginning of a variable's initialization.
// Name must be exact
void __cdecl _Init_thread_header(int * once)
{
	mtx_lock(&tss_mutex);

	if (*once == pie_uninitialized)
	{
		*once = pie_initializing;
	}
	else
	{
		while (*once == pie_initializing)
		{
			cnd_wait(&tss_cnd, &tss_mutex);

			if (*once == pie_uninitialized)
			{
				*once = pie_initializing;
				break;
			}
		}

		_Init_thread_epoch = _Init_global_epoch;
	}

	mtx_unlock(&tss_mutex);
}

// Called at the end of a variable's initialization
// Name must be exact
void __cdecl _Init_thread_footer(int * once)
{
	mtx_lock(&tss_mutex);

	++_Init_global_epoch;
	*once = _Init_global_epoch;
	_Init_thread_epoch = _Init_global_epoch;
	
	mtx_unlock(&tss_mutex);
	cnd_broadcast(&tss_cnd);
}

END_EXTERN_C
