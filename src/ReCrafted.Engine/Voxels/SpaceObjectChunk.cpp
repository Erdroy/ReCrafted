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

uint8_t SpaceObjectChunk::GetLodBorders()
{
    /*uint8_t borders = 0u;

    if (node->hasNeighLowerLoD(NodeDirection::Front))
        borders |= BORDER_FRONT;

    if (node->hasNeighLowerLoD(NodeDirection::Back))
        borders |= BORDER_BACK;

    if (node->hasNeighLowerLoD(NodeDirection::Left))
        borders |= BORDER_LEFT;

    if (node->hasNeighLowerLoD(NodeDirection::Right))
        borders |= BORDER_RIGHT;

    if (node->hasNeighLowerLoD(NodeDirection::Top))
        borders |= BORDER_TOP;

    if (node->hasNeighLowerLoD(NodeDirection::Back))
        borders |= BORDER_BOTTOM;*/

    // TODO: cull not needed skirts

    //return borders;
    return 0xFF;
}

void SpaceObjectChunk::Init(SpaceObjectOctreeNode* node, SpaceObject* spaceObject)
// WARNING: this function is called on WORKER THREAD!
{
    this->spaceObject = spaceObject;
    this->node = node;

    var storage = spaceObject->GetStorage();

    // create chunk data
    var nodePosition = node->GetPosition();
    m_chunkData = storage->CreateChunkData(nodePosition, node->GetSize());

    // calculate chunk position (origin)
    cvar positionOffset = Vector3::One() * static_cast<float>(node->GetSize()) * 0.5f;
    m_position = node->GetPosition() - positionOffset; // lower-left-back corner

    // calculate lod
    m_lod = int(node->GetSize() / float(SpaceObjectOctreeNode::MinimumNodeSize));

    // calculate id
    m_id = CalculateChunkId(node->GetPosition());

    // TODO: check if chunk has any visible surface
}

void SpaceObjectChunk::Generate(IVoxelMesher* mesher) // WARNING: this function is called on WORKER THREAD!
{
    var storage = spaceObject->GetStorage();

    // try to read chunk data (if not read actually)
    if (!m_chunkData->IsLoaded())
        storage->ReadChunkData(m_chunkData);

    if(!m_chunkData->HasSurface())
        return;

    Rebuild(mesher);
}

void SpaceObjectChunk::Rebuild(IVoxelMesher* mesher)
{
    if (!m_chunkData->IsLoaded() || !m_chunkData->HasSurface())
        return;

    var mesh = Mesh::CreateMesh();

    cvar borders = GetLodBorders();
    cvar voxelData = m_chunkData->GetData();

    mesher->Generate(m_position, m_lod, borders, mesh, voxelData);

    if(mesh->GetVertexCount() == 0)
    {
        SafeDispose(mesh);
        return;
    }

    ScopeLock(m_meshLock);
    SafeDispose(m_newMesh);
    m_newMesh = mesh;
}

void SpaceObjectChunk::Upload()
{
    // upload changes
    if (m_newMesh && m_newMesh->CanUpload())
    {
        ScopeLock(m_meshLock);

        // Upload new mesh
        m_newMesh->Upload();

        // Dispose old mesh
        if (m_mesh)
            SafeDispose(m_mesh);

        // Swap mesh
        m_mesh.swap(m_newMesh);
    }
}

void SpaceObjectChunk::Draw()
{
    if (!m_mesh || !m_mesh->IsUploaded())
        return;

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
