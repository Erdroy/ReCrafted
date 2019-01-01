// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef CAMERAACTOR_H
#define CAMERAACTOR_H

#include "ReCrafted.h"
#include "Common/Actors/Actor.h"
#include "Graphics/Camera.h"

class CameraActor final : public Actor<CameraActor>
{
private:
    SCRIPTING_API_IMPL();

private:
    Camera* m_camera = nullptr;

protected:
    void OnAwake() override;
    void OnDestroy() override;

    void OnLateUpdate() override;

    void OnEnable() override;
    void OnDisable() override;

public:
    Camera* GetCamera() const
    {
        return m_camera;
    }
};

#endif // CAMERAACTOR_H
