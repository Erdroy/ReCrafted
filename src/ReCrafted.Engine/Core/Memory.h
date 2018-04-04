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
inline void* operator new(const std::size_t size) noexcept
{
    return rpmalloc(size);
}

inline void* operator new[](const std::size_t size) noexcept
{
    return rpmalloc(size);
}

inline void operator delete(void* ptr) noexcept
{
    rpfree(ptr);
}

inline void operator delete[](void* ptr) noexcept
{
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
        rpmalloc_thread_finalize();
    }
};
#endif // MEMORY_H
