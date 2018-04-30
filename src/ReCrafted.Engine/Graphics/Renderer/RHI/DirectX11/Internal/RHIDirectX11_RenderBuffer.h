// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef RHIDIRECTX11_RENDERBUFFER_H
#define RHIDIRECTX11_RENDERBUFFER_H

#include "../DirectX11.h"
#include "../../../Renderer.hpp"
#include "../../../RendererConfig.h"

namespace Renderer
{
    struct Color;

    class RHIDirectX11_RenderBuffer
    {
    private:
        ID3D11Device* m_device = nullptr;

        int m_rtvCount = 0;

        ID3D11RenderTargetView* m_renderTargetViews[RENDERER_MAX_RENDER_BUFFER_TARGETS];
        ID3D11DepthStencilView* m_depthStencilView = nullptr;

    public:
        ID3D11RenderTargetView** GetRTVs() const
        {
            return const_cast<ID3D11RenderTargetView**>(m_renderTargetViews);
        }

        ID3D11DepthStencilView* GetDSV() const
        {
            return m_depthStencilView;
        }

        void SetRTVs(ID3D11RenderTargetView** rtvs)
        {
            for (var i = 0; i < m_rtvCount; i++)
                m_renderTargetViews[i] = rtvs[i];
        }

        void SetDSV(ID3D11DepthStencilView* dsv)
        {
            m_depthStencilView = dsv;
        }

    public:
        RHIDirectX11_RenderBuffer(ID3D11Device* device)
        {
            m_device = device;
        }

        void Clear(ID3D11DeviceContext* context, Color color, unsigned int depthMask = 0x0, int frameIndex = -1);

        void Bind(ID3D11DeviceContext* context, int frameIndex = -1);

        void Release();

    public:
        static RHIDirectX11_RenderBuffer* Create(
            ID3D11Device* device,
            int textureCount,
            ID3D11RenderTargetView* rtvs[RENDERER_MAX_RENDER_BUFFER_TARGETS],
            ID3D11DepthStencilView* dsv
        );
    };
}

#endif // RHIDIRECTX12_RENDERBUFFER_H
