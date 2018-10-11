// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef SYSTEM_H
#define SYSTEM_H

#include "EntityManager.h"
#include "Filter.h"
#include "Scripting/Script.h"
#include "Scripting/Object.h"

class World;

class System : public Object
{
    friend class EntityManager;
    friend class World;

private:
    SCRIPTING_API_IMPL()

private:
    World* m_world = nullptr;
    Filter m_filter = {};
    std::vector<Entity> m_activeEntities = {};

public:
    System() = default;
    virtual ~System() = default;

public:
    template<typename TRequire>
    void RequireComponent()
    {
        static_assert(std::is_base_of<Component, TRequire>(), "TRequire must inherit from Component!");
        m_filter.Require(GetComponentId<TRequire>());
    }

    template<typename TExclude>
    void ExcludeComponent()
    {
        static_assert(std::is_base_of<Component, TExclude>(), "TExclude must inherit from Component!");
        m_filter.Exclude(GetComponentId<TExclude>());
    }

public:
    virtual void Initialize() {}
    virtual void Shutdown() {}

    virtual void Update() {}

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


#endif // SYSTEM_H
