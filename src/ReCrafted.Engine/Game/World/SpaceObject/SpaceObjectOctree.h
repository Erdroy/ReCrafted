// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef SPACEOBJECTOCTREE_H
#define SPACEOBJECTOCTREE_H

// includes
#include "Core/Math/Vector3.h"
#include "Core/Math/BoundingBox.h"
#include "Core/Containers/Array.h"

class SpaceObjectOctree;
class SpaceObject;

/**
* \brief Octree structure of SpaceObject.
*/
class SpaceObjectOctree
{
	friend class SpaceObject;
	friend class SpaceObjectManager;
	friend class SpaceObjectOctreeNode;

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
