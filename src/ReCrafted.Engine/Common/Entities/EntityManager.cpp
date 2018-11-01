// ReCrafted (c) 2016-2018 Always Too Late

#include "EntityManager.h"
#include "Entity.h"
#include "World.h"
#include "ComponentSystem.h"

bool EntityManager::EntityDesc::HasSystem(ComponentSystem* system)
{
    return std::find(systems.begin(), systems.end(), system) != systems.end();
}

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
        ActivateNow(entityId);
    }

    // Deactivate entities
    for (auto& entityId : m_tempDeactivate)
    {
        DeactivateNow(entityId);
    }

    // Free entities
    for(auto entityId : m_tempFree)
    {
        ReleaseNow(entityId);
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
    _ASSERT_(IsEntityActive(entityId) == false, "Only nonactive entity can be activated!");

    m_tempActivate.emplace_back(entityId);
}

void EntityManager::ActivateNow(const EntityId entityId)
{
    auto& entityDesc = m_entities[entityId];
    entityDesc.active = true;

    // Activate in all matching systems (add to System::m_activeEntities and call OnEntityAdded event)
    for (auto& system : m_world->m_activeSystems)
    {
        if (!system)
            continue;

        const auto filter = system->GetFilter();

        // Fast system lookup by component id
        if (filter.MatchAll(entityDesc.componentTypes))
        {
            auto entity = MakeEntity(entityId);
            system->m_activeEntities.emplace_back(entity);
            system->OnEntityAdded(entity);

            // Add system to the entity
            entityDesc.systems.emplace_back(system);
        }
    }
}

void EntityManager::Deactivate(EntityId entityId)
{
    _ASSERT_(IsEntityActive(entityId) == true, "Only active entity can be deactivated!");

    m_tempDeactivate.emplace_back(entityId);
}

void EntityManager::DeactivateNow(const EntityId entityId)
{
    auto& entity = m_entities[entityId];
    entity.active = false;

    for (auto& system : entity.systems)
    {
        // Call entity removed event
        system->OnEntityRemoved(MakeEntity(entityId));
    }

    // Clear systems
    entity.systems.clear();
}

void EntityManager::Release(EntityId entityId)
{
    m_tempFree.emplace_back(entityId);
}

void EntityManager::ReleaseNow(const EntityId entityId)
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

void EntityManager::Clear(EntityId entityId)
{
    m_tempClear.emplace_back(entityId);
}

void EntityManager::Refresh(EntityId entityId)
{
    _ASSERT_(IsEntityActive(entityId) == true, "Only active entity can be refreshed!");
   
    // List new systems
    // List removed systems
    auto& entityDesc = m_entities[entityId];

    for (auto& system : m_world->m_activeSystems)
    {
        if (!system)
            continue;

        cvar hasSystem = entityDesc.HasSystem(system);
        cvar matchFilter = system->GetFilter().MatchAll(entityDesc.componentTypes);

        if (matchFilter && !hasSystem)
        {
            auto entity = MakeEntity(entityId);
            system->OnEntityAdded(entity);

            // Add entity to the system
            system->m_activeEntities.emplace_back(entity);

            // Add system to the entity
            entityDesc.systems.emplace_back(system);
        }

        if (!matchFilter && hasSystem)
        {
            // Call entity removed event
            system->OnEntityRemoved(MakeEntity(entityId));

            // Remove entity from the system
            auto& vec = system->m_activeEntities;
            vec.erase(std::remove_if(vec.begin(), vec.end(), [entityId](const Entity& entity)
            {
                return entity.GetId() == entityId;
            }), vec.end());

            // Remove system from the entity
            auto& sys = entityDesc.systems;
            sys.erase(std::remove_if(sys.begin(), sys.end(), [system](const ComponentSystem* sys)
            {
                return system == sys;
            }), sys.end());
        }
    }

    // Call OnEntityAdded and OnEntityRemoved for all systems that are listed
}

bool EntityManager::IsEntityValid(const Entity& entity) const
{
    return IsEntityValid(entity.GetId());
}

bool EntityManager::IsEntityActive(const Entity& entity) const
{
    return IsEntityActive(entity.GetId());
}
