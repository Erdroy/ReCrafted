#pragma once

#ifndef MESHINGHELPERS_H
#define MESHINGHELPERS_H

// includes
#include "Core/Math/Math.h"

/**
* \brief Calculates surface intersection on edge based on two data samples.
* \param positionA The first point.
* \param positionB The second point.
* \param densityA The first point value.
* \param densityB The second point value.
* \return The intersection of the given points.
*/
inline Vector3 GetIntersection(const Vector3& positionA, const Vector3& positionB, const float densityA, const float densityB)
{
    return positionA + (positionB - positionA) * -densityA  / (densityB - densityA);
}

/**
* \brief Calculates surface intersection on edge based on two data samples.
* \param positionA The first point.
* \param positionB The second point.
* \param densityA The first point value.
* \param densityB The second point value.
* \return The intersection of the given points.
*/
inline Vector3 GetIntersection(const Int3& positionA, const Int3& positionB, const float densityA, const float densityB)
{
    return GetIntersection(Vector3(positionA.x, positionA.y, positionA.z), Vector3(positionB.x, positionB.y, positionB.z), densityA, densityB);
}

inline sbyte GetVoxel(sbyte* data, const Vector3& point)
{
    return (VoxelChunkData::GetVoxel(data, int(point.x), int(point.y), int(point.z)));
}

inline sbyte GetVoxel(sbyte* data, const Int3& point)
{
    return (VoxelChunkData::GetVoxel(data, point.x, point.y, point.z));
}

inline sbyte GetVoxel(sbyte* data, int x, int y, int z)
{
    return (VoxelChunkData::GetVoxel(data, x, y, z));
}

#endif // MESHINGHELPERS_H
