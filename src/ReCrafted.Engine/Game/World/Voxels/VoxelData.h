// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef VOXELDATA_H
#define VOXELDATA_H

// includes
#include "ReCrafted.h"
#include "Core/Math/Math.h"

class VoxelData
{
public:

public:
	void generate();
	void dispose();
public:
	PROPERTY(Vector3, position) = {};
};

#endif // VOXELDATA_H
