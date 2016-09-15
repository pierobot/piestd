#pragma once

#include <pie/type_traits/integral_constant.hpp>

namespace pie
{
    template<class T> struct is_lvalue_reference : pie::false_type {};
    template<class T> struct is_lvalue_reference<T&> : pie::true_type {};
}
