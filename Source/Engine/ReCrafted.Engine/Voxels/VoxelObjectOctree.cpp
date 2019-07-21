// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "VoxelObjectOctree.h"
#include "Profiler/Profiler.h"

void VoxelObjectOctree::CreateRootNodes()
{

}

VoxelObjectOctree::VoxelObjectOctree(VoxelObjectBase* owner)
{
    m_owner = owner;
}

VoxelObjectOctree::~VoxelObjectOctree()
{
}

void VoxelObjectOctree::Update()
{
    CPU_PROFILE_FUNCTION(0);
}

VoxelObjectOctree::Node* VoxelObjectOctree::Find(const Vector3d& position, const int size) const
{
    for (auto i = 0; i < m_rootNodesCount; i++)
    {
        if (!m_rootNodes[i])
            continue;

        if (BoundingBoxD::Contains(m_rootNodes[i]->m_bounds, position))
        {
            const auto node = m_rootNodes[i]->Find(position, size);

            if (node)
                return node;
        }
    }

    return nullptr;
}

VoxelObjectOctree::Node* VoxelObjectOctree::Find(const Vector3d& position) const
{
    for (auto i = 0; i < m_rootNodesCount; i++)
    {
        if (BoundingBoxD::Contains(m_rootNodes[i]->m_bounds, position))
        {
            const auto node = m_rootNodes[i]->Find(position);

            if (node)
                return node;
        }
    }

    return nullptr;
}
