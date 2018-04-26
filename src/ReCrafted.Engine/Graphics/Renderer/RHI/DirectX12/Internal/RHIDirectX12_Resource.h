// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef RHIDIRECTX12_RESOURCE_H
#define RHIDIRECTX12_RESOURCE_H

#include "../DirectX12.h"
#include "../../../RendererDefines.h"

#if RENDERER_RENDERER_D3D12
namespace Renderer 
{
    class RHIDirectX12_Resource
    {
    private:
        ID3D12Resource* m_resource = nullptr;
        D3D12_RESOURCE_STATES m_resourceState = {};

    public:
        RHIDirectX12_Resource() { }

        RHIDirectX12_Resource(ID3D12Resource* type, D3D12_RESOURCE_STATES initialState)
        {
            m_resourceState = initialState;
            m_resource = type;
        }

    public:
        ID3D12Resource* GetResource() const
        {
            return m_resource;
        }

        void Release()
        {
            SafeRelease(m_resource);
        }

        void Transition(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES state)
        {
            if (!IsTransitionNeeded(m_resourceState, state))
                return;

            D3D12_RESOURCE_BARRIER barrier;
            barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

            barrier.Transition.pResource = m_resource;
            barrier.Transition.StateBefore = m_resourceState;

            barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
            barrier.Transition.StateAfter = state;

            commandList->ResourceBarrier(1, &barrier);

            m_resourceState = state;
        }

        D3D12_RESOURCE_STATES GetResourceState() const
        {
            return static_cast<D3D12_RESOURCE_STATES>(m_resourceState);
        }

    public:
        static bool IsTransitionNeeded(D3D12_RESOURCE_STATES currentState, D3D12_RESOURCE_STATES targetState)
        {
            // Source: http://flaxengine.com/blog/flax-facts-5-directx12/
            return currentState != targetState && ((currentState | targetState) != currentState || targetState == D3D12_RESOURCE_STATE_COMMON);
        }
    };
}
#endif
#endif // RHIDIRECTX12_RESOURCE_H
