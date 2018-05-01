// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef DIRECTX11IMPL_H
#define DIRECTX11IMPL_H

// includes
#include "ReCrafted.h"
#include <d3d11.h>
#include <direct.h>
#include <dxgi.h>

#define SafeRelease(res) if(res) { res->Release(); res = nullptr; }

class RendererImpl
{
public:
    static uint getWidth(ID3D11Texture2D* texture)
    {
        D3D11_TEXTURE2D_DESC desc;
        texture->GetDesc(&desc);
        return desc.Width;
    }

    static uint getHeight(ID3D11Texture2D* texture)
    {
        D3D11_TEXTURE2D_DESC desc;
        texture->GetDesc(&desc);
        return desc.Height;
    }

    static uint getFormat(ID3D11Texture2D* texture)
    {
        D3D11_TEXTURE2D_DESC desc;
        texture->GetDesc(&desc);
        return desc.Format;
    }

    static ID3D11Texture2D* openSharedTexture(void* sharedHandle)
    {
        /*cvar device = static_cast<ID3D11Device*>(bgfx::getInternalData()->context);

        ID3D11Texture2D* texture = nullptr;
        auto hr = device->OpenSharedResource(sharedHandle, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&texture));

        if (FAILED(hr))
            return nullptr;

        return texture;*/
        return nullptr;
    }

    static ID3D11Texture2D* createTexture(uint width, uint height, uint format)
    {
        /*cvar device = static_cast<ID3D11Device*>(bgfx::getInternalData()->context);

        D3D11_TEXTURE2D_DESC textureDesc;
        textureDesc.ArraySize = 1;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.Format = static_cast<DXGI_FORMAT>(format);
        textureDesc.Width = width;
        textureDesc.Height = height;
        textureDesc.MipLevels = 1;
        textureDesc.MiscFlags = 0;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;

        ID3D11Texture2D* texture = nullptr;
        var hr = device->CreateTexture2D(&textureDesc, nullptr, &texture);
        
        if (FAILED(hr))
            return nullptr;

        return texture;*/
        return nullptr;
    }
};

#endif // DIRECTX11IMPL_H
