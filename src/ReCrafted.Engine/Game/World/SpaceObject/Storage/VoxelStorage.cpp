// ReCrafted (c) 2016-2018 Always Too Late

#include "VoxelStorage.h"
#include "VoxelStorageHeader.h"
#include "VoxelStorageChunkEntry.h"

#include "Core/Logger.h"
#include "Core/Math/Math.h"
#include "Core/Streams/FileStream.h"
#include "Core/Streams/BinaryStream.h"

#include "../SpaceObjectChunk.h"
#include "../SpaceObjectSettings.h"
#include "../SpaceObjectTables.hpp"
#include "../Generator/VoxelCHM.h"
#include "../Utilities/VoxelUtils.h"

sbyte VoxelStorage::sdf_planet_generate(VoxelCHM* chm, const Vector3& origin, const Vector3& position, const int lod,
                                        const float radius, const float hillsHeight)
{
    // calculate current voxel size
    cvar lodSize = lodtable[lod];

    // the terrain height (over planet, sphere is the base)
    cvar terrainHeight = radius + chm->sample(position, radius, lod) * hillsHeight;

    // the height over sphere
    cvar sphereHeight = (position - origin).length();

    // calculate voxel value
    cvar voxelValue = (sphereHeight - terrainHeight) / lodSize;

    // convert voxel value to voxel
    return VOXEL_FROM_FLOAT(voxelValue);
}

void VoxelStorage::generateChunkFromCHM(sbyte* voxelData, const Vector3& position, const int lod)
{
    cvar chm = m_chm.get();
    cvar dataSize = VoxelChunkData::ChunkDataSize;
    cvar lod_f = static_cast<float>(lod);

    // TODO: return nullptr where there is no any proper voxel surface (the chunk is completely above or under surface)

    for (var x = 0; x < dataSize; x++)
    {
        for (var y = 0; y < dataSize; y++)
        {
            for (var z = 0; z < dataSize; z++)
            {
                cvar index = INDEX_3D(x, y, z, dataSize);

                cvar offset = Vector3(float(x), float(y), float(z));
                cvar voxelPosition = position + offset * lod_f;

                // tempoarary
                voxelData[index] = sdf_planet_generate(chm, spaceObject->m_position, voxelPosition, lod,
                                                       spaceObject->m_settings.minSurfaceHeight,
                                                       spaceObject->m_settings.hillsHeight);
            }
        }
    }
}

void VoxelStorage::loadHeader()
{
    // create header if needed
    if (!Platform::fileExists(settings.saveName))
    {
        // open voxel header file stream
        m_vxhStream = new FileStream(settings.saveName, OpenMode::OpenWrite);

        // no header is found, create new one
        VoxelStorageHeader header = {};

        m_vxhStream->write(&header, 0u, sizeof(VoxelStorageHeader));
        m_vxhStream->flush();
        m_vxhStream->dispose();
        SafeDisposeNN(m_vxhStream);
        SafeDelete(m_vxhStream);
    }

    // open voxel header file stream
    m_vxhStream = new FileStream(settings.saveName, OpenMode::OpenReadWrite);

    // load VoxelStorageHeader
    m_vxh = new VoxelStorageHeader();
    m_vxhStream->read(m_vxh, 0u, sizeof(VoxelStorageHeader));

    // read chunk map if needed
    if (m_vxh->chunkMapSize > 0u)
    {
        cvar length = m_vxh->chunkMapSize * sizeof(VoxelStorageChunkEntry);


        //m_vxhMap = new VoxelStorageChunkEntry[m_vxh->chunkMapSize];
        //m_vxhStream->read(m_vxhMap, sizeof(VoxelStorageHeader), length);
    }
}

void VoxelStorage::saveHeader()
{
    cvar length = m_vxh->chunkMapSize * sizeof(VoxelStorageChunkEntry);

    // write header
    m_vxhStream->write(m_vxh, 0u, sizeof(VoxelStorageHeader));
    //m_vxhStream->read(m_vxhMap, sizeof(VoxelStorageHeader), length);
}

void VoxelStorage::init(SpaceObjectSettings& settings)
{
    this->settings = settings;

    if (settings.generationType == GenerationType::PreGenerated)
    {
        // TODO: load pregen (VoxelPregen class neded) [*.rcv file]
        Logger::logError("getVoxelsRow for pregens is not implemented!");
        return;
    }

    // load header
    loadHeader();

    m_chm = VoxelCHM::loadFromDirectory(settings.fileName);
}

void VoxelStorage::dispose()
{
    m_voxelChunks.clear();

    // release all resources
    SafeDisposeNN(m_vxhStream);
    SafeDelete(m_vxhStream);
    SafeDelete(m_vxh);
}

Ref<VoxelChunkData> VoxelStorage::createChunkData(Vector3& nodePosition, const int nodeSize)
{
    ScopeLock(m_voxelChunksLock);

    cvar chunkId = SpaceObjectChunk::calculateChunkId(nodePosition);

    if (m_voxelChunks.contains(chunkId))
        return m_voxelChunks[chunkId];

    var chunk = std::make_shared<VoxelChunkData>();
    chunk->m_size = nodeSize;
    chunk->m_id = chunkId;
    chunk->m_nodePosition = nodePosition;
    chunk->m_chunkPosition = nodePosition - Vector3::one() * float(nodeSize) * 0.5f;

    m_voxelChunks[chunkId] = chunk;

    return chunk;
}

Ref<VoxelChunkData> VoxelStorage::getChunkData(Vector3& nodePosition)
{
    ScopeLock(m_voxelChunksLock);

    cvar chunkId = SpaceObjectChunk::calculateChunkId(nodePosition);

    if (!m_voxelChunks.contains(chunkId))
        return {};

    return m_voxelChunks[chunkId];
}

void VoxelStorage::readChunkData(Ref<VoxelChunkData> chunkData)
{
    if (settings.generationType == GenerationType::PreGenerated)
    {
        // TODO: pregen support
        Logger::logError("VoxelStorage::readChunkData for pre generated data is not implemented, yet!");
        return;
    }

    // TODO: check if we have some data saved, if so, read from file

    cvar nodePosition = chunkData->m_nodePosition;
    cvar nodeSize = chunkData->m_size;
    cvar positionOffset = Vector3::one() * float(nodeSize) * 0.5f;
    cvar chunkPosition = nodePosition - positionOffset; // lower-left-back corner
    cvar lod = chunkData->m_size / SpaceObjectOctreeNode::MinimumNodeSize;

    // generate chunk now using CHM
    generateChunkFromCHM(chunkData->getData(), chunkPosition, lod);

    // mark as loaded
    chunkData->m_loaded = true;
}

void VoxelStorage::writeChunkData(Ref<VoxelChunkData> chunkData)
{
    // TODO: check main thread
    // TODO: chunk save system
}

void VoxelStorage::freeChunkData(Ref<VoxelChunkData> chunkData)
{
    ScopeLock(m_voxelChunksLock);

    cvar chunkId = chunkData->m_id;
    // TODO: free chunk data and release space in hashmap
}
