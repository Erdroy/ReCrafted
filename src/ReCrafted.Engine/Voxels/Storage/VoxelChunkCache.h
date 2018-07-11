// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef VOXELCHUNKCACHE_H
#define VOXELCHUNKCACHE_H

// includes
#include "ReCrafted.h"
#include "Core/Lock.h"
#include "Core/Containers/Array.h"

#include "VoxelChunkData.h"

#include <spp.h>

class VoxelChunkCache : IDisposable
{
private:
    const float cacheTime = 20.0f * 1000.0f; // 20 seconds

private:
    Lock m_mapLock = {};
    Lock m_cacheLock = {};

    spp::sparse_hash_map<uint64_t, RefPtr<VoxelChunkData>> m_voxelChunksMap;
    Array<RefPtr<VoxelChunkData>> m_voxelChunks = {};
    Array<RefPtr<VoxelChunkData>> m_cachedChunks = {};

private:
    void AddChunkToCache(const RefPtr<VoxelChunkData>& chunk);
    void RemoveChunkFromCache(const RefPtr<VoxelChunkData>& chunk);

public:
    void Update();
    void Dispose() override;

    void AddChunk(const RefPtr<VoxelChunkData>& chunk);
    void RemoveChunk(uint64_t chunkId);
    void RemoveChunk(const RefPtr<VoxelChunkData>& chunk);
    void RemoveChunks(const Array<RefPtr<VoxelChunkData>>& chunks);
    RefPtr<VoxelChunkData> GetChunk(uint64_t chunkId);
    RefPtr<VoxelChunkData> TryGetChunk(uint64_t chunkId);

    void OnChunkReleased(const RefPtr<VoxelChunkData>& chunk);
};

#endif // VOXELCHUNKCACHE_H
