// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once


#include <ReCrafted.h>

#include "Common/Lock.h"
#include "Common/List.h"
#include "Voxels/VoxelChunkData.h"

#include <sparsepp/spp.h>

class VoxelChunkCache final
{
    DELETE_COPY_MOVE(VoxelChunkCache);

private:
    const float CacheTime = 20.0f * 1000.0f; // 20 seconds

private:
    Lock m_mapLock = {};
    Lock m_cacheLock = {};

    spp::sparse_hash_map<uint64_t, RefPtr<VoxelChunkData>> m_voxelChunksMap;
    List<RefPtr<VoxelChunkData>> m_voxelChunks = {};
    List<RefPtr<VoxelChunkData>> m_cachedChunks = {};

private:
    void AddChunkToCache(const RefPtr<VoxelChunkData>& chunk);
    void RemoveChunkFromCache(const RefPtr<VoxelChunkData>& chunk);

public:
    VoxelChunkCache() = default;
    ~VoxelChunkCache();

public:
    void Update();

public:
    void AddChunk(const RefPtr<VoxelChunkData>& chunk);
    void RemoveChunk(uint64_t chunkId);
    void RemoveChunk(const RefPtr<VoxelChunkData>& chunk);
    void RemoveChunks(const List<RefPtr<VoxelChunkData>>& chunks);
    RefPtr<VoxelChunkData> GetChunk(uint64_t chunkId);
    RefPtr<VoxelChunkData> TryGetChunk(uint64_t chunkId);

    void OnChunkReleased(const RefPtr<VoxelChunkData>& chunk);
};
