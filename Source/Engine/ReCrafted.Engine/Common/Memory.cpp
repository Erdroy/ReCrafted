// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Memory.h"

aligned_malloc_t Memory::aligned_malloc;
free_aligned_t Memory::free_aligned;
malloc_t Memory::malloc;
calloc_t Memory::calloc;
realloc_t Memory::realloc;
free_t Memory::free;

void Memory::Initialize(const AllocatorType allocatorType)
{
    switch (allocatorType)
    {
    case AllocatorType::OS:
        aligned_malloc = &::_aligned_malloc;
        free_aligned = &::_aligned_free;
        malloc = &::malloc;
        calloc = &::calloc;
        realloc = &::realloc;
        free = &::free;
        break;
    case AllocatorType::JEMalloc:
        /*malloc = &::jemalloc;
        calloc = &::jecalloc;
        realloc = &::jerealloc;
        free = &::jefree;*/
        break;
    case AllocatorType::TBB:
        /*malloc = &::tbb_malloc;
        calloc = &::tbb_calloc;
        realloc = &::tbb_realloc;
        free = &::tbb_free;*/
        break;
    }
}