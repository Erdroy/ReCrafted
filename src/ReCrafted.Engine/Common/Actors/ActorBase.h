// ReCrafted (c) 2016-2019 Always Too Late

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

    virtual void OnAddedChild(ActorBase* child) {}
    virtual void OnRemovedChild(ActorBase* child) {}
    virtual void OnParentChange(ActorBase* parent) {}
    virtual void OnParentActiveChange(bool parentActive) {}
    virtual void OnTransformChange() {}

public:
    void SetParent(ActorBase* newParent);
    void AddChild(ActorBase* child);
    void RemoveChild(ActorBase* child);

    void AddScript(Script* script);
    void RemoveScript(Script* script);

    void SetActive(bool active);

public:
    virtual void Destroy() = 0;

public:
    virtual void SetPosition(const Vector3& position);
    virtual void SetLocalPosition(const Vector3& position);

    virtual void SetRotation(const Quaternion& rotation);
    virtual void SetLocalRotation(const Quaternion& rotation);

    virtual void SetScale(const Vector3& scale);
    virtual void SetLocalScale(const Vector3& scale);

    void SetTransform(const Transform& transform);

    Vector3 GetPosition() const;
    Vector3 GetLocalPosition() const;

    Quaternion GetRotation() const;
    Quaternion GetLocalRotation() const;

    Vector3 GetScale() const;
    Vector3 GetLocalScale() const;

    Transform& GetTransform();

public:
    bool IsActiveSelf() const;
    bool IsActive() const;

    const Array<ActorBase*>& GetChildren() const;

    void SetName(const Text& name);
    const Text& GetName() const;

    ActorId_t GetId() const;
};

#endif // ACTORBASE_H
