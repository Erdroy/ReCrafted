// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef VOXELMATERIALMAP_H
#define VOXELMATERIALMAP_H

// includes
#include "Voxel.h"

typedef uint8_t VoxelTextureId_t; // TODO: Make this value 16-bit, change in code and in all shaders

typedef struct
{
public:
    /**
     * \brief Primary texture ids for current material. [Yn, Yp, XZ]
     */
    VoxelTextureId_t MaterialIdsA[3];

    /**
     * \brief Secondary texture ids for current material. [Yn, Yp, XZ]
     */
    VoxelTextureId_t MaterialIdsB[3];

    uint8_t Reserved[2];

public:
    uint64_t GetHash() const
    {
        return *reinterpret_cast<uint64_t*>((void*)this);
    }

} MaterialSet_t;

inline bool operator==(const MaterialSet_t& lhs, const MaterialSet_t& rhs)
{
    return lhs.GetHash() == rhs.GetHash();
}

class IVoxelMaterialMap
{
public:
    virtual ~IVoxelMaterialMap() = default;

public:
    virtual bool FindMaterial(MaterialSet_t& material, VoxelMaterial_t* voxelMaterial) = 0;
    virtual VoxelMaterial_t AddMaterial(const MaterialSet_t& material) = 0;
    virtual MaterialSet_t* GetMaterial(VoxelMaterial_t material) = 0;
};

#endif // VOXELMATERIALMAP_H
