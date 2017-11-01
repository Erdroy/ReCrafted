// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "EntityPool.h"

EntityPool* EntityPool::m_instance;

void EntityPool::initialize()
{
    m_instance = this;
}

void EntityPool::dispose()
{
}
