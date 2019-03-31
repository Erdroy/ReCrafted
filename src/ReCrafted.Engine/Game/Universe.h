// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef UNIVERSE_H
#define UNIVERSE_H

// includes
#include "Voxels/SpaceObject.h"
#include "Common/Actors/EmptyActor.h"
#include "Core/EngineComponent.h"
#include "Physics/RigidBodyActor.h"

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
    //RefPtr<SpaceObject> m_testObject1 = nullptr;
    //VoxelMaterial_t m_selectedMaterial = 1u;

    //RigidBodyActor* m_ball = nullptr;

public:
    virtual ~Universe() = default;

private:
    void OnInit() override;
    void OnDispose() override;
    void Update() override;

    void Simulate();

private:
    void Shoot();
    void DoVoxelModification(VoxelEditMode::_enum mode, VoxelMaterial_t material, float size) const;
};

#endif // UNIVERSE_H
