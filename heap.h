#ifndef HEAP_H
#define HEAP_H
#include <map>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <list>
#include <mimalloc.h>
using namespace __gnu_pbds;
template <class Key, class Value>
using TreeSet = tree <Key, Value, std::less<Key>, rb_tree_tag, tree_order_statistics_node_update>;

struct Block {
    uint32_t fake;
    char* addr;
    size_t n;
};

class Heap
{
    TreeSet<uint32_t, Block> morphism;
    std::list<Block> small_free_list[128] = {};
    std::multimap<size_t, Block> large_free_list;
    uint32_t global = 0x10000;
public:
    size_t size = 0;
    void clear();
    ~Heap();
    uint32_t alloc(size_t n);
    void dealloc(uint32_t addr);
    size_t order(uint32_t addr);
    template<class T>
    T* get(uint32_t index);
};

template<class T>
T *Heap::get(uint32_t index)
{
    auto iter = morphism.upper_bound(index);
    if(morphism.empty() || iter == morphism.begin()) {
        throw std::runtime_error {"invalid memory access"};
    }
    iter--;
    if(iter->second.fake + iter->second.n < index + sizeof (T)) {
        throw std::runtime_error {"invalid memory access"};
    }
    return reinterpret_cast<T*>(iter->second.addr + (index - iter->second.fake));
}




#endif // HEAP_H
