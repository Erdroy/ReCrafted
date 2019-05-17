// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Rendering/Renderer/Renderer.h"
#include "Rendering/Renderer/CommandList/CommandList.h"

#include "Common/Event.h"

namespace Renderer
{
    struct RHIContext;

    namespace RHI
    {
        class RHIBase
        {
        public:
            typedef void*(*vbhFreePtr)(VertexBufferHandle);
            typedef void*(*ibhFreePtr)(IndexBufferHandle);

        public:
            CommandList commandList = {};
            Event<> onPresentBegin = {};
            Event<> onPresentEnd = {};

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
            virtual void CreateVertexBuffer(VertexBufferHandle handle, uint vertexCount, uint vertexSize, bool dynamic, RendererMemory buffer) = 0;
            virtual void CreateIndexBuffer(IndexBufferHandle handle, uint indexCount, bool is32bit, bool dynamic, RendererMemory buffer) = 0;
            virtual void DestroyVertexBuffer(VertexBufferHandle handle) = 0;
            virtual void DestroyIndexBuffer(IndexBufferHandle handle) = 0;
            
            virtual void CreateWindowHandle(WindowHandle window, RenderBufferHandle renderBufferHandle, void* windowHandle) = 0;
            virtual void ResizeWindow(WindowHandle window, int width, int height) = 0;

        public:
            virtual void UpdateTextureSubresource(Texture2DHandle textureHandle, void* data, size_t dataSize, uint8_t subresourceId) = 0;
            virtual void CopyTextureSubresource(Texture2DHandle targetTexture, void* sourceTexturePtr, uint8_t subresourceId) = 0;
            virtual void GetTextureSubresource(Texture2DHandle textureHandle, void* buffer, size_t bufferSize, uint8_t subresourceId) = 0;
        };
    }
}
