// ReCrafted (c) 2016-2019 Always Too Late

#include "EngineComponentManager.h"
#include "Singleton.h"

SINGLETON_IMPL(EngineComponentManager)

void EngineComponentManager::OnLoad()
{
    ScopeLock(m_componentsLock);

    // call OnLoad in all components
    for (var&& component : m_components)
        component->OnLoad();
}

void EngineComponentManager::OnDispose()
{
    ScopeLock(m_componentsLock);

    // release all components in reverse order
    m_components.Reverse();

    for (var&& component : m_components)
    {
        // release this component
        ReleaseComponent(component);
        component = nullptr;
    }

    // clear list
    m_components.Clear();
}

void EngineComponentManager::Update()
{
    ScopeLock(m_componentsLock);

    // call Update in all components
    for (rvar component : m_components)
        component->Update();
}

void EngineComponentManager::LateUpdate()
{
    ScopeLock(m_componentsLock);

    // call LateUpdate in all components
    for (rvar component : m_components)
        component->LateUpdate();
}

void EngineComponentManager::FrameFinished()
{
    ScopeLock(m_componentsLock);

    // call OnFrameFinished in all components
    for (rvar component : m_components)
        component->OnFrameFinished();
}

void EngineComponentManager::ReleaseComponent(EngineComponentBase* component)
{
    ASSERT(component);

    // Shutdown and delete component
    component->Shutdown();
}

void EngineComponentManager::RegisterComponent(EngineComponentBase* component)
{
    ASSERT(component);

    ScopeLock(m_componentsLock);

    // initialize component
    component->Init();

    // add component to the component list
    m_components.Add(component);
}

void EngineComponentManager::UnregisterComponent(EngineComponentBase* component)
{
    ASSERT(component);

    ScopeLock(m_componentsLock);

    // release component
    ReleaseComponent(component);

    // Remove component from the list
    m_components.Remove(component);
}
