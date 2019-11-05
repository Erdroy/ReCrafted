// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Common/List.h"
#include "Core/Threading/Task.h"
#include "Physics/RigidBodyActor.h"
#include "Scripting/Object.h"
#include "Voxels/Assets/VoxelObjectAsset.h"
#include "Voxels/Voxel.h"
#include "Voxels/VoxelObjectOctree.h"
#include "Voxels/VoxelStorage.h"

API_USING("ReCrafted.API.Core.Actors");
API_USING("ReCrafted.API.Physics");
API_USING("ReCrafted.API.Mathematics");

API_CUSTOM_TRANSLATION(VoxelMaterial_t, "ushort")

/// <summary>
///     The base class of all voxel objects (planets, asteroids etc.).
/// </summary>
API_CLASS(public, abstract, partial)
class VoxelObjectBase : public Object
{
    DELETE_COPY_MOVE(VoxelObjectBase)
    API_CLASS_BODY();
    friend class VoxelObjectAsset;
    friend class VoxelObjectManager;

public:
    struct OctreeInitializeTask final : ITask
    {
    public: /* Input */
        VoxelObjectBase* VoxelObject = nullptr;
        VoxelObjectOctree* Octree = nullptr;

    public:
        void Execute(void* userData) override;
        void Finish() override;
    };

    struct OctreeViewUpdateTask final : ITask
    {
    public: /* Input */
        VoxelObjectBase* VoxelObject = nullptr;
        VoxelObjectOctree* Octree = nullptr;
        List<Vector3> Points = {};

    private: /* Local */
        List<VoxelObjectOctree::Node*> NodesToPopulate = {};
        List<VoxelObjectOctree::Node*> NodesToDepopulate = {};

    private:
        void UpdateNode(VoxelObjectOctree::Node* node);

    public:
        void Execute(void* userData) override;
        void Finish() override;
    };

protected:
    VoxelObjectOctree* m_octree = nullptr;
    VoxelObjectAsset* m_asset = nullptr;

    VoxelStorage* m_storage = nullptr;
    VoxelGenerator* m_generator = nullptr;

    RigidBodyActor* m_rigidBodyActor = nullptr;

    bool m_initialLoading = true;
    float m_viewUpdateFrequency = 1 / 3.0f;
    float m_lastViewUpdate = 0.0f;

    List<ActorBase*> m_viewActors = {};

private:
    void UpdateViews();

public:
    VoxelObjectBase();
    virtual ~VoxelObjectBase();

protected:
    API_FUNCTION()
    virtual void Initialize();

public:
    void Update();

public:
    /// <summary>
    ///     Adds given actor to the view list of this voxel object.
    ///     It will be used to make sure, that the terrain around the actor
    ///     is always loaded with proper LoD.
    /// </summary>
    /// <param name="viewActor">The actor.</param>
    /// <remarks>
    ///     Remember to remove the actor using RemoveViewActor(...) before it is destroyed.
    /// </remarks>
    API_FUNCTION()
    void AddViewActor(ActorBase* viewActor);

    /// <summary>
    ///     Removes given actor from the view list of this voxel object.
    /// </summary>
    /// <param name="viewActor">The actor.</param>
    API_FUNCTION()
    void RemoveViewActor(ActorBase* viewActor);

    /// <summary>
    ///     Modifies this voxel space object, using given method.
    /// </summary>
    /// <param name="material">The voxel material that will be applied on every voxel.</param>
    /// <param name="mode">The edit mode.</param>
    /// <param name="position">The edit position.</param>
    /// <param name="size">The edit size.</param>
    API_FUNCTION()
    void Modify(VoxelMaterial_t material, VoxelEditMode mode, const Vector3& position, float size);

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

    /// <summary>
    ///     Gets the RigidBody actor that is representing this VoxelObject's physics state and collision.
    /// </summary>
    API_PROPERTY()
    RigidBodyActor* RigidBody() const
    {
        return m_rigidBodyActor;
    }

    /// <summary>
    ///     Gets the VoxelStorage instance owned by this VoxelObject.
    /// </summary>
    VoxelStorage* Storage() const
    {
        return m_storage;
    }

    /// <summary>
    ///     Gets the VoxelGenerator instance owned by this VoxelObject.
    /// </summary>
    VoxelGenerator* Generator() const
    {
        return m_generator;
    }

    /// <summary>
    ///     Gets the VoxelObjectOctree instance owned by this VoxelObject.
    /// </summary>
    VoxelObjectOctree* Octree() const
    {
        return m_octree;
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
