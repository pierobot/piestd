// To access a symbol in thread local storage, it is necessary to obtain access to the Thread Environment Block(TEB).
// A pointer to the TLS array is located at offset 0x2c from the TEB.
// The tls index(_tls_index) is multiplied by 4 and added to the address of the TLS array to obtain the address of the thread’s TLS data area.

#define BEGIN_EXTERN_C extern "C" {
#define END_EXTERN_C }

BEGIN_EXTERN_C
unsigned long _tls_array = 0x2C;
unsigned long _tls_index = 0;

END_EXTERN_C