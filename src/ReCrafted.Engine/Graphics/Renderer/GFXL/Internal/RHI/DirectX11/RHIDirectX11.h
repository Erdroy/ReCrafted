// GFXL - Graphics Library (c) 2016-2017 Damian 'Erdroy' Korczowski

#pragma once

#ifndef RHIDIRECTX11_H
#define RHIDIRECTX11_H

#include "../RHIBase.h"

#if GFXL_RENDERER_D3D11
namespace GFXL
{
    namespace RHI
    {
        class RHIDirectX11 : public RHIBase
        {
        private:
            static void kickFrameEvent();
            static void waitForGPU();

        public:
            void assignCommands();

        public:
            void Initialize(Settings::_enum settings, ResetFlags::_enum flags) override;
            void Shutdown() override;

        public:
            void Frame() override;

        public:
            void CreateWindowHandle(WindowHandle window, RenderBufferHandle renderBufferHandle, void* windowHandle) override;
            void CreateTexture2D(Texture2DHandle textureHandle, uint16_t width, uint16_t height, TextureFormat::_enum textureFormat) override;

        public:
            void ResizeWindow(WindowHandle window, int width, int height) override;
        };
    }
}
#endif
#endif // RHIDIRECTX11_H
