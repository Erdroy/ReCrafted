// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Voxels/VoxelObjectBase.h"

/// <summary>
///     VoxelPlanet class. 
///     This is class that is being used by every voxel planet.
///     Uses cube height map for the voxel generation and octree population with dynamic LoD.
/// </summary>
API_CLASS(public, sealed, partial)
class VoxelPlanet final : public VoxelObjectBase
{
    API_CLASS_BODY();

protected:
    void Initialize() override;
};