// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "VoxelObjectOctree.h"
#include "Profiler/Profiler.h"
#include "VoxelObjectBase.h"
#include "VoxelChunk.h"
#include "Assets/VoxelObjectAsset.h"
#include "Rendering/Debug/DebugDraw.h"

void VoxelObjectOctree::CreateRootNodes()
{
    const auto asset = m_owner->Asset();
    const auto radius = asset->MaximumSurfaceHeight();
    const auto diameter = Math::RoundUpToPow2(radius * 2);

    // Setup bounds
    m_bounds = BoundingSphereD(Vector3d::Zero, double(radius));

    // Calculate Length of root nodes on single axis
    const auto rootNodesLength = Math::Pow(2, asset->InitialOctreeDepth());

    // Calculate the node size
    const auto rootNodeSize = diameter / rootNodesLength;

    // calculate the center offset
    const auto rootNodeOffset = static_cast<double>(static_cast<double>(rootNodesLength) / 2 * rootNodeSize) - rootNodeSize * 0.5;
    const auto rootNodePositionOffset = Vector3d(rootNodeOffset, rootNodeOffset, rootNodeOffset);

    // Calculate bounding box size
    const auto boundsSize = Vector3d::One * static_cast<double>(rootNodeSize);

    // Calculate the count of root nodes
    m_rootNodesCount = Math::Pow(rootNodesLength, 3);

    // Create root nodes
    m_rootNodes = new Node*[m_rootNodesCount];
    auto nodeId = 0;
    for (auto x = 0; x < rootNodesLength; x++)
    {
        for (auto y = 0; y < rootNodesLength; y++)
        {
            for (auto z = 0; z < rootNodesLength; z++)
            {
                ASSERT(nodeId < m_rootNodesCount);

                // Create root node
                m_rootNodes[nodeId] = new Node();
                auto node = m_rootNodes[nodeId];

                // Calculate node position
                auto position = Vector3d(
                    static_cast<double>(x * rootNodeSize),
                    static_cast<double>(y * rootNodeSize),
                    static_cast<double>(z * rootNodeSize));
                position -= rootNodePositionOffset;

                // set owner, parent, root, position, depth and size
                node->m_id = nodeId;
                node->m_owner = this;
                node->m_parent = nullptr;
                node->m_root = node;
                node->m_isRoot = true;
                node->m_bounds = BoundingBoxD(position, boundsSize);
                node->m_size = rootNodeSize;
                node->m_depth = 0;

                // Create and setup chunk
                node->m_chunk = new VoxelChunk(m_owner);
                ASSERT(node->m_chunk);

                // Setup chunk
                node->m_chunk->Initialize(node);

                // Add node it
                nodeId++;
            }
        }
    }

    // Calculate max depth
    // This value will be used mainly for mip map selection for CHM bitmap
    auto maxDepth = 0;

    auto currentSize = rootNodeSize;
    while (currentSize >= 16)
    {
        maxDepth++;
        currentSize /= 2;
    }

    // Set the max depth
    m_maxDepth = maxDepth;
}

void VoxelObjectOctree::RebuildRootNodes() const
{
    for (auto i = 0; i < m_rootNodesCount; i++)
    {
        const auto node = m_rootNodes[i];

        // Call OnCreate
        node->OnCreate();
        node->Rebuild();
    }
}

VoxelObjectOctree::VoxelObjectOctree(VoxelObjectBase* owner)
{
    m_owner = owner;
}

VoxelObjectOctree::~VoxelObjectOctree()
{
    for (auto i = 0; i < m_rootNodesCount; i++)
    {
        auto& node = m_rootNodes[i];

        // Call OnDestroy
        node->OnDestroy();

        // Delete node
        delete node;
        node = nullptr;
    }

    delete m_rootNodes;
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

void VoxelObjectOctree::FindIntersecting(List<Node*>& nodes, const BoundingBoxD& box, int targetNodeSize)
{
    // find all intersecting nodes
    // iterate all root nodes
    for (auto i = 0; i < m_rootNodesCount; i++)
    {
        const auto node = m_rootNodes[i];

        // check if this node intersects with given bounding box, if so, proceed further
        if (BoundingBoxD::Intersects(node->Bounds(), box))
        {
            if(node->Size() == targetNodeSize)
            {
                nodes.Add(node);
            }
            else
            {
                node->FindIntersecting(nodes, box, targetNodeSize);
            }
        }
    }
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
