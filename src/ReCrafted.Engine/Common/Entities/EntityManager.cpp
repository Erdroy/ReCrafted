// ReCrafted (c) 2016-2018 Always Too Late

#include "EntityManager.h"
#include "Entity.h"
#include "World.h"
#include "System.h"

EntityManager::~EntityManager()
{
}

void EntityManager::AllocateEntities(const int numEntities)
{
    for(auto i = 0; i < numEntities; i++)
    {
        // Add entity id to the free entities list
        m_freeEntities.emplace_back(static_cast<EntityId>(m_entities.size()));

        // Add empty entity to the global list
        m_entities.emplace_back(EntityDesc{});
    }
}

EntityId EntityManager::AcquireId()
{
    EntityId id;

    if (m_freeEntities.empty())
    {
        // Allocate entities
        AllocateEntities(EntityAllocateIncrement);

        // Get entity
        id = m_freeEntities.back();
        m_freeEntities.pop_back();
    }
    else
    {
        id = m_freeEntities.back();
        m_freeEntities.pop_back();
    }

    return id;
}

Entity EntityManager::MakeEntity(const EntityId& id) const
{
    return Entity{ id, m_world };
}

void EntityManager::Initialize()
{
    // Initialize entity pool
    AllocateEntities(EntityInitialCount);
}

void EntityManager::Update()
{
    if (m_tempActivate.empty() && m_tempDeactivate.empty() && m_tempFree.empty() && m_tempClear.empty())
        return;

    // Activate entities
    for (auto& entityId : m_tempActivate)
    {
        auto& entityDesc = m_entities[entityId];
        entityDesc.active = true;

        // Activate in all matching systems (add to System::m_activeEntities and call OnEntityAdded event)
        for(auto& system : m_world->m_activeSystems)
        {
            if (!system)
                continue;

            const auto filter = system->GetFilter();

            // Fast system lookup by component id
            if(filter.MatchAll(entityDesc.componentTypes))
            {
                auto entity = MakeEntity(entityId);
                system->m_activeEntities.emplace_back(entity);
                system->OnEntityAdded(entity);

                // Add system to the entity
                entityDesc.systems.emplace_back(system);
            }
        }
    }

    // Deactivate entities
    for (auto& entityId : m_tempDeactivate)
    {
        auto& entity = m_entities[entityId];
        entity.active = false;

        for(auto& system : entity.systems)
        {
            // Call entity removed event
            system->OnEntityRemoved(MakeEntity(entityId));
        }

        // Clear systems
        entity.systems.clear();
    }

    // Free entities
    for(auto entityId : m_tempFree)
    {
        auto& entity = m_entities[entityId];

        // Remove from all matching systems
        for (auto& system : entity.systems)
        {
            // Call entity removed event
            system->OnEntityRemoved(MakeEntity(entityId));
        }

        // Cleanup entity
        entity.Clear();

        // Free entity
        m_freeEntities.emplace_back(entityId);
    }

    // Clear entities
    for (auto& entityId : m_tempClear)
    {
        m_entities[entityId].Clear();
    }

    // Cleanup systems
    for(auto& system : m_world->m_activeSystems)
    {
        if (!system)
            continue;

        // Remove entities
        auto& vec = system->m_activeEntities;
        vec.erase(std::remove_if(vec.begin(), vec.end(), [](const Entity& entity)
        {
            return !entity.IsActive();
        }), vec.end());
    }

    // Clear temporary arrays
    m_tempActivate.clear();
    m_tempDeactivate.clear();
    m_tempFree.clear();
    m_tempClear.clear();
}

Entity EntityManager::Acquire()
{
    return MakeEntity(AcquireId());
}

void EntityManager::Activate(EntityId entityId)
{
    m_tempActivate.emplace_back(entityId);
}

void EntityManager::Deactivate(EntityId entityId)
{
    m_tempDeactivate.emplace_back(entityId);
}

void EntityManager::Release(EntityId entityId)
{
    m_tempFree.emplace_back(entityId);
}

void EntityManager::Clear(EntityId entityId)
{
    m_tempClear.emplace_back(entityId);
}

bool EntityManager::IsEntityValid(const Entity& entity) const
{
    return IsEntityValid(entity.GetId());
}

bool EntityManager::IsEntityActive(const Entity& entity) const
{
    return IsEntityActive(entity.GetId());
}
