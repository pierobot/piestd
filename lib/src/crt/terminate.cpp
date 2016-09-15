#include <pie/exception.hpp>

#ifdef _WIN32
#	include <Windows.h>
#else
#endif

namespace pie
{
	void terminate() noexcept
	{
	#ifdef _WIN32
		::ExitProcess(0);
	#else
	#endif
	}
}
