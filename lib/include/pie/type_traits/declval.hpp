#pragma once

#include <pie/type_traits/add_rvalue_reference.hpp>

namespace pie
{
    template<class T> typename pie::add_rvalue_reference<T>::type declval() noexcept;
}
