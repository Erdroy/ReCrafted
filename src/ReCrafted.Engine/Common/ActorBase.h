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
    ActorBase* m_root = nullptr;
    ActorBase* m_parent = nullptr;
    Transform m_transform = Transform::Identity;

    Text m_name;

    Array<ActorBase*> m_children;
    Array<Script*> m_scripts;

private: /* ActorPool events */
    void OnAcquire();
    void OnRelease();

private:
    void Start();
    void Update();
    void LateUpdate();
    void Simulate();

public:
    ActorBase() = default;

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

    Transform* GetTransform()
    {
        return &m_transform;
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
