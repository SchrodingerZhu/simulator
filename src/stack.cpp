//
// Created by schrodinger on 2/27/20.
//

#include "stack.h"

size_t Stack::size() const {
    return highest - current;
}

bool Stack::isEnoughFor(size_t delta) {
    return delta <= capacity;
}

Stack::Stack() {
    capacity = DEFAULT_SIZE;
    current = highest = static_cast<char *>(mi_zalloc(capacity)) + capacity;
}

Stack::~Stack() {
    auto start = highest - capacity;
    mi_free(start);
}


void Stack::grow(size_t new_cap) {
    auto new_start = static_cast<char *>(mi_zalloc(new_cap));
    std::memcpy(new_start + new_cap - size(), current, size());
    current = new_start + new_cap - size();
    mi_free(highest - capacity);
    capacity = new_cap;
    highest = new_start + new_cap;
}

void Stack::enlarge(size_t n) {
    if (!isEnoughFor(size() + n)) grow(nextPowerOfTwo(size() + n + 1));
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
    current = highest = static_cast<char *>(mi_zalloc(capacity)) + capacity;
}

uint32_t nextPowerOfTwo(uint32_t n) {
    n += !n;
    return (uint32_t) 1u << (31u - __builtin_clz((n << 1u) - 1u));
}
