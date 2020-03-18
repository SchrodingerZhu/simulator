#ifndef GLOBAL_H
#define GLOBAL_H

#include <cstddef>
#include <cstdint>
#include <memory>
#include <mimalloc.h>
/// start address of the MIPS program
#define BASE_ADDR 0x000000u
#define STATIC_LOW  0x500000u
extern size_t STATIC_HIGH;
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
/*!
 * prototype for real implementation of instruction behaviors
 */
struct InstructionImpl;
/*!
 * This namespace contains some workarounds to provide back support
 */
namespace _SIM {
    /*!
     * The deleter for the unique_ptr of instruction implementations
     */
    struct InstrDeleter {
        void operator()(InstructionImpl *t) {
            mi_free(t);
        }
    };
    /*!
     * Alisa of the unique_ptr of instruction implementations
     */
    using InstrPtr = std::unique_ptr<InstructionImpl, InstrDeleter>;
    /*!
     * C++ 11 do not have make_unique. This functions works as an subtitution.
     * @tparam T data type
     * @tparam Args initialization args type
     * @param args initialization args
     * @return the unique ptr
     */
    template<typename T, typename ...Args>
    InstrPtr make_unique(Args &&...args) {
        return InstrPtr(static_cast<InstructionImpl *>(new(mi_malloc(sizeof(T))) T(std::forward<Args>(args)...)),
                        InstrDeleter());
    }
}
#endif // GLOBAL_H
