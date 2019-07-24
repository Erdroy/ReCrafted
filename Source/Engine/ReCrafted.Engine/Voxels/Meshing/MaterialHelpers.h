// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Common/List.h"
#include "Voxels/Meshing/CommonTables.hpp"
#include "Voxels/Meshing/MeshingHelpers.h"

inline VoxelMaterial_t GetVoxelMaterial(const Voxel& voxelA, const Voxel& voxelB)
{
    return GetMinimalVoxel(voxelA, voxelB).material;
}

inline Vector4 EncodeMaterial(const List<VoxelMaterial_t>& materialSet, const VoxelMaterial_t material, int* materialChannel)
{
    auto materialId = -1;
 
    // Get id of this material in this section
    for(auto i = 0u; i < materialSet.Count(); i ++)
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
