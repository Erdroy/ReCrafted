// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef MATERIALHELPERS_H
#define MATERIALHELPERS_H

// includes
#include "ReCrafted.h"
#include "Core/Math/Math.h"
#include "CommonTables.hpp"

inline VoxelMaterial_t GetVoxelMaterial(const Voxel& voxelA, const Voxel& voxelB)
{
    return GetMinimalVoxel(voxelA, voxelB).material;
}

inline Vector4 EncodeMaterial(const Array<VoxelMaterial_t>& materialSet, const VoxelMaterial_t material, int* materialChannel)
{
    var materialId = -1;
 
    // Get id of this material in this section
    for(var i = 0u; i < materialSet.Count(); i ++)
    {
        if(materialSet[i] == material)
            materialId = static_cast<int>(i);
    }

    ASSERT(materialId != -1);

    // Calculate material channel
    *materialChannel = materialId / 4;

    // Encode material
    return EncodedMaterials[materialId];
}

#endif // MATERIALHELPERS_H
