// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef SPACEOBJECTCHUNK_H
#define SPACEOBJECTCHUNK_H

// includes
#include "Core/Math/Vector3.h"

interface IVoxelMesher;
class Mesh;
class SpaceObject;
class SpaceObjectOctreeNode;

class SpaceObjectChunk
{
	friend class SpaceObjectManager;

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
	Ptr<Mesh> m_mesh = nullptr;
    bool m_hasVoxels = false;

private:
	uint8_t getLodBorders();

public:
	void init(SpaceObjectOctreeNode* node, SpaceObject* spaceObject);
	void generate(IVoxelMesher* mesher);
	void upload();
	void update();
	void draw();
	void dispose();
};

#endif // SPACEOBJECTCHUNK_H
