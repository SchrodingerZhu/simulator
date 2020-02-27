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

void Stack::grow() {
    auto new_start =  static_cast<char *>(mi_malloc(capacity << 1u));
    std::memcpy(new_start + capacity, highest - capacity, capacity);
    current = new_start + capacity;
    std::memset(new_start, 0, capacity);
    mi_free(highest - capacity);
    capacity <<= 1u;
    highest = new_start + capacity;
}

void Stack::enlarge(size_t n) {
    while (!isEnoughFor(n)) grow();
    current -= n;
}

void Stack::decrease(size_t n) {
    std::memset(current, 0, n);
    current += n;
}

long Stack::order(uint32_t addr) {
    return static_cast<long>(size()) - long(STACK_HIGH - addr);
}

void Stack::clear() {
    mi_free(highest - capacity);
    capacity = DEFAULT_SIZE;
    current = highest = static_cast<char *>(mi_malloc(capacity)) + capacity;
    std::memset(current - capacity, 0, capacity);
}
