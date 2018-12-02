// ReCrafted (c) 2016-2018 Always Too Late

#include "WebUIEngine.h"
#include "Common/Time.h"
#include "Core/Logger.h"
#include "WebUI/WebUIView.h"
#include "WebUI/Impl/UltralightViewport.h"

#include <Ultralight/Ultralight.h>

SINGLETON_IMPL(WebUIEngine)

ultralight::RefPtr<ultralight::Renderer> g_ultralightRenderer = nullptr;

void WebUIEngine::Init()
{
    m_initialized = true;

    // Setup ultralight platform
    auto& platform = ultralight::Platform::instance();
    platform.set_config(ultralight::Config());
    // TODO: Setup FileSystem and FontLoader

    // Create ultralight renderer
    g_ultralightRenderer = ultralight::Renderer::Create();

    Logger::Log("WebUIEngine initialized using Ultralight {0} (WebKitCore)", ULTRALIGHT_VERSION);
}

void WebUIEngine::OnDispose()
{
    if (!IsInitialized())
        return;

    // Release ultralight renderer
    g_ultralightRenderer->Release();
}

void WebUIEngine::Update()
{
    g_ultralightRenderer->Update();
}

void WebUIEngine::Render()
{
    if (!IsInitialized())
        return;

    g_ultralightRenderer->Render();
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
