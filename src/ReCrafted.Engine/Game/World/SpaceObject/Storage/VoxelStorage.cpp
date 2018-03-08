// ReCrafted (c) 2016-2018 Always Too Late

#include "VoxelStorage.h"
#include "VoxelStorageHeader.h"
#include "VoxelStorageChunkEntry.h"

#include "Core/Logger.h"
#include "Core/Math/Math.h"
#include "Core/Streams/FileStream.h"
#include "Core/Streams/BinaryStream.h"

#include "../LODTable.h"
#include "../SpaceObjectChunk.h"
#include "../SpaceObjectSettings.h"
#include "../Generator/VoxelCHM.h"
#include "../Utilities/VoxelUtils.h"

sbyte VoxelStorage::sdf_planet_generate(VoxelCHM* chm, const Vector3& origin, const Vector3& position, const int lod, const float radius, const float hillsHeight)
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

sbyte* VoxelStorage::generateChunkFromCHM(const Vector3& position, const int lod)
{
    cvar chm = m_chm.get();
    cvar dataSize = SpaceObjectChunk::ChunkDataSize;
    cvar lod_f = static_cast<float>(lod);
    cvar data = new sbyte[dataSize * dataSize * dataSize];

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
                data[index] = sdf_planet_generate(chm, spaceObject->m_position, voxelPosition, lod, spaceObject->m_settings.minSurfaceHeight, spaceObject->m_settings.hillsHeight);
            }
        }
    }

    return data;
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
    if(m_vxh->chunkMapSize > 0u)
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

void VoxelStorage::writeChunk(sbyte* chunkData, const Vector3& position, int lod)
{
    ScopeLock(m_vxhLock);

    cvar chunkId = SpaceObjectChunk::calculateChunkId(position);

    if(m_voxelMap.contains(chunkId))
    {
        var pos = m_voxelMap[chunkId];
        return;
    }

    m_voxelMap[chunkId] = lod;

    // TODO: check if this chunk if saved, is so, get the offset
    // TODO: if not saved, find free offset
    // TODO: write chunk to given offset (and file)
}

void VoxelStorage::init(SpaceObjectSettings& settings)
{
    this->settings = settings;

    if(settings.generationType == GenerationType::PreGenerated)
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
    // release all resources
    SafeDisposeNN(m_vxhStream);
    SafeDelete(m_vxhStream);
    SafeDelete(m_vxh);
}

sbyte* VoxelStorage::getVoxelChunk(const Vector3& nodePosition, const Vector3& chunkPosition, const int lod)
{
    if (settings.generationType == GenerationType::PreGenerated)
    {
        // TODO: pregens
        Logger::logError("getVoxelsRow for pregens is not implemented!");
        return nullptr;
    }

    // TODO: check if this chunk is present in storage

    // generate chunk now using CHM
    cvar voxelData = generateChunkFromCHM(chunkPosition, lod);
    writeChunk(nullptr, nodePosition, lod);
    // TODO: VoxelCache class (should store chunks for some time or all the time if chunk is still being used)
    // TODO: check if this is cached (use start and lod) (and not modified by this time) and get if present
    // TODO: if not - generate and modify

    // done
    return voxelData;
}
