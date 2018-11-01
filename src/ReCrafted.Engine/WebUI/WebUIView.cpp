// ReCrafted (c) 2016-2018 Always Too Late

#include "WebUIView.h"
#include "WebUIEngine.h"
#include "Common/Text.h"
#include "Common/Display.h"
#include "Core/Action.h"
#include "Graphics/Graphics.h"
#include "Common/Profiler/Profiler.h"

void WebUIView::Init(uint width, uint height, bool fullscreen)
{
    m_fullscreen = fullscreen;
    m_width = fullscreen ? Display::GetWidth() : width;
    m_height = fullscreen ? Display::GetHeight() : height;

    m_overlay = WebUIEngine::CreateUIView(this, fullscreen);
}

void WebUIView::Resize(uint width, uint height)
{
    ASSERT(m_fullscreen);
    
    m_overlay->Resize(width, height);
}

void WebUIView::Render()
{
    Profiler::BeginProfile(__FUNCTION__);
    if (!m_Active)
        return;

    m_overlay->Draw();

    if (!m_fullscreen)
    {
        // TODO: Blit into back-buffer using created quad
    }
    Profiler::EndProfile();
}

void WebUIView::OnDestroy()
{
    delete m_overlay;
}

void WebUIView::UpdateSurface(const Vector3& vertex0, const Vector3& vertex1, const Vector3& vertex2,
    const Vector3& vertex3)
{
    // TODO: Build quad
}

void WebUIView::Navigate(Text& url)
{
    m_overlay->Navigate(url.StdStr());
}

void WebUIView::Execute(const char* javaScriptSource)
{

}

void WebUIView::Bind(const char* bindName, Action<void> delegate)
{
}
