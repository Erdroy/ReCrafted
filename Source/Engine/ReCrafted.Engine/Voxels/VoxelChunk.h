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
private:
    ModelComponent* m_model = nullptr;
    Transform m_transform = {};

public:
    VoxelChunk() = default;
    ~VoxelChunk();

public:

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