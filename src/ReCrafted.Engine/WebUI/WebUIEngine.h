// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef WEBUIENGINE_H
#define WEBUIENGINE_H

// includes
#include "ReCrafted.h"
#include "WebUIViewport.h"

class WebUIEngine : public Singleton<WebUIEngine>
{
private:
    bool m_initialized = false;

public:
    void Init();
    void OnDispose() override;

    void Update();
    void Render();

public:
    static WebUIViewport* CreateUIViewport(WebUIView* view, bool fullscreen);
    static bool IsInitialized();
};

#endif // WEBUIENGINE_H
