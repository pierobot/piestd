#pragma once

#include <pie/type_traits/integral_constant.hpp>

namespace pie
{
	template<class T> struct is_final : integral_constant<bool, __is_final(T)> {};
}
