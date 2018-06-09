// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef RHIBASE_H
#define RHIBASE_H

#include "../Renderer.hpp"
#include "../CommandList/CommandList.h"

namespace Renderer
{
    struct RHIContext;

    namespace RHI
    {
        class RHIBase
        {
        public:
            CommandList commandList = {};

        public:
            virtual ~RHIBase() = default;

        public:
            virtual void Initialize(Settings::_enum settings, RenderFlags::_enum flags) = 0;
            virtual void Shutdown() = 0;

            virtual void GetRenderStatistics(RenderStatistics* stats) = 0;
            virtual void GetContext(Renderer::RHIContext* context) = 0;

        public:
            virtual void Frame() = 0;

        public:
            virtual void CreateWindowHandle(WindowHandle window, RenderBufferHandle renderBufferHandle, void* windowHandle) = 0;
            virtual void ResizeWindow(WindowHandle window, int width, int height) = 0;
        };
    }
}

#endif // RHIBASE_H
