// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef ENTITY_H
#define ENTITY_H

#include "Component.h"
#include "World.h"
#include "EntityId.h"

#include <type_traits>

struct Entity
{
private:
    EntityId m_id = 0u;
    World* m_world;

private:
    SCRIPTING_API_IMPL();

public:
    Entity(const EntityId id, World* world) : m_id(id), m_world(world)
    {
    }

public:
    void Activate()
    {
        m_world->ActivateEntity(*this);
    }

    void Deactivate()
    {
        m_world->DeactivateEntity(*this);
    }

    void Clear()
    {
        m_world->ClearEntity(*this);
    }

    void Destroy()
    {
        m_world->DestroyEntity(*this);
    }

    template<typename T>
    T& AddComponent()
    {
        static_assert(std::is_base_of<Component, T>(), "T must inherit from Component!");

        // Create component instance
        auto component = new T();

        auto componentId = GetComponentId<T>();

        _ASSERT_(componentId < ECS_MAX_NATIVE_COMPONENTS, "Native ECS component id overflow!");

        // Add component
        m_world->AddEntityComponent(m_id, component, componentId);

        return *component;
    }

    template<typename T, typename ... TArgs>
    T& AddComponent(TArgs&& ... args)
    {
        static_assert(std::is_base_of<Component, T>(), "T must inherit from Component!");

        // Create component instance
        auto component = new T{ std::forward<TArgs>(args)... };

        auto componentId = GetComponentId<T>();

        _ASSERT_(componentId < ECS_MAX_NATIVE_COMPONENTS, "Native ECS component id overflow!");

        // Add component
        m_world->AddEntityComponent(m_id, component, componentId);

        return *component;
    }

    template<typename T>
    T& GetComponent() const
    {
        static_assert(std::is_base_of<Component, T>(), "T must inherit from Component!");

        // Get component
        return static_cast<T&>(m_world->GetEntityComponent(m_id, GetComponentId<T>()));
    }

    template<typename T>
    void RemoveComponent() const
    {
        static_assert(std::is_base_of<Component, T>(), "T must inherit from Component!");

        const TypeId componentId = GetComponentId<T>();
        m_world->RemoveEntityComponent(m_id, componentId);
    }

    template<typename T>
    void RemoveComponent(T* componentPtr)
    {
        static_assert(std::is_base_of<Component, T>(), "T must inherit from Component!");
        RemoveComponent<T>();
    }

    template<typename T>
    bool HasComponent()
    {
        static_assert(std::is_base_of<Component, T>(), "T must inherit from Component!");
        return m_world->HasEntityComponent(m_id, GetComponentId<T>());
    }

    template<typename T>
    bool HasComponent(T* componentPtr)
    {
        static_assert(std::is_base_of<Component, T>(), "T must inherit from Component!");
        return HasComponent<T>();
    }

    bool IsValid() const
    {
        return m_world->IsEntityValid(*this);
    }

    bool IsActive() const
    {
        return m_world->IsEntityActive(*this);
    }

    EntityId GetId() const
    {
        return m_id;
    }

    World* GetWorld() const
    {
        return m_world;
    }
};

static_assert(sizeof(Entity) == 16, "Invalid Entity stucture size! .NET binding may be invalid!");

#endif // ENTITY_H
