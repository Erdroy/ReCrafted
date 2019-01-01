// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef SYSTEM_H
#define SYSTEM_H

#include "EntityManager.h"
#include "Filter.h"
#include "Scripting/Method.h"
#include "Scripting/Object.h"

class World;

class ComponentSystem : public Object
{
    friend class EntityManager;
    friend class World;

private:
    SCRIPTING_API_IMPL()

private:
    World* m_world = nullptr;
    Filter m_filter = {};
    std::vector<Entity> m_activeEntities = {};

private:
    RefPtr<Method> m_init_method = nullptr;
    RefPtr<Method> m_shutdown_method = nullptr;
    RefPtr<Method> m_update_method = nullptr;

public:
    ComponentSystem() = default;
    virtual ~ComponentSystem() = default;

public:
    template<typename TRequire>
    void RequireComponent()
    {
        static_assert(std::is_base_of<Component, TRequire>(), "TRequire must inherit from Component!");
        RequireComponent(GetComponentId<TRequire>());
    }

    void RequireComponent(const TypeId componentTypeId)
    {
        m_filter.Require(componentTypeId);
    }

    template<typename TExclude>
    void ExcludeComponent()
    {
        static_assert(std::is_base_of<Component, TExclude>(), "TExclude must inherit from Component!");
        ExcludeComponent(GetComponentId<TExclude>());
    }

    void ExcludeComponent(const TypeId componentTypeId)
    {
        m_filter.Require(componentTypeId);
    }

public:
    virtual void Initialize()
    {
        if(GetManagedPtr() != nullptr)
        {
            m_init_method = FindMethod("ReCrafted.API.Common.System::Initialize");
            m_shutdown_method = FindMethod("ReCrafted.API.Common.System::Simulate");
            m_update_method = FindMethod("ReCrafted.API.Common.System::Update");
        }
        
        if(m_init_method)
            m_init_method->Invoke();

    }
    virtual void Shutdown()
    {
        if (m_shutdown_method)
            m_shutdown_method->Invoke();
    }

    virtual void Update()
    {
        if (m_update_method)
            m_update_method->Invoke();
    }

    virtual void OnEntityAdded(const Entity& entity) {}
    virtual void OnEntityRemoved(const Entity& entity) {}

public:
    Filter& GetFilter()
    {
        return m_filter;
    }

    const std::vector<Entity>& GetEntities() const
    {
        return m_activeEntities;
    }

    World& GetWorld() const
    {
        return *m_world;
    }
};

template<typename T>
TypeId GetComponentSystemId()
{
    static_assert(std::is_base_of<ComponentSystem, T>(), "T must inherit from ComponentSystem!");
    return ClassTypeId<ComponentSystem>::GetTypeId<T>();
}

#endif // SYSTEM_H
