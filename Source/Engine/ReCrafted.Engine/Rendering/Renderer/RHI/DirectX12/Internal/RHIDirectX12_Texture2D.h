// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#ifndef RHIDIRECTX12_TEXTURE_H
#define RHIDIRECTX12_TEXTURE_H

#include "Rendering/Renderer/Renderer.h"
#include "RHIDirectX12_Resource.h"

#if RENDERER_RHI_D3D12
namespace Renderer
{
    class RHIDirectX12_Texture2D
    {
    private:
        ID3D12Device* m_device = nullptr;
        RHIDirectX12_Resource m_texture = {};
        ID3D12DescriptorHeap* m_heap = nullptr;

    private:
        explicit RHIDirectX12_Texture2D(ID3D12Device* device) : m_device(device) {}

    public:
        void Release();

    public:
        static RHIDirectX12_Texture2D* Create(ID3D12Device* device, uint16_t width, uint16_t height, TextureFormat::_enum texture_format);
    };
}
#endif
#endif // RHIDIRECTX12_TEXTURE_H
