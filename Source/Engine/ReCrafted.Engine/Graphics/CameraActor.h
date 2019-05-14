// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Core/Actors/ActorBase.h"

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
    CameraActor() = default;
    ~CameraActor();

protected:
    void OnAwake() override;

    void OnLateUpdate() override;

    void OnEnable() override;
    void OnDisable() override;

public:
    Camera* GetCamera() const
    {
        return m_camera;
    }
};
