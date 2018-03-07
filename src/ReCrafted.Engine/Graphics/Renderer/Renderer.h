// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef RENDERER_H
#define RENDERER_H

// includes
#include "ReCrafted.h"
#include "Core/EngineComponent.h"

class Renderer : public EngineComponent<Renderer>
{
    friend class EngineMain;

private:
    SCRIPTING_API_IMPL()

private:
    void bgfx_initialize();
    void bgfx_shutdown();

private:
    void onInit() override;
    void onDispose() override;

    void update();
    void render();

public:
};

#endif // RENDERER_H
