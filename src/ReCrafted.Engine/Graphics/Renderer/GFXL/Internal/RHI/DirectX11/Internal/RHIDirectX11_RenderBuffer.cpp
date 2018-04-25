// ReCrafted (c) 2016-2018 Always Too Late

#include "RHIDirectX11_RenderBuffer.h"
#include "../../../../GFXL.hpp"

namespace GFXL
{
    void RHIDirectX11_RenderBuffer::Clear(ID3D11DeviceContext* context, Color color, unsigned depthMask, int frameIndex)
    {
        cvar rtv = m_renderTargetViews[frameIndex];

        // Clear the render target by using the ClearRenderTargetView command
        context->ClearRenderTargetView(rtv, reinterpret_cast<float*>(&color));
    }

    void RHIDirectX11_RenderBuffer::Bind(ID3D11DeviceContext* context, int frameIndex)
    {
        cvar rtv = m_renderTargetViews[frameIndex];

        context->OMSetRenderTargets(1, &rtv, m_depthStencilView);
    }

    void RHIDirectX11_RenderBuffer::Release()
    {
        SafeRelease(m_depthStencilView);

        for (var i = 0; i < m_rtvCount; i++)
            SafeRelease(m_renderTargetViews[i]);

        m_rtvCount = 0;
        delete this;
    }

    RHIDirectX11_RenderBuffer* RHIDirectX11_RenderBuffer::Create(ID3D11Device* device, int textureCount, ID3D11RenderTargetView* rtvs[16], ID3D11DepthStencilView* dsv)
    {
        cvar renderBuffer = new RHIDirectX11_RenderBuffer(device);

        // Copy resource pointers
        renderBuffer->m_rtvCount = textureCount;
        renderBuffer->m_depthStencilView = dsv;

        for (var i = 0; i < textureCount; i++)
            renderBuffer->m_renderTargetViews[i] = rtvs[i];

        return renderBuffer;
    }
}
