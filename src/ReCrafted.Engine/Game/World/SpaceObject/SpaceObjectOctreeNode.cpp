// ReCrafted (c) 2016-2018 Always Too Late

#include "SpaceObjectOctreeNode.h"
#include "SpaceObjectOctree.h"
#include "SpaceObjectChunk.h"
#include "SpaceObjectManager.h"
#include "SpaceObjectTables.hpp"
#include "Graphics/Camera.h"
#include "Utilities/VoxelUtils.h"

#define HAS_LOCAL_NEIGH(id, dir) LocalNeighTable[id] & (1 << dir)

bool SpaceObjectOctreeNode::HasPopulatedChildren()
{
    for (auto i = 0; i < 8; i++)
    {
        auto node = m_childrenNodes[i];

        if (node && node->m_populated)
            return true;
    }

    return false;
}

bool SpaceObjectOctreeNode::IsChildrenProcessing() const
{
    if (m_processing)
        return true;

    if (m_populated)
    {
        for (auto i = 0; i < 8; i++)
        {
            cvar node = m_childrenNodes[i];

            if (!node)
                continue;

            if (node->IsChildrenProcessing())
                return true;
        }
    }

    return false;
}

void SpaceObjectOctreeNode::MarkProcessing()
{
    m_processing = true;

    if (!m_populated)
        return;

    for (auto i = 0; i < 8; i++)
        m_childrenNodes[i]->MarkProcessing();
}

void SpaceObjectOctreeNode::CreateChunk(IVoxelMesher* mesher)
{
    // create chunk
    m_chunk = std::make_shared<SpaceObjectChunk>();
    m_chunk->Init(this, owner->spaceObject);
    m_chunk->Generate(mesher);
}

void SpaceObjectOctreeNode::WorkerPopulate(IVoxelMesher* mesher) // WARNING: this function is called on WORKER THREAD!
{
    cvar childrenSize = m_Size / 2;
    var boundsSize = Vector3::One() * static_cast<float>(childrenSize);
    for (auto i = 0; i < 8; i++)
    {
        auto position = m_Position + ChildrenNodeOffsets[i] * static_cast<float>(childrenSize);

        // construct node
        cvar childNode = m_childrenNodes[i] = new SpaceObjectOctreeNode();
        childNode->m_Position = position;
        childNode->m_Size = childrenSize;
        childNode->m_Bounds = BoundingBox(position, boundsSize);

        // set owner, parent and root
        childNode->owner = owner;
        childNode->parent = this;
        childNode->root = root;

        // set node id
        childNode->m_childrenId = i;

        // generate chunk
        childNode->CreateChunk(mesher);
    }
}

void SpaceObjectOctreeNode::WorkerDepopulate(IVoxelMesher* mesher)
// WARNING: this function is called on WORKER THREAD!
{
}

void SpaceObjectOctreeNode::WorkerRebuild(IVoxelMesher* mesher) // WARNING: this function is called on WORKER THREAD!
{
    // rebuild chunks mesh
    m_chunk->Rebuild(mesher);
}

void SpaceObjectOctreeNode::FindIntersecting(Array<SpaceObjectOctreeNode*>& nodes, BoundingBox& box,
                                             const int targetNodeSize)
{
    // iterate all children nodes
    for (var i = 0; i < 8; i++)
    {
        cvar node = m_childrenNodes[i];

        if (!node)
            continue;

        if (BoundingBox::Intersects(node->m_Bounds, box))
        {
            if (targetNodeSize == 0)
            {
                node->FindIntersecting(nodes, box, targetNodeSize);
                nodes.Add(node);
            }
            else if (node->m_Size == targetNodeSize)
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

void SpaceObjectOctreeNode::Populate()
{
    if (m_processing || m_populated || m_Size <= MinimumNodeSize)
        return;

    m_processing = true;

    // queue populate job
    SpaceObjectManager::Enqueue(this, ProcessMode::Populate, MakeDelegate(SpaceObjectOctreeNode::OnPopulate));
}

void SpaceObjectOctreeNode::Depopulate()
{
    if (m_processing || !m_populated || IsChildrenProcessing())
        return;

    // queue depopulate job
    SpaceObjectManager::Enqueue(this, ProcessMode::Depopulate, MakeDelegate(SpaceObjectOctreeNode::OnDepopulate));
}

void SpaceObjectOctreeNode::Rebuild()
{
    if (m_processing || m_rebuilding)
        return;

    m_rebuilding = true;

    // queue rebuild job
    SpaceObjectManager::Enqueue(this, ProcessMode::Rebuild, MakeDelegate(SpaceObjectOctreeNode::OnRebuild));
}

void SpaceObjectOctreeNode::Modify(VoxelEditMode::_enum mode, Vector3& position, float size)
{
    cvar chunk = GetChunk();

    if (chunk == nullptr)
        return;

    cvar chunkData = chunk->GetChunkData();
    cvar voxels = chunkData->GetData();
    cvar chunkScale = static_cast<float>(chunkData->GetLod());

    for (var x = 0; x < VoxelChunkData::ChunkDataSize; x++)
    {
        for (var y = 0; y < VoxelChunkData::ChunkDataSize; y++)
        {
            for (var z = 0; z < VoxelChunkData::ChunkDataSize; z++)
            {
                var point = Vector3(float(x), float(y), float(z)) * chunkScale + chunkData->GetChunkPosition();

                cvar currentValue = voxels[INDEX_3D(x, y, z, VoxelChunkData::ChunkDataSize)];
                cvar distance = Vector3::Distance(position, point);

                if (distance <= size + 0.5f)
                {
                    var value = size - distance;
                    var newValue = VOXEL_FROM_FLOAT(value);

                    if (mode == VoxelEditMode::Additive)
                    {
                        newValue = -newValue;

                        if (newValue < currentValue)
                            voxels[INDEX_3D(x, y, z, VoxelChunkData::ChunkDataSize)] = newValue;
                    }
                    else
                    {
                        if (newValue > currentValue)
                            voxels[INDEX_3D(x, y, z, VoxelChunkData::ChunkDataSize)] = newValue;
                    }
                }
            }
        }
    }

    chunkData->HasSurface(true);
}

void SpaceObjectOctreeNode::OnUpdate()
{
}

void SpaceObjectOctreeNode::OnCreate()
{
    m_processing = false;

    // upload chunk if needed
    if (m_chunk->NeedsUpload())
        m_chunk->Upload();
}

void SpaceObjectOctreeNode::OnRebuild()
{
    m_rebuilding = false;

    // upload chunk if needed
    if (m_chunk->NeedsUpload())
        m_chunk->Upload();
}

void SpaceObjectOctreeNode::OnDestroy()
{
    // Destroy chunk
    SafeDispose(m_chunk);
}

void SpaceObjectOctreeNode::OnPopulate()
{
    m_processing = false;
    m_populated = true;

    for (var i = 0; i < 8; i++)
    {
        cvar node = m_childrenNodes[i];

        if (node)
            node->OnCreate();
    }

    //OnDestroy();
}

void SpaceObjectOctreeNode::OnDepopulate()
{
    m_populated = false;
    m_processing = false;

    // Destroy all children
    for (auto i = 0; i < 8; i++)
    {
        auto node = m_childrenNodes[i];

        if (node)
        {
            // call event
            node->OnDestroy();

            // delete node
            delete node;

            // cleanup
            m_childrenNodes[i] = nullptr;
        }
    }

    OnCreate();
}

void SpaceObjectOctreeNode::Update()
{
    if (m_populated)
    {
        for (auto i = 0; i < 8; i++)
        {
            auto node = m_childrenNodes[i];

            if (node)
                node->Update();
        }
        return;
    }

    OnUpdate();
}

void SpaceObjectOctreeNode::UpdateViews(Array<Vector3>& views)
{
    cvar viewRangeMultiplier = 4.0f;

    if (m_processing)
        return;

    if (m_populated)
    {
        // Update children node views, because we are populated
        for (auto i = 0; i < 8; i++)
        {
            auto node = m_childrenNodes[i];

            if (node)
                node->UpdateViews(views);
        }
    }

    // we cannot depopulate/populate etc. because this node has populated children
    // which should be depoplated first, but this doesn't happen this time
    if (m_populated && HasPopulatedChildren())
        return;

    // do not populate/depopulate root
    if (m_isRoot)
        return;

    // X - node position
    // C - view position
    // A - populate range
    // B - depopulate range
    // P - outer bounds, actually no value for this, 
    // this is only for simplified version of the algorithm.
    // 
    // dist(X, A) = node_size + node_size * 0.5f
    // dist(X, B) = dist(X, A) + node_size * 0.25
    // 
    // X ---C--|----|------- P
    //         A    B
    // X --- dist(A, C) ---> P
    // 
    // Algorithm:
    // IF (any C is in X-A) AND NOT populated: populate - otherwise: go further
    // IF (all C's are in B-P) AND populated: depopulate - otherwise: go further
    // IF (there is no C's) AND populated: depopulate - otherwise: ignore.

    // fast exit
    if (views.Count() == 0)
    {
        // IF (there is no C's) AND populated: depopulate - otherwise: ignore.
        if (m_populated)
            Depopulate();
        return;
    }

    // dist(X, A) = node_size + node_size * 0.5f
    cvar distXA = m_Size + m_Size * 0.5f * viewRangeMultiplier;

    // dist(X, B) = dist(X, A) + node_size * 0.25
    cvar distXB = distXA + m_Size * 0.25f * viewRangeMultiplier;

    // flags
    var hasXA = false;
    var hasXB = false;

    // check view flags
    for (auto&& view : views)
    {
        cvar distanceXC = Vector3::Distance(m_Position, view);

        if (distanceXC <= distXA)
        {
            hasXA = true;
            break;
        }

        if (distanceXC <= distXB)
            hasXB = true;
    }

    // IF (any C is in X-A) AND NOT populated: populate - otherwise: go further
    if (hasXA)
    {
        if (!m_populated)
            Populate();
        return;
    }

    // IF (all C's are in B-P) AND populated: depopulate - otherwise: go further
    if (!hasXA && !hasXB)
    {
        if (m_populated)
            Depopulate();
        return;
    }

    // IF (there is no C's) AND populated: depopulate - otherwise: ignore.
    if (m_populated && views.Count() > 0)
        Depopulate();
}

void SpaceObjectOctreeNode::Draw()
{
    if (!Camera::GetMainCamera()->GetBoundingFrustum().Contains(m_Bounds))
        return;

    if (m_chunk && !m_populated)
    {
        m_chunk->Draw();
        return;
    }

    for (var i = 0; i < 8; i++)
    {
        var node = m_childrenNodes[i];

        if (node)
        {
            node->Draw();
        }
    }
}

void SpaceObjectOctreeNode::Dispose()
{
    // draw chunk if exists
    if (m_chunk)
        m_chunk->Dispose();

    if (!m_populated)
        return;

    for (auto i = 0; i < 8; i++)
    {
        auto node = m_childrenNodes[i];

        if (node)
        {
            // Dispose
            node->Dispose();

            // delete node
            delete node;

            // cleanup
            m_childrenNodes[i] = nullptr;
        }
    }
}

SpaceObjectOctreeNode* SpaceObjectOctreeNode::GetNeighNode(NodeDirection::_enum direction) const
{
    // calculate target position
    var targetPosition = m_Position + DirectionOffset[direction] * float(m_Size);
    var targetLod = m_Size;

    // traverse from root to the same target lod as this node
    for (var i = 0; i < owner->m_rootNodesCount; i++)
    {
        var node = owner->m_rootNodes[i]->FindNode(targetPosition, targetLod);;

        if (node)
            return node;
    }

    return nullptr;
}

SpaceObjectOctreeNode* SpaceObjectOctreeNode::FindNode(Vector3 position, int size)
{
    if (m_Size == size)
    {
        // do not search anymore, because we found the exact node!
        return this;
    }

    if (!m_populated)
    {
        // uhh, this is lower-lod node, client will not be happy with this, or will be?
        return this;
    }

    // NOTE: we don't have to check 'equality' of position because 'size' does this
    var xSign = position.x > m_Position.x;
    var ySign = position.y > m_Position.y;
    var zSign = position.z > m_Position.z;

    var caseCode = (zSign ? 1 : 0) | (ySign ? 1 : 0) << 1 | (xSign ? 1 : 0) << 2;
    var childId = NodeDirIds[caseCode];

    var node = m_childrenNodes[childId];

    // go further
    return node->FindNode(position, size);
}
