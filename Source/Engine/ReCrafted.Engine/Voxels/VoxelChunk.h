// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Common/ConcurrentQueue.h"
#include "Core/Actors/ActorBase.h"
#include "Physics/Colliders/MeshCollider.h"
#include "Rendering/Models/ModelComponent.h"
#include "Voxels/VoxelChunkData.h"
#include "Voxels/VoxelObjectOctree.h"
#include "Voxels/VoxelChunkMesh.h"

/// <summary>
///     VoxelChunk class.
/// </summary>
class VoxelChunk final
{
    DELETE_COPY_MOVE(VoxelChunk);

private:
    enum class UploadType
    {
        None,

        Swap,
        Clear,

        Count
    };

private:
    VoxelObjectBase* m_voxelObject = nullptr;
    VoxelObjectOctree::Node* m_octreeNode = nullptr;

    RefPtr<VoxelChunkData> m_chunkData = nullptr;
    ModelComponent* m_model = nullptr;
    
    int m_lod = 0;
    uint64_t m_id = 0u;
    Transform m_transform = {};

    Lock m_uploadLock = {};
    std::atomic<UploadType> m_uploadType = UploadType::None;
    RefPtr<VoxelChunkMesh> m_mesh;
    RefPtr<VoxelChunkMesh> m_newMesh;

    ConcurrentQueue<std::pair<RefPtr<VoxelChunkMesh>, MeshCollider*>> m_disposeQueue;

    bool m_collisionAttached = false;
    MeshCollider* m_meshCollider = nullptr;
    MeshCollider* m_newMeshCollider = nullptr;

public:
    explicit VoxelChunk(VoxelObjectBase* voxelObject) : m_voxelObject(voxelObject) {}
    ~VoxelChunk() = default;

public:
    void Initialize(VoxelObjectOctree::Node* node);

public:
    void Upload();
    void SetUpload(const RefPtr<VoxelChunkMesh>& mesh, MeshCollider* collider, UploadType uploadType);

    void OnBeginRender(int meshIndex);
    void OnCreate();
    void OnDestroy();

public:
    void Generate(IVoxelMesher* mesher);
    void Rebuild(IVoxelMesher* mesher);

public:
    /// <summary>
    ///     Sets the physics state of this chunk.
    /// </summary>
    /// <param name="hasCollision">
    ///     When true, this chunk will get collision with the dynamic world.
    /// </param>
    void SetPhysicsState(bool hasCollision);

    /// <summary>
    ///     Sets transform of this chunk.
    /// </summary>
    /// <param name="transform"></param>
    void SetTransform(const Transform& transform)
    {
        m_transform = transform;
    }

    /// <summary>
    ///     Gets or sets the chunk's visibility state. Invisible when true.
    /// </summary>
    void SetVisible(bool isVisible);

    /// <summary>
    ///     Gets or sets the chunk's visibility state. Invisible when true.
    /// </summary>
    bool GetVisible() const
    {
        return m_model && m_model->Active;
    }

    /// <summary>
    ///     Returns true when this chunk needs upload, i.e. got new mesh.
    /// </summary>
    bool NeedsUpload() const
    {
        return m_uploadType != UploadType::None;
    }

    /// <summary>
    ///     Returns the chunk data structure that contains the hermite voxel data for this chunk.
    /// </summary>
    RefPtr<VoxelChunkData> ChunkData() const
    {
        return m_chunkData;
    }

    /// <summary>
    ///     Returns true, when this chunk has mesh and it is a valid one.
    /// </summary>
    bool HasMesh() const
    {
        return m_mesh != nullptr && m_mesh->IsValid();
    }

public:
    /// <summary>
    ///     Calculates unique 64-bit chunk id from given position 
    ///     (that is the unique position in center of the chunk).
    /// </summary>
    /// <param name="position">The chunk position.</param>
    /// <returns>The 64-bit chunk id.</returns>
    static uint64_t CalculateChunkId(const Vector3& position);
};