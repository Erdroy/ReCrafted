// ReCrafted (c) 2016-2018 Always Too Late

#include "Entity.h"
#include "EntityPool.h"

void Entity::AddScript(Ref<Script> script)
{
    scripts.Add(script);
}

void Entity::RemoveScript(Ref<Script> script)
{
}

void Entity::AddChildren(Ref<Entity> entity)
{
    children.Add(entity);
}

void Entity::RemoveChildren(Ref<Entity> entity)
{
}

void Entity::Update()
{
    for (auto&& script : scripts)
    {
        script->Update();
    }
}

void Entity::Simulate()
{
    for (auto&& script : scripts)
    {
        script->Simulate();
    }
}

void Entity::OnDestroy()
{
    // Dispose scripts
    for (auto&& script : scripts)
    {
        Destroy(script);
        script->Dispose();
    }

    // Destroy children
    for (auto&& child : children)
    {
        Destroy(child);
    }

    scripts.Clear();
    children.Clear();

    EntityPool::DestroyEntity(this);
}
