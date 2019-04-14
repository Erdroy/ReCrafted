// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "SubSystemManager.h"

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

    // Shutdown and delete subsystem
    subsystem->Shutdown();
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
