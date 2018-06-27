// ReCrafted (c) 2016-2018 Always Too Late

#include "WebUIEngine.h"
#include "Common/Time.h"
#include "Common/Display.h"
#include "Core/Logger.h"
#include "WebUIView.h"

#pragma warning(push, 0)
#include "Impl/FileSystemWin.h"
#include "Impl/FontLoaderWin.h"
#include "Impl/GPUDriverD3D.h"
#include "Impl/D3DRenderer.h"
#include "Impl/Overlay.h"

#include <Ultralight/Renderer.h>
#include <Ultralight/platform/Platform.h>
#include <Ultralight/platform/Config.h>
#pragma warning(pop)

#include <Shlwapi.h>
#include <thread>

SINGLETON_IMPL(WebUIEngine)

class UltralightRenderer : public Renderable
{
private:
    ultralight::RefPtr<ultralight::Renderer> m_renderer = nullptr;
    ultralight::GPUDriver* m_driver = nullptr;

    bool m_needsViewUpdate = false;

public:
    UltralightRenderer(D3DContext* context)
    {
        // Determine the path to our asset directory.
        TCHAR cur_dir[_MAX_PATH];
        GetCurrentDirectory(_MAX_PATH, cur_dir);
        TCHAR asset_dir[_MAX_PATH];
        PathCombine(asset_dir, cur_dir, L"./../assets/");

        // Setup our Platform API handlers
        rvar platform = ultralight::Platform::instance();

        m_driver = new ultralight::GPUDriverD3D(context);

        // Setup ultralight config
        ultralight::Config config;
        config.face_winding = ultralight::kFaceWinding_Clockwise;
        config.device_scale_hint = 1.0f;
        config.use_distance_field_fonts = true;
        config.use_distance_field_paths = true;

        platform.set_config(config);
        platform.set_gpu_driver(m_driver);
        platform.set_font_loader(new ultralight::FontLoaderWin());
        platform.set_file_system(new ultralight::FileSystemWin(asset_dir));

        m_renderer = ultralight::Renderer::Create();
        
    }

    virtual ~UltralightRenderer()
    {
        // Destroy our Platform handlers
        rvar platform = ultralight::Platform::instance();

        delete static_cast<ultralight::FileSystemWin*>(platform.file_system());
        platform.set_file_system(nullptr);

        delete static_cast<ultralight::FontLoaderWin*>(platform.font_loader());
        platform.set_font_loader(nullptr);

        delete static_cast<ultralight::GPUDriverD3D*>(platform.gpu_driver());
        platform.set_gpu_driver(nullptr);
    }

    void Render(D3DContext* context, float delta) override
    {
        m_renderer->Update();
        m_renderer->Render();

        crvar platform = ultralight::Platform::instance();
        cvar driver = platform.gpu_driver();

        m_needsViewUpdate = driver->HasCommandsPending();

        if(m_needsViewUpdate)
            driver->DrawCommandList();
    }

    void* CreateUIView(WebUIView* view) const
    {
        var overlay = new Overlay(m_renderer.get(), m_driver, view->Width(), view->Height(), 0, 0, 1.0f);
        overlay->view()->set_load_listener(overlay);
        return overlay;
    }

    bool NeedsViewUpdate() const
    {
        return m_needsViewUpdate;
    }

    static UltralightRenderer* Create(D3DContext* context)
    {
        return new UltralightRenderer(context);
    }
};

D3DRenderer* m_context = nullptr;
UltralightRenderer* m_renderer = nullptr;

void WebUIEngine::Init()
{
    // Create D3DRenderer and Ultralight renderer
    m_context = new D3DRenderer(nullptr, false);
    m_renderer = UltralightRenderer::Create(m_context);

    // Initialize context
    m_context->set_scale(1.0);
    m_context->set_screen_size(Display::GetWidth(), Display::GetHeight());
    m_context->AddRenderable(m_renderer);

    m_initialized = true;
    Logger::Log("WebUIEngine initialized using Ultralight {0} (WebKitCore)", ULTRALIGHT_VERSION);
}

void WebUIEngine::OnDispose()
{
    if (!IsInitialized())
        return;

    delete m_renderer;
    delete m_context;
}

void WebUIEngine::Render()
{
    if (!IsInitialized())
        return;

    m_context->Render(float(Time::DeltaTime()));
    m_needsViewUpdate = m_renderer->NeedsViewUpdate();
}

void WebUIEngine::Resize(uint width, uint height)
{
    if (!IsInitialized())
        return;

    m_context->Resize(width, height);
}

void* WebUIEngine::CreateUIView(WebUIView* view)
{
    if (!IsInitialized())
        return nullptr;

    return m_renderer->CreateUIView(view);
}

bool WebUIEngine::IsInitialized()
{
    return m_instance && m_instance->m_initialized;
}
