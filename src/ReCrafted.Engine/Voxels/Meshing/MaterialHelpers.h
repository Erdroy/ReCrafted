// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef MATERIALHELPERS_H
#define MATERIALHELPERS_H

// includes
#include "Core/Math/Math.h"

/**
* \brief Selects material from two given voxels.
* \param voxelA The first voxel.
* \param voxelB The second voxel.
* \return The selected material.
*/
inline uint8_t SelectMaterial(const Voxel& voxelA, const Voxel& voxelB)
{
    // Warning: smaller value (-127) is FILL value
    return voxelA.value >= voxelB.value ? voxelB.material : voxelA.material;
}

#endif // MATERIALHELPERS_H
