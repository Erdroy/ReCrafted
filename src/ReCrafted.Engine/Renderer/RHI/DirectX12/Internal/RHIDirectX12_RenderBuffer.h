// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef RHIDIRECTX12_RENDERBUFFER_H
#define RHIDIRECTX12_RENDERBUFFER_H

#include "RHIDirectX12_Resource.h"
#include "../../../RendererConfig.h"

#if RENDERER_RHI_D3D12
namespace Renderer 
{
    struct Color;

    class RHIDirectX12_RenderBuffer
    {
    private:
        ID3D12Device* m_device = nullptr;

        int m_rtvCount = 0;
        size_t m_handleIncrementSize = 0u;

        RHIDirectX12_Resource m_renderTargetViews[RENDERER_MAX_RENDER_BUFFER_TARGETS];
        RHIDirectX12_Resource m_depthStencilView;
        ID3D12DescriptorHeap* m_heap = nullptr;

    public:
        RHIDirectX12_RenderBuffer(ID3D12Device* device)
        {
            m_device = device;
        }

        void Clear(ID3D12GraphicsCommandList* commandList, Color color, unsigned int depthMask = 0x0, int frameIndex = -1);

        void Bind(ID3D12GraphicsCommandList* commandList, int frameIndex = -1);

        void Release();

    public:
        static RHIDirectX12_RenderBuffer* Create(
            ID3D12Device* device, 
            size_t handleIncrementSize, 
            ID3D12DescriptorHeap* heap, 
            int textureCount, 
            ID3D12Resource* rtvs[RENDERER_MAX_RENDER_BUFFER_TARGETS], 
            ID3D12Resource* dsv
        );
    };
}
#endif
#endif // RHIDIRECTX12_RENDERBUFFER_H
