// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Scripting/Object.h"
#include "Voxels/Voxel.h"
#include "Voxels/VoxelObjectOctree.h"
#include "Voxels/Assets/VoxelObjectAsset.h"

/// <summary>
///     The base class of all voxel objects (planets, asteroids etc.).
/// </summary>
API_CLASS(public, abstract, partial)
class VoxelObjectBase : public Object
{
    DELETE_COPY_MOVE(VoxelObjectBase);
    API_CLASS_BODY();

protected:
    VoxelObjectOctree* m_octree;

    BoundingSphereD m_bounds = {};
    VoxelObjectAsset* m_asset = nullptr;

public:
    VoxelObjectBase();
    virtual ~VoxelObjectBase();

protected:
    API_FUNCTION()
    virtual void Initialize();

    void Load(const char* assetFile);

public:
    void Update();

public:
    /// <summary>
    ///     Modifies this voxel space object, using given method.
    /// </summary>
    /// <param name="material">The voxel material that will be applied on every voxel.</param>
    /// <param name="mode">The edit mode.</param>
    /// <param name="position">The edit position.</param>
    /// <param name="size">The edit size.</param>
    void Modify(VoxelMaterial_t material, VoxelEditMode mode, const Vector3d& position, float size);

public:
    double Radius() const
    {
        return m_bounds.radius;
    }

    Vector3d Position() const
    {
        return m_bounds.center;
    }

    VoxelObjectAsset* Asset() const
    {
        return m_asset;
    }
};
