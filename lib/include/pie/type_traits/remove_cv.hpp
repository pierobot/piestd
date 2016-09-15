#pragma once

#include <pie/type_traits/remove_const.hpp>
#include <pie/type_traits/remove_volatile.hpp>

namespace pie
{
    template<class T>
    struct remove_cv
    {
        typedef typename pie::remove_volatile<typename pie::remove_const<T>::type>::type type;
    };
	
	template<class T>
	using remove_cv_t = typename pie::remove_cv<T>::type;
}
