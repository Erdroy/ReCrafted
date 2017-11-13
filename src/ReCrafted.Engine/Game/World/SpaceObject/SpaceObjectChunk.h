// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef SPACEOBJECTCHUNK_H
#define SPACEOBJECTCHUNK_H

// includes
#include "Core/Math/Vector3.h"

class SpaceObject;
class SpaceObjectOctreeNode;

class SpaceObjectChunk
{
public:
	static const int ChunkSize = 16; // 16 'cubes' on signle axis
	static const int ChunkDataSize = ChunkSize + 1; // 17 corners on signle axis (cubes_count + 1)

private:
	SpaceObject* spaceObject = nullptr;
	SpaceObjectOctreeNode* node = nullptr;

	/**
	 * \brief The normal from the SpaceObject center.
	 */
	Vector3 m_chunkNormal = {};

	/**
	 * \brief Stores voxel hermite data in -127 to 128 range. TODO: VoxelStorage, Cache
	 */
	sbyte* m_voxelData = nullptr;

public:
	void init(SpaceObjectOctreeNode* node, SpaceObject* spaceObject);
	void generate();
	void update();
	void draw();
	void dispose();
};

#endif // SPACEOBJECTCHUNK_H
