#pragma once

#include <pie/type_traits/remove_reference.hpp>
#include <pie/type_traits/conditional.hpp>
#include <pie/type_traits/remove_extent.hpp>
#include <pie/type_traits/is_array.hpp>
#include <pie/type_traits/is_function.hpp>
#include <pie/type_traits/add_pointer.hpp>
#include <pie/type_traits/remove_cv.hpp>

namespace pie
{
    template<class T>
    struct decay
    {
    private:
        typedef typename pie::remove_reference<T>::type U;
    public:
        typedef typename pie::conditional<
            pie::is_array<U>::value,
            typename pie::remove_extent<U>::type,
                typename pie::conditional<
                    pie::is_function<U>::value,
                    typename pie::add_pointer<U>::type,
                    typename pie::remove_cv<U>::type
                >::type
            >::type type;
    };

    template<class T>
    using decay_t = typename pie::decay<T>::type;

    template<class T>
    pie::decay_t<T> decay_copy(T&& v) { return pie::forward<T>(v); }
}
