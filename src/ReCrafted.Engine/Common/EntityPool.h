// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef ENTITYPOOL_H
#define ENTITYPOOL_H

// includes
#include "ReCrafted.h"
#include "Core/EngineComponent.h"
#include "Entity.h"
#include "Platform/Platform.h"
#include "Core/Containers/Array.h"

class EntityPool : public EngineComponent<EntityPool>
{
private:
    Array<Ref<Entity>> m_entities = {};

private:
    void OnInit() override;
    void OnDispose() override;

public:
    virtual ~EntityPool() = default;

public:
    void Update() override;
    void Simulate();

public:
    static Ref<Entity> CreateEntity(Text entityName)
    {
        Ref<Entity> entity(new Entity);
        entity->guid = Platform::NewGuid();
        entity->name = entityName;

        m_instance->m_entities.Add(entity);

        return entity;
    }

    static void DestroyEntity(Entity* entity)
    {
    }
};

#endif // ENTITYPOOL_H
