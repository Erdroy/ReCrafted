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
	Array<Ptr<Entity>> m_entities = {};

private:
    void onInit() override;
    void onDispose() override;

public:
    virtual ~EntityPool() = default;

public:
    void update() override;
    void simulate();

public:
	static Ptr<Entity> createEntity(Text entityName)
	{
		Ptr<Entity> entity(new Entity);
        entity->guid = Platform::newGuid();
		entity->name = entityName;

        m_instance->m_entities.add(entity);

		return entity;
	}

	static void destroyEntity(Entity* entity)
	{
		
	}
};

#endif // ENTITYPOOL_H
