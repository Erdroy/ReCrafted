#pragma once

#ifndef D3DRENDERER_H
#define D3DRENDERER_H

#include <Ultralight/platform/GPUDriver.h>

#include "D3DContext.h"

#include <Windows.h>
#include <vector>
#include <cassert>

/**
* Renderable interface, any renderable classes should implement this
* and call D3DRenderer::AddRenderable to get a slice of time to render.
*/
class Renderable {
public:
    virtual ~Renderable() {}
    virtual void Render(D3DContext* context, float delta) = 0;
};

/**
* Direct3D Renderer context, implements the D3DContext interface and
* renders any active Renderables.
*/
class D3DRenderer : public D3DContext {
public:
    D3DRenderer(HWND hWnd, bool fullscreen, bool sRGB = true, int samples = 1);
    virtual ~D3DRenderer();

    void Resize(uint16_t width, uint16_t height);

    // Render all renderables.
    void Render(float delta);

    // Add Renderable to be rendered during next Render call
    void AddRenderable(Renderable* renderable);

    // Remove Renderable
    void RemoveRenderable(Renderable* renderable);

    // Clear all Renderables
    void ClearRenderables();

    // Inherited from D3DContext:
    virtual ID3D11Device* device() override { assert(device_.Get()); return device_.Get(); }
    virtual ID3D11DeviceContext* immediate_context() override { assert(immediate_context_.Get()); return immediate_context_.Get(); }
    virtual IDXGISwapChain* swap_chain() override { assert(swap_chain_.Get()); return swap_chain_.Get(); }
    virtual ID3D11RenderTargetView* render_target_view() override { assert(back_buffer_view_.Get()); return back_buffer_view_.Get(); }
    virtual void set_scale(double scale) { scale_ = scale; }
    virtual double scale() const { return scale_; }
    virtual void set_screen_size(uint32_t width, uint32_t height) override { width_ = width; height_ = height; }
    virtual void screen_size(uint32_t& width, uint32_t& height) override { width = width_; height = height_; }

private:
    bool Initialize(HWND hWnd, bool fullscreen, bool sRGB, int samples);
    void OnResize();

    HWND hwnd_;
    ComPtr<ID3D11Device> device_;
    ComPtr<ID3D11DeviceContext> immediate_context_;
    ComPtr<IDXGISwapChain> swap_chain_;
    ComPtr<ID3D11RenderTargetView> back_buffer_view_;
    ComPtr<ID3D11BlendState> blend_state_;
    ComPtr<ID3D11RasterizerState> rasterizer_state_;
    double scale_;
    uint32_t width_;
    uint32_t height_;
    UINT back_buffer_width_;
    UINT back_buffer_height_;
    int samples_ = 1;
    std::vector<Renderable*> renderables_;
};

#endif // D3DRENDERER_H