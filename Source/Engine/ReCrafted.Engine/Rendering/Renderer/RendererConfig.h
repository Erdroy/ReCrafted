// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#define RENDERER_YES	1
#define RENDERER_NO		0

#define RENDERER_RHI_D3D11                  RENDERER_YES
#define RENDERER_RHI_D3D12                  RENDERER_NO
#define RENDERER_RHI_VULKAN                 RENDERER_NO
#define RENDERER_RHI_METAL                  RENDERER_NO

#define RENDERER_COMMAND_LIST_FULL_DISCARD	RENDERER_NO
#define RENDERER_SUPPORTS_MULTI_THREADING	RENDERER_YES

#define RENDERER_COMMAND_LIST_SIZE			(16 << 10)
#define RENDERER_COMMAND_LIST_INCREMENT		(8 << 10)
#define RENDERER_COMMAND_LIST_MIN_CMD_COUNT	(128)

#define RENDERER_MEMORY_AUTO_DEALLOC_ENABLE	RENDERER_YES

#define RENDERER_MAX_RENDER_THREADS			(1)
#define RENDERER_MAX_WINDOWS				(16)
#define RENDERER_MAX_RENDER_BUFFERS			(128)
#define RENDERER_MAX_RENDER_BUFFER_TARGETS  (16)
#define RENDERER_MAX_TEXTURES2D             (4 << 10)
#define RENDERER_MAX_VERTEX_BUFFERS         (16 << 10)
#define RENDERER_MAX_INDEX_BUFFERS          (16 << 10)
#define RENDERER_MAX_SHADER_PROGRAMS        (1 << 10)

#if RENDERER_MAX_RENDER_THREADS > 1
#error Only single multi-threading worker thread is currently supported!
#endif

