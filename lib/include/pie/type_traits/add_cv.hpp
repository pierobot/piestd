#pragma once

#include <pie/type_traits/add_const.hpp>
#include <pie/type_traits/add_volatile.hpp>

namespace pie
{
	template<class T> struct add_cv { typedef typename pie::add_volatile<typename pie::add_const<T>::type>::type type; };
}
