#include "heap.h"
#include <sys/mman.h>
#include <sstream>
uint32_t Heap::alloc(size_t n) {
    auto res = reinterpret_cast<uint64_t>
        (mmap(nullptr, n, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_32BIT,-1, 0));
    if (!res) throw std::runtime_error("out of memory");
    mapping[res] = n;
    size += n;
    return res;
}

void Heap::dealloc(uint32_t addr)
{
    auto length = mapping.find(addr);
    if (length == mapping.end()) throw std::runtime_error("invalid dealloc");
    munmap(reinterpret_cast<void *>(addr), length->second);
    size -= length->second;
}

void Heap::clear() {
    for(auto &i : mapping) {
        munmap(reinterpret_cast<void *>(i.first), i.second);
    }
}

Heap::~Heap() {
    clear();
}

size_t Heap::order(uint32_t addr) {
    return mapping.order_of_key(addr);
}

void segfault_sigaction(int signal, siginfo_t *si, void *arg) {
    std::stringstream ss;
    ss << "Caught segfault at address: " << std::hex << (size_t)si->si_addr;
    throw std::runtime_error(ss.str());
}

void bind_sigsegv() {
    struct sigaction sa {};
    memset(&sa, 0, sizeof(struct sigaction));
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = segfault_sigaction;
    sa.sa_flags   = SA_SIGINFO;
    sigaction(SIGSEGV, &sa, nullptr);
}
