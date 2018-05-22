// ReCrafted (c) 2016-2018 Always Too Late

#include "WebUIEngine.h"
#include "Core/Logger.h"

#pragma warning(push, 0)
#include "Utils/FileSystemWin.h"
#include "Utils/FontLoaderWin.h"

#include <Ultralight/Renderer.h>
#include <Ultralight/platform/Platform.h>
#include <Ultralight/platform/Config.h>
#pragma warning(pop)

#include <Shlwapi.h>
#include <thread>

SINGLETON_IMPL(WebUIEngine)

ultralight::RefPtr<ultralight::Renderer> m_renderer = nullptr;

void WebUIEngine::init()
{
    // Determine the path to our asset directory.
    TCHAR cur_dir[_MAX_PATH];
    GetCurrentDirectory(_MAX_PATH, cur_dir);
    TCHAR asset_dir[_MAX_PATH];
    PathCombine(asset_dir, cur_dir, L"./../assets/");

    // Setup our Platform API handlers
    rvar platform = ultralight::Platform::instance();

    // Setup ultralight config
    ultralight::Config config;
    config.face_winding = ultralight::kFaceWinding_Clockwise;
    config.device_scale_hint = 1.0f;
    config.use_distance_field_fonts = false; // TODO: detect high dpi and set to true
    config.use_distance_field_paths = true;

    platform.set_config(config);
    //platform.set_gpu_driver(new ultralight::GPUDriverD3D(context));
    platform.set_font_loader(new ultralight::FontLoaderWin());
    platform.set_file_system(new ultralight::FileSystemWin(asset_dir));
    
    m_renderer = ultralight::Renderer::Create();

    m_initialized = true;
    Logger::log("WebUIEngine initialized using Ultralight {0} (WebKitCore)", ULTRALIGHT_VERSION);
}

void WebUIEngine::onDispose()
{
    if (!isInitialized())
        return;

    // Destroy our Platform handlers
    rvar platform = ultralight::Platform::instance();

    delete static_cast<ultralight::FileSystemWin*>(platform.file_system());
    platform.set_file_system(nullptr);

    delete static_cast<ultralight::FontLoaderWin*>(platform.font_loader());
    platform.set_font_loader(nullptr);

    //delete static_cast<GPUDriverD3D*>(platform.gpu_driver());
    //platform.set_gpu_driver(nullptr);
}

void WebUIEngine::Update()
{
    if (!isInitialized())
        return;

    m_renderer->Update();
}

void WebUIEngine::Render()
{
    if (!isInitialized())
        return;

    m_renderer->Render();

    rvar platform = ultralight::Platform::instance();
    /*var driver = static_cast<GPUDriverD3D*>(platform.gpu_driver());

    if (driver->HasCommandsPending())
    {
        driver->DrawCommandList();
        // ui_->Draw(); ?
    }*/
}

bool WebUIEngine::isInitialized()
{
    if (!m_instance)
        return false;

    return m_instance->m_initialized;
}
