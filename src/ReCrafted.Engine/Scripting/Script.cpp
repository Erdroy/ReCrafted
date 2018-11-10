// ReCrafted (c) 2016-2018 Always Too Late

#include "Script.h"
#include "Method.h"

void Script::Awake()
{
    m_awake = FindMethod("ReCrafted.API.Common.Script::Awake");
    m_start = FindMethod("ReCrafted.API.Common.Script::Start");
    m_update = FindMethod("ReCrafted.API.Common.Script::Update");
    m_lateUpdate = FindMethod("ReCrafted.API.Common.Script::LateUpdate");
    m_simulate = FindMethod("ReCrafted.API.Common.Script::Simulate");

    m_onDestroy = FindMethod("ReCrafted.API.Common.Script::OnDestroy");

    m_onEnable = FindMethod("ReCrafted.API.Common.Script::OnEnable");
    m_onDisable = FindMethod("ReCrafted.API.Common.Script::OnDisable");

    if (m_awake)
        m_awake->Invoke();
}

void Script::Start() const
{
    if (m_start)
        m_start->Invoke();
}

void Script::Update() const
{
    if (m_update)
        m_update->Invoke();
}

void Script::LateUpdate() const
{
    if (m_lateUpdate)
        m_lateUpdate->Invoke();
}

void Script::Simulate() const
{
    if (m_simulate)
        m_simulate->Invoke();
}

void Script::OnDestroy()
{
    if (m_onDestroy)
        m_onDestroy->Invoke();
}
