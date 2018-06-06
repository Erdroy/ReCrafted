// ReCrafted (c) 2016-2018 Always Too Late

#include "VoxelStorage.h"
#include "VoxelStorageHeader.h"
#include "VoxelStorageChunkEntry.h"

#include "Core/Logger.h"
#include "Core/Math/Math.h"
#include "Core/Streams/FileStream.h"
#include "Core/Streams/BinaryStream.h"

#include "Voxels/SpaceObjectChunk.h"
#include "Voxels/SpaceObjectSettings.h"
#include "Voxels/Generator/VoxelGenerator.h"

void VoxelStorage::LoadHeader()
{
    // create header if needed
    if (!Platform::FileExists(settings->saveName))
    {
        // open voxel header file stream
        m_vxhStream = new FileStream(settings->saveName, OpenMode::OpenWrite);

        // no header is found, create new one
        VoxelStorageHeader header = {};

        m_vxhStream->Write(&header, 0u, sizeof(VoxelStorageHeader));
        m_vxhStream->Flush();
        m_vxhStream->Dispose();
        SafeDisposeNN(m_vxhStream);
        SafeDelete(m_vxhStream);
    }

    // open voxel header file stream
    m_vxhStream = new FileStream(settings->saveName, OpenMode::OpenReadWrite);

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

void VoxelStorage::Init(SpaceObjectSettings* settings)
{
    this->settings = settings;

    if (settings->generationType == GenerationType::PreGenerated)
    {
        // TODO: load pregen (VoxelPregen class neded) [*.rcv file]
        Logger::LogError("getVoxelsRow for pregens is not implemented!");
        return;
    }

    // load header
    LoadHeader();
}

void VoxelStorage::Dispose()
{
    m_voxelChunks.clear();

    // release all resources
    SafeDisposeNN(m_vxhStream);
    SafeDelete(m_vxhStream);
    SafeDelete(m_vxh);
}

RefPtr<VoxelChunkData> VoxelStorage::CreateChunkData(Vector3& nodePosition, const int nodeSize, const int nodeDepth)
{
    ScopeLock(m_voxelChunksLock);

    cvar chunkId = SpaceObjectChunk::CalculateChunkId(nodePosition);

    if (m_voxelChunks.contains(chunkId))
        return m_voxelChunks[chunkId];

    RefPtr<VoxelChunkData> chunk(new VoxelChunkData());
    chunk->m_size = nodeSize;
    chunk->m_id = chunkId;
    chunk->m_nodeDepth = nodeDepth;
    chunk->m_nodePosition = nodePosition;
    chunk->m_chunkPosition = nodePosition - Vector3::One() * float(nodeSize) * 0.5f;

    m_voxelChunks[chunkId] = chunk;

    return chunk;
}

RefPtr<VoxelChunkData> VoxelStorage::GetChunkData(Vector3& nodePosition)
{
    ScopeLock(m_voxelChunksLock);

    cvar chunkId = SpaceObjectChunk::CalculateChunkId(nodePosition);

    if (!m_voxelChunks.contains(chunkId))
        return {};

    return m_voxelChunks[chunkId];
}

void VoxelStorage::ReadChunkData(RefPtr<VoxelChunkData> chunkData)
{
    if (settings->generationType == GenerationType::PreGenerated)
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
    cvar hasSurface = spaceObject->GetGenerator()->GenerateChunkData(chunkData->GetData(), chunkPosition, lod, chunkData->m_nodeDepth);

    // mark as loaded
    chunkData->m_hasSurface = hasSurface;
    chunkData->m_loaded = true;

    // TODO: improve our surface detection by checking for modified chunk data - if there is none, chunkData should be null.
    // When we want to modify the terrain, we should at first create chunk data.
}

void VoxelStorage::WriteChunkData(RefPtr<VoxelChunkData> chunkData)
{
    // TODO: check main thread
    // TODO: chunk save system
}

void VoxelStorage::FreeChunkData(RefPtr<VoxelChunkData> chunkData)
{
    ScopeLock(m_voxelChunksLock);

    cvar chunkId = chunkData->m_id;
    // TODO: free chunk data and release space in hashmap
}
