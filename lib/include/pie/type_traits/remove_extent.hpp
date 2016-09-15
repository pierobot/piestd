#pragma once

namespace pie
{
    template<class Type> struct remove_extent { typedef Type type; };
    template<class Type> struct remove_extent<Type[]> { typedef Type type; };
    template<class Type, size_t Size> struct remove_extent<Type[Size]> { typedef Type type; };
}
