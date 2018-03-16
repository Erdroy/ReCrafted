// ReCrafted (c) 2016-2018 Always Too Late

#include "bgfxPrerequisites.h"

#ifdef DEBUG
#pragma comment (lib, "bgfxDebug.lib")
#pragma comment (lib, "bxDebug.lib")

#pragma comment (lib, "bimgDebug.lib")
#pragma comment (lib, "bimg_decodeDebug.lib")
#else
#pragma comment (lib, "bgfxRelease.lib")
#pragma comment (lib, "bxRelease.lib")
#pragma comment (lib, "bimgRelease.lib")
#pragma comment (lib, "bimg_decodeRelease.lib")
#endif

void releasebgfxmemory(void* _ptr, void* _userData)
{
    const auto memory = static_cast<bgfxMemoryEx*>(_userData);

    if (memory->memory)
        delete[] memory->memory;
}

const bgfx::Memory* bgfxMemoryEx::getMemory()
{
    return bgfx::makeRef(memory, size, releasebgfxmemory, this);
}

void bgfxMemoryEx::release()
{
    releasebgfxmemory(nullptr, this);
}
