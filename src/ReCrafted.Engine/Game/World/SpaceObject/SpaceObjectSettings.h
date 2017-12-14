// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef SPACEOBJECTSETTINGS_H
#define SPACEOBJECTSETTINGS_H

// includes
#include "Core/Math/Vector3.h"

struct SpaceObjectSettings
{
public:
    /**
	 * \brief The world-space position of the SpaceObject.
	 */
	Vector3 position;

    /**
	 * \brief The minimal surface height.
	 */
	float minSurfaceHeight;

    /**
	 * \brief The maximal surface height.
	 */
	float maxSurfaceHeight;

    /**
     * \brief The initial virtual-depth of space object octree.
     */
    int rootOctreeDepth = 2;
};

#endif // SPACEOBJECTSETTINGS_H
