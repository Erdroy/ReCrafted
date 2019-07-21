// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Scripting/Object.h"
#include "Voxels/VoxelObjectOctree.h"

/// <summary>
///     The base class of all voxel objects (planets, asteroids etc.).
/// </summary>
API_CLASS(public, abstract, partial)
class VoxelObjectBase : public Object
{
    API_CLASS_BODY();

private:
    VoxelObjectOctree* m_octree;

public:
    VoxelObjectBase();
    virtual ~VoxelObjectBase();

protected:
    virtual void Initialize() = 0;
};
