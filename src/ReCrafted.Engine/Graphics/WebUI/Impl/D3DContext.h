#pragma once

#ifndef D3DCONTEXT_H
#define D3DCONTEXT_H

// includes
#include <d3d11.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

/**
* Direct3D 11 Context interface.
*/
class D3DContext {
public:
    virtual ~D3DContext() {}

    virtual ID3D11Device* device() = 0;
    virtual ID3D11DeviceContext* immediate_context() = 0;
    virtual ID3D11RenderTargetView* back_buffer_view() = 0;

    virtual void GetViewportSize(uint32_t& width, uint32_t& height) = 0;
    virtual void SetViewportSize(int width, int height) = 0;
};


#endif // D3DCONTEXT_H
