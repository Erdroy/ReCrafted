// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef VOXELSTORAGE_H
#define VOXELSTORAGE_H

// includes
#include "ReCrafted.h"
#include "Core/Lock.h"
#include "Core/Streams/FileStream.h"
#include "Game/World/SpaceObject/SpaceObject.h"

#include "VoxelStorageHeader.h"
#include "VoxelStorageChunkEntry.h"

#include "Core/sparsepp/spp.h"

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
    Ptr<VoxelCHM> m_chm = nullptr;

    Lock m_vxhLock = {};
    FileStream* m_vxhStream = nullptr;
    VoxelStorageHeader* m_vxh = nullptr;
    spp::sparse_hash_map<uint64_t, int> m_voxelMap;

private:
    FORCEINLINE static sbyte sdf_planet_generate(VoxelCHM* chm, const Vector3& origin, const Vector3& position, const int lod, const float radius, const float hillsHeight);
    FORCEINLINE sbyte* generateChunkFromCHM(const Vector3& position, int lod);

    void loadHeader();
    void saveHeader();

public:
    void writeChunk(sbyte* chunkData, const Vector3& position, int lod);

public:
    void init(SpaceObjectSettings& settings);
    void dispose() override;

public:
    /**
     * \brief Gets x (SpaceObjectChunk::ChunkDataSize^3) voxels with all modifications. 
     * Uses CHM cache if needed. Automatically generates data if needed.
     * \param nodePosition The octree node position.
     * \param chunkPosition The voxel chunk position.
     * \param lod The LoD for voxels.
     */
    sbyte* getVoxelChunk(const Vector3& nodePosition, const Vector3& chunkPosition, int lod);
};

#endif // VOXELSTORAGE_H
