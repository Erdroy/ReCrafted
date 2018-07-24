// ReCrafted (c) 2016-2018 Always Too Late

#include "SpaceObjectChunk.h"
#include "SpaceObjectOctree.h"
#include "SpaceObjectOctreeNode.h"
#include "SpaceObject.h"
#include "Graphics/Mesh.h"
#include "Graphics/Camera.h"
#include "Graphics/Graphics.h"
#include "Storage/VoxelStorage.h"
#include "Meshing/IVoxelMesher.h"

void SpaceObjectChunk::SetUpload(RefPtr<Mesh> mesh, UploadType uploadType)
{
    // Lock Upload
    ScopeLock(m_uploadLock);

    // Set upload type to UPLOAD-MESH
    m_newMesh = mesh;
    m_uploadType = uploadType;
}

void SpaceObjectChunk::Init(SpaceObjectOctreeNode* node, SpaceObject* spaceObject)
// WARNING: this function is called on WORKER THREAD!
{
    this->spaceObject = spaceObject;

    // Set owner
    owner = node;

    var storage = spaceObject->GetStorage();

    // create chunk data
    cvar nodePosition = node->GetPosition();
    m_chunkData = storage->CreateChunkData(nodePosition, node->GetSize(), node->GetDepth());

    // Reset cache
    m_chunkData->ResetCache();

    // calculate chunk position (origin)
    cvar positionOffset = Vector3::One() * static_cast<float>(node->GetSize()) * 0.5f;
    m_position = node->GetPosition() - positionOffset; // lower-left-back corner

    // calculate lod
    m_lod = int(node->GetSize() / float(SpaceObjectOctreeNode::MinimumNodeSize));

    // calculate id
    m_id = CalculateChunkId(nodePosition);
}

void SpaceObjectChunk::Generate(IVoxelMesher* mesher) // WARNING: this function is called on WORKER THREAD!
{
    var storage = spaceObject->GetStorage();

    // try to read chunk data (if not read actually)
    if (!m_chunkData->IsLoaded())
        storage->ReadChunkData(m_chunkData);

    // Reset cache
    m_chunkData->ResetCache();

    if(!m_chunkData->HasSurface() || !m_chunkData->HasData())
        return;

    Rebuild(mesher);
}

void SpaceObjectChunk::Rebuild(IVoxelMesher* mesher)
{
    if (!m_chunkData->IsLoaded() || !m_chunkData->HasSurface())
        return;

    ASSERT(m_newMesh == nullptr);

    cvar borders = 0xFF; // All borders TODO: Build border mask from node
    cvar voxelData = m_chunkData->GetData();

    // Try to generate mesh data
    mesher->Generate(m_position, m_lod, borders, voxelData);

    // Check if we have any triangles and if don't, 
    // then we are going to clean everything up, including chunk data.
    if(!mesher->HasTriangles())
    {
        // Mark chunk data as no-surface, and dealloc it's data if it contains any data, NO, DON'T DEALLOC!!! (This is handled by VoxelChunkCache)
        m_chunkData->HasSurface(false);

        // Set upload type to CLEAR-MESH
        SetUpload(nullptr, ClearMesh);
        return;
    }

    // We have at least one valid triangle,
    // so create new mesh now and set upload state.

    // Create new mesh
    cvar mesh = Mesh::CreateMesh();

    // Apply mesh
    mesher->Apply(mesh);

    // Upload new mesh
    mesh->UploadNow();

    SetUpload(mesh, SwapMesh);
}

void SpaceObjectChunk::Upload()
{
    ASSERT(IS_MAIN_THREAD());
    ASSERT(NeedsUpload());

    // Lock Upload
    //ScopeLock(m_uploadLock);

    switch (m_uploadType)
    {
    case SwapMesh:
    {
        // Dispose old mesh and swap mesh (m_newMesh will be nullptr - because m_mesh is nullptr (SafeDispose))
        SafeDispose(m_mesh);
        m_mesh.swap(m_newMesh);
        break;
    }
    case UploadMesh:
    {
        break;
    }
    case ClearMesh:
    {
        SafeDispose(m_mesh);
        SafeDispose(m_newMesh);
        break;
    }
    default: break;
    }

    // Clean upload type
    m_uploadType = None;
}

void SpaceObjectChunk::Draw()
{
    if (m_mesh)
        Graphics::GetInstance()->Draw(m_mesh, spaceObject->m_terrainShader);
}

void SpaceObjectChunk::Dispose()
{
    ASSERT(IS_MAIN_THREAD());

    SafeDispose(m_mesh);
    SafeDispose(m_newMesh);

    // Release chunk data - when it has data
    // Free chunk data - when it has no any data
    if(m_chunkData->HasSurface() && m_chunkData->HasData())
    {
        cvar storage = spaceObject->GetStorage();
        storage->ReleaseChunkData(m_chunkData); // NOTE: This will not free this chunk data now but later
        m_chunkData = nullptr;
    }
    else
    {
        cvar storage = spaceObject->GetStorage();
        storage->FreeChunkData(m_chunkData);
        m_chunkData = nullptr;
    }
}

uint64_t SpaceObjectChunk::CalculateChunkId(const Vector3& position)
{
    // limit: 8 388 608 (world space) / 1 048 576 (node space)
    // size: 20 bit's per integer

    // convert components into integers
    cvar posX = static_cast<uint64_t>(position.x / 8.0f);
    cvar posY = static_cast<uint64_t>(position.y / 8.0f);
    cvar posZ = static_cast<uint64_t>(position.z / 8.0f);

    // mask-off the last 20 bits and pack into uint64
    var chunkId = (posX & 0xFFFFF) << 40;
    chunkId |= (posY & 0xFFFFF) << 20;
    chunkId |= posZ & 0xFFFFF;

    return chunkId;
}
