// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef CONFIG_H
#define CONFIG_H

// Maximum amount of vertex buffers. Default: 65536
#define RENDERER_MAX_VERTEX_BUFFERS uint(64 << 10)

// Maximum amount of index buffers. Default: 65536
#define RENDERER_MAX_INDEX_BUFFERS uint(64 << 10)

// Maximum amount of shaders. Default: 1024
#define RENDERER_MAX_SHADERS uint(1 << 10) 

// Maximum amount of textures (Texture2D and other). Default: 1024
#define RENDERER_MAX_TEXTURES uint(1 << 10) 

// Maximum amount of render buffers. Default: 16
#define RENDERER_MAX_RENDER_BUFFERS uint(8 << 1) 

// Maximum amount of render buffer targets (textures incl. depth). Default: 16
#define RENDERER_MAX_RENDER_BUFFER_TARGETS uint(8 << 1)

#endif // CONFIG_H
