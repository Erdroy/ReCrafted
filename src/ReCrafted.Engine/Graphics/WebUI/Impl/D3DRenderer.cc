#include "D3DRenderer.h"
#include <iostream>
#include <algorithm>
#include "Graphics/Renderer/Renderer.hpp"
#include "Graphics/Renderer/RHI/RHIContext.h"
#include <atlcomcli.h>

D3DRenderer::D3DRenderer(HWND hWnd, bool fullscreen, bool sRGB, int samples) {
    Initialize(hWnd, fullscreen, sRGB, samples);
}

D3DRenderer::~D3DRenderer() {
    if (device_)
        immediate_context_->ClearState();
}

void D3DRenderer::Resize(uint16_t width, uint16_t height)
{
    immediate_context_->OMSetRenderTargets(0, nullptr, nullptr);
}

bool D3DRenderer::Initialize(HWND hWnd, bool fullscreen, bool sRGB, int samples) {
    HRESULT hr = S_OK;

    RECT rc;
    ::GetClientRect(hWnd, &rc);
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    Renderer::RHIContext context;
    Renderer::GetContext(&context);

    device_ = CComPtr<ID3D11Device>(static_cast<ID3D11Device*>(context.device));
    immediate_context_ = CComPtr<ID3D11DeviceContext>(static_cast<ID3D11DeviceContext*>(context.deviceContext));

    D3D11_RENDER_TARGET_BLEND_DESC rt_blend_desc;
    ZeroMemory(&rt_blend_desc, sizeof(rt_blend_desc));
    rt_blend_desc.BlendEnable = true;
    rt_blend_desc.SrcBlend = D3D11_BLEND_ONE;
    rt_blend_desc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    rt_blend_desc.BlendOp = D3D11_BLEND_OP_ADD;
    rt_blend_desc.SrcBlendAlpha = D3D11_BLEND_INV_DEST_ALPHA;
    rt_blend_desc.DestBlendAlpha = D3D11_BLEND_ONE;
    rt_blend_desc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
    rt_blend_desc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    D3D11_BLEND_DESC blend_desc;
    ZeroMemory(&blend_desc, sizeof(blend_desc));
    blend_desc.AlphaToCoverageEnable = false;
    blend_desc.IndependentBlendEnable = false;
    blend_desc.RenderTarget[0] = rt_blend_desc;

    device()->CreateBlendState(&blend_desc, blend_state_.GetAddressOf());

    immediate_context_->OMSetBlendState(blend_state_.Get(), NULL, 0xffffffff);

    D3D11_RASTERIZER_DESC rasterizer_desc;
    ZeroMemory(&rasterizer_desc, sizeof(rasterizer_desc));
    rasterizer_desc.FillMode = D3D11_FILL_SOLID;
    rasterizer_desc.CullMode = D3D11_CULL_NONE;
    rasterizer_desc.FrontCounterClockwise = false;
    rasterizer_desc.DepthBias = 0;
    rasterizer_desc.SlopeScaledDepthBias = 0.0f;
    rasterizer_desc.DepthBiasClamp = 0.0f;
    rasterizer_desc.DepthClipEnable = false;
    rasterizer_desc.ScissorEnable = false;
    rasterizer_desc.MultisampleEnable = false;
    rasterizer_desc.AntialiasedLineEnable = false;

    device()->CreateRasterizerState(&rasterizer_desc, rasterizer_state_.GetAddressOf());

    immediate_context_->RSSetState(rasterizer_state_.Get());

    // Setup the viewport
    D3D11_VIEWPORT vp;
    ZeroMemory(&vp, sizeof(vp));
    vp.Width = (float)width;
    vp.Height = (float)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    immediate_context_->RSSetViewports(1, &vp);

    return true;
}

void D3DRenderer::Render(float delta) {

    immediate_context_->OMSetBlendState(blend_state_.Get(), NULL, 0xffffffff);
    immediate_context_->RSSetState(rasterizer_state_.Get());

    std::for_each(renderables_.begin(), renderables_.end(), [this, delta](auto renderable)
    {
        renderable->Render(this, delta);
    });
}

void D3DRenderer::AddRenderable(Renderable* renderable) {
    renderables_.push_back(renderable);
}

void D3DRenderer::RemoveRenderable(Renderable* renderable) {
    renderables_.erase(std::remove(renderables_.begin(), renderables_.end(), renderable));
}

void D3DRenderer::ClearRenderables() {
    renderables_.clear();
}
