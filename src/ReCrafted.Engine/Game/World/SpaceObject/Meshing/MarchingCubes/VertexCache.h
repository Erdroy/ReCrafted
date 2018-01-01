// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef VERTEXCACHE_H
#define VERTEXCACHE_H

// includes
#include "Core/Math/Vector3.h"
#include "Game/World/SpaceObject/SpaceObjectChunk.h"

struct RegularCellCache {
private:
	const static int CacheWidth = SpaceObjectChunk::ChunkSize + 1;

public:
	struct cell {
		uint8_t caseIndex;
		int verts[4] = {};
	};

	cell& get(int x, int y, int z) {
		return cache[INDEX_3D(x, y, z, CacheWidth)];
	}

	cell& get(const Vector3& p) {
		return get(int(p.x), int(p.y), int(p.z));
	}

private:
	cell cache[CacheWidth * CacheWidth * CacheWidth];
};

struct TransitionCellCache {
private:
	const static int CacheWidth = SpaceObjectChunk::ChunkSize + 1;

public:
	struct cell {
		int verts[10];
	};

	cell& get(int x, int y) {
		return cache[y & 1][x];
	}

private:
	cell cache[2][CacheWidth];
};

#endif // VERTEXCACHE_H
