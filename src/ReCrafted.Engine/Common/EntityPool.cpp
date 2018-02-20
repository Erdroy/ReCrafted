// ReCrafted (c) 2016-2018 Always Too Late

#include "EntityPool.h"

EntityPool* EntityPool::m_instance;

void EntityPool::onInit()
{
    m_instance = this;
}

void EntityPool::onShutdown()
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
