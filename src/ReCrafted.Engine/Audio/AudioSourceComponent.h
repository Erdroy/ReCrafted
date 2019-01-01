// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef AUDIOSOURCECOMPONENT_H
#define AUDIOSOURCECOMPONENT_H

#include "Common/Entities/Component.h"
#include "AudioSound.h"

struct AudioSourceComponent : Component
{
    AudioSound* sound;
};

#endif // AUDIOSOURCECOMPONENT_H
