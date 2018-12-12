// ReCrafted (c) 2016-2018 Always Too Late

#include "WebUIView.h"
#include "WebUIEngine.h"
#include "Common/Text.h"
#include "Common/Display.h"
#include "Core/Action.h"
#include "Graphics/Graphics.h"
#include "Common/Profiler/Profiler.h"

void WebUIView::Init(const uint width, const uint height, const bool fullscreen)
{
    m_fullscreen = fullscreen;
    m_width = fullscreen ? Display::GetWidth() : width;
    m_height = fullscreen ? Display::GetHeight() : height;

    m_viewport = WebUIEngine::CreateUIViewport(this, fullscreen);

    ASSERT(m_viewport);
}

void WebUIView::Resize(const uint width, const uint height)
{
    ASSERT(m_viewport);

    m_viewport->Resize(width, height);
    m_width = width;
    m_height = height;
}

void WebUIView::Update()
{
    ASSERT(m_viewport);

    m_viewport->Update();
}

void WebUIView::Render()
{
    Profiler::BeginProfile(__FUNCTION__);

    if (!m_Active)
        return;

    ASSERT(m_viewport);

    m_viewport->Render();

    if (!m_fullscreen)
    {
        // TODO: Render world-space quad
    }
    else
    {
        // Blit into back-buffer
       // Renderer::BlitTexture(Graphics::GetInstance()->GetFrameBuffer(), m_viewport->GetTexture());
    }

    Profiler::EndProfile();
}

void WebUIView::OnDestroy()
{
    SafeDelete(m_viewport);
}

void WebUIView::UpdateSurface(const Vector3& vertex0, const Vector3& vertex1, const Vector3& vertex2,
    const Vector3& vertex3)
{
    // TODO: Build world space quad
}

void WebUIView::Navigate(const char* url)
{
    ASSERT(m_viewport);
    m_viewport->Navigate(url);
}

void WebUIView::Execute(const char* javaScriptSource)
{
    ASSERT(m_viewport);
    m_viewport->Execute(javaScriptSource);
}

void WebUIView::Bind(const char* bindName, Action<void> delegate)
{
}
