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
/*!
 * A Policy Based Statistics Tree to help us keep the order of the allocated chunks
 */
template<class Key, class Value>
using TreeSet = tree<Key, Value, std::less<Key>, rb_tree_tag, tree_order_statistics_node_update>;

/*!
 * Heap help us to record and manage heap storage
 */
class Heap {
    /// Used to store the block size of each allocation
    TreeSet<uint32_t, size_t> mapping;
public:
    /// The size of current heap
    size_t size = 0;

    /*!
     * Clear all heap content
     */
    void clear();

    /*!
     * Deconstruct the heap, will do the same thing as clear();
     */
    ~Heap();

    /*!
     * Alloc a new memory block.
     * @param n memory block size
     * @return the address of the newly allocated memory block
     */
    uint32_t alloc(size_t n);

    /*!
     * Dealloc a memory block
     * @param addr the start address of the memory block.
     */
    void dealloc(uint32_t addr);

    /*!
     * Check the order of an address.
     * This is used in UI display to check which block to remove from the list.
     * @param addr the address to check
     * @return the order of the block
     */
    size_t order(uint32_t addr);
};

/*!
 * A segfault handler that is used to handle the invalid memory access
 * @param signal signal value
 * @param si signal information
 * @param arg arguments list
 */
void segfault_sigaction(int signal, siginfo_t *si, void *arg);

/*!
 * Bind the signal `SIGSEGV` to our own handler.
 */
void bind_sigsegv();


#endif // HEAP_H
