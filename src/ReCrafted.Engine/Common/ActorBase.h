// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef ACTORBASE_H
#define ACTORBASE_H

// includes
#include "ReCrafted.h"
#include "Common/Transform.h"
#include "Core/Containers/Array.h"
#include "Scripting/ScriptingAPI.h"

#include <atomic>

using ActorId_t = uint64_t;

class ActorBase : public Object
{
    friend class ActorPoolBase;

protected:
    SCRIPTING_API_IMPL()

private:
    ActorId_t m_id = 0u;
    bool m_static = false;
    ActorBase* m_root = nullptr;
    ActorBase* m_parent = nullptr;
    Transform m_transform = Transform::Identity;

    Text m_name;

    Array<ActorBase*> m_children;
    Array<Script*> m_scripts;

private:
    /* ActorPool events */
    void OnAcquire();
    void OnRelease();

public:
    ActorBase() = default;

protected:
    virtual void OnAwake() {}
    virtual void OnStart() {}
    virtual void OnUpdate() {}
    virtual void OnSimulate() {}

    virtual void OnEnable() {}
    virtual void OnDisable() {}

public:
    void SetParent(ActorBase* actor);
    void AddChild(ActorBase* actor);
    void RemoveChild(ActorBase* actor);

    void AddScript(Script* script);
    void RemoveScript(Script* script);

    void SetActive(bool active);
    bool IsActive() const;

    virtual void Destroy() = 0;

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
