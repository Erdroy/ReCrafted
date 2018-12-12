// ReCrafted (c) 2016-2018 Always Too Late

#ifndef WORLD_H
#define WORLD_H

#include "Component.h"
#include "ComponentSystem.h"
#include "EntityId.h"
#include "Scripting/Method.h"
#include "Scripting/Object.h"

class EntityManager;
class EntityComponents;

class World : public Object
{
    friend struct Entity;
    friend class EntityManager;

private:
    SCRIPTING_API_IMPL()

private:
    EntityManager* m_entityManager;
    std::array<ComponentSystem*, ECS_MAX_SYSTEMS> m_systems{};
    std::vector<ComponentSystem*> m_activeSystems{};

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
    void ActivateEntityNow(Entity& entity);
    void DeactivateEntity(Entity& entity);
    void DestroyEntity(Entity& entity);
    void ClearEntity(Entity& entity);
    void RefreshEntity(Entity& entity);

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
        static_assert(std::is_base_of<ComponentSystem, TSystem>(), "TSystem must inherit from System!");
        auto system = new TSystem();
        system->m_world = this;

        auto systemId = ClassTypeId<ComponentSystem>::GetTypeId<TSystem>();

        _ASSERT_(systemId < ECS_MAX_NATIVE_SYSTEMS, "Native ECS system id overflow!");

        AddSystem(system, systemId);

        return *system;
    }

    void AddSystem(ComponentSystem* system, const TypeId systemId)
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
        static_assert(std::is_base_of<ComponentSystem, TSystem>(), "TSystem must inherit from System!");

        auto systemId = ClassTypeId<ComponentSystem>::GetTypeId<TSystem>();

        _ASSERT_(systemId < ECS_MAX_NATIVE_SYSTEMS, "Native ECS system id overflow!");

        auto system = m_systems[systemId];

        if (system)
        {
            // Shutdown system
            system->Shutdown();

            m_systems[systemId] = nullptr;

            Object::Destroy(system);
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
                Object::Destroy(system);
            }
        }
    }

    template<typename TSystem>
    bool HasSystem()
    {
        static_assert(std::is_base_of<ComponentSystem, TSystem>(), "TSystem must inherit from System!");

        auto systemId = ClassTypeId<ComponentSystem>::GetTypeId<TSystem>();

        _ASSERT_(systemId < ECS_MAX_NATIVE_SYSTEMS, "Native ECS system id overflow!");

        auto system = m_systems[systemId];

        return system != nullptr;
    }

    bool HasSystem(const TypeId systemId)
    {
        return m_systems[systemId] != nullptr;
    }

public:
    EntityManager* GetEntityManager() const
    {
        return m_entityManager;
    }
};

#endif // WORLD_H
