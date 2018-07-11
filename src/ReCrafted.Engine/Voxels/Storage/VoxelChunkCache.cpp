// ReCrafted (c) 2016-2018 Always Too Late

#include "VoxelChunkCache.h"
#include "Common/Profiler/Profiler.h"

void VoxelChunkCache::AddChunkToCache(const RefPtr<VoxelChunkData>& chunk)
{
    ASSERT(chunk);
    ASSERT(!chunk->m_cached);

    ScopeLock(m_cacheLock);

    m_cachedChunks.Add(chunk);

    chunk->m_timeCached = Platform::GetMiliseconds();
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

void VoxelChunkCache::Update()
{
    // Chunks-to-remove queue - NOTE: This function is called only on MAIN-THREAD, 
    // then we can optimize memory usage by using static or member function (Array's memory won't be released)
    static Array<RefPtr<VoxelChunkData>> chunksToRemove = {};

    Profiler::BeginProfile("VoxelChunkCache::Update");

    cvar currentTime = Platform::GetMiliseconds();

    // Iterate all chunk to find any not used chunks
    m_cacheLock.LockNow();
    for (rvar chunk : m_cachedChunks)
    {
        ASSERT(chunk);
        ASSERT(chunk->m_cached);

        if (currentTime - chunk->m_timeCached > cacheTime)
            chunksToRemove.Add(chunk);
    }
    m_cacheLock.UnlockNow();

    // Remove chunks
    RemoveChunks(chunksToRemove);

    // Clear queue
    chunksToRemove.Clear();

    Profiler::EndProfile();
}

void VoxelChunkCache::Dispose()
{
    m_voxelChunksMap.clear();

    // Free all chunks
    for(rvar chunk : m_voxelChunks)
    {
        if (chunk)
            chunk->DeallocateData();
    }

    // Clear chunk array
    m_voxelChunks.Clear();
}

void VoxelChunkCache::AddChunk(const RefPtr<VoxelChunkData>& chunk)
{
    ASSERT(chunk);

    ScopeLock(m_mapLock);

    m_voxelChunksMap[chunk->m_id] = chunk;
    m_voxelChunks.Add(chunk);
}

void VoxelChunkCache::RemoveChunk(uint64_t chunkId)
{
    cvar chunk = TryGetChunk(chunkId);

    if(chunk)
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

void VoxelChunkCache::RemoveChunks(const Array<RefPtr<VoxelChunkData>>& chunks)
{
    ScopeLock(m_mapLock);

    for (rvar chunk : chunks)
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

RefPtr<VoxelChunkData> VoxelChunkCache::GetChunk(uint64_t chunkId)
{
    ScopeLock(m_mapLock);

    cvar chunk = m_voxelChunksMap[chunkId];

    // Remove chunk from cache if needed
    if (chunk->m_cached)
        RemoveChunkFromCache(chunk);

    return chunk;
}

RefPtr<VoxelChunkData> VoxelChunkCache::TryGetChunk(uint64_t chunkId)
{
    ScopeLock(m_mapLock);

    if(m_voxelChunksMap.contains(chunkId))
    {
        cvar chunk = m_voxelChunksMap[chunkId];

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
