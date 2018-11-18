// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef UNIVERSE_H
#define UNIVERSE_H

// includes
#include "Voxels/SpaceObject.h"
#include "Common/Actors/EmptyActor.h"
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
    RefPtr<SpaceObject> m_testObject1 = nullptr;
    VoxelMaterial_t m_selectedMaterial = 1u;

public:
    virtual ~Universe() = default;

private:
    void OnInit() override;
    void OnDispose() override;
    void Update() override;

    void Simulate();
    void RenderUI();
};

#endif // UNIVERSE_H
