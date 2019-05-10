// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

// includes
#include <ReCrafted.h>
#include "Core/Transform.h"
#include "Common/List.h"
#include "Scripting/Object.h"

using ActorId_t = uint64_t;

API_USING("ReCrafted.API.Common")
API_USING("ReCrafted.API.Mathematics")

/// <summary>
///     Base class for all actors. Provides basic actor behavior.
/// </summary>
/// <remarks>
///     Actors can be defined only in C++ source code.
/// </remarks>
API_CLASS(public, abstract, partial)
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
    List<Script*> m_scripts = {};

private:
    void Start();
    void Update();
    void LateUpdate();
    void FixedUpdate();

public:
    ActorBase();
    virtual ~ActorBase();

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
    /// <summary>
    ///     Sets given actor as parent of this actor.
    /// </summary>
    /// <param name="newParent">The parent instance.</param>
    API_FUNCTION()
    void SetParent(ActorBase* newParent);

    /// <summary>
    ///     Adds given child to the children of this actor.
    /// </summary>
    /// <param name="child">The children instance.</param>
    API_FUNCTION()
    void AddChild(ActorBase* child);

    /// <summary>
    ///     Removes given child from being child of this actor.
    /// </summary>
    /// <param name="child">The children instance.</param>
    API_FUNCTION()
    void RemoveChild(ActorBase* child);

    /// <summary>
    ///     Adds given script to this actor.
    /// </summary>
    API_FUNCTION()
    void AddScript(Script* script);

    /// <summary>
    ///     Removes given script to this actor.
    /// </summary>
    API_FUNCTION()
    void RemoveScript(Script* script);

    /// <summary>
    ///     Sets active state.
    /// </summary>
    /// <param name="active">The active state.</param>
    API_FUNCTION()
    void SetActive(bool active);

public:
    /// <summary>
    ///     Gets the parent of this actor.
    /// </summary>
    API_PROPERTY(noprefix)
    ActorBase* GetParent() const;

    /// <summary>
    ///     Gets or sets world-space position of this actor.
    /// </summary>
    API_PROPERTY()
    virtual void Position(const Vector3& position);

    /// <summary>
    ///     Gets or sets world-space position of this actor.
    /// </summary>
    API_PROPERTY()
    virtual const Vector3& Position() const;

    /// <summary>
    ///     Gets or sets local-space position of this actor.
    ///     It is relative to the parent actor, if there is none,
    ///     this returns the same value as Position.
    /// </summary>
    API_PROPERTY()
    virtual void LocalPosition(const Vector3& position);

    /// <summary>
    ///     Gets or sets local-space position of this actor.
    ///     It is relative to the parent actor, if there is none,
    ///     this returns the same value as Position.
    /// </summary>
    API_PROPERTY()
    virtual const Vector3& LocalPosition() const;

    /// <summary>
    ///     Gets or sets world-space rotation of this actor.
    /// </summary>
    API_PROPERTY()
    virtual void Rotation(const Quaternion& rotation);

    /// <summary>
    ///     Gets or sets world-space rotation of this actor.
    /// </summary>
    API_PROPERTY()
    virtual const Quaternion& Rotation() const;

    /// <summary>
    ///     Gets or sets local-space rotation of this actor.
    ///     It is relative to the parent actor, if there is none,
    ///     this returns the same value as Rotation.
    /// </summary>
    API_PROPERTY()
    virtual void LocalRotation(const Quaternion& rotation);
    
    /// <summary>
    ///     Gets or sets local-space rotation of this actor.
    ///     It is relative to the parent actor, if there is none,
    ///     this returns the same value as Rotation.
    /// </summary>
    API_PROPERTY()
    virtual const Quaternion& LocalRotation() const;

    /// <summary>
    ///     Gets or sets world-space scale of this actor.
    /// </summary>
    API_PROPERTY()
    virtual void Scale(const Vector3& scale);

    /// <summary>
    ///     Gets or sets world-space scale of this actor.
    /// </summary>
    API_PROPERTY()
    virtual const Vector3& Scale() const;

    /// <summary>
    ///     Gets or sets local-space scale of this actor.
    ///     It is relative to the parent actor, if there is none,
    ///     this returns the same value as Scale.
    /// </summary>
    API_PROPERTY()
    virtual void LocalScale(const Vector3& scale);

    /// <summary>
    ///     Gets or sets local-space scale of this actor.
    ///     It is relative to the parent actor, if there is none,
    ///     this returns the same value as Scale.
    /// </summary>
    API_PROPERTY()
    virtual const Vector3& LocalScale() const;

    /// <summary>
    ///     Gets or sets the transform of this actor.
    /// </summary>
    API_PROPERTY(noprefix)
    virtual void SetTransform(const Transform& transform);

    /// <summary>
    ///     Gets or sets the transform of this actor.
    /// </summary>
    API_PROPERTY(noprefix)
    virtual const Transform& GetTransform() const;

public:
    /// <summary>
    ///     Returns true when this actor's enable state is true.
    /// </summary>
    API_PROPERTY()
    bool IsActiveSelf() const;

    /// <summary>
    ///     Returns true when this actor's enable state is true and 
    ///     false when parent's IsActive is also false.
    /// </summary>
    API_PROPERTY()
    bool IsActive() const;

    /// <summary>
    ///     Gets or sets the name of this actor.
    /// </summary>
    API_PROPERTY(noprefix)
    void SetName(const String& name);

    /// <summary>
    ///     Gets or sets the name of this actor.
    /// </summary>
    API_PROPERTY(noprefix)
    const String& GetName() const;

    /// <summary>
    ///     Gets all children instances.
    /// </summary>
    const List<ActorBase*>& GetChildren() const;

    /// <summary>
    ///     Gets all script instances contained by this actor.
    /// </summary>
    const List<Script*>& GetScripts() const;

    ActorId_t GetId() const;
};
