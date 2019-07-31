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
        //m_model->Bounds = BoundingSphere(m_transform.translation, ); TODO: Calculate radius (sqrt(size * 2))
        //m_model->Material = ; // TODO: Get material
        //m_model->Mesh = ; // TODO: Get mesh (Note: Mesh should be changed on main thread)
        //m_model->Active = true;
    }
    else
    {
        ModelRenderingSystem::ReleaseModelComponent(m_model);
        m_model = nullptr;
    }
}

uint64_t VoxelChunk::CalculateChunkId(const Vector3& position)
{
    // TODO: Convert from Vector3d world-space to object-space

    // Limit: 8 388 608 (world space) / 1 048 576 (node space)
    // Size: 20 bit's per integer

    // Convert components into integers
    const auto posX = static_cast<uint64_t>(position.x / 8.0f);
    const auto posY = static_cast<uint64_t>(position.y / 8.0f);
    const auto posZ = static_cast<uint64_t>(position.z / 8.0f);

    // Mask-off the last 20 bits and pack into uint64
    auto chunkId = (posX & 0xFFFFF) << 40;
    chunkId |= (posY & 0xFFFFF) << 20;
    chunkId |= posZ & 0xFFFFF;

    return chunkId;
}
