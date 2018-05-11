// ReCrafted (c) 2016-2018 Always Too Late

#include "EntityPool.h"

SINGLETON_IMPL(EntityPool)

void EntityPool::onInit()
{
}

void EntityPool::onDispose()
{
}

void EntityPool::update()
{
    for (auto&& entity : m_entities)
    {
        if (entity->parent)
            continue;

        entity->update();
    }
}

void EntityPool::simulate()
{
    for (auto&& entity : m_entities)
    {
        if (entity->parent)
            continue;

        entity->simulate();
    }
}
