// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "WebUIManager.h"
#include "Common/Logger.h"
#include "Impl/FileSystemWin.h"
#include "Impl/FontLoaderWin.h"
#include "Scripting/Object.h"
#include "WebUIView.h"
#include "Profiler/Profiler.h"
#include "Impl/d3d11/GPUContextD3D11.h"
#include "MonoJSCore/MonoJSCore.h"

void WebUIManager::Initialize()
{
    // Create GPU context
    m_context = ultralight::GPUContextD3D11::Create();

    // Setup ultralight config
    auto config = ultralight::Config();
    config.user_agent = ultralight::String16("ReCrafted/WebUI/1.0");
    // TODO: Maybe setup custom CSS? So we could get all builtin buttons style etc. 

    // Set 2x super sample for UI
    config.device_scale_hint = 2.0;

    // Setup ultralight platform
    auto& platform = ultralight::Platform::instance();
    platform.set_config(config);
    platform.set_file_system(new ultralight::FileSystemWin(L"../Content/WebUI/"));
    platform.set_font_loader(new ultralight::FontLoaderWin());
    platform.set_gpu_driver(m_context->driver());

    // Create ultralight renderer
    m_renderer = ultralight::Renderer::Create();

    // Initialize MonoJSCore
    MonoJSCore::Initialize();

    Logger::Log("WebUI initialized using Ultralight {0} (WebKitCore)", ULTRALIGHT_VERSION);
}

void WebUIManager::Shutdown()
{
    List<WebUIView*> views;
    views.Copy(m_views);

    for(auto& view : views)
        Object::DestroyNow(view);

    m_views.Clear();
    views.Clear();

    // Delete platform
    delete ultralight::Platform::instance().file_system();
    delete ultralight::Platform::instance().font_loader();

    m_renderer = nullptr;
    m_context.reset();
}

void WebUIManager::OnUpdate()
{
    CPU_PROFILE_FUNCTION(0);

    m_renderer->Update();

    // Update views
    for(auto& view : m_views)
    {
        view->Update();
    }
}

void WebUIManager::Render()
{
    CPU_PROFILE_FUNCTION(0);

    const auto driver = ultralight::Platform::instance().gpu_driver();

    m_context->BeginDrawing();
    {
        driver->BeginSynchronize();
        {
            m_renderer->Render();
        }
        driver->EndSynchronize();

        if (driver->HasCommandsPending())
        {
            driver->DrawCommandList();
        }
    }
    m_context->EndDrawing();

    // Render views
    for (auto& view : m_views)
    {
        view->Render();
    }
}

void WebUIManager::RenderViews()
{
    CPU_PROFILE_FUNCTION(0);

    // Render views
    for (auto& view : m_views)
    {
        view->RenderFullscreen();
    }
}

void WebUIManager::ResizeFullscreenViews(const uint width, const uint height)
{
    // TODO: Resize only fullscreen views

    // Resize views
    for (auto& view : m_views)
    {
        view->Resize(width, height);
    }
}

WebUIView* WebUIManager::CreateView(const int width, const int height)
{
    const auto view = Object::New<WebUIView>();
    view->Initialize(width, height, &GetInstance()->m_renderer->CreateView(width, height, true).get());

    // Add to list
    GetInstance()->m_views.Add(view);

    return view;
}
