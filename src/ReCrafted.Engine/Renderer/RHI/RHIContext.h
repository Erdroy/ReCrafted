// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef RHICONTEXT_H
#define RHICONTEXT_H

// Includes
#include "../Renderer.hpp"

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

#endif // RHICONTEXT_H
