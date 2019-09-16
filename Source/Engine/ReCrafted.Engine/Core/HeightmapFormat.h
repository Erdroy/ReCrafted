// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

/// <summary>
///     The heightmap formats enum. 
///     Contains all formats that are supported by Heightmap class implementation.
/// </summary>
enum class HeightmapFormat : byte
{
    Unknown = 0,

    /// <summary>
    ///     Simple, plane-like 8-bit heightmap.
    /// </summary>
    Flat_8bit,

    /// <summary>
    ///     Simple, plane-like 16-bit heightmap.
    /// </summary>
    Flat_16bit,

    /// <summary>
    ///     Cubical 8-bit heightmap.
    ///     This maps 6 faces on a cube, and provides SampleCube function support, 
    ///     that is able to sample the heightmap at any given point on the surface of the cube.
    ///     Also, it makes it possible to map the cube into a sphere and thus, store heightmap information
    ///     for a planet.
    /// </summary>
    Cubical_8bit,

    /// <summary>
    ///     Cubical 16-bit heightmap.
    ///     This maps 6 faces on a cube, and provides SampleCube function support, 
    ///     that is able to sample the heightmap at any given point on the surface of the cube.
    ///     Also, it makes it possible to map the cube into a sphere and thus, store heightmap information
    ///     for a planet.
    /// </summary>
    Cubical_16bit,

    Count
};