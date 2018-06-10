// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef WEBUIENGINE_H
#define WEBUIENGINE_H

// includes
#include "ReCrafted.h"

typedef void* WebUIRenderer;
typedef void* WebUIDriver;

class WebUIEngine : public Singleton<WebUIEngine>
{
private:
    bool m_initialized = false;
    bool m_needsViewUpdate = false;

public:
    void Init();
    void OnDispose() override;

    void Update();
    void Render();

public:
    bool NeedsViewsUpdate() const
    {
        return m_needsViewUpdate;
    }

public:
    static WebUIRenderer GetRenderer();
    static WebUIDriver GetDriver();

    static bool IsInitialized();
};

#endif // WEBUIENGINE_H
