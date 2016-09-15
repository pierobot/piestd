#pragma once

#include <pie/utility.hpp>
#include <pie/type_traits/integral_constant.hpp>

namespace pie
{
	template<class From, class To> struct is_convertible : pie::integral_constant<bool, __is_convertible_to(From, To)> {};
}
