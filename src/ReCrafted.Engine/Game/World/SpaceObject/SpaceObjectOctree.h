// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef SPACEOBJECTOCTREE_H
#define SPACEOBJECTOCTREE_H

// includes
#include "Core/Math/Vector3.h"
#include "Core/Math/BoundingBox.h"
#include "Core/Containers/Array.h"

class SpaceObjectOctree;
class SpaceObjectChunk;
class SpaceObject;

class SpaceObjectOctreeNode
{
	friend class SpaceObjectOctree;

public:
	static const int MinimumNodeSize = 16;

private:
	SpaceObjectOctree* owner = nullptr;
	SpaceObjectOctreeNode* m_childrenNodes[8] = {};
	bool m_populated = false;
	bool m_root = false;

	Ptr<SpaceObjectChunk> m_chunk = nullptr;

private:
	bool hasPopulatedChildren();

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
	void updateViews(Array<Vector3>& views);
	void draw();
	void dispose();

public:
	void onCreate();
	void onDestroy();

	void onPopulate();
	void onDepopulate();

public:
	PROPERTY(Vector3, position) = {}; // NOTE: center position
	PROPERTY(int, size) = {};
};

class SpaceObjectOctree
{
	friend class SpaceObjectOctreeNode;
	friend class SpaceObject;

private:
	SpaceObject* spaceObject = nullptr;

public:
	SpaceObjectOctreeNode* m_rootNode = nullptr;

public:
	void init(float objectRadius);
	void update();
	void updateViews(Array<Vector3>& views);
	void draw();
	void dispose();

public:
	PROPERTY(Vector3, position) = {}; // NOTE: center position
	PROPERTY(Vector3, size) = {};
	PROPERTY(BoundingBox, bounds) = {};
};

#endif // SPACEOBJECTOCTREE_H
