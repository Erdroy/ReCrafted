// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "ReCrafted.h"
#include "Common/Entities/Entity.h"
#include "Core/Containers/Array.h"
#include "Scripting/Object.h"

#include "TransformComponent.h"

class GameObject : public Object
{
private:
    friend class Object;
    friend class GameObjectPool;
    friend class SceneManager;

private:
    SCRIPTING_API_IMPL()

private:
    GameObject* m_root = nullptr;
    GameObject* m_parent = nullptr;

    Array<GameObject*> m_children = {};
    Array<Component*> m_components = {};
    Array<Script*> m_scripts = {};

    Entity m_entity = {};

    bool m_active = true;

private:
    void SetupEntity();
    void OnDestroy() override;

private:
    GameObject();

public:
    ~GameObject()
    {
        // Destroy entity
        m_entity.Destroy();
        m_entity = Entity::Empty();

        // Destroy this gameObject
        Destroy(this);
    }

public:
    void AddChildren(GameObject* children, bool resetPosition = false, bool resetRotation = false);
    void RemoveChildren(GameObject* children);
    void SetActive(bool active);

public:
    TransformComponent* GetTransform() const
    {
        ASSERT(m_components.Count() != 0u);
        return static_cast<TransformComponent*>(m_components[0]);
    }

    void SetPosition(const Vector3& position)
    {
        // Update relative children position to this gameObject
        for(rvar child : m_children)
            child->SetPosition(child->GetPosition() + (position - GetPosition()));

        // Set position
        SetPosition(position);
    }

    const Vector3& GetPosition() const
    {
        return {};
    }

    void SetRotation(const Vector3& rotation)
    {
    }

    const Vector3& GetRotation() const
    {
        return {};
    }

    void SetForward(const Vector3& forward)
    {
    }

    const Vector3& GetForward() const
    {
        return {};
    }

    void SetRight(const Vector3& right)
    {
    }

    const Vector3& GetRight() const
    {
        return {};
    }

    void SetUp(const Vector3& up)
    {
    }

    const Vector3& GetUp() const
    {
        return {};
    }
};

#endif // GAMEOBJECT_H
