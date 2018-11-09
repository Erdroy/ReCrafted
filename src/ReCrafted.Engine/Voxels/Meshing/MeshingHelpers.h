// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef MESHINGHELPERS_H
#define MESHINGHELPERS_H

// includes
#include "ReCrafted.h"
#include "Core/Math/Int3.h"

struct VertexInfo
{
public:
    uint vertexId = 0u;

    uint collisionVertexId = 0u;

    Vector3 vertexPosition = {};
    Vector3 vertexNormal = {};
    VoxelMaterial_t voxelMaterial = 0u;

    uint normalUses = 0u;

public:
    VertexInfo() = default;

    VertexInfo(
        const uint vertexId,
        const uint collisionVertexId,
        const Vector3& vertexPosition,
        const VoxelMaterial_t& voxelMaterial) : 
    vertexId(vertexId),
    collisionVertexId(collisionVertexId),
    vertexPosition(vertexPosition),
    vertexNormal({}),
    voxelMaterial(voxelMaterial) { }
};

inline int CalculateVertexId(const Int3& position)
{
    return (position.x + 2) * 3 * (VoxelChunkData::ChunkDataSize * 3) * (VoxelChunkData::ChunkDataSize * 3) +
        (position.y + 2) * 3 * (VoxelChunkData::ChunkDataSize * 3) +
        (position.z + 2) * 3;
}

inline int CalculateEdgeVertexId(const Int3& pos, const int edgeCode)
{
    cvar edge = edgeCode & 0x0F;
    cvar owner = (edgeCode & 0xF0) >> 4;

    cvar diffX = owner % 2;
    cvar diffY = (owner >> 2) % 2;
    cvar diffZ = (owner >> 1) % 2;

    DEBUG_ASSERT((diffX == 0) || (diffX == 1));
    DEBUG_ASSERT((diffY == 0) || (diffY == 1));
    DEBUG_ASSERT((diffZ == 0) || (diffZ == 1));

    return CalculateVertexId(pos - Int3(diffX, diffY, diffZ)) + (edge - 1);
}

/**
 * \brief Calculates interpolant value based on two density values.
* \param densityA The first density value.
* \param densityB The second density value.
 * \return The calculated interpolant.
 */
inline float GetInterpolant(const float densityA, const float densityB)
{
    return -densityA / (densityB - densityA);
}

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
    if (std::abs(densityA) < 0.00001f)
        return positionA;

    if (std::abs(densityB) < 0.00001f)
        return positionB;

    if (std::abs(densityA - densityB) < 0.00001f)
        return positionA;

    return positionA + (positionB - positionA) * GetInterpolant(densityA, densityB);
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
 * \brief Selects the minimal voxel.
 * \param voxelA The first voxel.
 * \param voxelB The second voxel.
 * \return The selected minimal voxel, which value is lower than the other's one.
 */
inline Voxel GetMinimalVoxel(const Voxel& voxelA, const Voxel& voxelB)
{
    // Warning: smaller value (-127) is FILL value
    return voxelA.value < voxelB.value ? voxelA : voxelB;
}

/**
* \brief Selects the minimal voxel.
* \param voxelA The first voxel.
* \param voxelB The second voxel.
* \param voxelC The third voxel.
* \param voxelD The fourth voxel.
* \return The selected minimal voxel, which value is lower than the others.
*/
inline Voxel GetMinimalVoxel(const Voxel& voxelA, const Voxel& voxelB, const Voxel& voxelC, const Voxel& voxelD)
{
    var minimalVoxel = voxelA;

    if (minimalVoxel.value > voxelB.value)
        minimalVoxel = voxelB;

    if (minimalVoxel.value > voxelC.value)
        minimalVoxel = voxelC;

    if (minimalVoxel.value > voxelD.value)
        minimalVoxel = voxelD;

    return minimalVoxel;
}

/**
 * \brief Selects minimal corner from given corner set.
 * Warning: This can be only run for non-trivial corner sets!
 * \param corners The corner set.
 * \return The selected voxel.
 */
inline Voxel GetMinimalCorner(const Voxel* corners)
{
    var minimalCorner = Voxel::Empty;
    var minimalCornerIndex = 0;
    for(var i = 0; i < 8; i ++)
    {
        if (minimalCorner.value > corners[i].value)
        {
            minimalCorner = corners[i];
            minimalCornerIndex = i;
        }
    }

    return corners[minimalCornerIndex];
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
