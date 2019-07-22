// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "VoxelObjectOctree.h"
#include "VoxelLookupTables.h"
#include "Common/List.h"
#include "VoxelObjectBase.h"

VoxelObjectOctree::Node::Node()
{
}

VoxelObjectOctree::Node::~Node()
{
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

void VoxelObjectOctree::Node::FindIntersecting(List<Node*>& nodes, const BoundingBoxD& box, const int targetNodeSize)
{
    // iterate all children nodes
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

VoxelObjectOctree::Node* VoxelObjectOctree::Node::FindNeighbor(const NodeDirection direction) const
{
    // calculate target position
    const auto targetPosition = m_position + VoxelLookup::DirectionOffset[int(direction)] * float(m_size);
    return m_owner->FindNode(targetPosition, m_size);
}

VoxelObjectOctree::Node* VoxelObjectOctree::Node::Find(const Vector3d& position, const int size)
{
    // Check if this is the node that we are looking for
    if (m_size == size)
        return this;

    const auto xSign = position.x > m_position.x;
    const auto ySign = position.y > m_position.y;
    const auto zSign = position.z > m_position.z;

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
    if (m_position.x == position.x && m_position.y == position.y && m_position.z == position.z)
        return this;

    //if (!m_populated)
    //    return nullptr;

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
