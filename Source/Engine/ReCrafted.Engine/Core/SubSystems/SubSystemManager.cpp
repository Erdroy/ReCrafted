// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "SubSystemManager.h"
#include "Profiler/Profiler.h"

SubSystemManager::SubSystemManager()
{
}

SubSystemManager::~SubSystemManager()
{
}

void SubSystemManager::Shutdown()
{
    ScopeLock(m_lockSubSystems);

    // Reverse subsystems list
    m_subSystems.Reverse();

    // Release all subsystems
    for (auto&& subsystem : m_subSystems)
    {
        if (subsystem == nullptr)
            continue;

        Release(subsystem);
    }

    // Clear and release memory
    m_subSystems.Clear();
    m_subSystems.Release();
}

void SubSystemManager::Release(SubSystemBase* subsystem)
{
    ASSERT(subsystem);

    // Shutdown and Release subsystem
    // NOTE: Release function calls Singleton<T>::Dispose function
    subsystem->Shutdown();
    subsystem->Release();
}

void SubSystemManager::Update()
{
    CPU_PROFILE_SCOPE(0, __FUNCTION__);

    for (auto&& subsystem : m_subSystems)
    {
        if (subsystem == nullptr)
            continue;

        subsystem->OnUpdate();
    }
}

void SubSystemManager::FixedUpdate()
{
    CPU_PROFILE_SCOPE(0, __FUNCTION__);

    for (auto&& subsystem : m_subSystems)
    {
        if (subsystem == nullptr)
            continue;

        subsystem->OnFixedUpdate();
    }
}

void SubSystemManager::LateUpdate()
{
    CPU_PROFILE_SCOPE(0, __FUNCTION__);

    for (auto&& subsystem : m_subSystems)
    {
        if (subsystem == nullptr)
            continue;

        subsystem->OnLateUpdate();
    }
}

void SubSystemManager::FrameDone()
{
    CPU_PROFILE_SCOPE(0, __FUNCTION__);

    for (auto&& subsystem : m_subSystems)
    {
        if (subsystem == nullptr)
            continue;

        subsystem->OnFrameDone();
    }
}

void SubSystemManager::Register(SubSystemBase* subsystem)
{
    ASSERT(subsystem);

    // initialize component
    subsystem->Initialize();

    // add subsystem to the component list
    ScopeLock(m_lockSubSystems);
    m_subSystems.Add(subsystem);
}

void SubSystemManager::Unregister(SubSystemBase* subsystem)
{
    ASSERT(subsystem);

    // release component
    Release(subsystem);

    // Remove subsystem from the list
    ScopeLock(m_lockSubSystems);
    m_subSystems.Remove(subsystem);
}
