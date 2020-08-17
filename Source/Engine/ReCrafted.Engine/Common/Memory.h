// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <memory>
#include <cstdlib>
#include <new>

typedef void*(*aligned_malloc_t)(size_t, size_t);
typedef void(*free_aligned_t)(void*);
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
    static void Initialize(AllocatorType allocatorType);

public:
    static aligned_malloc_t aligned_malloc;
    static free_aligned_t free_aligned;
    static malloc_t malloc;
    static calloc_t calloc;
    static realloc_t realloc;
    static free_t free;
};

#pragma warning( push )
#pragma warning( disable : 4595)

inline void* rc_aligned_alloc(const size_t size, const size_t alignment)
{
    return Memory::aligned_malloc(size, alignment);
}

inline void rc_free_aligned(void* block)
{
    Memory::free_aligned(block);
}

inline void* rc_malloc(const size_t size)
{
    return Memory::malloc(size);
}

inline void* rc_calloc(const size_t num, const size_t size)
{
    return Memory::calloc(num, size);
}

inline void* rc_realloc(void *ptr, const size_t size)
{
    return Memory::realloc(ptr, size);
}

inline void rc_free(void* ptr)
{
    Memory::free(ptr);
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

