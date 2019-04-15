// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Memory.h"

malloc_t Memory::malloc;
calloc_t Memory::calloc;
realloc_t Memory::realloc;
free_t Memory::free;

void Memory::Initialize(const AllocatorType allocatorType)
{
    switch (allocatorType)
    {
    case AllocatorType::OS:
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