// ReCrafted (c) 2016-2019 Always Too Late

#include "World.h"
#include "Entity.h"
#include "EntityManager.h"

World::World()
{
    m_entityManager = new EntityManager(this);

    // Initialize
    m_entityManager->Initialize();
}

World::~World()
{
    Clear();

    SafeDelete(m_entityManager);

    // Clear active systems
    m_activeSystems.clear();
}

void World::Update(const bool updateSystems)
{
    // Update entity pool
    m_entityManager->Update();

    if(updateSystems)
    {
        // Update every system
        // NOTE: This can be dispatched (if system allows MT and Pararell dispatching)
        // over few threads (pararell Update, select some entities and pass it to Update)
        for (auto& system : m_activeSystems)
        {
            if (system)
                system->Update();
        }
    }
}

void World::Clear()
{
    // Update last time the world, this is needed 
    // to properly call all OnEntityRemoved events etc.
    //Update();

    // Delete all systems and clear hash map
    for (auto& system : m_systems)
    {
        if (system == nullptr)
            continue;

        Destroy(system);
        system = nullptr;
    }
}

Entity World::CreateEntity()
{
    return m_entityManager->Acquire();
}

void World::DestroyEntity(Entity& entity)
{
    entity.Clear();
    m_entityManager->Release(entity.GetId());
}

void World::ActivateEntity(Entity& entity)
{
    m_entityManager->Activate(entity.GetId());
}

void World::ActivateEntityNow(Entity& entity)
{
    m_entityManager->ActivateNow(entity.GetId());
}

void World::DeactivateEntity(Entity& entity)
{
    m_entityManager->Deactivate(entity.GetId());
}

void World::ClearEntity(Entity& entity)
{
    m_entityManager->Clear(entity.GetId());
}

void World::RefreshEntity(Entity& entity)
{
    m_entityManager->Refresh(entity.GetId());
}
