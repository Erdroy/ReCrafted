// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef MATERIALHELPERS_H
#define MATERIALHELPERS_H

// includes
#include "ReCrafted.h"

inline VoxelMaterial_t GetVoxelMaterial(const Voxel& voxelA, const Voxel& voxelB)
{
    return GetMinimalVoxel(voxelA, voxelB).material;
}

inline Vector4 EncodeMaterial(const VoxelMaterial_t& material)
{
    switch (material)
    {
    case 0:
        return { 1.0f, 0.0f, 0.0f, 0.0f };
    case 1:
        return { 0.0f, 1.0f, 0.0f, 0.0f };
    case 2:
        return { 0.0f, 0.0f, 1.0f, 0.0f };
    case 3:
        return { 0.0f, 0.0f, 0.0f, 1.0f };
    default:
        return { 0.0f, 0.0f, 0.0f, 0.0f };
    }
}

#endif // MATERIALHELPERS_H
