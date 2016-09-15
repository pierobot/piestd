#include "./crt/string.c"

#ifdef _WIN32
#    include "./crt/threads_windows.c"
#endif

#ifdef _MSC_VER
#    include "./crt/atexit.c"
#endif

#ifdef __cplusplus
#    ifdef _MSC_VER
#        include "./crt/thread_safe_statics.cpp"
#        include "./crt/tls.cpp"
#    endif

#    include "./crt/main_pie.cpp"
#    include "./crt/memset.cpp"
#    include "./crt/purecall.cpp"
#    include "./crt/terminate.cpp"
#endif
