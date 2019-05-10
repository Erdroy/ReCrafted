// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Script.h"

void Script::Initialize()
{
    const auto cls = GetClass();

    m_awake = cls.GetMethod("ReCrafted.API.Script::Awake");
    m_start = cls.GetMethod("ReCrafted.API.Script::Start");
    m_update = cls.GetMethod("ReCrafted.API.Script::Update");
    m_lateUpdate = cls.GetMethod("ReCrafted.API.Script::LateUpdate");
    m_fixedUpdate = cls.GetMethod("ReCrafted.API.Script::FixedUpdate");

    m_onDestroy = cls.GetMethod("ReCrafted.API.Script::OnDestroy");

    m_onEnable = cls.GetMethod("ReCrafted.API.Script::OnEnable");
    m_onDisable = cls.GetMethod("ReCrafted.API.Script::OnDisable");
}

void Script::Awake()
{
    if (m_awake.IsValid())
        m_awake.Invoke(this, nullptr);
}

void Script::Start()
{
    if (m_start.IsValid())
        m_start.Invoke(this, nullptr);
}

void Script::Update()
{
    if (m_update.IsValid())
        m_update.Invoke(this, nullptr);
}

void Script::LateUpdate()
{
    if (m_lateUpdate.IsValid())
        m_lateUpdate.Invoke(this, nullptr);
}

void Script::FixedUpdate()
{
    if (m_fixedUpdate.IsValid())
        m_fixedUpdate.Invoke(this, nullptr);
}

void Script::OnEnable()
{
    if (m_onEnable.IsValid())
        m_onEnable.Invoke(this, nullptr);
}

void Script::OnDisable()
{
    if (m_onDisable.IsValid())
        m_onDisable.Invoke(this, nullptr);
}

void Script::OnDestroy()
{
    if (m_onDestroy.IsValid())
        m_onDestroy.Invoke(this, nullptr);
}

bool Script::Enabled() const
{
    return m_enabled;
}

void Script::Enabled(const bool enabled)
{
    if(m_enabled != enabled)
    {
        m_enabled = enabled;

        if (enabled)
            OnEnable();
        else
            OnDisable();
    }
}

void Script::Actor(ActorBase* actor)
{
    ASSERT(m_actor == nullptr);
    m_actor = actor;
}

ActorBase* Script::Actor() const
{
    return m_actor;
}
