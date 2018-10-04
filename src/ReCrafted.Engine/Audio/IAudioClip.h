// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef IAUDIOCLIP_H
#define IAUDIOCLIP_H

// includes
#include "ReCrafted.h"

class IAudioClip
{
public:
    virtual ~IAudioClip() = default;
    
public:
    virtual void Create() = 0;
    virtual void Unload() = 0;
};


#endif // IAUDIOCLIP_H
