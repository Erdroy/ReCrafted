// ReCrafted (c) 2016-2018 Always Too Late

#include "WebUIEngine.h"
#include "Common/Time.h"
#include "Core/Logger.h"

#include "WebUIGPUDriver.h"
#include "WebUIRenderer.h"

#include "Impl/FontLoaderRoboto.h"
#include "Impl/Overlay.h"

#include <Ultralight/Ultralight.h>

SINGLETON_IMPL(WebUIEngine)

ultralight::RefPtr<ultralight::Renderer> m_ultralightRenderer = nullptr;

void WebUIEngine::Init()
{
    m_initialized = true;

    m_gpuDriver = new WebUIGPUDriver();

    // Setup ultralight config
    ultralight::Config config;
    config.face_winding = ultralight::kFaceWinding_Clockwise;
    config.device_scale_hint = 1.0f;
    config.use_distance_field_fonts = true;
    config.use_distance_field_paths = true;

    auto& platform = ultralight::Platform::instance();
    platform.set_config(ultralight::Config());
    platform.set_gpu_driver(m_gpuDriver);
    platform.set_font_loader(new ultralight::FontLoaderRoboto()); // TODO: Font loader
    // TODO: File system

    // Create the Renderer
    m_ultralightRenderer = ultralight::Renderer::Create();

    m_renderer = new WebUIRenderer(m_ultralightRenderer.get(), m_gpuDriver);

    Logger::Log("WebUIEngine initialized using Ultralight {0} (WebKitCore)", ULTRALIGHT_VERSION);
}

void WebUIEngine::OnDispose()
{
    if (!IsInitialized())
        return;

    rvar platform = ultralight::Platform::instance();

    //delete static_cast<ultralight::FileSystemWin*>(platform.file_system());
    //platform.set_file_system(nullptr);

    delete static_cast<ultralight::FontLoaderRoboto*>(platform.font_loader());
    platform.set_font_loader(nullptr);

    delete static_cast<WebUIGPUDriver*>(platform.gpu_driver());
    platform.set_gpu_driver(nullptr);

    m_ultralightRenderer->Release();

    delete m_renderer;
}

void WebUIEngine::Update()
{
    m_renderer->Update();
}

void WebUIEngine::Render()
{
    if (!IsInitialized())
        return;

    m_renderer->Render();
}

void WebUIEngine::Resize(uint width, uint height)
{
    if (!IsInitialized())
        return;

    m_renderer->Resize(width, height);
}

bool WebUIEngine::NeedsViewsUpdate() const
{
    return m_renderer->NeedsViewsUpdate();
}

WebUIOverlay* WebUIEngine::CreateUIView(WebUIView* view, const bool fullscreen)
{
    if (!IsInitialized())
        return nullptr;

    cvar overlay = new Overlay(fullscreen, m_ultralightRenderer.get(), m_instance->m_gpuDriver, view->Width(), view->Height());
    overlay->view()->set_load_listener(overlay);
    overlay->view()->set_view_listener(overlay);

    return static_cast<WebUIOverlay*>(overlay);
}

bool WebUIEngine::IsInitialized()
{
    return m_instance && m_instance->m_initialized;
}
