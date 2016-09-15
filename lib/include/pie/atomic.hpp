#pragma once

#ifdef _WIN32
#	include <pie/atomic/atomic_windows.hpp>
#else
#endif

namespace pie
{
	template<class T>
	class atomic : public pie::detail::basic_atomic<T>
	{
	public:
		using pie::detail::basic_atomic<T>::basic_atomic;
		using pie::detail::basic_atomic<T>::operator=;
		
		~atomic() {}
	};

	typedef atomic<char> atomic_char;
	typedef atomic<signed char> atomic_schar;
	typedef atomic<unsigned char> atomic_uchar;

	typedef atomic<short> atomic_short;
	typedef atomic<unsigned short> atomic_ushort;

	typedef atomic<int> atomic_int;
	typedef atomic<unsigned int> atomic_uint;

	typedef atomic<long> atomic_long;
	typedef atomic<unsigned long> atomic_ulong;

	typedef atomic<long long> atomic_llong;
	typedef atomic<unsigned long long> atomic_ullong;

	typedef atomic<wchar_t> atomic_wchar_t;
}
