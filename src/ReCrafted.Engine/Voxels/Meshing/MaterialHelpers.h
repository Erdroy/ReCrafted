// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef MATERIALHELPERS_H
#define MATERIALHELPERS_H

// includes
#include "ReCrafted.h"
#include <set>

inline VoxelMaterial_t GetVoxelMaterial(const Voxel& voxelA, const Voxel& voxelB)
{
    return GetMinimalVoxel(voxelA, voxelB).material;
}

inline Vector4 EncodeMaterial(const Array<VoxelMaterial_t>& materialSet, const VoxelMaterial_t& material)
{
    var materialId = 0u;
 
    // Get id of this material in this section
    for(var i = 0u; i < materialSet.Count(); i ++)
    {
        if(i == material)
            materialId = i;
    }

    // Encode material
    Vector4 materialA;
    Vector4 materialB;
    Vector4 materialC;
    Vector4 materialD;

    switch(materialId)
    {
    // Material A
    case 0:
        materialA = Vector4(1.0f, 0.0f, 0.0f, 0.0f);
        break;
    case 1:
        materialA = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
        break;
    case 2:
        materialA = Vector4(0.0f, 0.0f, 1.0f, 0.0f);
        break;
    case 3:
        materialA = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
        break;

    // Material B
    case 4:
        materialB = Vector4(1.0f, 0.0f, 0.0f, 0.0f);
        break;
    case 5:
        materialB = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
        break;
    case 6:
        materialB = Vector4(0.0f, 0.0f, 1.0f, 0.0f);
        break;
    case 7:
        materialB = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
        break;

    // Material C
    case 8:
        materialC = Vector4(1.0f, 0.0f, 0.0f, 0.0f);
        break;
    case 9:
        materialC = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
        break;
    case 10:
        materialC = Vector4(0.0f, 0.0f, 1.0f, 0.0f);
        break;
    case 11:
        materialC = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
        break;

    // Material D
    case 12:
        materialD = Vector4(1.0f, 0.0f, 0.0f, 0.0f);
        break;
    case 13:
        materialD = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
        break;
    case 14:
        materialD = Vector4(0.0f, 0.0f, 1.0f, 0.0f);
        break;
    case 15:
        materialD = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
        break;
    }

    return materialA;
}

#endif // MATERIALHELPERS_H
