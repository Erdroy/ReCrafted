// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef WEBUIRENDERER_H
#define WEBUIRENDERER_H

#include "WebUIGPUDriver.h"

#include <Ultralight/Ultralight.h>

class WebUIRenderer
{
private:
    ultralight::Renderer* m_renderer = nullptr;
    WebUIGPUDriver* m_driver = nullptr;

    bool m_needsViewUpdate = false;

public:
    WebUIRenderer(ultralight::Renderer* renderer, WebUIGPUDriver* driver);
    ~WebUIRenderer();

public:
    void Update();
    void Render();
    void Resize(uint16_t width, uint16_t height);

public:
    bool NeedsViewsUpdate() const
    {
        return m_needsViewUpdate;
    }
};

#endif // WEBUIRENDERER_H
