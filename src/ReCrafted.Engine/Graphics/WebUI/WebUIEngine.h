// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef WEBUIENGINE_H
#define WEBUIENGINE_H

// includes
#include "ReCrafted.h"
#include "WebUIOverlay.h"

class WebUIEngine : public Singleton<WebUIEngine>
{
private:
    bool m_initialized = false;
    bool m_needsViewUpdate = false;

public:
    void Init();
    void OnDispose() override;

    void Render();
    void OnRendered();

public:
    void Resize(uint width, uint height);

    bool NeedsViewsUpdate() const
    {
        return m_needsViewUpdate;
    }

public:
    static WebUIOverlay* CreateUIView(WebUIView* view, bool fullscreen);
    static bool IsInitialized();
};

#endif // WEBUIENGINE_H
