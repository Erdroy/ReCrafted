// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef WEBUIENGINE_H
#define WEBUIENGINE_H

// includes
#include "ReCrafted.h"

class WebUIEngine : public Singleton<WebUIEngine>
{
private:
    bool m_initialized = false;
    bool m_needsViewUpdate = false;

public:
    void Init();
    void OnDispose() override;

    void Render();

public:
    bool NeedsViewsUpdate() const
    {
        return m_needsViewUpdate;
    }

public:
    static void* CreateUIView(WebUIView* view);
    static bool IsInitialized();
};

#endif // WEBUIENGINE_H
