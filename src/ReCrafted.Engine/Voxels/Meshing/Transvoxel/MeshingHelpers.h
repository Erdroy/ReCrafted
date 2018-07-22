// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef MESHINGHELPERS_H
#define MESHINGHELPERS_H

// includes
#include "Core/Math/Math.h"
#include "Voxels/Utilities/VoxelUtils.h"
#include "Voxels/Storage/VoxelChunkData.h"

/**
* \brief Calculates surface intersection on edge based on two data samples.
* \param positionA The first point.
* \param positionB The second point.
* \param densityA The first point value.
* \param densityB The second point value.
* \return The intersection of the given points.
*/
inline Vector3 GetIntersection(Vector3& positionA, Vector3& positionB, const float densityA, const float densityB)
{
    return positionA + (positionB - positionA) * -densityA  / (densityB - densityA);
}

inline float GetVoxel(sbyte* data, const Vector3& point)
{
    return VOXEL_TO_FLOAT(VoxelChunkData::GetVoxel(data, int(point.x), int(point.y), int(point.z)));
}

inline float GetVoxel(sbyte* data, const Int3& point)
{
    return VOXEL_TO_FLOAT(VoxelChunkData::GetVoxel(data, point.x, point.y, point.z));
}

inline float GetVoxel(sbyte* data, int x, int y, int z)
{
    return VOXEL_TO_FLOAT(VoxelChunkData::GetVoxel(data, x, y, z));
}
#endif // MESHINGHELPERS_H
