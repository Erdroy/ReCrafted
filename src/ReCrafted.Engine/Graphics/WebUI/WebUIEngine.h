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

private:
    void runCEF();

public:
    void init();
    void onDispose() override;

public:
    static void runChildren();
    static bool isInitialized();
};

#endif // WEBUIENGINE_H
