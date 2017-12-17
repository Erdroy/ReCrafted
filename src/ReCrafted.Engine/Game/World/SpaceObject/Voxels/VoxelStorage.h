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

public:
    void init(SpaceObjectSettings& settings);

    sbyte getVoxel(Vector3 position);

    void getVoxelRow(Vector3 start, Vector3 end, int lod, sbyte* voxels);
};

#endif // VOXELSTORAGE_H
