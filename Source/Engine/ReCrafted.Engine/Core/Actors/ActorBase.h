// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

// includes
#include <ReCrafted.h>
#include "Core/Transform.h"
#include "Common/List.h"
#include "Scripting/Object.h"

using ActorId_t = uint64_t;

API_CLASS(public, abstract)
class ActorBase : public Object
{
    API_CLASS_BODY()
    friend class SceneManager;

private:
    ActorId_t m_id = 0u;
    bool m_static = false;
    bool m_active = true;
    bool m_firstFrame = true;
    ActorBase* m_parent = nullptr;

    Transform m_transform = Transform::Identity;
    Transform m_localTransform = Transform::Identity;

    String m_name;

    List<ActorBase*> m_children = {};
    //List<Script*> m_scripts = {};

private:
    void Cleanup(ActorId_t id = 0u);

private:
    void Start();
    void Update();
    void LateUpdate();
    void Simulate();

public:
    ActorBase() = default;
    virtual ~ActorBase() = default;

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
    API_FUNCTION();
    void SetParent(ActorBase* newParent);

    API_FUNCTION();
    void AddChild(ActorBase* child);

    API_FUNCTION();
    void RemoveChild(ActorBase* child);

    //API_FUNCTION()
    //void AddScript(Script* script);
    //
    //API_FUNCTION()
    //void RemoveScript(Script* script);

    API_FUNCTION();
    void SetActive(bool active);

public:
    virtual void SetPosition(const Vector3& position);
    virtual void SetLocalPosition(const Vector3& position);

    virtual void SetRotation(const Quaternion& rotation);
    virtual void SetLocalRotation(const Quaternion& rotation);

    virtual void SetScale(const Vector3& scale);
    virtual void SetLocalScale(const Vector3& scale);

    void SetTransform(const Transform& transform);

    const Vector3& GetPosition() const;
    const Vector3& GetLocalPosition() const;

    const Quaternion& GetRotation() const;
    const Quaternion& GetLocalRotation() const;

    const Vector3& GetScale() const;
    const Vector3& GetLocalScale() const;

    Transform& GetTransform();

public:
    bool IsActiveSelf() const;
    bool IsActive() const;

    const List<ActorBase*>& GetChildren() const;

    API_FUNCTION();
    void SetName(const String& name);

    API_FUNCTION();
    const String& GetName() const;

    ActorId_t GetId() const;
};
