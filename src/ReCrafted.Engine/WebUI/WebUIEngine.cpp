// ReCrafted (c) 2016-2018 Always Too Late

#include "WebUIEngine.h"
#include "Common/Time.h"
#include "Core/Logger.h"
#include "WebUIView.h"

SINGLETON_IMPL(WebUIEngine)

void WebUIEngine::Init()
{
    m_initialized = true;
    Logger::Log("WebUIEngine initialized using Ultralight {0} (WebKitCore)", 0/*ULTRALIGHT_VERSION*/);
}

void WebUIEngine::OnDispose()
{
    if (!IsInitialized())
        return;

    //delete m_renderer;
}

void WebUIEngine::Render()
{
    if (!IsInitialized())
        return;

    //D3DRenderer::GetInstance()->Render(float(Time::DeltaTime()));
    //m_needsViewUpdate = m_renderer->NeedsViewUpdate();
}

void WebUIEngine::OnRendered()
{
    //D3DRenderer::GetInstance()->AfterRender();
}

void WebUIEngine::Resize(uint width, uint height)
{
    if (!IsInitialized())
        return;

    //D3DRenderer::GetInstance()->Resize(width, height);
}

WebUIOverlay* WebUIEngine::CreateUIView(WebUIView* view, bool fullscreen)
{
    //if (!IsInitialized())
        return nullptr;

    //return m_renderer->CreateUIView(view, fullscreen);
}

bool WebUIEngine::IsInitialized()
{
    return m_instance && m_instance->m_initialized;
}
