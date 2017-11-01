// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef ENTITYPOOL_H
#define ENTITYPOOL_H

// includes
#include "ReCrafted.h"
#include "Entity.h"
#include "Platform/Platform.h"

class EntityPool
{
private:
	static EntityPool* m_instance;

private:
	std::vector<Ptr<Entity>> m_entities = {};

public:
	void initialize();
	void dispose();

public:
	static Ptr<Entity> createEntity(Text entityName)
	{
		Ptr<Entity> entity(new Entity);
        entity->guid = Platform::newGuid();
		entity->name = entityName;

        m_instance->m_entities.push_back(entity);

		return entity;
	}

	static void destroyEntity(Entity* entity)
	{
		
	}
};

#endif // ENTITYPOOL_H
