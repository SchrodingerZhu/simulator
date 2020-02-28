#ifndef HEAP_H
#define HEAP_H
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <list>
#include <queue>
#include <mimalloc.h>
#include <csignal>
#include <cstring>

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
    TreeSet<uint32_t, size_t> mapping;
public:
    size_t size = 0;
    void clear();
    ~Heap();
    uint32_t alloc(size_t n);
    void dealloc(uint32_t addr);
    size_t order(uint32_t addr);
};


void segfault_sigaction(int signal, siginfo_t *si, void *arg);

void bind_sigsegv();


#endif // HEAP_H
