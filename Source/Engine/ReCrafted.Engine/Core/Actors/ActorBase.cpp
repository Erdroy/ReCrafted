// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "ActorBase.h"
#include "Scene/SceneManager.h"
#include "Scripting/Script.h"

void ActorBase::Start()
{
    for (auto&& script : m_scripts)
    {
        if (script->Enabled())
            script->Start();
    }

    for (auto&& child : m_children)
    {
        if (child->IsActive())
            child->Start();
    }

    OnStart();
}

void ActorBase::Update()
{
    if (m_firstFrame)
    {
        Start();
        m_firstFrame = false;
    }

    for (auto&& script : m_scripts)
    {
        if (script->Enabled())
            script->Update();
    }

    for (auto&& child : m_children)
    {
        if (child->IsActive())
            child->Update();
    }

    OnUpdate();
}

void ActorBase::LateUpdate()
{
    for (auto&& script : m_scripts)
    {
        if (script->Enabled())
            script->LateUpdate();
    }
    
    for (auto&& child : m_children)
    {
        if (child->IsActive())
            child->LateUpdate();
    }

    OnLateUpdate();
}

void ActorBase::FixedUpdate()
{
    for (auto&& script : m_scripts)
    {
        if (script->Enabled())
            script->FixedUpdate();
    }

    for (auto&& child : m_children)
    {
        if (child->IsActive())
            child->FixedUpdate();
    }

    OnSimulate();
}

ActorBase::ActorBase()
{
    // Add to scene
    SceneManager::GetInstance()->AddActor(this);
}

ActorBase::~ActorBase()
{
    // Destroy scripts
    for (auto& script : m_scripts)
    {
        script->Enabled(false);
        script->OnDestroy();
        Destroy(script);
    }
    m_scripts.Clear();

    if (m_parent)
    {
        // Remove from parent
        SetParent(nullptr);
    }

    // Destroy all children
    for(auto&& child : m_children)
    {
        // Force child's parent object pointer to be null while destroying it.
        child->m_parent = nullptr;

        // Destroy the child actor
        DestroyNow(child);
    }

    // Remove from scene
    SceneManager::GetInstance()->RemoveActor(this);
}

void ActorBase::UpdateTransform()
{
    MAIN_THREAD_ONLY();

    // We need to construct the world transform now.
    // If we have a parent, this gets a bit complicated, because we need to transform the local-space transform to world-space.
    // This is accomplished by using parent's world-space transform and ToWorld function.
    // Otherwise just use the local-space transform as world-space.
    // ... Then broadcast UpdateTransform to all children actors.

    if(m_parent)
    {
        // Transform to world-space by using parent's transform.
        m_transform = m_parent->GetTransform().ToWorld(m_localTransform);
    }
    else
    {
        // Just use the local-space transform as world-space.
        m_transform = m_localTransform;
    }

    // Raise transform change event
    OnTransformChange();

    // Broadcast UpdateTransform
    for(auto&& child : m_children)
        child->UpdateTransform();
}

void ActorBase::AddChild(ActorBase* child)
{
    MAIN_THREAD_ONLY();
    ASSERT(child);
    ASSERT(child->m_parent == nullptr);

    // Set this actor as parent for the child
    child->SetParent(this);
}

void ActorBase::RemoveChild(ActorBase* child)
{
    MAIN_THREAD_ONLY();
    ASSERT(child);
    ASSERT(child->m_parent == this);

    // Reset parent to null pointer
    child->SetParent(nullptr);
}

void ActorBase::AddScript(Script* script)
{
    MAIN_THREAD_ONLY();
    _ASSERT_(script->Actor() == nullptr, "Cannot add script to this actor as the script already is added to another actor.");

    // Initialize script
    script->Initialize();

    // Set this actor as parent of this script
    script->Actor(this);

    // Set script as enabled
    script->Enabled(true);

    // Call Awake callback
    script->Awake();

    m_scripts.Add(script);
}

void ActorBase::RemoveScript(Script* script)
{
    MAIN_THREAD_ONLY();
    _ASSERT_(script->Actor() == this, "Cannot remove script from an actor that is not a parent of given script.");
   
    // Set script as disabled
    script->Enabled(false);

    // Call OnDestroy callback
    script->OnDestroy();

    // Remove from this actor
    m_scripts.Remove(script);

    // Destroy script
    Destroy(script);
}

void ActorBase::SetActive(const bool active)
{
    MAIN_THREAD_ONLY();

    if (m_active == active)
        return;

    // Call enable or disable event
    if (m_active)
        OnEnable();
    else
        OnDisable();

    // Set state
    m_active = active;
    
    // Call children event
    for (auto&& child : m_children)
    {
        if (child->IsActive())
            child->OnParentActiveChange(active);
    }
}

void ActorBase::TestArr(const Array<ActorBase*>& test)
{
    const auto t1 = test[1];

}

void ActorBase::SetParent(ActorBase* newParent)
{
    MAIN_THREAD_ONLY();
    ASSERT(newParent != this);

    if (m_parent)
    {
        // Remove this from current parent
        m_parent->m_children.Remove(this);

        // Call remove child event
        m_parent->OnRemovedChild(this);

        // Return to the scene
        SceneManager::GetInstance()->AddActor(this);

        // Set parent as null
        m_parent = nullptr;
    }

    if (newParent)
    {
        // Set new parent of this actor
        m_parent = newParent;

        // Add this to the new parent
        m_parent->m_children.Add(this);

        // Call added child event
        m_parent->OnAddedChild(this);

        // Remove from the scene
        SceneManager::GetInstance()->RemoveActor(this);
    }

    UpdateTransform();

    // Call parent change event
    OnParentChange(m_parent);
}

ActorBase* ActorBase::GetParent() const
{
    MAIN_THREAD_ONLY();
    return m_parent;
}

void ActorBase::Position(const Vector3& position)
{
    MAIN_THREAD_ONLY();

    if (Vector3::NearEqual(m_transform.translation, position))
        return;

    if(m_parent)
    {
        m_localTransform.translation = m_parent->GetTransform().ToLocal(position);
    }
    else
    {
        m_localTransform.translation = position;
    }

    // Update transform
    UpdateTransform();
}

const Vector3& ActorBase::Position() const
{
    MAIN_THREAD_ONLY();
    return m_transform.translation;
}

void ActorBase::LocalPosition(const Vector3& position)
{
    MAIN_THREAD_ONLY();

    if (Vector3::NearEqual(m_localTransform.translation, position))
        return;

    m_localTransform.translation = position;

    // Update transform
    UpdateTransform();
}

const Vector3& ActorBase::LocalPosition() const
{
    MAIN_THREAD_ONLY();
    return m_localTransform.translation;
}

void ActorBase::Rotation(const Quaternion& rotation)
{
    MAIN_THREAD_ONLY();

    if (Quaternion::NearEqual(m_transform.orientation, rotation))
        return;

    if(m_parent)
    {
        m_localTransform.orientation = m_parent->GetTransform().ToLocal(rotation);
    }
    else
    {
        m_localTransform.orientation = rotation;
    }

    // Update transform
    UpdateTransform();
}

const Quaternion& ActorBase::Rotation() const
{
    MAIN_THREAD_ONLY();
    return m_transform.orientation;
}

void ActorBase::LocalRotation(const Quaternion& rotation)
{
    MAIN_THREAD_ONLY();

    if (Quaternion::NearEqual(m_localTransform.orientation, rotation))
        return;

    m_localTransform.orientation = rotation;

    // Update transform
    UpdateTransform();
}

const Quaternion& ActorBase::LocalRotation() const
{
    MAIN_THREAD_ONLY();
    return m_localTransform.orientation;
}

void ActorBase::Scale(const Vector3& scale)
{
    MAIN_THREAD_ONLY();

    if (Vector3::NearEqual(m_transform.scale, scale))
        return;

    if (m_parent)
    {
        m_localTransform.scale = scale / m_parent->Scale();
    }
    else
    {
        m_localTransform.scale = scale;
    }

    // Update transform
    UpdateTransform();
}

const Vector3& ActorBase::Scale() const
{
    MAIN_THREAD_ONLY();
    return m_transform.scale;
}

void ActorBase::LocalScale(const Vector3& scale)
{
    MAIN_THREAD_ONLY();

    if (Vector3::NearEqual(m_localTransform.scale, scale))
        return;

    m_localTransform.scale = scale;

    // Update transform
    UpdateTransform();
}

const Vector3& ActorBase::LocalScale() const
{
    MAIN_THREAD_ONLY();
    return m_localTransform.scale;
}

void ActorBase::SetTransform(const Transform& transform)
{
    MAIN_THREAD_ONLY();

    if(m_parent)
    {
        m_localTransform = m_parent->GetTransform().ToLocal(transform);
    }
    else
    {
        m_localTransform = transform;
    }

    // Update transform
    UpdateTransform();
}

const Transform& ActorBase::GetTransform() const
{
    MAIN_THREAD_ONLY();
    return m_transform;
}

bool ActorBase::IsActiveSelf() const
{
    MAIN_THREAD_ONLY();
    return m_active;
}

bool ActorBase::IsActive() const
{
    MAIN_THREAD_ONLY();
    if (m_parent)
        return m_active && m_parent->IsActive();

    return m_active;
}

void ActorBase::SetName(const String& name)
{
    MAIN_THREAD_ONLY();
    m_name = name;
}

const String& ActorBase::GetName() const
{
    MAIN_THREAD_ONLY();
    return m_name;
}

Array<ActorBase*> ActorBase::GetChildren()
{
    MAIN_THREAD_ONLY();
    return { m_children.Data(), m_children.Size() };
}

Array<Script*> ActorBase::GetScripts()
{
    return { m_scripts.Data(), m_scripts.Size() };
}

ActorId_t ActorBase::GetId() const
{
    MAIN_THREAD_ONLY();
    return m_id;
}
