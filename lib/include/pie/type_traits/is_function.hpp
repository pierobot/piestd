#pragma once

#include <pie/type_traits/integral_constant.hpp>

namespace pie
{
    // primary template
    template<class>
    struct is_function : pie::false_type { };

    // specialization for regular functions
    template<class Ret, class... Args>
    struct is_function<Ret(Args...)> : pie::true_type {};

    // specialization for variadic functions such as pie::printf
    //template<class Ret, class... Args>
    //struct is_function<Ret(Args......)> : pie::true_type {};

    // specialization for function types that have cv-qualifiers
    template<class Ret, class... Args>
    struct is_function<Ret(Args...)const> : pie::true_type {};
    template<class Ret, class... Args>
    struct is_function<Ret(Args...)volatile> : pie::true_type {};
    template<class Ret, class... Args>
    struct is_function<Ret(Args...)const volatile> : pie::true_type {};
    //template<class Ret, class... Args>
    //struct is_function<Ret(Args......)const> : pie::true_type {};
    //template<class Ret, class... Args>
    //struct is_function<Ret(Args......)volatile> : pie::true_type {};
    //template<class Ret, class... Args>
    //struct is_function<Ret(Args......)const volatile> : pie::true_type {};

    // specialization for function types that have ref-qualifiers
    template<class Ret, class... Args>
    struct is_function<Ret(Args...) &> : pie::true_type {};
    template<class Ret, class... Args>
    struct is_function<Ret(Args...)const &> : pie::true_type {};
    template<class Ret, class... Args>
    struct is_function<Ret(Args...)volatile &> : pie::true_type {};
    template<class Ret, class... Args>
    struct is_function<Ret(Args...)const volatile &> : pie::true_type {};
    //template<class Ret, class... Args>
    //struct is_function<Ret(Args......) &> : pie::true_type {};
    //template<class Ret, class... Args>
    //struct is_function<Ret(Args......)const &> : pie::true_type {};
    //template<class Ret, class... Args>
    //struct is_function<Ret(Args......)volatile &> : pie::true_type {};
    //template<class Ret, class... Args>
    //struct is_function<Ret(Args......)const volatile &> : pie::true_type {};
    template<class Ret, class... Args>
    struct is_function<Ret(Args...) && > : pie::true_type {};
    template<class Ret, class... Args>
    struct is_function<Ret(Args...)const &&> : pie::true_type {};
    template<class Ret, class... Args>
    struct is_function<Ret(Args...)volatile &&> : pie::true_type {};
    template<class Ret, class... Args>
    struct is_function<Ret(Args...)const volatile &&> : pie::true_type {};
    //template<class Ret, class... Args>
    //struct is_function<Ret(Args......) && > : pie::true_type {};
    //template<class Ret, class... Args>
    //struct is_function<Ret(Args......)const &&> : pie::true_type {};
    //template<class Ret, class... Args>
    //struct is_function<Ret(Args......)volatile &&> : pie::true_type {};
    //template<class Ret, class... Args>
    //struct is_function<Ret(Args......)const volatile &&> : pie::true_type {};
    // 

    template<class T>
    constexpr bool is_function_v = pie::is_function<T>::value;
}
