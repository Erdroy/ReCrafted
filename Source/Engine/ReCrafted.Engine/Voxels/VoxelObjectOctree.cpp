// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "VoxelObjectOctree.h"
#include "Profiler/Profiler.h"
#include "VoxelObjectBase.h"

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

void VoxelObjectOctree::Initialize()
{
    // Warning: This function is being called from task worker thread,
    // and octree should not be accessed when loading it for the first time.

    CreateRootNodes();

    m_isLoading = false;
}

void VoxelObjectOctree::Update()
{
    CPU_PROFILE_FUNCTION(0);
}

VoxelObjectOctree::Node* VoxelObjectOctree::FindNode(const Vector3d& position, const int size) const
{
    ASSERT(m_isLoading == false);

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

VoxelObjectOctree::Node* VoxelObjectOctree::FindNode(const Vector3d& position) const
{
    ASSERT(m_isLoading == false);

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
