// ReCrafted (c) 2016-2018 Always Too Late

#ifndef WORLD_H
#define WORLD_H

#include "Component.h"
#include "System.h"
#include "EntityId.h"

class EntityManager;
class EntityComponents;

class World
{
    friend class Entity;
    friend class EntityManager;

private:
    EntityManager* m_entityManager;
    std::array<System*, ECS_MAX_SYSTEMS> m_systems{};
    std::vector<System*> m_activeSystems{};

public:
    World();
    ~World();

private:
    void AddEntityComponent(const EntityId entityId, Component* component, const TypeId componentId) const
    {
        m_entityManager->AddEntityComponent(entityId, component, componentId);
    }

    Component& GetEntityComponent(const EntityId entityId, const TypeId componentId) const
    {
        return m_entityManager->GetEntityComponent(entityId, componentId);
    }
    
    bool HasEntityComponent(const EntityId entityId, const TypeId componentId) const
    {
        return m_entityManager->HasEntityComponent(entityId, componentId);
    }

    void RemoveEntityComponent(const EntityId entityId, const TypeId componentId) const
    {
        return m_entityManager->RemoveEntityComponent(entityId, componentId);
    }

public:
    void Update(bool updateSystems = true);
    void Clear();

public:
    Entity CreateEntity();

    void ActivateEntity(Entity& entity);
    void DeactivateEntity(Entity& entity);
    void DestroyEntity(Entity& entity);
    void ClearEntity(Entity& entity);

    bool IsEntityValid(const Entity& entity) const
    {
        return m_entityManager->IsEntityValid(entity);
    }

    bool IsEntityActive(const Entity& entity) const
    {
        return m_entityManager->IsEntityActive(entity);
    }

public:
    template<typename TSystem>
    TSystem& AddSystem()
    {
        static_assert(std::is_base_of<System, TSystem>(), "TSystem must inherit from System!");
        auto system = new TSystem();
        system->m_world = this;

        auto systemId = ClassTypeId<System>::GetTypeId<TSystem>();

        _ASSERT_(systemId < ECS_MAX_NATIVE_SYSTEMS, "Native ECS system id overflow!");

        AddSystem(system, systemId);

        return *system;
    }

    void AddSystem(System* system, const TypeId systemId)
    {
        system->m_world = this;
        m_systems[systemId] = system;

        m_activeSystems.emplace_back(system);

        // Initialize system
        system->Initialize();
    }

    template<typename TSystem>
    void RemoveSystem()
    {
        static_assert(std::is_base_of<System, TSystem>(), "TSystem must inherit from System!");

        auto systemId = ClassTypeId<System>::GetTypeId<TSystem>();

        _ASSERT_(systemId < ECS_MAX_NATIVE_SYSTEMS, "Native ECS system id overflow!");

        auto system = m_systems[systemId];

        if (system)
        {
            // Shutdown system
            system->Shutdown();

            m_systems[systemId] = nullptr;

            delete system;
        }
    }

    void RemoveSystem(const TypeId systemId, const bool deleteSystem = true)
    {
        auto system = m_systems[systemId];

        if (system)
        {
            // Shutdown system
            system->Shutdown();

            m_systems[systemId] = nullptr;

            if(deleteSystem)
            {
                delete system;
            }
        }
    }

    template<typename TSystem>
    bool HasSystem()
    {
        static_assert(std::is_base_of<System, TSystem>(), "TSystem must inherit from System!");

        auto systemId = ClassTypeId<System>::GetTypeId<TSystem>();

        _ASSERT_(systemId < ECS_MAX_NATIVE_SYSTEMS, "Native ECS system id overflow!");

        auto system = m_systems[systemId];

        return system != nullptr;
    }

    bool HasSystem(const TypeId systemId)
    {
        return m_systems[systemId] != nullptr;
    }
};

#endif // WORLD_H
