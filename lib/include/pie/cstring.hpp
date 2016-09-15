#pragma once

#include <pie/stdint.hpp>
#include <pie/memset.hpp>

namespace pie
{
    /*void * __cdecl memset(void * dest, int ch, pie::size_t count)
    {
        if (dest != nullptr)
        {
            register char * dest_tmp = static_cast<char *>(dest);
            register char * end = dest_tmp + count;

            while (dest_tmp != end)
            {
                *dest_tmp++ = ch;
            }
        }

        return dest;
    }*/

	inline void * __cdecl memcpy(void * dest, void const * src, pie::size_t count)
	{
		if (dest != nullptr && src != nullptr)
		{
			register char * dest_tmp = reinterpret_cast<char *>(dest);
			register char const * src_tmp = reinterpret_cast<char const *>(src);

			while (count--)
			{
				*dest_tmp++ = *src_tmp++;
			}
		}
		
		return dest;
	}

	inline void * __cdecl memmove(void * dest, void const * src, pie::size_t count)
	{
		if (dest != nullptr && src != nullptr)
		{
			register char * dest_tmp = reinterpret_cast<char *>(dest);
			register char const * src_tmp = reinterpret_cast<char const *>(src);

			if (dest_tmp > src_tmp && dest_tmp < dest_tmp + count)
			{
				while (count--)
				{
					dest_tmp[count] = src_tmp[count];
				}
			}
			else
			{
				return memcpy(dest_tmp, src_tmp, count);
			}
		}

		return dest;
	}
}
