// ReCrafted (c) 2016-2018 Always Too Late

#include "SpaceObjectChunk.h"
#include "SpaceObjectOctree.h"
#include "SpaceObjectOctreeNode.h"
#include "SpaceObject.h"
#include "Meshing/MarchingCubes/MCMesher.h"
#include "Graphics/Mesh.h"
#include "Graphics/Camera.h"
#include "Graphics/Graphics.h"
#include "Storage/VoxelStorage.h"

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
    this->node = node;

    var storage = spaceObject->GetStorage();

    // create chunk data
    var nodePosition = node->GetPosition();
    m_chunkData = storage->CreateChunkData(nodePosition, node->GetSize(), node->GetDepth());

    // calculate chunk position (origin)
    cvar positionOffset = Vector3::One() * static_cast<float>(node->GetSize()) * 0.5f;
    m_position = node->GetPosition() - positionOffset; // lower-left-back corner

    // calculate lod
    m_lod = int(node->GetSize() / float(SpaceObjectOctreeNode::MinimumNodeSize));

    // calculate id
    m_id = CalculateChunkId(node->GetPosition());
}

void SpaceObjectChunk::Generate(IVoxelMesher* mesher) // WARNING: this function is called on WORKER THREAD!
{
    var storage = spaceObject->GetStorage();

    // try to read chunk data (if not read actually)
    if (!m_chunkData->IsLoaded())
        storage->ReadChunkData(m_chunkData);

    if(!m_chunkData->HasSurface())
        return;

    ASSERT(m_chunkData->HasData());

    Rebuild(mesher);
}

void SpaceObjectChunk::Rebuild(IVoxelMesher* mesher)
{
    if (!m_chunkData->IsLoaded() || !m_chunkData->HasSurface())
        return;

    ASSERT(m_newMesh == nullptr);

    cvar borders = 0x0; // All borders TODO: Build border mask from node
    cvar voxelData = m_chunkData->GetData();

    // Try to generate mesh data
    mesher->Generate(m_position, m_lod, borders, voxelData);

    // Check if we have any triangles and if don't, 
    // then we are going to clean everything up, including chunk data.
    if(!mesher->HasTriangles())
    {
        // Mar chunk data as no-surface, and dealloc it's data if it contains any data
        m_chunkData->HasSurface(false);

        if (m_chunkData->HasData())
            m_chunkData->DeallocateData();

        // Set upload type to CLEAR-MESH
        SetUpload(nullptr, ClearMesh);
        return;
    }

    // We have at least one valid triangle,
    // so create new mesh now and set upload state.

    // Create and apply mesh
    cvar mesh = Mesh::CreateMesh();
    mesher->Apply(mesh);

    SetUpload(mesh, UploadMesh);
}

void SpaceObjectChunk::Upload()
{
    ASSERT(IS_MAIN_THREAD());
    ASSERT(NeedsUpload());

    // Lock Upload
    ScopeLock(m_uploadLock);

    switch (m_uploadType)
    {
    case UploadMesh:
    {
        // Upload new mesh
        m_newMesh->Upload();

        // Dispose old mesh and swap mesh (m_newMesh will be nullptr - because m_mesh is nullptr (SafeDispose))
        SafeDispose(m_mesh);
        m_mesh.swap(m_newMesh);
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
        Graphics::GetInstance()->Draw(m_mesh);
}

void SpaceObjectChunk::Dispose()
{
    SafeDispose(m_mesh);
    SafeDispose(m_newMesh);
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
