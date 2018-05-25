// ReCrafted (c) 2016-2018 Always Too Late

#include "WebUIView.h"
#include "WebUIEngine.h"
#include "Common/Text.h"
#include "Common/Display.h"
#include "Core/Lock.h"
#include "Core/Delegate.h"
#include "Graphics/Graphics.h"

void WebUIView::Init()
{
    if (!WebUIEngine::IsInitialized())
        return;

}

void WebUIView::Update()
{
    if (!WebUIEngine::IsInitialized())
        return;

    //m_viewBase->Update();
}

void WebUIView::Resize(uint width, uint height)
{
    if (!WebUIEngine::IsInitialized())
        return;

    //cvar view = getView();
    //view->Update();
}

void WebUIView::Render()
{
    if (!WebUIEngine::IsInitialized())
        return;

    // swap texture
    //cvar view = getView();
    //view->swap();

    // draw texture
    Graphics::GetInstance()->SetStage(RenderStage::DrawWebUI);
    // Graphics::GetInstance()->blit(0, view->getRenderTexture(), true);
}

void WebUIView::OnDestroy()
{
    cvar view = GetView();

    //if (view && view->m_browser)
    //    view->m_browser->GetHost()->CloseBrowser(false);
}

CEFView* WebUIView::GetView() const
{
    return static_cast<CEFView*>(m_viewBase);
}

void WebUIView::Navigate(Text& url)
{
    if (!WebUIEngine::IsInitialized())
        return;

    cvar view = GetView();
    cvar curl = url.StdStr();
    //var frame = view->m_browser->GetMainFrame();
    //frame->LoadURL(curl);
}

void WebUIView::Execute(const char* javaScriptSource)
{
    if (!WebUIEngine::IsInitialized())
        return;

    //cvar view = getView();
    //view->m_browser->GetMainFrame()->ExecuteJavaScript(CefString(javaScriptSource), CefString(""), 0);
}

void WebUIView::Bind(const char* bindName, Delegate<void> delegate)
{
}
