// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "VoxelStorage.h"

#include "Profiler/Profiler.h"
#include "Voxels/Generator/VoxelGenerator.h"
#include "Voxels/VoxelObjectBase.h"
#include "Voxels/VoxelChunkCache.h"
#include "Voxels/VoxelChunk.h"

VoxelStorage::VoxelStorage(VoxelObjectBase* voxelObject)
{
    m_voxelObject = voxelObject;

    // Create chunk cache
    m_chunkCache.reset(new VoxelChunkCache);
}

VoxelStorage::~VoxelStorage()
{
    // Dispose chunk cache
    m_chunkCache.reset();
}

void VoxelStorage::Update()
{
    CPU_PROFILE_FUNCTION(0);

    m_chunkCache->Update();
}

RefPtr<VoxelChunkData> VoxelStorage::CreateChunkData(const Vector3& nodePosition, const int nodeSize, const int nodeDepth)
{
    const auto chunkId = VoxelChunk::CalculateChunkId(nodePosition);

    auto chunk = m_chunkCache->TryGetChunk(chunkId);

    if (chunk)
        return chunk;

    chunk.reset(new VoxelChunkData());
    chunk->m_size = nodeSize;
    chunk->m_id = chunkId;
    chunk->m_nodeDepth = nodeDepth;
    chunk->m_nodePosition = nodePosition;
    chunk->m_chunkPosition = nodePosition - Vector3::One * float(nodeSize) * 0.5f;

    // Add chunk to cache
    m_chunkCache->AddChunk(chunk);

    return chunk;
}

RefPtr<VoxelChunkData> VoxelStorage::GetChunkData(const Vector3& nodePosition)
{
    const auto chunkId = VoxelChunk::CalculateChunkId(nodePosition);
    return m_chunkCache->TryGetChunk(chunkId);
}

void VoxelStorage::ReadChunkData(const RefPtr<VoxelChunkData>& chunkData)
{
    // TODO: check if we have some data saved, if so, read from file

    const auto nodePosition = chunkData->m_nodePosition;
    const auto nodeSize = chunkData->m_size;
    const auto positionOffset = Vector3::One * float(nodeSize) * 0.5f;
    const auto chunkPosition = nodePosition - positionOffset; // Lower-Left-Back corner
    const auto lod = chunkData->m_size / VoxelObjectOctree::Node::MinimumNodeSize;

    // Generate chunk now using CHM
    const auto hasSurface = m_voxelObject->Generator()->GenerateChunkData(chunkData, Vector3d(double(chunkPosition.x), double(chunkPosition.y), double(chunkPosition.z)), lod, chunkData->m_nodeDepth);

    // Mark as loaded
    chunkData->m_hasSurface = hasSurface;
    chunkData->m_loaded = true;
}

void VoxelStorage::WriteChunkData(const RefPtr<VoxelChunkData>& chunkData)
{
    // TODO: check main thread
    // TODO: chunk save system
}

void VoxelStorage::FreeChunkData(const RefPtr<VoxelChunkData>& chunkData)
{
    ASSERT(chunkData);

    m_chunkCache->RemoveChunk(chunkData);
}

void VoxelStorage::ReleaseChunkData(const RefPtr<VoxelChunkData>& chunkData)
{
    ASSERT(chunkData);

    m_chunkCache->OnChunkReleased(chunkData);
}
