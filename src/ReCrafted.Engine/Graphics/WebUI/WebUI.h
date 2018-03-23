// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef WEBUI_H
#define WEBUI_H

// includes
#include "ReCrafted.h"
#include "Core/EngineComponent.h"
#include "Core/Containers/Array.h"

class WebUI : public EngineComponent<WebUI>
{
    friend class Renderer;
    friend class EngineMain;

private:
    SCRIPTING_API_IMPL()

private:
    Array<Ptr<WebUIView>> m_views = {};
    Ptr<WebUIEngine> m_engine = nullptr;

private:
    void onInit() override;
    void onDispose() override;
    void update() override;

    void resize(uint width, uint height);
    void render();

public:
    virtual ~WebUI() = default;

public:
    Ptr<WebUIView> createView();
};

#endif // WEBUI_H
