// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef WEBUIENGINE_H
#define WEBUIENGINE_H

// includes
#include "ReCrafted.h"

class WebUIEngine : public IDisposable
{
public:
    void init();
    void update();
    void dispose() override;

public:
    static void runChildren();
};

#endif // WEBUIENGINE_H
