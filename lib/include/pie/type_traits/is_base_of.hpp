#pragma once

#include <pie/type_traits/integral_constant.hpp>

namespace pie
{
	template<class Base, class Derived>
	struct is_base_of : pie::integral_constant<bool, __is_base_of(Base, Derived)> {};

    template<class Base, class Derived>
    constexpr bool is_base_of_v = typename pie::is_base_of<Base, Derived>::value;
}
