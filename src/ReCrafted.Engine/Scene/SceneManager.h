// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

// includes
#include "ReCrafted.h"
#include "Common/GameObject.h"
#include "Core/EngineComponent.h"

class SceneManager : public EngineComponent<SceneManager>
{
    friend class EngineMain;

private:
SCRIPTING_API_IMPL()

private:
    Array<GameObject*> m_gameObjects;

public:
    virtual ~SceneManager() = default;

private:
    void OnInit() override;
    void OnDispose() override;

private:
    void Update() override;
};

#endif // SCENEMANAGER_H
