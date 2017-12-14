// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef SPACEOBJECTOCTREE_H
#define SPACEOBJECTOCTREE_H

// includes
#include "SpaceObjectOctreeNode.h"

#include "Core/Math/Vector3.h"
#include "Core/Math/BoundingBox.h"
#include "Core/Containers/Array.h"

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

    int m_rootNodesCount = 1;
    SpaceObjectOctreeNode** m_rootNodes = nullptr;

public:
	void init();
	void update();
	void updateViews(Array<Vector3>& views);
	void draw();
	void dispose();

public:
	PROPERTY(BoundingBox, bounds) = {};
};

#endif // SPACEOBJECTOCTREE_H
