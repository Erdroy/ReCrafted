// ReCrafted (c) 2016-2018 Always Too Late

#include "bgfxPrerequisites.h"

void releasebgfxmemory(void* _ptr, void* _userData)
{
    free(_ptr);
}

const bgfx::Memory* bgfxMemoryEx::getMemory() const
{
    return bgfx::makeRef(memory, static_cast<uint32_t>(size), releasebgfxmemory);
}

void bgfxMemoryEx::release()
{
    releasebgfxmemory(nullptr, this);
}
