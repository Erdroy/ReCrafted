// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "VoxelChunkCache.h"
#include "Profiler/Profiler.h"

void VoxelChunkCache::AddChunkToCache(const RefPtr<VoxelChunkData>& chunk)
{
    ASSERT(chunk);
    ASSERT(!chunk->m_cached);

    ScopeLock(m_cacheLock);

    m_cachedChunks.Add(chunk);

    chunk->m_timeCached = Platform::GetMilliseconds();
    chunk->m_cached = true;
}

void VoxelChunkCache::RemoveChunkFromCache(const RefPtr<VoxelChunkData>& chunk)
{
    ASSERT(chunk);

    ScopeLock(m_cacheLock);

    chunk->m_timeCached = 0.0;
    chunk->m_cached = false;

    m_cachedChunks.Remove(chunk);
}

VoxelChunkCache::~VoxelChunkCache()
{
    m_voxelChunksMap.clear();

    // Free all chunks
    for (auto& chunk : m_voxelChunks)
    {
        if (chunk)
            chunk->DeallocateData();
    }

    // Clear chunk array
    m_voxelChunks.Clear();
}

void VoxelChunkCache::Update()
{
    // Chunks-to-remove queue - NOTE: This function is called only on MAIN-THREAD, 
    // then we can optimize memory usage by using static or member function (Array's memory won't be released)
    static List<RefPtr<VoxelChunkData>> chunksToRemove = {};
    static List<RefPtr<VoxelChunkData>> chunksToReset = {};

    CPU_PROFILE_FUNCTION(0);

    const auto currentTime = Platform::GetMilliseconds();

    // Iterate all chunks to find any not used chunks
    m_cacheLock.LockNow();
    for (auto& chunk : m_cachedChunks)
    {
        ASSERT(chunk);

        if (!chunk->m_cached)
        {
            chunksToReset.Add(chunk);
            continue;
        }

        if (currentTime - chunk->m_timeCached > CacheTime)
            chunksToRemove.Add(chunk);
    }
    m_cacheLock.UnlockNow();

    // Reset un-cached chunks
    for (auto& chunk : chunksToReset)
    {
        RemoveChunkFromCache(chunk);
    }

    // Remove chunks
    RemoveChunks(chunksToRemove);

    // Clear queues
    chunksToRemove.Clear();
    chunksToReset.Clear();
}

void VoxelChunkCache::AddChunk(const RefPtr<VoxelChunkData>& chunk)
{
    ASSERT(chunk);

    ScopeLock(m_mapLock);

    m_voxelChunksMap[chunk->m_id] = chunk;
    m_voxelChunks.Add(chunk);
}

void VoxelChunkCache::RemoveChunk(const uint64_t chunkId)
{
    const auto chunk = TryGetChunk(chunkId);

    if (chunk)
        RemoveChunk(chunk);
}

void VoxelChunkCache::RemoveChunk(const RefPtr<VoxelChunkData>& chunk)
{
    if (chunk == nullptr)
        return;

    ScopeLock(m_mapLock);

    if (chunk->m_cached)
        RemoveChunkFromCache(chunk);

    chunk->DeallocateData();
    m_voxelChunksMap.erase(chunk->m_id);
    m_voxelChunks.Remove(chunk);
}

void VoxelChunkCache::RemoveChunks(const List<RefPtr<VoxelChunkData>>& chunks)
{
    ScopeLock(m_mapLock);

    for (auto& chunk : chunks)
    {
        if (!chunk)
            continue;

        if (chunk->m_cached)
            RemoveChunkFromCache(chunk);

        chunk->DeallocateData();
        m_voxelChunksMap.erase(chunk->m_id);
        m_voxelChunks.Remove(chunk);
    }
}

RefPtr<VoxelChunkData> VoxelChunkCache::GetChunk(const uint64_t chunkId)
{
    ScopeLock(m_mapLock);

    const auto chunk = m_voxelChunksMap[chunkId];

    // Remove chunk from cache if needed
    if (chunk->m_cached)
        RemoveChunkFromCache(chunk);

    return chunk;
}

RefPtr<VoxelChunkData> VoxelChunkCache::TryGetChunk(const uint64_t chunkId)
{
    ScopeLock(m_mapLock);

    if (m_voxelChunksMap.contains(chunkId))
    {
        const auto chunk = m_voxelChunksMap[chunkId];

        // Remove chunk from cache if needed
        if (chunk->m_cached)
            RemoveChunkFromCache(chunk);

        return chunk;
    }

    return nullptr;
}

void VoxelChunkCache::OnChunkReleased(const RefPtr<VoxelChunkData>& chunk)
{
    AddChunkToCache(chunk);
}
