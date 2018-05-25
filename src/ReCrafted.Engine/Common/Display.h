// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef DISPLAY_H
#define DISPLAY_H

// includes
#include "ReCrafted.h"

class Display
{
private:
SCRIPTING_API_IMPL()

public:
STATIC_PROPERTY(uint, Width);
STATIC_PROPERTY(uint, Height);

public:
    FORCEINLINE static float GetAspectRatio()
    {
        return static_cast<float>(m_Width) / static_cast<float>(m_Height);
    }
};

#endif // DISPLAY_H
