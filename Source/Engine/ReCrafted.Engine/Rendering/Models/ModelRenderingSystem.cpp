// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "ModelRenderingSystem.h"

void ModelRenderingSystem::Initialize()
{
    m_renderList = {};
    m_renderList.Reserve(DefaultRenderListLength);
}

void ModelRenderingSystem::Shutdown()
{
    m_renderList.Clear();
}

void ModelRenderingSystem::Render()
{
}

void ModelRenderingSystem::Release()
{
}
