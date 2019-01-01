// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef IAUDIOENGINE_H
#define IAUDIOENGINE_H

// includes
#include "ReCrafted.h"

class IAudioEngine
{
public:
    virtual ~IAudioEngine() = default;

public:
    virtual void Initialize() = 0;
    virtual void Update() = 0;
    virtual void Shutdown() = 0;
};

#endif // IAUDIOENGINE_H
