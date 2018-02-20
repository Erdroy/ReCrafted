// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef UNIVERSE_H
#define UNIVERSE_H

// includes
#include "World/SpaceObject/SpaceObject.h"

/**
 * \brief Universe class - main class for whole game world.
 * Manages voxel engine, AI, players, handles multiplayer etc.
 */
class Universe : public EngineComponent
{
    friend class EngineMain;

private:
    static Universe* m_instance;

private:
    Ptr<SpaceObject> m_testObject1 = nullptr;

public:
    Universe() { m_instance = this; }

private:
    void onInit() override;
    void onShutdown() override;

    void update();
    void simulate();
    void render();

public:
    static Universe* getInstance()
    {
        return m_instance;
    }
};

#endif // UNIVERSE_H