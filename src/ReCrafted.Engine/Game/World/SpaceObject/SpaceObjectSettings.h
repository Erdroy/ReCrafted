// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef SPACEOBJECTSETTINGS_H
#define SPACEOBJECTSETTINGS_H

// includes
#include "Core/Math/Vector3.h"

/**
* \brief SpaceObjectSettings GenerationType enum.
*/
struct GenerationType
{
public:
    enum _enum
    {
        CubeHeightMap,
        PreGenerated
    };
};

/**
 * \brief SpaceObjectSettings structure.
 */
struct SpaceObjectSettings
{
public:
    /**
     * \brief The name of the space object.
     */
    const char* name = nullptr;

    /**
    * \brief The file name (directory), this will be used to find pregens or CHM.
    */
    const char* fileName = nullptr;

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
    * \brief The base height of hills (ovarall terrain).
    */
    float hillsHeight = 100.0f;

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
