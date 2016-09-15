#pragma once

#include <pie/type_traits/add_reference_helper.hpp>

namespace pie
{
	template<class T> struct add_rvalue_reference { typedef typename pie::add_reference_helper<T>::rvalue type; };
}
