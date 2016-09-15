#pragma once

namespace pie
{
    template<class T> struct remove_volatile { typedef T type; };
    template<class T> struct remove_volatile<volatile T> { typedef T type; };
}
