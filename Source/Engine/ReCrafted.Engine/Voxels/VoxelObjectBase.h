// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Core/Threading/Task.h"
#include "Scripting/Object.h"
#include "Voxels/Assets/VoxelObjectAsset.h"
#include "Voxels/Voxel.h"
#include "Voxels/VoxelObjectOctree.h"
#include "Voxels/VoxelStorage.h"

/// <summary>
///     The base class of all voxel objects (planets, asteroids etc.).
/// </summary>
API_CLASS(public, abstract, partial)
class VoxelObjectBase : public Object
{
    DELETE_COPY_MOVE(VoxelObjectBase);
    API_CLASS_BODY();
    friend class VoxelObjectAsset;
    friend class VoxelObjectManager;

public:
    struct OctreeInitializeTask final : ITask
    {
        VoxelObjectBase* VoxelObject = nullptr;
        VoxelObjectOctree* Octree = nullptr;

    public:
        void Execute(void* userData) override;
        void Finish() override;
    };

protected:
    VoxelObjectOctree* m_octree = nullptr;
    VoxelObjectAsset* m_asset = nullptr;

    VoxelStorage* m_storage = nullptr;
    VoxelGenerator* m_generator = nullptr;

    bool m_initialLoading = true;

public:
    VoxelObjectBase();
    virtual ~VoxelObjectBase();

protected:
    API_FUNCTION()
    virtual void Initialize();

public:
    void Update();

    void DebugDraw();

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
    /// <summary>
    ///     The radius of this voxel object.
    /// </summary>
    API_PROPERTY()
    double Radius() const
    {
        return m_octree->m_bounds.radius;
    }

    /// <summary>
    ///     The world-space position of this voxel object.
    /// </summary>
    Vector3d Position() const
    {
        return m_octree->m_bounds.center;
    }

    /// <summary>
    ///     The asset of this voxel object.
    /// </summary>
    API_PROPERTY()
    VoxelObjectAsset* Asset() const
    {
        return m_asset;
    }

    VoxelStorage* Storage() const
    {
        return m_storage;
    }

    VoxelGenerator* Generator() const
    {
        return m_generator;
    }

    /// <summary>
    ///     Gets the loading state. True when this voxel object is still loading the initial data.
    /// </summary>
    API_PROPERTY()
    bool IsLoading() const 
    {
        return m_initialLoading;
    }
};
