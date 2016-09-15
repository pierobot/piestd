#pragma once

extern "C" void * __cdecl memset(void * dest, int ch, pie::size_t count);
#pragma intrinsic(memset)