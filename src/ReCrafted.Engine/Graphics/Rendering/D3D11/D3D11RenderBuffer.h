// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef D3D11RENDERBUFFER_H
#define D3D11RENDERBUFFER_H

#include "../Config.h"

struct ID3D11RenderBuffer
{
public:
	// TODO: impl

public:
	void Release()
	{
		delete this;
	}
};

inline ID3D11RenderBuffer* CreateRenderBuffer(
	uint textureCount, 
	ID3D11Texture2D* textures[RENDERER_MAX_RENDER_BUFFER_TARGETS])
{
	auto rb = new ID3D11RenderBuffer;

	// TODO: process textures

	return rb;
}


#endif // D3D11RENDERBUFFER_H
