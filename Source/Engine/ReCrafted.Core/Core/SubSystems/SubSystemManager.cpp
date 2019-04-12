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
}

void SubSystemManager::Release(SubSystemBase* subsystem)
{
    //ASSERT(subsystem);

    // Shutdown and delete subsystem
    subsystem->Shutdown();
}

void SubSystemManager::Register(SubSystemBase* subsystem)
{
    //ASSERT(component);

    // initialize component
    subsystem->Initialize();

    // add subsystem to the component list
    ScopeLock(m_lockSubSystems);
    m_subSystems.Add(subsystem);
}

void SubSystemManager::Unregister(SubSystemBase* subsystem)
{
    //ASSERT(component);

    // release component
    Release(subsystem);

    // Remove subsystem from the list
    ScopeLock(m_lockSubSystems);
    m_subSystems.Remove(subsystem);
}
