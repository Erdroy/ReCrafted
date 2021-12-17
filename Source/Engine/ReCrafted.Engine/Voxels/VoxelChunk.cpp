// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "VoxelChunk.h"
#include "Rendering/Models/ModelRenderingSystem.h"
#include "VoxelObjectBase.h"
#include "Meshing/IVoxelMesher.h"
#include "VoxelChunkMesh.h"
#include "Rendering/Materials/Material.h"
#include "Physics/RigidBodyActor.h"
#include "Profiler/Profiler.h"

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
    m_transform.translation = nodePosition - positionOffset; // lower-left-back corner

    // Calculate lod
    m_lod = int(float(node->Size()) / float(VoxelObjectOctree::Node::MinimumNodeSize));

    // Calculate id
    m_id = CalculateChunkId(nodePosition);
}

void VoxelChunk::Upload()
{
    CPU_PROFILE_FUNCTION(0);
    MAIN_THREAD_ONLY();
    ASSERT(NeedsUpload());

    // Dispose all queued meshes
    std::pair<RefPtr<VoxelChunkMesh>, MeshCollider*> toDispose;
    while (m_disposeQueue.TryDequeue(toDispose))
    {
        if (toDispose.first)
            toDispose.first.reset();
        if (toDispose.second)
            Object::Destroy(toDispose.second);
    }

    switch (m_uploadType)
    {
    case UploadType::Swap:
    {
        // Dispose old mesh and swap mesh (m_newMesh will be nullptr - because m_mesh is nullptr (SafeDispose))
        ScopeLock(m_uploadLock);

        m_mesh.reset();
        m_mesh.swap(m_newMesh);

        // Set new mesh
        const auto sections = m_mesh->GetSections();

        m_model->Meshes.Clear();
        for (auto&& section : sections)
        {
            m_model->Meshes.Add(section.mesh);
        }

        // Detach collider from the VoxelObject's physics actor
        if(m_meshCollider)
        {
            // Detach collision
            SetPhysicsState(false);

            Object::Destroy(m_meshCollider);
            m_meshCollider = nullptr;
        }

        if (sections.Count() > 0 && m_newMeshCollider)
        {
            // Setup new collision mesh
            m_meshCollider = m_newMeshCollider;
            m_newMeshCollider = nullptr;

            if(!m_octreeNode->IsPopulated())
            {
                // Attach collision only when the octree node is not populated
                SetPhysicsState(true);
            }
        }
        break;
    }
    case UploadType::Clear:
    {
        ScopeLock(m_uploadLock);

        m_mesh.reset();
        m_newMesh.reset();

        if(m_model)
        {
            // Clear meshes
            m_model->Meshes.Clear();
        }

        if (m_meshCollider)
        {
            // Detach collision
            SetPhysicsState(false);

            Object::Destroy(m_meshCollider);
            m_meshCollider = nullptr;
        }

        if (m_newMeshCollider)
        {
            // New collider is not attached
            Object::Destroy(m_newMeshCollider);
            m_newMeshCollider = nullptr;
        }
        break;
    }
    default: break;
    }

    // Clean upload type
    m_uploadType = UploadType::None;
}

void VoxelChunk::SetUpload(const RefPtr<VoxelChunkMesh>& mesh, MeshCollider* collider, const UploadType uploadType)
{
    // Lock Upload
    ScopeLock(m_uploadLock);

    if (m_newMesh || m_newMeshCollider)
    {
        // Queue for dispose
        m_disposeQueue.Enqueue(std::make_pair(m_newMesh, m_newMeshCollider));
    }
    
    m_newMesh = mesh;
    m_newMeshCollider = collider;
    m_uploadType = uploadType;
}

void VoxelChunk::OnBeginRender(const int meshIndex)
{
    if (m_mesh == nullptr)
        return;

    const auto& sections = m_mesh->GetSections();
    const auto& textures = sections[meshIndex].textures;

    // Set textures
    m_model->Material->SetTextureArray(0, textures.cbNear);
}

void VoxelChunk::OnCreate()
{
    MAIN_THREAD_ONLY();
    ASSERT(m_model == nullptr);

    m_model = ModelRenderingSystem::AcquireModelComponent(true);
    m_model->Transform = &m_transform;
    m_model->Bounds = BoundingSphere(m_transform.translation, Math::Sqrt(float(m_octreeNode->Size())) * 0.5f);
    m_model->Material = VoxelMaterialManager::GetMainMaterial();

    // Bind OnBeginRender function callback
    m_model->OnBeginRender = Action<void, int>::New<VoxelChunk, &VoxelChunk::OnBeginRender>(this);
}

void VoxelChunk::OnDestroy()
{
    MAIN_THREAD_ONLY();
    ASSERT(m_model);

    // Dispose all queued meshes
    std::pair<RefPtr<VoxelChunkMesh>, MeshCollider*> toDispose;
    while (m_disposeQueue.TryDequeue(toDispose))
    {
        if (toDispose.first)
            toDispose.first.reset();
        if (toDispose.second)
            Object::Destroy(toDispose.second);
    }

    // Reset mesh pointers
    m_mesh.reset();
    m_newMesh.reset();

    if(m_meshCollider)
    {
        Object::Destroy(m_meshCollider);
        m_meshCollider = nullptr;
    }

    if(m_newMeshCollider)
    {
        Object::Destroy(m_newMeshCollider);
        m_newMeshCollider = nullptr;
    }

    ModelRenderingSystem::ReleaseModelComponent(m_model);
    m_model = nullptr;

    if(m_chunkData->HasData())
    {
        // This will mark the chunk data as unused, so it can be reused.
        // Also this data will be deleted after some time.
        m_voxelObject->Storage()->ReleaseChunkData(m_chunkData);
    }
    else
    {
        // This will delete the chunk data
        m_voxelObject->Storage()->FreeChunkData(m_chunkData);
        m_chunkData.reset();
    }
    m_chunkData = nullptr;
}

void VoxelChunk::Generate(IVoxelMesher* mesher)
{
    CPU_PROFILE_FUNCTION(0);

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
    CPU_PROFILE_FUNCTION(0);

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
    mesher->Generate(m_transform.translation, m_lod, borders, voxelData);

    // Check if we have any triangles and if don't, 
    // then we are going to clean everything up, including chunk data.
    if (!mesher->HasTriangles())
    {
        // Mark chunk data as no-surface, and dealloc it's data if it contains any data, NO, DON'T DEALLOC!!! (This is handled by VoxelChunkCache)
        m_chunkData->HasSurface(false);

        // Set upload type to CLEAR-MESH
        SetUpload(nullptr, nullptr, UploadType::Clear);
        return;
    }

    // We have at least one valid triangle,
    // so create new mesh now and set upload state.

    // Create new mesh
    RefPtr<VoxelChunkMesh> mesh(new VoxelChunkMesh());
    const auto collider = Object::New<MeshCollider>();

    // Apply mesh and collision
    if(!mesher->Apply(mesh, collider))
    {
        // We've failed to apply the mesh... Release all resources and set upload to Clear
        m_chunkData->HasSurface(false);

        // Reset now
        mesh.reset();

        // Destroy the collider
        Object::DestroyNow(collider);

        // Set upload type to CLEAR-MESH
        SetUpload(nullptr, nullptr, UploadType::Clear);
        return;
    }

    // Upload new mesh
    mesh->UploadNow();

    // Set upload
    SetUpload(mesh, collider, UploadType::Swap);
}

void VoxelChunk::SetPhysicsState(const bool hasCollision)
{
    ASSERT(m_meshCollider);

    if (m_collisionAttached != hasCollision)
    {
        if(hasCollision)
        {
            m_voxelObject->RigidBody()->AttachCollider(m_meshCollider);
            m_voxelObject->RigidBody()->CollisionLayer(0x1);
        }
        else
        {
            m_voxelObject->RigidBody()->DetachCollider(m_meshCollider);
        }

        // Update flag
        m_collisionAttached = hasCollision;

    }
}

void VoxelChunk::SetVisible(const bool isVisible)
{
    m_model->Active = isVisible;

    if (m_meshCollider)
        SetPhysicsState(isVisible);
}

uint64_t VoxelChunk::CalculateChunkId(const Vector3& position)
{
    // TODO: Assert shit out of this (bounds check and sign check)
    // TODO: Convert from Vector3d world-space to object-space
    // TODO: Fix sign (is it broken anyway...?)

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
