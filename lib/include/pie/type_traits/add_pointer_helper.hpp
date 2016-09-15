#pragma once

#include <pie/type_traits/is_reference.hpp>
#include <pie/type_traits/is_void.hpp>
#include <pie/type_traits/remove_reference.hpp>

namespace pie
{
    template<class T, bool = pie::is_reference<T>::value || pie::is_void<T>::value>
    struct add_pointer_helper { typedef typename pie::remove_reference<T>::type* type; };

    template<class T> struct add_pointer_helper<T, false> { typedef T type; };
    template<class T, class... Args> struct add_pointer_helper<T(Args...), true> { using type = T(*)(Args...); };
    template<class T, class... Args> struct add_pointer_helper<T(Args..., ...), true> { using type = T(*)(Args..., ...); };
}

