// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef SPACEOBJECTSETTINGS_H
#define SPACEOBJECTSETTINGS_H

// includes
#include "Core/Math/Vector3.h"

struct GenerationType
{
public:
    enum _enum
    {
        CubeHeightMap,
        PreGenerated
    };
};

struct SpaceObjectSettings
{
public:
    /**
     * \brief The name, this will be used to find pregens or CHM's.
     */
    const char* name = nullptr;

    /**
	 * \brief The world-space position of the SpaceObject.
	 */
	Vector3 position = {};

    /**
	 * \brief The minimal surface height.
	 */
	float minSurfaceHeight = 300.0f;

    /**
	 * \brief The maximal surface height.
	 */
	float maxSurfaceHeight = 100.0f;

    /**
     * \brief The initial virtual-depth of space object octree.
     */
    int rootOctreeDepth = 2;

    /**
     * \brief The generation type.
     */
    GenerationType::_enum generationType = GenerationType::CubeHeightMap;
};

#endif // SPACEOBJECTSETTINGS_H
