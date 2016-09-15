#pragma once

#include <pie/type_traits/integral_constant.hpp>

namespace pie
{
	template<class T>
	struct is_integral : pie::false_type
	{
	};

	template<>
	struct is_integral<bool> : pie::true_type
	{
	};

	template<>
	struct is_integral<char> : pie::true_type
	{
	};

	template<>
	struct is_integral<unsigned char> : pie::true_type
	{
	};

	template<>
	struct is_integral<char16_t> : pie::true_type
	{
	};

	template<>
	struct is_integral<char32_t> : pie::true_type
	{
	};

	template<>
	struct is_integral<short> : pie::true_type
	{
	};

	template<>
	struct is_integral<unsigned short> : pie::true_type
	{
	};

	template<>
	struct is_integral<int> : pie::true_type
	{
	};

	template<>
	struct is_integral<unsigned int> : pie::true_type
	{
	};

	template<>
	struct is_integral<long> : pie::true_type
	{
	};

	template<>
	struct is_integral<unsigned long> : pie::true_type
	{
	};

	template<>
	struct is_integral<long long> : pie::true_type
	{
	};

	template<>
	struct is_integral<unsigned long long> : pie::true_type
	{
	};

#ifdef _WIN32
#	ifdef _NATIVE_WCHAR_T_DEFINED
	template<>
	struct is_integral<wchar_t> : pie::true_type
	{
	};
#	endif
#endif
}