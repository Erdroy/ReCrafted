// ReCrafted (c) 2016-2018 Always Too Late

#include "SpaceObjectOctree.h"
#include "SpaceObject.h"
#include "Common/Profiler/Profiler.h"
#include "Core/Math/Math.h"
#include "Graphics/Graphics.h"

void SpaceObjectOctree::GeneratePrimary()
{
    // calculate diameter
    rvar settings = spaceObject->GetSettings();
    cvar radius = settings.maxSurfaceHeight;
    cvar objectSize = Math::RoundUpToPow2(static_cast<int>(radius * 2)); // diameter rounded up to power of 2

    // calculate bounds size
    var size = Vector3::One() * static_cast<float>(objectSize);

    // build bounds
    m_Bounds = BoundingBox(settings.position, size);

    // calculate Length of root nodes on single axis
    cvar rootNodesLength = Math::Pow(2, settings.rootOctreeDepth);

    // calculate the node size
    cvar rootNodeSize = objectSize / rootNodesLength;
    
    // calculate the center offset
    cvar rootNodeOffset = static_cast<float>(static_cast<float>(rootNodesLength) / 2 * rootNodeSize) - rootNodeSize * 0.5f;
    cvar rootNodePositionOffset = Vector3(rootNodeOffset, rootNodeOffset, rootNodeOffset);

    // Calculate bounding box size
    var boundsSize = Vector3::One() * static_cast<float>(rootNodeSize);

    // calculate the count of root nodes
    m_rootNodesCount = Math::Pow(rootNodesLength, 3);

    // create root nodes
    m_rootNodes = new SpaceObjectOctreeNode*[m_rootNodesCount];
    var nodeId = 0;
    for (var x = 0; x < rootNodesLength; x ++)
    {
        for (var y = 0; y < rootNodesLength; y++)
        {
            for (var z = 0; z < rootNodesLength; z++)
            {
                ASSERT(nodeId < m_rootNodesCount);

                // create root node
                m_rootNodes[nodeId] = new SpaceObjectOctreeNode();
                var node = m_rootNodes[nodeId];

                // calculate node position
                var position = Vector3(
                    static_cast<float>(x * rootNodeSize),
                    static_cast<float>(y * rootNodeSize),
                    static_cast<float>(z * rootNodeSize));
                position -= rootNodePositionOffset;

                // set position and size
                node->SetPosition(position);
                node->SetSize(rootNodeSize);

                // set owner, parent and root
                node->owner = this;
                node->parent = nullptr;
                node->root = node;
                node->m_isRoot = true;
                node->m_Bounds = BoundingBox(position, boundsSize);
                
                node->SetDepth(0); // Set 0 as first node depth

                // populate node
                node->Populate();

                nodeId++;
            }
        }
    }

    // Calculate max depth
    // This value will be used mainly for mip map selection for CHM bitmap
    var maxDepth = 0;
    
    var currentSize = rootNodeSize;
    while(currentSize >= 16) // TODO: This can be optimized using bit operations ([number of 0 from the end] - rootOctreeDepth)
    {
        maxDepth++;
        currentSize /= 2;
    }

    // Set the max depth
    SetMaxDepth(maxDepth);
}

void SpaceObjectOctree::Update()
{
}

void SpaceObjectOctree::UpdateViews(Array<Vector3>& views)
{
    Profiler::BeginProfile("SpaceObjectOctree::updateViews");
    for (var i = 0; i < m_rootNodesCount; i++)
        m_rootNodes[i]->UpdateViews(views);
    Profiler::EndProfile();
}

void SpaceObjectOctree::Dispose()
{
    for (var i = 0; i < m_rootNodesCount; i++)
    {
        SafeDisposeNN(m_rootNodes[i]);
        SafeDelete(m_rootNodes[i]);
    }

    delete [] m_rootNodes;
}

SpaceObjectOctreeNode* SpaceObjectOctree::FindNode(const Vector3& position, int size) const
{
    for (var i = 0; i < m_rootNodesCount; i++)
    {
        if (!m_rootNodes[i])
            continue;

        if(BoundingBox::Contains(m_rootNodes[i]->GetBounds(), position))
        {
            cvar node = m_rootNodes[i]->FindNode(position, size);

            if (node)
                return node;
        }
    }

    return nullptr;
}

SpaceObjectOctreeNode* SpaceObjectOctree::FindNode(const Vector3& position) const
{
    for (var i = 0; i < m_rootNodesCount; i++)
    {
        if (BoundingBox::Contains(m_rootNodes[i]->GetBounds(), position))
        {
            cvar node = m_rootNodes[i]->FindNode(position);

            if (node)
                return node;
        }
    }

    return nullptr;
}

Array<SpaceObjectOctreeNode*> SpaceObjectOctree::FindIntersecting(const BoundingBox& box, bool leafOnly) const
{
    Array<SpaceObjectOctreeNode*> intersecting = {};

    cvar targetSize = leafOnly ? 16 : 0;

    // find all intersecting nodes
    // iterate all root nodes
    for (var i = 0; i < m_rootNodesCount; i++)
    {
        cvar node = m_rootNodes[i];

        // check if this node intersects with given bounding box, if so, proceed further
        if (BoundingBox::Intersects(node->m_Bounds, box))
        {
            node->FindIntersecting(intersecting, box, targetSize);
            intersecting.Add(node);
        }
    }

    return intersecting;
}
