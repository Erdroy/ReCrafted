// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef ACTOR_H
#define ACTOR_H

#include "ReCrafted.h"
#include "Common/Entities/Entity.h"
#include "Core/Containers/Array.h"
#include "Scripting/Object.h"

#include "TransformComponent.h"

class Actor : public Object
{
private:
    friend class ActorPool;
    friend class SceneManager;

private:
    SCRIPTING_API_IMPL()

private:
    Actor* m_root = nullptr;
    Actor* m_parent = nullptr;
    Array<Actor*> m_children = {};

    Array<Component*> m_components = {};
    Array<Script*> m_scripts = {};

    bool m_active = true;

private:
    void SetEntity(const Entity& entity);
    void OnDestroy() override;

public:
    ~Actor()
    {
        Destroy(this);
    }

public:
    void AddChildren(Actor* children, bool resetPosition = false, bool resetRotation = false);
    void RemoveChildren(Actor* children);
    void SetActive(bool active);

public:
    TransformComponent* GetTransform() const
    {
        return nullptr;
    }

    void SetPosition(const Vector3& position)
    {
    }

    const Vector3& GetPosition() const
    {
    }

    void SetRotation(const Vector3& rotation)
    {
    }

    const Vector3& GetRotation() const
    {
    }

    void SetForward(const Vector3& forward)
    {
    }

    const Vector3& GetForward() const
    {
    }

    void SetRight(const Vector3& right)
    {
    }

    const Vector3& GetRight() const
    {
    }

    void SetUp(const Vector3& up)
    {
    }

    const Vector3& GetUp() const
    {
    }
};

#endif // ACTOR_H
