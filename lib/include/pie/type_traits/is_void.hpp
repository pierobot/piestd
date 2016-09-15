#pragma once

#include <pie/type_traits/integral_constant.hpp>

namespace pie
{
	template<class T> struct is_void : false_type {};
    template<> struct is_void<void> : true_type {};
}
