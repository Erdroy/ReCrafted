#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <stdint.h>
#include "GPUDriverD3D11.h"
#include <Ultralight/Ultralight.h>
#include "PlatformD3D11.h"
#include "WebUI/Impl/GPUContext.h"
#include "Core/Display.h"

namespace ultralight {

    class GPUContextD3D11 : public GPUContext, public PlatformGPUContextD3D11
    {
    public:
        GPUContextD3D11();

        virtual ~GPUContextD3D11();

        virtual void BeginDrawing();

        virtual void EndDrawing();

        virtual void PresentFrame();

        virtual void Resize(int width, int height);

        virtual ID3D11Device* device();

        virtual ID3D11DeviceContext* immediate_context();

        virtual IDXGISwapChain* swap_chain();

        virtual ID3D11RenderTargetView* render_target_view();

        // Scale is calculated from monitor DPI, see Application::SetScale
        virtual void set_scale(double scale);
        virtual double scale() const;

        // This is in virtual units, not actual pixels.
        virtual void set_screen_size(uint32_t width, uint32_t height);
        virtual void screen_size(uint32_t& width, uint32_t& height);

        bool Initialize(HWND hWnd, int screen_width, int screen_height, double screen_scale, bool fullscreen, bool enable_vsync, bool sRGB, int samples);

        UINT back_buffer_width();
        UINT back_buffer_height();

        // Inherited from GPUContext
        GPUDriver* GPUContextD3D11::driver() const { return driver_.get(); }
        FaceWinding GPUContextD3D11::face_winding() const { return kFaceWinding_Clockwise; }

        void SetViewportSize(int width, int height);

    public:
        static std::unique_ptr<GPUContext> Create(bool enable_vsync);

    private:
        HWND hwnd_;
        ID3D11Device* device_;
        ID3D11DeviceContext* immediate_context_;
        IDXGISwapChain* swap_chain_;
        Microsoft::WRL::ComPtr<ID3D11BlendState> blend_state_;
        Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_state_;
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

}  // namespace ultralight