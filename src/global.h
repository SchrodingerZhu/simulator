#ifndef GLOBAL_H
#define GLOBAL_H

#include <cstddef>
#include <cstdint>
#include <memory>
#include <mimalloc.h>
/// start address of the MIPS program
#define BASE_ADDR 0x000000u
#define STATIC_LOW  0x1000000u
#define STATIC_HIGH 0x1400000u
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
struct InstructionImpl;
namespace _SIM {
    struct InstrDeleter {
        void operator()(InstructionImpl *t) {
            mi_free(t);
        }
    };

    using InstrPtr = std::unique_ptr<InstructionImpl, InstrDeleter>;

    template<typename T, typename ...Args>
    InstrPtr make_unique(Args &&...args) {
        return InstrPtr(static_cast<InstructionImpl *>(new(mi_malloc(sizeof(T))) T(std::forward<Args>(args)...)),
                        InstrDeleter());
    }
}
#endif // GLOBAL_H
