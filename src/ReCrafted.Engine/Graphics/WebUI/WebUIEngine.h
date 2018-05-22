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

public:
    void init();
    void onDispose() override;

    void Update();
    void Render();

public:
    static bool isInitialized();
};

#endif // WEBUIENGINE_H
