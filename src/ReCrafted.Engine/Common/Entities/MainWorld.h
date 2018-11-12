// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef MAINWORLD_H
#define MAINWORLD_H

// includes
#include "ReCrafted.h"
#include "Common/EntityComponentSystem.h"
#include "Core/EngineComponent.h"
#include "Audio/AudioSystem.h"

class MainWorld : public EngineComponent<MainWorld>
{
private:
    World* m_world = nullptr;

    AudioSystem* m_audioSystem = nullptr;

public:
    void OnInit() override;
    void OnDispose() override;

    void Update() override;
    void Simulate();

public:
    static World* GetWorld()
    {
        ASSERT(m_instance);
        return m_instance->m_world;
    }
};

#endif // MAINWORLD_H
