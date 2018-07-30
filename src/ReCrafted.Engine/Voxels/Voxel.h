// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef VOXEL_H
#define VOXEL_H

// includes
#include "ReCrafted.h"
#include "Utilities/VoxelUtils.h"
#include "Core/Math/Math.h"

/**
* \brief Voxel structure.
*/
ALIGN(1) struct Voxel
{
public:
    /**
    * \brief The voxel value.
    */
    sbyte value;

    /**
    * \brief The voxel material id.
    */
    uint8_t material;

public:
    /**
     * \brief Creates voxel from value and material.
     * \param value The voxel value.
     * \param material The voxel material.
     * \return The created voxel.
     */
    static Voxel Create(const sbyte value, const uint8_t material)
    {
        return Voxel{ value, material };
    }

    /**
    * \brief Creates voxel from value and material.
    * \param value The voxel float value.
    * \param material The voxel material.
    * \return The created voxel.
    */
    static Voxel Create(const float value, const uint8_t material)
    {
        return Create(VOXEL_FROM_FLOAT(value), material);
    }

    /**
    * \brief Creates voxel from value. Material is 0.
    * \param value The voxel value.
    * \return The created voxel.
    */
    static Voxel Create(const sbyte value)
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
        return Voxel{ VOXEL_FROM_FLOAT(value), 0u };
    }

public:
    bool operator ==(const Voxel& other) const
    {
        return value == other.value && material == other.material;
    }

public:
    /**
    * \brief The full voxel value (-127).
    */
    static const sbyte FullValue;

    /**
    * \brief The empty voxel value (127).
    */
    static const sbyte EmptyValue;

    /**
     * \brief Default voxel with [value=0] and [material=0].
     */
    static const Voxel Default;

    /**
    * \brief Full voxel with [value=FullValue] and [material=0].
    */
    static const Voxel Full;

    /**
    * \brief Empty voxel with [value=EmptyValue] and [material=0].
    */
    static const Voxel Empty;
};

#endif // VOXEL_H
