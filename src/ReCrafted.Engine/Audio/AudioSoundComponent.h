// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef AUDIOSOUNDCOMPONENT_H
#define AUDIOSOUNDCOMPONENT_H

#include "Common/Entities/Component.h"
#include "AudioSound.h"

struct AudioSoundComponent : Component
{
    AudioSound* sound;
};

#endif // AUDIOSOUNDCOMPONENT_H
