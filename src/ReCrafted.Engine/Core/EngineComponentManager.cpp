// ReCrafted (c) 2016-2018 Always Too Late

#include "EngineComponentManager.h"

EngineComponentManager* Singleton<EngineComponentManager>::m_instance;

void EngineComponentManager::onLoad()
{
    ScopeLock(m_componentsLock);

    // call onLoad in all components
    for (var && component : m_components)
        component->onLoad();
}

void EngineComponentManager::onDispose()
{
    ScopeLock(m_componentsLock);

    // release all components in reverse order
    m_components.reverse();

    for (var && component : m_components)
    {
        // release this component
        releaseComponent(component);
    }

    // clear list
    m_components.clear();
}

void EngineComponentManager::releaseComponent(EngineComponent* component)
{
    assert(component);

    // dispose and delete component
    SafeDispose(component);
    SafeDelete(component);
}

void EngineComponentManager::registerComponent(EngineComponent* component)
{
    assert(component);

    ScopeLock(m_componentsLock);

    // initialize component
    component->init();

    // add component to the component list
    m_components.add(component);
}

void EngineComponentManager::unregisterComponent(EngineComponent* component)
{
    assert(component);

    ScopeLock(m_componentsLock);

    // release component
    releaseComponent(component);

    // remove component from the list
    m_components.remove(component);
}
