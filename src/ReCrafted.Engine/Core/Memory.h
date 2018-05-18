#pragma once

#ifndef MEMORY_H
#define MEMORY_H

// includes
#include "ReCraftedConfig.h"

#include <memory>
#include <rpmalloc.h>

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
