// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Core/Actors/ActorBase.h"
#include "Rendering/Camera.h"

API_USING("ReCrafted.API.Core.Actors")

/// <summary>
///     CameraActor actor class.
/// </summary>
API_CLASS(public, sealed)
class CameraActor final : public ActorBase
{
    API_CLASS_BODY()

private:
    Camera* m_camera = nullptr;

public:
    ACTOR_BODY(CameraActor)
    ~CameraActor();

protected:
    void OnAwake() override;

    void OnLateUpdate() override;

    void OnEnable() override;
    void OnDisable() override;

public:
    /// <summary>
    ///     Gets the camera contained by this actor. See also: <see cref="ReCrafted.API.Rendering.Camera"/>.
    /// </summary>
    API_PROPERTY(noprefix)
    Camera* GetCamera() const
    {
        return m_camera;
    }
};
