#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <Ultralight/Ultralight.h>
#include "GPUDriverD3D11.h"
#include "WebUI/Impl/GPUContext.h"
#include "Core/Display.h"

namespace ultralight {

    class GPUContextD3D11 : public GPUContext
    {
    public:
        GPUContextD3D11();

        virtual ~GPUContextD3D11();

        virtual void BeginDrawing();

        virtual void EndDrawing();

        virtual void Resize(int width, int height);

        virtual ID3D11Device* device();

        virtual ID3D11DeviceContext* immediate_context();

        // Scale is calculated from monitor DPI, see Application::SetScale
        virtual void set_scale(double scale);
        virtual double scale() const;

        bool Initialize(double screen_scale, int samples);

        // Inherited from GPUContext
        GPUDriver* GPUContextD3D11::driver() const { return driver_.get(); }
        FaceWinding GPUContextD3D11::face_winding() const { return kFaceWinding_Clockwise; }

    public:
        static std::unique_ptr<GPUContext> Create();

    private:
        ID3D11Device* device_{};
        ID3D11DeviceContext* immediate_context_{};
        Microsoft::WRL::ComPtr<ID3D11BlendState> blend_state_;
        Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_state_;
        double scale_{};
        int samples_ = 1;
        std::unique_ptr<GPUDriverD3D11> driver_;
    };

}  // namespace ultralight