#pragma once

#ifndef SPACEOBJECTOCTREE_H
#define SPACEOBJECTOCTREE_H

// includes
#include "Core/Math/Vector3.h"
#include "Core/Math/BoundingBox.h"

class SpaceObjectOctreeNode
{
public:
	static const int MinimumNodeSize = 16;

private:
	SpaceObjectOctreeNode* m_childrenNodes[8] = {};
	bool m_populated = false;

public:
	SpaceObjectOctreeNode(Vector3 position, int size)
	{
		m_position = position;
		m_size = size;
	}

public:
	void populate();
	void depopulate();
	void update();
	void dispose();

public:
	PROPERTY(Vector3, position) = {}; // NOTE: center position
	PROPERTY(int, size) = {};
};

class SpaceObjectOctree
{
public:
	SpaceObjectOctreeNode* m_rootNode = nullptr;

public:
	void init(float objectRadius);
	void update();
	void dispose();

public:
	PROPERTY(Vector3, position) = {}; // NOTE: center position
	PROPERTY(Vector3, size) = {};
	PROPERTY(BoundingBox, bounds) = {};
};

#endif // SPACEOBJECTOCTREE_H
