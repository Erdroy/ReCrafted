// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Renderer/Renderer.h"

namespace Renderer
{
    typedef void* RHIDevice;
    typedef void* RHIDeviceContext;
    typedef void* RHIWindowHandle;
    typedef void* RHISwapChain;
    typedef void* RHIRenderTargetView;

    struct RHIContext
    {
    public:
        struct WindowDescription
        {
            RHIWindowHandle windowHandle = nullptr;
            RHISwapChain swapChain = nullptr;
            RHIRenderTargetView backBuffer = {};
        };

    public:
        RHIDevice device = nullptr;
        RHIDeviceContext deviceContext = nullptr;
        WindowDescription windows[RENDERER_MAX_WINDOWS] = {};
    };
}
