// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef SPACEOBJECTCHUNK_H
#define SPACEOBJECTCHUNK_H

// includes
#include "Core/Math/Vector3.h"

#include "Storage/VoxelChunkData.h"

interface IVoxelMesher;
class Mesh;
class SpaceObject;
class SpaceObjectOctreeNode;

class SpaceObjectChunk
{
	friend class SpaceObjectManager;

private:
	SpaceObject* spaceObject = nullptr;
	SpaceObjectOctreeNode* node = nullptr;

    int m_lod = 0;
    uint64_t m_id = 0u;
    Vector3 m_position = {};
    Ptr<VoxelChunkData> m_chunkData = {};

	Ptr<Mesh> m_mesh = nullptr;
    bool m_hasVoxels = false;

private:
	uint8_t getLodBorders();

public:
	void init(SpaceObjectOctreeNode* node, SpaceObject* spaceObject);
	void generate(IVoxelMesher* mesher);
	void upload();
	void draw();
	void dispose();

public:
    Ptr<VoxelChunkData> getChunkData() const
    {
        return m_chunkData;
    }

public:
    static uint64_t calculateChunkId(const Vector3& position);
};

#endif // SPACEOBJECTCHUNK_H
