#pragma once

#ifndef TEST_PIE_ALLOC
#include <pie/stdint.hpp>

#include <Windows.h>

namespace pie
{
    inline void * malloc(pie::size_t size)
    {
	return ::HeapAlloc(::GetProcessHeap(), 0, size);
    }

    inline void * calloc(pie::size_t count, pie::size_t size)
    {
        return ::HeapAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, count * size);
    }

    inline void free(void * memory)
    {
        ::HeapFree(::GetProcessHeap(), 0, memory);
    }
}

#pragma warning(push)
#pragma warning(disable:4595)

// non-placement new
inline void * __cdecl operator new(pie::size_t size)
{
    return pie::malloc(size);
}

// placement new
inline void * __cdecl operator new(pie::size_t, void * where)
{
    return where;
}

// non-placement new[]
inline void * __cdecl operator new[](pie::size_t size)
{
    return pie::malloc(size);
}

// placement new[]
inline void * __cdecl operator new[](pie::size_t, void * where)
{
    return where;
}

// non-placement delete
inline void __cdecl operator delete(void * memory)
{
    pie::free(memory);
}

// placement delete
inline void __cdecl operator delete(void * memory, void *)
{
}

// non-placement delete
inline void __cdecl operator delete(void * memory, pie::size_t)
{
    pie::free(memory);
}

// placement delete[]
inline void __cdecl operator delete[](void * memory, void *)
{
}

// non-placement delete
inline void __cdecl operator delete[](void * memory)
{
    pie::free(memory);
}

// non-placement delete
inline void __cdecl operator delete[](void * memory, pie::size_t)
{
    pie::free(memory);
}

#pragma warning(pop)

#endif
