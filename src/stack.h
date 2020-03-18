//
// Created by schrodinger on 2/27/20.
//

#ifndef SIMULATOR_STACK_H
#define SIMULATOR_STACK_H
#define DEFAULT_SIZE    1u
#define STACK_HIGH      0x7fffffffu

#include <cstddef>
#include <mimalloc.h>
#include <cstring>
#include <stdexcept>
/*!
 * a build in overflow check
 * @param a operand a
 * @param b operand b
 * @return whether the addition will result in an overflow
 */
static inline bool will_overflow(uint32_t a, uint32_t b) {
    static uint32_t c = 3;
    return __builtin_add_overflow(a, b, &c);
}
/*!
 * The stack simulator
 */
struct Stack {
    /// current capacity
    size_t capacity;
    /// memory area ending
    char *highest;
    /// memory available address
    char *current;
    /*!
     * Construct the stack.
     * This will pre-allocate some memory and initialize the pointers
     */
    Stack();
    /*!
     * Destruct the stack.
     * This will free the holding memory
     */
    ~Stack();
    /*!
     * Enlarge the stack capacity
     * @param scale The capacity to enlarge to
     */
    void grow(size_t scale);
    /*!
     * Check whether the current capacity is enough
     * @param target target size
     * @return the checking result
     */
    bool isEnoughFor(size_t target);
    /*!
     * The current size
     * @return The current size
     */
    [[nodiscard]] size_t size() const;

    /*!
     * Get the address at the simulated memory index
     * @tparam T data type
     * @param addr target index
     * @return real address
     */
    template<class T>
    T *get(uint32_t addr);

    /*!
     * Enlarge the stack size
     * @param n The size to enlarge by
     */
    void enlarge(size_t n);
    /*!
     * Decrease the stack size
     * @param n The size to decrease by
     */
    void decrease(size_t n);
    /*!
     * Get the order of an address
     * @param addr target address
     * @return order
     */
    long order(uint32_t addr);
    /*!
     * Clear the stack.
     * The behavior is quite similar to initialization
     */
    void clear();
};

template<class T>
T *Stack::get(uint32_t addr) {
    uint32_t type_size = sizeof(T);
    if (will_overflow(addr, type_size)) {
        throw std::runtime_error("invalid stack access");
    }
    auto real = size() - (STACK_HIGH - addr);
    return reinterpret_cast<T *>(current + real);
}


uint32_t nextPowerOfTwo(uint32_t n);

#endif //SIMULATOR_STACK_H
