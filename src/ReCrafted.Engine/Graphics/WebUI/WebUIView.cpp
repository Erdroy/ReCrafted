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
#include "Core/Delegate.h"
#include "WebUIEngine.h"

class HtmlView : public CefClient, public CefRenderHandler, public CefLifeSpanHandler
{
    friend class WebUIView;

private:
    CefRefPtr<CefBrowser> m_browser;

public:
    CefRefPtr<CefRenderHandler> GetRenderHandler() override
    {
        return this;
    }

    CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override {
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
        // https://github.com/daktronics/cef-mixer/blob/master/src/html_layer.cpp
        // https://github.com/Erdroy/ReCrafted/blob/WebUI/src/ReCrafted.Engine/Graphics/WebUI/WebUI.cpp
    }

    void OnAfterCreated(CefRefPtr<CefBrowser> browser) override
    {
        assert(CefCurrentlyOn(TID_UI));

        m_browser = browser;
    }

    bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        const CefString& target_url,
        const CefString& target_frame_name,
        WindowOpenDisposition target_disposition,
        bool user_gesture,
        const CefPopupFeatures& popupFeatures,
        CefWindowInfo& windowInfo,
        CefRefPtr<CefClient>& client,
        CefBrowserSettings& settings,
        bool* no_javascript_access) override
    {
        return false;
    }

public:
    IMPLEMENT_REFCOUNTING(HtmlView);
};

void WebUIView::init()
{
    if (!WebUIEngine::isInitialized())
        return;

    /*CefBrowserSettings viewSettings = {};
    viewSettings.windowless_frame_rate = 120;
    viewSettings.webgl = STATE_DISABLED;
    viewSettings.plugins = STATE_DISABLED;

    CefWindowInfo window_info = {};
    window_info.SetAsWindowless(nullptr);
    window_info.shared_texture_enabled = true;
	window_info.external_begin_frame_enabled = true;

    CefRefPtr<HtmlView> view(new HtmlView());

    CefBrowserHost::CreateBrowser(
        window_info,
        view,
        "about:blank",
        viewSettings,
        nullptr);*/


}

void WebUIView::resize(uint width, uint height)
{
}

void WebUIView::onDestroy()
{
    //m_browser->GetHost()->CloseBrowser(false);
    //m_browserClient->Release();
}

void WebUIView::navigate(Text url)
{
    if (!WebUIEngine::isInitialized())
        return;

    /*cvar curl = url.std_str();

    var frame = m_browser->GetMainFrame();
    frame->LoadURL(curl);*/
}

void WebUIView::execute(const char* javaScriptSource)
{
}

void WebUIView::bind(const char* bindName, Delegate<void> delegate)
{
}
