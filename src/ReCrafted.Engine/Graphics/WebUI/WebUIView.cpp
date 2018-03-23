// ReCrafted (c) 2016-2018 Always Too Late

#include "WebUIView.h"

#include <cef_app.h>
#include <cef_client.h>
#include <cef_render_handler.h>
#include <internal/cef_ptr.h>
#include <cef_browser.h>

#undef TEXT

#include "Common/Text.h"
#include "Core/Logger.h"
#include "Core/Delegate.h"
#include "Platform/Platform.h"

void WebUIView::init()
{
    CefBrowserSettings browserSettings = {};
    browserSettings.windowless_frame_rate = 60;
    browserSettings.plugins = STATE_DISABLED;
    browserSettings.webgl = STATE_ENABLED;

    CefWindowInfo window_info = {};
    window_info.SetAsWindowless(nullptr);
    window_info.shared_textures_enabled = true;

    m_browserClient = new BrowserClient();
    m_browser = CefBrowserHost::CreateBrowserSync(window_info, m_browserClient, "", browserSettings, nullptr);

    var host = m_browser->GetHost();
    host->WasResized();
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
    cvar curl = url.std_str();

    var frame = m_browser->GetMainFrame();
    frame->LoadURL(curl);
}

void WebUIView::execute(const char* javaScriptSource)
{
}

void WebUIView::bind(const char* bindName, Delegate<void> delegate)
{
}
