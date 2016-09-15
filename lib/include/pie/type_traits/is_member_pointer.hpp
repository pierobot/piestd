#pragma once

#include <pie/type_traits/integral_constant.hpp>
#include <pie/type_traits/remove_cv.hpp>

namespace pie
{
	namespace detail
	{
		template<class T> struct is_member_pointer_helper : pie::false_type {};
		template<class T, class U> struct is_member_pointer_helper<T U::*> : pie::true_type {};
	}

    template<class T> struct is_member_pointer : detail::is_member_pointer_helper<typename pie::remove_cv<T>::type> {};

    template<class T>
    constexpr bool is_member_pointer_v = pie::is_member_pointer<T>::value;
}
