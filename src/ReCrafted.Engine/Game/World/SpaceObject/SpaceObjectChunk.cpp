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

uint8_t SpaceObjectChunk::getLodBorders()
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

void SpaceObjectChunk::init(SpaceObjectOctreeNode* node, SpaceObject* spaceObject)
// WARNING: this function is called on WORKER THREAD!
{
    this->spaceObject = spaceObject;
    this->node = node;

    var storage = spaceObject->getStorage();

    // create chunk data
    var nodePosition = node->get_position();
    m_chunkData = storage->createChunkData(nodePosition, node->get_size());

    // calculate chunk position (origin)
    cvar positionOffset = Vector3::one() * static_cast<float>(node->get_size()) * 0.5f;
    m_position = node->get_position() - positionOffset; // lower-left-back corner

    // calculate lod
    m_lod = int(node->get_size() / float(SpaceObjectOctreeNode::MinimumNodeSize));

    // calculate id
    m_id = calculateChunkId(node->get_position());

    // TODO: check if chunk has any visible surface
}

void SpaceObjectChunk::generate(IVoxelMesher* mesher) // WARNING: this function is called on WORKER THREAD!
{
    var storage = spaceObject->getStorage();

    // try to read chunk data (if not read actually)
    if (!m_chunkData->isLoaded())
        storage->readChunkData(m_chunkData);

    if(!m_chunkData->HasSurface())
        return;

    rebuild(mesher);
}

void SpaceObjectChunk::rebuild(IVoxelMesher* mesher)
{
    if (!m_chunkData->isLoaded() || !m_chunkData->HasSurface())
        return;

    var mesh = Mesh::createMesh();

    cvar borders = getLodBorders();
    cvar voxelData = m_chunkData->getData();

    mesher->generate(m_position, m_lod, borders, mesh, voxelData);

    if(mesh->getVertexCount() == 0)
    {
        SafeDispose(mesh);
        return;
    }

    ScopeLock(m_meshLock);
    SafeDispose(m_newMesh);
    m_newMesh = mesh;
}

void SpaceObjectChunk::upload()
{
    ScopeLock(m_meshLock);

    // upload changes
    if (m_newMesh && m_newMesh->canUpload())
    {
        if (m_mesh)
            SafeDispose(m_mesh)

        m_newMesh->upload();
        m_mesh = m_newMesh;
        m_newMesh = nullptr;
    }
}

void SpaceObjectChunk::draw()
{
    if (!m_mesh || !m_mesh->isUploaded())
        return;

    Graphics::getInstance()->draw(m_mesh);
}

void SpaceObjectChunk::dispose()
{
    SafeDispose(m_mesh);
}

uint64_t SpaceObjectChunk::calculateChunkId(const Vector3& position)
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
