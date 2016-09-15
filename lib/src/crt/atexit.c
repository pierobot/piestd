
// The code in the atexit function should not contain any dependency on any DLL which could have already been unloaded when the atexit function is called.
extern "C" int __cdecl atexit(void(__cdecl * function)(void))
{
	return 0;
}
