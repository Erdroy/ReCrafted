// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef D3D11RENDERBUFFER_H
#define D3D11RENDERBUFFER_H

#include "../Config.h"
#include "Scripting/Assembly.h"
#include "D3D11.h"

struct ID3D11RenderBuffer
{
private:
	ID3D11RenderTargetView* m_renderTargetViews[RENDERER_MAX_RENDER_BUFFER_TARGETS] = {};
	ID3D11DepthStencilView* m_depthStencilView = nullptr;

	uint m_srvCount = 0u;

public:
	ID3D11RenderBuffer(uint textureCount, ID3D11Texture2D* textures[RENDERER_MAX_RENDER_BUFFER_TARGETS], ID3D11DepthStencilView* dsv);

public:
	void Bind();

	void Clear(float color[4], unsigned int depthMask = 0x0);

public:
	void Release()
	{
		for(auto i = 0u; i < m_srvCount; i ++)
		{
			m_renderTargetViews[i]->Release();
		}

		m_depthStencilView->Release();

		delete this;
	}
};

ID3D11RenderBuffer* CreateRenderBuffer(uint textureCount, ID3D11Texture2D* textures[RENDERER_MAX_RENDER_BUFFER_TARGETS], ID3D11DepthStencilView* dsv);

#endif // D3D11RENDERBUFFER_H
