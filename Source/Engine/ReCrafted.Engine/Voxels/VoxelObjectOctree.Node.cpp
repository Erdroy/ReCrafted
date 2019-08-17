// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "VoxelObjectOctree.h"

#include "Common/List.h"

#include "VoxelLookupTables.h"
#include "VoxelObjectManager.h"
#include "VoxelChunk.h"
#include "Rendering/Debug/DebugDraw.h"

void VoxelObjectOctree::Node::DestroyChildren()
{
    ASSERT(m_isPopulated);

    // Destroy all children
    for (auto& node : m_childrenNodes)
    {
        // Call OnDestroy
        node->OnDestroy();

        // Delete node
        delete node;
        node = nullptr;
    }

    // Not populated anymore...
    m_isPopulated = false;
}

void VoxelObjectOctree::Node::UpdateNeighborNodes()
{
    m_neighborNodes[int(NodeDirection::Front)] = FindNeighbor(NodeDirection::Front);
    m_neighborNodes[int(NodeDirection::Back)] = FindNeighbor(NodeDirection::Back);
    m_neighborNodes[int(NodeDirection::Left)] = FindNeighbor(NodeDirection::Left);
    m_neighborNodes[int(NodeDirection::Right)] = FindNeighbor(NodeDirection::Right);
    m_neighborNodes[int(NodeDirection::Top)] = FindNeighbor(NodeDirection::Top);
    m_neighborNodes[int(NodeDirection::Bottom)] = FindNeighbor(NodeDirection::Bottom);
}

void VoxelObjectOctree::Node::DebugDraw()
{
    if(m_isPopulated)
    {
        for(auto& child : m_childrenNodes)
        {
            if (child)
                child->DebugDraw();
        }
    }
    else
    {
        const auto c = m_bounds.center;
        const auto s = m_bounds.size;

        DebugDraw::DrawWireBox({ float(c.x),  float(c.y),  float(c.z) }, { float(s.x),  float(s.y),  float(s.z) });
    }
}

void VoxelObjectOctree::Node::Populate()
{
    MAIN_THREAD_ONLY();
    ASSERT(!m_isPopulated);
    ASSERT(!m_isProcessing);

    if (m_size <= MinimumNodeSize)
        return;

    // Queue populate task and set processing flag
    VoxelObjectManager::Enqueue(this, VoxelObjectManager::ProcessMode::Populate, Action<void>::New<Node, &Node::OnPopulate>(this));
    m_isProcessing = true;
}

void VoxelObjectOctree::Node::Depopulate()
{
    MAIN_THREAD_ONLY();
    ASSERT(m_isPopulated);
    ASSERT(!m_isProcessing);

    // Process
    DestroyChildren();

    // Depopulate called.
    OnDepopulate();
}

void VoxelObjectOctree::Node::Rebuild()
{
    MAIN_THREAD_ONLY();
    ASSERT(!m_isPopulated);
    ASSERT(!m_isProcessing);

    // Rebuilds mesh of this node

    // Queue populate task and set processing flag
    VoxelObjectManager::Enqueue(this, VoxelObjectManager::ProcessMode::Rebuild, Action<void>::New<Node, &Node::OnRebuild>(this));
    m_isProcessing = true;
}

void VoxelObjectOctree::Node::OnCreate()
{
    MAIN_THREAD_ONLY();
    ASSERT(m_chunk);

    const auto c = m_bounds.center;
    m_chunk->SetTransform(Transform(Vector3(float(c.x), float(c.y), float(c.z)), Quaternion::Identity, Vector3::One));

    // Make chunk visible
    m_chunk->SetVisible(true);
}

void VoxelObjectOctree::Node::OnDestroy()
{
    MAIN_THREAD_ONLY();

    if (m_isPopulated)
    {
        // Destroy children nodes
        DestroyChildren();
    }

    if(m_chunk)
    {
        // Destroy the chunk
        delete m_chunk;
        m_chunk = nullptr;
    }
}

void VoxelObjectOctree::Node::OnPopulate()
{
    MAIN_THREAD_ONLY();
    ASSERT(m_chunk);

    for(auto& node : m_childrenNodes)
    {
        ASSERT(node);
        node->OnCreate();
    }

    // Make chunk invisible
    m_chunk->SetVisible(false);

    m_isPopulated = true;
    m_isProcessing = false;
}

void VoxelObjectOctree::Node::OnDepopulate()
{
    MAIN_THREAD_ONLY();
    ASSERT(m_chunk);

    // Make chunk visible
    m_chunk->SetVisible(true);
}

void VoxelObjectOctree::Node::OnRebuild()
{
    MAIN_THREAD_ONLY();
    ASSERT(m_chunk);

    m_isProcessing = false;
}

void VoxelObjectOctree::Node::WorkerPopulate(IVoxelMesher* mesher)
{
    const auto childrenSize = m_size / 2;
    const auto boundsSize = Vector3d::One * static_cast<float>(childrenSize);

    for(auto i = 0; i < ChildrenNodeCount; i ++)
    {
        const auto position = m_bounds.center + VoxelLookup::ChildrenNodeOffsets[i] * static_cast<float>(childrenSize);

        // Construct node
        const auto node = new Node();
        node->m_id = i;
        node->m_size = childrenSize;
        node->m_bounds = BoundingBoxD(position, boundsSize);

        // Set owner, parent and root
        node->m_owner = m_owner;
        node->m_parent = this;
        node->m_root = m_root;
        node->m_depth = m_depth + 1;

        // Create chunk
        node->m_chunk = new VoxelChunk(m_owner->m_owner);
        ASSERT(node->m_chunk);

        // Setup chunk
        node->m_chunk->Initialize(node);

        // Set node
        m_childrenNodes[i] = node;
    }

    // Generate mesh of all the new children nodes
    for(auto& node : m_childrenNodes)
    {
        ASSERT(node);

        const auto chunk = node->m_chunk;
        ASSERT(chunk);

        // Generate the chunk
        chunk->Generate(mesher);
    }

    // TODO: Update neighbor nodes and rebuild them
}

void VoxelObjectOctree::Node::WorkerRebuild(IVoxelMesher* mesher)
{
    // Generate the chunk
    m_chunk->Generate(mesher);
}

void VoxelObjectOctree::Node::FindIntersecting(List<Node*>& nodes, const BoundingBoxD& box, const int targetNodeSize)
{
    // TODO: Make sure that this node is not processing

    // Iterate all children nodes
    for (auto node : m_childrenNodes)
    {
        if (node == nullptr)
            return;

        if (BoundingBoxD::Intersects(node->m_bounds, box))
        {
            if (targetNodeSize == 0)
            {
                node->FindIntersecting(nodes, box, targetNodeSize);
                nodes.Add(node);
            }
            else if (node->m_size == targetNodeSize)
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

VoxelObjectOctree::Node* VoxelObjectOctree::Node::FindNeighbor(const NodeDirection direction)
{
    // TODO: Make sure that this node is not processing

    // Calculate target position
    const auto targetPosition = m_bounds.center + VoxelLookup::DirectionOffset[int(direction)] * float(m_size);
    return m_owner->FindNode(targetPosition, m_size);
}

VoxelObjectOctree::Node* VoxelObjectOctree::Node::Find(const Vector3d& position, const int size)
{
    // TODO: Make sure that this node is not processing

    // Check if this is the node that we are looking for
    if (m_size == size)
        return this;

    const auto xSign = position.x > m_bounds.center.x;
    const auto ySign = position.y > m_bounds.center.y;
    const auto zSign = position.z > m_bounds.center.z;

    // Build case code
    auto caseCode = 0;
    caseCode |= zSign << 0;
    caseCode |= ySign << 1;
    caseCode |= xSign << 2;

    // Select children id from caseCode and lookup table 
    const auto childId = VoxelLookup::NodeDirIds[caseCode];

    // Select node and process
    auto node = m_childrenNodes[childId]; // For non-populated nodes, this array IS initialized, but contains null pointers.

    // Children node is not populated, so, just return current node.
    if (!node)
        return this;

    // Go further
    return node->Find(position, size);
}

VoxelObjectOctree::Node* VoxelObjectOctree::Node::Find(const Vector3d& position)
{
    // TODO: Make sure that this node is not processing

    if (m_bounds.center.x == position.x && m_bounds.center.y == position.y && m_bounds.center.z == position.z)
        return this;

    if (!m_isPopulated)
        return nullptr;

    for (auto node : m_childrenNodes)
    {
        if (node)
        {
            const auto foundNode = node->Find(position);

            if (foundNode)
                return foundNode;
        }
    }

    return nullptr;
}
