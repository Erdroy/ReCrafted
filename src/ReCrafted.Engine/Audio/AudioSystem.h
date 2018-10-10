// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

// includes
#include "ReCrafted.h"
#include "Common/EntityComponentSystem.h"

#include "IAudioEngine.h"
#include "AudioSoundComponent.h"

class AudioSystem : public System
{
public:
    /**
     * \brief The maximal amount of audio channels.
     */
    static const int MaxChannels = 16;

private:
    static IAudioEngine* m_engine;

public:
    AudioSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<AudioSoundComponent>();
    }

public:
    void Initialize() override;
    void Shutdown() override;

    void Update() override;

public:
    static IAudioEngine* Physics()
    {
        return m_engine;
    }
};

#endif // AUDIOSYSTEM_H
