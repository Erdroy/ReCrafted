// ReCrafted (c) 2016-2018 Always Too Late

#include "WebUIView.h"
#include "WebUIEngine.h"

#include <cef_app.h>
#include <cef_client.h>
#include <cef_render_handler.h>
#include <internal/cef_ptr.h>
#include <cef_browser.h>

#include <condition_variable>
#include <atomic>

#include "directx11impl.h"

#undef TEXT

#include "Common/Text.h"
#include "Common/Display.h"
#include "Core/Lock.h"
#include "Core/Delegate.h"
#include "Graphics/Graphics.h"

// https://github.com/daktronics/cef-mixer/blob/master/src/html_layer.cpp
class CEFView : public CefClient, public CefRenderHandler, public CefLifeSpanHandler
{
    friend class WebUIView;

private:
    CefRefPtr<CefBrowser> m_browser = nullptr;
    Lock m_browserLock = {};
    std::mutex m_lock = {};

    std::condition_variable m_syncVar = {};
    std::atomic_int32_t m_swapCount;

    //bgfx::TextureHandle m_bgfxFrontBuffer = {};
    //bgfx::TextureHandle m_bgfxBackBuffer = {};

    uint64 m_syncKey = 0u;

    void* m_sharedHandle = nullptr;
    ID3D11Texture2D* m_sharedBuffer = nullptr;
    ID3D11Texture2D* m_frontBuffer = nullptr;
    ID3D11Texture2D* m_backBuffer = nullptr;

    ID3D11Device* m_device = nullptr;
    ID3D11DeviceContext* m_deviceContext = nullptr;

public:
    ~CEFView()
    {
        //bgfx::destroy(m_bgfxFrontBuffer);
        //bgfx::destroy(m_bgfxBackBuffer);
        SafeRelease(m_frontBuffer);
        SafeRelease(m_backBuffer);
    }

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

    void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height) override { }

    void OnAcceleratedPaint(
        CefRefPtr<CefBrowser> browser,
        PaintElementType type,
        const RectList& dirtyRects,
        void* share_handle,
        uint64 sync_key) override
    {
        if (type == PET_POPUP)
            return;

        //https://github.com/daktronics/cef-mixer/blob/master/src/html_layer.cpp#L83
        //https://github.com/daktronics/cef-mixer/blob/master/src/d3d11.cpp#L526


        if(m_browser)
        {
            // TODO: if previous frame is not rendered yet, wait here
            /*{
                std::unique_lock<std::mutex> lock(m_lock);
                m_syncVar.wait(m_lock, [&]() {
                    return m_swapCount.load() == 0;
                });
            }*/

            // setup lock guard
            std::lock_guard<std::mutex> guard(m_lock);

            // check shared handle
            if (m_sharedBuffer)
            {
                // check if shared handle changed if shared buffer is not null,
                // if so - release textures
                if (m_sharedHandle != share_handle)
                {
                    m_sharedHandle = nullptr;

                    // release resources
                    //bgfx::destroy(m_bgfxFrontBuffer);
                    //bgfx::destroy(m_bgfxBackBuffer);
                    SafeRelease(m_frontBuffer);
                    SafeRelease(m_backBuffer);
                }
            }

            if (!m_sharedBuffer)
            {
                // open shared buffer texture
                m_sharedBuffer = RendererImpl::openSharedTexture(share_handle);
                m_sharedHandle = share_handle;

                // create back and front buffer if shared buffer is present
                if (m_sharedBuffer)
                {
                    /*cvar flags = 0
                        | BGFX_TEXTURE_RT
                        | BGFX_TEXTURE_MIN_POINT
                        | BGFX_TEXTURE_MAG_POINT
                        | BGFX_TEXTURE_MIP_POINT
                        | BGFX_TEXTURE_U_CLAMP
                        | BGFX_TEXTURE_V_CLAMP;

                    // get shared buffer texture info
                    cvar width = RendererImpl::getWidth(m_sharedBuffer);
                    cvar height = RendererImpl::getHeight(m_sharedBuffer);
                    cvar format = RendererImpl::getFormat(m_sharedBuffer);

                    // create textures
                    m_bgfxFrontBuffer = bgfx::createTexture2D(width, height, false, 1, bgfx::TextureFormat::BGRA8, flags, nullptr);
                    m_bgfxBackBuffer = bgfx::createTexture2D(width, height, false, 1, bgfx::TextureFormat::BGRA8, flags, nullptr);

                    m_frontBuffer = RendererImpl::createTexture(width, height, format);
                    m_backBuffer = RendererImpl::createTexture(width, height, format);

                    // override bgfx texture
                    bgfx::overrideInternal(m_bgfxFrontBuffer, reinterpret_cast<uintptr_t>(m_frontBuffer));
                    bgfx::overrideInternal(m_bgfxBackBuffer, reinterpret_cast<uintptr_t>(m_backBuffer));*/
                }
            }

            // set sync key
            m_syncKey = sync_key;
            m_swapCount = 1;
        }
    }

public:
    void init()
    {
        //m_device = static_cast<ID3D11Device*>(bgfx::getInternalData()->context);
        //m_device->GetImmediateContext(&m_deviceContext);
    }

    void update()
    {
        const int64_t time_us = 0u;
        const int64_t deadline_us = 0u;
        const int64_t interval_us = 0u;

        ScopeLock(m_browserLock);
        
        if(m_browser)
            m_browser->GetHost()->SendExternalBeginFrame(time_us, deadline_us, interval_us);
    }

    /*bgfx::TextureHandle getRenderTexture() const
    {
        return m_bgfxFrontBuffer;
    }
    */
    void swap()
    {
        // setup lock guard
        std::lock_guard<std::mutex> guard(m_lock);
        ScopeLock(m_browserLock);

        if (!m_browser || !m_frontBuffer || !m_backBuffer)
            return;

        // swap front with back buffer ONLY when any frame was rendered
        if(m_swapCount)
        {
            std::swap(m_frontBuffer, m_backBuffer);
            //std::swap(m_bgfxFrontBuffer, m_bgfxBackBuffer);
        }
        
        // issue copy when necessary
        if(m_backBuffer && m_sharedBuffer)
        {
            IDXGIKeyedMutex* syncMutex;
            m_sharedBuffer->QueryInterface(__uuidof(IDXGIKeyedMutex), reinterpret_cast<LPVOID*>(&syncMutex));

            if (!syncMutex)
                return;

            if (syncMutex->AcquireSync(m_syncKey, 0u) == WAIT_OBJECT_0)
            {
                // if state is WAIT_OBJECT_0 we have something 
                // new to render, so copy the resources now.
                m_deviceContext->CopyResource(m_backBuffer, m_sharedBuffer);
            }

            // release sync right now
            syncMutex->ReleaseSync(m_syncKey);
        }

        // notify about this swap
        m_swapCount = 0;
        m_syncVar.notify_one();
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

    // initialize cef view
    m_viewBase = new CEFView();
    m_viewBase->init();

    // create brower for current view
    CefBrowserHost::CreateBrowser(
        window_info,
        static_cast<CEFView*>(m_viewBase),
        "about:blank",
        viewSettings,
        nullptr);
}

void WebUIView::update()
{
    if (!WebUIEngine::isInitialized())
        return;

    m_viewBase->update();
}

void WebUIView::resize(uint width, uint height)
{
    if (!WebUIEngine::isInitialized())
        return;

    cvar view = getView();
    view->update();
}

void WebUIView::render()
{
    if (!WebUIEngine::isInitialized())
        return;

    // swap texture
    cvar view = getView();
    view->swap();

    // draw texture
    Graphics::getInstance()->setStage(RenderStage::DrawWebUI);
   // Graphics::getInstance()->blit(0, view->getRenderTexture(), true);
}

void WebUIView::onDestroy()
{
    cvar view = getView();

    if(view && view->m_browser)
        view->m_browser->GetHost()->CloseBrowser(false);
}

CEFView* WebUIView::getView() const
{
    return static_cast<CEFView*>(m_viewBase);
}

void WebUIView::navigate(Text url)
{
    if (!WebUIEngine::isInitialized())
        return;

    cvar view = getView();
    cvar curl = url.std_str();
    var frame = view->m_browser->GetMainFrame();
    frame->LoadURL(curl);
}

void WebUIView::execute(const char* javaScriptSource)
{
    if (!WebUIEngine::isInitialized())
        return;

    cvar view = getView();
    view->m_browser->GetMainFrame()->ExecuteJavaScript(CefString(javaScriptSource), CefString(""), 0);
}

void WebUIView::bind(const char* bindName, Delegate<void> delegate)
{
}
