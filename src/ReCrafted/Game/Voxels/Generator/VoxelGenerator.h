// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef VOXELGENERATOR_H
#define VOXELGENERATOR_H

// includes
#include "../../../recraftedPrerequisites.h"
#include "../VoxelChunk.h"

class VoxelGenerator
{
private:

public:
	static void initialize();

	static void beginChunk(int x, int z);

	static voxelid generate(int x, int y, int z);
};

#endif // VOXELGENERATOR_H
