// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef ACTORBASE_H
#define ACTORBASE_H

// includes
#include "ReCrafted.h"
#include "Common/Transform.h"
#include "Core/Containers/Array.h"
#include "Scripting/ScriptingAPI.h"

using ActorId_t = uint64_t;

class ActorBase : public Object
{
    friend class ActorPoolBase;
    friend class SceneManager;

protected:
    SCRIPTING_API_IMPL()

private:
    ActorId_t m_id = 0u;
    bool m_static = false;
    bool m_active = true;
    bool m_firstFrame = true;
    ActorBase* m_parent = nullptr;

    Transform m_transform = Transform::Identity;
    Transform m_localTransform = Transform::Identity;

    Text m_name;

    Array<ActorBase*> m_children = {};
    Array<Script*> m_scripts = {};

private: /* ActorPool events */
    void OnAcquire();
    void OnRelease();

private:
    void Cleanup(ActorId_t id = 0u);

private:
    void Start();
    void Update();
    void LateUpdate();
    void Simulate();

public:
    ActorBase() = default;

protected:
    void UpdateTransform();

protected:
    virtual void OnAwake() {}
    virtual void OnStart() {}
    virtual void OnUpdate() {}
    virtual void OnLateUpdate() {}
    virtual void OnSimulate() {}

    virtual void OnEnable() {}
    virtual void OnDisable() {}

protected:
    virtual void OnAddedChild(ActorBase* child) {}
    virtual void OnRemovedChild(ActorBase* child) {}
    virtual void OnParentChange(ActorBase* parent) {}
    virtual void OnParentActiveChange(bool parentActive) {}

public:
    void SetParent(ActorBase* newParent);
    void AddChild(ActorBase* child);
    void RemoveChild(ActorBase* child);

    void AddScript(Script* script);
    void RemoveScript(Script* script);

    void SetActive(bool active);

public:
    void SetPosition(const Vector3& position);
    void SetLocalPosition(const Vector3& position);

    void SetRotation(const Quaternion& rotation);
    void SetLocalRotation(const Quaternion& rotation);

    void SetScale(const Vector3& scale);
    void SetLocalScale(const Vector3& scale);

    Vector3 GetPosition() const
    {
        return m_transform.translation;
    }

    Vector3 GetLocalPosition() const
    {
        return m_localTransform.translation;
    }

    Quaternion GetRotation() const
    {
        return m_transform.orientation;
    }

    Quaternion GetLocalRotation() const
    {
        return m_localTransform.orientation;
    }

    Vector3 GetScale() const
    {
        return m_transform.scale;
    }

    Vector3 GetLocalScale() const
    {
        return m_localTransform.scale;
    }

    Transform* GetTransform()
    {
        return &m_transform;
    }

    void SetTransform(const Transform& transform);

public:
    virtual void Destroy() = 0;

public:
    bool IsActiveSelf() const
    {
        return m_active;
    }

    bool IsActive() const
    {
        if (m_parent)
            return m_active && m_parent->IsActive();

        return m_active;
    }

    const Array<ActorBase*>& GetChildren() const
    {
        return m_children;
    }

    void SetName(const Text& name)
    {
        m_name = name;
    }

    const Text& GetName() const
    {
        return m_name;
    }

    ActorId_t GetId() const
    {
        return m_id;
    }
};

#endif // ACTORBASE_H
