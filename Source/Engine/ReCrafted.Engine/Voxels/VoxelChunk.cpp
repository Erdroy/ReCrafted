// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "VoxelChunk.h"
#include "Rendering/Models/ModelRenderingSystem.h"
#include "VoxelObjectBase.h"
#include "Meshing/IVoxelMesher.h"
#include "VoxelChunkMesh.h"

VoxelChunk::~VoxelChunk()
{
}

void VoxelChunk::SetUpload(const RefPtr<VoxelChunkMesh>& mesh, UploadType uploadType)
{
    // Lock Upload
   /* ScopeLock(m_uploadLock);

    if (m_newMesh || m_newCollision)
    {
        // Queue for dispose
        m_disposeQueue.enqueue(std::make_pair(m_newMesh, m_newCollision));
    }

    // Set upload type to UPLOAD-MESH
    m_newMesh = mesh;
    //m_newCollision = collision;*/
    m_uploadType = uploadType;
}

void VoxelChunk::Upload()
{
    MAIN_THREAD_ONLY();
    /*ASSERT(NeedsUpload());

    // Dispose all queued meshes
    std::pair<RefPtr<VoxelChunkMesh>, RefPtr<VoxelChunkCollsion>> toDispose;
    while (m_disposeQueue.try_dequeue(toDispose))
    {
        if (toDispose.first)
            SafeDispose(toDispose.first);

        if (toDispose.second)
            SafeDispose(toDispose.second);
    }

    switch (m_uploadType)
    {
    case Swap:
    {
        // Dispose old mesh and swap mesh (m_newMesh will be nullptr - because m_mesh is nullptr (SafeDispose))
        ScopeLock(m_uploadLock);

        SafeDispose(m_mesh);
        m_mesh.swap(m_newMesh);

        SafeDispose(m_collision);
        m_collision.swap(m_newCollision);
        break;
    }
    case Clear:
    {
        ScopeLock(m_uploadLock);

        SafeDispose(m_mesh);
        SafeDispose(m_newMesh);

        SafeDispose(m_collision);
        SafeDispose(m_newCollision);
        break;
    }
    default: break;
    }

    // Clean upload type
    m_uploadType = None;*/
}

void VoxelChunk::Initialize(VoxelObjectOctree::Node* node)
{
    m_octreeNode = node;

    // Get storage
    auto storage = m_voxelObject->Storage();

    // Create chunk data
    const auto nodePosition = Vector3(static_cast<float>(node->Position().x), static_cast<float>(node->Position().y), static_cast<float>(node->Position().z));
    m_chunkData = storage->CreateChunkData(nodePosition, node->Size(), node->Depth());

    // Reset cache
    m_chunkData->ResetCache();

    // Calculate chunk position (origin)
    const auto positionOffset = Vector3::One * (float(node->Size()) * 0.5f);
    m_position = nodePosition - positionOffset; // lower-left-back corner

    // Calculate lod
    m_lod = int(float(node->Size()) / float(VoxelObjectOctree::Node::MinimumNodeSize));

    // Calculate id
    m_id = CalculateChunkId(nodePosition);
}

void VoxelChunk::Generate(IVoxelMesher* mesher)
{
    ASSERT(m_voxelObject);

    auto storage = m_voxelObject->Storage();

    // Try to read chunk data (if not read actually)
    if (!m_chunkData->IsLoaded())
        storage->ReadChunkData(m_chunkData);

    // Reset cache
    m_chunkData->ResetCache();

    if (!m_chunkData->HasSurface() || !m_chunkData->HasData())
        return;

    Rebuild(mesher);
}

void VoxelChunk::Rebuild(IVoxelMesher* mesher)
{
    if (!m_chunkData->IsLoaded() || !m_chunkData->HasSurface())
        return;

    auto borders = 0x0;

    auto neigh = m_octreeNode->FindNeighbor(VoxelObjectOctree::NodeDirection::Front);
    if (neigh && neigh->IsPopulated())
        borders |= 1 << 0;

    neigh = m_octreeNode->FindNeighbor(VoxelObjectOctree::NodeDirection::Back);
    if (neigh && neigh->IsPopulated())
        borders |= 1 << 1;

    neigh = m_octreeNode->FindNeighbor(VoxelObjectOctree::NodeDirection::Left);
    if (neigh && neigh->IsPopulated())
        borders |= 1 << 2;

    neigh = m_octreeNode->FindNeighbor(VoxelObjectOctree::NodeDirection::Right);
    if (neigh && neigh->IsPopulated())
        borders |= 1 << 3;

    neigh = m_octreeNode->FindNeighbor(VoxelObjectOctree::NodeDirection::Top);
    if (neigh && neigh->IsPopulated())
        borders |= 1 << 4;

    neigh = m_octreeNode->FindNeighbor(VoxelObjectOctree::NodeDirection::Bottom);
    if (neigh && neigh->IsPopulated())
        borders |= 1 << 5;

    const auto voxelData = m_chunkData->GetData();

    // Try to generate mesh data
    mesher->Generate(m_position, m_lod, borders, voxelData);

    // Check if we have any triangles and if don't, 
    // then we are going to clean everything up, including chunk data.
    if (!mesher->HasTriangles())
    {
        // Mark chunk data as no-surface, and dealloc it's data if it contains any data, NO, DON'T DEALLOC!!! (This is handled by VoxelChunkCache)
        m_chunkData->HasSurface(false);

        // Set upload type to CLEAR-MESH
        SetUpload(nullptr, UploadType::Clear);
        return;
    }

    // We have at least one valid triangle,
    // so create new mesh now and set upload state.

    // Create new mesh
    RefPtr<VoxelChunkMesh> mesh = std::make_unique<VoxelChunkMesh>();

    // Create new collision
    RefPtr<VoxelChunkCollision> collision = std::make_unique<VoxelChunkCollision>();

    // Apply mesh and collision
    mesher->Apply(mesh, collision);

    // Upload new mesh
    mesh->UploadNow();

    // TODO: SetUpload and Upload implementation
    SetUpload(mesh, UploadType::Swap);
}

void VoxelChunk::SetVisible(const bool isVisible)
{
    if(isVisible)
    {
        m_model = ModelRenderingSystem::AcquireModelComponent();
        m_model->Transform = &m_transform;
        m_model->Bounds = BoundingSphere(m_transform.translation, Math::Sqrt(float(m_octreeNode->Size()) * 2));
        //m_model->Material = ; // TODO: Get material
        // TODO: We will need multi-mesh model
        //m_model->Mesh = ; // TODO: Get meshes (Note: Mesh should be changed on main thread)
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
