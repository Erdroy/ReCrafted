// ReCrafted (c) 2016-2018 Always Too Late

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
    return GetIntersection(Vector3(float(positionA.x), float(positionA.y), float(positionA.z)),
        Vector3(float(positionB.x), float(positionB.y), float(positionB.z)), densityA, densityB);
}

/**
 * \brief Gets voxel from given data based on given point.
 * \param data The voxel data.
 * \param point The voxel point.
 * \return The voxel from data.
 */
inline Voxel GetVoxel(Voxel* data, const Vector3& point)
{
    return VoxelChunkData::GetVoxel(data, int(point.x), int(point.y), int(point.z));
}

/**
* \brief Gets voxel from given data based on given point.
* \param data The voxel data.
* \param point The voxel point.
* \return The voxel from data.
*/
inline Voxel GetVoxel(Voxel* data, const Int3& point)
{
    return VoxelChunkData::GetVoxel(data, point.x, point.y, point.z);
}

/**
* \brief Gets voxel from given data based on given point.
* \param data The voxel data.
* \param x The voxel point X component.
* \param y The voxel point Y component.
* \param z The voxel point Z component.
* \return The voxel from data.
*/
inline Voxel GetVoxel(Voxel* data, const int x, const int y, const int z)
{
    return VoxelChunkData::GetVoxel(data, x, y, z);
}

#endif // MESHINGHELPERS_H
