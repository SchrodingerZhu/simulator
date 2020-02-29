//
// Created by schrodinger on 2/27/20.
//

#include "stack.h"

size_t Stack::size() const {
    return highest - current;
}

bool Stack::isEnoughFor(size_t delta) {
    return current - delta + capacity >= highest;
}

Stack::Stack() {
    capacity = DEFAULT_SIZE;
    current = highest = static_cast<char *>(mi_malloc(capacity)) + capacity;
    std::memset(current - capacity, 0, capacity);
}

Stack::~Stack() {
    auto start = highest - capacity;
    mi_free(start);
}


void Stack::grow(size_t scale) {
    auto new_cap = capacity << scale;
    auto new_start = static_cast<char *>(mi_malloc(capacity << scale));
    std::memcpy(new_start + new_cap - capacity, highest - capacity, capacity);
    current = new_start + new_cap - capacity;
    std::memset(new_start, 0, new_cap - capacity);
    mi_free(highest - capacity);
    capacity <<= new_cap;
    highest = new_start + new_cap;
}

void Stack::enlarge(size_t n) {
    if (!isEnoughFor(n)) grow(nextPowerOfTwo(n) / capacity);
    current -= n;
}

void Stack::decrease(size_t n) {
    std::memset(current, 0, n);
    current += n;
}

long Stack::order(uint32_t addr) {
    return static_cast<long>(addr) - long(STACK_HIGH - size());
}

void Stack::clear() {
    mi_free(highest - capacity);
    capacity = DEFAULT_SIZE;
    current = highest = static_cast<char *>(mi_malloc(capacity)) + capacity;
    std::memset(current - capacity, 0, capacity);
}

uint32_t nextPowerOfTwo(uint32_t n) {
    n += !n;
    return (uint32_t) 1u << (31u - __builtin_clz((n << 1u) - 1u));
}
