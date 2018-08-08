// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef MATERIALHELPERS_H
#define MATERIALHELPERS_H

// includes
#include "ReCrafted.h"

/**
* \brief Calculates materials from two given voxels.
* \param materialMap The material map pointer.
* \param lod The level of detail.
* \param voxelA The first voxel.
* \param voxelB The second voxel.
* \return The selected material.
*/
inline VertexMaterial_t CalculateMaterials(IVoxelMaterialMap* materialMap, const int lod, const Voxel& voxelA, const Voxel& voxelB)
{
    VertexMaterial_t material = {};

    cvar minimalVoxel = GetMinimalVoxel(voxelA, voxelB);
    cvar matSet = materialMap->GetMaterial(minimalVoxel.material);

    material.Blend = minimalVoxel.blend;

    material.Tny = matSet->MaterialIdsA[0];
    material.Tpy = matSet->MaterialIdsA[1];
    material.Txz = matSet->MaterialIdsA[2];

    material.Uny = matSet->MaterialIdsB[0];
    material.Upy = matSet->MaterialIdsB[1];
    material.Uxz = matSet->MaterialIdsB[2];

    return material;
}

#endif // MATERIALHELPERS_H
