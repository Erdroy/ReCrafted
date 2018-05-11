// ReCrafted (c) 2016-2018 Always Too Late

#include "Entity.h"
#include "EntityPool.h"

void Entity::addScript(Ref<Script> script)
{
    scripts.add(script);
}

void Entity::removeScript(Ref<Script> script)
{
}

void Entity::addChildren(Ref<Entity> entity)
{
    children.add(entity);
}

void Entity::removeChildren(Ref<Entity> entity)
{
}

void Entity::update()
{
    for (auto&& script : scripts)
    {
        script->update();
    }
}

void Entity::simulate()
{
    for (auto&& script : scripts)
    {
        script->simulate();
    }
}

void Entity::onDestroy()
{
    // Dispose scripts
    for (auto&& script : scripts)
    {
        destroy(script);
        script->dispose();
    }

    // Destroy children
    for (auto&& child : children)
    {
        destroy(child);
    }

    scripts.clear();
    children.clear();

    EntityPool::destroyEntity(this);
}
