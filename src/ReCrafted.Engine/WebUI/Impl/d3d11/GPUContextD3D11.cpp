#include "WebUI/Impl/GPUContext.h"
#include "GPUDriverD3D11.h"
#include "Platform/Platform.h"
#include "Common/Display.h"
#include "Renderer/Renderer.hpp"
#include "Renderer/RHI/RHIContext.h"

#include <memory>
#include <atlcomcli.h>

namespace ultralight
{
    class GPUContextD3D11 : public GPUContext, public PlatformGPUContextD3D11
    {
    public:
        GPUContextD3D11()
        {
        }

        virtual ~GPUContextD3D11()
        {
            if (device_)
                immediate_context_->ClearState();
        }

        // Inherited from GPUContext
        GPUDriver* driver() const override { return driver_.get(); }
        FaceWinding face_winding() const override { return kFaceWinding_Clockwise; }

        void BeginDrawing() override
        {
            immediate_context_->OMSetBlendState(blend_state_.Get(), nullptr, 0xffffffff);
            immediate_context_->RSSetState(rasterizer_state_.Get());
            SetViewportSize(width_, height_);
        }

        void EndDrawing() override
        {
        }

        void PresentFrame() override
        {
            swap_chain()->Present(enable_vsync_ ? 1 : 0, 0);
        }

        void Resize(int width, int height) override
        {
            set_screen_size(width, height);
            immediate_context_->OMSetRenderTargets(0, nullptr, nullptr);
        }

        // Inherited from D3DContext:
        ID3D11Device* device() override
        {
            assert(device_.Get());
            return device_.Get();
        }

        ID3D11DeviceContext* immediate_context() override
        {
            assert(immediate_context_.Get());
            return immediate_context_.Get();
        }

        IDXGISwapChain* swap_chain() override
        {
            assert(swap_chain_.Get());
            return swap_chain_.Get();
        }

        ID3D11RenderTargetView* render_target_view() override
        {
            Renderer::RHIContext context;
            GetContext(&context);
            return static_cast<ID3D11RenderTargetView*>(context.windows[1].backBuffer);
        }

        void set_scale(double scale) override { scale_ = scale; }
        double scale() const override { return scale_; }

        void set_screen_size(uint32_t width, uint32_t height) override
        {
            width_ = width;
            height_ = height;
        }

        void screen_size(uint32_t& width, uint32_t& height) override
        {
            width = width_;
            height = height_;
        }

        bool Initialize(HWND hWnd, int screen_width, int screen_height, double screen_scale, bool fullscreen,
                        bool enable_vsync, bool sRGB, int samples)
        {
            samples_ = samples;
            enable_vsync_ = enable_vsync;
            set_screen_size(screen_width, screen_height);
            set_scale(screen_scale);


            Renderer::RHIContext context;
            GetContext(&context);

            device_ = CComPtr<ID3D11Device>(static_cast<ID3D11Device*>(context.device));
            immediate_context_ = CComPtr<ID3D11DeviceContext>(static_cast<ID3D11DeviceContext*>(context.deviceContext));

            HRESULT hr = S_OK;

            hwnd_ = hWnd;

            // Get the actual device width/height (may be different than screen size)
            RECT rc;
            GetClientRect(hWnd, &rc);
            UINT width = rc.right - rc.left;
            UINT height = rc.bottom - rc.top;

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

            back_buffer_width_ = width;
            back_buffer_height_ = height;

            // Initialize backbuffer with white so we don't get flash of black while loading views.
            float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
            immediate_context_->ClearRenderTargetView(render_target_view(), color);

            driver_ = std::make_unique<GPUDriverD3D11>(this);

            return true;
        }

        void SetViewportSize(int width, int height)
        {
            width_ = width;
            height_ = height;

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
        }

        UINT back_buffer_width() { return back_buffer_width_; }
        UINT back_buffer_height() { return back_buffer_height_; }

    private:
        HWND hwnd_;
        ComPtr<ID3D11Device> device_;
        ComPtr<ID3D11DeviceContext> immediate_context_;
        ComPtr<IDXGISwapChain> swap_chain_;
        ComPtr<ID3D11BlendState> blend_state_;
        ComPtr<ID3D11RasterizerState> rasterizer_state_;
        D3D_FEATURE_LEVEL feature_level_;
        double scale_;
        bool enable_vsync_;
        uint32_t width_;
        uint32_t height_;
        UINT back_buffer_width_;
        UINT back_buffer_height_;
        int samples_ = 1;
        std::unique_ptr<GPUDriverD3D11> driver_;
    };

    std::unique_ptr<GPUContext> GPUContext::Create(bool enable_vsync)
    {
        auto gpu_context = std::make_unique<GPUContextD3D11>();
        if (gpu_context->Initialize(static_cast<HWND>(Platform::GetCurrentWindow()), Display::GetWidth(),
                                    Display::GetHeight(), 1.0f, false, enable_vsync, false, 1))
            return std::move(gpu_context);

        return nullptr;
    }
} // namespace framework
