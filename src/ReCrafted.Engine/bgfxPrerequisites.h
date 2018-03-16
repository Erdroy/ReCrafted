// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef BGFXPREREQUISITES_H
#define BGFXPREREQUISITES_H

#define BGFX_CONFIG_MAX_VERTEX_BUFFERS 65535
#define BGFX_CONFIG_MAX_INDEX_BUFFERS  65535

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <math.h>

struct bgfxMemoryEx
{
public:
    char* memory = nullptr;
    size_t size = 0u;

public:
    const bgfx::Memory* getMemory();
    void release();

public:
    static bgfxMemoryEx alloc(size_t size)
    {
        bgfxMemoryEx mem;
        mem.memory = new char[size];
        mem.size = size;
        return mem;
    }
};

#endif // BGFXPREREQUISITES_H