#pragma once

#include <pie/type_traits/integral_constant.hpp>
#include <pie/type_traits/is_function.hpp>
#include <pie/type_traits/remove_cv.hpp>

namespace pie
{
	namespace detail
	{
		template< class T >
		struct is_member_function_pointer_helper : pie::false_type {};
		 
		template< class T, class U>
		struct is_member_function_pointer_helper<T U::*> : pie::is_function<T> {};
	}
	 
	template< class T >
	struct is_member_function_pointer : pie::detail::is_member_function_pointer_helper< pie::remove_cv_t<T> > {};
}
