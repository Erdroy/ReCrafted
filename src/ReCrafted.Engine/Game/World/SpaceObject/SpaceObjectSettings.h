// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef SPACEOBJECTSETTINGS_H
#define SPACEOBJECTSETTINGS_H

// includes
#include "Core/Math/Vector3.h"

struct SpaceObjectSettings
{
public:
	Vector3 position;
	float minSurfaceHeight;
	float maxBuildHeight;
};

#endif // SPACEOBJECTSETTINGS_H
