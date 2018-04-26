// ReCrafted (c) 2016-2018 Always Too Late

#include "RHIDirectX12_Texture2D.h"

#if RENDERER_RENDERER_D3D12
namespace Renderer
{
    void RHIDirectX12_Texture2D::Release()
    {
        m_texture.Release();
        SafeRelease(m_heap);
        delete this;
    }

    RHIDirectX12_Texture2D* RHIDirectX12_Texture2D::Create(ID3D12Device* device, uint16_t width, uint16_t height, TextureFormat::_enum texture_format)
    {
        auto texture = new RHIDirectX12_Texture2D(device);

        // TODO: implementation

        return texture;
    }
}
#endif