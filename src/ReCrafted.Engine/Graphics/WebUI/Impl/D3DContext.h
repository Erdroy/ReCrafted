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
    virtual IDXGISwapChain* swap_chain() = 0;
    virtual ID3D11RenderTargetView* render_target_view() = 0;

    // Scale is calculated from monitor DPI, see Application::SetScale
    virtual void set_scale(double scale) = 0;
    virtual double scale() const = 0;

    // This is in virtual units, not actual pixels.
    virtual void set_screen_size(uint32_t width, uint32_t height) = 0;
    virtual void screen_size(uint32_t& width, uint32_t& height) = 0;
};


#endif // D3DCONTEXT_H
