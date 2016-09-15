#pragma once

#include <pie/type_traits/integral_constant.hpp>

namespace pie
{
    template<class T, class U> struct is_same : pie::false_type {};
    template<class T> struct is_same<T, T> : pie::true_type {};
}
