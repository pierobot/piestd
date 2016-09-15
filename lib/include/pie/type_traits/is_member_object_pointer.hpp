#pragma once

#include <pie/type_traits/integral_constant.hpp>
#include <pie/type_traits/is_member_pointer.hpp>
#include <pie/type_traits/is_member_function_pointer.hpp>

namespace pie
{
	template<class T>
	struct is_member_object_pointer : pie::integral_constant<bool, pie::is_member_pointer<T>::value && !pie::is_member_function_pointer<T>::value > {};
}
