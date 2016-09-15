#pragma once

#include <pie/type_traits/integral_constant.hpp>
#include <pie/type_traits/add_const.hpp>
#include <pie/type_traits/add_lvalue_reference.hpp>
#include <pie/type_traits/add_rvalue_reference.hpp>

namespace pie
{
	template<class ToType, class FromType>
	struct is_assignable : pie::integral_constant<bool, __is_assignable(ToType, FromType)> {};
	
	template<class ToType, class FromType>
	struct is_copy_assignable : pie::is_assignable<typename pie::add_lvalue_reference<ToType>::type,
	                                               typename pie::add_lvalue_reference<typename pie::add_const<FromType>::type>::type>::type
	{
	};
	
	template<class ToType, class FromType>
	struct is_move_assignable : pie::is_assignable<typename pie::add_lvalue_reference<ToType>::type,
	                                               typename pie::add_rvalue_reference<FromType>::type>::type
	{
	};
}
