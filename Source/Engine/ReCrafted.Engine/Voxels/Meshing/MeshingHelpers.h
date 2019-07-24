// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Voxels/Voxel.h"
#include "Voxels/Storage/VoxelChunkData.h"

struct VertexInfo
{
public:
    uint vertexId = 0u;

    uint collisionVertexId = 0u;

    Vector3 vertexPosition = {};
    Vector3 vertexNormal = {};
    VoxelMaterial_t voxelMaterial = 0u;

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

constexpr int CalculateVertexId(const Vector3i& position)
{
    return (position.x + 2) * 3 * (VoxelChunkData::ChunkDataSize * 3) * (VoxelChunkData::ChunkDataSize * 3) +
        (position.y + 2) * 3 * (VoxelChunkData::ChunkDataSize * 3) +
        (position.z + 2) * 3;
}

constexpr int CalculateEdgeVertexId(const Vector3i& pos, const int edgeCode)
{
    const auto edge = edgeCode & 0x0F;
    const auto owner = (edgeCode & 0xF0) >> 4;

    const auto diffX = owner % 2;
    const auto diffY = (owner >> 2) % 2;
    const auto diffZ = (owner >> 1) % 2;

    DEBUG_ASSERT((diffX == 0) || (diffX == 1));
    DEBUG_ASSERT((diffY == 0) || (diffY == 1));
    DEBUG_ASSERT((diffZ == 0) || (diffZ == 1));

    return CalculateVertexId(pos - Vector3i(diffX, diffY, diffZ)) + (edge - 1);
}

/// <summary>
///      Calculates interpolant value based on two density values.
/// </summary>
/// <param name="densityA">The first density value.</param>
/// <param name="densityB">The second density value.</param>
/// <returns>The calculated interpolant.</returns>
constexpr float GetInterpolant(const float densityA, const float densityB)
{
    return -densityA / (densityB - densityA);
}

/// <summary>
///     Calculates surface intersection on edge based on two data samples.
/// </summary>
/// <param name="positionA">The first point.</param>
/// <param name="positionB">The second point.</param>
/// <param name="densityA">The first point value.</param>
/// <param name="densityB">The second point value.</param>
/// <returns>The intersection of the given points.</returns>
constexpr Vector3 GetIntersection(const Vector3& positionA, const Vector3& positionB, const float densityA, const float densityB)
{
    if (std::abs(densityA) < 0.00001f)
        return positionA;

    if (std::abs(densityB) < 0.00001f)
        return positionB;

    if (std::abs(densityA - densityB) < 0.00001f)
        return positionA;

    return positionA + (positionB - positionA) * GetInterpolant(densityA, densityB);
}

/// <summary>
///     Calculates surface intersection on edge based on two data samples.
/// </summary>
/// <param name="positionA">The first point.</param>
/// <param name="positionB">The second point.</param>
/// <param name="densityA">The first point value.</param>
/// <param name="densityB">The second point value.</param>
/// <returns>The intersection of the given points.</returns>
constexpr Vector3 GetIntersection(const Vector3i& positionA, const Vector3i& positionB, const float densityA, const float densityB)
{
    return GetIntersection(Vector3(float(positionA.x), float(positionA.y), float(positionA.z)),
        Vector3(float(positionB.x), float(positionB.y), float(positionB.z)), densityA, densityB);
}

/// <summary>
///     Selects the minimal voxel.
/// </summary>
/// <param name="voxelA">The first voxel.</param>
/// <param name="voxelB">The second voxel.</param>
/// <returns>The selected minimal voxel, which value is lower than the other's one.</returns>
constexpr Voxel GetMinimalVoxel(const Voxel& voxelA, const Voxel& voxelB)
{
    // Warning: smaller value (-127) is FILL value
    return voxelA.value < voxelB.value ? voxelA : voxelB;
}

/// <summary>
///     Selects the minimal voxel.
/// </summary>
/// <param name="voxelA">The first voxel.</param>
/// <param name="voxelB">The second voxel.</param>
/// <param name="voxelC">The third voxel.</param>
/// <param name="voxelD">The fourth voxel.</param>
/// <returns>The selected minimal voxel, which value is lower than the others.</returns>
constexpr Voxel GetMinimalVoxel(const Voxel& voxelA, const Voxel& voxelB, const Voxel& voxelC, const Voxel& voxelD)
{
    auto minimalVoxel = voxelA;

    if (minimalVoxel.value > voxelB.value)
        minimalVoxel = voxelB;

    if (minimalVoxel.value > voxelC.value)
        minimalVoxel = voxelC;

    if (minimalVoxel.value > voxelD.value)
        minimalVoxel = voxelD;

    return minimalVoxel;
}

/// <summary>
///     Selects minimal corner from given corner set.
/// </summary>
/// <param name="corners">The corner set.</param>
/// <returns>The selected voxel.</returns>
/// <remarks>This can be only run for non-trivial corner sets!</remarks>
inline Voxel GetMinimalCorner(const Voxel* corners)
{
    auto minimalCorner = Voxel::Empty;
    auto minimalCornerIndex = 0;
    for(auto i = 0; i < 8; i ++)
    {
        if (minimalCorner.value > corners[i].value)
        {
            minimalCorner = corners[i];
            minimalCornerIndex = i;
        }
    }

    return corners[minimalCornerIndex];
}

/// <summary>
///     Gets voxel from given data based on given point.
/// </summary>
/// <param name="data">The voxel data.</param>
/// <param name="point">The voxel point.</param>
/// <returns>The voxel from data.</returns>
inline Voxel GetVoxel(Voxel* data, const Vector3& point)
{
    return VoxelChunkData::GetVoxel(data, int(point.x), int(point.y), int(point.z));
}

/// <summary>
///     Gets voxel from given data based on given point.
/// </summary>
/// <param name="data">The voxel data.</param>
/// <param name="point">The voxel point.</param>
/// <returns>The voxel from data.</returns>
inline Voxel GetVoxel(Voxel* data, const Vector3i& point)
{
    return VoxelChunkData::GetVoxel(data, point.x, point.y, point.z);
}

/// <summary>
///     Gets voxel from given data based on given point.
/// </summary>
/// <param name="data">The voxel data.</param>
/// <param name="x">The voxel point X component.</param>
/// <param name="y">The voxel point Y component.</param>
/// <param name="z">The voxel point Z component.</param>
/// <returns>The voxel from data.</returns>
inline Voxel GetVoxel(Voxel* data, const int x, const int y, const int z)
{
    return VoxelChunkData::GetVoxel(data, x, y, z);
}
