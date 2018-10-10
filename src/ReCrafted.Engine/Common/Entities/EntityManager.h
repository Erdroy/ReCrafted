// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "Component.h"
#include "Filter.h"
#include "EntityId.h"

#include <array>
#include <vector>

class World;
class System;
class Entity;

class EntityManager
{
public:
    static const int EntityInitialCount = 1024;
    static const int EntityAllocateIncrement = 16;

private:
    struct EntityDesc
    {
        typedef std::array<Component*, ECS_MAX_COMPONENTS> ComponentArray;

    public:
        bool active;
        ComponentTypeList componentTypes;
        ComponentArray components;
        std::vector<System*> systems;

    public:
        EntityDesc() : active(false), componentTypes({}), components({}), systems({}) {}

    public:
        void Clear()
        {
            active = false;

            for(auto i = 0; i < ECS_MAX_COMPONENTS; i ++)
            {
                delete components[i];

                components[i] = nullptr;
                componentTypes[i] = false;
            }

            systems.clear();
        }
    };

private:
    World* m_world = nullptr;

private:
    std::vector<EntityDesc> m_entities;
    std::vector<EntityId> m_freeEntities;

    std::vector<EntityId> m_tempClear;
    std::vector<EntityId> m_tempFree;
    std::vector<EntityId> m_tempActivate;
    std::vector<EntityId> m_tempDeactivate;

public:
    EntityManager(World* world) : m_world(world) { }
    ~EntityManager();

private:
    void AllocateEntities(int numEntities);
    EntityId AcquireId();
    Entity MakeEntity(const EntityId& id) const;

public:
    void Initialize();
    void Update();

    Entity Acquire();
    void Activate(EntityId entityId);
    void Deactivate(EntityId entityId);
    void Release(EntityId entityId);
    void Clear(EntityId entityId);

    void AddEntityComponent(const EntityId entityId, Component* component, const TypeId componentId)
    {
        m_entities[entityId].components[componentId] = component;
        m_entities[entityId].componentTypes[componentId] = true;
    }

    Component& GetEntityComponent(const EntityId entityId, const TypeId componentId)
    {
        // NOTE: This function is used very heavily! 
        // This must be as fast as possible!!!

        const auto& desc = m_entities[entityId];
        const auto& component = desc.components[componentId];

        return *component;
    }

    bool HasEntityComponent(const EntityId entityId, const TypeId componentId)
    {
        return m_entities[entityId].componentTypes[componentId];
    }

    void RemoveEntityComponent(const EntityId entityId, const TypeId componentId)
    {
        delete m_entities[entityId].components[componentId];

        m_entities[entityId].components[componentId] = nullptr;
        m_entities[entityId].componentTypes[componentId] = false;
    }

    bool IsEntityValid(const Entity& entity) const;
    bool IsEntityActive(const Entity& entity) const;
};

#endif // ENTITYMANAGER_H
