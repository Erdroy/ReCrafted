// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include "../DirectX11.h"
#include "Rendering/Renderer/Renderer.h"
#include "Rendering/Renderer/RendererConfig.h"

namespace Renderer
{
    struct Color;

    class RHIDirectX11_RenderBuffer
    {
    private:
        ID3D11Device* m_device = nullptr;

        uint16_t m_width = 0u;
        uint16_t m_height = 0u;

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
            for (auto i = 0; i < m_rtvCount; i++)
                m_renderTargetViews[i] = rtvs[i];
        }

        void SetDSV(ID3D11DepthStencilView* dsv)
        {
            m_depthStencilView = dsv;
        }

        void SetSize(uint16_t width, uint16_t height)
        {
            m_width = width;
            m_height = height;
        }

    public:
        RHIDirectX11_RenderBuffer(ID3D11Device* device)
        {
            m_device = device;
        }

        void Clear(ID3D11DeviceContext* context, Color color, float depth);

        void Bind(ID3D11DeviceContext* context);

        void Release();

    public:
        static RHIDirectX11_RenderBuffer* Create(
            ID3D11Device* device,
            uint16_t width,
            uint16_t height,
            int textureCount,
            ID3D11RenderTargetView* rtvs[RENDERER_MAX_RENDER_BUFFER_TARGETS],
            ID3D11DepthStencilView* dsv
        );
    };
}
