// ReCrafted (c) 2016-2018 Always Too Late

#include "Script.h"
#include "Method.h"

Script::Script()
{
    m_awake = FindMethod("Awake");
    m_start = FindMethod("Start");
    m_update = FindMethod("Update");
    m_lateUpdate = FindMethod("LateUpdate");
    m_simulate = FindMethod("Simulate");
    m_onDestroy = FindMethod("OnDestroy");
}

void Script::Awake() const
{
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
