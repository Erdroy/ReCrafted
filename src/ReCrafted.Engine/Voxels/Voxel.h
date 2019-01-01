// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef VOXEL_H
#define VOXEL_H

// includes
#include "ReCrafted.h"
#include "Utilities/VoxelUtils.h"
#include "Core/Math/Math.h"

/**
 * \brief The voxel value type.
 */
typedef sbyte VoxelValue_t;

/**
* \brief The voxel material type.
* This is the identificator of the voxel material set.
*/
typedef uint16_t VoxelMaterial_t;

/**
* \brief The voxel hardness level.
* This is the hardness level of the voxel material.
*/
typedef sbyte VoxelHardness_t;

/**
* \brief Voxel structure.
*/
ALIGN(4) struct Voxel
{
public:
    /**
    * \brief The voxel value.
    */
    VoxelValue_t value;

    /**
    * \brief The voxel material id.
    * This id can be used to get MaterialSet_t from IVoxelMaterialMap::GetMaterial(...).
    */
    VoxelMaterial_t material;

public:
    /**
     * \brief Creates voxel from value and material.
     * \param value The voxel value.
     * \param material The voxel material.
     * \return The created voxel.
     */
    static Voxel Create(const VoxelValue_t value, const VoxelMaterial_t material)
    {
        return Voxel{ value, material };
    }

    /**
    * \brief Creates voxel from value and material.
    * \param value The voxel float value.
    * \param material The voxel material.
    * \return The created voxel.
    */
    static Voxel Create(const float value, const VoxelMaterial_t material)
    {
        return Create(VOXEL_FROM_FLOAT(value), material);
    }

    /**
    * \brief Creates voxel from value. Material is 0.
    * \param value The voxel value.
    * \return The created voxel.
    */
    static Voxel Create(const VoxelValue_t value)
    {
        return Voxel{ value, 0u };
    }

    /**
    * \brief Creates voxel from float value. Material is 0.
    * \param value The voxel float value.
    * \return The created voxel.
    */
    static Voxel Create(const float value)
    {
        return Voxel{ VOXEL_FROM_FLOAT(value), value < 0 ? 255u : EmptyMaterial };
    }

public:
    bool operator ==(const Voxel& other) const
    {
        return value == other.value && material == other.material;
    }

public:
    /**
     * \brief The material that is being used for empty voxels.
     */
    static const VoxelMaterial_t EmptyMaterial;

    /**
    * \brief The full voxel value (-127).
    */
    static const VoxelValue_t FullValue;

    /**
    * \brief The empty voxel value (127).
    */
    static const VoxelValue_t EmptyValue;

    /**
     * \brief Default voxel with [value=0] and [material=0].
     */
    static const Voxel Default;

    /**
    * \brief Full voxel with [value=FullValue] and [material=0].
    */
    static const Voxel Full;

    /**
    * \brief Empty voxel with [value=EmptyValue] and [material=EmptyMaterial].
    */
    static const Voxel Empty;
};

#endif // VOXEL_H
