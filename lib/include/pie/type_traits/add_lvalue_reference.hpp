#pragma once

#include <pie/type_traits/add_reference_helper.hpp>

namespace pie
{
    template<class T> struct add_lvalue_reference { typedef typename pie::add_reference_helper<T>::lvalue type; };
}
