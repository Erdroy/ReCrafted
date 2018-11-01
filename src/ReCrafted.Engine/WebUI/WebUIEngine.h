// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef WEBUIENGINE_H
#define WEBUIENGINE_H

// includes
#include "ReCrafted.h"
#include "WebUIOverlay.h"
#include "WebUIView.h"

class WebUIGPUDriver;
class WebUIRenderer;

class WebUIEngine : public Singleton<WebUIEngine>
{
private:
    bool m_initialized = false;
    bool m_needsViewUpdate = false;

    WebUIGPUDriver* m_gpuDriver = nullptr;
    WebUIRenderer* m_renderer = nullptr;

public:
    void Init();
    void OnDispose() override;

    void Update();
    void Render();

public:
    void Resize(uint width, uint height);
    bool NeedsViewsUpdate() const;

public:
    static WebUIOverlay* CreateUIView(WebUIView* view, bool fullscreen);
    static bool IsInitialized();
};

#endif // WEBUIENGINE_H
