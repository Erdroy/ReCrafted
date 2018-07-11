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
#include "Common/Profiler/Profiler.h"

void VoxelStorage::LoadHeader()
{
    // create chunk cache
    m_chunkCache.reset(new VoxelChunkCache);

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
        Logger::LogError("getVoxelsRow for pregens is not implemented!");
        return;
    }

    // load header
    LoadHeader();
}

void VoxelStorage::Update()
{
    Profiler::BeginProfile("SpaceObjectOctree::Update");
    m_chunkCache->Update();
    Profiler::EndProfile();
}

void VoxelStorage::Dispose()
{
    // Dispose chunk cache
    SafeDispose(m_chunkCache);

    // release all resources
    SafeDisposeNN(m_vxhStream);
    SafeDelete(m_vxhStream);
    SafeDelete(m_vxh);
}

RefPtr<VoxelChunkData> VoxelStorage::CreateChunkData(Vector3& nodePosition, const int nodeSize, const int nodeDepth)
{
    cvar chunkId = SpaceObjectChunk::CalculateChunkId(nodePosition);
    
    var chunk = m_chunkCache->TryGetChunk(chunkId);

    if (chunk)
        return chunk;

    chunk.reset(new VoxelChunkData());
    chunk->m_size = nodeSize;
    chunk->m_id = chunkId;
    chunk->m_nodeDepth = nodeDepth;
    chunk->m_nodePosition = nodePosition;
    chunk->m_chunkPosition = nodePosition - Vector3::One() * float(nodeSize) * 0.5f;

    // Add chunk to cache
    m_chunkCache->AddChunk(chunk);

    return chunk;
}

RefPtr<VoxelChunkData> VoxelStorage::GetChunkData(Vector3& nodePosition)
{
    cvar chunkId = SpaceObjectChunk::CalculateChunkId(nodePosition);
    return m_chunkCache->TryGetChunk(chunkId);
}

void VoxelStorage::ReadChunkData(const RefPtr<VoxelChunkData>& chunkData)
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
    cvar hasSurface = spaceObject->GetGenerator()->GenerateChunkData(chunkData, chunkPosition, lod, chunkData->m_nodeDepth);

    // mark as loaded
    chunkData->m_hasSurface = hasSurface;
    chunkData->m_loaded = true;
}

void VoxelStorage::WriteChunkData(RefPtr<VoxelChunkData> chunkData)
{
    // TODO: check main thread
    // TODO: chunk save system
}

void VoxelStorage::FreeChunkData(RefPtr<VoxelChunkData> chunkData)
{
    ASSERT(chunkData);

    m_chunkCache->RemoveChunk(chunkData);
}

void VoxelStorage::ReleaseChunkData(RefPtr<VoxelChunkData> chunkData)
{
    ASSERT(chunkData);

    m_chunkCache->OnChunkReleased(chunkData);
}
