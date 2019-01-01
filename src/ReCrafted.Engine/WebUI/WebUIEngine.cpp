// ReCrafted (c) 2016-2019 Always Too Late

#include "WebUIEngine.h"
#include "Common/Time.h"
#include "Core/Logger.h"

#include "WebUI/WebUIView.h"

#include "WebUI/Impl/UltralightViewport.h"
#include "WebUI/Impl/FileSystemWin.h"
#include "WebUI/Impl/FontLoaderWin.h"
#include "WebUI/Impl/GPUContext.h"

#include <Ultralight/Ultralight.h>

SINGLETON_IMPL(WebUIEngine)

ultralight::RefPtr<ultralight::Renderer> g_ultralightRenderer = nullptr;
std::unique_ptr<ultralight::GPUContext> m_webui_context = nullptr;

void WebUIEngine::Init()
{
    m_initialized = true;

    m_webui_context = ultralight::GPUContext::Create(false);

    // Setup ultralight config
    auto config = ultralight::Config();
    config.user_agent = ultralight::String16("ReCrafted/WebUI/1.0");

    // Setup ultralight platform
    auto& platform = ultralight::Platform::instance();
    platform.set_config(config);
    platform.set_file_system(new ultralight::FileSystemWin(L"./../content/UI/"));
    platform.set_font_loader(new ultralight::FontLoaderWin());
    platform.set_gpu_driver(m_webui_context->driver());

    // Create ultralight renderer
    g_ultralightRenderer = ultralight::Renderer::Create();

    Logger::Log("WebUIEngine initialized using Ultralight {0} (WebKitCore)", ULTRALIGHT_VERSION);
}

void WebUIEngine::OnDispose()
{
    if (!IsInitialized())
        return;

    // Delete platform
    delete ultralight::Platform::instance().file_system();
    delete ultralight::Platform::instance().font_loader();

    // Release ultralight renderer
    g_ultralightRenderer->Release();
    m_webui_context.reset();
}

void WebUIEngine::Update()
{
    g_ultralightRenderer->Update();
}

void WebUIEngine::Render()
{
    if (!IsInitialized())
        return;

    cvar driver = ultralight::Platform::instance().gpu_driver();

    m_webui_context->BeginDrawing();
    {
        driver->BeginSynchronize();
        {
            g_ultralightRenderer->Render();
        }
        driver->EndSynchronize();

        if (driver->HasCommandsPending())
        {
            driver->DrawCommandList();
        }
    }
    m_webui_context->EndDrawing();
}

WebUIViewport* WebUIEngine::CreateUIViewport(WebUIView* view, const bool fullscreen)
{
    ASSERT(IsInitialized());

    cvar width = view->Width();
    cvar height = view->Height();

    cvar ultralightView = g_ultralightRenderer->CreateView(width, height, true);
    cvar ultralightViewport = new UltralightViewport(width, height, ultralightView);
    return static_cast<WebUIViewport*>(ultralightViewport);
}

bool WebUIEngine::IsInitialized()
{
    return m_instance && m_instance->m_initialized;
}
