#pragma once

#include <pie/type_traits/integral_constant.hpp>

namespace pie
{
    template <class T> struct is_reference : pie::false_type {};
    template <class T> struct is_reference<T&> : pie::true_type {};
    template <class T> struct is_reference<T&&> : pie::true_type {};
}
