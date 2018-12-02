// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef WEBUIVIEWPORT_H
#define WEBUIVIEWPORT_H
#include "Graphics/Texture.h"

class WebUIViewport
{
public:
    virtual ~WebUIViewport() = default;

public:
    virtual void Update() = 0;
    virtual void Render() = 0;

    virtual void Resize(uint width, uint height) = 0;

    virtual void Navigate(const char* url) = 0;
    virtual void Execute(const char* javaScriptSource) = 0;

    virtual Renderer::Texture2DHandle GetTexture() const = 0;
};

#endif // WEBUIVIEWPORT_H
