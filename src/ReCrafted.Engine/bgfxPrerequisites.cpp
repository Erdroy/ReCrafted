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
    delete[] static_cast<char*>(_ptr);
}

const bgfx::Memory* bgfxMemoryEx::getMemory() const
{
    return bgfx::makeRef(memory, static_cast<uint32_t>(size), releasebgfxmemory);
}

void bgfxMemoryEx::release()
{
    releasebgfxmemory(nullptr, this);
}
