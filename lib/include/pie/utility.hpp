#pragma once

#include <pie/algorithm.hpp>
#include <pie/type_traits/add_rvalue_reference.hpp>
#include <pie/type_traits/declval.hpp>
#include <pie/type_traits/is_lvalue_reference.hpp>
#include <pie/type_traits/remove_reference.hpp>
#include <pie/type_traits/decay.hpp>

namespace pie
{
	template<class T>
	void swap(T & a, T & b)
	{
		T tmp = a;
		a = b;
		b = tmp;
	}

	template<class T, pie::size_t N>
	void swap(T (&a)[N], T (&b)[N])
	{
		T tmp[N];
		pie::copy(&a[0], &a[N], &tmp[0]);
		pie::copy(&b[0], &b[N], &a[0]);
		pie::copy(&tmp[0], &tmp[N], &b[0]);
	}

	template<class Type>
	Type && forward(typename pie::remove_reference<Type>::type & what)
	{
		return (static_cast<Type &&>(what));
	}

	template<class Type>
	Type && forward(typename pie::remove_reference<Type>::type && what)
	{
		static_assert(!pie::is_lvalue_reference<Type>::value, "bad forward call");
		return (static_cast<Type &&>(what));
	}

	template<class Type>
	typename pie::remove_reference<Type>::type && move(Type && what)
	{
		return (static_cast<typename pie::remove_reference<Type>::type &&>(what));
	}
	
	template<class Type1, class Type2>
	struct pair
	{
		typedef Type1 first_type;
		typedef Type2 second_type;

		first_type first;
		second_type second;

		pair(first_type const & _first, second_type const & _second) :
			first(_first), second(_second)
		{

		}

		pair(first_type && _first, second_type && _second) :
			first(pie::move(_first)), second(pie::move(_second))
		{

		}

		pair(pair<first_type, second_type> const & other) :
			first(other.first),
			second(other.second)
		{
		}

		pair(pair<first_type, second_type> && other) :
			first(pie::move(other.first)),
			second(pie::move(other.second))
		{
		}
	};

	template<class Type1, class Type2>
	inline pair<Type1, Type2> make_pair(Type1 && val1, Type2 && val2)
	{
		return (pie::pair<Type1, Type2>(val1, val2));
	}
};
