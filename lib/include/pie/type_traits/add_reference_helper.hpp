#pragma once

#include <pie/type_traits/is_reference.hpp>

namespace pie
{
    template<class T, bool = pie::is_reference<T>::value> struct add_reference_helper { typedef T lvalue; typedef T rvalue; };
    template<class T> struct add_reference_helper<T, true> { typedef T& lvalue; typedef T&& rvalue; };
}
