// ReCrafted © 2016-2017 Always Too Late

#include "Entity.h"
#include "EntityPool.h"

void Entity::addScript(Ptr<Script> script)
{
    scripts.push_back(script);
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
        destroy(script.get());
        script->dispose();
    }

    // Destroy children
    for (auto && child : children)
    {
        destroy(child.get());
    }

    scripts.clear();
    children.clear();

    EntityPool::destroyEntity(this);
}
