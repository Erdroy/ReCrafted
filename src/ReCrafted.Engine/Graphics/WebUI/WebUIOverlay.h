// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef WEBUIOVERLAY_H
#define WEBUIOVERLAY_H

class WebUIOverlay
{
public:
    virtual ~WebUIOverlay() = default;

public:
    virtual void Draw() = 0;
    virtual void Resize(uint32_t width, uint32_t height) = 0;
};

#endif // WEBUIOVERLAY_H
