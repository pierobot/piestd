#pragma once

#include <pie/stdint.hpp>
#include <pie/type_traits/integral_constant.hpp>

namespace pie
{
	template<class T>
	struct alignment_of : pie::integral_constant<pie::size_t, alignof(T)>
	{
	};
}
