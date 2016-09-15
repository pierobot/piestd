#include <pie/stdint.hpp>
#include <pie/memset.hpp>

#pragma function(memset)
extern "C" void * __cdecl memset(void * dest, int ch, pie::size_t count)
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
}

