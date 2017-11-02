// ReCrafted © 2016-2017 Always Too Late

#include "Entity.h"
#include "EntityPool.h"

void Entity::addScript(Ptr<Script> script)
{
    scripts.add(script);
}

void Entity::removeScript(Ptr<Script> script)
{
}

void Entity::addChildren(Ptr<Entity> entity)
{
    children.add(entity);
}

void Entity::removeChildren(Ptr<Entity> entity)
{

}

void Entity::update()
{
    for (auto && script : scripts)
    {
        script->update();
    }
}

void Entity::simulate()
{
    for (auto && script : scripts)
    {
        script->simulate();
    }
}

void Entity::onDestroy()
{
    // Dispose scripts
    for (auto && script : scripts)
    {
        destroy(script);
        script->dispose();
    }

    // Destroy children
    for (auto && child : children)
    {
        destroy(child);
    }

    scripts.clear();
    children.clear();

    EntityPool::destroyEntity(this);
}
