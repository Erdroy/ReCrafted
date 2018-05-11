// ReCrafted (c) 2016-2018 Always Too Late

#include "RHIDirectX11_RenderBuffer.h"
#include "../../../Renderer.hpp"

namespace Renderer
{
    void RHIDirectX11_RenderBuffer::Clear(ID3D11DeviceContext* context, Color color, float depth)
    {
        for (var i = 0; i < m_rtvCount; i ++)
        {
            // Clear the render target by using the ClearRenderTargetView command
            cvar rtv = m_renderTargetViews[i];
            context->ClearRenderTargetView(rtv, reinterpret_cast<float*>(&color));
        }

        if (m_depthStencilView)
            context->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, 0);
    }

    void RHIDirectX11_RenderBuffer::Bind(ID3D11DeviceContext* context)
    {
        D3D11_VIEWPORT vpd = {};
        vpd.Width = static_cast<float>(m_width);
        vpd.Height = static_cast<float>(m_height);
        vpd.MinDepth = 0.0f;
        vpd.MaxDepth = 1.0f;

        context->RSSetViewports(1, &vpd);
        context->OMSetRenderTargets(m_rtvCount, m_renderTargetViews, m_depthStencilView);
    }

    void RHIDirectX11_RenderBuffer::Release()
    {
        m_rtvCount = 0;
        delete this;
    }

    RHIDirectX11_RenderBuffer* RHIDirectX11_RenderBuffer::Create(ID3D11Device* device, uint16_t width, uint16_t height,
                                                                 int textureCount, ID3D11RenderTargetView* rtvs[16],
                                                                 ID3D11DepthStencilView* dsv)
    {
        cvar renderBuffer = new RHIDirectX11_RenderBuffer(device);

        // Copy resource pointers
        renderBuffer->m_width = width;
        renderBuffer->m_height = height;
        renderBuffer->m_rtvCount = textureCount;
        renderBuffer->m_depthStencilView = dsv;

        for (var i = 0; i < textureCount; i++)
            renderBuffer->m_renderTargetViews[i] = rtvs[i];

        return renderBuffer;
    }
}
