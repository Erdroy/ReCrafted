// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef INTERPOLATION_H
#define INTERPOLATION_H

// includes
#include "ReCrafted.h"
#include "Core/Math/Math.h"

class Interpolation
{
public:
    static float sampleCubic(const float v0, const float v1, const float v2, const float v3, const float frac)
    {
        cvar a = (v3 - v2) - (v0 - v1);
        cvar b = (v0 - v1) - a;
        cvar c = v2 - v0;
        cvar d = v1;

        return a * Math::powf(frac, 3) + b * Math::powf(frac, 2) + c * frac + d;
    }
};

#endif // INTERPOLATION_H
