// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef UNIVERSE_H
#define UNIVERSE_H

// includes
#include "World/SpaceObject/SpaceObject.h"
#include "Core/EngineComponent.h"

/**
 * \brief Universe class - main class for whole game world.
 * Manages voxel engine, AI, players, handles multiplayer etc.
 */
class Universe : public EngineComponent<Universe>
{
private:
    friend class EngineMain;
    friend class Graphics;

private:
    Ref<SpaceObject> m_testObject1 = nullptr;

public:
    virtual ~Universe() = default;

private:
    void onInit() override;
    void onDispose() override;
    void update() override;

    void simulate();
    void render();
};

#endif // UNIVERSE_H
