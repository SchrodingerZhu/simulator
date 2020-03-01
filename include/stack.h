//
// Created by schrodinger on 2/27/20.
//

#ifndef SIMULATOR_STACK_H
#define SIMULATOR_STACK_H
#define DEFAULT_SIZE    64u
#define STACK_HIGH      0xffffffffu

#include <cstddef>
#include <mimalloc.h>
#include <cstring>
#include <stdexcept>

static inline bool will_overflow(uint32_t a, uint32_t b) {
    static uint32_t c = 3;
    return __builtin_add_overflow(a, b, &c);
}

struct Stack {
    size_t capacity;
    char *highest;
    char *current;

    Stack();

    ~Stack();

    void grow(size_t scale);

    bool isEnoughFor(size_t delta);

    [[nodiscard]] size_t size() const;

    template<class T>
    T *get(uint32_t addr);

    void enlarge(size_t n);

    void decrease(size_t n);

    long order(uint32_t addr);

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
