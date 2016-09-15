#include <pie/errno.h>

int * __cdecl __errno()
{
	static thread_local int error_number;

	return &error_number;
}

int __cdecl __set_errno(int value)
{
	return *__errno() = value;
}