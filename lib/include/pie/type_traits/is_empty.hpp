#pragma once

#include <pie/type_traits/integral_constant.hpp>
#include <pie/type_traits/conditional.hpp>

#pragma warning(push)
#pragma warning(disable:4624)

namespace pie
{
	namespace detail
	{
		template<class T>
		struct is_empty_helper_1 : public T
		{
		public:
            is_empty_helper_1() {}
			
			int dummy[256];
		private:
            is_empty_helper_1(const is_empty_helper_1&);
            is_empty_helper_1& operator=(const is_empty_helper_1&);
		};
		
		struct is_empty_helper_2 { int dummy[256]; };
	}
	
	template<class T> struct is_empty :
        pie::integral_constant<bool, sizeof(pie::detail::is_empty_helper_1<T>) == sizeof(pie::detail::is_empty_helper_2)>
    {};
}

#pragma warning(pop)

