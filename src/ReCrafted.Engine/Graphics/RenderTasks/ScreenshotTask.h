// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef SCREENSHOTTASK_H
#define SCREENSHOTTASK_H

// includes
#include "Renderer/Renderer.hpp"
#include "Renderer/RHI/RHIBase.h"
#include "lodepng.h"

struct ScreenshotTask : Renderer::RenderTask
{
private:
    Renderer::Texture2DHandle m_frameTexture;
    Text m_outputFile;

public:
    ScreenshotTask(Renderer::Texture2DHandle frameTexture, Text outputFile) : m_frameTexture(frameTexture), m_outputFile(outputFile) { }

public:
    void execute() override
    {
        cvar width = Display::GetWidth();
        cvar height = Display::GetHeight();
        cvar bufferSize = width * height * 4;
        cvar buffer = new byte[bufferSize];

        // Get texture data
        static_cast<Renderer::RHI::RHIBase*>(rhi)->GetTextureSubresource(m_frameTexture, buffer, bufferSize, 0u);

        lodepng::encode(m_outputFile.StdStr(), buffer, width, height, LCT_RGBA);

        delete[] buffer;
    }
};

#endif // SCREENSHOTTASK_H
