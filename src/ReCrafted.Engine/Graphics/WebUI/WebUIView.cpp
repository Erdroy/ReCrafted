// ReCrafted (c) 2016-2018 Always Too Late

#include "WebUIView.h"

#include <cef_app.h>
#include <cef_client.h>
#include <cef_render_handler.h>
#include <internal/cef_ptr.h>
#include <cef_browser.h>

#undef TEXT

#include "Common/Text.h"
#include "Common/Display.h"
#include "Core/Logger.h"
#include "Core/Lock.h"
#include "Core/Delegate.h"
#include "WebUIEngine.h"

// https://github.com/daktronics/cef-mixer/blob/master/src/html_layer.cpp
class CEFView : public WebUIViewBase, public CefClient, public CefRenderHandler, public CefLifeSpanHandler
{
    friend class WebUIView;

private:
    CefRefPtr<CefBrowser> m_browser;
    Lock m_browserLock = {};

public:
    CefRefPtr<CefRenderHandler> GetRenderHandler() override
    {
        return this;
    }

    CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override 
    {
        return this;
    }

    bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override
    {
        rect = CefRect(0, 0, Display::get_Width(), Display::get_Height());
        return true;
    }

    bool GetRootScreenRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override
    {
        return GetViewRect(browser, rect);
    }

    bool GetScreenPoint(CefRefPtr<CefBrowser> browser, int viewX, int viewY, int& screenX, int& screenY) override
    {
        screenX = viewX;
        screenY = viewY;
        return true;
    }

    void OnAfterCreated(CefRefPtr<CefBrowser> browser) override
    {
        assert(CefCurrentlyOn(TID_UI));

        ScopeLock(m_browserLock);
        m_browser = browser;
    }

    void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height) override
    {
    }

    void OnAcceleratedPaint(
        CefRefPtr<CefBrowser> browser,
        PaintElementType type,
        const RectList& dirtyRects,
        void* share_handle,
        uint64 sync_key) override
    {
        if (type == PET_POPUP)
            return;

    }

public:
    void update() override
    {
        const int64_t time_us = 0u;
        const int64_t deadline_us = 0u;
        const int64_t interval_us = 0u;

        ScopeLock(m_browserLock);
        
        if(m_browser)
        {
            m_browser->GetHost()->SendExternalBeginFrame(time_us, deadline_us, interval_us);
        }
    }

public:
    IMPLEMENT_REFCOUNTING(CEFView);
};

void WebUIView::init()
{
    if (!WebUIEngine::isInitialized())
        return;

    CefBrowserSettings viewSettings = {};
    viewSettings.windowless_frame_rate = 120;
    viewSettings.webgl = STATE_DISABLED;
    viewSettings.plugins = STATE_DISABLED;

    CefWindowInfo window_info = {};
    window_info.SetAsWindowless(nullptr);
    window_info.shared_texture_enabled = true;
	window_info.external_begin_frame_enabled = true;

    m_viewBase = new CEFView();

    CefBrowserHost::CreateBrowser(
        window_info,
        static_cast<CEFView*>(m_viewBase),
        "about:blank",
        viewSettings,
        nullptr);
}

void WebUIView::update()
{
    m_viewBase->update();
}

void WebUIView::resize(uint width, uint height)
{
    cvar view = static_cast<CEFView*>(m_viewBase);
    view->update();
}

void WebUIView::onDestroy()
{
    cvar view = static_cast<CEFView*>(m_viewBase);

    if(view && view->m_browser)
        view->m_browser->GetHost()->CloseBrowser(false);
}

void WebUIView::navigate(Text url)
{
    if (!WebUIEngine::isInitialized())
        return;

    cvar curl = url.std_str();
    cvar view = static_cast<CEFView*>(m_viewBase);

    var frame = view->m_browser->GetMainFrame();
    frame->LoadURL(curl);
}

void WebUIView::execute(const char* javaScriptSource)
{
    cvar view = static_cast<CEFView*>(m_viewBase);

    view->m_browser->GetMainFrame()->ExecuteJavaScript(CefString(javaScriptSource), CefString(""), 0);
}

void WebUIView::bind(const char* bindName, Delegate<void> delegate)
{
}
