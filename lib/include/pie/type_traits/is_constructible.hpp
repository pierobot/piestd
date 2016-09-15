#pragma once

#include <pie/type_traits/integral_constant.hpp>
#include <pie/type_traits/add_const.hpp>
#include <pie/type_traits/add_lvalue_reference.hpp>
#include <pie/type_traits/add_rvalue_reference.hpp>

namespace pie
{
    template<class Type, class... Args>
    struct is_constructible : pie::integral_constant<bool, __is_constructible(Type, Args...)> {};
    
    template<class Type>
    struct is_default_constructible : pie::is_constructible<Type> {};
    
    template<class Type>
    struct is_copy_constructible : pie::is_constructible<Type, typename pie::add_const<
                                                               typename pie::add_lvalue_reference<Type>::type>::type>
    {
    };
    
    template<class Type>
    struct is_move_constructible : pie::is_constructible<Type, typename pie::add_rvalue_reference<Type>::type> {};
	
	// ---------------------
	
	template<class Type, class... Args>
    struct is_nothrow_constructible : pie::integral_constant<bool, __is_nothrow_constructible(Type, Args...)> {};
    
    template<class Type>
    struct is_nothrow_default_constructible : pie::is_nothrow_constructible<Type> {};
    
    template<class Type>
    struct is_nothrow_copy_constructible : pie::is_nothrow_constructible<Type, typename pie::add_const<typename pie::add_lvalue_reference<Type>::type>::type>
    {
    };
    
    template<class Type>
    struct is_nothrow_move_constructible : pie::is_nothrow_constructible<Type, typename pie::add_rvalue_reference<Type>::type> {};

	template<class Type>
	bool const is_nothrow_move_constructible_v = is_nothrow_move_constructible<Type>::value;
}
