// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef RHIBASE_H
#define RHIBASE_H

#include "../../GFXL.hpp"
#include "../CommandList/CommandList.h"

namespace GFXL
{
	namespace RHI
	{
		class RHIBase
		{
		public:
			CommandList commandList = {};

		public:
			virtual ~RHIBase() = default;

		public:
			virtual void Initialize(Settings::_enum settings, ResetFlags::_enum flags) = 0;
			virtual void Shutdown() = 0;

		public:
			virtual void Frame() = 0;

        public:
            virtual void CreateWindowHandle(WindowHandle window, RenderBufferHandle renderBufferHandle, void* windowHandle) = 0;
            virtual void CreateTexture2D(Texture2DHandle textureHandle, uint16_t width, uint16_t height, TextureFormat::_enum textureFormat) = 0;

		public:
            virtual void ResizeWindow(WindowHandle window, int width, int height) = 0;
		};
	}
}

#endif // RHIBASE_H
