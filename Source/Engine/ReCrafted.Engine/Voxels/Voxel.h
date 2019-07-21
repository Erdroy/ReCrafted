// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

/// <summary>
///     The voxel value type.
/// </summary>
typedef sbyte VoxelValue_t;

/// <summary>
///     The voxel material type.
///     This is the identificator of the voxel material set.
/// </summary>
typedef uint16_t VoxelMaterial_t;

/// <summary>
///     The voxel hardness level.
///     This is the hardness level of the voxel material.
/// </summary>
typedef sbyte VoxelHardness_t;

constexpr sbyte VoxelFromFloat(const float value)
{
    return static_cast<sbyte>(Math::Clamp(value * 127.0f, -127.0f, 127.0f));
}

constexpr float VoxelToFloat(const sbyte value)
{
    return float(value) / 127.0f;
}

/// <summary>
///     Voxel structure.
/// </summary>
ALIGN(4) struct Voxel
{
public:
    /// <summary>
    ///     The voxel value.
    /// </summary>
    VoxelValue_t value;

    /// <summary>
    ///     The voxel material id.
    ///     This id can be used to get MaterialSet_t from IVoxelMaterialMap::GetMaterial(...).
    /// </summary>
    VoxelMaterial_t material;

public:
    /// <summary>
    ///     Creates voxel from value and material.
    /// </summary>
    /// <param name="value">The voxel value.</param>
    /// <param name="material">The voxel material.</param>
    /// <returns>The created voxel.</returns>
    static Voxel Create(const VoxelValue_t value, const VoxelMaterial_t material)
    {
        return Voxel{ value, material };
    }

    /// <summary>
    ///     Creates voxel from value and material.
    /// </summary>
    /// <param name="value">The voxel float value.</param>
    /// <param name="material">The voxel material.</param>
    /// <returns>The created voxel.</returns>
    static Voxel Create(const float value, const VoxelMaterial_t material)
    {
        return Create(VoxelFromFloat(value), material);
    }

    /// <summary>
    ///     Creates voxel from value. Material is 0.
    /// </summary>
    /// <param name="value">The voxel value.</param>
    /// <returns>The created voxel.</returns>
    static Voxel Create(const VoxelValue_t value)
    {
        return Voxel{ value, 0u };
    }

    /// <summary>
    ///     Creates voxel from float value. Material is 0.
    /// </summary>
    /// <param name="value">The voxel float value.</param>
    /// <returns>The created voxel.</returns>
    static Voxel Create(const float value)
    {
        return Voxel{ VoxelFromFloat(value), static_cast<VoxelMaterial_t>(value < 0 ? 255u : EmptyMaterial)};
    }

public:
    bool operator ==(const Voxel& other) const
    {
        return value == other.value && material == other.material;
    }

public:
    /// <summary>
    ///     The material that is being used for empty voxels.
    /// </summary>
    static const VoxelMaterial_t EmptyMaterial;

    /// <summary>
    ///     The full voxel value (-127).
    /// </summary>
    static const VoxelValue_t FullValue;

    /// <summary>
    ///     The empty voxel value (127).
    /// </summary>
    static const VoxelValue_t EmptyValue;

    /// <summary>
    ///     Default voxel with [value=0] and [material=0].
    /// </summary>
    static const Voxel Default;

    /// <summary>
    ///     Full voxel with [value=FullValue] and [material=0].
    /// </summary>
    static const Voxel Full;

    /// <summary>
    ///     Empty voxel with [value=EmptyValue] and [material=EmptyMaterial].
    /// </summary>
    static const Voxel Empty;
};