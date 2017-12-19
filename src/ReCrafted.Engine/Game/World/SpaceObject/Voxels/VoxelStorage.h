// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef VOXELSTORAGE_H
#define VOXELSTORAGE_H

// includes
#include "ReCrafted.h"
#include "Game/World/SpaceObject/SpaceObject.h"

struct Vector3;
struct SpaceObjectSettings;

class VoxelStorage
{
    friend class SpaceObject;

private:
    SpaceObject* spaceObject = nullptr;
    SpaceObjectSettings* settings = nullptr;

private:
    sbyte* generateChunkFromCHM(Vector3 position, int lod);

public:
    void init(SpaceObjectSettings& settings);
    void dispose();

    // TODO: apply modifications applyCSG or something

    sbyte getVoxel(Vector3 position);

    /**
     * \brief Gets x (SpaceObjectChunk::ChunkDataSize^3) voxels with all modifications. 
     * Uses CHM cache if needed. Automatically generates data if needed.
     * \param position The voxel chunk position.
     * \param lod The LoD for voxels.
     */
    sbyte* getVoxelChunk(Vector3 position, int lod);
};

#endif // VOXELSTORAGE_H
