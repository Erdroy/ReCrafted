// ReCrafted (c) 2016-2018 Always Too Late

#include "WebUIView.h"
#include "WebUIEngine.h"
#include "Common/Text.h"
#include "Common/Display.h"
#include "Core/Delegate.h"
#include "Graphics/Graphics.h"
#include "Impl/Overlay.h"

void WebUIView::Init()
{
    m_width = Display::GetWidth();
    m_height = Display::GetHeight();

    m_overlay = WebUIEngine::CreateUIView(this);
}

void WebUIView::Update()
{

}

void WebUIView::Resize(uint width, uint height)
{

}

void WebUIView::Render()
{
    cvar overlay = static_cast<Overlay*>(m_overlay);
    overlay->Draw();
}

void WebUIView::OnDestroy()
{
    cvar overlay = static_cast<Overlay*>(m_overlay);
    delete overlay;
}

void WebUIView::Navigate(Text& url)
{
    cvar overlay = static_cast<Overlay*>(m_overlay);

    var view = overlay->view();
    view->LoadURL(url.StdStr().c_str());
}

void WebUIView::Execute(const char* javaScriptSource)
{

}

void WebUIView::Bind(const char* bindName, Delegate<void> delegate)
{
}
