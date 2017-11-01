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
