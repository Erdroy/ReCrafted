// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef VOXELSTORAGE_H
#define VOXELSTORAGE_H

// includes
#include "ReCrafted.h"

#include "Core/Lock.h"
#include "Core/Streams/FileStream.h"
#include "Game/World/SpaceObject/SpaceObject.h"

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
    FORCEINLINE void VoxelStorage::generateChunkFromCHM(sbyte** voxelData, const Vector3& position, const int lod);

    void loadHeader();
    void saveHeader();

public:
    void init(SpaceObjectSettings& settings);
    void dispose() override;

public:
    Ref<VoxelChunkData> createChunkData(Vector3& nodePosition, int nodeSize);
    Ref<VoxelChunkData> getChunkData(Vector3& nodePosition);
    void readChunkData(Ref<VoxelChunkData> chunkData);
    void writeChunkData(Ref<VoxelChunkData> chunkData);
    void freeChunkData(Ref<VoxelChunkData> chunkData);
};

#endif // VOXELSTORAGE_H
