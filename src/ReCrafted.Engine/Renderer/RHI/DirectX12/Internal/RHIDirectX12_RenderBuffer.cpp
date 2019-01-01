// ReCrafted (c) 2016-2019 Always Too Late

#include "RHIDirectX12_RenderBuffer.h"
#include "../../../Renderer.hpp"

#if RENDERER_RHI_D3D12

namespace Renderer 
{
    void RHIDirectX12_RenderBuffer::Clear(ID3D12GraphicsCommandList* commandList, Color color, unsigned depthMask, int frameIndex)
    {
        if (frameIndex >= 0)
        {
            auto resource = m_renderTargetViews[frameIndex];

            // Transition to render target state
            resource.Transition(commandList, D3D12_RESOURCE_STATE_RENDER_TARGET);

            auto handle = m_heap->GetCPUDescriptorHandleForHeapStart();
            handle.ptr += m_handleIncrementSize * frameIndex;

            // Clear the render target by using the ClearRenderTargetView command
            commandList->ClearRenderTargetView(handle, reinterpret_cast<float*>(&color), 0, nullptr);

            // Transition to present state
            resource.Transition(commandList, D3D12_RESOURCE_STATE_PRESENT);
        }
        else
        {
            // TODO: all
        }
    }

    void RHIDirectX12_RenderBuffer::Bind(ID3D12GraphicsCommandList* commandList, int frameIndex)
    {
        if (frameIndex >= 0)
        {
            auto resource = m_renderTargetViews[frameIndex];

            auto handle = m_heap->GetCPUDescriptorHandleForHeapStart();
            handle.ptr += m_handleIncrementSize * frameIndex;

            // set the render target for the output merger stage (the output of the pipeline)
            commandList->OMSetRenderTargets(1, &handle, FALSE, nullptr);
        }
        else
        {
            // TODO: all
        }
    }

    /*void RHIDirectX12_RenderBuffer::PreparePresent(ID3D12GraphicsCommandList* commandList, int frameIndex)
    {
        if (frameIndex >= 0)
        {
            auto resource = m_renderTargetViews[frameIndex];

            if(resource.GetResource())
                resource.Transition(commandList, D3D12_RESOURCE_STATE_PRESENT);

            if(m_depthStencilView.GetResource())
                m_depthStencilView.Transition(commandList, D3D12_RESOURCE_STATE_PRESENT);
        }
        else
        {
            // TODO: all
        }
    }*/

    void RHIDirectX12_RenderBuffer::Release()
    {
        m_depthStencilView.Release();

        for (auto i = 0; i < m_rtvCount; i++)
            m_renderTargetViews[i].Release();

        SafeRelease(m_heap);

        m_rtvCount = 0;

        delete this;
    }

    RHIDirectX12_RenderBuffer* RHIDirectX12_RenderBuffer::Create(ID3D12Device* device, size_t handleIncrementSize, ID3D12DescriptorHeap* heap, int textureCount, ID3D12Resource* rtvs[16], ID3D12Resource* dsv)
    {
        auto renderBuffer = new RHIDirectX12_RenderBuffer(device);

        // Copy resource pointers
        renderBuffer->m_rtvCount = textureCount;
        renderBuffer->m_depthStencilView = RHIDirectX12_Resource(dsv, D3D12_RESOURCE_STATE_PRESENT);
        renderBuffer->m_handleIncrementSize = handleIncrementSize;
        renderBuffer->m_heap = heap;

        for (auto i = 0; i < textureCount; i++)
            renderBuffer->m_renderTargetViews[i] = RHIDirectX12_Resource(rtvs[i], D3D12_RESOURCE_STATE_PRESENT);

        return renderBuffer;
    }
}
#endif
