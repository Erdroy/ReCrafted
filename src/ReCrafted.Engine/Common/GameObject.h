// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "ReCrafted.h"
#include "Common/Entities/Entity.h"
#include "Core/Containers/Array.h"
#include "Scripting/Object.h"
#include "Scripting/Script.h"

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

    TransformComponent* m_transform = nullptr;

    Array<GameObject*> m_children = {};
    Array<Component*> m_components = {};
    Array<Script*> m_scripts = {};
    Entity m_entity = {};

    bool m_active = true;

    bool m_firstFrame = true;

private:
    void SetupEntity();
    void Cleanup();
    void OnAcquire();
    void OnDestroy() override;
    void OnParentChangeActive(bool active);

private:
    void Start();
    void Update();
    void LateUpdate();
    void Simulate();

private:
    GameObject();
    ~GameObject();

public:
    /**
     * \brief Sets given game object as children of this game object.
     * \param gameObject The game object that will be children.
     * \param resetPosition Should game object reset it's own local position to [0,0,0]?
     * \param resetRotation Should game object reset it's own local rotation to rotation of this game object?
     */
    void AddChildren(GameObject* gameObject, bool resetPosition = false, bool resetRotation = false);

    /**
     * \brief Removes given game object from being a children of this game object.
     * \param gameObject The game object that will be alone, now.
     */
    void RemoveChildren(GameObject* gameObject);

    /**
     * \brief Adds given script to this game object.
     * \param script The script.
     */
    void AddScript(Script* script);
    
    /**
     * \brief Removes given script.
     * \param script The script.
     */
    void RemoveScript(Script* script);

    /**
     * \brief Sets active state of this game object.
     * \param active The new active state.
     */
    void SetActive(bool active);

    /**
     * \brief Gets game object active state.
     * \return True when active.
     */
    bool IsActive() const;

    /**
     * \brief Returns array containing all children game objects.
     * \return The array of children game objects.
     */
    const Array<GameObject*>& Children() const
    {
        return m_children;
    }

public:
    /**
     * \brief Gets pointer to transform (ECS component) of this game object.
     * \return The transform pointer.
     */
    TransformComponent* GetTransform() const;

    /**
     * \brief Sets the position of this game object and updates all children
     *  to match it's relative local position.
     * \param position The new position of this game object.
     */
    void SetPosition(const Vector3& position);

    /**
     * \brief Gets position of this game object.
     */
    const Vector3& GetPosition() const;

    /**
     * \brief Sets rotation of this game objectand updates all children
     *  to match it's relative local rotation.
     * \param rotation The new rotation of this game object.
     */
    void SetRotation(const Vector3& rotation);

    /**
     * \brief Gets rotation of this game object.
     */
    const Vector3& GetRotation() const;

public:
    /**
     * \brief Creates new game object.
     * \return The newly created game object, just for you.
     */
    static GameObject* Create();
};

#endif // GAMEOBJECT_H
