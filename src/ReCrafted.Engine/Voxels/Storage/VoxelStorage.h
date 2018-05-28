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

struct Vector3;
struct SpaceObjectSettings;
class VoxelCHM;

class VoxelStorage : IDisposable
{
    friend class SpaceObject;

private:
    SpaceObject* spaceObject = nullptr;
    SpaceObjectSettings settings = {};

private:
    Ref<VoxelCHM> m_chm = nullptr;

    FileStream* m_vxhStream = nullptr;
    VoxelStorageHeader* m_vxh = nullptr;
    spp::sparse_hash_map<uint64_t, Ref<VoxelChunkData>> m_voxelChunks;
    // TODO: move into separate class and make some proper caching
    Lock m_voxelChunksLock = {};

private:
    FORCEINLINE static sbyte sdf_planet_generate(VoxelCHM* chm, const Vector3& origin, const Vector3& position,
                                                 const int lod, const float radius, const float hillsHeight);
    FORCEINLINE void GenerateChunkFromCHM(sbyte** voxelData, const Vector3& position, const int lod);

    void LoadHeader();
    void SaveHeader();

public:
    void Init(SpaceObjectSettings& settings);
    void Dispose() override;

public:
    Ref<VoxelChunkData> CreateChunkData(Vector3& nodePosition, int nodeSize);
    Ref<VoxelChunkData> GetChunkData(Vector3& nodePosition);
    void ReadChunkData(Ref<VoxelChunkData> chunkData);
    void WriteChunkData(Ref<VoxelChunkData> chunkData);
    void FreeChunkData(Ref<VoxelChunkData> chunkData);
};

#endif // VOXELSTORAGE_H
