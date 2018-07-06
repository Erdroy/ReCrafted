// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef RHIBASE_H
#define RHIBASE_H

#include "../Renderer.hpp"
#include "../CommandList/CommandList.h"

#include "Core/Delegate.h"
#include "Core/Event.h"

namespace Renderer
{
    struct RHIContext;

    namespace RHI
    {
        class RHIBase
        {
        public:
            CommandList commandList = {};
            Event<void> callbacksBeforeRender = {};

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
            virtual void CreateVertexBuffer(VertexBufferHandle handle, uint vertexCount, uint vertexSize, bool dynamic, RendererMemory* buffer) = 0;
            virtual void CreateIndexBuffer(IndexBufferHandle handle, uint indexCount, bool is32bit, bool dynamic, RendererMemory* buffer) = 0;
            virtual void CreateWindowHandle(WindowHandle window, RenderBufferHandle renderBufferHandle, void* windowHandle) = 0;
            virtual void ResizeWindow(WindowHandle window, int width, int height) = 0;
        };
    }
}

#endif // RHIBASE_H
