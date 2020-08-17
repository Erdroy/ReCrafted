// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include "../RHIBase.h"

#if RENDERER_RHI_D3D12
namespace Renderer
{
	namespace RHI
	{
		class RHIDirectX12 : public RHIBase
		{
		private:
			static void kickFrameEvent();
			static void waitForGPU();

		public:
			void assignCommands();

		public:
			void Initialize(Settings::_enum settings, RenderFlags::_enum flags) override;
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
