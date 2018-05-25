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
    friend class Graphics;
    friend class EngineMain;

private:
SCRIPTING_API_IMPL()

private:
    Array<Ref<WebUIView>> m_views = {};
    WebUIEngine* m_engine = nullptr;

private:
    void OnInit() override;
    void OnDispose() override;
    void Update() override;

    void Resize(uint width, uint height);
    void Render();

public:
    virtual ~WebUI() = default;

public:
    Ref<WebUIView> CreateView();
};

#endif // WEBUI_H
