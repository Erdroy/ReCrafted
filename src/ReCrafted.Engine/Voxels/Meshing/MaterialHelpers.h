// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef MATERIALHELPERS_H
#define MATERIALHELPERS_H

// includes
#include "ReCrafted.h"

/**
* \brief Setups material from given voxel.
* \param materialMap The material map instance.
* \param voxel The voxel that contain material information.
* \return The resulting vertex material.
*/
inline VertexMaterial_t SetupMaterial(IVoxelMaterialMap* materialMap, const Voxel& voxel)
{
    cvar matSet = materialMap->GetMaterial(voxel.material);

    VertexMaterial_t material = {};
    material.Blend = voxel.blend;

    material.Tny = matSet->MaterialIdsA[0];
    material.Tpy = matSet->MaterialIdsA[1];
    material.Txz = matSet->MaterialIdsA[2];

    material.Uny = matSet->MaterialIdsB[0];
    material.Upy = matSet->MaterialIdsB[1];
    material.Uxz = matSet->MaterialIdsB[2];

    return material;
}

/**
* \brief Calculates materials from two given voxels.
* \param materialMap The material map pointer.
* \param lod The level of detail.
* \param voxelA The first voxel.
* \param voxelB The second voxel.
* \return The resulting vertex material.
*/
inline VertexMaterial_t CalculateMaterials(IVoxelMaterialMap* materialMap, const int lod, const Voxel& voxelA, const Voxel& voxelB)
{
    cvar minimalVoxel = GetMinimalVoxel(voxelA, voxelB);
    return SetupMaterial(materialMap, minimalVoxel);
}

#endif // MATERIALHELPERS_H
