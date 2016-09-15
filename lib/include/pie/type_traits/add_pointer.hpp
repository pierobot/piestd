#pragma once

#include <pie/type_traits/add_pointer_helper.hpp>
#include <pie/type_traits/is_function.hpp>

namespace pie
{
    template<class T> struct add_pointer : pie::add_pointer_helper<T, pie::is_function<T>::value> {};
}
