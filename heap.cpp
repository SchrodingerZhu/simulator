#include "heap.h"


uint32_t Heap::alloc(size_t n) {
    if (n == 0) {return 0; }
    if (n <= 128) {
        if (!small_free_list[n - 1].empty()) {
            auto block = small_free_list[n - 1].back();
            small_free_list[n - 1].pop_back();
            morphism.insert({block.fake, block});
            size += n;
            return block.fake;
        } else {
            auto block = Block { .fake = global, .addr = static_cast<char *>(mi_malloc(n)), .n = n};
            if (block.addr == nullptr) throw std::runtime_error {"failed to malloc"};
            global += n;
            size += n;
            morphism.insert({block.fake, block});
            return block.fake;
        }
    }

    auto iter = large_free_list.lower_bound(n);
    if (iter != large_free_list.end()) {
        auto block = iter->second;
        block.addr = static_cast<char *>(mi_malloc(block.n));
        if (block.addr == nullptr) throw std::runtime_error {"failed to malloc"};
        large_free_list.erase(iter);
        morphism.insert({block.fake, block});
        size += block.n;
        return block.fake;
    } else {
        auto block = Block {.fake = global, .addr = static_cast<char *>(mi_malloc(n))};
        if (block.addr == nullptr) throw std::runtime_error {"failed to malloc"};
        global += n;
        morphism.insert({block.fake, block});
        size += n;
        return block.fake;
    }
}

void Heap::dealloc(uint32_t addr)
{
    auto iter = morphism.find(addr);
    if (iter == morphism.end()) throw std::runtime_error {"invalid deallocation"};
    auto block = iter->second;
    morphism.erase(iter);
    if (block.n <= 128) {
        small_free_list[block.n - 1].push_front(block);
    } else {
        block.addr = nullptr;
        mi_free(block.addr);
        large_free_list.insert({block.n, block});
    }
    size -= block.n;
}




void Heap::clear() {
    for (auto& l: small_free_list) {
        for (auto& i: l) {
            mi_free(i.addr);
        }
        l.clear();
    }
    global = 0x10000;
    for (auto& i : morphism) {
        mi_free(i.second.addr);
    }
    morphism.clear();
    large_free_list.clear();
}

Heap::~Heap() {
    clear();
}

size_t Heap::order(uint32_t addr) {
    return morphism.order_of_key(addr);
}
