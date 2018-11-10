// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef UNIVERSE_H
#define UNIVERSE_H

// includes
#include "Voxels/SpaceObject.h"
#include "Common/GameObject.h"
#include "Core/EngineComponent.h"
#include "Physics/IPhysicsScene.h"

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
    RefPtr<SpaceObject> m_testObject1 = nullptr;
    RefPtr<IPhysicsScene> m_physicsScene = nullptr;

    VoxelMaterial_t m_selectedMaterial = 1u;

    Array<GameObject*> m_projectiles;

public:
    virtual ~Universe() = default;

private:
    void OnInit() override;
    void OnDispose() override;
    void Update() override;

    void TestActors();

    void Simulate();
    void RenderUI();

public:
    static RefPtr<IPhysicsScene> GetPhysicsScene()
    {
        return m_instance->m_physicsScene;
    }
};

#endif // UNIVERSE_H
