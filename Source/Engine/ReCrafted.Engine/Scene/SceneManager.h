// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Core/SubSystems/SubSystem.h"
#include "Common/List.h"

/// <summary>
///     SceneManager class. Provides scene management functionality.
/// </summary>
API_CLASS(public, static, partial, noinherit)
class SceneManager final : public SubSystem<SceneManager>
{
    DELETE_OPERATOR_COPY_MOVE(SceneManager)
    DELETE_CTOR_COPY(SceneManager)
    API_CLASS_BODY()

private:
    List<ActorBase*> m_actors = {};

public:
    SceneManager() = default;
    ~SceneManager() = default;

protected:
    void Initialize() override;
    void Shutdown() override;

    void OnUpdate() override;
    void OnLateUpdate() override;
    void OnFixedUpdate() override;

public:
    void AddActor(ActorBase* actor);
    void RemoveActor(ActorBase* actor);
};
