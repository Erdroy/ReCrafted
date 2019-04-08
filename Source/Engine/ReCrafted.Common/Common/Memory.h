// ReCrafted (c) 2016-2019 Always Too Late. All rights reserved.. All rights reserved.

#pragma once

#ifndef MEMORY_H
#define MEMORY_H

#include <ReCrafted.h>

#include <memory>
#include <cstdlib>
#include <new>

typedef void*(*malloc_t)(size_t);
typedef void*(*calloc_t)(size_t, size_t);
typedef void*(*realloc_t)(void*, size_t);
typedef void(*free_t)(void*);

class Memory
{
public:
    enum class AllocatorType
    {
        OS,
        JEMalloc,
        TBB
    };

public:
    static void RC_API Initialize(AllocatorType allocatorType);
};

#pragma warning( push )
#pragma warning( disable : 4595)

inline void* rc_malloc(const size_t size)
{
    return malloc(size);
}

inline void* rc_calloc(const size_t num, const size_t size)
{
    return calloc(num, size);
}

inline void* rc_realloc(void *ptr, const size_t size)
{
    return realloc(ptr, size);
}

inline void rc_free(void* ptr)
{
    free(ptr);
}

inline void* operator new(size_t size) {
    return rc_malloc(size);
}

inline void operator delete(void* ptr) noexcept {
    rc_free(ptr);
}

inline void* operator new[](size_t size) {
    return rc_malloc(size);
}

inline void operator delete[](void* ptr) noexcept {
    rc_free(ptr);
}

inline void* operator new(size_t size, const std::nothrow_t&) noexcept {
    return rc_malloc(size);
}

inline void* operator new[](size_t size, const std::nothrow_t&) noexcept {
    return rc_malloc(size);
}

inline void operator delete(void* ptr, const std::nothrow_t&) noexcept {
    rc_free(ptr);
}

inline void operator delete[](void* ptr, const std::nothrow_t&) noexcept {
    rc_free(ptr);
}

inline void operator delete(void* ptr, size_t) noexcept {
    rc_free(ptr);
}

inline void operator delete[](void* ptr, size_t) noexcept {
    rc_free(ptr);
}

#pragma warning( pop )

#endif // MEMORY_H
