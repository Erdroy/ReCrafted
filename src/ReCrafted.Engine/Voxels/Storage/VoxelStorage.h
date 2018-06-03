// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef VOXELSTORAGE_H
#define VOXELSTORAGE_H

// includes
#include "ReCrafted.h"

#include "Core/Lock.h"
#include "Core/Streams/FileStream.h"

#include "Voxels/SpaceObject.h"
#include "VoxelChunkData.h"
#include "VoxelStorageHeader.h"

#include <spp.h>

class VoxelStorage : IDisposable
{
    friend class SpaceObject;

private:
    SpaceObject* spaceObject = nullptr;
    SpaceObjectSettings* settings = nullptr;

private:
    FileStream* m_vxhStream = nullptr;
    VoxelStorageHeader* m_vxh = nullptr;
    spp::sparse_hash_map<uint64_t, RefPtr<VoxelChunkData>> m_voxelChunks; // TODO: move into separate class and make some proper caching
    Lock m_voxelChunksLock = {};

private:
    void LoadHeader();
    void SaveHeader();

public:
    void Init(SpaceObjectSettings* settings);
    void Dispose() override;

public:
    RefPtr<VoxelChunkData> CreateChunkData(Vector3& nodePosition, int nodeSize);
    RefPtr<VoxelChunkData> GetChunkData(Vector3& nodePosition);
    void ReadChunkData(RefPtr<VoxelChunkData> chunkData);
    void WriteChunkData(RefPtr<VoxelChunkData> chunkData);
    void FreeChunkData(RefPtr<VoxelChunkData> chunkData);
};

#endif // VOXELSTORAGE_H
