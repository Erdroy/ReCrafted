// ReCrafted (c) 2016-2018 Always Too Late

#include "WebUIView.h"
#include "WebUIEngine.h"
#include "Common/Text.h"
#include "Common/Display.h"
#include "Core/Lock.h"
#include "Core/Delegate.h"
#include "Graphics/Graphics.h"

void WebUIView::init()
{
    if (!WebUIEngine::isInitialized())
        return;

}

void WebUIView::update()
{
    if (!WebUIEngine::isInitialized())
        return;

    //m_viewBase->update();
}

void WebUIView::resize(uint width, uint height)
{
    if (!WebUIEngine::isInitialized())
        return;

    //cvar view = getView();
    //view->update();
}

void WebUIView::render()
{
    if (!WebUIEngine::isInitialized())
        return;

    // swap texture
    //cvar view = getView();
    //view->swap();

    // draw texture
    Graphics::getInstance()->setStage(RenderStage::DrawWebUI);
    // Graphics::getInstance()->blit(0, view->getRenderTexture(), true);
}

void WebUIView::onDestroy()
{
    cvar view = getView();

    //if (view && view->m_browser)
    //    view->m_browser->GetHost()->CloseBrowser(false);
}

CEFView* WebUIView::getView() const
{
    return static_cast<CEFView*>(m_viewBase);
}

void WebUIView::navigate(Text& url)
{
    if (!WebUIEngine::isInitialized())
        return;

    cvar view = getView();
    cvar curl = url.std_str();
    //var frame = view->m_browser->GetMainFrame();
    //frame->LoadURL(curl);
}

void WebUIView::execute(const char* javaScriptSource)
{
    if (!WebUIEngine::isInitialized())
        return;

    //cvar view = getView();
    //view->m_browser->GetMainFrame()->ExecuteJavaScript(CefString(javaScriptSource), CefString(""), 0);
}

void WebUIView::bind(const char* bindName, Delegate<void> delegate)
{
}
