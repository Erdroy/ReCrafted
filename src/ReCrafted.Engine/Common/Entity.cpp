// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Entity.h"
#include "EntityPool.h"

void Entity::addScript(Ptr<Script> script)
{
}

void Entity::removeScript(Ptr<Script> script)
{
}

void Entity::addChildren(Ptr<Entity> entity)
{
}

void Entity::removeChildren(Ptr<Entity> entity)
{
}

void Entity::onDestroy()
{
    // Dispose scripts
    for (auto && script : m_scripts)
        script->dispose();

    // Destroy children
    for (auto && child : m_children)
        destroy(child.get());

    EntityPool::destroyEntity(this);
}
