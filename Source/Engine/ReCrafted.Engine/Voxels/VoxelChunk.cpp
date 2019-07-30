// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "VoxelChunk.h"
#include "Rendering/Models/ModelRenderingSystem.h"

VoxelChunk::~VoxelChunk()
{
}

void VoxelChunk::SetVisible(const bool isVisible)
{
    if(isVisible)
    {
        m_model = ModelRenderingSystem::AcquireModelComponent();
        m_model->Transform = &m_transform;
        //m_model->Bounds = BoundingSphere(m_transform.translation, ); TODO: Calculate radius
        //m_model->Material = ;
        //m_model->Mesh = ;
        m_model->Active = true;
    }
    else
    {
        ModelRenderingSystem::ReleaseModelComponent(m_model);
        m_model = nullptr;
    }
}
