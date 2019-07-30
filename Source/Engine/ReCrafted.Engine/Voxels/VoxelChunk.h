// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Core/Actors/ActorBase.h"
#include "Rendering/Models/ModelComponent.h"

API_USING("ReCrafted.API.Core.Actors")

/// <summary>
///     VoxelChunk actor class.
/// </summary>
API_CLASS(public, sealed)
class VoxelChunk final : public ActorBase
{
    API_CLASS_BODY()

private:
    ModelComponent* m_model = nullptr;
    
public:
    ACTOR_BODY(VoxelChunk)
    ~VoxelChunk();

public:

public:
    /// <summary>
    ///     Gets or sets the chunk's visibility state. Invisible when true.
    /// </summary>
    API_PROPERTY(noprefix)
    void SetVisible(bool isVisible);

    /// <summary>
    ///     Gets or sets the chunk's visibility state. Invisible when true.
    /// </summary>
    API_PROPERTY(noprefix)
    bool GetVisible() const
    {
        return m_model && m_model->Active;
    }
};