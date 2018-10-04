// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

// includes
#include "ReCrafted.h"
#include "Core/EngineComponent.h"
#include "IAudioEngine.h"

class AudioManager : public EngineComponent<AudioManager>
{
public:
    /**
     * \brief The maximal amount of audio channels.
     */
    static const int MaxChannels = 16;

private:
    IAudioEngine* m_engine = nullptr;

protected:
    void OnInit() override;
    void Update() override;
    void OnDispose() override;
};

#endif // AUDIOMANAGER_H
