// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

// includes
#include "ReCrafted.h"
#include "Common/EntityComponentSystem.h"

#include "IAudioEngine.h"
#include "AudioSourceComponent.h"

class AudioSystem : public ComponentSystem
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
        RequireComponent<AudioSourceComponent>();
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
