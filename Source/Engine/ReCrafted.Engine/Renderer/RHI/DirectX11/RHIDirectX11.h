// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include "../RHIBase.h"

#if RENDERER_RHI_D3D11

namespace Renderer
{
    struct RHIContext;

    namespace RHI
    {
        class RHIDirectX11 : public RHIBase
        {
        public:
            static RHIDirectX11* m_instance;

        private:
            static void kickFrameEvent();
            static void waitForGPU();

            void PreFrameRender();

        public:
            void assignCommands();

        public:
            void Initialize(Settings::_enum settings, RenderFlags::_enum flags) override;
            void Shutdown() override;

            void GetRenderStatistics(RenderStatistics* stats) override;
            void GetContext(Renderer::RHIContext* context) override;

        public:
            void Frame() override;

        public:
            void CreateVertexBuffer(VertexBufferHandle handle, uint vertexCount, uint vertexSize, bool dynamic, RendererMemory buffer) override;
            void CreateIndexBuffer(IndexBufferHandle handle, uint indexCount, bool is32bit, bool dynamic, RendererMemory buffer) override;
            void DestroyVertexBuffer(VertexBufferHandle handle) override;
            void DestroyIndexBuffer(IndexBufferHandle handle) override;
            
            void CreateWindowHandle(WindowHandle window, RenderBufferHandle renderBufferHandle, void* windowHandle) override;
            void ResizeWindow(WindowHandle window, int width, int height) override;

        public:
            void UpdateTextureSubresource(Texture2DHandle textureHandle, void* data, size_t dataSize, uint8_t subresourceId) override;
            void CopyTextureSubresource(Texture2DHandle targetTexture, void* sourceTexturePtr, uint8_t subresourceId) override;
            void GetTextureSubresource(Texture2DHandle textureHandle, void* buffer, size_t bufferSize, uint8_t subresourceId) override;
        };
    }
}
#endif