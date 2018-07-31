// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef MATERIALHELPERS_H
#define MATERIALHELPERS_H

// includes
#include "ReCrafted.h"

inline const Voxel& SelectMajorVoxel(const Voxel& voxelA, const Voxel& voxelB)
{
    // Warning: smaller value (-127) is FILL value
    return voxelA.value <= voxelB.value ? voxelA : voxelB;
}

inline const Voxel& SelectMajorVoxelPlane(const Voxel& voxelA, const Voxel& voxelB, const Voxel& voxelC, const Voxel& voxelD)
{
    return SelectMajorVoxel(
        SelectMajorVoxel(SelectMajorVoxel(voxelA, voxelB), SelectMajorVoxel(voxelB, voxelC)), 
        SelectMajorVoxel(SelectMajorVoxel(voxelC, voxelD), SelectMajorVoxel(voxelA, voxelD))
    );
}

/**
* \brief Selects material from two given voxels.
* \param voxelA The first voxel.
* \param voxelB The second voxel.
* \return The selected material.
*/
inline VoxelMaterial_t SelectMaterial(const Voxel& voxelA, const Voxel& voxelB)
{
    // Warning: smaller value (-127) is FILL value
    return SelectMajorVoxel(voxelA, voxelB).material;
}

inline uint64_t CalculateCellMaterials(const Voxel* corners)
{
    uint8_t materials[6];

    // Select material for -X/+X
    materials[0] = SelectMajorVoxelPlane(corners[0], corners[2], corners[4], corners[6]).material;
    materials[1] = SelectMajorVoxelPlane(corners[1], corners[3], corners[5], corners[7]).material;

    // Select material for -Y/+Y
    materials[2] = SelectMajorVoxelPlane(corners[0], corners[1], corners[2], corners[3]).material;
    materials[3] = SelectMajorVoxelPlane(corners[4], corners[5], corners[6], corners[7]).material;

    // Select material for -Z/+Z
    materials[4] = SelectMajorVoxelPlane(corners[0], corners[1], corners[4], corners[5]).material;
    materials[5] = SelectMajorVoxelPlane(corners[2], corners[3], corners[6], corners[7]).material;
    
#undef SELECT_MAT_PLANE
    return static_cast<uint64_t>(materials[0]) |
        static_cast<uint64_t>(materials[1]) << 8 |
        static_cast<uint64_t>(materials[2]) << 16 |
        static_cast<uint64_t>(materials[3]) << 24 |
        static_cast<uint64_t>(materials[4]) << 32 |
        static_cast<uint64_t>(materials[5]) << 40;
}

#endif // MATERIALHELPERS_H
