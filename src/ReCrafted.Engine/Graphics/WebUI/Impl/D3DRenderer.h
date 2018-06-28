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
private:
    ID3D11RenderTargetView* GetBackBufferView();

public:
    D3DRenderer();
    virtual ~D3DRenderer();

    void Resize(uint16_t width, uint16_t height);

    // Render all renderables.
    void Render(float delta);
    void AfterRender();

    // Add Renderable to be rendered during next Render call
    void AddRenderable(Renderable* renderable);

    // Remove Renderable
    void RemoveRenderable(Renderable* renderable);

    // Clear all Renderables
    void ClearRenderables();

    // Inherited from D3DContext:
    virtual ID3D11Device* device() override
    {
        assert(device_.Get()); 
        return device_.Get();
    }

    virtual ID3D11DeviceContext* immediate_context() override
    {
        assert(immediate_context_.Get()); 
        return immediate_context_.Get();
    }

    ID3D11RenderTargetView* back_buffer_view() override
    {
        return m_backBufferView;
    }

    void GetViewportSize(uint32_t& width, uint32_t& height) override { width = width_; height = height_; }
    void SetViewportSize(int width, int height) override;

public:
    static D3DRenderer* GetInstance()
    {
        static auto instance = new D3DRenderer();
        return instance;
    }

private:
    bool Initialize();

    ID3D11RenderTargetView* m_backBufferView = nullptr;
    ComPtr<ID3D11Device> device_;
    ComPtr<ID3D11DeviceContext> immediate_context_;
    ComPtr<ID3D11BlendState> blend_state_;
    ComPtr<ID3D11RasterizerState> rasterizer_state_;
    uint32_t width_;
    uint32_t height_;
    std::vector<Renderable*> renderables_;
    bool resize_;
};

#endif // D3DRENDERER_H