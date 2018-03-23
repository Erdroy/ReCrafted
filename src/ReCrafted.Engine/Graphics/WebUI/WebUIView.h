// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef WEBUIVIEW_H
#define WEBUIVIEW_H

// includes
#include "ReCrafted.h"
#include "Common/Display.h"
#include "Scripting/Object.h"

#include <cef_render_handler.h>
#include <cef_client.h>

class BrowserClient : public CefClient, public CefRenderHandler
{
    IMPLEMENT_REFCOUNTING(BrowserClient);

public:
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

    CefRefPtr<CefRenderHandler> GetRenderHandler() override
    {
        return this;
    }
};

class WebUIView : public Object
{
    friend class WebUI;
    
private:
    SCRIPTING_API_IMPL()

private:
    CefRefPtr<CefBrowser> m_browser = nullptr;
    CefRefPtr<BrowserClient> m_browserClient = nullptr;

private:
    void init();
    void resize(uint width, uint height);
    void onDestroy() override;

public:
    void navigate(Text url);
    void execute(const char* javaScriptSource);
    void bind(const char* bindName, Delegate<void> delegate);
};

#endif // WEBUIVIEW_H
