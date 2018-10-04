// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef FMODCLIP_H
#define FMODCLIP_H

// includes
#include "ReCrafted.h"
#include "Audio/IAudioClip.h"

#include <fmod.hpp>

class FMODClip : public IAudioClip
{
private:
    FMOD::Sound* m_sound = nullptr;

public:
    void Create() override;
    void Unload() override;
};

#endif // FMODCLIP_H
