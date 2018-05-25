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
    cvar lodSize = LoDTable[lod];

    // the terrain height (over planet, sphere is the base)
    cvar terrainHeight = radius + chm->Sample(position, radius, lod) * hillsHeight;

    // the height over sphere
    cvar sphereHeight = (position - origin).Length();

    // calculate voxel value
    cvar voxelValue = (sphereHeight - terrainHeight) / lodSize;

    // convert voxel value to voxel
    return VOXEL_FROM_FLOAT(voxelValue);
}

void VoxelStorage::GenerateChunkFromCHM(sbyte** voxelData, const Vector3& position, const int lod)
{
    cvar chm = m_chm.get();
    cvar dataSize = VoxelChunkData::ChunkDataSize;
    cvar lod_f = static_cast<float>(lod);

    sbyte last = 0u;
    var first = true;
    var hasSurface = false;

    for (var x = 0; x < dataSize; x++)
    {
        for (var y = 0; y < dataSize; y++)
        {
            for (var z = 0; z < dataSize; z++)
            {
                cvar index = INDEX_3D(x, y, z, dataSize);

                cvar offset = Vector3(float(x), float(y), float(z));
                cvar voxelPosition = position + offset * lod_f;

                cvar value = sdf_planet_generate(chm, spaceObject->m_Position, voxelPosition, lod,
                    spaceObject->m_settings.minSurfaceHeight,
                    spaceObject->m_settings.hillsHeight);

                // tempoarary
                (*voxelData)[index] = value;

                if(last != value && !first)
                    hasSurface = true;

                last = value;
                first = false;
            }
        }
    }

    if (!hasSurface)
        *voxelData = nullptr;
}

void VoxelStorage::LoadHeader()
{
    // create header if needed
    if (!Platform::FileExists(settings.saveName))
    {
        // open voxel header file stream
        m_vxhStream = new FileStream(settings.saveName, OpenMode::OpenWrite);

        // no header is found, create new one
        VoxelStorageHeader header = {};

        m_vxhStream->Write(&header, 0u, sizeof(VoxelStorageHeader));
        m_vxhStream->Flush();
        m_vxhStream->Dispose();
        SafeDisposeNN(m_vxhStream);
        SafeDelete(m_vxhStream);
    }

    // open voxel header file stream
    m_vxhStream = new FileStream(settings.saveName, OpenMode::OpenReadWrite);

    // load VoxelStorageHeader
    m_vxh = new VoxelStorageHeader();
    m_vxhStream->Read(m_vxh, 0u, sizeof(VoxelStorageHeader));

    // read chunk map if needed
    if (m_vxh->chunkMapSize > 0u)
    {
        cvar length = m_vxh->chunkMapSize * sizeof(VoxelStorageChunkEntry);


        //m_vxhMap = new VoxelStorageChunkEntry[m_vxh->chunkMapSize];
        //m_vxhStream->read(m_vxhMap, sizeof(VoxelStorageHeader), Length);
    }
}

void VoxelStorage::SaveHeader()
{
    cvar length = m_vxh->chunkMapSize * sizeof(VoxelStorageChunkEntry);

    // write header
    m_vxhStream->Write(m_vxh, 0u, sizeof(VoxelStorageHeader));
    //m_vxhStream->read(m_vxhMap, sizeof(VoxelStorageHeader), Length);
}

void VoxelStorage::Init(SpaceObjectSettings& settings)
{
    this->settings = settings;

    if (settings.generationType == GenerationType::PreGenerated)
    {
        // TODO: load pregen (VoxelPregen class neded) [*.rcv file]
        Logger::LogError("getVoxelsRow for pregens is not implemented!");
        return;
    }

    // load header
    LoadHeader();

    m_chm = VoxelCHM::LoadFromDirectory(settings.fileName);
}

void VoxelStorage::Dispose()
{
    m_voxelChunks.clear();

    // release all resources
    SafeDisposeNN(m_vxhStream);
    SafeDelete(m_vxhStream);
    SafeDelete(m_vxh);
}

Ref<VoxelChunkData> VoxelStorage::CreateChunkData(Vector3& nodePosition, const int nodeSize)
{
    ScopeLock(m_voxelChunksLock);

    cvar chunkId = SpaceObjectChunk::CalculateChunkId(nodePosition);

    if (m_voxelChunks.contains(chunkId))
        return m_voxelChunks[chunkId];

    var chunk = std::make_shared<VoxelChunkData>();
    chunk->m_size = nodeSize;
    chunk->m_id = chunkId;
    chunk->m_nodePosition = nodePosition;
    chunk->m_chunkPosition = nodePosition - Vector3::One() * float(nodeSize) * 0.5f;

    m_voxelChunks[chunkId] = chunk;

    return chunk;
}

Ref<VoxelChunkData> VoxelStorage::GetChunkData(Vector3& nodePosition)
{
    ScopeLock(m_voxelChunksLock);

    cvar chunkId = SpaceObjectChunk::CalculateChunkId(nodePosition);

    if (!m_voxelChunks.contains(chunkId))
        return {};

    return m_voxelChunks[chunkId];
}

void VoxelStorage::ReadChunkData(Ref<VoxelChunkData> chunkData)
{
    if (settings.generationType == GenerationType::PreGenerated)
    {
        // TODO: pregen support
        Logger::LogError("VoxelStorage::readChunkData for pre generated data is not implemented, yet!");
        return;
    }

    // TODO: check if we have some data saved, if so, read from file

    cvar nodePosition = chunkData->m_nodePosition;
    cvar nodeSize = chunkData->m_size;
    cvar positionOffset = Vector3::One() * float(nodeSize) * 0.5f;
    cvar chunkPosition = nodePosition - positionOffset; // lower-left-back corner
    cvar lod = chunkData->m_size / SpaceObjectOctreeNode::MinimumNodeSize;

    // generate chunk now using CHM
    var data = chunkData->GetData();
    GenerateChunkFromCHM(&data, chunkPosition, lod);

    // mark as loaded
    chunkData->m_hasSurface = data != nullptr;
    chunkData->m_loaded = true;

    // TODO: improve our surface detection by checking for modified chunk data - if there is none, chunkData should be null.
    // When we want to modify the terrain, we should at first create chunk data.
}

void VoxelStorage::WriteChunkData(Ref<VoxelChunkData> chunkData)
{
    // TODO: check main thread
    // TODO: chunk save system
}

void VoxelStorage::FreeChunkData(Ref<VoxelChunkData> chunkData)
{
    ScopeLock(m_voxelChunksLock);

    cvar chunkId = chunkData->m_id;
    // TODO: free chunk data and release space in hashmap
}
