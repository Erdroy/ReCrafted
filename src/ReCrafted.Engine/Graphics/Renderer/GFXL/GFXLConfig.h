// GFXL - Graphics Library (c) 2016-2017 Damian 'Erdroy' Korczowski

#pragma once

#ifndef GFXL_CONFIG_H
#define GFXL_CONFIG_H

#ifdef _DEBUG
#	include <cassert>
#	define GFXL_ASSERT(x) assert(x)
#else
#	define GFXL_ASSERT(x)
#endif

#define GFXL_YES	1
#define GFXL_NO		0

#define GFXL_RENDERER_D3D11             GFXL_YES
#define GFXL_RENDERER_D3D12             GFXL_NO
#define GFXL_RENDERER_VULKAN            GFXL_NO
#define GFXL_RENDERER_METAL             GFXL_NO

#define GFXL_COMMAND_LIST_FULL_DISCARD	GFXL_NO
#define GFXL_SUPPORTS_MULTI_THREADING	GFXL_YES

#define GFXL_COMMAND_LIST_SIZE			(16 << 10)
#define GFXL_COMMAND_LIST_CMD_COUNT		(4 << 10)
#define GFXL_COMMAND_LIST_INCREMENT		(8 << 10)

#define GFXL_MAX_WORKER_THREADS			(16)
#define GFXL_MAX_WINDOWS				(16)
#define GFXL_MAX_RENDER_BUFFERS			(128)
#define GFXL_MAX_RENDER_BUFFER_TARGETS  (16)
#define GFXL_MAX_TEXTURES2D             (4 << 10)
#define GFXL_MAX_SHADERS                (1 << 10)

#endif // GFXL_CONFIG_H
