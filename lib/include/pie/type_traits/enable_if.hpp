#pragma once

namespace pie
{
    template<bool, class T = void> struct enable_if {};
    template<class T> struct enable_if<true, T> { typedef T type; };

    template<bool B, class T>
    using enable_if_t = typename pie::enable_if<B, T>::type;
}
