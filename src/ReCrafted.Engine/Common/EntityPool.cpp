// ReCrafted (c) 2016-2018 Always Too Late

#include "EntityPool.h"

SINGLETON_IMPL(EntityPool)

void EntityPool::OnInit()
{
}

void EntityPool::OnDispose()
{
}

void EntityPool::Update()
{
    for (auto&& entity : m_entities)
    {
        if (entity->parent)
            continue;

        entity->Update();
    }
}

void EntityPool::Simulate()
{
    for (auto&& entity : m_entities)
    {
        if (entity->parent)
            continue;

        entity->Simulate();
    }
}
