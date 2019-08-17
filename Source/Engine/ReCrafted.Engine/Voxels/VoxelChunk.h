// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Core/Actors/ActorBase.h"
#include "Rendering/Models/ModelComponent.h"
#include "Voxels/VoxelChunkData.h"
#include "Voxels/VoxelObjectOctree.h"
#include "VoxelChunkMesh.h"

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
    Transform m_transform = {};

    int m_lod = 0;
    uint64_t m_id = 0u;

    Vector3 m_position = {};

    Lock m_uploadLock = {};
    std::atomic<UploadType> m_uploadType = UploadType::None;
    RefPtr<VoxelChunkMesh> m_mesh;

public:
    explicit VoxelChunk(VoxelObjectBase* voxelObject) : m_voxelObject(voxelObject) {}
    ~VoxelChunk() = default;

private:
    void SetUpload(const RefPtr<VoxelChunkMesh>& mesh, UploadType uploadType);
    void OnBeginRender(int meshIndex);

public:
    void Upload();
    void Initialize(VoxelObjectOctree::Node* node);

public:
    void Generate(IVoxelMesher* mesher);
    void Rebuild(IVoxelMesher* mesher);

public:
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

public:
    static uint64_t CalculateChunkId(const Vector3& position);
};