#pragma once

#include <pie/type_traits/integral_constant.hpp>

namespace pie
{
	template<class Type> struct is_array : pie::false_type {};
	template<class Type> struct is_array<Type[]> : pie::true_type {};
	template<class Type, size_t Size> struct is_array<Type[Size]> : pie::true_type {};
}
