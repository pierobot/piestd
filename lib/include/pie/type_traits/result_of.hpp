#pragma once

#include <pie/type_traits/declval.hpp>
#include <pie/type_traits/enable_if.hpp>
#include <pie/type_traits/is_function.hpp>
#include <pie/type_traits/is_base_of.hpp>
#include <pie/type_traits/is_void.hpp>
#include <pie/type_traits/decay.hpp>
#include <pie/type_traits/is_member_function_pointer.hpp>
#include <pie/type_traits/is_member_object_pointer.hpp>

namespace pie
{
	namespace detail
	{
		struct invoke_pmf_object
		{
			template<class Decayed, class Type, class... ArgTypes>
			static auto call(Decayed pmf, Type && arg, ArgTypes &&... args) ->
				decltype((pie::forward<Type>(arg).*pmf)(pie::forward<ArgTypes>(args)...))
			{
				return ((pie::forward<Type>(arg).*pmf)(pie::forward<ArgTypes>(args)...));
			}	
		};
	
		// Invokes a pointer to member function on a 'smart' pointer
		struct invoke_pmf_pointer
		{
			template<class Decayed, class Type, class... ArgTypes>
			static auto call(Decayed pmf, Type && arg, ArgTypes &&... args) ->
				decltype(((*pie::forward<Type>(arg)).*pmf)(pie::forward<ArgTypes>(args)...))
			{
				return (((*pie::forward<Type>(arg)).*pmf)(pie::forward<ArgTypes>(args)...));
			}
		};

		// Invokes a pointer to member data data on an object
		struct invoke_pmd_object
		{
			template<class Decayed, class Type>
			static auto call(Decayed pmd, Type && arg) -> decltype(pie::forward<Type>(arg).*pmd)
			{
				return pie::forward<Type>(arg).*pmd;
			}		
		};

		// Invokes a pointer to member data data on a 'smart' pointer
		struct invoke_pmd_pointer
		{
			template<class Decayed, class Type>
			static auto call(Decayed pmd, Type && arg) -> decltype((*pie::forward<Type>(arg)).*pmd)
			{
				return (*pie::forward<Type>(arg)).*pmd;
			}
		};

		// Invoke a function object
		struct invoke_functor
		{
			template<class Callable, class... ArgTypes>
			static auto call(Callable && object, ArgTypes &&... args) ->
				decltype(pie::forward<Callable>(object)(pie::forward<ArgTypes>(args)...))
			{
				return pie::forward<Callable>(object)(pie::forward<ArgTypes>(args)...);
			}
		};
		
		template<bool, class T, class U>
		struct If
		{	// type is U for assumed false
			typedef U type;
		};

		template<class T, class U>
		struct If<true, T, U>
		{	// type is T for assumed true
			typedef T type;
		};

		template<class Callable,
			     class Type,
			     class Decayed = typename pie::decay<Type>::type,
			     bool is_pmf = pie::is_member_function_pointer<Decayed>::value,
				 bool is_pmd = pie::is_member_object_pointer<Decayed>::value>
		struct invoker_helper;
		
		// Wrapper for pointer to member function
		template<class Callable, class Type, class Decayed>
		struct invoker_helper<Callable, Type, Decayed, true, false> :
			pie::detail::If<pie::is_base_of<Decayed, typename pie::decay<Type>::type>::value,
							pie::detail::invoke_pmf_object,
							pie::detail::invoke_pmf_pointer>::type
		{
		};
		
		// Wrapper for pointer to member data
		template<class Callable, class Type, class Decayed>
		struct invoker_helper<Callable, Type, Decayed, false, true> :
			pie::detail::If<pie::is_base_of<Decayed, typename pie::decay<Type>::type>::value,
							pie::detail::invoke_pmd_object,
							pie::detail::invoke_pmd_pointer>::type
		{
		};
		
		// Wrapper for function object
		template<class Callable, class Type, class Decayed>
		struct invoker_helper<Callable, Type, Decayed, false, false> : pie::detail::invoke_functor
		{
		};
		
		template<class Callable, class... ArgTypes>
		struct invoker;
		
		template<class Callable>
		struct invoker<Callable> : pie::detail::invoke_functor
		{
		};
		
		template<class Callable, class Type, class... ArgTypes>
		struct invoker<Callable, Type, ArgTypes...> : pie::detail::invoker_helper<Callable, Type>
		{
		};

		template<class Result, bool = pie::is_void<Result>::value>
		struct Forced
		{
		};

		struct Unforced
		{
		};
	}
	
	template<class F, class... ArgTypes>
    inline auto invoke(F&& f, ArgTypes &&... args)
        -> decltype(pie::detail::invoker<F, ArgTypes...>::call(pie::forward<F>(f), pie::forward<ArgTypes>(args)...))
    {
        return pie::detail::invoker<F, ArgTypes...>::call(pie::forward<F>(f), pie::forward<ArgTypes>(args)...);
    }

	template<class Result, class Function, class... Args>
	typename pie::enable_if<pie::is_void<Result>::value>::type invoke_ret(Function && f, Args &&... args)
	{
		pie::invoke(pie::forward<Function>(f), pie::forward<Args>(args)...);
	}

	template<class Result, class Function, class... Args>
	typename pie::enable_if<!pie::is_void<Result>::value, Result>::type invoke_ret(Function && f, Args &&... args)
	{
		return pie::invoke(pie::forward<Function>(f), pie::forward<Args>(args)...);
	}

	/*template<class R, class = typename pie::enable_if<pie::is_void<R>::value>::type, class... ArgTypes>
	inline void invoke_ret(ArgTypes &&... args)
	{
		pie::invoke(pie::forward<ArgTypes>(args)...);
	}

	template<class R, class = typename pie::enable_if<!pie::is_void<R>::value>::type, class... ArgTypes>
	inline R invoke_ret(ArgTypes &&... args)
	{
		return pie::invoke(pie::forward<ArgTypes>(args)...);
	}*/

	namespace detail
	{
		struct unique_tag_result_of
		{
		};
		
		template<class... ArgTypes>
		struct result_of
		{
		};

		template<class F, class... ArgTypes>
		struct result_of<F(ArgTypes...)>
		{
			typedef decltype(pie::invoke(pie::declval<F>(), pie::declval<ArgTypes>()...)) type;
		};
	}

	template<class>
	struct result_of
	{
	};
	
	template<class F, class... ArgTypes>
	struct result_of<F(ArgTypes...)> : pie::detail::result_of<F(ArgTypes...)>
	{
	};
	
    template<class T>
    using result_of_t = typename pie::result_of<T>::type;
}
