// ReCrafted © 2016-2017 Always Too Late

#include "EntityPool.h"

EntityPool* EntityPool::m_instance;

void EntityPool::initialize()
{
    m_instance = this;
}

void EntityPool::dispose()
{
}

void EntityPool::update()
{
    for (auto && entity : m_entities)
    {
        if (entity->parent)
            continue;

        entity->update();
    }
}

void EntityPool::simulate()
{
    for (auto && entity : m_entities)
    {
        if (entity->parent)
            continue;

        entity->simulate();
    }
}
