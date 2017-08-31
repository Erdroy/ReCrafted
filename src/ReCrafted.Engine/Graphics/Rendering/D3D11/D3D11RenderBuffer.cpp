// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "D3D11RenderBuffer.h"
#include "D3D11.h"
#include "D3D11Renderer.h"

ID3D11RenderBuffer::ID3D11RenderBuffer(uint textureCount, ID3D11Texture2D* textures[16], ID3D11DepthStencilView* dsv)
{
	auto device = static_cast<ID3D11Device*>(D3D11Renderer::getDevice());

	// create render target views
	for(auto i = 0u; i < textureCount; i ++)
	{
		device->CreateRenderTargetView(textures[i], nullptr, &m_renderTargetViews[i]);
	}

	m_depthStencilView = dsv;
}

void ID3D11RenderBuffer::Bind()
{
	auto deviceContext = static_cast<ID3D11DeviceContext*>(D3D11Renderer::getDeviceContext());
	deviceContext->OMSetRenderTargets(m_srvCount, m_renderTargetViews, m_depthStencilView);
}

void ID3D11RenderBuffer::Clear(float color[4], unsigned int depthMask)
{
	auto deviceContext = static_cast<ID3D11DeviceContext*>(D3D11Renderer::getDeviceContext());
	
	for (auto i = 0u; i < m_srvCount; i++)
	{
		deviceContext->ClearRenderTargetView(m_renderTargetViews[i], color);
	}

	if(m_depthStencilView)
	{
		deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, static_cast<UINT8>(depthMask));
	}
}

ID3D11RenderBuffer* CreateRenderBuffer(uint textureCount, ID3D11Texture2D* textures[16], ID3D11DepthStencilView* dsv)
{
	return new ID3D11RenderBuffer(textureCount, textures, dsv);
}
