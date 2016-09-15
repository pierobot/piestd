#pragma once

#include <pie/stdint.hpp>

namespace pie
{
	template<pie::size_t Size, pie::size_t AlignSize = sizeof(pie::size_t)>
	struct aligned_storage
	{
		typedef struct  
		{
			alignas(AlignSize) unsigned char data[Size];
		} type;
	};
}
