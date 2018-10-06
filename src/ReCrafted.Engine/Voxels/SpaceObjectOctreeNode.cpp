// ReCrafted (c) 2016-2018 Always Too Late

#include "SpaceObjectOctreeNode.h"
#include "SpaceObjectOctree.h"
#include "SpaceObjectChunk.h"
#include "SpaceObjectManager.h"
#include "SpaceObjectTables.hpp"
#include "SpaceObject.h"

#include "Core/Action.h"
#include "Graphics/Camera.h"
#include "Graphics/DebugDraw.h"
#include "Graphics/Rendering/Rendering.h"
#include "Utilities/VoxelUtils.h"

#define HAS_LOCAL_NEIGH(id, dir) LocalNeighTable[id] & (1 << dir)

bool SpaceObjectOctreeNode::HasPopulatedChildren()
{
    if (!m_populated)
        return false;

    for (cvar node : m_childrenNodes)
    {
        if (!node)
            continue;

        if (node->m_populated)
            return true;
    }

    return false;
}

bool SpaceObjectOctreeNode::IsProcessing() const
{
    if (m_processing)
        return true;

    if (!m_populated)
        return false;

    for (cvar node : m_childrenNodes)
    {
        if (!node)
            continue;

        if (node->IsProcessing())
            return true;
    }

    return false;
}

void SpaceObjectOctreeNode::UpdateNeighborNodes()
{
    m_neighborNodes[NodeDirection::Front] = FindNeighNode(NodeDirection::Front);
    m_neighborNodes[NodeDirection::Back] = FindNeighNode(NodeDirection::Back);
    m_neighborNodes[NodeDirection::Left] = FindNeighNode(NodeDirection::Left);
    m_neighborNodes[NodeDirection::Right] = FindNeighNode(NodeDirection::Right);
    m_neighborNodes[NodeDirection::Top] = FindNeighNode(NodeDirection::Top);
    m_neighborNodes[NodeDirection::Bottom] = FindNeighNode(NodeDirection::Bottom);
}

void SpaceObjectOctreeNode::CreateChunk()
{
    ASSERT(m_chunk == nullptr);

    // create chunk
    m_chunk.reset(new SpaceObjectChunk());
    m_chunk->Init(this, owner->spaceObject);
}

void SpaceObjectOctreeNode::GenerateChunk(IVoxelMesher* mesher)
{
    // Update neighbors
    UpdateNeighborNodes();

    m_chunk->Generate(mesher);
}

void SpaceObjectOctreeNode::WorkerPopulate(IVoxelMesher* mesher) // WARNING: this function is called on WORKER THREAD!
{
    cvar childrenSize = m_Size / 2;
    cvar boundsSize = Vector3::One() * static_cast<float>(childrenSize);

    // Create children nodes
    for (var i = 0; i < 8; i++)
    {
        cvar position = m_Position + ChildrenNodeOffsets[i] * static_cast<float>(childrenSize);

        // Construct node
        cvar node = new SpaceObjectOctreeNode();
        
        node->m_id = i;

        node->m_Position = position;
        node->m_Size = childrenSize;
        node->m_Bounds = BoundingBox(position, boundsSize);

        // Set owner, parent and root
        node->owner = owner;
        node->parent = this;
        node->root = root;

        // Set node depth
        node->SetDepth(GetDepth() + 1);

        // Create node's chunk
        node->CreateChunk(); 

        // Initialize node
        node->Initialize();

        // Set node in children array
        m_childrenNodes[i] = node;
    }

    // Generate chunks
    for (var node : m_childrenNodes)
        node->GenerateChunk(mesher);

    // Force rebuild neighbor meshes
    UpdateNeighborNodes();

    for(var node : m_neighborNodes)
    {
        if (!node)
            continue;

        node->m_forceRebuild = true;
    }
}

void SpaceObjectOctreeNode::WorkerDepopulate(IVoxelMesher* mesher) // WARNING: this function is called on WORKER THREAD!
{
}

void SpaceObjectOctreeNode::WorkerRebuild(IVoxelMesher* mesher) // WARNING: this function is called on WORKER THREAD!
{
    // Update neighbors
    UpdateNeighborNodes();

    // Rebuild chunk
    m_chunk->Rebuild(mesher);
}

void SpaceObjectOctreeNode::DestroyChildren()
{
    ASSERT(m_populated);

    // Destroy all children
    for (rvar node : m_childrenNodes)
    {
        // Dispose node
        SafeDisposeNN(node);

        // delete node
        delete node;

        node = nullptr;
    }
}

void SpaceObjectOctreeNode::FindIntersecting(Array<SpaceObjectOctreeNode*>& nodes, const BoundingBox& box,
                                             const int targetNodeSize)
{
    // iterate all children nodes
    for (var node : m_childrenNodes)
    {
        if (node == nullptr)
            return;

        if (BoundingBox::Intersects(node->GetBounds(), box))
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
    if (m_Size <= MinimumNodeSize)
        return;

    ASSERT(!IsProcessing() && m_populated == false);

    m_processing = true;

    // queue populate job
    SpaceObjectManager::Enqueue(this, ProcessMode::Populate, Action<void>::New<SpaceObjectOctreeNode, &SpaceObjectOctreeNode::OnPopulate>(this));
}

void SpaceObjectOctreeNode::Depopulate()
{
    ASSERT(!IsProcessing());
    ASSERT(m_populated == true);

    DestroyChildren();
    OnDepopulate();
    OnCreate();
}

void SpaceObjectOctreeNode::Rebuild()
{
    if (IsProcessing())
        return;

    // queue rebuild job
    SpaceObjectManager::Enqueue(this, ProcessMode::Rebuild, Action<void>::New<SpaceObjectOctreeNode, &SpaceObjectOctreeNode::OnRebuild>(this));
}

void SpaceObjectOctreeNode::OnCreate()
{
    // Upload chunk if needed
    if (m_chunk && m_chunk->NeedsUpload())
    {
        m_chunk->Upload();
    }

    // Add this chunk to rendering
    if (m_chunk->HasMesh())
        Rendering::AddRenderable(m_chunk.get());
}

void SpaceObjectOctreeNode::OnRebuild()
{
    if (!m_chunk)
        return;

    // Upload chunk if needed
    if (m_chunk && m_chunk->NeedsUpload())
    {
        cvar hasMesh = m_chunk->HasMesh();

        m_chunk->Upload();

        // Add this chunk to rendering, as it got new mesh (if we are not populated)
        if (!hasMesh && m_chunk->HasMesh() && !m_populated)
            Rendering::AddRenderable(m_chunk.get());

        // Try to remove this chunk from rendering
        if(hasMesh && !m_chunk->HasMesh())
            Rendering::RemoveRenderable(m_chunk.get());
    }
}

void SpaceObjectOctreeNode::OnDestroy()
{
    //ASSERT(!IsProcessing());

    // Try to remove this chunk from rendering
    if (m_chunk && m_chunk->HasMesh())
        Rendering::RemoveRenderable(m_chunk.get());

    // Dispose chunk if exists
    SafeDispose(m_chunk);

    if (!m_populated)
        return;

    DestroyChildren();
}

void SpaceObjectOctreeNode::OnPopulate()
{
    m_processing = false;
    m_populated = true;

    for (var i = 0; i < 8; i++)
    {
        cvar node = m_childrenNodes[i];
        node->OnCreate();
    }

    // Try to remove this chunk from rendering
    if(m_chunk && m_chunk->HasMesh())
        Rendering::RemoveRenderable(m_chunk.get());
}

void SpaceObjectOctreeNode::OnDepopulate()
{
    m_populated = false;
    m_processing = false;
}

void SpaceObjectOctreeNode::Initialize()
{
    // Current bode has been created just now, so register this node.
    owner->RegisterNode(this);
}

void SpaceObjectOctreeNode::Dispose()
{
    OnDestroy();

    // Unregister this node, as it is going to be deleted.
    owner->UnregisterNode(this);
}

void SpaceObjectOctreeNode::DrawDebug()
{
    DebugDraw::SetColor(NodeLoDDebugColors[m_Depth]);
    DebugDraw::DrawWireBox(m_Bounds);

    static const Color LinkColors[6] = {
        Color(0xFF0000FF), // Front
        Color(0x0000FFFF), // Back
        Color(0x00FF00FF), // Left
        Color(0x00AA00FF), // Right
        Color(0xFFFFFFFF), // Top
        Color(0xAAAAAAFF), // Bottom
    };

    var id = -1;
    for(var neigh : m_neighborNodes)
    {
        id++;

        if (!neigh)
            continue;

        DebugDraw::SetColor(LinkColors[id]);
        DebugDraw::DrawArrow(m_Position, neigh->m_Position, 1.0f);

    }
}

void SpaceObjectOctreeNode::UpdateViews(Array<Vector3>& views)
{
    cvar viewRangeMultiplier = 4.0f;

    if (m_processing)
        return;

    if (m_populated)
    {
        // Update children node views, because we are populated
        for (rvar node : m_childrenNodes)
        {
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
        if (m_populated && !IsProcessing())
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
        if (m_populated && !IsProcessing())
            Depopulate();
        return;
    }

    // IF (there is no C's) AND populated: depopulate - otherwise: ignore.
    if (m_populated && views.Count() == 0)
        Depopulate();
}

bool SpaceObjectOctreeNode::Modify(const VoxelMaterial_t material, const VoxelEditMode::_enum mode, const Vector3& position, float size)
{
    cvar chunk = GetChunk();

    if (chunk == nullptr)
        return false;

    cvar chunkData = chunk->GetChunkData();

    if (!chunkData->HasData())
        chunkData->AllocateData();

    cvar chunkScale = static_cast<float>(chunkData->GetLod());
    var modified = true;

    for (var x = VoxelChunkData::ChunkDataStart; x < VoxelChunkData::ChunkDataLength; x++)
    {
        for (var y = VoxelChunkData::ChunkDataStart; y < VoxelChunkData::ChunkDataLength; y++)
        {
            for (var z = VoxelChunkData::ChunkDataStart; z < VoxelChunkData::ChunkDataLength; z++)
            {
                cvar point = Vector3(float(x), float(y), float(z)) * chunkScale + chunkData->GetChunkPosition();
                cvar distance = Vector3::Distance(position, point);
                cvar currentValue = chunkData->GetVoxel(x, y, z);

                if (distance <= size + 0.5f)
                {
                    cvar value = size - distance;
                    if(mode == VoxelEditMode::MaterialPaint)
                    {
                        chunkData->SetVoxel(x, y, z, Voxel::Create(currentValue.value, material));
                        modified = true;
                        continue;
                    }
                    
                    if (mode == VoxelEditMode::Additive)
                    {
                        var newValue = Voxel::Create(-value, material);

                        if (newValue.value <= currentValue.value)
                        {
                            chunkData->SetVoxel(x, y, z, newValue);
                        }
                        else if(currentValue.material != newValue.material)
                        {
                            newValue.value = currentValue.value;
                            chunkData->SetVoxel(x, y, z, newValue);
                        }
                        modified = true;
                        continue;
                    }
                    
                    if(mode == VoxelEditMode::Subtractive)
                    {
                        var newValue = Voxel::Create(value, currentValue.material);

                        if (newValue.value >= currentValue.value)
                        {
                            chunkData->SetVoxel(x, y, z, newValue);
                        }
                        else
                        {
                            newValue.value = currentValue.value;
                            chunkData->SetVoxel(x, y, z, newValue);
                        }
                        modified = true;
                        continue;
                    }
                }
            }
        }
    }

    if(modified)
        chunkData->HasSurface(true);

    return modified;
}

SpaceObjectOctreeNode* SpaceObjectOctreeNode::FindNeighNode(NodeDirection::_enum direction) const
{
    // calculate target position
    cvar targetPosition = m_Position + DirectionOffset[direction] * float(m_Size);
    return owner->FindNode(targetPosition, m_Size);
}

SpaceObjectOctreeNode* SpaceObjectOctreeNode::FindNode(const Vector3& position, int size)
{
    // Check if this is the node that we are looking for
    if (m_Size == size)
        return this;

    cvar xSign = position.x > m_Position.x;
    cvar ySign = position.y > m_Position.y;
    cvar zSign = position.z > m_Position.z;

    // Build case code
    var caseCode = 0;
    caseCode |= zSign << 0;
    caseCode |= ySign << 1;
    caseCode |= xSign << 2;

    // Select children id from caseCode and lookup table 
    // TODO: this can be optimized, lookup table is not really required, but we need to get the case code right.
    cvar childId = NodeDirIds[caseCode];

    // Select node and process
    var node = m_childrenNodes[childId]; // For non-populated nodes, this array IS initialized, but contains nullptrs.

    // Children node is not populated, so, just return current node.
    if (!node)
        return this;

    // Go further
    return node->FindNode(position, size);
}

SpaceObjectOctreeNode* SpaceObjectOctreeNode::FindNode(const Vector3& position)
{
    if (m_Position.x == position.x && m_Position.y == position.y && m_Position.z == position.z)
        return this;

    if (!m_populated)
        return nullptr;

    for(var node : m_childrenNodes)
    {
        if(node)
        {
            cvar foundNode = node->FindNode(position);

            if (foundNode)
                return foundNode;
        }
    }

    return nullptr;
}
