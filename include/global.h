#ifndef GLOBAL_H
#define GLOBAL_H

#include <cstddef>
#include <cstdint>
/// start address of the MIPS program
#define BASE_ADDR 0x000000u
/// a mapping between register no to register name
extern const char *REG_NAME[32];
#if defined(__GNUC__)
/// a macro to improve branch prediction
#define LIKELY(x) __glibc_likely(x)
/// a macro to improve branch prediction
#define UNLIKELY(x) __glibc_unlikely(x)
#else
#define LIKELY(x) (x)
#define UNLIKELY(x) (x)
#endif
#endif // GLOBAL_H
