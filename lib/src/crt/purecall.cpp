#include <pie/exception.hpp>

int __cdecl _purecall(void)
{
	// This function should never be called. If so, immediately terminate execution.
	pie::terminate();
	
	return 0;
}
