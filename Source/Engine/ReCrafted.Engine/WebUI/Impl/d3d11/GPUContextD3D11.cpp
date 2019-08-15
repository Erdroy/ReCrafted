
#include "GPUContextD3D11.h"
#include "Rendering/Renderer/Renderer.h"
#include "Rendering/Renderer/RHI/RHIBase.h"
#include "Rendering/Renderer/RHI/RHIContext.h"

namespace ultralight {
    GPUContextD3D11::GPUContextD3D11()
    {
    }

    GPUContextD3D11::~GPUContextD3D11()
    {
        if (device_)
            immediate_context_->ClearState();
    }


    void GPUContextD3D11::BeginDrawing()
    {
        immediate_context_->OMSetBlendState(blend_state_.Get(), nullptr, 0xffffffff);
        immediate_context_->RSSetState(rasterizer_state_.Get());
    }

    void GPUContextD3D11::EndDrawing()
    {
    }

    void GPUContextD3D11::Resize(int width, int height)
    {
        immediate_context_->OMSetRenderTargets(0, nullptr, nullptr);
    }

    // Inherited from D3DContext:
    ID3D11Device* GPUContextD3D11::device()
    {
        assert(device_);
        return device_;
    }

    ID3D11DeviceContext* GPUContextD3D11::immediate_context()
    {
        assert(immediate_context_);
        return immediate_context_;
    }


    void GPUContextD3D11::set_scale(double scale)
    {
        scale_ = scale;
    }

    double GPUContextD3D11::scale() const
    {
        return scale_;
    }

    bool GPUContextD3D11::Initialize(const double screen_scale, const int samples)
    {
        samples_ = samples;
        set_scale(screen_scale);

        ::Renderer::RHIContext context;
        ::Renderer::GetContext(&context);

        device_ = static_cast<ID3D11Device*>(context.device);
        immediate_context_ = static_cast<ID3D11DeviceContext*>(context.deviceContext);

        HRESULT hr = S_OK;

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

        immediate_context_->OMSetBlendState(blend_state_.Get(), nullptr, 0xffffffff);

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

        driver_ = std::make_unique<GPUDriverD3D11>(this);

        return true;
    }

    std::unique_ptr<GPUContext> GPUContextD3D11::Create()
    {
        auto gpu_context = std::make_unique<GPUContextD3D11>();
        if (gpu_context->Initialize(1.0, 1))
            return std::move(gpu_context);

        return nullptr;
    }

}
