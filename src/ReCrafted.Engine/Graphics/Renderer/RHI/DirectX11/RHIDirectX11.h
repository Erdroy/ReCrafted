// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef RHIDIRECTX11_H
#define RHIDIRECTX11_H

#include "../RHIBase.h"

#if RENDERER_RHI_D3D11
namespace Renderer
{
    namespace RHI
    {
        class RHIDirectX11 : public RHIBase
        {
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

        public:
            void Frame() override;

        public:
            void CreateWindowHandle(WindowHandle window, RenderBufferHandle renderBufferHandle, void* windowHandle)
            override;

        public:
            void ResizeWindow(WindowHandle window, int width, int height) override;
        };
    }
}
#endif
#endif // RHIDIRECTX11_H
