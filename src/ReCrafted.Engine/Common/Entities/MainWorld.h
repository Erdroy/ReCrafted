// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef MAINWORLD_H
#define MAINWORLD_H

// includes
#include "ReCrafted.h"
#include "Common/EntityComponentSystem.h"
#include "Core/EngineComponent.h"

class MainWorld : public EngineComponent<MainWorld>
{
private:
    RefPtr<World> m_world = nullptr;

public:
    void OnInit() override;
    void OnDispose() override;

    void Update() override;
    void Simulate();
};

#endif // MAINWORLD_H
