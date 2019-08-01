// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Core/Actors/ActorBase.h"
#include "Rendering/Models/ModelComponent.h"

/// <summary>
///     VoxelChunk class.
/// </summary>
class VoxelChunk final
{
    DELETE_COPY_MOVE(VoxelChunk);

private:
    VoxelObjectBase* m_voxelObject = nullptr;

    ModelComponent* m_model = nullptr;
    Transform m_transform = {};

public:
    VoxelChunk() = default;
    explicit VoxelChunk(VoxelObjectBase* voxelObject) : m_voxelObject(voxelObject) {}
    ~VoxelChunk();

public:

public:
    void Generate(IVoxelMesher* mesher);
    //void Rebuild(IVoxelMesher* mesher);

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