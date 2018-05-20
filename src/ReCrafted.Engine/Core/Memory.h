#pragma once

#ifndef MEMORY_H
#define MEMORY_H

// includes
#include "ReCraftedConfig.h"

#include <memory>
#include <rpmalloc.h>
#include <cstdlib>
#include <new>

#if COMPILE_WITH_RPMALLOC
#pragma warning( push )
#pragma warning( disable : 4595)
inline void* operator new(size_t size) {
    return rpmalloc(size);
}

inline void operator delete(void* ptr) noexcept {
    if (rpmalloc_is_thread_initialized())
        rpfree(ptr);
}

inline void* operator new[](size_t size) {
    return rpmalloc(size);
}

inline void operator delete[](void* ptr) noexcept {
    if (rpmalloc_is_thread_initialized())
        rpfree(ptr);
}

inline void* operator new(size_t size, const std::nothrow_t&) noexcept {
    return rpmalloc(size);
}

inline void* operator new[](size_t size, const std::nothrow_t&) noexcept {
    return rpmalloc(size);
}

inline void operator delete(void* ptr, const std::nothrow_t&) noexcept {
    if (rpmalloc_is_thread_initialized())
        rpfree(ptr);
}

inline void operator delete[](void* ptr, const std::nothrow_t&) noexcept {
    if (rpmalloc_is_thread_initialized())
        rpfree(ptr);
}

inline void operator delete(void* ptr, size_t) noexcept {
    if (rpmalloc_is_thread_initialized())
        rpfree(ptr);
}

inline void operator delete[](void* ptr, size_t) noexcept {
    if (rpmalloc_is_thread_initialized())
        rpfree(ptr);
}

template <class T>
struct RPAllocator {
    typedef T value_type;
    RPAllocator() = default;
    template <class U> constexpr RPAllocator(const RPAllocator<U>&) noexcept {}
    T* allocate(std::size_t n) {
        if (n > std::size_t(-1) / sizeof(T)) throw std::bad_alloc();
        if (auto p = static_cast<T*>(rpmalloc(n * sizeof(T)))) return p;
        throw std::bad_alloc();
    }
    void deallocate(T* p, std::size_t) noexcept { rpfree(p); }
};
template <class T, class U>
bool operator==(const RPAllocator<T>&, const RPAllocator<U>&) { return true; }
template <class T, class U>
bool operator!=(const RPAllocator<T>&, const RPAllocator<U>&) { return false; }

#pragma warning( pop )
#endif

struct RPMallocThread
{
public:
    RPMallocThread()
    {
        rpmalloc_thread_initialize();
    }

    ~RPMallocThread()
    {
        // TODO: Fix thread finalization
        //rpmalloc_thread_finalize();
    }
};
#endif // MEMORY_H
