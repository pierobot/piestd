#pragma once

#include <pie/functional.hpp>
#include <pie/stdint.hpp>
#include <pie/type_traits.hpp>

namespace pie
{
    // http://codereview.stackexchange.com/questions/52272/standard-layout-tuple-implementation

    template <class... Ts>
    struct tuple;

    template <class T, class... Ts>
    struct tuple<T, Ts...>
    {
        T first;
        tuple<Ts...> rest;

        tuple() = default;
        template <class U, class...Us, class = typename pie::enable_if<!pie::is_base_of<tuple, typename pie::decay<U>::type>::value>::type>
        tuple(U && u, Us &&...tail) :
            first(pie::forward<U>(u)),
            rest(pie::forward<Us>(tail)...) {}
    };

    template <class T>
    struct tuple<T>
    {
        T first;

        tuple() = default;
        template <class U, class = typename pie::enable_if<!pie::is_base_of<tuple, typename pie::decay<U>::type>::value>::type>
        tuple(U&& u) :
            first(pie::forward<U>(u)) {}
    };

    template <>
    struct tuple<> {};

	template<class T>
	struct tuple_size
	{
	};

	template<class... Types>
	struct tuple_size<tuple<Types...>> : 
		pie::integral_constant<pie::size_t, sizeof...(Types)>
	{
	};

	template<class Tuple>
	struct tuple_size<const Tuple> : tuple_size<Tuple>
	{
	};

	template<class Tuple>
	struct tuple_size<volatile Tuple> : tuple_size<Tuple>
	{
	};

	template<class Tuple>
	struct tuple_size<const volatile Tuple> : tuple_size<Tuple>
	{
	};

    namespace detail {

        template<pie::size_t i, class T>
        struct tuple_element;

        template<pie::size_t i, class T, class... Ts>
        struct tuple_element<i, tuple<T, Ts...>>
            : tuple_element<i - 1, tuple<Ts...>>
        {};

        template <class T, class... Ts>
        struct tuple_element<0, tuple<T, Ts...>>
        {
            using type = T;
        };


        template<pie::size_t i>
        struct tuple_accessor
        {
            template <class... Ts>
            static inline typename tuple_element<i, tuple<Ts...>>::type & get(tuple<Ts...> & t)
            {
                return tuple_accessor<i - 1>::get(t.rest);
            }

            template <class... Ts>
            static inline const typename tuple_element<i, tuple<Ts...>>::type & get(const tuple<Ts...> & t)
            {
                return tuple_accessor<i - 1>::get(t.rest);
            }
        };

        template <>
        struct tuple_accessor<0>
        {
            template <class... Ts>
            static inline typename tuple_element<0, tuple<Ts...>>::type & get(tuple<Ts...> & t)
            {
                return t.first;
            }

            template <class... Ts>
            static inline const typename tuple_element<0, tuple<Ts...>>::type & get(const tuple<Ts...> & t)
            {
                return t.first;
            }
        };

    }

    template<class... Ts>
    inline tuple<typename pie::decay<Ts>::type...> make_tuple(Ts &&... x)
    {
        return tuple<typename pie::decay<Ts>::type...>(pie::forward<Ts>(x)...);
    }

    template<pie::size_t i, class... Ts>
    inline typename detail::tuple_element<i, tuple<Ts...> >::type & get(tuple<Ts...> & t)
    {
        return detail::tuple_accessor<i>::get(t);
    }

    template<pie::size_t i, class... Ts>
    inline const typename detail::tuple_element<i, tuple<Ts...> >::type & get(const tuple<Ts...> & t)
    {
        return detail::tuple_accessor<i>::get(t);
    }

	template<class T>
	struct unwrap_reference_wrapper
	{
		typedef T type;
	};

	template<class T>
	struct unwrap_reference_wrapper<pie::reference_wrapper<T>>
	{
		typedef T& type;
	};

	template<class T>
	using special_decay_t = typename unwrap_reference_wrapper<typename pie::decay<T>::type>::type;

	template<class... Types>
	auto make_tuple(Types &&... args) -> pie::tuple<special_decay_t<Types>...>
	{
		return pie::tuple<special_decay_t<Types>...>(pie::forward<Types>(args)...);
	}

	template<class... Types>
	auto forward_as_tuple(Types &&... args) noexcept -> pie::tuple<Types...>
	{
		return pie::tuple<Types...>(pie::forward<Types>(args)...);
	}

}
